#!/bin/bash

# Загрузить модуль
sudo insmod kernel_stack.ko

# Добавить элементы
echo "Push: 10, 20 , 30"
echo 10 > /sys/kernel/kernel_stack/push
echo 20 > /sys/kernel/kernel_stack/push
echo 30 > /sys/kernel/kernel_stack/push

# Проверить размер
echo "Size (expect 3):"
cat /sys/kernel/kernel_stack/size
# Вывод: 3

# Получить вершину без удаления
echo "Peek (expect 30):"
cat /sys/kernel/kernel_stack/peek
# Вывод: 30

# Удалить и получить элемент
echo "Pop (expect 30):"
cat /sys/kernel/kernel_stack/pop
# Вывод: 30

# Проверить новый размер
echo "Size (expect 2):"
cat /sys/kernel/kernel_stack/size
# Вывод: 2

# Проверить пустоту
echo "Is empty (expect 0):"
cat /sys/kernel/kernel_stack/is_empty
# Вывод: 0

# Очистить стек
echo "Clear stack"
echo 1 > /sys/kernel/kernel_stack/clear

# Проверить пустоту
echo "Is empty (expect 1):"
cat /sys/kernel/kernel_stack/is_empty
# Вывод: 1

# Проверить новый размер
echo "Size (expect 0):"
cat /sys/kernel/kernel_stack/size
# Вывод: 0

# Добавить элементы
echo "Push: 999"
echo 999 > /sys/kernel/kernel_stack/push

# Получить вершину без удаления
echo "Peek (expect 999):"
cat /sys/kernel/kernel_stack/peek
# Вывод: 999

# Проверить пустоту
echo "Is empty (expect 0):"
cat /sys/kernel/kernel_stack/is_empty
# Вывод: 0

# Проверить новый размер
echo "Size (expect 1):"
cat /sys/kernel/kernel_stack/size
# Вывод: 1

# Удалить и получить элемент
echo "Pop (expect 999):"
cat /sys/kernel/kernel_stack/pop
# Вывод: 999

# Проверить пустоту
echo "Is empty (expect 1):"
cat /sys/kernel/kernel_stack/is_empty
# Вывод: 1

# Проверить новый размер
echo "Size (expect 0):"
cat /sys/kernel/kernel_stack/size
# Вывод: 0

# Получить вершину без удаления
echo "Peek (expect -1):"
cat /sys/kernel/kernel_stack/peek
# Вывод: -1

# Удалить и получить элемент
echo "Pop (expect -1):"
cat /sys/kernel/kernel_stack/pop
# Вывод: -1

# Очистить стек
echo "Clear stack"
echo 1 > /sys/kernel/kernel_stack/clear

# Проверить пустоту
echo "Is empty (expect 1):"
cat /sys/kernel/kernel_stack/is_empty
# Вывод: 1

# Проверить новый размер
echo "Size (expect 0):"
cat /sys/kernel/kernel_stack/size
# Вывод: 0

# Выгрузить модуль
sudo rmmod kernel_stack
