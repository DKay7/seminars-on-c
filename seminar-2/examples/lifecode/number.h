
#include <stddef.h>

typedef struct {
  char *digits;
  size_t size;
  signed char sign;
} number_t;

typedef enum {
  ADD = 0,
  SUB = 1,
  MUL = 2,
  DIV = 3,
} operation_t;

void numCtor(number_t *num, signed char *digits_str);
void readDigits(signed char **);

void addNums(number_t *lhs, number_t *rhs, number_t *result);

void dispatchOperation(operation_t op, number_t *lhs, number_t *rhs);
