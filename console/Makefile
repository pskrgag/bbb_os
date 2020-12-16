TOPDIR=../

LFLAGS += -lstdc++

CONSOLE_DIR=$(BUILD_DIR)/console
SRCS = $(wildcard *.cpp)

link:
	$(MAKE) --silent -C $(CONSOLE_DIR)

compile: $(CONSOLE_DIR)
	@echo "CMAKE 	$(CONSOLE_DIR)/console"
	INCLUDE_LIBRARY_PATH=$(TOPDIR)/include LD_LIBRARY_PATH=$(CONSOLE_DIR)/../libs cmake -DCMAKE_CXX_COMPILER=$(CXX_HOST) -DCMAKE_BUILD_TYPE=Release -S . -B $(CONSOLE_DIR) >/dev/null 2>&1

$(CONSOLE_DIR):
	mkdir -p $@

include $(TOPDIR)/Makefile.variables
