SUBDIRS := $(wildcard */.)

DAEMONS_DIR=$(BUILD_DIR)/daemons

all: $(DAEMONS_DIR) $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@ DAEMONS_DIR=$(DAEMONS_DIR)

$(DAEMONS_DIR):
	mkdir -p $(DAEMONS_DIR)

.PHONY: all $(SUBDIRS)