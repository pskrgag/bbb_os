# For building user should define O varible

include Makefile.variables

SUBDIRS = $(filter-out include/., $(wildcard */.))

BUILD_DIR=$(O)/build

all: $(SUBDIRS) $(CMDS)

$(CMDS):
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir $@ BUILD_DIR=$(BUILD_DIR); \
	done

.PHONY: $(CMDS) $(SUBDIRS)
.SILENT: $(CMDS)