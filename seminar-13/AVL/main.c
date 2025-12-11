#include <stdio.h>
#include <stdlib.h>

#include "avl.h"

static void insert_values(AvlTree *tree, const int *values, size_t count);

int main(void) {
    AvlTree *tree = avl_create();

    const int before_rotation[] = {30, 20};
    insert_values(tree, before_rotation, sizeof(before_rotation) / sizeof(before_rotation[0]));

    printf("Before rotation: size=%zu depth=%zu\n", tree->size, avl_depth(tree));
    avl_dump(tree, "before_rotation.png");

    avl_insert(tree, 10);
    printf("After rotation: size=%zu depth=%zu\n", tree->size, avl_depth(tree));
    avl_dump(tree, "after_rotation.png");

    const int extra_values[] = {25, 27, 50, 60, 55};
    insert_values(tree, extra_values, sizeof(extra_values) / sizeof(extra_values[0]));

    avl_dump(tree, "final.png");

    AvlNode *found = avl_search(tree, 27);
    printf("Search 27: %s\n", found ? "found" : "not found");

    avl_remove(tree, 27);
    printf("After removing 27: size=%zu depth=%zu\n", tree->size, avl_depth(tree));
    avl_dump(tree, "after_remove.png");

    avl_destroy(tree);
    return EXIT_SUCCESS;
}

static void insert_values(AvlTree *tree, const int *values, size_t count) {
    SOFT_ASSERT(tree != NULL, (void)0);

    for (size_t i = 0; i < count; ++i)
        avl_insert(tree, values[i]);
}
