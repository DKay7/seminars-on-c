
#include <stdio.h>

#ifndef OneginErrno

#define OneginErrno(failed_function, error_text)                                                   \
    OneginErrnoFunc(__FILE__, __LINE__, __PRETTY_FUNCTION__, failed_function, error_text)
#endif

#ifndef RED_COLOR
#define RED_COLOR(text) "\033[91m" text "\033[0m"
#endif

#ifndef CLOSE_FILE
#define CLOSE_FILE(file, func_name, ret)                                                           \
    if (fclose(file) != 0) {                                                                       \
        OneginErrno((func_name), "Error while closing " #file);                                    \
        return ret;                                                                                \
    }
#endif

#ifndef CHECK_FILE_OPENED
#define CHECK_FILE_OPENED(file, func_name, ret)                                                    \
    if (!(file) || ferror(file)) {                                                                 \
        OneginErrno((func_name), "Error while opening " #file);                                    \
        return ret;                                                                                \
    }
#endif

#ifndef CHECK_POINTER
#define CHECK_POINTER(ptr, func_name, ret)                                                         \
    if (!(ptr)) {                                                                                  \
        OneginErrno((func_name), "Error while allocating memory for " #ptr);                       \
        return ret;                                                                                \
    }
#endif

typedef struct String {
    size_t len;
    char *start;
} String;

int StringCtor(String *String, char *data, size_t len);

char *ReadFileToBuffer(FILE *file, int num_symbols);
int CountSymbols(FILE *file);
int CountLines(char *buffer);
int OneginFillPArray(char *buffer, String **ptr_array, String *str_array, unsigned line_num,
                     unsigned num_symbols);
int OneginSavePArrToFile(String **array, FILE *file, unsigned line_num);
int OneginSaveOrigToFile(String *array, FILE *file, unsigned line_num);
void OneginErrnoFunc(const char *file, const int line, const char *current_function,
                     const char *failed_function, const char *error_text);
