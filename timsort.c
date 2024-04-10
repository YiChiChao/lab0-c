#include "timsort.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

static inline size_t run_size(struct list_head *head)
{
    if (!head)
        return 0;
    if (!head->next)
        return 1;
    return (size_t) (head->next->prev);
}

struct pair {
    struct list_head *head, *next;
};

static size_t stk_size;



static struct list_head *merge(list_cmp_func_t cmp,
                               struct list_head *a,
                               struct list_head *b)
{
    struct list_head *head = a;
    struct list_head **tail = &head;
    for (; a && b;) {
        /* if equal, take 'a' -- important for sort stability */
        struct list_head **target = (cmp(a, b) <= 0) ? &a : &b;
        *tail = *target;
        *target = (*target)->next;
        tail = &((*tail)->next);
    }
    *tail = (struct list_head *) ((uintptr_t) a | (uintptr_t) b);

    return head;
}

static void build_prev_link(struct list_head *head,
                            struct list_head *tail,
                            struct list_head *list)
{
    tail->next = list;
    do {
        list->prev = tail;
        tail = list;
        list = list->next;
    } while (list);

    /* The final links to make a circular doubly-linked list */
    tail->next = head;
    head->prev = tail;
}

static void merge_final(list_cmp_func_t cmp,
                        struct list_head *head,
                        struct list_head *a,
                        struct list_head *b)
{
    struct list_head *tail = head;
    for (; a && b;) {
        /* if equal, take 'a' -- important for sort stability */
        struct list_head **target = (cmp(a, b) <= 0) ? &a : &b;
        tail->next = *target;
        (*target)->prev = tail;
        tail = *target;
        *target = (*target)->next;
    }
    struct list_head *result =
        (struct list_head *) ((uintptr_t) a | (uintptr_t) b);

    /* Finish linking remainder of list b on to tail */
    build_prev_link(head, tail, result);
}



static struct pair *find_run(struct list_head *list, list_cmp_func_t cmp)
{
    size_t len = 1;
    struct list_head *next = list->next, *head = list;
    struct pair *result = malloc(sizeof(struct pair));
    /* when there is only one element in the list*/
    if (!next) {
        result->head = head, result->next = next;
        return result;
    }

    if (cmp(list, next) > 0) {
        /* decending run, also reverse the list */
        struct list_head *prev = NULL;
        do {
            len++;
            list->next = prev;
            prev = list;
            list = next;
            next = list->next;
            head = list;
        } while (next && cmp(list, next) > 0);
        list->next = prev;
    } else {
        do {
            len++;
            list = next;
            next = list->next;
        } while (next && cmp(list, next) <= 0);
        list->next = NULL;
    }


    head->prev = NULL;
    head->next->prev = (struct list_head *) len;
    result->head = head, result->next = next;

    return result;
}

static struct list_head *merge_at(list_cmp_func_t cmp, struct list_head *at)
{
    size_t len = run_size(at) + run_size(at->prev);
    struct list_head *prev = at->prev->prev;
    struct list_head *list = merge(cmp, at->prev, at);
    list->prev = prev;
    list->next->prev = (struct list_head *) len;
    --stk_size;
    return list;
}

static struct list_head *merge_force_collapse(list_cmp_func_t cmp,
                                              struct list_head *tp)
{
    while (stk_size >= 3) {
        if (run_size(tp->prev->prev) < run_size(tp)) {
            tp->prev = merge_at(cmp, tp->prev);
        } else {
            tp = merge_at(cmp, tp);
        }
    }
    return tp;
}

static struct list_head *merge_collapse(list_cmp_func_t cmp,
                                        struct list_head *tp)
{
    int n;
    while ((n = stk_size) >= 2) {
        if ((n >= 3 &&
             run_size(tp->prev->prev) <= run_size(tp->prev) + run_size(tp)) ||
            (n >= 4 && run_size(tp->prev->prev->prev) <=
                           run_size(tp->prev->prev) + run_size(tp->prev))) {
            if (run_size(tp->prev->prev) < run_size(tp)) {
                tp->prev = merge_at(cmp, tp->prev);
            } else {
                tp = merge_at(cmp, tp);
            }
        } else if (run_size(tp->prev) <= run_size(tp)) {
            tp = merge_at(cmp, tp);
        } else {
            break;
        }
    }

    return tp;
}

void timsort(struct list_head *head, list_cmp_func_t cmp)
{
    stk_size = 0;

    struct list_head *list = head->next, *tp = NULL;
    if (head == head->prev)
        return;

    /* Convert to a null-terminated singly-linked list. */
    head->prev->next = NULL;

    do {
        /* Find next run */
        struct pair *result = find_run(list, cmp);
        result->head->prev = tp;
        tp = result->head;
        list = result->next;
        free(result);
        result = NULL;
        stk_size++;
        tp = merge_collapse(cmp, tp);
    } while (list);

    /* End of input; merge together all the runs. */
    tp = merge_force_collapse(cmp, tp);

    /* The final merge; rebuild prev links */
    struct list_head *stk0 = tp, *stk1 = stk0->prev;
    while (stk1 && stk1->prev)
        stk0 = stk0->prev, stk1 = stk1->prev;
    if (stk_size <= 1) {
        build_prev_link(head, head, stk0);
        return;
    }
    merge_final(cmp, head, stk1, stk0);
}