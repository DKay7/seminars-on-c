#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

char luck(int *a);
double compute_team_average_luck(int *data, size_t team_size);
bool compare_double_eq_zero(double num);

#define CHECK(cond, err, ret)                                                                      \
    do {                                                                                           \
        if (!(cond)) {                                                                             \
            fprintf(stderr, "Condition `%s` failed. Error: %s\n", #cond, err);                     \
            return (ret);                                                                          \
        }                                                                                          \
    } while (0)

int main(void) {
    static const int MAX_TEAM_SIZE = 1000;
    int array[MAX_TEAM_SIZE];

    int num = 0;
    int idx = 0;
    while (scanf("%d", &num) == 1) {
        array[idx++] = num;

        if (num == 0)
            break;
    }

    char res = luck(array);
    CHECK(res != -1, "Error in luck function", EXIT_FAILURE);

    printf("Result: %c\n", res);
}

char luck(int *teams) {
    CHECK(teams != NULL, "NUll pointer passed", -1);

    int *team_luck_ptr = teams;

    // First pass is to read size;
    while (*team_luck_ptr != 0) {
        ++team_luck_ptr;
    }

    // compute size as pointers difference
    size_t teams_size = (size_t)(team_luck_ptr - teams);

    // Check that we compute size properly
    CHECK(teams_size % 2 == 0, "Uneven size of two equal-sized teams!", -1);

    size_t one_team_size = teams_size / 2;

    // Now we know team size, so we can compute average luck
    double first_team_avg_luck = compute_team_average_luck(teams, one_team_size);
    double second_team_avg_luck = compute_team_average_luck(teams + one_team_size, one_team_size);

    // first we must check if doubles are equal with some precision!
    if (compare_double_eq_zero(first_team_avg_luck - second_team_avg_luck))
        return 'F';

    if (first_team_avg_luck > second_team_avg_luck)
        return 'Y';

    return 'I';
}

double compute_team_average_luck(int *data, size_t team_size) {
    CHECK(data != NULL, "NUll pointer passed", -1);

    unsigned team_total_luck = 0;

    for (size_t idx = 0; idx < team_size; ++idx) {
        team_total_luck += data[idx];
    }

    return ((double)team_total_luck) / team_size;
}

bool compare_double_eq_zero(double num) {
    const double EPSILON = 1e-5;

    return fabs(num) < EPSILON;
}
