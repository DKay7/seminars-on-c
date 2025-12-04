#include "bintree.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static BintreeNode *bintree_node_create(int value) {
    BintreeNode *node = calloc(1, sizeof(*node));
    SOFT_ASSERT(node != NULL, NULL);

    node->value = value;
    return node;
}

Bintree *bintree_create(void) {
    Bintree *tree = calloc(1, sizeof(*tree));
    SOFT_ASSERT(tree != NULL, NULL);

    tree->root = NULL;
    tree->size = 0;

    return tree;
}

static void bintree_node_destroy(BintreeNode *node) {
    if (node == NULL) // base case
        return;

    bintree_node_destroy(node->left);
    bintree_node_destroy(node->right);
    free(node);
}

void bintree_destroy(Bintree *tree) {
    SOFT_ASSERT(tree != NULL, (void)0);

    bintree_node_destroy(tree->root);

    tree->root = (BintreeNode *)0xDEADBEEF;
    tree->size = -1;

    free(tree);
}

static size_t bintree_node_depth(const BintreeNode *node) {
    if (node == NULL)
        return 0;

    size_t left_depth = bintree_node_depth(node->left);
    size_t right_depth = bintree_node_depth(node->right);

    return (left_depth > right_depth ? left_depth : right_depth) + 1;
}

size_t bintree_depth(const Bintree *tree) {
    SOFT_ASSERT(tree != NULL, 0);

    return bintree_node_depth(tree->root);
}

BintreeNode *bintree_search(const Bintree *tree, int value) {
    SOFT_ASSERT(tree != NULL, NULL);

    BintreeNode *node = tree->root;

    while (node != NULL) {
        if (value == node->value)
            return node;

        node = (value < node->value) ? node->left : node->right;
    }

    return NULL;
}

bool bintree_insert(Bintree *tree, int value) {
    SOFT_ASSERT(tree != NULL, false);

    BintreeNode **link = &tree->root;

    while (*link != NULL) {
        if (value == (*link)->value)
            return false;

        link = (value < (*link)->value) ? &(*link)->left : &(*link)->right;
    }

    BintreeNode *node = bintree_node_create(value);
    SOFT_ASSERT(node != NULL, false);

    *link = node;
    ++tree->size;

    return true;
}

bool bintree_remove(Bintree *tree, int value) {
    // TODO: do it yourself

    (void)tree;
    (void)value;

    return false;
}

static void bintree_dump_node(FILE *dot, const BintreeNode *node) {
    if (node == NULL)
        return;

    fprintf(dot,
            "    \"%p\" [label=\"%d\", shape=circle, style=filled, fillcolor=\"#%02x%02x%02x\"];\n",
            (const void *)node, node->value, 0xc0, 0xd8, 0xff);

    if (node->left) {
        fprintf(dot, "    \"%p\" -> \"%p\" [label=\"L\"];\n", (const void *)node,
                (const void *)node->left);
        bintree_dump_node(dot, node->left);
    }

    if (node->right) {
        fprintf(dot, "    \"%p\" -> \"%p\" [label=\"R\"];\n", (const void *)node,
                (const void *)node->right);
        bintree_dump_node(dot, node->right);
    }
}

static void bintree_compile_dot(const char *dot_path, const char *image_path) {
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

void bintree_dump(const Bintree *tree, const char *image_path) {
    SOFT_ASSERT(tree != NULL, (void)0);
    SOFT_ASSERT(image_path != NULL, (void)0);

    char dot_path[] = "/tmp/bintree-XXXXXX.dot";
    int fd = mkstemps(dot_path, 4);
    SOFT_ASSERT(fd != -1, (void)0);

    FILE *dot = fdopen(fd, "w");
    if (dot == NULL)
        close(fd);
    SOFT_ASSERT(dot != NULL, (void)0);

    fprintf(dot, "digraph bintree {\n");
    fprintf(dot, "    graph [rankdir=TB];\n");
    fprintf(dot, "    node [fontname=\"Helvetica\"];\n");

    if (tree->root == NULL) {
        fprintf(dot, "    empty [label=\"empty\", shape=box, style=dashed];\n");
    } else {
        bintree_dump_node(dot, tree->root);
    }

    fprintf(dot, "}\n");
    fclose(dot);

    bintree_compile_dot(dot_path, image_path);
}
