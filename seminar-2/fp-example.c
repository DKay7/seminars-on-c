// RUN-1: clang fp-example.c -o fp
// RUN-2: clang fp-example.c -DMORE_DIGITS -o fp

#include <assert.h>
#include <math.h>
#include <stdio.h>

enum {
  EQUAL = 0,
  NOT_EQUAL = 1,

};

int compare_float(float a, float b);

int main() {

  float a = 0.1;
  float b = 0.2;
  float sum = a + b;

#ifndef MORE_DIGITS
  printf("%f\n", sum);
#else
  printf("%.10f\n", sum);
#endif

  float correct = 0.3;

  const float EPS = 0.00001;

  // assert(fabs(correct - sum) < EPS);

  if (compare_float(correct, sum) == EQUAL) {
  }
}

int compare_float(float a, float b) {
  const float EPS = 0.0001;

  if (fabs(a - b) < EPS)
    return EQUAL;

  return NOT_EQUAL;
}
