#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *queue = malloc(sizeof(struct list_head));
    if (queue) {
        INIT_LIST_HEAD(queue);
        return queue;
    }
    return NULL;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    if (l == NULL) {
        return;
    }
    struct list_head *node, *next;
    list_for_each_safe (node, next, l) {
        element_t *element = list_entry(node, element_t, list);
        free(element);
    }
    free(l);
}

/* hi */
element_t *create_element(char *s)
{
    element_t *element = malloc(sizeof(element_t));
    INIT_LIST_HEAD(&element->list);
    int len = strlen(s);
    char *s_copy = malloc(len + 1);
    strncpy(s_copy, s, len);
    s_copy[len] = 0;
    element->value = s_copy;

    return element;
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    element_t *element = create_element(s);
    list_add(&element->list, head);

    // printf("e: %p, v: %p\n", element, element->value);

    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    element_t *element = create_element(s);
    list_add_tail(&element->list, head);

    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (head == head->next) {
        return NULL;
    }
    element_t *elem = list_entry(head->next, element_t, list);
    list_del(head->next);
    if (sp != NULL) {
        strncpy(sp, elem->value, bufsize - 1);
    }
    return elem;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    element_t *elem = list_entry(head->prev, element_t, list);
    list_del(head->prev);
    if (sp != NULL) {
        strncpy(sp, elem->value, bufsize - 1);
    }
    return elem;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int len = 0;
    struct list_head *li;
    list_for_each (li, head)
        len++;

    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (head == NULL || head == head->next) {
        return false;
    }

    struct list_head *onestep = head->next;
    struct list_head *twostep = head->next;

    while (twostep->next != head && twostep->next->next != head) {
        onestep = onestep->next;
        twostep = twostep->next->next;
    }
    list_del(onestep);

    element_t *elem = list_entry(onestep, element_t, list);
    free(elem->value);
    free(elem);

    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head) {}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending order */
int q_merge(struct list_head *head)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
