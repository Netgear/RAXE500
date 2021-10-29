#
# Linux NVRAM Makefile
#
# Copyright (C) 2020, Broadcom. All Rights Reserved.
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
# SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
# OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
# CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#
# $Id: Makefile 692019 2017-03-24 19:24:28Z $
#

include $(TOP)/.config

CFLAGS	+= -I. -I$(SRCBASE)/include ${WLAN_StdIncPathA} -I$(SRCBASE)/../components/shared -I$(SRCBASE)/../components/wlioctl/include -I$(SRCBASE)/../components/proto/include -I$(SRCBASE)/common/include -Wall -fPIC
#CFLAGS	+= -g -DDEBUG
CFLAGS	+= -s
ifeq ($(CONFIG_RTR_OPTIMIZE_SIZE),y)
CFLAGS  += -Os
else
CFLAGS  += -O2
endif
LDFLAGS += -L.

ifeq ($(CONFIG_NVRAMD),y)
OBJ = nvram_shared.o
CFLAGS += -I$(SRCBASE)/../components/router/shared
else
OBJ = nvram_linux.o
endif

vpath %.c $(SRCBASE)/shared/nvram

ifneq ($(strip $(BCA_HNDROUTER)),)
all:
	@true
else
all: libnvram.so $(if $(CONFIG_NVRAM_UTILITY),nvram) $(if $(CONFIG_NVRAMD),nvramd)
endif

install: all
ifneq ($(strip $(BCA_HNDROUTER)),)
	@true
else
	install -D libnvram.so $(INSTALLDIR)/usr/lib/libnvram.so
	$(STRIP) $(INSTALLDIR)/usr/lib/libnvram.so
ifneq ($(CONFIG_NVRAM_UTILITY),)
	install -D nvram $(INSTALLDIR)/usr/sbin/nvram
	$(STRIP) $(INSTALLDIR)/usr/sbin/nvram
else
	# Utility not wanted: get rid of any leftover one
	rm -f $(INSTALLDIR)/usr/sbin/nvram
endif
endif
ifneq ($(CONFIG_NVRAMD),)
	install -D nvramd $(INSTALLDIR)/usr/sbin/nvramd
	$(STRIP) $(INSTALLDIR)/usr/sbin/nvramd
endif

clean:
	rm -f *.o *.a *.so nvram nvramd

libnvram.so: ${OBJ}
	$(LD) $(LDFLAGS) -shared -o $@ $^

nvram: main.o libnvram.so
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) -lnvram

nvramd: nvramd.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) $(EXTRA_LDFLAGS) -lnvram -L$(TOP)/shared -lshared -lrt -pthread
