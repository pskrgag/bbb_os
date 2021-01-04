TOPDIR=../

SUBDIRS := $(wildcard */.)

include $(TOPDIR)/Makefile.variables

LIBS_DIR=$(BUILD_DIR)/libs

$(CMDS): $(LIBS_DIR)
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir $@ LIBS_DIR=$(LIBS_DIR) $@; \
	done

$(LIBS_DIR):
	mkdir -p $@

.PHONY: all $(SUBDIRS)
