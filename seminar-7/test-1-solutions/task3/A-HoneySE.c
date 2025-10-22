#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHECK(cond, err, ret)                                                                      \
    do {                                                                                           \
        if (!(cond)) {                                                                             \
            fprintf(stderr, "Condition `%s` failed. Error: %s\n", #cond, err);                     \
            return (ret);                                                                          \
        }                                                                                          \
    } while (0)

static const int BIT_PER_HIDDEN_NUM = 10;
static const int LOWEST_10_BITS = 0x000003FF; // it is | 00000000 | 00000000 | 00000011 | 11111111 |
                                              //   = 0x     00         00         03         FF

int main(void) {
    char *book_str = NULL;

    scanf("%m[^\n]s", &book_str);
    CHECK(book_str != NULL, "input failure", EXIT_FAILURE);

    char *password_chars = (char *)calloc(strlen(book_str) + 1, sizeof(char));

    uint32_t complex_num = 0;
    int num_password_chars = 0;

    while (scanf("%u", &complex_num) == 1 && complex_num != 0) {

        int count_of_hidden = complex_num >> (3 * BIT_PER_HIDDEN_NUM);

        while (count_of_hidden > 0) {
            int hidden_num = complex_num & LOWEST_10_BITS;
            char hidden_char = book_str[hidden_num];
            password_chars[num_password_chars++] = hidden_char;

            complex_num >>= BIT_PER_HIDDEN_NUM;
            --count_of_hidden;
        }
    }
    // no need in null-terminating since we used calloc
    // password_chars[num_password_chars] = 0;

    printf("%s\n", password_chars);

    free(book_str);
    free(password_chars);
}
