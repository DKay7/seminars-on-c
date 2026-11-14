#include <stdio.h>

int main(void) {
  int i = 3;
  for (; i > 0; --i) {
    printf("Countdown: %d\n", i);
  }

  for (int value = 1; value < 10;) {
    printf("Power of two: %d\n", value);
    value *= 2;
  }

  i = 0;
  for (;;) {
    if (i == 3) {
      break;
    }
    printf("Explicit break: %d\n", i++);
  }

  for (int left = 0, right = 3; left < right; ++left, --right) {
    printf("Pair: %d, %d\n", left, right);
  }

  const char text[] = "C loops";
  char ch = 0;
  for (const char *cursor = text; (ch = *cursor) != '\0'; ++cursor) {
    putchar(ch);
  }
  putchar('\n');

  // Advance the index on every iteration so that the loop terminates.
  i = 0;

  while ((ch = text[i]) != '\0')
  {
    putchar(ch);
    ++i;
  }
  putchar('\n');

  // A precondition loop may execute zero times.
  int remaining = 0;
  while (remaining > 0) {
    puts("This while body is skipped");
    --remaining;
  }
  printf("while: remaining = %d\n", remaining);

  // A postcondition loop executes once even if the condition starts false.
  int attempts = 0;
  do {
    printf("do-while: attempt %d\n", ++attempts);
  } while (attempts < 0);

  // Processing digits with do-while handles zero as a one-digit number.
  int number = 0;
  int digits = 0;
  do {
    ++digits;
    number /= 10;
  } while (number != 0);
  printf("do-while: zero has %d digit\n", digits);

  return 0;
}
