#ifndef TIMSORT_H
#define TIMSORT_H
struct list_head;

typedef int (*list_cmp_func_t)(const struct list_head *,
                               const struct list_head *);



void timsort(struct list_head *head, list_cmp_func_t cmp);
#endif /* TIMSORT_H */