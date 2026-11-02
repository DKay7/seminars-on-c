#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>

typedef struct {
    unsigned char *data;
    size_t size;
} FileBuffer;

typedef enum {
    FILE_OK = 0,
    FILE_ERR_INVALID_ARGUMENT,
    FILE_ERR_OPEN,
    FILE_ERR_WRITE,
    FILE_ERR_READ,
    FILE_ERR_ALLOC,
    FILE_ERR_SIZE
} FileStatus;

FileStatus file_write_all(const char *path, const void *buffer, size_t size);
FileStatus file_read_all(const char *path, FileBuffer *buffer);
void file_buffer_destroy(FileBuffer *buffer);
ssize_t file_calculate_size(FILE *stream);

#endif // FILE_UTILS_H
