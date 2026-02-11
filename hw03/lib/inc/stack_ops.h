
#ifndef __STACK_OPS_H__
#define __STACK_OPS_H__

#include "stack.h"

#define STACK_OK 0       /* Операция успешна */
#define STACK_EMPTY -1   /* Стек пуст */
#define STACK_NOMEM -2   /* Нет памяти */
#define STACK_INVALID -3 /* Неверный параметр */

void stack_init(struct stack *st);
int stack_push(struct stack *st, int value);
int stack_pop(struct stack *st);
int stack_peek(struct stack *st);
int stack_is_empty(struct stack *st);
int stack_size(struct stack *st);
void stack_clear(struct stack *st);

#endif /* __STACK_OPS_H__ */
