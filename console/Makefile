TOPDIR=../

LFLAGS += -lstdc++

CONSOLE_DIR=$(BUILD_DIR)/console
SRCS = $(wildcard *.cpp)

all: $(CONSOLE _DIR) console

.PHONY: console build all

console: build
	make -C $(CONSOLE_DIR)

build:
	cmake -D CMAKE_CXX_COMPILER=$(CXX_HOST) -S . -B $(CONSOLE_DIR)

$(CONSOLE_DIR):
	mdkir -p $(CONSOLE_DIR)

include $(TOPDIR)/Makefile.variables
