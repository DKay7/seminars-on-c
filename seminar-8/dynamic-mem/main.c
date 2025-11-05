
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STACK_OK = 0,
    STACK_NULL_PTR = 1 << 0,
    STACK_OVERFLOW = 1 << 1,
    STACK_UNDERFLOW = 1 << 2,
    STACK_ALLOC_FAIL = 1 << 3,
    STACK_CORRUPTED = 1 << 4
} stack_error_t;

#define SOFT_ASSERT(cond, err_code, stack, ...)                                                    \
    do {                                                                                           \
        if (!(cond)) {                                                                             \
            stack->error |= (err_code);                                                            \
            return __VA_ARGS__;                                                                    \
        }                                                                                          \
    } while (0)

typedef struct {
    int *data;
    size_t size;
    size_t capacity;
    stack_error_t error;
} stack_t;

void stack_ctor(stack_t *stack, size_t init_capacity);
void stack_dtor(stack_t *stack);
void stack_push(stack_t *stack, int value);
int stack_pop(stack_t *stack);
void stack_dump(const stack_t *stack);
void stack_print_errors(const stack_t *stack);
bool stack_has_error(stack_t *stack);

int main() {
    stack_t stack;

    stack_ctor(&stack, 2);

    stack_push(&stack, 10);
    stack_push(&stack, 20);
    stack_push(&stack, 30);

    stack_dump(&stack);

    int val = stack_pop(&stack);
    printf("Pop: %d\n", val);

    val = stack_pop(&stack);
    printf("Pop: %d\n", val);

    val = stack_pop(&stack);
    printf("Pop: %d\n", val);

    val = stack_pop(&stack);

    stack_dump(&stack);
    stack_dtor(&stack);
}

void stack_ctor(stack_t *stack, size_t init_capacity) {
    SOFT_ASSERT(stack != NULL, STACK_NULL_PTR, stack);

    stack->data = (int *)calloc(init_capacity, sizeof(int));

    SOFT_ASSERT(stack->data != NULL, STACK_ALLOC_FAIL, stack);

    stack->size = 0;
    stack->capacity = init_capacity;
}

void stack_dtor(stack_t *stack) {
    SOFT_ASSERT(stack != NULL, STACK_NULL_PTR, stack);

    free(stack->data);

    // poisoning!
    stack->data = NULL;
    stack->size = 0;
    stack->capacity = 0;
}

static void stack_resize(stack_t *stack, size_t new_capacity) {
    SOFT_ASSERT(stack != NULL, STACK_NULL_PTR, stack);

    int *new_data = (int *)realloc(stack->data, new_capacity * sizeof(int));
    SOFT_ASSERT(new_data != NULL, STACK_ALLOC_FAIL, stack);

    stack->data = new_data;
    stack->capacity = new_capacity;
}

void stack_push(stack_t *stack, int value) {
    SOFT_ASSERT(stack != NULL, STACK_NULL_PTR, stack);

    if (stack->size >= stack->capacity) {
        stack_resize(stack, stack->capacity * 2 + 1); // resizing on multiplier

        SOFT_ASSERT(stack_has_error(stack), STACK_CORRUPTED, stack);
    }

    stack->data[stack->size++] = value;
}

int stack_pop(stack_t *stack) {
    SOFT_ASSERT(stack != NULL, STACK_NULL_PTR, stack, 0);

    SOFT_ASSERT(stack->size == 0, STACK_UNDERFLOW, stack, 0);

    int val = stack->data[--stack->size];
    return val;
}

void stack_dump(const stack_t *stack) {
    printf("STACK DUMP\n{\n");

    printf("\tptr = %p\n\tsize = %lu\n\tcapacity = %lu\n", (void *)stack->data, stack->size,
           stack->capacity);

    printf("\tData:\n\t{\n");

    for (size_t i = 0; i < stack->size; i++)
        printf("\t\t[%zu] = %d;\n", i, stack->data[i]);

    printf("\t}\n");

    stack_print_errors(stack);

    printf("}\n");
}

bool stack_has_error(stack_t *stack) { return stack->error == STACK_OK; }

void stack_print_errors(const stack_t *stack) {
    stack_error_t err = stack->error;

    if (err == STACK_OK) {
        printf("No errors.\n");
        return;
    }

    printf("Stack errors:\n");
    if (err & STACK_NULL_PTR)
        printf(" - Null pointer to stack\n");
    if (err & STACK_OVERFLOW)
        printf(" - Overflow (capacity exceeded)\n");
    if (err & STACK_UNDERFLOW)
        printf(" - Underflow (pop from empty stack)\n");
    if (err & STACK_ALLOC_FAIL)
        printf(" - Memory allocation failed\n");
    if (err & STACK_CORRUPTED)
        printf(" - Stack data corrupted\n");
}
