#
#  Makefile for application compiling under uClinux
#  Author: jack.tseng@foxconn.com
#

include ../config.mk
include ../config.in

#################################################
# Set the executive file name
#################################################
TARGET = uci


#################################################
# Set the include directory
# Example:
#   INCLUDEDIRS += -I./include -I./vendor_include
#################################################
TOP_DIR = $(shell pwd)
INCLUDEDIRS += -I$(TOP_DIR)


#################################################
# Set the include directory
# Example:
#   CFLAGS += -DHELLO_FLAG -DHELLO_VENDOR
#################################################
#COPTS=-O2
#WOPTS=-pedantic -Werror -Wall
#CFLAGS=$(COPTS) $(WOPTS) -std=gnu99
CFLAGS += -Werror -Wall -DUSE_NFS # -fPIC
FPIC=-fPIC
LIBS=-lc


SOURCES      = $(wildcard *.c)
OBJECTS      = $(patsubst %.c,%.o,$(SOURCES))
DEPENDENCIES = $(SOURCES:.c=.d)


AS           = $(CROSS_COMPILE)as
LD           = $(CROSS_COMPILE)ld
CC           = $(CROSS_COMPILE)gcc
CPP          = $(CC) -E
AR           = $(CROSS_COMPILE)ar
NM           = $(CROSS_COMPILE)nm
STRIP        = $(CROSS_COMPILE)strip
OBJCOPY      = $(CROSS_COMPILE)objcopy
OBJDUMP      = $(CROSS_COMPILE)objdump
RANLIB       = $(CROSS_COMPILE)ranlib

PLUGIN_SUPPORT=1
DEBUG=0
DEBUG_TYPECAST=0
VERSION=0.7

define add_feature
	@echo "$(if $(findstring 1,$($(1))),#define UCI_$(1) 1,#undef UCI_$(1))" >> $@.tmp
endef

LIBUCI_DEPS = file.c history.c list.c util.c uci.h uci_config.h uci_internal.h
SHLIB_FILE=libuci.so.$(VERSION)
SHLIB_FLAGS = -shared -Wl,-soname,$(SHLIB_FILE)

.PHONY: all install clean uci uci-static


all: uci uci-static


cli.o: cli.c uci.h uci_config.h

uci_config.h: FORCE
	@$(RM) -f "$@.tmp"
	$(call add_feature,PLUGIN_SUPPORT)
	$(call add_feature,DEBUG)
	$(call add_feature,DEBUG_TYPECAST)
	@if [ \! -f "$@" ] || ! cmp "$@.tmp" "$@" >/dev/null; then \
		mv "$@.tmp" "$@"; \
	else \
		$(RM) -f "$@.tmp"; \
	fi

uci-static: cli.o libuci.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

libuci-static.o: libuci.c $(LIBUCI_DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

libuci.a: libuci-static.o
	$(RM) -f $@
	$(AR) rc $@ $^
	$(RANLIB) $@

uci: cli.o libuci.so
	$(CC) -o $@ $< -L. -luci

libuci-shared.o: libuci.c $(LIBUCI_DEPS)
	$(CC) $(CFLAGS) $(FPIC) -c -o $@ $<

libuci.so: libuci-shared.o
	$(CC) $(SHLIB_FLAGS) -o $(SHLIB_FILE) $^ $(LIBS)
	ln -sf $(SHLIB_FILE) $@

#libucimap.a: ucimap.o
#	$(RM) -f $@
#	$(AR) rc $@ $^
#	$(RANLIB) $@

%.d: %.c
	@set -e; $(CC) -MM $(CPPFLAGS) $(CFLAGS) $(INCLUDEDIRS) $< \
	| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
	[ -s $@ ] || $(RM) -f $@
	@echo "	Generate the dependency file: $@"

#-include $(DEPENDENCIES)

#.c.o:
#	$(CC) $(CFLAGS) $(INCLUDEDIRS) -c $*.c


install: $(TARGET)
	install -d $(TARGETDIR)/usr/sbin
	install -D -m 0755 $(TARGET) $(TARGETDIR)/usr/sbin
	$(STRIP) $(TARGETDIR)/usr/sbin/$(TARGET)
	install -D -m 0644 $(SHLIB_FILE) $(TARGETDIR)/usr/lib/$(SHLIB_FILE)
	ln -sf $(TARGETDIR)/usr/lib/$(SHLIB_FILE) $(TARGETDIR)/usr/lib/libuci.so
	install -D -m 0644 uci.h $(TARGETDIR)/usr/include/uci.h
	install -D -m 0644 uci_config.h $(TARGETDIR)/usr/include/uci_config.h

clean:
	$(RM) $(TARGET) uci-static $(OBJECTS) $(DEPENDENCIES) $(SOURCES:.c=.gdb) $(TARGET).gdb *.[oa] *.so* uci_config.h
	$(RM) $(TARGETDIR)/usr/sbin/$(TARGET)
	$(RM) $(TARGETDIR)/usr/lib/$(SHLIB_FILE)
	$(RM) $(TARGETDIR)/usr/lib/libuci.so
	$(RM) $(TARGETDIR)/usr/include/uci.h
	$(RM) $(TARGETDIR)/usr/include/uci_config.h
	$(RM) $(TARGETDIR)/sbin/$(TARGET)

FORCE: ;
.PHONY: FORCE
