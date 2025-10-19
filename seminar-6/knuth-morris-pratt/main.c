
#include <assert.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const char *strstr_knuth_morris_pratt(const char *str, const char *pattern);
const char *strstr_naive(const char *str, const char *pattern);

typedef const char *pattern_array_t[];
typedef const char *func_names_array_t[];
typedef const char *(*function_array_t[])(const char *, const char *);
char *read_file(const char *filename);
void compare_strstr(function_array_t functions, func_names_array_t func_names, int num_functions,
                    pattern_array_t patterns, int num_patterns, const char *text);

#define COMMA ,

#define SOFT_ASSERT(cond, msg, ret)                                                                \
    do {                                                                                           \
        if (!(cond)) {                                                                             \
            fprintf(stderr, "condition `" #cond "` failed\n");                                     \
            perror((msg));                                                                         \
            return (ret);                                                                          \
        }                                                                                          \
    } while (0)

static const int NUM_REPETITIONS = 30;

int main(int argc, const char **argv) {

    // setlocale(LC_ALL, "ru_RU.UTF-8");

    SOFT_ASSERT(argc == 2, "usage: strstr <path-to-text>", EXIT_FAILURE);

    // clang-format off
    function_array_t functions = {
                                                      &strstr_naive, 
                                                      &strstr_knuth_morris_pratt,
        (const char *(*)(const char *, const char *)) &strstr // cast pointer to strstr to our format
    };
    
    func_names_array_t names = {
        "strstr naive",
        "strstr kmp",
        "strstr stdlib"
    };

    pattern_array_t patterns = {
        "Она шла, шла не спеша",
        "Нет, нет, не то!",
        "Смело, смело, ребята!",
        "салон", "madame", "mon cher", "обман",
        "Кутузов", "Наполеон", "друг", "враг", "гвардия", "бог",
        "есть только две добродетели: деятельность и ум",
        "есть только два источника людских пороков: праздность и суеверие",
        "Нездоровы, брат, бывают только дураки да развратники",
    };
    // clang-format on

    char *text = read_file(argv[1]);
    SOFT_ASSERT(text, "reading file failed", EXIT_FAILURE);

    const int NUM_PATTERNS = sizeof(patterns) / sizeof(*patterns);
    const int NUM_FUNCTIONS = sizeof(functions) / sizeof(*functions);

    compare_strstr(functions, names, NUM_FUNCTIONS, patterns, NUM_PATTERNS, text);

    free((void *)text);

    return 0;
}

void compare_strstr(function_array_t functions, func_names_array_t func_names, int num_functions,
                    pattern_array_t patterns, int num_patterns, const char *text) {
    SOFT_ASSERT(functions, "nullptr passed", (void)0);
    SOFT_ASSERT(func_names, "nullptr passed", (void)0);
    SOFT_ASSERT(patterns, "nullptr passed", (void)0);
    SOFT_ASSERT(text, "nullptr passed", (void)0);
    SOFT_ASSERT(num_functions > 0, "negative size passed", (void)0);
    SOFT_ASSERT(num_patterns > 0, "negative size passed", (void)0);

    for (int func_num = 0; func_num < num_functions; ++func_num) {
        printf("Measuring %16s ...", func_names[func_num]);

        clock_t start = clock();

        for (int num_repetition = 0; num_repetition < NUM_REPETITIONS; ++num_repetition) {
            for (int pattern_num = 0; pattern_num < num_patterns; ++pattern_num) {
                const char *pattern_to_search = patterns[pattern_num];
                const char *res = functions[func_num](text, pattern_to_search);
                if (res != strstr(text, pattern_to_search))
                    abort();
                SOFT_ASSERT(res == strstr(text, pattern_to_search), "NOT CORRECT!",
                            printf("\n%s\n", pattern_to_search) COMMA(void) 0);
            }
        }

        clock_t end = clock();
        double seconds = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf(" = %10.2lf sec.\n", seconds);
    }
}

char *read_file(const char *filename) {

    SOFT_ASSERT(filename, "nullptr passed", NULL);

    FILE *file = fopen(filename, "r");

    SOFT_ASSERT(file, "error opening file", NULL);

    // Determine file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = (char *)calloc(file_size + 1, sizeof(*buffer));
    SOFT_ASSERT(buffer, "calloc failed", NULL);

    size_t bytes_read = fread(buffer, 1, file_size, file);
    SOFT_ASSERT(bytes_read == file_size, "failed to read file", free(buffer) COMMA NULL);

    fclose(file);

    return buffer;
}
