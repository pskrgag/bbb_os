TOPDIR = ../

PACKAGES = linux
PACKAGES_DIR = $(BUILD_DIR)/packages
LOGS_DIR = $(PACKAGES_DIR)/logs

compile: $(LOGS_DIR) $(PACKAGES_DIR) $(PACKAGES) clean
link:

# linux kernel

LINUX_URL = https://github.com/beagleboard/linux/archive/4.14.zip

linux: $(PACKAGES_DIR)/logs/linux.d

$(PACKAGES_DIR)/logs/linux.d: $(PACKAGES_DIR)/linux


$(PACKAGES_DIR)/linux:
	@echo "Downloading linux..."
	wget $(LINUX_URL) -O $(PACKAGES_DIR)/linux.zip > /dev/null 2>&1
	unzip $(PACKAGES_DIR)/linux.zip -d $(PACKAGES_DIR) > /dev/null 2>&1
	mv $(PACKAGES_DIR)/linux-4.14 $(PACKAGES_DIR)/linux
	cp linux.config $(PACKAGES_DIR)/linux/.config

clean:
	rm -f $(PACKAGES_DIR)/*.zip


$(PACKAGES_DIR):
	mkdir -p $@

$(LOGS_DIR):
	mkdir -p $@

include $(TOPDIR)/Makefile.variables
