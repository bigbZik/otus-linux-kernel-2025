#ifndef __KERNEL_STACK_H__
#define __KERNEL_STACK_H__

#include <linux/kobject.h>

int kernel_stack_sysfs_add(struct kobject *kobj);

void kernel_stack_sysfs_remove(struct kobject *kobj);

#endif /* __KERNEL_STACK_H__ */
