#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "bintree.h"

static void build_balanced(Bintree *tree, const int *values, size_t left, size_t right) {
    SOFT_ASSERT(tree != NULL, (void)0);

    if (left >= right)
        return;

    size_t mid = left + (right - left) / 2;
    bintree_insert(tree, values[mid]);

    build_balanced(tree, values, left, mid);
    build_balanced(tree, values, mid + 1, right);
}

static void build_unbalanced(Bintree *tree, const int *values, size_t count) {
    SOFT_ASSERT(tree != NULL, (void)0);

    for (size_t i = 0; i < count; ++i)
        bintree_insert(tree, values[i]);
}

static double benchmark_search(const Bintree *tree, const int *keys, size_t key_count,
                               size_t rounds, size_t *found_count) {
    SOFT_ASSERT(tree != NULL, 0.0);

    clock_t start = clock();

    for (size_t round = 0; round < rounds; ++round) {
        for (size_t i = 0; i < key_count; ++i) {
            BintreeNode *node = bintree_search(tree, keys[i]);
            if (found_count != NULL)
                *found_count += (node != NULL);
        }
    }

    clock_t end = clock();
    return (double)(end - start) / CLOCKS_PER_SEC;
}

int main(void) {
    const size_t node_count = 31;
    int values[node_count];
    for (size_t i = 0; i < node_count; ++i)
        values[i] = (int)(i + 1);

    Bintree *balanced = bintree_create();
    Bintree *unbalanced = bintree_create();


    build_balanced(balanced, values, 0, node_count);
    build_unbalanced(unbalanced, values, node_count);

    printf("Balanced:   size=%zu depth=%zu\n", balanced->size, bintree_depth(balanced));
    printf("Unbalanced: size=%zu depth=%zu\n", unbalanced->size, bintree_depth(unbalanced));

    bintree_dump(balanced, "balanced.png");
    bintree_dump(unbalanced, "unbalanced.png");

    const size_t rounds = 10000;
    size_t balanced_hits = 0;
    size_t unbalanced_hits = 0;

    double balanced_time = benchmark_search(balanced, values, node_count, rounds, &balanced_hits);
    double unbalanced_time =
        benchmark_search(unbalanced, values, node_count, rounds, &unbalanced_hits);

    printf("Search timing over %zu rounds * %zu keys:\n", rounds, node_count);
    printf("  Balanced:   %.4f sec (hits=%zu)\n", balanced_time, balanced_hits);
    printf("  Unbalanced: %.4f sec (hits=%zu)\n", unbalanced_time, unbalanced_hits);

    bintree_destroy(balanced);
    bintree_destroy(unbalanced);
    
    return EXIT_SUCCESS;
}
