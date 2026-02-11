#ifndef __STACK_H__
#define __STACK_H__

#include <linux/list.h>

struct stack {
  struct list_head elements; /* Голова связного списка */
  int size; /* Текущее количество элементов */
};

struct stack *stack_create(void);
void stack_destroy(struct stack *st);

#endif /* __STACK_H__ */
