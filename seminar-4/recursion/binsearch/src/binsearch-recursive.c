#include "binsearch.h"
#include <assert.h>
#include <stddef.h>

ssize_t binary_search(const int *array, size_t left, size_t right, int target) {
  if (left >= right) {
    return NOT_FOUND;
  }

  size_t mid = left + (right - left) / 2;

  if (array[mid] == target)
    return (int)mid;

  else if (array[mid] < target)
    return binary_search(array, mid + 1, right, target);

  else
    return binary_search(array, left, mid, target);
}
