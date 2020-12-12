TOPDIR=../../

CFLAGS += -I $(TOPDIR)/include

SRCS = $(wildcard *.c)
OUT = libolaf.so

compile:
	@echo "LD 	$(LIBS_DIR)/$(OUT)"
	$(CC_HOST) $(CFLAGS) -shared -o $(LIBS_DIR)/$(OUT) $(SRCS)

link:


include $(TOPDIR)/Makefile.variables