TOPDIR=../..
CC = arm-linux-gnueabihf-gcc
CFLAGS += -I $(TOPDIR)/include
LFLAGS += -lpthread

ifdef DEBUG
CFLAGS += -D DEBUG
endif

SRCS = $(wildcard *.c)
OUT = $(patsubst %.c,%,$(SRCS))

all: $(OUT)
%: %.c
	$(CC) -o $@ $^ $(CFLAGS) $(LFLAGS)

.PHONY: clean
clean:
	rm ${TARGETS}
