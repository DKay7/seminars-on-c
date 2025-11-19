#define _GNU_SOURCE

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include "file_utils.h"

#define SOFT_ASSERT(cond, msg, action)                                                             \
    do {                                                                                           \
        if (!(cond)) {                                                                             \
            fprintf(stderr, "condition `%s` failed at %s:%d\n", #cond, __FILE__, __LINE__);        \
            perror((msg));                                                                         \
            do {                                                                                   \
                action;                                                                            \
            } while (0);                                                                           \
        }                                                                                          \
    } while (0)

typedef int (*payload_fn_t)(void);

// we use a union to convert between function pointers and data pointers
// since its undefined behavior to cast directly between them
typedef union {
    void *object;
    payload_fn_t function;
} payload_mapping_t;

typedef struct {
    unsigned char *bytes;
    size_t size;
} FunctionImage;

static FunctionImage capture_function(payload_fn_t fn);
static void function_image_destroy(FunctionImage *image);
static int write_payload(const char *path, const FunctionImage *image);
static int replay_payload_from_file(const char *path, int *result_out);
static size_t locate_function_size(payload_fn_t fn);
static const unsigned char *function_bytes(payload_fn_t fn);
static payload_fn_t function_from_address(void *address);
__attribute__((noinline)) static int hello_payload(void);

int main(void) {
    FunctionImage image = capture_function(hello_payload);
    SOFT_ASSERT(image.bytes != NULL, "capture_function failed", return EXIT_FAILURE;);

    const char *payload_path = "hello_payload.bin";
    int rc = write_payload(payload_path, &image);
    SOFT_ASSERT(rc == EXIT_SUCCESS, "write_payload failed", function_image_destroy(&image);
                return EXIT_FAILURE;);

    puts("Executing payload copied from function bytes:");
    int payload_result = 0;
    rc = replay_payload_from_file(payload_path, &payload_result);
    SOFT_ASSERT(rc == EXIT_SUCCESS, "replay_payload_from_file failed",
                function_image_destroy(&image);
                return EXIT_FAILURE;);
    printf("Payload returned %d\n", payload_result);

    function_image_destroy(&image);
    return EXIT_SUCCESS;
}

static FunctionImage capture_function(payload_fn_t fn) {
    FunctionImage image = {.bytes = NULL, .size = 0u};

    size_t code_size = locate_function_size(fn);
    SOFT_ASSERT(code_size > 0u, "locate_function_size failed", return image;);

    image.bytes = calloc(code_size, sizeof(unsigned char));
    SOFT_ASSERT(image.bytes != NULL, "calloc snapshot", return image;);

    const unsigned char *source = function_bytes(fn);
    memcpy(image.bytes, source, code_size);
    image.size = code_size;
    return image;
}

static void function_image_destroy(FunctionImage *image) {
    if (image == NULL) {
        return;
    }

    free(image->bytes);
    image->bytes = NULL;
    image->size = 0u;
}

static int write_payload(const char *path, const FunctionImage *image) {
    SOFT_ASSERT(path != NULL, "payload path is null", return EXIT_FAILURE;);
    SOFT_ASSERT(image != NULL, "image is null", return EXIT_FAILURE;);
    SOFT_ASSERT(image->bytes != NULL, "image bytes null", return EXIT_FAILURE;);
    SOFT_ASSERT(image->size > 0u, "image size is zero", return EXIT_FAILURE;);

    int status = file_write_all(path, image->bytes, image->size);
    SOFT_ASSERT(status == 0, "file_write_all payload", return EXIT_FAILURE;);
    return EXIT_SUCCESS;
}

static int replay_payload_from_file(const char *path, int *result_out) {
    SOFT_ASSERT(path != NULL, "payload path is null", return EXIT_FAILURE;);
    SOFT_ASSERT(result_out != NULL, "result_out null", return EXIT_FAILURE;);

    FileBuffer disk_image = {.data = NULL, .size = 0u};

    int status = file_read_all(path, &disk_image);
    SOFT_ASSERT(status == 0, "file_read_all payload", return EXIT_FAILURE;);
    SOFT_ASSERT(disk_image.size > 0u, "payload file empty", file_buffer_destroy(&disk_image);
                return EXIT_FAILURE;);

    size_t payload_size = disk_image.size;

    void *region = mmap(NULL, payload_size, PROT_READ | PROT_WRITE | PROT_EXEC,
                        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    SOFT_ASSERT(region != MAP_FAILED, "mmap payload", file_buffer_destroy(&disk_image);
                return EXIT_FAILURE;);

    memcpy(region, disk_image.data, payload_size);
    file_buffer_destroy(&disk_image);

    payload_fn_t fn = function_from_address(region);
    *result_out = fn();

    SOFT_ASSERT(munmap(region, payload_size) == 0, "munmap payload", return EXIT_FAILURE;);
    return EXIT_SUCCESS;
}

static size_t locate_function_size(payload_fn_t fn) {
    const unsigned char *bytes = function_bytes(fn);
    SOFT_ASSERT(bytes != NULL, "function bytes null", return 0u;);

    for (size_t idx = 0; idx < 64 * 1024u; ++idx) {
        if (bytes[idx] == 0xC3) {
            return idx + 1u;
        }
    }

    SOFT_ASSERT(0, "ret opcode not found", return 0u;);
    return 0u;
}

static const unsigned char *function_bytes(payload_fn_t fn) {
    payload_mapping_t view = {.function = fn};
    return (const unsigned char *)view.object;
}

static payload_fn_t function_from_address(void *address) {
    payload_mapping_t view = {.object = address};
    return view.function;
}

static int hello_payload(void) {

    // We use a local function pointer to avoid potential issues with direct calls
    // Compiler counts offset for call and that offset may not be preserved when function is copied.
    // So we just use a function pointer to avoid that problem.
    int (*local_printf)(const char *, ...) = printf;
    local_printf("Hello from loaded payload!\n");

    return 42;
}
