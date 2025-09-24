#include "array-io.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>

int find_max_in_array(const int *array, size_t size);

int main(void) {
  const size_t array_size = 20;
  int array[array_size];

  input_array(array, array_size);

  int max = find_max_in_array(array, array_size);

  printf("Array's max: %d\n", max);
  print_array(array, array_size);

  return 0;
}

int find_max_in_array(const int *array, size_t size) {
  assert(array != NULL);
  assert(size > 0);

  int max = array[0];

  for (size_t idx = 1; idx < size; idx++) {
    if (array[idx] > max) {
      max = array[idx];
    }
  }
  return max;
}
