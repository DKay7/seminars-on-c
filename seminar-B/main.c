#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "list_algorithms.h"

int main(void) {
    List *list = list_create(1);

    list_insert(list, 0, 10);
    list_insert(list, 1, 20);
    list_insert(list, 2, 30);
    list_insert(list, 1, 15);

    printf("List contents:");
    list_dump(list);

    list_reverse(list);
    printf("Reversed list:");
    list_dump(list);

    printf("List has cycle: %s\n", list_has_cycle(list) ? "yes" : "no");

    ListNode *found = list_find(list, 20);
    if (found)
        printf("Found node with value %d\n", found->value);

    list_destroy(list);
    return EXIT_SUCCESS;
}
