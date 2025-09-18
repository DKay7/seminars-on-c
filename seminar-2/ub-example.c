// RUN-1: clang ub-example.c -O0 -o ub
// RUN-2: clang ub-example.c -Ofast -o ub
// RUN-3: clang ub-example.c -DUB_PRINT -Ofast -o ub

#include <limits.h>
#include <stdio.h>

void unreachable_function();

#ifndef UB_PRINT
void unreachable_function() { printf("You would never see this!\n"); }
#endif

int main() {
  unsigned int x = 0;

  while (x >= 0) {
    x = x + 1;
    /*
      Infinite loop without side-effects is UB!
    */
  }
}

#ifdef UB_PRINT
void unreachable_function() { printf("You would never see this!\n"); }
#endif


