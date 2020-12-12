TOPDIR=../..

CFLAGS += -I $(TOPDIR)/include
LFLAGS += -lpthread

SRCS = $(wildcard *.c)
OUT = olafd

OLAF_DIR = $(DAEMONS_DIR)/olaf

compile: $(OLAF_DIR) $(OLAF_DIR)/$(OUT).o

$(OLAF_DIR)/$(OUT).o: $(SRCS)
	@echo "CC	$@"
	$(CC) $(CFLAGS) -c $(SRCS) -o $@

$(OLAF_DIR):
	mkdir -p $(OLAF_DIR)

link: $(OLAF_DIR)/$(OUT)

$(OLAF_DIR)/$(OUT):
	@echo "LD	$@"
	$(CC) $(OLAF_DIR)/$(OUT).o -o $(OLAF_DIR)/$(OUT) $(LFLAGS)


.PHONY: clean


include $(TOPDIR)/Makefile.variables
