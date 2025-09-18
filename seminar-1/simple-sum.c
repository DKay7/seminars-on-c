// RUN: clang simple-sum.c -o simple-sum

#include <stdio.h>

int compute_sum(int, int); // parameter names are NOT part of decl

int main() {
  int a, b;
  
  // Do we really need next two lines?
  a = 0;
  b = 0;
  
  printf("Enter numbers to sum up:\n");
  scanf("%d %d", &a, &b);

  int sum = compute_sum(a, b);

  printf("Computed: %d + %d = %d\n", a, b, sum);

  // no return from main is OK
}

int compute_sum(int lhs, int rhs) { return lhs + rhs; }

