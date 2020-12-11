# For building user should define O varible

SUBDIRS := $(wildcard */.)

BUILD_DIR=$(O)/build

all: $(BUILD_DIR) $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@ BUILD_DIR=$(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

.PHONY: all $(SUBDIRS)
