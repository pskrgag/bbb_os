TOPDIR=../

include $(PWD)/Makefile.variables

MODULES_DIR=$(BUILD_DIR)/modules

model-y := main/main_module.o main/node.o main/system.o
obj-m := model.o

EXTRA_CFLAGS = -I $(PWD)/include

compile: $(MODULES_DIR)
	cp -r * $(MODULES_DIR)
	make ARCH=$(ARCH) CROSS_COMPILE=arm-linux-gnueabihf- -C $(BUILD_DIR)/packages/linux/ M=$(MODULES_DIR) modules

link: $(ROOTFS_DIR)/lib/modules
	@cp $(MODULES_DIR)/model.ko $(ROOTFS_DIR)/lib/modules

$(MODULES_DIR):
	mkdir -p $@

$(ROOTFS_DIR)/lib/modules:
	@mkdir -p $@
