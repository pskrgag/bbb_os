TOPDIR = ../

PACKAGES = linux busybox glibc
PACKAGES_DIR = $(BUILD_DIR)/packages
LOGS_DIR = $(PACKAGES_DIR)/logs
ROOTFS_DIR = $(PACKAGES_DIR)/rootfs

HOST = i686-pc-linux-gnu

compile: $(LOGS_DIR) $(PACKAGES_DIR) $(PACKAGES) clean
link:

BUSYBOX = busybox-1.32.1
GLIBC = glibc-2.24

LINUX_URL = https://github.com/beagleboard/linux/archive/4.14.zip
BUSYBOX_USL = https://www.busybox.net/downloads/$(BUSYBOX).tar.bz2
GLIBC_URL = http://ftp.gnu.org/gnu/libc/$(GLIBC).tar.xz

linux: $(PACKAGES_DIR)/logs/linux.d
busybox: $(PACKAGES_DIR)/logs/$(BUSYBOX).d
glibc: $(PACKAGES_DIR)/logs/$(GLIBC).d

$(PACKAGES_DIR)/logs/linux.d: $(PACKAGES_DIR)/linux

# linux kernel

$(PACKAGES_DIR)/linux:
	@echo "Downloading linux..."
	@wget -q $(LINUX_URL) -O $(PACKAGES_DIR)/linux.zip
	@unzip $(PACKAGES_DIR)/linux.zip -d $(PACKAGES_DIR) > /dev/null 2>&1
	@mv $(PACKAGES_DIR)/linux-4.14 $(PACKAGES_DIR)/linux
	@cp linux.config $(PACKAGES_DIR)/linux/.config

# busybox

$(PACKAGES_DIR)/logs/$(BUSYBOX).d: $(PACKAGES_DIR)/$(BUSYBOX) $(ROOTFS_DIR)
	$(MAKE) -s -C $(PACKAGES_DIR)/$(BUSYBOX)				\
		ARCH=$(ARCH)							\
		CROSS_COMPILE=$(TARGET)-					\
		distclean

	$(MAKE) -s -C $(PACKAGES_DIR)/$(BUSYBOX)				\
		ARCH=$(ARCH)							\
		CROSS_COMPILE=$(TARGET)-					\
		defconfig

	$(MAKE) -s -C $(PACKAGES_DIR)/$(BUSYBOX)				\
		ARCH=$(ARCH)							\
		CROSS_COMPILE=$(TARGET)-					\
		-j10 > $(PACKAGES_DIR)/logs/$(BUSYBOX).d

	$(MAKE) -s -C $(PACKAGES_DIR)/$(BUSYBOX)				\
		ARCH=$(ARCH)							\
		CROSS_COMPILE=$(TARGET)-					\
		install								\
		CONFIG_PREFIX=$(PACKAGES_DIR)/rootfs				\
		>> $(PACKAGES_DIR)/logs/$(BUSYBOX).d

$(PACKAGES_DIR)/$(BUSYBOX):
	@echo "Downloading busybox..."
	@wget -q $(BUSYBOX_USL) -O $(PACKAGES_DIR)/busybox.tar.bz2
	@tar -xf $(PACKAGES_DIR)/busybox.tar.bz2 -C $(PACKAGES_DIR)

# glibc

$(PACKAGES_DIR)/logs/$(GLIBC).d: $(PACKAGES_DIR)/$(GLIBC)
	@mkdir -p $(PACKAGES_DIR)/glibc_build
	cd $(PACKAGES_DIR)/glibc_build;						\
	../$(GLIBC)/configure $(TARGET)						\
	--target=$(TARGET)							\
	--build=$(HOST)								\
	--prefix=								\
	--enable-add-ons;							\
	$(MAKE) -j10;								\
	$(MAKE) install install_root=$(ROOTFS_DIR)				\
	> $(PACKAGES_DIR)/logs/$(GLIBC).d;					\

$(PACKAGES_DIR)/$(GLIBC):
	@echo "Downloading glibc..."
	@wget -q $(GLIBC_URL) -O $(PACKAGES_DIR)/glibc.tar.xz
	@tar -xf $(PACKAGES_DIR)/glibc.tar.xz -C $(PACKAGES_DIR)

clean:
	@rm -f $(PACKAGES_DIR)/*.zip
	@rm -f $(PACKAGES_DIR)/*.bz2
	@rm -f $(PACKAGES_DIR)/*.xz

$(ROOTFS_DIR):
	@mkdir -p $@

$(PACKAGES_DIR):
	@mkdir -p $@

$(LOGS_DIR):
	@mkdir -p $@

include $(TOPDIR)/Makefile.variables
