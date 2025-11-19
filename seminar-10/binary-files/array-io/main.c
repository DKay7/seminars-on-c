#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

typedef struct {
    float *values;
    size_t length;
} floatBuffer;

static int write_measurements(const char *path, const float *data, size_t length);
static floatBuffer read_measurements(const char *path);
static floatBuffer float_buffer_ctor(size_t length);
static void print_measurements(const floatBuffer *buffer);
static void destroy_buffer(floatBuffer *buffer);

int main(void) {
    const float temperature_readings[] = {18.2, 19.3, 21.0, 20.4, 19.8, 18.9};
    const size_t reading_count = sizeof(temperature_readings) / sizeof(temperature_readings[0]);
    const char *file_path = "measurements.bin";

    int rc = write_measurements(file_path, temperature_readings, reading_count);
    SOFT_ASSERT(rc == EXIT_SUCCESS, "write_measurements failed", return EXIT_FAILURE;);

    floatBuffer loaded = read_measurements(file_path);
    SOFT_ASSERT(loaded.values != NULL, "read_measurements failed", return EXIT_FAILURE;);

    print_measurements(&loaded);
    destroy_buffer(&loaded);

    return EXIT_SUCCESS;
}

static int write_measurements(const char *path, const float *data, size_t length) {
    SOFT_ASSERT(path != NULL, "invalid file path", return EXIT_FAILURE;);
    SOFT_ASSERT(data != NULL, "data pointer is null", return EXIT_FAILURE;);

    const size_t byte_count = length * sizeof(float);
    FileStatus status = file_write_all(path, data, byte_count);
    SOFT_ASSERT(status == FILE_OK, "file_write_all failed", return EXIT_FAILURE;);
    return EXIT_SUCCESS;
}

static floatBuffer read_measurements(const char *path) {
    floatBuffer buffer = float_buffer_ctor(0u);
    SOFT_ASSERT(path != NULL, "invalid file path", return buffer;);

    FileBuffer raw = {.data = NULL, .size = 0u};
    FileStatus status = file_read_all(path, &raw);
    SOFT_ASSERT(status == FILE_OK, "file_read_all failed", return buffer;);

    if (raw.size == 0u) {
        file_buffer_destroy(&raw);
        return buffer;
    }

    SOFT_ASSERT(raw.size % sizeof(float) == 0u, "file size is not aligned",
                file_buffer_destroy(&raw);
                return buffer;);

    destroy_buffer(&buffer);
    buffer = float_buffer_ctor(raw.size / sizeof(float));
    SOFT_ASSERT(buffer.values != NULL, "buffer ctor failed", file_buffer_destroy(&raw);
                return buffer;);

    memcpy(buffer.values, raw.data, raw.size);
    file_buffer_destroy(&raw);
    return buffer;
}

static floatBuffer float_buffer_ctor(size_t length) {
    floatBuffer buffer = {.values = NULL, .length = 0u};
    if (length == 0u) {
        return buffer;
    }

    buffer.values = calloc(length, sizeof(float));
    SOFT_ASSERT(buffer.values != NULL, "calloc buffer ctor failed", return buffer;);
    buffer.length = length;
    return buffer;
}

static void print_measurements(const floatBuffer *buffer) {
    SOFT_ASSERT(buffer != NULL, "buffer is null", return;);
    SOFT_ASSERT(buffer->values != NULL, "buffer values are null", return;);

    puts("Loaded measurements:");
    for (size_t idx = 0; idx < buffer->length; ++idx) {
        printf("  #%zu -> %.2f\n", idx + 1, buffer->values[idx]);
    }
}

static void destroy_buffer(floatBuffer *buffer) {
    SOFT_ASSERT(buffer != NULL, "buffer is null", return;);
    free(buffer->values);
    buffer->values = NULL;
    buffer->length = 0u;
}
