// RUN-1: clang fp-example.c -o fp
// RUN-2: clang fp-example.c -DMORE_DIGITS -o fp

#include <assert.h>
#include <math.h>
#include <stdio.h>

int main() {

  float a = 0.001;
  float b = 0.002;
  float sum = a + b;

#ifndef MORE_DIGITS
  printf("%f\n", sum);
#else
  printf("%.10f\n", sum);
#endif
  
  float correct = 0.003;
  const float EPS = 0.00001;

  assert(fabs(correct - 0.003) < EPS);
}

int compare_float(float a, float b) {
  const float EPS = 0.0001;

  return fabs(a - b) < EPS;
}
