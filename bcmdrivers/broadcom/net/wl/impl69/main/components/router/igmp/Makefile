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
#
# <<Broadcom-WL-IPTag/Open:>>
#

WLAN_ComponentsInUse :=
include ../../../src/makefiles/WLAN_Common.mk

CROSS =
CC = $(CROSS)gcc
AR = $(CROSS)ar
STRIP = $(CROSS)strip

ALL_OBJS = igmprt.o proxy.o igmpv2.o util.o filter.o

CCFLAGS += -DHND_FIX -Wall $(WLAN_IncPathR) $(WLAN_ComponentIncPathR) -I../shared -s -Os -fomit-frame-pointer
LDFLAGS += -Wl,-allow-shlib-undefined
LDFLAGS += -L$(TOP)/netconf -L$(INSTALLDIR)/netconf/usr/lib -lnetconf
LDFLAGS += -L$(TOP)/nvram -L$(INSTALLDIR)/nvram/usr/lib -lnvram
LDFLAGS += $(EXTRA_LDFLAGS)

ifneq ($(LINUX_VERSION),2_6)
CCFLAGS += -DLINUX_INET
endif

ifneq ($(wildcard ./prebuilt),)
all:
	-cp prebuilt/igmp igmp
else
all: igmp
endif

igmp: $(ALL_OBJS)
	$(CC) $(CCFLAGS) $(LDFLAGS) -o $@ $(ALL_OBJS) $(LDLIBS)
	$(STRIP) $@

$(ALL_OBJS): %.o: %.c
	@echo " "
	@echo "Making $<"
	$(CC) $(CCFLAGS) -c $<

install: all
	install -d $(INSTALLDIR)/igmp/usr/sbin
	install -m 755 igmp $(INSTALLDIR)/igmp/usr/sbin

.PHONY:	clean
clean:
	-rm -f *.o *~ core igmp
