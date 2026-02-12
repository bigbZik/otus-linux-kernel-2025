PWD := $(shell pwd)
KERNEL := $(shell uname -r)
KERNEL_DIR := /lib/modules/$(KERNEL)
KERNEL_BUILD_DIR := $(KERNEL_DIR)/build

MODULE_NAME := kernel_fifo
MODULE_VERSION := 1.0
TARGZ_NAME := vagin_$(MODULE_NAME)

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
FORMAT_FILES := $(PWD)/src/*.c $(PWD)/inc/*.h

.PHONY: build run remove format install uninstall clean dkms-install dkms-remove test targz

build:
	$(MAKE) -C $(KERNEL_BUILD_DIR) M=$(PWD) modules

run:
	insmod $(PWD)/$(MODULE_NAME).ko

remove:
	rmmod $(MODULE_NAME).ko

test:
	./checker/test.sh

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
	mkdir -p $(DKMS_DIR)/src
	mkdir -p $(DKMS_DIR)/inc
	cp src/*.c $(DKMS_DIR)/src
	cp inc/*.h $(DKMS_DIR)/inc
	cp Makefile $(DKMS_DIR)
	cp Kbuild $(DKMS_DIR)
	echo "$$DKMS_CONF" > $(DKMS_DIR)/dkms.conf
	/usr/sbin/dkms install "$(MODULE_NAME)/$(MODULE_VERSION)"
	
dkms-remove:
	/usr/sbin/dkms remove "$(MODULE_NAME)/$(MODULE_VERSION)"
	rm -rf $(DKMS_DIR)

targz:
	find . \
	\( -name "*.c" -o -name "*.h" -o -name "Makefile" -o -name "Kbuild" \) \
	-print | tar -czvf $(TARGZ_NAME).tar.gz -T -
