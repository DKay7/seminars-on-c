#include "file_utils.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

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

FileStatus file_write_all(const char *path, const void *buffer, size_t size) {
    SOFT_ASSERT(path != NULL, "path is null", return FILE_ERR_INVALID_ARGUMENT;);
    SOFT_ASSERT(size == 0u || buffer != NULL, "buffer is null", return FILE_ERR_INVALID_ARGUMENT;);

    FILE *stream = fopen(path, "wb");
    SOFT_ASSERT(stream != NULL, "fopen for write", return FILE_ERR_OPEN;);

    unsigned long wrote = fwrite(buffer, 1u, size, stream);
    SOFT_ASSERT(wrote == size, "fwrite payload", fclose(stream); return FILE_ERR_WRITE;);

    fclose(stream);

    return FILE_OK;
}

FileStatus file_read_all(const char *path, FileBuffer *buffer) {
    SOFT_ASSERT(path != NULL, "path is null", return FILE_ERR_INVALID_ARGUMENT;);
    SOFT_ASSERT(buffer != NULL, "buffer is null", return FILE_ERR_INVALID_ARGUMENT;);

    buffer->data = NULL;
    buffer->size = 0u;

    FILE *stream = fopen(path, "rb");
    SOFT_ASSERT(stream != NULL, "fopen for read", return FILE_ERR_OPEN;);

    ssize_t file_size = file_calculate_size(stream);
    SOFT_ASSERT(file_size >= 0, "file_calculate_size failed", fclose(stream);
                return FILE_ERR_SIZE;);

    unsigned char *data = calloc((size_t)file_size, sizeof(unsigned char));
    SOFT_ASSERT(data != NULL, "calloc for file buffer", fclose(stream); return FILE_ERR_ALLOC;);

    unsigned long read = fread(data, 1u, (size_t)file_size, stream);
    SOFT_ASSERT(read == (size_t)file_size, "fread payload", free(data); fclose(stream);
                return FILE_ERR_READ;);

    fclose(stream);

    buffer->data = data;
    buffer->size = (size_t)file_size;
    return FILE_OK;
}

void file_buffer_destroy(FileBuffer *buffer) {
    SOFT_ASSERT(buffer != NULL, "invalid buffer", return;);

    free(buffer->data);
    buffer->data = NULL;
    buffer->size = 0u;
}

ssize_t file_calculate_size(FILE *stream) {
    SOFT_ASSERT(stream != NULL, "invalid stream", return -1;);

    long current = ftell(stream);

    fseek(stream, 0L, SEEK_END);

    long end = ftell(stream);

    fseek(stream, current, SEEK_SET);

    return (ssize_t)end;
}
