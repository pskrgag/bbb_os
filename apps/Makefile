TOPDIR=../

include $(TOPDIR)/Makefile.variables

SUBDIRS := $(wildcard */.)

APPS_DIR=$(BUILD_DIR)/apps

$(CMDS):
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir $@ APPS_DIR=$(APPS_DIR); \
	done

.PHONY: all $(SUBDIRS)
