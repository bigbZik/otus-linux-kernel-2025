#!/bin/bash

# Загрузить модуль
echo "Loading kernel_fifo module..."
insmod kernel_fifo.ko

# Проверить пустоту
echo "Is the queue empty? (expect 1):"
cat /sys/module/kernel_fifo/parameters/is_empty
echo ""
# Вывод: 1

# Проверить переполненность
echo "Is the queue full? (expect 0):"
cat /sys/module/kernel_fifo/parameters/is_full
echo ""
# Вывод: 0

# Проверить новый размер
echo "Current queue size (expect 0):"
cat /sys/module/kernel_fifo/parameters/size
echo ""
# Вывод: 0

# Проверить свободное место
echo "Available space in queue (expect 64):"
cat /sys/module/kernel_fifo/parameters/available
echo ""
# Вывод: 64

# Получить элемент начала очереди без удаления
echo "Peeking at the front element (expect -1):"
cat /sys/module/kernel_fifo/parameters/peek
echo ""
# Вывод: -1

# Удалить и получить элемент из начала очереди
echo "Dequeuing an element (expect -1):"
cat /sys/module/kernel_fifo/parameters/dequeue
echo ""
# Вывод: -1

# Добавить элементы в очередь через параметры
echo "Enqueuing elements... 10, 20, 30"
echo 10 > /sys/module/kernel_fifo/parameters/enqueue
echo 20 > /sys/module/kernel_fifo/parameters/enqueue
echo 30 > /sys/module/kernel_fifo/parameters/enqueue

# Проверить размер очереди
echo 'Current queue size (expect 3):'
cat /sys/module/kernel_fifo/parameters/size
echo ""
# Вывод: 3

# Получить элемент начала очереди без удаления
echo "Peeking at the front element (expect 10):"
cat /sys/module/kernel_fifo/parameters/peek
echo ""
# Вывод: 10

# Удалить и получить элемент из начала очереди
echo "Dequeuing an element (expect 10):"
cat /sys/module/kernel_fifo/parameters/dequeue
echo ""
# Вывод: 10

# Проверить новый размер
echo "Current queue size (expect 2):"
cat /sys/module/kernel_fifo/parameters/size
echo ""
# Вывод: 2

# Проверить свободное место
echo "Available space in queue (expect 62):"
cat /sys/module/kernel_fifo/parameters/available
echo ""
# Вывод: 62

# Проверить пустоту
echo "Is the queue empty? (expect 0):"
cat /sys/module/kernel_fifo/parameters/is_empty
echo ""
# Вывод: 0

# Проверить переполненность
echo "Is the queue full? (expect 0):"
cat /sys/module/kernel_fifo/parameters/is_full
echo ""
# Вывод: 0

# Очистить очередь
echo "Clearing the queue..."
echo 1 > /sys/module/kernel_fifo/parameters/clear
echo ""

# Проверить пустоту
echo "Is the queue empty? (expect 1):"
cat /sys/module/kernel_fifo/parameters/is_empty
echo ""
# Вывод: 1

# Проверить переполненность
echo "Is the queue full? (expect 0):"
cat /sys/module/kernel_fifo/parameters/is_full
echo ""
# Вывод: 0

# Проверить новый размер
echo "Current queue size (expect 0):"
cat /sys/module/kernel_fifo/parameters/size
echo ""
# Вывод: 0

# Проверить свободное место
echo "Available space in queue (expect 64):"
cat /sys/module/kernel_fifo/parameters/available
echo ""
# Вывод: 64

# Получить элемент начала очереди без удаления
echo "Peeking at the front element (expect -1):"
cat /sys/module/kernel_fifo/parameters/peek
echo ""
# Вывод: -1

# Удалить и получить элемент из начала очереди
echo "Dequeuing an element (expect -1):"
cat /sys/module/kernel_fifo/parameters/dequeue
echo ""
# Вывод: -1

# Добавить элементы в очередь через параметры
echo "Enqueuing elements... 99"
echo 99 > /sys/module/kernel_fifo/parameters/enqueue

# Проверить новый размер
echo "Current queue size (expect 1):"
cat /sys/module/kernel_fifo/parameters/size
echo ""
# Вывод: 1

# Проверить свободное место
echo "Available space in queue (expect 63):"
cat /sys/module/kernel_fifo/parameters/available
echo ""
# Вывод: 63

# Проверить пустоту
echo "Is the queue empty? (expect 0):"
cat /sys/module/kernel_fifo/parameters/is_empty
echo ""
# Вывод: 0

# Проверить переполненность
echo "Is the queue full? (expect 0):"
cat /sys/module/kernel_fifo/parameters/is_full
echo ""
# Вывод: 0

# Удалить и получить элемент из начала очереди
echo "Dequeuing an element (expect 99):"
cat /sys/module/kernel_fifo/parameters/dequeue
echo ""
# Вывод: 99

# Удалить и получить элемент из начала очереди
echo "Dequeuing an element (expect -1):"
cat /sys/module/kernel_fifo/parameters/dequeue
echo ""
# Вывод: -1

# Проверить пустоту
echo "Is the queue empty? (expect 1):"
cat /sys/module/kernel_fifo/parameters/is_empty
echo ""
# Вывод: 1

# Проверить переполненность
echo "Is the queue full? (expect 0):"
cat /sys/module/kernel_fifo/parameters/is_full
echo ""
# Вывод: 0

# Проверить новый размер
echo "Current queue size (expect 0):"
cat /sys/module/kernel_fifo/parameters/size
echo ""
# Вывод: 0

# Проверить свободное место
echo "Available space in queue (expect 64):"
cat /sys/module/kernel_fifo/parameters/available
echo ""
# Вывод: 64

# Выгрузить модуль
echo "Unloading kernel_fifo module..."
rmmod kernel_fifo
