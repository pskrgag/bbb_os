TOPDIR=../../

CFLAGS += -I $(TOPDIR)/include

SRCS = $(wildcard *.c)
OUT = libolaf.so

all: $(OUT)

$(OUT):
	$(CC_HOST) $(CFLAGS) -shared -o $(LIBS_DIR)/$@ $(SRCS)

include $(TOPDIR)/Makefile.variables