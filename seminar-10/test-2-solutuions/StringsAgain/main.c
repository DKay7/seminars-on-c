#include <malloc.h>
#include <stddef.h>
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

typedef enum {
    READ_OK = 0,
    READ_ERR = 1,
} IO_ERR;

typedef struct {
    char *ptr;
    size_t len;
} String;

IO_ERR read_string(String *str);
void string_ctor_empty(String *str, size_t size);
void permute_strings(String *str_lhs, String *str_rhs, String *result);

int main(void) {
    const size_t MAX_SIZE = 1000 + 1 + 1; // Do not forget about +1 byte for \0
                                          // And +1 for possible \n from fgets

    String strings[2] = {};
    string_ctor_empty(&strings[0], MAX_SIZE);
    string_ctor_empty(&strings[1], MAX_SIZE);

    String result[2] = {};
    string_ctor_empty(&result[0], MAX_SIZE);
    string_ctor_empty(&result[1], MAX_SIZE);

    // Won't work on windows:
    // SOFT_ASSERT(scanf("%ms[^\n] %ms[^\n]", &strings[0], &strings[1]) == 1, "input failed",
    //             EXIT_FAILURE);

    read_string(&strings[0]);
    read_string(&strings[1]);

    permute_strings(&strings[0], &strings[1], &result[0]);
    permute_strings(&strings[1], &strings[0], &result[1]);

    int compare_result = strcmp(result[0].ptr, result[1].ptr);

    if (compare_result < 0) {
        printf("%s\n", result[0].ptr);
        return EXIT_SUCCESS;
    }

    printf("%s\n", result[1].ptr);
}

void string_ctor_empty(String *str, size_t size) {
    SOFT_ASSERT(str != NULL, "nullptr passed", (void)0);

    str->ptr = (char *)calloc(size, sizeof(char));
    str->len = size;
}

IO_ERR read_string(String *str) {
    SOFT_ASSERT(str != NULL, "nullptr passed", READ_ERR);

    // reads until new line but not more that `size` bytes
    char *ret = fgets(str->ptr, str->len, stdin);
    SOFT_ASSERT(ret != NULL, "Read failed", READ_ERR);

    str->len = strlen(str->ptr);

    // if \n is in string, replace it
    if (str->ptr[str->len - 1] == '\n') {
        str->ptr[str->len - 1] = '\0';
        str->len -= 1;
    }

    return READ_OK;
}

void permute_strings(String *str_lhs, String *str_rhs, String *result) {
    SOFT_ASSERT(str_lhs != NULL, "nullptr passed", (void)0);
    SOFT_ASSERT(str_rhs != NULL, "nullptr passed", (void)0);
    SOFT_ASSERT(result != NULL, "nullptr passed", (void)0);
    SOFT_ASSERT(str_lhs->len == str_rhs->len, "strings size mismatch", (void)0);

    // this +1 always will provide us with ceiling up
    // 3 / 2 -> 1, (3 + 1) / 2 -> 2: Ok for odd
    // 4 / 2 -> 2, (4 + 1) / 2 -> 2: Ok for even
    size_t middle = (str_lhs->len + 1) / 2;

    size_t len_left_part = middle;
    size_t len_right_path = str_rhs->len - middle;

    memcpy(result->ptr, str_lhs->ptr, len_left_part);
    memcpy(result->ptr + middle, str_rhs->ptr + middle, len_right_path);
}
