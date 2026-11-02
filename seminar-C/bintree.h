#ifndef BINTREE_H
#define BINTREE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#define SOFT_ASSERT(cond, value)                                                                   \
    do {                                                                                           \
        if (!(cond)) {                                                                             \
            fprintf(stderr, "Soft assertion failed: %s at %s:%d\n", #cond, __FILE__, __LINE__);    \
            return (value);                                                                        \
        }                                                                                          \
    } while (0)

typedef struct s_bintree_node {
    int value;
    struct s_bintree_node *left;
    struct s_bintree_node *right;
} BintreeNode;

typedef struct {
    BintreeNode *root;
    size_t size;
} Bintree;

Bintree *bintree_create(void);
void bintree_destroy(Bintree *tree);

size_t bintree_depth(const Bintree *tree);

BintreeNode *bintree_search(const Bintree *tree, int value);
bool bintree_insert(Bintree *tree, int value);
bool bintree_remove(Bintree *tree, int value);

void bintree_dump(const Bintree *tree, const char *image_path);

#endif // BINTREE_H
