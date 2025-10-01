#include "binsearch.h"
#include <assert.h>
#include <stddef.h>

ssize_t binary_search(const int *array, size_t left, size_t right, int target) {
  assert(left <= right);

  while (left < right) {
    size_t mid = left + (right - left) / 2;

    if (array[mid] == target)
      return (int)mid;

    else if (array[mid] < target)
      left = mid + 1;

    else
      right = mid;
  }

  return NOT_FOUND;
}
