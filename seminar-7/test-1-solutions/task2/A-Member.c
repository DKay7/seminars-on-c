#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define CHECK(cond, err, ret)                                                  \
  do {                                                                         \
    if (!(cond)) {                                                             \
      fprintf(stderr, "Condition `%s` failed. Error: %s\n", #cond, err);       \
      return (ret);                                                            \
    }                                                                          \
  } while (0)

void find_and_print_subsequences(int *required_signs, int num_collegues,
                                 int num_signs_required, int current_num_signs);

void print_sequence(int *sequence, int size);

#define MAX_NUM_COLLEGUES 9
#define MAX_NUM_SIGNS 9

int main(void) {
  int num_signs = 0;
  int num_colleagues = 0;

  CHECK(scanf("%d %d ", &num_signs, &num_colleagues) == 2, "input error",
        EXIT_FAILURE);

  CHECK(num_colleagues > 0, "negative value", EXIT_FAILURE);
  CHECK(num_signs > 0, "negative value", EXIT_FAILURE);

  int required_signs[MAX_NUM_COLLEGUES];

  for (int idx = 0; idx < num_colleagues; ++idx) {
    int ret = scanf("%d", &required_signs[idx]);
    CHECK(ret == 1, "array input error", EXIT_FAILURE);
  }

  // We must print all sub-sequences
  // of array that:
  // - start with 0,
  // - each element not less than subseq before it
  // - has length exactly of num_signs

  find_and_print_subsequences(required_signs, num_colleagues, num_signs, 0);
}

void find_and_print_subsequences(int *required_signs, int num_collegues,
                                 int num_signs_required,
                                 int current_num_signs) {

  CHECK(required_signs != NULL, "null ptr", (void)0);
  CHECK(num_signs_required > 0, "non-positive value", (void)0);
  CHECK(current_num_signs >= 0, "negative value", (void)0);

  static int current_sequence[MAX_NUM_SIGNS];
  static int already_signed[MAX_NUM_COLLEGUES];

  if (current_num_signs == num_signs_required) {
    print_sequence(current_sequence, num_signs_required);
    return;
  }

  for (int coleg_idx = 0; coleg_idx < num_collegues;
       ++coleg_idx) {

    // already signed
    if (already_signed[coleg_idx])
      continue;
    
    // we can not sign
    if (required_signs[coleg_idx] > current_num_signs)
      continue;

    // we can sign
    current_sequence[current_num_signs] = coleg_idx;
    already_signed[coleg_idx] = true;

    // search for others who can sign
    find_and_print_subsequences(required_signs, num_collegues,
                                num_signs_required, current_num_signs + 1);

    already_signed[coleg_idx] = false;
  }
}

void print_sequence(int *sequence, int size) {
  CHECK(sequence != NULL, "null ptr", (void)0);
  CHECK(size > 0, "negative size", (void)0);

  for (int idx = 0; idx < size; ++idx)
    printf("%d ", sequence[idx]);

  printf("\n");
}
