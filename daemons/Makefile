TOPDIR=../

include $(TOPDIR)/Makefile.variables

SUBDIRS := $(wildcard */.)

DAEMONS_DIR=$(BUILD_DIR)/daemons

$(CMDS):
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir $@ DAEMONS_DIR=$(DAEMONS_DIR); \
	done

.PHONY: all $(SUBDIRS)