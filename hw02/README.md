## ДЗ02. Создание модуля Linux

Выполнено для Ubuntu 24.03:    
`Linux bzkUbuntu24 6.14.0 #4 SMP PREEMPT_DYNAMIC Tue Jan 27 11:10:10 PST 2026 x86_64 x86_64 x86_64 GNU/Linux`

### Выполненные шаги:
1. Создание кода модуля с требуемыми параметрами.
2. Создание Makefile для сборки и инсталяции файла.
3. Ручное тестирование модуля и Makefile.

```shell
$ make build
make -C /lib/modules/6.14.0/build M=/home/bzk/dev/otus-linux-kernel-2025/hw02 modules
make[1]: Entering directory '/usr/src/linux-headers-6.14.0'
make[2]: Entering directory '/home/bzk/dev/otus-linux-kernel-2025/hw02'
  CC [M]  hw02_module.mod.o
  LD [M]  hw02_module.ko
make[2]: Leaving directory '/home/bzk/dev/otus-linux-kernel-2025/hw02'
make[1]: Leaving directory '/usr/src/linux-headers-6.14.0'

$ sudo make run
insmod /home/bzk/dev/otus-linux-kernel-2025/hw02/hw02_module.ko

$ ls -l /sys/module/hw02_module/parameters/
total 0
-rw-r--r-- 1 root root 4096 Feb  2 21:09 ch_val
-rw-r--r-- 1 root root 4096 Feb  2 21:09 idx
-r--r--r-- 1 root root 4096 Feb  2 21:09 my_str

$ sudo echo -n "Hello" > /sys/module/hw02_module/parameters/my_str
-bash: /sys/module/hw02_module/parameters/my_str: Permission denied

$ sudo bash -c 'echo -n 'H' > /sys/module/hw02_module/parameters/ch_val'

$ sudo bash -c 'echo -n "H1" > /sys/module/hw02_module/parameters/ch_val'
bash: line 1: echo: write error: Invalid argument

$ sudo bash -c 'echo -n "99" > /sys/module/hw02_module/parameters/ch_val'
bash: line 1: echo: write error: Invalid argument

$ sudo make remove
rmmod hw02_module.ko

$ sudo make install 
cp hw02_module.ko /lib/modules/6.14.0
/sbin/depmod -a
/sbin/modprobe hw02_module

$ sudo make uninstall 
/sbin/modprobe -r hw02_module
rm -f /lib/modules/6.14.0/hw02_module.ko
/sbin/depmod -a

$ make clean 
make -C /lib/modules/6.14.0/build M=/home/bzk/dev/otus-linux-kernel-2025/hw02 clean
make[1]: Entering directory '/usr/src/linux-headers-6.14.0'
make[2]: Entering directory '/home/bzk/dev/otus-linux-kernel-2025/hw02'
  CLEAN   Module.symvers
make[2]: Leaving directory '/home/bzk/dev/otus-linux-kernel-2025/hw02'
make[1]: Leaving directory '/usr/src/linux-headers-6.14.0'

```
4. Добавление в проект автоматическое тестирование через python скипт.     
   Добавление цели `test` в Makefile.    
   Тестирование:

```shell
$ sudo make test
python3 checker/main.py hw02_module

--- Запуск тестов my_str ---
my_str_test: записано Hello, прочитано Hello        , ожидалось Hello
my_str_test: записано World!, прочитано World!       , ожидалось World!
my_str_test: записано Hello, World!, прочитано Hello, World!, ожидалось Hello, World!

Тесты my_str завершены: OK
Проверка my_str: True

--- Запуск тестов ch_val ---
ch_val_test: записано A, прочитано A, ожидалось A
ch_val_test: записано 123, прочитано 1, ожидалось 1
ch_val_test: записано 
, прочитано [Errno 22] Invalid argument, ожидалось [Errno 22] Invalid argument

Тесты ch_val завершены: OK
Проверка ch_val: True

--- Запуск тестов idx ---
idx_test: записано 0, прочитано 0, ожидалось 0
idx_test: записано 2, прочитано 2, ожидалось 2
idx_test: записано A, прочитано [Errno 22] Invalid argument, ожидалось [Errno 22] Invalid argument

Тесты idx завершены: OK
Проверка idx: True
```

5. Установка `clang-format`. Добавление цели `format` в Makefile для автоматического тестирования модуля.    
Тестирование:
```shell
$ make format
clang-format-18 -i /home/bzk/dev/otus-linux-kernel-2025/hw02/*.c
```

6. Добавление целей `dkms-install` и `dkms-remove` в Makefile для включения/удаления модуля в/из dkms.     
Тестирование:

```shell
$ sudo make dkms-install 
mkdir -p /usr/src/hw02_module-1.0
cp *.c /usr/src/hw02_module-1.0
cp Makefile /usr/src/hw02_module-1.0
echo "$DKMS_CONF" > /usr/src/hw02_module-1.0/dkms.conf
/usr/sbin/dkms install "hw02_module/1.0"
Kernel config /lib/modules/6.14.0/build/.config not found, modules won't be signed
Creating symlink /var/lib/dkms/hw02_module/1.0/source -> /usr/src/hw02_module-1.0

Building module:
Cleaning build area...
make -j6 KERNELRELEASE=6.14.0 build...
cp: cannot stat '/lib/modules/6.14.0/build/.config': No such file or directory
Cleaning build area...

hw02_module.ko:
Running module version sanity check.
 - Original module
   - No original module exists within this kernel
 - Installation
   - Installing to /lib/modules/6.14.0/updates/dkms/
depmod...
```
```shell
$ ls -l /usr/src/hw02_module-1.0/
total 16
-rw-r--r-- 1 root root  177 Feb  2 21:03 dkms.conf
-rw-r--r-- 1 root root 2619 Feb  2 21:03 hw02_module.c
-rw-r--r-- 1 root root 1955 Feb  2 21:03 hw02_module.mod.c
-rw-r--r-- 1 root root 1478 Feb  2 21:03 Makefile

$ cat /usr/src/hw02_module-1.0/dkms.conf 
PACKAGE_NAME="hw02_module"
PACKAGE_VERSION="1.0"
BUILT_MODULE_NAME[0]="hw02_module"
DEST_MODULE_LOCATION[0]="/updates"
AUTOINSTALL="yes"
MAKE[0]="make build"
CLEAN="make clean"
```
```
$ sudo make dkms-remove 
/usr/sbin/dkms remove "hw02_module/1.0"
Module hw02_module-1.0 for kernel 6.14.0 (x86_64).
Before uninstall, this module version was ACTIVE on this kernel.

hw02_module.ko:
 - Uninstallation
   - Deleting from: /lib/modules/6.14.0/updates/dkms/
 - Original module
   - No original module was found for this module on this kernel.
   - Use the dkms install command to reinstall any previous module version.
depmod...
Deleting module hw02_module-1.0 completely from the DKMS tree.
rm -rf /usr/src/hw02_module-1.0

$ ls -l /usr/src/hw02_module-1.0/
ls: cannot access '/usr/src/hw02_module-1.0/': No such file or directory
```
