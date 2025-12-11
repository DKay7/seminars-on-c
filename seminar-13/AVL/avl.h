#ifndef AVL_H
#define AVL_H

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

typedef struct s_avl_node {
    int value;
    struct s_avl_node *left;
    struct s_avl_node *right;
    int height;
} AvlNode;

typedef struct {
    AvlNode *root;
    size_t size;
} AvlTree;

AvlTree *avl_create(void);
void avl_destroy(AvlTree *tree);

size_t avl_depth(const AvlTree *tree);

AvlNode *avl_search(const AvlTree *tree, int value);
bool avl_insert(AvlTree *tree, int value);
bool avl_remove(AvlTree *tree, int value);

void avl_dump(const AvlTree *tree, const char *image_path);

#endif // AVL_H
