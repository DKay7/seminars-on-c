#include <assert.h>
#include <stdio.h>

void divmod(int a, int b, int *div, int *remainder);

int main() {

  int a = 0, b = 0;

  int num_inputed = scanf("%d %d", &a, &b);

  if (num_inputed != 2 || b == 0) {
    printf("Incorrect input\n");
    return -1;
  }

  int div = 0;
  int rem = 0;
  divmod(a, b, &div, &rem);

  printf("%d / %d  = %d [rem. %d]\n", a, b, div, rem);
}

void divmod(int a, int b, int *div, int *remainder) {
  assert(div != NULL);
  assert(remainder != NULL);
  assert(b != 0);

  *div = a / b;
  *remainder = a % b;
}
