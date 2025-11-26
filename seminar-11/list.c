#include "list.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

static ListNode *list_node_create(int value) {
    ListNode *node = calloc(1, sizeof(*node));
    SOFT_ASSERT(node != NULL, NULL);

    node->value = value;
    return node;
}

List *list_create(size_t size) {
    SOFT_ASSERT(size > 0, NULL);

    List *list = calloc(1, sizeof(*list));
    SOFT_ASSERT(list != NULL, NULL);

    list->head = list_node_create(0);
    ListNode *cur_node = list->head;

    for (size_t idx = 0; idx < size - 1; ++idx) {
        cur_node->next = list_node_create(0);
        cur_node = cur_node->next;
    }

    // making list circular!
    cur_node->next = list->head;
    list->size = size;

    return list;
}

static ListNode *list_get_node(const List *list, size_t index) {
    SOFT_ASSERT(list != NULL, NULL);
    SOFT_ASSERT(list->head != NULL, NULL);
    SOFT_ASSERT(index < list->size, NULL);

    ListNode *node = list->head;

    for (size_t i = 0; i < index; ++i)
        node = node->next;

    return node;
}
void list_destroy(List *list) {
    SOFT_ASSERT(list != NULL, (void)0);

    if (list->head) {
        ListNode *node = list->head;
        for (size_t i = 0; i < list->size; ++i) {
            ListNode *next = node->next;
            free(node);
            node = next;
        }
    }

    list->head = NULL;
    list->size = 0;
    free(list);
}

bool list_insert(List *list, size_t index, int value) {
    SOFT_ASSERT(list != NULL, false);
    SOFT_ASSERT(index <= list->size, false);

    ListNode *node = list_node_create(value);
    SOFT_ASSERT(node, false);

    size_t prev_index = index >= 1 ? index : list->size - index - 1;
    ListNode *prev = list_get_node(list, prev_index);
    SOFT_ASSERT(prev, false);

    node->next = prev->next;
    prev->next = node;

    if (index == 0)
        list->head = node;

    ++list->size;
    return true;
}

bool list_delete(List *list, size_t index, int *out_value) {
    SOFT_ASSERT(list != NULL, false);
    SOFT_ASSERT(index < list->size, false);
    SOFT_ASSERT(list->size >= 1, false);

    size_t prev_index = index >= 1 ? index : list->size - index;
    ListNode *prev = list_get_node(list, prev_index);
    SOFT_ASSERT(prev, false);
    ListNode *removed = prev->next;

    if (out_value)
        *out_value = removed->value;

    prev->next = removed->next;
    if (index == 0)
        list->head = removed->next;

    free(removed);
    --list->size;

    if (list->size == 0)
        list->head = NULL;

    return true;
}

void list_dump(const List *list) {
    SOFT_ASSERT(list != NULL, (void)0);

    printf("List(size=%zu): ", list->size);

    if (list->size == 0 || list->head == NULL) {
        printf("<empty>\n");
        return;
    }

    const ListNode *node = list->head;
    for (size_t i = 0; i < list->size; ++i) {
        printf("[%zu]=%d", i, node->value);
        node = node->next;
        if (i + 1 < list->size)
            printf(" -> ");
    }

    printf("\n");
}
