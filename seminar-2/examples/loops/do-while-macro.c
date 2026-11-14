#include <stdio.h>

#define PRINT_PAIR(first, second)                                              \
  do {                                                                         \
    printf("First: %d\n", (first));                                            \
    printf("Second: %d\n", (second));                                          \
  } while (0)

int main(void) {
  for (int enabled = 0; enabled <= 1; ++enabled) {
    if (enabled)
      PRINT_PAIR(10, 20);
    else
      puts("Printing is disabled");
  }

  int next = 5;
  PRINT_PAIR(next++, 42);
  printf("Next: %d\n", next);

  return 0;
}
