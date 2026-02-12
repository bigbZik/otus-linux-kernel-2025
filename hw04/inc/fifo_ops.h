#ifndef __FIFO_OPS_H__
#define __FIFO_OPS_H__

#include <linux/kfifo.h>

#define FIFO_OK 0       /* Операция успешна */
#define FIFO_EMPTY -1   /* Очередь пуста */
#define FIFO_FULL -2    /* Очередь полна */
#define FIFO_NOMEM -3   /* Нет памяти */
#define FIFO_INVALID -4 /* Неверный параметр */

int fifo_init(struct kfifo *queue, int size);
int fifo_enqueue(struct kfifo *queue, int value);
int fifo_dequeue(struct kfifo *queue, int *value);
int fifo_peek(struct kfifo *queue, int *value);
int fifo_is_empty(struct kfifo *queue);
int fifo_is_full(struct kfifo *queue);
int fifo_available(struct kfifo *queue);
int fifo_size(struct kfifo *queue);
void fifo_clear(struct kfifo *queue);

#endif /* __FIFO_OPS_H__ */
