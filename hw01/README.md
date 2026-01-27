## ДЗ01. Компиляция ядра Linux

Выполнено для Ubuntu 24.03
```shell
$ lsb_release -a
No LSB modules are available.
Distributor ID:	Ubuntu
Description:	Ubuntu 24.04.3 LTS
Release:	24.04
Codename:	noble
```

Оригинальное ядро:       
```
Linux bzkUbuntu24 6.14.0-37-generic #37~24.04.1-Ubuntu SMP PREEMPT_DYNAMIC Thu Nov 20 10:25:38 UTC 2 x86_64 x86_64 x86_64 GNU/Linux
```

### Выполненные шаги
1. Скачивание исходных кодов. Распаковка.
2. Создание config файла на основе текщей системы (VM) через `make localmodconfig`.
3. Отключение и включение указанных опций в config файле через `make menuconfig`. Проверка через текстовый редактор.
4. Компиляция ядра `make -j(nproc) 2>errors.log`
5. Упаковка результата в deb пакеты: `make bindeb-pkg`
```shell
$ ls -l ~/dev/linux-*6.14*.deb
-rw-r--r-- 1 bzk bzk   9285316 Jan 27 11:13 /home/bzk/dev/linux-headers-6.14.0_6.14.0-4_amd64.deb
-rw-r--r-- 1 bzk bzk  22344838 Jan 27 11:11 /home/bzk/dev/linux-image-6.14.0_6.14.0-4_amd64.deb
-rw-r--r-- 1 bzk bzk 146594818 Jan 27 11:13 /home/bzk/dev/linux-image-6.14.0-dbg_6.14.0-4_amd64.deb
-rw-r--r-- 1 bzk bzk   1457612 Jan 27 11:13 /home/bzk/dev/linux-libc-dev_6.14.0-4_amd64.deb
```
6. Установка полученного пакета: `sudo dpkg -i linux-image-6.14.0_6.14.0-4_amd64.deb`
```shell
$ ls -l /boot/
total 149976
-rw-r--r-- 1 root root   178742 Jan 27 11:09 config-6.14.0
-rw-r--r-- 1 root root   296189 Nov 20 00:36 config-6.14.0-37-generic
drwxr-xr-x 3 root root     4096 Dec 31  1969 efi
drwxr-xr-x 5 root root     4096 Jan 27 12:09 grub
lrwxrwxrwx 1 root root       28 Jan 25 11:43 initrd.img -> initrd.img-6.14.0-37-generic
-rw-r--r-- 1 root root 29045038 Jan 27 11:57 initrd.img-6.14.0
-rw-r--r-- 1 root root 73107108 Jan 25 11:47 initrd.img-6.14.0-37-generic
lrwxrwxrwx 1 root root       28 Jan 25 11:43 initrd.img.old -> initrd.img-6.14.0-37-generic
-rw-r--r-- 1 root root   142796 Apr  8  2024 memtest86+ia32.bin
-rw-r--r-- 1 root root   143872 Apr  8  2024 memtest86+ia32.efi
-rw-r--r-- 1 root root   147744 Apr  8  2024 memtest86+x64.bin
-rw-r--r-- 1 root root   148992 Apr  8  2024 memtest86+x64.efi
-rw-r--r-- 1 root root  5669664 Jan 27 11:09 System.map-6.14.0
-rw------- 1 root root  9159323 Nov 20 00:36 System.map-6.14.0-37-generic
lrwxrwxrwx 1 root root       25 Jan 25 11:43 vmlinuz -> vmlinuz-6.14.0-37-generic
-rw-r--r-- 1 root root 19931648 Jan 27 11:09 vmlinuz-6.14.0
-rw------- 1 root root 15571336 Nov 20 00:38 vmlinuz-6.14.0-37-generic
lrwxrwxrwx 1 root root       25 Jan 25 11:43 vmlinuz.old -> vmlinuz-6.14.0-37-generic
```

7. Установка полученного ядра для загрузки по умолчанию в grub:
```shell
sudo grub-set-default "Advanced options for Ubuntu>Ubuntu, with Linux 6.14.0"
sudo update-grub
```

8. Перезагрузка. Проверка ядра:
```
$ uname -a
Linux bzkUbuntu24 6.14.0 #4 SMP PREEMPT_DYNAMIC Tue Jan 27 11:10:10 PST 2026 x86_64 x86_64 x86_64 GNU/Linux
```
