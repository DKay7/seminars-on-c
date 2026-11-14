#include "array-io.h"
#include "qsort.h"
#include <assert.h>
#include <stddef.h>

int compare_integers(const void *, const void *);

int main() {

  const size_t array_size = 20;
  int array[array_size];

  input_array(array, array_size);

  qsort(array, array_size, sizeof(array[0]), compare_integers);
  print_array(array, array_size);

  return 0;
}

int compare_integers(const void *lhs, const void *rhs) {
  assert(lhs != NULL);
  assert(rhs != NULL);

  int lhs_casted = *(int *)lhs;
  int rhs_casted = *(int *)rhs;

  return lhs_casted - rhs_casted;
}
