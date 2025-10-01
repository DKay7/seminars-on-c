#include <stdarg.h>
#include <stdio.h>

int sum_all(int count, ...) {
  va_list args;
  va_start(args, count);

  int sum = 0;
  for (int i = 0; i < count; ++i) {
    sum += va_arg(args, int);
  }

  va_end(args);
  return sum;
}

int main(void) {
  printf("sum_all(3, 1, 2, 3) = %d\n", sum_all(3, 1, 2, 3));
  printf("sum_all(5, 10, 20, 30, 40, 50) = %d\n",
         sum_all(5, 10, 20, 30, 40, 50));
  printf("sum_all(2, -5, 100) = %d\n", sum_all(2, -5, 100));

  return 0;
}
