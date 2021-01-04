TOPDIR = ../

include $(TOPDIR)/Makefile.variables

PACKAGES = linux busybox glibc dropbear
LOGS_DIR = $(PACKAGES_DIR)/logs

HOST = i686-pc-linux-gnu
LOADADDR = 0x82000000

IMAGE_NAME = image.img

compile: $(LOGS_DIR) $(PACKAGES_DIR) $(PACKAGES)
link: root initramfs main_image clean

.PHONY: root initramfs main_image

BUSYBOX = busybox-1.32.1
GLIBC = glibc-2.26
DROPBEAR = dropbear-2020.80

LINUX_URL = https://github.com/beagleboard/linux/archive/4.14.zip
BUSYBOX_USL = https://www.busybox.net/downloads/$(BUSYBOX).tar.bz2
GLIBC_URL = http://ftp.gnu.org/gnu/libc/$(GLIBC).tar.xz
DROPBEAR_URL = https://matt.ucc.asn.au/dropbear/$(DROPBEAR).tar.bz2

linux: $(LOGS_DIR)/linux.d
busybox: $(LOGS_DIR)/$(BUSYBOX).d
glibc: $(LOGS_DIR)/$(GLIBC).d
dropbear: $(LOGS_DIR)/$(DROPBEAR).d

$(LOGS_DIR)/linux.d: $(PACKAGES_DIR)/linux
	$(MAKE) -C $(PACKAGES_DIR)/linux					\
	ARCH=$(ARCH)								\
	CROSS_COMPILE=$(TARGET)-						\
	uImage									\
	dtbs									\
	LOADADDR=$(LOADADDR)							\
	-j10 >> $(LOGS_DIR)/linux.d 2>&1

# linux kernel

$(PACKAGES_DIR)/linux:
	@echo "Building linux..."
	@wget -q $(LINUX_URL) -O $(PACKAGES_DIR)/linux.zip
	@unzip $(PACKAGES_DIR)/linux.zip -d $(PACKAGES_DIR) > /dev/null 2>&1
	@mv $(PACKAGES_DIR)/linux-4.14 $(PACKAGES_DIR)/linux
	@cp linux.config $(PACKAGES_DIR)/linux/.config

# busybox

$(LOGS_DIR)/$(BUSYBOX).d: | $(PACKAGES_DIR)/$(BUSYBOX) $(ROOTFS_DIR)
	$(MAKE) -s -C $(PACKAGES_DIR)/$(BUSYBOX)				\
		ARCH=$(ARCH)							\
		CROSS_COMPILE=$(TARGET)-					\
		distclean							\
		>> $(LOGS_DIR)/$(BUSYBOX).d 2>&1

	$(MAKE) -s -C $(PACKAGES_DIR)/$(BUSYBOX)				\
		ARCH=$(ARCH)							\
		CROSS_COMPILE=$(TARGET)-					\
		defconfig							\
		>> $(LOGS_DIR)/$(BUSYBOX).d 2>&1

	$(MAKE) -s -C $(PACKAGES_DIR)/$(BUSYBOX)				\
		ARCH=$(ARCH)							\
		CROSS_COMPILE=$(TARGET)-					\
		-j10								\
		>> $(LOGS_DIR)/$(BUSYBOX).d 2>&1

	$(MAKE) -s -C $(PACKAGES_DIR)/$(BUSYBOX)				\
		ARCH=$(ARCH)							\
		CROSS_COMPILE=$(TARGET)-					\
		install								\
		CONFIG_PREFIX=$(PACKAGES_DIR)/rootfs				\
		>> $(LOGS_DIR)/$(BUSYBOX).d 2>&1

$(PACKAGES_DIR)/$(BUSYBOX):
	@echo "Building busybox..."
	@wget -q $(BUSYBOX_USL) -O $(PACKAGES_DIR)/busybox.tar.bz2
	@tar -xf $(PACKAGES_DIR)/busybox.tar.bz2 -C $(PACKAGES_DIR)

# glibc

$(LOGS_DIR)/$(GLIBC).d: $(PACKAGES_DIR)/$(GLIBC)
	@mkdir -p $(PACKAGES_DIR)/glibc_build
	@cd $(PACKAGES_DIR)/glibc_build;					\
	../$(GLIBC)/configure $(TARGET)						\
	--target=$(TARGET)							\
	--build=$(HOST)								\
	--prefix=								\
	--enable-add-ons							\
	--disable-nls								\
	--disable-multi-arch							\
	--disable-werror							\
	--disable-java								\
	--disable-native-java >> $(LOGS_DIR)/$(GLIBC).d 2>&1;		\
	$(MAKE) -j10 >> $(LOGS_DIR)/$(GLIBC).d 2>&1;			\
	$(MAKE) install install_root=$(ROOTFS_DIR)				\
	>> $(LOGS_DIR)/$(GLIBC).d 2>&1;

$(PACKAGES_DIR)/$(GLIBC):
	@echo "Building glibc..."
	@wget -q $(GLIBC_URL) -O $(PACKAGES_DIR)/glibc.tar.xz
	@tar -xf $(PACKAGES_DIR)/glibc.tar.xz -C $(PACKAGES_DIR)

# dropbear

$(LOGS_DIR)/$(DROPBEAR).d: | $(PACKAGES_DIR)/$(DROPBEAR)
	@cd $(PACKAGES_DIR)/$(DROPBEAR);					\
	./configure								\
	--host=$(TARGET)							\
	--disable-zlib								\
	--prefix=$(ROOTFS_DIR)							\
	CC=$(CC)								\
	>> $(LOGS_DIR)/$(DROPBEAR).d 2>&1

	$(MAKE) -C $(PACKAGES_DIR)/$(DROPBEAR)					\
	PROGRAMS="dropbear dropbearkey dbclient scp" -j10			\
	>> $(LOGS_DIR)/$(DROPBEAR).d 2>&1;

	$(MAKE) -C $(PACKAGES_DIR)/$(DROPBEAR)					\
	PROGRAMS="dropbear dropbearkey dbclient scp" install			\
	>> $(LOGS_DIR)/$(DROPBEAR).d 2>&1;

$(PACKAGES_DIR)/$(DROPBEAR):
	@echo "Building dropbear..."
	@wget -q $(DROPBEAR_URL) -O $(PACKAGES_DIR)/dropbear.tar.bz2
	@tar -xf $(PACKAGES_DIR)/dropbear.tar.bz2 -C $(PACKAGES_DIR)

main_image:
	@echo "Generating main image..."
	@mkimage -A $(ARCH)							\
	-T kernel								\
	-C none									\
	-a $(LOADADDR)								\
	-e $(LOADADDR)								\
	-n "Custom Image"							\
	-d $(PACKAGES_DIR)/linux/arch/arm/boot/zImage				\
	$(PACKAGES_DIR)/initramfs						\
	$(PACKAGES_DIR)/linux/arch/arm/boot/dts/am335x-boneblack.dtb		\
	$(PACKAGES_DIR)/$(IMAGE_NAME) >> /dev/null 2>&1;

root:
	@rsync -aq $(PWD)/rootfs/ $(ROOTFS_DIR)

initramfs:
	@echo "Generating initramfs..."
	@cd $(ROOTFS_DIR);								\
	find $(ROOTFS_DIR) | cpio --quiet -H newc -o > $(PACKAGES_DIR)/initramfs.cpio;	\
	cat $(PACKAGES_DIR)/initramfs.cpio | gzip > $(PACKAGES_DIR)/initramfs.gz

	@mkimage								\
	-A $(ARCH)								\
	-O Linux								\
	-T ramdisk								\
	-C none									\
	-a 0x8294f200								\
	-n "Root Filesystem"							\
	-d $(PACKAGES_DIR)/initramfs.gz 					\
	$(PACKAGES_DIR)/initramfs >> /dev/null 2>&1;				\

clean:
	@rm -f $(PACKAGES_DIR)/*.zip
	@rm -f $(PACKAGES_DIR)/*.bz2
	@rm -f $(PACKAGES_DIR)/*.xz
	@rm -f $(PACKAGES_DIR)/*.gz
	@rm -f $(PACKAGES_DIR)/*.cpio

$(ROOTFS_DIR):
	@mkdir -p $@

$(PACKAGES_DIR):
	@mkdir -p $@

$(LOGS_DIR):
	@mkdir -p $@
