#include "stack.h"
#include "stack_ops.h"
#include <linux/slab.h>

struct stack *stack_create() {
  struct stack *st;
  st = kmalloc(sizeof(struct stack), GFP_KERNEL);
  if (!st) {
    return NULL;
  }

  stack_init(st);
  return st;
}

void stack_destroy(struct stack *st) {
  stack_clear(st);
  kfree(st);
}
