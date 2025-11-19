
#include "file-utils.h"
#include <assert.h>
#include <ctype.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int reversed_lexicographic_comparator(const void *string_1, const void *string_2);
int lexicographic_comparator(const void *string_1, const void *string_2);

int main(int argc, char **argv) {

    if (argc != 3) {
        fprintf(stderr, "usage: %s FILE OUTPUT", argv[0]);
        return 0;
    }

    const char *sort_filepath = argv[1];
    const char *save_filepath = argv[2];

    FILE *sort_file = fopen(sort_filepath, "r");
    FILE *save_file = fopen(save_filepath, "w");

    CHECK_FILE_OPENED(sort_file, "main", -1);
    CHECK_FILE_OPENED(save_file, "main", -1);

    size_t num_symbols = CountSymbols(sort_file);
    char *file_buffer = ReadFileToBuffer(sort_file, num_symbols);
    CLOSE_FILE(sort_file, "main", -1);

    size_t num_lines = CountLines(file_buffer);
    String **ptr_array = (String **)calloc(num_lines, sizeof(String *));
    String *str_array = (String *)calloc(num_lines, sizeof(String));

    CHECK_POINTER(ptr_array, "main", -1);
    CHECK_POINTER(str_array, "main", -1);

    OneginFillPArray(file_buffer, ptr_array, str_array, num_lines, num_symbols);

    qsort(ptr_array, num_lines, sizeof(String *), lexicographic_comparator);
    OneginSavePArrToFile(ptr_array, save_file, num_lines);

    qsort(ptr_array, num_lines, sizeof(String *), reversed_lexicographic_comparator);

    OneginSavePArrToFile(ptr_array, save_file, num_lines);
    OneginSaveOrigToFile(str_array, save_file, num_lines);

    CLOSE_FILE(save_file, "main", -1);

    free(file_buffer);
    free(str_array);
    free(ptr_array);

    return 0;
}

int compare_letters(char a, char b) {
    a = tolower(a);
    b = tolower(b);

    if (a != b) {
        return (a > b) ? 1 : -1;
    }

    return 0;
}

int lexicographic_comparator(const void *string_1, const void *string_2) {
    assert(string_1);
    assert(string_2);

    String *str_1 = *((String **)string_1);
    String *str_2 = *((String **)string_2);

    unsigned i = 0;
    unsigned j = 0;

    while (i < str_1->len && j < str_2->len) {
        if (isalpha(str_1->start[i]) && isalpha(str_2->start[j]) &&
            tolower(str_1->start[i]) != tolower(str_2->start[j])) {
            return compare_letters(str_1->start[i], str_2->start[j]);
        }

        else if (!isalpha(str_1->start[i]) && isalpha(str_2->start[j])) {
            i++;
        }

        else if (isalpha(str_1->start[i]) && !isalpha(str_2->start[j])) {
            j++;
        }

        else {
            i++;
            j++;
        }
    }

    if (str_1->len == str_2->len) {
        return 0;
    }

    return (str_1->len > str_2->len) ? 1 : -1;
}

int reversed_lexicographic_comparator(const void *string_1, const void *string_2) {
    assert(string_1);
    assert(string_2);

    String *str_1 = *((String **)string_1);
    String *str_2 = *((String **)string_2);

    int i = str_1->len - 1;
    int j = str_2->len - 1;

    while (i >= 0 && j >= 0) {
        if (isalpha(str_1->start[i]) && isalpha(str_2->start[j]) &&
            tolower(str_1->start[i]) != tolower(str_2->start[j])) {
            return compare_letters(str_1->start[i], str_2->start[j]);
        }

        else if (!isalpha(str_1->start[i]) && isalpha(str_2->start[j])) {
            i--;
        }

        else if (isalpha(str_1->start[i]) && !isalpha(str_2->start[j])) {
            j--;
        }

        else {
            i--;
            j--;
        }
    }

    if (str_1->len == str_2->len) {
        return 0;
    }

    return (str_1->len < str_2->len) ? 1 : -1;
}
