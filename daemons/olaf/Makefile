TOPDIR=../..

CFLAGS += -I $(TOPDIR)/include
LFLAGS += -lpthread

ifdef DEBUG
CFLAGS += -D DEBUG
endif

SRCS = $(wildcard *.c)
OUT = $(patsubst %.c,%,$(SRCS))

OLAF_DIR = $(DAEMONS_DIR)/olaf

all: $(OLAF_DIR) $(OUT)
%: %.c
	@echo $(CC)
	$(CC) -o $(OLAF_DIR)/$@ $^ $(CFLAGS) $(LFLAGS)

$(OLAF_DIR):
	mkdir -p $(OLAF_DIR)


.PHONY: clean
clean:
	rm ${TARGETS}

include $(TOPDIR)/Makefile.variables