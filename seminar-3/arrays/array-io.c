
#include "array-io.h"
#include <assert.h>
#include <stdio.h>

enum IO_ERR input_array(int *arr, size_t size) {
  assert(arr != NULL);

  printf("Input %lu elements of array", size);

  for (size_t idx = 0; idx < size; idx++) {

    if (scanf("%d", &arr[idx]) != 1) {
      return IO_ERROR;
    }
  }

  return IO_OK;
}

void print_array(const int *arr, size_t n) {
  assert(arr != NULL);

  printf("Array:\n{");

  for (size_t i = 0; i < n; i++)
    printf("%d, ", arr[i]);

  printf("}\n");
}
