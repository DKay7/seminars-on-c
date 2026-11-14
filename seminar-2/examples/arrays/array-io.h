#include <stddef.h>

enum IO_ERR {
  IO_OK = 0,
  IO_ERROR = 1,
};

enum IO_ERR input_array(int *arr, size_t size);
void print_array(const int *arr, size_t n);
