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
        free(element->value);
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
    if (head == NULL || head == head->next) {
        return false;
    }
    element_t *now, *next;
    element_t *dup_now;
    struct list_head *dup_element = q_new();

    list_for_each_entry_safe (now, next, head, list) {
        bool is_duplicated = false;
        list_for_each_entry (dup_now, dup_element, list) {
            if (strcmp(now->value, dup_now->value) == 0) {
                is_duplicated = true;
                break;
            }
        }

        if (!is_duplicated) {
            element_t *new_elem = create_element(now->value);
            list_add(dup_element, &new_elem->list);
        } else {
            list_del(&now->list);
        }
    }

    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (head == NULL) {
        return;
    }
    struct list_head *node = head->next;
    while (node != NULL && node->next != NULL && node->next != head) {
        struct list_head *node1 = node;
        struct list_head *node2 = node->next;

        /// n <-> n1 <-> n2 <-> nn
        /// n <-> n2 <-> n1 <-> nn
        node2->prev = node1->prev;
        node1->next = node2->next;

        node1->prev = node2;
        node2->next = node1;

        node = node->next->next;
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (head == NULL) {
        return;
    }
    struct list_head *node, *node_safe;
    list_for_each_safe (node, node_safe, head) {
        struct list_head *temp = node->next;
        node->next = node->prev;
        node->prev = temp;
    }

    struct list_head *temp = head->next;
    head->next = head->prev;
    head->prev = temp;
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    if (head == NULL) {
        return;
    }

    struct list_head *node, *node_safe;
    struct list_head *nhead;
    int i = 0;

    list_for_each_safe (node, node_safe, head) {
        if (i == 0) {
            nhead = node->prev;
        }
        struct list_head *temp = node->next;
        node->next = node->prev;
        node->prev = temp;
        ++i;

        if (i == k) {
            node_safe->prev = nhead->next;
            nhead->next = node;
            i = 0;
        }
    }
}

/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head)
{
    if (head == NULL || head->next == head) {
        return;
    }
    struct list_head *node_i;

    list_for_each (node_i, head) {
        // printf("node_i: %p\n", node_i);
        for (struct list_head *node_j = head->next; node_j != node_i;
             node_j = node_j->next) {
            // printf("node_j: %p\n", node_j);
            element_t *elem1 = list_entry(node_i, element_t, list);
            element_t *elem2 = list_entry(node_j, element_t, list);

            // printf("elem1: %s, elem2: %s\n", elem1->value, elem2->value);
            if (strcmp(elem1->value, elem2->value) < 0) {
                // printf("insert i: %p to j: %p\n", node_i, node_j);
                // a1 <-> i <-> a2, b1 <-> j <-> b2
                struct list_head *a1 = node_i->prev;
                struct list_head *a2 = node_i->next;
                struct list_head *b1 = node_j->prev;

                a1->next = a2;
                a2->prev = a1;

                b1->next = node_i;
                node_i->prev = b1;

                node_j->prev = node_i;
                node_i->next = node_j;

                // element_t* temp;

                // printf("l = [");
                // list_for_each_entry(temp, head, list) {
                //     printf("%s ", temp->value);
                // }
                // printf("]\n");
                break;
            }
        }
    }
}

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
    struct list_head *entry_list;
    queue_contex_t *first_queue = list_entry(head->next, queue_contex_t, chain);

    list_for_each (entry_list, head) {
        queue_contex_t *entry = list_entry(entry_list, queue_contex_t, chain);
        element_t *first_queue_now =
            list_entry(first_queue->q->next, element_t, list);
        element_t *node, *node_safe;

        if (entry_list == head->next) {
            continue;
        }

        list_for_each_entry_safe (node, node_safe, entry->q, list) {
            bool is_added = false;
            while (&first_queue_now->list != first_queue->q) {
                if (strcmp(first_queue_now->value, node->value) > 0) {
                    // remove now from old queue
                    node->list.prev->next = node->list.next;
                    node->list.next->prev = node->list.prev;

                    // add now to new queue
                    node->list.prev = first_queue_now->list.prev;
                    node->list.next = &first_queue_now->list;

                    first_queue_now->list.prev->next = &node->list;
                    first_queue_now->list.prev = &node->list;

                    is_added = true;
                    break;
                } else {
                    first_queue_now = list_entry(first_queue_now->list.next, element_t, list);
                }
            }
            if (!is_added) {
                // remove now from old queue
                node->list.prev->next = node->list.next;
                node->list.next->prev = node->list.prev;

                // add now to new queue
                node->list.prev = first_queue_now->list.prev;
                node->list.next = &first_queue_now->list;

                first_queue_now->list.prev->next = &node->list;
                first_queue_now->list.prev = &node->list;
            }
        }
    }

    return q_size(first_queue->q);
}
