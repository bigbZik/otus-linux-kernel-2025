#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include "fifo_ops.h"
#include "kernel_fifo.h"
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kfifo.h>
#include <linux/module.h>
#include <linux/slab.h>

struct fifo_device *queue_dev = NULL;

static int fifo_device_create(void) {
  int res;

  queue_dev = kmalloc(sizeof(struct fifo_device), GFP_KERNEL);
  if (!queue_dev) {
    pr_err("Failed to allocate memory for FIFO device\n");
    res = -ENOMEM;
    goto out;
  }
  queue_dev->max_size = QUEUE_SIZE;

  res = fifo_init(&queue_dev->queue, queue_dev->max_size);
  if (res) {
    pr_err("Failed to initialize FIFO device: %d\n", res);
    res = -ENOMEM;
    kfree(queue_dev);
    queue_dev = NULL;
  }

out:
  return res;
}

static void fifo_device_destroy(void) {
  if (queue_dev) {
    kfifo_free(&queue_dev->queue);
    kfree(queue_dev);
    queue_dev = NULL;
  }
}

/* Main Code */

static int __init kernel_fifo_init(void) {
  int ret;

  pr_info("Kernel FIFO module loaded\n");
  ret = fifo_device_create();
  if (ret) {
    pr_err("Failed to create FIFO device: %d\n", ret);
  }
  return ret;
}

static void __exit kernel_fifo_exit(void) {
  pr_info("Kernel FIFO module exiting\n");
  fifo_device_destroy();
}

module_init(kernel_fifo_init);
module_exit(kernel_fifo_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Evgeny Vagin");
MODULE_DESCRIPTION("Otus Homework 04 Module: simple kernel FIFO module");
