#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double **alloc_ptr_matrix(size_t rows, size_t cols);
void free_ptr_matrix(double **m, size_t rows);
double *alloc_buf_matrix(size_t rows, size_t cols);
void fill_ptr_matrix(double **m, size_t rows, size_t cols);
void fill_buf_matrix(double *m, size_t rows, size_t cols);
double test_ptr_matrix(double **m, size_t rows, size_t cols, int repeats);
double test_buf_matrix(double *m, size_t rows, size_t cols, int repeats);

int main(void) {
    const size_t ROWS = 40;
    const size_t COLS = 7000;
    // const size_t ROWS = 500000;
    // const size_t COLS = 1000;

    const int REPEATS = 50000;

    printf("Benchmark for %zux%zu matrices (%d iterations)\n", ROWS, COLS, REPEATS);

    double **ptr_matrix = alloc_ptr_matrix(ROWS, COLS);
    double *buf_matrix = alloc_buf_matrix(ROWS, COLS);

    if (!ptr_matrix || !buf_matrix) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    fill_ptr_matrix(ptr_matrix, ROWS, COLS);
    fill_buf_matrix(buf_matrix, ROWS, COLS);

    printf("\n=== Timed runs ===\n");
    double t_ptr = test_ptr_matrix(ptr_matrix, ROWS, COLS, REPEATS);
    double t_buf = test_buf_matrix(buf_matrix, ROWS, COLS, REPEATS);

    printf("\nResults:\n");
    printf("Pointer matrix: %.3f s\n", t_ptr);
    printf("Linear  matrix: %.3f s\n", t_buf);

    free_ptr_matrix(ptr_matrix, ROWS);
    free(buf_matrix);
    return 0;
}

#define index2D(row, col, cols) (row) * (cols) + (col)

double **alloc_ptr_matrix(size_t rows, size_t cols) {
    double **m = calloc(rows, sizeof(double *));

    if (!m)
        return NULL;

    for (size_t i = 0; i < rows; i++) {
        m[i] = calloc(cols, sizeof(double));

        if (!m[i]) {
            for (size_t k = 0; k < i; k++)
                free(m[k]);
            free(m);
            return NULL;
        }
    }
    return m;
}

void free_ptr_matrix(double **m, size_t rows) {
    for (size_t i = 0; i < rows; i++)
        free(m[i]);
    free(m);
}

double *alloc_buf_matrix(size_t rows, size_t cols) { return calloc(rows * cols, sizeof(double)); }

void fill_ptr_matrix(double **m, size_t rows, size_t cols) {
    for (size_t i = 0; i < rows; i++)
        for (size_t j = 0; j < cols; j++)
            m[i][j] = (double)(i + j);
}

void fill_buf_matrix(double *m, size_t rows, size_t cols) {
    for (size_t i = 0; i < rows; i++)
        for (size_t j = 0; j < cols; j++)
            m[index2D(i, j, cols)] = (double)(i + j);
}

double test_ptr_matrix(double **m, size_t rows, size_t cols, int repeats) {
    volatile double sum = 0.0;
    clock_t start = clock();

    for (int r = 0; r < repeats; r++) {
        for (size_t i = 0; i < rows; i++)
            for (size_t j = 0; j < cols; j++)
                sum += m[i][j] * 0.5;
    }

    clock_t end = clock();
    printf("Pointer sum: %.2f\n", sum);
    return (double)(end - start) / CLOCKS_PER_SEC;
}

double test_buf_matrix(double *m, size_t rows, size_t cols, int repeats) {
    volatile double sum = 0.0;
    clock_t start = clock();

    for (int r = 0; r < repeats; r++) {
        for (size_t i = 0; i < rows; i++)
            for (size_t j = 0; j < cols; j++)
                sum += m[index2D(i, j, cols)] * 0.5;
    }

    clock_t end = clock();
    printf("Buffer  sum: %.2f\n", sum);
    return (double)(end - start) / CLOCKS_PER_SEC;
}
