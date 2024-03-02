#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>
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
        if (new_queue) {
        INIT_LIST_HEAD(new_queue);
    }

    }

    return new_queue;
}

/* Free all storage used by queue */
void q_free(struct list_head *head) {
    if (!head) {
        return;
    }
    struct list_head *cur, *safe;

    list_for_each_safe (cur, safe, head) {
        element_t *free_node = list_entry(cur, element_t, list);
        free(free_node->value);
        list_del(cur);
        free(free_node);
    }
    // Free head pointer
    free(head);
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
    return q_insert_head(head->prev, s);
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (list_empty(head)) {
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
                free(prob);
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
    struct list_head *node = head->next;
    while (node != head) {
        if (node->next == head)
            break;
        element_t *cur = list_entry(node, element_t, list);
        element_t *next = list_entry(node->next, element_t, list);
        char *tmp = cur->value;
        cur->value = next->value;
        next->value = tmp;
        node = node->next->next;
    }
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head)) {
        return;
    }
    struct list_head *tmp = head->prev;
    head->prev = head->next;
    head->next = tmp;
    struct list_head *cur = head->next;
    while (cur != head) {
        tmp = cur->prev;
        cur->prev = cur->next;
        cur->next = tmp;
        cur = cur->next;
    }
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    if (!head || list_empty(head) || list_is_singular(head)) {
        return;
    }
    // Check if the list size is over k
    int length = q_size(head);

    if (length < k)
        return;
    int iterate = length / k;
    struct list_head *nownode = head->next;
    struct list_head *head_prev = head;
    while (iterate--) {
        struct list_head *list_first = nownode;
        struct list_head *list_tail = NULL;
        struct list_head *tmp = NULL;
        for (int i = k; i > 0; i--) {
            if (i == 1) {
                list_tail = tmp;
            }
            tmp = nownode->next;
            nownode->next = nownode->prev;
            nownode->prev = tmp;
            nownode = tmp;
        }
        if (list_tail) {
            head_prev->next = list_tail;
            tmp = list_tail->prev;
            list_tail->prev = list_first->next;
            list_first->next = tmp;
            nownode->prev = list_first;
            head_prev = list_first;
        }
    }


    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

struct list_head *merge(struct list_head *L1, struct list_head *L2)
{
    struct list_head *head = L1, **ptr = &head;
    struct list_head *prev = NULL;
    while (L1 && L2) {
        if (strcmp(list_entry(L1, element_t, list)->value,
                   list_entry(L2, element_t, list)->value) <= 0) {
            *ptr = L1;
            (*ptr)->prev = prev;
            ptr = &(*ptr)->next;
            prev = L1;
            L1 = L1->next;
        } else {
            *ptr = L2;
            (*ptr)->prev = prev;
            ptr = &(*ptr)->next;
            prev = L2;
            L2 = L2->next;
        }
    }

    if (L1) {
        *ptr = L1;
        (*ptr)->prev = prev;
    }

    if (L2) {
        *ptr = L2;
        (*ptr)->prev = prev;
    }


    return head;
}

struct list_head *mergesort(struct list_head *head)
{
    if (!head || !head->next) {
        return head;
    }

    struct list_head *slow = head;
    struct list_head *fast = head->next;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    // Cut into two single linked list
    fast = slow->next;
    slow->next = NULL;
    return merge(mergesort(head), mergesort(fast));
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    if (!head || list_empty(head) || list_is_singular(head)) {
        return;
    }
    head->prev->next = NULL;
    head->next = mergesort(head->next);
    head->next->prev = head;
    struct list_head *get_tail = head->next;
    while (get_tail->next) {
        get_tail = get_tail->next;
    }
    head->prev = get_tail;
    get_tail->next = head;
    if (descend) {
        q_reverse(head);
    }
}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    char *less = NULL;
    struct list_head *nownode;
    struct list_head *safe;
    for (nownode = (head)->prev, safe = nownode->prev; nownode != (head);
         nownode = safe, safe = nownode->prev) {
        element_t *nownode_entry = list_entry(nownode, element_t, list);
        if (!less || strcmp(nownode_entry->value, less) < 0) {
            if (less) {
                free(less);
                less = NULL;
            }
            less = strdup(nownode_entry->value);
        } else {
            list_del(&nownode_entry->list);
            free(nownode_entry->value);
            free(nownode_entry);
        }
    }
    if (less) {
        free(less);
        less = NULL;
    }
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    char *greater = NULL;
    struct list_head *nownode;
    struct list_head *safe;
    for (nownode = (head)->prev, safe = nownode->prev; nownode != (head);
         nownode = safe, safe = nownode->prev) {
        element_t *nownode_entry = list_entry(nownode, element_t, list);
        if (!greater || strcmp(nownode_entry->value, greater) > 0) {
            if (greater) {
                free(greater);
                greater = NULL;
            }
            greater = strdup(nownode_entry->value);
        } else {
            list_del(&nownode_entry->list);
            free(nownode_entry->value);
            free(nownode_entry);
        }
    }
    if (greater) {
        free(greater);
        greater = NULL;
    }
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    if (!head || list_empty(head))
        return 0;

    queue_contex_t *node = list_entry(head->next, queue_contex_t, chain);
    if (list_is_singular(head))
        return node->size;

    struct list_head *tmp;
    for (tmp = head->next->next; tmp != head; tmp = tmp->next) {
        queue_contex_t *next = list_entry(tmp, queue_contex_t, chain);
        list_splice_init(next->q, node->q);
        next->size = 0;
        node->size = q_size(node->q);
    }
    q_sort(node->q, false);
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return node->size;
}
