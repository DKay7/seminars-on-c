#include <stdio.h>

int main() {
  int num = 0;

  scanf("%d", &num);

  if (num < 0) {
    printf("Error! Num must be in range [0, INT_MAX)");
    return -1;
  }

  int result = 0;

  while (num > 0) {
    int next_digit = num % 10;
    num = num / 10;

    result *= 10;
    result += next_digit;
  }

  printf("%d\n", result);
}
