TOPDIR=../

LFLAGS += -lstdc++

CONSOLE_DIR=$(BUILD_DIR)/console
SRCS = $(wildcard *.cpp)

link:
	$(MAKE) --silent -C $(CONSOLE_DIR)

compile: $(CONSOLE_DIR)
	@echo "CMAKE 	console"
	cmake -quiet -D CMAKE_CXX_COMPILER=$(CXX_HOST) -S . -B $(CONSOLE_DIR) 1>2 /dev/null

$(CONSOLE_DIR):
	mkdir -p $@

include $(TOPDIR)/Makefile.variables
