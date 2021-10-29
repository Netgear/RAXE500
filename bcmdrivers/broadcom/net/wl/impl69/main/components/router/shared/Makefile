#
# Linux router shared code Makefile
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
# $Id: Makefile 788123 2020-06-22 10:44:39Z $
#

include $(SRCBASE_ROUTER)/.config

CFLAGS	+= -I. -I../httpd -I$(SRCBASE)/include ${WLAN_StdIncPathA} -I$(SRCBASE)/../components/shared -I$(SRCBASE)/../components/wlioctl/include -I$(SRCBASE)/../components/proto/include -I$(SRCBASE)/common/include -Wall -fPIC
CFLAGS	+= $(WLAN_ComponentIncPathA)

ifneq ($(strip $(BCA_HNDROUTER)),)
CFLAGS += -I$(INC_BRCMDRIVER_PUB_PATH)/$(BRCM_BOARD)
CFLAGS += -I$(INC_BRCMSHARED_PUB_PATH)/$(BRCM_BOARD)
endif

ifneq ($(CMWIFI),)
CFLAGS += -I$(WL_CMWIFI_DIR)/libs/wlcsm/include
endif

ifneq (2_4,$(LINUX_VERSION))
ifeq ($(strip $(BCA_HNDROUTER)),)
ifneq ($(CONFIG_STBAP),y)
CFLAGS += -I$(LINUXDIR)/include
endif # CONFIG_STBAP
endif
CFLAGS += -DLINUX26
ifneq (2_6,$(LINUX_VERSION))
CFLAGS += -DAUTOFW_PORT_DEPRECATED
endif
endif
#CFLAGS	+= -g -DDEBUG -pthread
CFLAGS	+= -s -O2 -pthread
ifeq ($(CONFIG_RTR_OPTIMIZE_SIZE),y)
CFLAGS  += -Os
else
CFLAGS  += -O2
endif
LDFLAGS += -L.

ifeq ($(CONFIG_BCM_CEVENT),y)
CFLAGS += -DBCM_CEVENT
endif

OBJS := nvparse.o shutils.o wl.o wl_linux.o linux_timer.o defaults.o wlif_utils.o bcmwifi_channels.o et_linux.o confmtd_utils.o common_utils.o appevent_util.o
OBJS += bcm_usched.o
OBJS += event_utils.o

ifeq ($(CONFIG_WBD),y)
OBJS += wbd_rc_shared.o
endif

# Adding passpoint_nvparse.c dependency
ifeq ($(CONFIG_HSPOT),y)
INCLUDE += -I$(PASSPOINT_DIR)/pkt
INCLUDE += -I$(PASSPOINT_ROUTER)/shared/
CFLAGS  += $(INCLUDE)
vpath %.c $(PASSPOINT_ROUTER)/shared/
OBJS += passpoint_nvparse.o
endif

# Actually all builds use the new 11ac ratespec/chanspec
CFLAGS  += -DD11AC_IOTYPES
vpath %.c $(WLAN_StdSrcDirsA) $(WLAN_ComponentSrcDirsA)

vpath %.c $(SRCBASE)/../../main/src/shared/
OBJS += bcmxtlv.o bcmutils.o

all: libshared.so
ifeq ($(BUILD_RDKWIFI),y)
	install -d $(INSTALLDIR)/shared/usr/lib
	install -m 755 libshared.so $(INSTALLDIR)/shared/usr/lib
endif

install: all
	install -d $(INSTALLDIR)/usr/lib
	install -m 755 libshared.so $(INSTALLDIR)/usr/lib
	$(STRIP) $(INSTALLDIR)/usr/lib/libshared.so

clean:
	rm -f *.o *.so $(INSTALLDIR)/usr/lib/libshared.so

libshared.so: $(OBJS)
	$(CC) -shared -o $@ $^ -lpthread
