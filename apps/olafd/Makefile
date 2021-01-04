TOPDIR=../..

include $(TOPDIR)/Makefile.variables

CFLAGS += -I $(TOPDIR)/include
LFLAGS += -lpthread

SRCS = $(wildcard *.c)
HEADERS = $(wildcard *.h)
OUT = olafd

OLAF_DIR = $(APPS_DIR)/olafd

compile: $(OLAF_DIR) $(OLAF_DIR)/$(OUT).o

$(OLAF_DIR)/$(OUT).o: $(SRCS) $(HEADERS)
	@echo "CC	$@"
	$(CC) $(CFLAGS) -c $(SRCS) -o $@

$(OLAF_DIR):
	mkdir -p $(OLAF_DIR)

link: $(OLAF_DIR)/$(OUT) install

$(OLAF_DIR)/$(OUT): $(OLAF_DIR)/$(OUT).o
	@echo "LD	$@"
	$(CC) $(OLAF_DIR)/$(OUT).o -o $(OLAF_DIR)/$(OUT) $(LFLAGS)

install:
	@cp $(OLAF_DIR)/$(OUT) $(ROOTFS_DIR)/bin

.PHONY: clean install
