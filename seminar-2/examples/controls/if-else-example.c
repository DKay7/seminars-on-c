#include <stdio.h>

int main(void) {
  int temperature = -5;

  if (temperature > 0) {
    puts("Temperature is above zero");
  } else {
    puts("Temperature is at or below zero");
  }

  if (temperature > 0);
  {
    puts("Bug: this block runs unconditionally!");
  }

  return 0;
}
