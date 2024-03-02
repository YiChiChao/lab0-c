#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>
#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{  // instead of using LIST_HEAD creating local variable
    // use INIT_LIST_HEAD and malloc for both declaration and initialization
    struct list_head *new_queue = malloc(sizeof(struct list_head));
    if (new_queue) {
        INIT_LIST_HEAD(new_queue);
    }

    return new_queue;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    if (!l) {
        return;
    }
    struct list_head *cur, *safe;

    list_for_each_safe (cur, safe, l) {
        element_t *free_node = list_entry(cur, element_t, list);
        free(free_node->value);
        list_del(cur);
        free(free_node);
    }
    // Free head pointer
    free(l);
    return;
}


/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head) {
        // false for queue is NULL
        return false;
    }

    element_t *nownode = malloc(sizeof(element_t));
    assert(nownode);
    if (!nownode) {
        return false;
    }
    nownode->value = strdup(s);

    if (!nownode->value) {
        free(nownode);
        return false;
    }
    list_add(&nownode->list, head);
    return true;
}


/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    return q_insert_head(head->prev, s);
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (list_empty(head)) {
        return NULL;
    }
    element_t *remove_node = list_first_entry(head, element_t, list);
    list_del(&remove_node->list);

    // Check if sp is not a null pointer
    if (sp) {
        strncpy(sp, remove_node->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    return remove_node;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    return q_remove_head(head->prev->prev, sp, bufsize);
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head || list_empty(head)) {
        return 0;
    }

    int count = 0;
    struct list_head *node;
    list_for_each (node, head) {
        count++;
    }
    return count;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/

    if (!head || list_empty(head)) {
        return false;
    }

    int mid = q_size(head) / 2;

    // Get to the target node
    struct list_head *current = head->next;
    for (int i = mid; i > 0; i--) {
        current = current->next;
    }
    list_del(current);
    element_t *delete_node = list_entry(current, element_t, list);
    free(delete_node->value);
    delete_node->value = NULL;
    free(delete_node);

    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    if (!head || list_empty(head)) {
        return false;
    }
    // Only one element
    if (list_is_singular(head))
        return true;

    // tmp for traverse input linked list
    // prob to record the current duplicate string
    char *prob = NULL;
    element_t *cur;
    element_t *next;
    list_for_each_entry_safe (cur, next, head, list) {
        if (prob == NULL || strcmp(prob, cur->value)) {
            // Check if the current node has the same string
            // as the next node
            if (next == NULL || strcmp(cur->value, next->value)) {
                continue;
            } else {
                // Update the prob if cur and next have the same string
                prob = strdup(cur->value);
            }
        }
        list_del(&cur->list);
        free(cur->value);
        free(cur);
    }
    // Free the memory allocated for prob
    // In case prob is not a null pointer
    if (prob) {
        free(prob);
    }
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

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
