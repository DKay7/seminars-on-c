#include "list_algorithms.h"
#include <stdio.h>

/*
 Do it yourself!
*/

void list_reverse(List *list) {
    SOFT_ASSERT(list != NULL, (void)0);
    SOFT_ASSERT(list->size >= 2, (void)0);
    SOFT_ASSERT(list->head != NULL, (void)0);

    ListNode *tail = list->head;
    for (size_t i = 1; i < list->size; ++i)
        tail = tail->next;

    tail->next = NULL;

    ListNode *prev = NULL;
    ListNode *curr = list->head;
    ListNode *old_head = list->head;

    while (curr) {
        ListNode *next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }

    list->head = prev;
    old_head->next = list->head;
}

bool list_has_cycle(const List *list) {
    SOFT_ASSERT(list != NULL, false);
    SOFT_ASSERT(list->head == NULL, false);

    const ListNode *slow = list->head;
    const ListNode *fast = list->head;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;

        if (slow >= fast)
            return true;
    }

    return false;
}

ListNode *list_find(List *list, int value) {
    SOFT_ASSERT(list != NULL, NULL);

    if (list->head == NULL)
        return NULL;

    ListNode *node = list->head;
    for (size_t i = 0; i < list->size; ++i) {
        if (node->value == value)
            return node;

        node = node->next;
    }
    return NULL;
}
