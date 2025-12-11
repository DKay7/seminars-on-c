#include "avl.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static AvlNode *avl_node_create(int value) {
    AvlNode *node = calloc(1, sizeof(*node));
    SOFT_ASSERT(node != NULL, NULL);

    node->value = value;
    node->height = 1;
    return node;
}

AvlTree *avl_create(void) {
    AvlTree *tree = calloc(1, sizeof(*tree));
    SOFT_ASSERT(tree != NULL, NULL);

    tree->root = NULL;
    tree->size = 0;

    return tree;
}

static void avl_node_destroy(AvlNode *node) {
    if (node == NULL) // base case
        return;

    avl_node_destroy(node->left);
    avl_node_destroy(node->right);
    free(node);
}

void avl_destroy(AvlTree *tree) {
    SOFT_ASSERT(tree != NULL, (void)0);

    avl_node_destroy(tree->root);

    tree->root = (AvlNode *)0xDEADBEEF;
    tree->size = (size_t)-1;

    free(tree);
}

static int avl_node_height(const AvlNode *node) {
    return node ? node->height : 0;
}

static void avl_update_height(AvlNode *node) {
    int left_height = avl_node_height(node->left);
    int right_height = avl_node_height(node->right);
    node->height = (left_height > right_height ? left_height : right_height) + 1;
}

static int avl_balance_factor(const AvlNode *node) {
    return avl_node_height(node->left) - avl_node_height(node->right);
}

size_t avl_depth(const AvlTree *tree) {
    SOFT_ASSERT(tree != NULL, 0);

    return (size_t)avl_node_height(tree->root);
}

static AvlNode *avl_rotate_right(AvlNode *y) {
    AvlNode *x = y->left;
    AvlNode *t2 = x->right;

    x->right = y;
    y->left = t2;

    avl_update_height(y);
    avl_update_height(x);

    return x;
}

static AvlNode *avl_rotate_left(AvlNode *x) {
    AvlNode *y = x->right;
    AvlNode *t2 = y->left;

    y->left = x;
    x->right = t2;

    avl_update_height(x);
    avl_update_height(y);

    return y;
}

static AvlNode *avl_rebalance(AvlNode *node) {
    avl_update_height(node);

    int balance = avl_balance_factor(node);

    if (balance > 1) { // left heavy
        if (avl_balance_factor(node->left) < 0)
            node->left = avl_rotate_left(node->left);

        return avl_rotate_right(node);
    }

    if (balance < -1) { // right heavy
        if (avl_balance_factor(node->right) > 0)
            node->right = avl_rotate_right(node->right);

        return avl_rotate_left(node);
    }

    return node;
}

static AvlNode *avl_insert_node(AvlNode *node, int value, bool *inserted) {
    if (node == NULL) {
        *inserted = true;
        return avl_node_create(value);
    }

    if (value == node->value)
        return node;

    if (value < node->value)
        node->left = avl_insert_node(node->left, value, inserted);
    else
        node->right = avl_insert_node(node->right, value, inserted);

    if (!*inserted)
        return node;

    return avl_rebalance(node);
}

AvlNode *avl_search(const AvlTree *tree, int value) {
    SOFT_ASSERT(tree != NULL, NULL);

    AvlNode *node = tree->root;

    while (node != NULL) {
        if (value == node->value)
            return node;

        node = (value < node->value) ? node->left : node->right;
    }

    return NULL;
}

bool avl_insert(AvlTree *tree, int value) {
    SOFT_ASSERT(tree != NULL, false);

    bool inserted = false;
    tree->root = avl_insert_node(tree->root, value, &inserted);

    if (inserted)
        ++tree->size;

    return inserted;
}

static AvlNode *avl_find_min(AvlNode *node) {
    while (node->left != NULL)
        node = node->left;

    return node;
}

static AvlNode *avl_remove_node(AvlNode *node, int value, bool *removed) {
    if (node == NULL)
        return NULL;

    if (value < node->value) {
        node->left = avl_remove_node(node->left, value, removed);
    } else if (value > node->value) {
        node->right = avl_remove_node(node->right, value, removed);
    } else {
        *removed = true;

        if (node->left == NULL || node->right == NULL) {
            AvlNode *child = node->left ? node->left : node->right;
            free(node);
            return child;
        }

        AvlNode *successor = avl_find_min(node->right);
        node->value = successor->value;
        node->right = avl_remove_node(node->right, successor->value, removed);
    }

    if (node == NULL)
        return NULL;

    return avl_rebalance(node);
}

bool avl_remove(AvlTree *tree, int value) {
    SOFT_ASSERT(tree != NULL, false);

    bool removed = false;
    tree->root = avl_remove_node(tree->root, value, &removed);

    if (removed)
        --tree->size;

    return removed;
}

static void avl_dump_node(FILE *dot, const AvlNode *node) {
    if (node == NULL)
        return;

    fprintf(dot,
            "    \"%p\" [label=\"%d\\n(h=%d,b=%d)\", shape=circle, style=filled, "
            "fillcolor=\"#%02x%02x%02x\"];\n",
            (const void *)node, node->value, node->height, avl_balance_factor(node), 0xc0, 0xd8,
            0xff);

    if (node->left) {
        fprintf(dot, "    \"%p\" -> \"%p\" [label=\"L\"];\n", (const void *)node,
                (const void *)node->left);
        avl_dump_node(dot, node->left);
    }

    if (node->right) {
        fprintf(dot, "    \"%p\" -> \"%p\" [label=\"R\"];\n", (const void *)node,
                (const void *)node->right);
        avl_dump_node(dot, node->right);
    }
}

static void avl_compile_dot(const char *dot_path, const char *image_path) {
    SOFT_ASSERT(image_path != NULL, (void)0);
    SOFT_ASSERT(dot_path != NULL, (void)0);

    char command[512] = "";
    int written =
        snprintf(command, sizeof(command), "dot -Tpng \"%s\" -o \"%s\"", dot_path, image_path);
    SOFT_ASSERT(written > 0 && (size_t)written < sizeof(command), (void)0);

    int status = system(command);
    
    if (status != 0)
        fprintf(stderr, "Failed to run '%s' (status=%d)\n", command, status);
}

void avl_dump(const AvlTree *tree, const char *image_path) {
    SOFT_ASSERT(tree != NULL, (void)0);
    SOFT_ASSERT(image_path != NULL, (void)0);

    char dot_path[] = "/tmp/avltree-XXXXXX.dot";
    int fd = mkstemps(dot_path, 4);
    SOFT_ASSERT(fd != -1, (void)0);

    FILE *dot = fdopen(fd, "w");

    if (dot == NULL)
        close(fd);

    SOFT_ASSERT(dot != NULL, (void)0);

    fprintf(dot, "digraph avltree {\n");
    fprintf(dot, "    graph [rankdir=TB];\n");
    fprintf(dot, "    node [fontname=\"Helvetica\"];\n");

    if (tree->root == NULL) {
        fprintf(dot, "    empty [label=\"empty\", shape=box, style=dashed];\n");
    } else {
        avl_dump_node(dot, tree->root);
    }

    fprintf(dot, "}\n");
    fclose(dot);

    avl_compile_dot(dot_path, image_path);
}
