#include <assert.h>
#include <stdio.h>

// same as below
//
// enum  OPS {
//   INVALID = 0,
//   ADD = 1,
//   SUB = 2,
//   MUL = 3,
//   NUM_OPS,
// };
// typedef enum OPS ops_t;

typedef enum {
  INVALID = 0,
  ADD = 1,
  SUB = 2,
  MUL = 3,
  NUM_OPS,
} ops_t;


int main(void) {
  ops_t operation = INVALID;

  int a = 0;
  int b = 0;

  scanf("%d %d %d", &operation, &a, &b);
  assert(operation > INVALID);
  assert(operation < NUM_OPS);

  switch (operation) {

  case ADD: {
    // The {} block defines the scope of the local variable result.
    int result = a + b;
    printf("Sum: %d\n", result);
    break;
    }
  case SUB: {
    int result = a - b;
    printf("Difference: %d\n", result);
    break;
  }
  case MUL:
    // block is required only if we want a scope
    printf("Multiplication: %d\n", a * b);
    break;

  default:
    puts("Unknown operation");
    break;
  }

  return 0;
}
