#ifndef __KERNEL_FIFO_H__
#define __KERNEL_FIFO_H__

#define QUEUE_SIZE 64 /* Максимальный размер очереди в элементах*/

struct fifo_device {
  struct kfifo queue; /* Встроенный буфер kfifo */
  int max_size;       /* Максимальный размер очереди */
};

#endif /* __KERNEL_FIFO_H__ */