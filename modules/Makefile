TOPDIR=../

include $(PWD)/Makefile.variables

.PHONY: install

MODULES_DIR = $(BUILD_DIR)/modules
LINUX_SOURCE_DIR = $(BUILD_DIR)/packages/linux/

model-y := main/main_module.o main/node.o main/system.o
obj-m := model.o

EXTRA_CFLAGS = -I $(PWD)/include

compile: $(MODULES_DIR)
	cp -r * $(MODULES_DIR)
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=arm-linux-gnueabihf- -C $(LINUX_SOURCE_DIR) M=$(MODULES_DIR) modules

link: install

install:
	$(MAKE) INSTALL_MOD_PATH=$(ROOTFS_DIR) -C $(LINUX_SOURCE_DIR) \
	M=$(MODULES_DIR) modules_install

$(MODULES_DIR):
	mkdir -p $@

$(ROOTFS_DIR)/lib/modules:
	@mkdir -p $@
