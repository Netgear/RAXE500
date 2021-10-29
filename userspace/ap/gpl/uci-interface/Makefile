#
#  Makefile for application compiling under uClinux
#  Author: kaiyen.chang@foxconn.com
#

include ../config.mk
include ../config.in

#################################################
# Set the executive file name
#################################################
#TARGET = module-static
TARGET = module-shared


#################################################
# Set the include directory
# Example:
#   INCLUDEDIRS += -I./include -I./vendor_include
#################################################
INCLUDEDIRS += -I$(TARGETDIR)/usr/include


#################################################
# Set the compiling and linking flags
# Example:
#   CFLAGS += -DHELLO_FLAG -DHELLO_VENDOR
#################################################
CFLAGS  +=  -Wall -Wunused -DUSE_NFS #-DDEBUG -Werror -fPIC
ifeq ($(HW_PLATFORM),APDK01)
CFLAGS  += -DOPENWRT
endif

FPIC=-fPIC
LIBS=-lc


SOURCES      = $(wildcard *.c)
OBJECTS      = $(patsubst %.c,%.o,$(SOURCES))
DEPENDENCIES = $(SOURCES:.c=.d)


AS      = $(CROSS_COMPILE)as
LD      = $(CROSS_COMPILE)ld
CC      = $(CROSS_COMPILE)gcc
CPP     = $(CC) -E
AR      = $(CROSS_COMPILE)ar
NM      = $(CROSS_COMPILE)nm
STRIP   = $(CROSS_COMPILE)strip
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
RANLIB  = $(CROSS_COMPILE)ranlib

VERSION=1.0

SHLIB_FILE=libuci-interface.so.$(VERSION)
SHLIB_FLAGS = -shared -Wl,-soname,$(SHLIB_FILE)


.PHONY: all install clean libuci

all: libuci $(TARGET)


module-static: module.o libuci-interface.a $(UCI_DIR)/libuci.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

module.o: module.c
	$(CC) $(CFLAGS) -c -o $@ $< $(INCLUDEDIRS)

libuci-interface-static.o: libuci-interface.c $(LIBUCI_DEPS)
	$(CC) $(CFLAGS) $(INCLUDEDIRS) -c -o $@ $<

libuci-interface.a: libuci-interface-static.o
	$(RM) -f $@
	$(AR) rc $@ $^
	$(RANLIB) $@

module-shared: module.o libuci-interface.so
	$(CC) -o $@ $< -L./ -luci-interface -L$(TARGETDIR)/usr/lib -luci

libuci-interface-shared.o: libuci-interface.c $(LIBUCI_DEPS)
	$(CC) $(CFLAGS) $(FPIC) $(INCLUDEDIRS) -c -o $@ $<

libuci-interface.so: libuci-interface-shared.o
	$(CC) $(SHLIB_FLAGS) -o $(SHLIB_FILE) $^ $(LIBS)
	ln -sf $(SHLIB_FILE) $@

%.d: %.c
	@set -e; $(CC) -MM $(CPPFLAGS) $(CFLAGS) $(INCLUDEDIRS) $< \
	| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
	[ -s $@ ] || $(RM) -f $@
	@echo "	Generate the dependency file: $@"

#-include $(DEPENDENCIES)

#.c.o:
#	$(CC) $(CFLAGS) $(INCLUDEDIRS) -c $*.c

libuci:
	@if [ ! -f $(TARGETDIR)/usr/lib/libuci.so ]; then \
		cd $(GPLTOPDIR)/uci && make install; \
	fi

install: libuci $(TARGET)
	$(STRIP) $(TARGET)
	install -D -m 0755 ./$(TARGET) $(TARGETDIR)/usr/sbin/$(TARGET)
	install -D -m 0644 ./$(SHLIB_FILE) $(TARGETDIR)/usr/lib/$(SHLIB_FILE)
	ln -sf $(TARGETDIR)/usr/lib/$(SHLIB_FILE) $(TARGETDIR)/usr/lib/libuci-interface.so
	install -D -m 0644 ./uci-interface.h $(TARGETDIR)/usr/include/uci-interface.h

clean:
	$(RM) $(TARGET) $(OBJECTS) $(DEPENDENCIES) $(SOURCES:.c=.gdb) $(TARGET).gdb *.[oa] *.so*
	$(RM) $(TARGETDIR)/usr/sbin/$(TARGET)
	$(RM) $(TARGETDIR)/usr/lib/$(SHLIB_FILE)
	$(RM) $(TARGETDIR)/usr/lib/libuci-interface.so
	$(RM) $(TARGETDIR)/usr/include/uci-interface.h

