// RUN: clang -Wall -Wextra -Wpedantic bin-ops.c -o bin-ops

#include <stdint.h>
#include <stdio.h>

uint32_t negate_twos_complement(uint32_t bits);
void print_binary(uint32_t value);

int main(void) {
  uint32_t value = 0;
  unsigned int power = 0;

  printf("Enter a number and a power of two (for example, 173 3): ");
  if (scanf("%u %u", &value, &power) != 2) {
    fputs("Error: expected two non-negative integers.\n", stderr);
    return 1;
  }

  if (power >= 32U) {
    fputs("Error: power must be between 0 and 31.\n", stderr);
    return 1;
  }

  const uint32_t divisor = 1ull << power;
  const uint32_t quotient = value >> power;
  const uint32_t remainder = value & (divisor - 1ull);

  printf("%u / 2^%u = %u\n", value, power, quotient);
  printf("Remainder: %u\n", remainder);

  const uint32_t negative_bits = negate_twos_complement(value);
  const uint32_t restored_bits = negate_twos_complement(negative_bits);

  puts("\n32-bit two's complement:");

  printf("  +%u: ", value);
  print_binary(value);
  printf("  0x%08X\n", value);

  printf("  -%u: ", value);
  print_binary(negative_bits);
  printf("  0x%08X\n", negative_bits);

  printf("  Negate again: %u\n", restored_bits);

  return 0;
}

uint32_t negate_twos_complement(uint32_t bits) {
  return ~bits + 1ull;
}

void print_binary(uint32_t value) {
  for (unsigned int position = 32U; position > 0U; --position) {
    const uint32_t bit = (value >> (position - 1U)) & 1ull;

    putchar(bit != 0U ? '1' : '0');
    if (position > 1U && position % 8U == 1U) {
      putchar(' ');
    }
  }
}
