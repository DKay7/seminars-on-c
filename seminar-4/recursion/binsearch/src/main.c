#include "binsearch.h"
#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ============================================

void fill_sorted_array(int *array, size_t size, const int max_element);
double measure_binsearch_time(const int *array, size_t size,
                              int num_repetitions);

// ============================================

#define CHECK_ERROR(condition, msg, ret_val)                                   \
  do {                                                                         \
    if (!(condition)) {                                                        \
      perror(msg);                                                             \
      return ret_val;                                                          \
    }                                                                          \
  } while (0)

// ============================================

int main(int argc, char **argv) {

  srand((unsigned)time(NULL));

  size_t size = 0;

#define ARRAY_SIZE (size_t)1e5;

#ifndef ARRAY_SIZE
  printf("Input array size:\n");
  int read_ok = scanf("%lu", &size);
  CHECK_ERROR(read_ok == 1, "Input failure", EXIT_FAILURE);
#else
  size = ARRAY_SIZE;
#endif

  int *array = (int *)calloc(size, sizeof(*array));

  CHECK_ERROR(array != NULL, "Calloc failure", EXIT_FAILURE);

  const int max_element = size;
  fill_sorted_array(array, size, max_element);

  double elapsed_time = NAN;
  const int num_tests = 100000000;

  elapsed_time = measure_binsearch_time(array, size, num_tests);

  printf("Total time: %lf\n", elapsed_time);

  free(array);

  return EXIT_SUCCESS;
}

// ============================================

static int compare_ints(const void *a, const void *b) {
  int int_a = *(const int *)a;
  int int_b = *(const int *)b;
  return int_a - int_b;
}

// ============================================

void fill_sorted_array(int *array, size_t size, const int max_element) {

  for (size_t i = 0; i < size; ++i)
    array[i] = i;
}

double measure_binsearch_time(const int *array, size_t size,
                              int num_repetitions) {

  clock_t start = clock();

  for (int test_num = 0; test_num < num_repetitions; ++test_num) {
    int target = rand() % size; // since our array[i]
    int result = binary_search(array, 0, size, target);

    assert(result == target); // since our array[i] = i;
  }

  clock_t end = clock();

  return ((double)(end - start)) / CLOCKS_PER_SEC;
}
