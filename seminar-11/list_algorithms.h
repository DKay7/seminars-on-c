#ifndef LIST_ALGORITHMS_H
#define LIST_ALGORITHMS_H

#include "list.h"

void list_reverse(List *list);
bool list_has_cycle(const List *list);
ListNode *list_find(List *list, int value);

#endif // LIST_ALGORITHMS_H
