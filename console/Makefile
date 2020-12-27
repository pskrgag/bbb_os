TOPDIR=../

LFLAGS += -lstdc++

CONSOLE_DIR=$(BUILD_DIR)/console
SRCS = $(wildcard *.cpp)

CONSOLE_FLAGS = -I $(PWD)/include -I $(PWD)/libs/libolaf/include -L $(CONSOLE_DIR)/../libs
BUILD_TYPE=Release

link:
	$(MAKE) --silent -C $(CONSOLE_DIR)

compile: $(CONSOLE_DIR)
	@echo "CMAKE 	$(CONSOLE_DIR)/console"
	cmake -DCMAKE_CXX_FLAGS="$(CONSOLE_FLAGS)" -DCMAKE_CXX_COMPILER=$(CXX_HOST) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -S . -B $(CONSOLE_DIR) >/dev/null 2>&1

$(CONSOLE_DIR):
	mkdir -p $@

include $(TOPDIR)/Makefile.variables
