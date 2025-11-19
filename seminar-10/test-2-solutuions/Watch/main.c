#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SOFT_ASSERT(cond, msg, ret)                                                                \
    do {                                                                                           \
        if (!(cond)) {                                                                             \
            fprintf(stderr, "condition `" #cond "` failed\n");                                     \
            perror((msg));                                                                         \
            return (ret);                                                                          \
        }                                                                                          \
    } while (0)

char *watch(const char *playlist, int rows, int cols);

#ifdef LOCAL_TESTING
#include <assert.h>

int main(void) {
    char *res = watch("AAAAA\nBBBB\nCCC\n", 5, 3);
    assert(strcmp(res, "CBBAAC BAAC B A") == 0);

    printf("'%s'\n", res);
    free(res);
    return 0;
}
#endif

char *watch(const char *playlist, int rows, int cols) {
    SOFT_ASSERT(playlist != NULL, "playlist is null", NULL);
    SOFT_ASSERT(rows > 0 && cols > 0, "invalid display geometry", NULL);

    size_t total_cells = (size_t)rows * (size_t)cols;

    char *row_major = calloc(total_cells + 1u, sizeof(char));
    SOFT_ASSERT(row_major != NULL, "row_major alloc failed", NULL);

    size_t playlist_offset = 0u;
    size_t buffer_offset = 0u;
    int row_size = 0;

    while (sscanf(playlist + playlist_offset, "%s%n", &row_major[buffer_offset], &row_size) > 0) {
        buffer_offset += (size_t)row_size;
        playlist_offset += (size_t)row_size + 1u; // skip '\n'

        // fill-up to the end of the row
        while (buffer_offset % cols != 0u) {
            row_major[buffer_offset++] = ' ';
        }
    }

    // fill remaining cells with spaces
    while (buffer_offset < total_cells)
        row_major[buffer_offset++] = ' ';

    row_major[total_cells] = '\0';

    char *rotated = calloc(total_cells + 1u, sizeof(char));
    SOFT_ASSERT(rotated != NULL, "rotated alloc failed", (free(row_major), NULL));

    // allocate as 2D arrays for easier indexing
    // just casting pointers, no extra memory used
    // it's hepful since we do not use advanced pointer arithmetic
    char (*source)[cols] = (char (*)[cols])row_major;
    char (*destination)[rows] = (char (*)[rows])rotated;

    // rotate 90 degrees clockwise
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            destination[col][row] = source[rows - 1 - row][col];
        }
    }

    rotated[total_cells] = '\0';
    free(row_major);

    return rotated;
}
