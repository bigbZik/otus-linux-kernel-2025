## ДЗ03. Создание модуля ядра Linux со стеком на основе связных списков

Выполнено для Ubuntu 24.03:    
`Linux bzkUbuntu24 6.14.0 #4 SMP PREEMPT_DYNAMIC Tue Jan 27 11:10:10 PST 2026 x86_64 x86_64 x86_64 GNU/Linux`

### Выполненные шаги:
1. Создание кода модуля с требуемыми параметрами (см task.md[task.md])
2. Создание Makefile и Kbuild для сборки и инсталяции файла.
3. Ручное тестирование модуля и Makefile.

```shell
$ make build
make -C /lib/modules/6.14.0/build M=/home/bzk/dev/otus-linux-kernel-2025/hw03 modules
make[1]: Entering directory '/usr/src/linux-headers-6.14.0'
make[2]: Entering directory '/home/bzk/dev/otus-linux-kernel-2025/hw03'
  CC [M]  src/main.o
  CC [M]  src/sysfs.o
  CC [M]  lib/stack.o
  CC [M]  lib/stack_ops.o
  LD [M]  kernel_stack.o
  MODPOST Module.symvers
  CC [M]  kernel_stack.mod.o
  CC [M]  .module-common.o
  LD [M]  kernel_stack.ko
make[2]: Leaving directory '/home/bzk/dev/otus-linux-kernel-2025/hw03'
make[1]: Leaving directory '/usr/src/linux-headers-6.14.0'

$ $ sudo ls -l /sys/kernel/kernel_stack/
total 0
--w------- 1 root root 4096 Feb 10 18:15 clear
-r--r--r-- 1 root root 4096 Feb 10 18:15 is_empty
-r--r--r-- 1 root root 4096 Feb 10 18:15 peek
-r--r--r-- 1 root root 4096 Feb 10 18:15 pop
--w------- 1 root root 4096 Feb 10 18:15 push
-r--r--r-- 1 root root 4096 Feb 10 18:15 size

$ sudo make remove
rmmod kernel_stack.ko

$ sudo make install
cp kernel_stack.ko /lib/modules/6.14.0
/sbin/depmod -a
/sbin/modprobe kernel_stack

$ sudo make uninstall
/sbin/modprobe -r kernel_stack
rm -f /lib/modules/6.14.0/kernel_stack.ko
/sbin/depmod -a

$ make clean
make -C /lib/modules/6.14.0/build M=/home/bzk/dev/otus-linux-kernel-2025/hw03 clean
make[1]: Entering directory '/usr/src/linux-headers-6.14.0'
make[2]: Entering directory '/home/bzk/dev/otus-linux-kernel-2025/hw03'
  CLEAN   Module.symvers
make[2]: Leaving directory '/home/bzk/dev/otus-linux-kernel-2025/hw03'
make[1]: Leaving directory '/usr/src/linux-headers-6.14.0'
```
4. Добавление в проект автоматическое тестирование через shell скипт.       Добавление цели `test` в Makefile.    
   Тестирование:

```shell
$ sudo make test
./checker/test.sh
Push: 10, 20 , 30
Size (expect 3):
3
Peek (expect 30):
30
Pop (expect 30):
30
Size (expect 2):
2
Is empty (expect 0):
0
Clear stack
Is empty (expect 1):
1
Size (expect 0):
0
Push: 999
Peek (expect 999):
999
Is empty (expect 0):
0
Size (expect 1):
1
Pop (expect 999):
999
Is empty (expect 1):
1
Size (expect 0):
0
Peek (expect -1):
-1
Pop (expect -1):
-1
Clear stack
Is empty (expect 1):
1
Size (expect 0):
0
```

5. Добавление цели `format` в Makefile для автоматического тестирования модуля.    
Тестирование:
```shell
$ make format
clang-format-18 -i /home/bzk/dev/otus-linux-kernel-2025/hw03/src/*.c /home/bzk/dev/otus-linux-kernel-2025/hw03/inc/*.h /home/bzk/dev/otus-linux-kernel-2025/hw03/lib/*.c /home/bzk/dev/otus-linux-kernel-2025/hw03/lib/inc/*.h
```

6. Добавление целей `dkms-install` и `dkms-remove` в Makefile для включения/удаления модуля в/из dkms.     
Тестирование:

```shell
$ sudo make dkms-remove 
/usr/sbin/dkms remove "kernel_stack/1.0"
Module kernel_stack-1.0 for kernel 6.14.0 (x86_64).
Before uninstall, this module version was ACTIVE on this kernel.

kernel_stack.ko:
 - Uninstallation
   - Deleting from: /lib/modules/6.14.0/updates/dkms/
 - Original module
   - No original module was found for this module on this kernel.
   - Use the dkms install command to reinstall any previous module version.
depmod...
Deleting module kernel_stack-1.0 completely from the DKMS tree.
rm -rf /usr/src/kernel_stack-1.0
bzk@bzkUbuntu24:~/dev/otus-linux-kernel-2025/hw03$ 
bzk@bzkUbuntu24:~/dev/otus-linux-kernel-2025/hw03$ 
bzk@bzkUbuntu24:~/dev/otus-linux-kernel-2025/hw03$ 
bzk@bzkUbuntu24:~/dev/otus-linux-kernel-2025/hw03$ sudo make dkms-install
mkdir -p /usr/src/kernel_stack-1.0/src
mkdir -p /usr/src/kernel_stack-1.0/inc
mkdir -p /usr/src/kernel_stack-1.0/lib
mkdir -p /usr/src/kernel_stack-1.0/lib/inc
cp src/*.c /usr/src/kernel_stack-1.0/src
cp inc/*.h /usr/src/kernel_stack-1.0/inc
cp lib/*.c /usr/src/kernel_stack-1.0/lib
cp lib/inc/*.h /usr/src/kernel_stack-1.0/lib/inc
cp Makefile /usr/src/kernel_stack-1.0
cp Kbuild /usr/src/kernel_stack-1.0
echo "$DKMS_CONF" > /usr/src/kernel_stack-1.0/dkms.conf
/usr/sbin/dkms install "kernel_stack/1.0"
Kernel config /lib/modules/6.14.0/build/.config not found, modules won't be signed
Creating symlink /var/lib/dkms/kernel_stack/1.0/source -> /usr/src/kernel_stack-1.0

Building module:
Cleaning build area...
make -j6 KERNELRELEASE=6.14.0 build...
cp: cannot stat '/lib/modules/6.14.0/build/.config': No such file or directory
Cleaning build area...

kernel_stack.ko:
Running module version sanity check.
 - Original module
   - No original module exists within this kernel
 - Installation
   - Installing to /lib/modules/6.14.0/updates/dkms/
depmod...
```
```shell
$ ls -l /usr/src/kernel_stack-1.0/
total 24
-rw-r--r-- 1 root root  179 Feb 10 18:20 dkms.conf
drwxr-xr-x 2 root root 4096 Feb 10 18:20 inc
-rw-r--r-- 1 root root  155 Feb 10 18:20 Kbuild
drwxr-xr-x 3 root root 4096 Feb 10 18:20 lib
-rw-r--r-- 1 root root 1693 Feb 10 18:20 Makefile
drwxr-xr-x 2 root root 4096 Feb 10 18:20 src

$ cat /usr/src/kernel_stack-1.0/dkms.conf 
PACKAGE_NAME="kernel_stack"
PACKAGE_VERSION="1.0"
BUILT_MODULE_NAME[0]="kernel_stack"
DEST_MODULE_LOCATION[0]="/updates"
AUTOINSTALL="yes"
MAKE[0]="make build"
CLEAN="make clean"
```
```
$ sudo make dkms-remove
/usr/sbin/dkms remove "kernel_stack/1.0"
Module kernel_stack-1.0 for kernel 6.14.0 (x86_64).
Before uninstall, this module version was ACTIVE on this kernel.

kernel_stack.ko:
 - Uninstallation
   - Deleting from: /lib/modules/6.14.0/updates/dkms/
 - Original module
   - No original module was found for this module on this kernel.
   - Use the dkms install command to reinstall any previous module version.
depmod...
Deleting module kernel_stack-1.0 completely from the DKMS tree.
rm -rf /usr/src/kernel_stack-1.0

$ ls -l /usr/src/kernel_stack-1.0/
ls: cannot access '/usr/src/kernel_stack-1.0/': No such file or directory
```
