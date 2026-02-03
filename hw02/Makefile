PWD := $(shell pwd)
KERNEL := $(shell uname -r)
KERNEL_DIR := /lib/modules/$(KERNEL)
KERNEL_BUILD_DIR := $(KERNEL_DIR)/build

MODULE_NAME := hw02_module
MODULE_VERSION := 1.0

DKMS_DIR := /usr/src/$(MODULE_NAME)-$(MODULE_VERSION)
define DKMS_CONF
PACKAGE_NAME="$(MODULE_NAME)"
PACKAGE_VERSION="$(MODULE_VERSION)"
BUILT_MODULE_NAME[0]="$(MODULE_NAME)"
DEST_MODULE_LOCATION[0]="/updates"
AUTOINSTALL="yes"
MAKE[0]="make build"
CLEAN="make clean"
endef
export DKMS_CONF

CLANG_FORMAT_VERS ?= 18
CLANG_FORMAT := clang-format-$(CLANG_FORMAT_VERS)
CLANG_FORMAT_FLAGS += -i
FORMAT_FILES := $(PWD)/*.c

obj-m := $(MODULE_NAME).o

.PHONY: build run remove test format install uninstall clean dkms-install dkms-remove

build:
	$(MAKE) -C $(KERNEL_BUILD_DIR) M=$(PWD) modules

run:
	insmod $(PWD)/$(MODULE_NAME).ko

remove:
	rmmod $(MODULE_NAME).ko

test:
	python3 checker/main.py $(MODULE_NAME)

format:
	$(CLANG_FORMAT) $(CLANG_FORMAT_FLAGS) $(FORMAT_FILES)

install:
	cp $(MODULE_NAME).ko $(KERNEL_DIR)
	/sbin/depmod -a
	/sbin/modprobe $(MODULE_NAME)

uninstall:
	/sbin/modprobe -r $(MODULE_NAME)
	rm -f $(KERNEL_DIR)/$(MODULE_NAME).ko
	/sbin/depmod -a

clean:
	$(MAKE) -C $(KERNEL_BUILD_DIR) M=$(PWD) clean

dkms-install:
	mkdir -p $(DKMS_DIR)
	cp *.c $(DKMS_DIR)
	cp Makefile $(DKMS_DIR)
	echo "$$DKMS_CONF" > $(DKMS_DIR)/dkms.conf
	/usr/sbin/dkms install "$(MODULE_NAME)/$(MODULE_VERSION)"
	
dkms-remove:
	/usr/sbin/dkms remove "$(MODULE_NAME)/$(MODULE_VERSION)"
	rm -rf $(DKMS_DIR)
