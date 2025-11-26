#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stddef.h>

#define SOFT_ASSERT(cond, value)                                                                   \
    do {                                                                                           \
        if (!(cond)) {                                                                             \
            fprintf(stderr, "Soft assertion failed: %s at %s:%d\n", #cond, __FILE__, __LINE__);    \
            return (value);                                                                        \
        }                                                                                          \
    } while (0)

typedef struct s_list_node {
    int value;
    struct s_list_node *next;
} ListNode;

typedef struct {
    ListNode *head;
    size_t size;
} List;

List *list_create(size_t size);
void list_destroy(List *list);

bool list_insert(List *list, size_t index, int value);
bool list_delete(List *list, size_t index, int *out_value);

void list_dump(const List *list);

#endif // LIST_H
