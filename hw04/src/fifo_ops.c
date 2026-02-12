#include "fifo_ops.h"
#include <linux/kfifo.h>

struct fifo_entry {
  int data; /* Данные (целое число) */
};

int fifo_init(struct kfifo *queue, int size) {
  int ret = FIFO_OK;
  int res;

  res = kfifo_alloc(queue, size, GFP_KERNEL);
  if (res) {
    ret = FIFO_NOMEM;
    pr_err("Failed to initialize FIFO: %d\n", res);
  }

  pr_debug("FIFO initialized with size %d\n", size);

  return ret;
}

int fifo_enqueue(struct kfifo *queue, int value) {
  int ret = FIFO_OK;
  struct fifo_entry entry;

  if (fifo_is_full(queue)) {
    ret = FIFO_FULL;
    goto out;
  }

  entry.data = value;
  if (!kfifo_in(queue, &entry, 1)) {
    ret = FIFO_NOMEM;
    pr_err("Failed to enqueue data\n");
  } else {
    pr_debug("Enqueued data: %d\n", value);
  }

out:
  return ret;
}

int fifo_dequeue(struct kfifo *queue, int *value) {
  int ret = FIFO_OK;
  struct fifo_entry entry;

  if (fifo_is_empty(queue)) {
    ret = FIFO_EMPTY;
    goto out;
  }

  if (!kfifo_out(queue, &entry, 1)) {
    ret = FIFO_INVALID;
    pr_err("Failed to dequeue data\n");
  } else {
    *value = entry.data;
    pr_debug("Dequeued data: %d\n", *value);
  }

out:
  return ret;
}

int fifo_peek(struct kfifo *queue, int *value) {
  int ret = FIFO_OK;
  struct fifo_entry entry;

  if (fifo_is_empty(queue)) {
    ret = FIFO_EMPTY;
    goto out;
  }

  if (!kfifo_peek(queue, &entry)) {
    ret = FIFO_INVALID;
    pr_err("Failed to peek data\n");
  } else {
    *value = entry.data;
    pr_debug("Peeked data: %d\n", *value);
  }

out:
  return ret;
}

int fifo_is_empty(struct kfifo *queue) { return kfifo_is_empty(queue) ? 1 : 0; }

int fifo_is_full(struct kfifo *queue) { return kfifo_is_full(queue) ? 1 : 0; }

int fifo_available(struct kfifo *queue) { return kfifo_avail(queue); }

int fifo_size(struct kfifo *queue) { return kfifo_len(queue); }

void fifo_clear(struct kfifo *queue) { kfifo_reset(queue); }
