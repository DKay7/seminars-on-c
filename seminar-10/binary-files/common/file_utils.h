#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stddef.h>
#include <sys/types.h>

typedef struct {
    unsigned char *data;
    size_t size;
} FileBuffer;

int file_write_all(const char *path, const void *buffer, size_t size);
int file_read_all(const char *path, FileBuffer *buffer);
void file_buffer_destroy(FileBuffer *buffer);
ssize_t file_calculate_size_fd(int fd);

#endif // FILE_UTILS_H
