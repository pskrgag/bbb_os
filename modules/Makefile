TOPDIR=../

include $(PWD)/Makefile.variables

.PHONY: install

MODULES_DIR = $(BUILD_DIR)/modules
LINUX_SOURCE_DIR = $(PACKAGES_DIR)/linux/

model-y := main/main_module.o main/node.o main/system.o
hardware-y := hardware/pd_driver_dt.o
obj-m := model.o hardware.o

EXTRA_CFLAGS = -I $(PWD)/include

compile: $(MODULES_DIR) $(LINUX_SOURCE_DIR)
	cp -r * $(MODULES_DIR)
	$(MAKE) C=1 ARCH=$(ARCH) CROSS_COMPILE=arm-linux-gnueabihf- -C $(LINUX_SOURCE_DIR) M=$(MODULES_DIR) modules

link: install

install:
	$(MAKE) INSTALL_MOD_PATH=$(ROOTFS_DIR) -C $(LINUX_SOURCE_DIR) \
	M=$(MODULES_DIR) modules_install

$(MODULES_DIR):
	mkdir -p $@

$(ROOTFS_DIR)/lib/modules:
	@mkdir -p $@
