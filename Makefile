# For building user should define O varible

include Makefile.variables

SUBDIRS = apps console libs modules packages

BUILD_DIR=$(O)/build

all: $(BUILD_DIR) $(SUBDIRS) $(CMDS)

$(CMDS):
	for dir in $(SUBDIRS); do \
		$(MAKE) -s -C $$dir $@ BUILD_DIR=$(BUILD_DIR) || exit; \
	done

$(BUILD_DIR):
	mkdir -p $@

.PHONY: $(CMDS) $(SUBDIRS) clean
.SILENT: $(CMDS)
