#include <stdio.h>
#include <time.h>

static const size_t REPETITIONS = 1000000;
static const int NUM = 54;

unsigned long long factorial_recursive(unsigned int n) {
  if (n <= 1) {
    return 1ULL;
  }
  return n * factorial_recursive(n - 1);
}

unsigned long long factorial_iterative(unsigned int n) {
  unsigned long long result = 1ULL;
  for (unsigned int i = 2; i <= n; ++i) {
    result *= i;
  }
  return result;
}

static double measure_recursive(unsigned int n) {
  clock_t start = clock();
  unsigned long long res = 0;
  for (int i = 0; i < REPETITIONS; ++i) {
    res = factorial_recursive(n);
  }
  clock_t end = clock();
  printf("Recursive factorial(%u) = %llu\n", n, res);
  return (double)(end - start) / CLOCKS_PER_SEC;
}

static double measure_iterative(unsigned int n) {
  clock_t start = clock();
  unsigned long long res = 0;
  for (int i = 0; i < REPETITIONS; ++i) {
    res = factorial_iterative(n);
  }
  clock_t end = clock();
  printf("Iterative factorial(%u) = %llu\n", n, res);
  return (double)(end - start) / CLOCKS_PER_SEC;
}

int main(void) {
  double recursive_time = measure_recursive(NUM);
  double iterative_time = measure_iterative(NUM);

  printf("Execution time (recursive): %.6f sec\n", recursive_time);
  printf("Execution time (iterative): %.6f sec\n", iterative_time);

  return 0;
}
