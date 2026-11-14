#include "qsort.h"
#include <assert.h>
#include <ctype.h>
#include <stddef.h>

void qsort(void *array, size_t n_memb, size_t el_size,
           int (*comparator)(const void *, const void *)) {
  assert(array);
  assert(comparator);

  if (n_memb <= 0) {
    return;
  }

  char *array_pointer = (char *)array;

  unsigned pivot_index = choose_pivot(n_memb);

  unsigned left =
      paritation(array_pointer, n_memb, el_size, pivot_index, comparator);

  if (left > 0) {
    qsort(array_pointer, left, el_size, comparator);
  }

  if (n_memb - left - 1 > 0) {
    qsort(array_pointer + (left + 1) * el_size, n_memb - left - 1, el_size,
          comparator);
  }
}

unsigned paritation(char *array, size_t n_memb, size_t el_size,
                    unsigned pivot_index,
                    int (*comparator)(const void *, const void *)) {
  assert(array);
  assert(comparator);

  void *pivot = array + pivot_index * el_size;
  unsigned left = 0;

  swap(array, pivot, el_size);
  pivot = array;

  for (unsigned i = 1; i < n_memb; i++) {
    if (comparator(array + i * el_size, pivot) < 0) {
      left++;
      swap(array + left * el_size, array + i * el_size, el_size);
    }
  }

  swap(pivot, array + left * el_size, el_size);

  return left;
}

void swap(void *a, void *b, size_t el_size) {
  assert(a);
  assert(b);

  char *a_arr = (char *)a;
  char *b_arr = (char *)b;
  char tmp = 0;

  for (unsigned i = 0; i < el_size; i++) {
    tmp = a_arr[i];
    a_arr[i] = b_arr[i];
    b_arr[i] = tmp;
  }
}

unsigned choose_pivot(size_t n_memb) { return n_memb / 2; }

int compare_letters(char a, char b) {
  a = tolower(a);
  b = tolower(b);

  if (a != b) {
    return (a > b) ? 1 : -1;
  }

  return 0;
}
