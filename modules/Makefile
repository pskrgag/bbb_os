TOPDIR=../

model-y := main/main_module.o
obj-m := model.o

MODULES_DIR=$(BUILD_DIR)/modules

compile: $(MODULES_DIR)
	cp -r * $(MODULES_DIR)
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(BUILD_DIR)/packages/linux/ M=$(MODULES_DIR) modules

link:

$(MODULES_DIR):
	mkdir -p $@

