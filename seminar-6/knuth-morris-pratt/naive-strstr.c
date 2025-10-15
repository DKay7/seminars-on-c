
#include <stdio.h>
#include <string.h>

const char *strstr_naive(const char *str, const char *pattern) {
    for (const char *str_ptr = str; *str_ptr != '\0'; ++str_ptr) {
        const char *pattern_ptr = pattern;
        const char *str_inner_ptr = str;

        while (*pattern_ptr != '\0' && *str_inner_ptr != '\0' && (*pattern_ptr == *str_inner_ptr)) {
            ++pattern_ptr;
            ++str_inner_ptr;
        }

        if (*pattern_ptr == '\0') {
            printf("found!\n");
            return str_ptr;
        }
    }

    return NULL;
}
