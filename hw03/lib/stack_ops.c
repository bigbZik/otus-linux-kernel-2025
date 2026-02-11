#include "stack_ops.h"
#include "stack.h"
#include <linux/slab.h>

struct stack_entry {
  struct list_head list; /* Узел связного списка */
  int data;              /* Данные (целое число) */
};

void stack_init(struct stack *st) {
  INIT_LIST_HEAD(&st->elements);
  st->size = 0;
}

int stack_push(struct stack *st, int value) {
  struct stack_entry *entry;
  int res = STACK_OK;

  entry = kmalloc(sizeof(struct stack_entry), GFP_KERNEL);
  if (!entry) {
    res = STACK_NOMEM;
    goto out;
  }
  entry->data = value;
  list_add_tail(&entry->list, &st->elements);
  st->size++;

out:
  return res;
}

int stack_pop(struct stack *st) {
  int val;
  struct stack_entry *entry;

  if (stack_is_empty(st)) {
    val = STACK_EMPTY;
    goto out;
  }

  entry = list_last_entry(&st->elements, struct stack_entry, list);
  list_del(&entry->list);
  st->size--;
  val = entry->data;
  kfree(entry);

out:
  return val;
}

int stack_peek(struct stack *st) {
  struct stack_entry *entry;
  int val;

  if (stack_is_empty(st)) {
    val = STACK_EMPTY;
    goto out;
  }

  entry = list_last_entry(&st->elements, struct stack_entry, list);
  val = entry->data;

out:
  return val;
}

int stack_is_empty(struct stack *st) { return (st->size == 0 ? 1 : 0); }

int stack_size(struct stack *st) { return st->size; }

void stack_clear(struct stack *st) {
  while (!stack_is_empty(st)) {
    stack_pop(st);
  }
}
