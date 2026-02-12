## ДЗ04. Создание модуля ядра Linux на основе встроенного буфера kfifo

Выполнено для Ubuntu 24.03:    
`Linux bzkUbuntu24 6.14.0 #4 SMP PREEMPT_DYNAMIC Tue Jan 27 11:10:10 PST 2026 x86_64 x86_64 x86_64 GNU/Linux`

### Выполненные шаги:
1. Создание кода модуля с требуемыми параметрами (см task.md[task.md])
2. Создание Makefile и Kbuild для сборки и инсталяции файла.
3. Ручное тестирование модуля и Makefile.

```shell
$ make build
make -C /lib/modules/6.14.0/build M=/home/bzk/dev/otus-linux-kernel-2025/hw04 modules
make[1]: Entering directory '/usr/src/linux-headers-6.14.0'
make[2]: Entering directory '/home/bzk/dev/otus-linux-kernel-2025/hw04'
  CC [M]  src/main.o
  CC [M]  src/fifo_ops.o
  CC [M]  src/params.o
  LD [M]  kernel_fifo.o
  MODPOST Module.symvers
  CC [M]  kernel_fifo.mod.o
  CC [M]  .module-common.o
  LD [M]  kernel_fifo.ko
make[2]: Leaving directory '/home/bzk/dev/otus-linux-kernel-2025/hw04'
make[1]: Leaving directory '/usr/src/linux-headers-6.14.0'

$ sudo make run
insmod /home/bzk/dev/otus-linux-kernel-2025/hw04/kernel_fifo.ko

$ ls -l /sys/module/kernel_fifo/parameters/
total 0
-r-------- 1 root root 4096 Feb 11 17:15 available
--w------- 1 root root 4096 Feb 11 17:15 clear
-r-------- 1 root root 4096 Feb 11 17:15 dequeue
--w------- 1 root root 4096 Feb 11 17:15 enqueue
-r-------- 1 root root 4096 Feb 11 17:15 is_empty
-r-------- 1 root root 4096 Feb 11 17:15 is_full
-r-------- 1 root root 4096 Feb 11 17:15 peek
-r-------- 1 root root 4096 Feb 11 17:15 size

$ sudo make remove
rmmod kernel_fifo.ko

$ sudo make install
cp kernel_fifo.ko /lib/modules/6.14.0
/sbin/depmod -a
/sbin/modprobe kernel_fifo

$ sudo make uninstall
/sbin/modprobe -r kernel_fifo
rm -f /lib/modules/6.14.0/kernel_fifo.ko
/sbin/depmod -a

$ make clean
make -C /lib/modules/6.14.0/build M=/home/bzk/dev/otus-linux-kernel-2025/hw04 clean
make[1]: Entering directory '/usr/src/linux-headers-6.14.0'
make[2]: Entering directory '/home/bzk/dev/otus-linux-kernel-2025/hw04'
  CLEAN   Module.symvers
make[2]: Leaving directory '/home/bzk/dev/otus-linux-kernel-2025/hw04'
make[1]: Leaving directory '/usr/src/linux-headers-6.14.0'
rm *.tar.gz
rm: cannot remove '*.tar.gz': No such file or directory
make: *** [Makefile:56: clean] Error 1
```
4. Добавление в проект автоматическое тестирование через shell скипт.       Добавление цели `test` в Makefile.    
   Тестирование:

```shell
$ sudo make test
./checker/test.sh
Loading kernel_fifo module...
Is the queue empty? (expect 1):
1
Is the queue full? (expect 0):
0
Current queue size (expect 0):
0
Available space in queue (expect 64):
64
Peeking at the front element (expect -1):
-1
Dequeuing an element (expect -1):
-1
Enqueuing elements... 10, 20, 30
Current queue size (expect 3):
3
Peeking at the front element (expect 10):
10
Dequeuing an element (expect 10):
10
Current queue size (expect 2):
2
Available space in queue (expect 62):
62
Is the queue empty? (expect 0):
0
Is the queue full? (expect 0):
0
Clearing the queue...

Is the queue empty? (expect 1):
1
Is the queue full? (expect 0):
0
Current queue size (expect 0):
0
Available space in queue (expect 64):
64
Peeking at the front element (expect -1):
-1
Dequeuing an element (expect -1):
-1
Enqueuing elements... 99
Current queue size (expect 1):
1
Available space in queue (expect 63):
63
Is the queue empty? (expect 0):
0
Is the queue full? (expect 0):
0
Dequeuing an element (expect 99):
99
Dequeuing an element (expect -1):
-1
Is the queue empty? (expect 1):
1
Is the queue full? (expect 0):
0
Current queue size (expect 0):
0
Available space in queue (expect 64):
64
Unloading kernel_fifo module...
```

5. Добавление цели `format` в Makefile для автоматического тестирования модуля.    
Тестирование:
```shell
$ make format 
clang-format-18 -i /home/bzk/dev/otus-linux-kernel-2025/hw04/src/*.c /home/bzk/dev/otus-linux-kernel-2025/hw04/inc/*.h
```

6. Добавление целей `dkms-install` и `dkms-remove` в Makefile для включения/удаления модуля в/из dkms.     
Тестирование:

```shell
$ sudo make dkms-install 
mkdir -p /usr/src/kernel_fifo-1.0/src
mkdir -p /usr/src/kernel_fifo-1.0/inc
cp src/*.c /usr/src/kernel_fifo-1.0/src
cp inc/*.h /usr/src/kernel_fifo-1.0/inc
cp Makefile /usr/src/kernel_fifo-1.0
cp Kbuild /usr/src/kernel_fifo-1.0
echo "$DKMS_CONF" > /usr/src/kernel_fifo-1.0/dkms.conf
/usr/sbin/dkms install "kernel_fifo/1.0"
Kernel config /lib/modules/6.14.0/build/.config not found, modules won't be signed
Creating symlink /var/lib/dkms/kernel_fifo/1.0/source -> /usr/src/kernel_fifo-1.0

Building module:
Cleaning build area...
make -j6 KERNELRELEASE=6.14.0 build...
cp: cannot stat '/lib/modules/6.14.0/build/.config': No such file or directory
Cleaning build area...

kernel_fifo.ko:
Running module version sanity check.
 - Original module
   - No original module exists within this kernel
 - Installation
   - Installing to /lib/modules/6.14.0/updates/dkms/
depmod...
```
```shell
$ ls -l /usr/src/kernel_fifo-1.0/
total 20
-rw-r--r-- 1 root root  177 Feb 11 17:18 dkms.conf
drwxr-xr-x 2 root root 4096 Feb 11 17:18 inc
-rw-r--r-- 1 root root  119 Feb 11 17:18 Kbuild
-rw-r--r-- 1 root root 1717 Feb 11 17:18 Makefile
drwxr-xr-x 2 root root 4096 Feb 11 17:18 src

$ cat /usr/src/kernel_fifo-1.0/dkms.conf 
PACKAGE_NAME="kernel_fifo"
PACKAGE_VERSION="1.0"
BUILT_MODULE_NAME[0]="kernel_fifo"
DEST_MODULE_LOCATION[0]="/updates"
AUTOINSTALL="yes"
MAKE[0]="make build"
CLEAN="make clean"
```
```shell
$ sudo make dkms-remove 
/usr/sbin/dkms remove "kernel_fifo/1.0"
Module kernel_fifo-1.0 for kernel 6.14.0 (x86_64).
Before uninstall, this module version was ACTIVE on this kernel.

kernel_fifo.ko:
 - Uninstallation
   - Deleting from: /lib/modules/6.14.0/updates/dkms/
 - Original module
   - No original module was found for this module on this kernel.
   - Use the dkms install command to reinstall any previous module version.
depmod...
Deleting module kernel_fifo-1.0 completely from the DKMS tree.
rm -rf /usr/src/kernel_fifo-1.0

$ ls -l /usr/src/kernel_fifo-1.0/
ls: cannot access '/usr/src/kernel_fifo-1.0/': No such file or directory
```
