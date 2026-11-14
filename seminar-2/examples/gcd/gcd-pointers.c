
#include "gcd.h"
#include <assert.h>

void swap(int *x, int *y) {
  int tmp = *x;
  *x = *y;
  *y = tmp;
}

int gcd(int a, int b) {
  while (a != b) {
    if (a < b)
      swap(&a, &b); // No more need in lots of checks
    a = a - b;
  }
  return a;
}
