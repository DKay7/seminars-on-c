#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define CHECK(cond, err, ret)                                                                      \
    do {                                                                                           \
        if (!(cond)) {                                                                             \
            fprintf(stderr, "Condition `%s` failed. Error: %s\n", #cond, err);                     \
            return (ret);                                                                          \
        }                                                                                          \
    } while (0)

// type to store num signs and original index of colleagues
typedef struct {
    int num_signs_req;
    int original_idx;
} colleague_t;

// Our own COMPARATOR!
// It is used by qsort to COMPARE elements of our array
int compare_collegues(const void *lhs, const void *rhs);

void find_and_print_subsequences(colleague_t *order_array, int num_collegues,
                                 int num_signs_required, int current_num_signs);

void print_sequence(int *sequence, int size);

#define MAX_NUM_COLLEGUES 9
#define MAX_NUM_SIGNS 9

int main(void) {
    int num_signs = 0;
    int num_colleagues = 0;

    CHECK(scanf("%d %d ", &num_signs, &num_colleagues) == 2, "input error", EXIT_FAILURE);

    CHECK(num_colleagues > 0, "negative value", EXIT_FAILURE);
    CHECK(num_signs > 0, "negative value", EXIT_FAILURE);

    colleague_t order_array[MAX_NUM_COLLEGUES];

    for (int idx = 0; idx < num_colleagues; ++idx) {

        // we must store original indexes since we will sort array
        order_array[idx].original_idx = idx;
        int ret = scanf("%d", &order_array[idx].num_signs_req);

        CHECK(ret == 1, "array input error", EXIT_FAILURE);
    }

    // Using standart sorting algorithm
    qsort(order_array, num_colleagues, sizeof(order_array[0]), compare_collegues);

    // Now our array is sorted, so we must print all sub-sequences
    // of array that:
    // - start with 0,
    // - go in ascending order
    // - has length exactly of num_signs

    find_and_print_subsequences(order_array, num_colleagues, num_signs, 0);
}

void find_and_print_subsequences(colleague_t *order_array, int num_collegues,
                                 int num_signs_required, int current_num_signs) {

    CHECK(order_array != NULL, "null ptr", (void)0);
    CHECK(num_signs_required > 0, "non-positive value", (void)0);
    CHECK(current_num_signs >= 0, "negative value", (void)0);

    static int current_sequence[MAX_NUM_SIGNS];

    if (current_num_signs == num_signs_required) {
        print_sequence(current_sequence, num_signs_required);
        return;
    }

    for (int coleg_idx = current_num_signs; coleg_idx < num_collegues; ++coleg_idx) {

        // we cant' sign and noone after us can since array is sorted
        if (order_array[coleg_idx].num_signs_req > current_num_signs)
            return;

        // we can sign
        current_sequence[current_num_signs] = order_array[coleg_idx].original_idx;

        // search for others who can sign
        find_and_print_subsequences(order_array, num_collegues, num_signs_required,
                                    current_num_signs + 1);
    }
}

void print_sequence(int *sequence, int size) {
    CHECK(sequence != NULL, "null ptr", (void)0);
    CHECK(size > 0, "negative size", (void)0);

    for (int idx = 0; idx < size; ++idx)
        printf("%d ", sequence[idx]);

    printf("\n");
}

// Our own COMPARATOR!
// It is used by qsort to COMPARE elements of our array
int compare_collegues(const void *lhs, const void *rhs) {
    CHECK(lhs != NULL, "null pointer in comparator", -1);
    CHECK(rhs != NULL, "null pointer in comparator", -1);

    // cast our values to actual type (int) since it easy to work with it, rather
    // than with void*
    colleague_t *left_colleague = (colleague_t *)lhs;
    colleague_t *right_colleague = (colleague_t *)rhs;

    // We must..
    // return any > 0, if LHS >  RHS
    // reutrn 0        if LHS == RHS
    // reutrn any < 0  if LHS <  RHS

    // so we may return just difference between lhs and rhs
    return left_colleague->num_signs_req - right_colleague->num_signs_req;
}
