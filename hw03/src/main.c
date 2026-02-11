#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include "kernel_stack.h"
#include "stack.h"
#include "stack_ops.h"
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/module.h>

struct stack *st = NULL;
static struct kobject *kernel_stack_kobj = NULL;

/* Main code */

static int __init kernel_stack_module_init(void) {
  int ret = 0;

  kernel_stack_kobj = kobject_create_and_add("kernel_stack", kernel_kobj);
  if (!kernel_stack_kobj) {
    ret = -ENOMEM;
    pr_err("Failed to create kobject\n");
    goto out;
  }

  st = stack_create();
  if (!st) {
    pr_err("Failed to create stack\n");
    ret = -ENOMEM;
    goto stack_error;
  }

  if (kernel_stack_sysfs_add(kernel_stack_kobj)) {
    pr_err("Error on attempt ot add sysfs attributes\n");
    ret = -EINVAL;
    goto sysfs_error;
  }

  goto out;

sysfs_error:
  stack_destroy(st);
  st = NULL;

stack_error:
  kobject_put(kernel_stack_kobj);

out:
  return ret;
}

static void __exit kernel_stack_module_exit(void) {
  pr_info("kernel_stack module exiting\n");
  if (kernel_stack_kobj) {
    kernel_stack_sysfs_remove(kernel_stack_kobj);
    kobject_put(kernel_stack_kobj);
  }
  if (st) {
    stack_destroy(st);
    st = NULL;
  }
}

module_init(kernel_stack_module_init);
module_exit(kernel_stack_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Evgeny Vagin");
MODULE_DESCRIPTION("Otus Homework 03 Module: Kernel stack implementation");
