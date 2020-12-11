SUBDIRS := $(wildcard */.)

LIBS_DIR=$(BUILD_DIR)/libs

all: $(LIBS_DIR) $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@ LIBS_DIR=$(LIBS_DIR) LIBS_DIR=$(LIBS_DIR)

$(LIBS_DIR):
	mkdir -p $(LIBS_DIR)

.PHONY: all $(SUBDIRS)