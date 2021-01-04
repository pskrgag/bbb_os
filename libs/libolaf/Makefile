TOPDIR=../../

CFLAGS += -I include -I $(TOPDIR)/include

SRCS = $(wildcard *.c)
LIB = libolaf.so

compile: $(LIBS_DIR)/$(LIB)

$(LIBS_DIR)/$(LIB): $(SRCS)
	@echo "LD 	$(LIBS_DIR)/$(LIB)"
	$(CC_HOST) $(CFLAGS) -shared -o $(LIBS_DIR)/$(LIB) $(SRCS)

link:

include $(TOPDIR)/Makefile.variables