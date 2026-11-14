#include "number.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void readDigits(signed char **out_str) {
  assert(out_str != NULL);

  scanf("%ms[^\n]", out_str);
}

void numCtor(number_t *num, signed char *digits_str) {
  assert(num != NULL);
  assert(digits_str != NULL);

  // TODO: case when digits_str is empty string

  if (digits_str[0] == '-') {
    num->sign = -1;
    digits_str += 1;
  }

  size_t nums_len = strlen(digits_str);
  num->size = nums_len;

  char *tmp = (char *)calloc(num->size, sizeof(char));
  assert(tmp != NULL);

  num->digits = tmp;

  for (size_t idx = 0; idx < nums_len; ++idx) {
    assert(isdigit(digits_str[idx]) != 0);

    num->digits[num->size - idx - 1] = (digits_str[idx] - '0');
  }
}

void addNums(number_t *lhs, number_t *rhs, number_t *result) {
  assert(lhs);
  assert(rhs);
  assert(result);

  // FIXME: workaround for negatives
  assert(lhs->sign > 0);
  assert(rhs->sign > 0);

  size_t max_size = lhs->size > rhs->size ? lhs->size : rhs->size;

  result->size = max_size + 1;
  result->digits = (char *)calloc(result->size, sizeof(char));
  char carry = 0;

  for (size_t idx = 0; idx < max_size; ++idx) {
    char lhs_digit = 0;
    char rhs_digit = 0;

    if (idx < lhs->size)
      lhs_digit = lhs->digits[idx];

    if (idx < rhs->size)
      rhs_digit = rhs->digits[idx];

    char sum = (rhs_digit + lhs_digit) % 10;

    result->digits[idx] = sum + carry;

    carry = (rhs_digit + lhs_digit + carry) / 10;
  }
}

void dispatchOperation(operation_t op, number_t *lhs, number_t *rhs,
                       number_t *result) {
  assert(lhs);
  assert(rhs);
  assert(result);

  switch (op) {
  case ADD: {
    // both positives
    // both negatives
    // different signs

    if (lhs->sign > 0 && rhs->sign > 0) {
      addNums(lhs, rhs, result);
    } else if (lhs->sign < 0 && rhs->sign < 0) {
      lhs->sign = rhs->sign = 1;
      addNums(lhs, rhs, result);
      lhs->sign = rhs->sign = result->sign = -1;
    } else {
      printf("not implemented sorry :(");
    }

    break;
  }

  case SUB:
  case MUL:
  case DIV:
    printf("not implemented sorry :(\n");
  }
}



