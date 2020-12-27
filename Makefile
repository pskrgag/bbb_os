# For building user should define O varible

include Makefile.variables

SUBDIRS = $(filter-out include/., $(wildcard */.))

BUILD_DIR=$(O)/build

all: $(BUILD_DIR) $(SUBDIRS) $(CMDS)

$(CMDS):
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir $@ BUILD_DIR=$(BUILD_DIR); \
	done

$(BUILD_DIR):
	mkdir -p $@

.PHONY: $(CMDS) $(SUBDIRS) clean
.SILENT: $(CMDS)
