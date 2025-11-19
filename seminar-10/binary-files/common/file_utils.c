#include "file_utils.h"

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

#ifndef SSIZE_MAX
#define SSIZE_MAX ((ssize_t)(((size_t)-1) >> 1))
#endif

int file_write_all(const char *path, const void *buffer, size_t size) {
    SOFT_ASSERT(path != NULL, "path is null", errno = EINVAL; return -1;);
    SOFT_ASSERT(size == 0u || buffer != NULL, "buffer is null", errno = EINVAL; return -1;);

    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    SOFT_ASSERT(fd >= 0, "open for write", return -1;);

    size_t total_written = 0u;
    const unsigned char *bytes = (const unsigned char *)buffer;
    while (total_written < size) {
        ssize_t chunk = write(fd, bytes + total_written, size - total_written);
        if (chunk < 0) {
            if (errno == EINTR) {
                continue;
            }
            perror("write chunk");
            close(fd);
            return -1;
        }

        SOFT_ASSERT(chunk > 0, "write returned zero bytes", close(fd); return -1;);
        total_written += (size_t)chunk;
    }

    int status = 0;
    SOFT_ASSERT(close(fd) == 0, "close after write", status = -1;);

    return status;
}

int file_read_all(const char *path, FileBuffer *buffer) {
    SOFT_ASSERT(path != NULL, "path is null", errno = EINVAL; return -1;);
    SOFT_ASSERT(buffer != NULL, "buffer is null", errno = EINVAL; return -1;);

    buffer->data = NULL;
    buffer->size = 0u;

    int fd = open(path, O_RDONLY);
    SOFT_ASSERT(fd >= 0, "open for read", return -1;);

    ssize_t file_size = file_calculate_size_fd(fd);
    SOFT_ASSERT(file_size >= 0, "file_calculate_size_fd failed", close(fd); return -1;);

    if (file_size == 0) {
        SOFT_ASSERT(close(fd) == 0, "close after empty read", (void)0);
        return 0;
    }

    unsigned char *data = calloc((size_t)file_size, sizeof(unsigned char));
    SOFT_ASSERT(data != NULL, "calloc for file buffer", close(fd); return -1;);

    size_t total_read = 0u;
    int status = 0;
    while (total_read < (size_t)file_size) {
        ssize_t chunk = read(fd, data + total_read, (size_t)file_size - total_read);
        if (chunk < 0) {
            if (errno == EINTR) {
                continue;
            }
            perror("read chunk");
            status = -1;
            break;
        }

        SOFT_ASSERT(chunk > 0, "unexpected end of file", status = -1; break;);
        total_read += (size_t)chunk;
    }

    SOFT_ASSERT(close(fd) == 0, "close after read", status = -1;);

    if (status != 0) {
        free(data);
        return -1;
    }

    buffer->data = data;
    buffer->size = (size_t)file_size;
    return 0;
}

void file_buffer_destroy(FileBuffer *buffer) {
    if (buffer == NULL) {
        return;
    }

    free(buffer->data);
    buffer->data = NULL;
    buffer->size = 0u;
}

ssize_t file_calculate_size_fd(int fd) {
    SOFT_ASSERT(fd >= 0, "invalid fd", errno = EBADF; return -1;);

    off_t current = lseek(fd, 0, SEEK_CUR);
    SOFT_ASSERT(current != (off_t)-1, "lseek current", return -1;);

    off_t end = lseek(fd, 0, SEEK_END);
    SOFT_ASSERT(end != (off_t)-1, "lseek end", return -1;);

    SOFT_ASSERT(lseek(fd, current, SEEK_SET) != (off_t)-1, "lseek restore", return -1;);
    SOFT_ASSERT(end <= (off_t)SSIZE_MAX, "file too big", errno = EOVERFLOW; return -1;);

    return (ssize_t)end;
}
