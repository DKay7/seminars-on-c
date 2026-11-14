#include "gcd.h"
#include <assert.h>
#include <stdio.h>

int main() {
  int a = 0, b = 0;

  int num_inputed = scanf("%d %d", &a, &b);

  if (num_inputed != 2 || b == 0) {
    printf("Incorrect input\n");
    return -1;
  }

  int great_common_divisor = gcd(a, b);

  printf("GCD(%d, %d) = %d\n", a, b, great_common_divisor);
}
