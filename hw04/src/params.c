#include "fifo_ops.h"
#include "kernel_fifo.h"
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>

extern struct fifo_device *queue_dev;

/* Param enqueue */
static int parm_set_enqueue(const char *val, const struct kernel_param *kp) {
  int ret = 0;
  int value;

  ret = kstrtoint(val, 10, &value);
  if (ret) {
    ret = -EINVAL;
    goto out;
  }
  fifo_enqueue(&queue_dev->queue, value);
out:
  return ret;
}

static const struct kernel_param_ops parm_ops_enqueue = {
    .set = parm_set_enqueue,
    .get = NULL,
};
module_param_cb(enqueue, &parm_ops_enqueue, NULL, 0200);
MODULE_PARM_DESC(enqueue, "An integer value to enqueue into the FIFO device");

/* Param dequeue */

static int parm_get_dequeue(char *buf, const struct kernel_param *kp) {
  int value, res;

  res = fifo_dequeue(&queue_dev->queue, &value);
  if (res) {
    pr_err("Failed to dequeue: %d\n", res);
    value = res; // Return error code as value for visibility
  }

  return sprintf(buf, "%d", value);
}

static const struct kernel_param_ops parm_ops_dequeue = {
    .set = NULL,
    .get = parm_get_dequeue,
};
module_param_cb(dequeue, &parm_ops_dequeue, NULL, 0400);
MODULE_PARM_DESC(dequeue, "Dequeue from the FIFO device");

/* Param peek */

static int parm_get_peek(char *buf, const struct kernel_param *kp) {
  int value, res;

  res = fifo_peek(&queue_dev->queue, &value);
  if (res) {
    pr_err("Failed to peek: %d\n", res);
    value = res; // Return error code as value for visibility
  }

  return sprintf(buf, "%d", value);
}

static const struct kernel_param_ops parm_ops_peek = {
    .set = NULL,
    .get = parm_get_peek,
};
module_param_cb(peek, &parm_ops_peek, NULL, 0400);
MODULE_PARM_DESC(peek, "Peek at the FIFO device");

/* Param available */

static int parm_get_available(char *buf, const struct kernel_param *kp) {
  return sprintf(buf, "%d", fifo_available(&queue_dev->queue));
}

static const struct kernel_param_ops parm_ops_available = {
    .set = NULL,
    .get = parm_get_available,
};
module_param_cb(available, &parm_ops_available, NULL, 0400);
MODULE_PARM_DESC(available, "Available space in the FIFO device");

/* Param size */

static int parm_get_size(char *buf, const struct kernel_param *kp) {
  return sprintf(buf, "%d", fifo_size(&queue_dev->queue));
}

static const struct kernel_param_ops parm_ops_size = {
    .set = NULL,
    .get = parm_get_size,
};
module_param_cb(size, &parm_ops_size, NULL, 0400);
MODULE_PARM_DESC(size, "Size of the FIFO device");

/* Param is_empty */

static int parm_get_is_empty(char *buf, const struct kernel_param *kp) {
  return sprintf(buf, "%d", fifo_is_empty(&queue_dev->queue));
}

static const struct kernel_param_ops parm_ops_is_empty = {
    .set = NULL,
    .get = parm_get_is_empty,
};
module_param_cb(is_empty, &parm_ops_is_empty, NULL, 0400);
MODULE_PARM_DESC(is_empty, "Check if the FIFO device is empty");

/* Param is_full */

static int parm_get_is_full(char *buf, const struct kernel_param *kp) {
  return sprintf(buf, "%d", fifo_is_full(&queue_dev->queue));
}

static const struct kernel_param_ops parm_ops_is_full = {
    .set = NULL,
    .get = parm_get_is_full,
};
module_param_cb(is_full, &parm_ops_is_full, NULL, 0400);
MODULE_PARM_DESC(is_full, "Check if the FIFO device is full");

/* Param clear */
static int parm_set_clear(const char *val, const struct kernel_param *kp) {
  fifo_clear(&queue_dev->queue);
  return 0;
}

static const struct kernel_param_ops parm_ops_clear = {
    .set = parm_set_clear,
    .get = NULL,
};
module_param_cb(clear, &parm_ops_clear, NULL, 0200);
MODULE_PARM_DESC(clear, "Clear the FIFO device");
