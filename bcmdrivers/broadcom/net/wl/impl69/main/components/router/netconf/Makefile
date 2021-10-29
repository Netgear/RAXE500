#
# Linux netconf Makefile
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
# $Id: Makefile 768736 2018-10-23 21:53:02Z $
#

#CFLAGS	+= -g -DDEBUG
CFLAGS	+= -s

ifneq ($(strip $(BCA_HNDROUTER)),)
CFLAGS += -DLINUX26 -DLINUX_2_6_36
CFLAGS += -I$(BUILD_DIR)/userspace/gpl/apps/iptables/iptables-1.4.21/include
CFLAGS += -I$(BCM_FSBUILD_DIR)/gpl/include
LDFLAGS += -L. -L$(BCM_FSBUILD_DIR)/gpl/lib -lip4tc
CFLAGS += -D__EXPORTED_HEADERS__ -fPIC
else ifeq (2_6_36,$(LINUX_VERSION))
CFLAGS += -DLINUX26 -DLINUX_2_6_36
CFLAGS  += -I$(TOP)/iptables-1.4.12/src/include
LDFLAGS += -L. -L$(TOP)/iptables-1.4.12
CFLAGS += -D__EXPORTED_HEADERS__ -fPIC
else ifeq (2_6,$(LINUX_VERSION))
CFLAGS += -DLINUX26
CFLAGS  += -I$(TOP)/iptables-1.x/include
LDFLAGS += -L. -L$(TOP)/iptables-1.x/libiptc
else
CFLAGS  += -I$(TOP)/iptables/include
LDFLAGS += -L. -L$(TOP)/iptables/libiptc
endif

CFLAGS	+= -I. -I$(TOP)/shared -I$(SRCBASE)/include ${WLAN_StdIncPathA} -I$(SRCBASE)/../components/shared -I$(SRCBASE)/../components/wlioctl/include -I$(SRCBASE)/../components/proto/include -I$(SRCBASE)/common/include -Wall

ifeq ($(strip $(BCA_HNDROUTER)),)
CFLAGS += -I$(LINUXDIR)/include
endif

all: libnetconf.so netconf

install: all
	install -D libnetconf.so $(INSTALLDIR)/usr/lib/libnetconf.so
	$(STRIP) $(INSTALLDIR)/usr/lib/libnetconf.so

clean:
	rm -f *.o *.a *.so netconf

libnetconf.so: netconf_linux.o
ifneq ($(strip $(BCA_HNDROUTER)),)
	$(CC) -shared -o $@ $^ $(LDFLAGS) -liptc
else
	$(LD) -shared -o $@ $^ $(LDFLAGS) -liptc
endif

netconf: main.o libnetconf.so
	$(CC) -o $@ $< $(LDFLAGS) -lnetconf
