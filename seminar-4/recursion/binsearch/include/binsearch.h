#ifndef BINARY_SEARCH_H
#define BINARY_SEARCH_H

#include <sys/types.h>

enum SEARCH_RESULT {
  NOT_FOUND = -1,
};

ssize_t binary_search(const int *array, size_t left, size_t right, int target);

#endif // BINARY_SEARCH_H
