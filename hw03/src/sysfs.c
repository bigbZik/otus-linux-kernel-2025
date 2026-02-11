#include "kernel_stack.h"
#include "stack.h"
#include "stack_ops.h"
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/list.h>
#include <linux/sysfs.h>

extern struct stack *st;

static ssize_t push_store(struct kobject *kobj, struct kobj_attribute *attr,
                          const char *buf, size_t count) {
  int value;
  int res;

  res = kstrtoint(buf, 10, &value);
  if (res) {
    res = -EINVAL;
    goto out;
  }

  res = stack_push(st, value);

out:
  return res ? res : count;
}
static struct kobj_attribute kernel_stack_push_attr = __ATTR_WO(push);

static ssize_t pop_show(struct kobject *kobj, struct kobj_attribute *attr,
                        char *buf) {
  int res;

  res = stack_pop(st);
  if (!res) {
    goto out;
  }

  res = sysfs_emit(buf, "%d\n", res);

out:
  return res;
}
static struct kobj_attribute kernel_stack_pop_attr = __ATTR_RO(pop);

static ssize_t peek_show(struct kobject *kobj, struct kobj_attribute *attr,
                         char *buf) {
  int res;

  res = stack_peek(st);
  if (!res) {
    goto out;
  }

  res = sysfs_emit(buf, "%d\n", res);

out:
  return res;
}
static struct kobj_attribute kernel_stack_peek_attr = __ATTR_RO(peek);

static ssize_t size_show(struct kobject *kobj, struct kobj_attribute *attr,
                         char *buf) {
  return sysfs_emit(buf, "%d\n", stack_size(st));
}
static struct kobj_attribute kernel_stack_size_attr = __ATTR_RO(size);

static ssize_t is_empty_show(struct kobject *kobj, struct kobj_attribute *attr,
                             char *buf) {
  return sysfs_emit(buf, "%d\n", stack_is_empty(st));
}
static struct kobj_attribute kernel_stack_is_empty_attr = __ATTR_RO(is_empty);

static ssize_t clear_store(struct kobject *kobj, struct kobj_attribute *attr,
                           const char *buf, size_t count) {
  stack_clear(st);
  return count;
}
static struct kobj_attribute kernel_stack_clear_attr = __ATTR_WO(clear);

static struct attribute *kernel_stack_attrs[] = {
    &kernel_stack_push_attr.attr,
    &kernel_stack_pop_attr.attr,
    &kernel_stack_peek_attr.attr,
    &kernel_stack_size_attr.attr,
    &kernel_stack_is_empty_attr.attr,
    &kernel_stack_clear_attr.attr,
    NULL,
};

static const struct attribute_group kernel_stack_attr_group = {
    .attrs = kernel_stack_attrs,
};

int kernel_stack_sysfs_add(struct kobject *kobj) {
  int ret;

  ret = sysfs_create_group(kobj, &kernel_stack_attr_group);
  if (!ret) {
    kobject_uevent(kobj, KOBJ_CHANGE);
  }

  return ret;
}

void kernel_stack_sysfs_remove(struct kobject *kobj) {
  sysfs_remove_group(kobj, &kernel_stack_attr_group);
  kobject_uevent(kobj, KOBJ_CHANGE);
}
