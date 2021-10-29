#
# Router rc control scripts Makefile
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
# $Id: Makefile 783588 2020-02-03 05:02:04Z $
#

include $(TOP)/.config

ifneq (2_4,$(LINUX_VERSION))
ifneq (2_6,$(LINUX_VERSION))
CFLAGS += -DLINUX26 -DLINUX_2_6_36 -DAUTOFW_PORT_DEPRECATED
else
CFLAGS += -DLINUX26
endif

CFLAGS += -I$(SRCBASE)/../components/bcmcrypto/include

ifeq ($(shell ( [ ! -z $(BCA_CPEROUTER) ]) && [ ! -z $(BUILD_BRCM_HOSTAPD) ];  echo $$?),0)
CPEROUTER_HOSTAPD_BUILD:=y
endif

ifneq ($(strip $(BCA_HNDROUTER)),)
BB_BASE_PATH = $(BUILD_DIR)/userspace/gpl/apps/busybox
BB_HEADER_PATH = $(firstword $(dir $(shell find $(BB_BASE_PATH) -name  Config.h)))
ifeq ($(strip $(BB_HEADER_PATH)),)
$(info "Could not find busybox header path in $(BB_BASE_PATH)")
else
CFLAGS += -I$(BB_HEADER_PATH)
endif
CFLAGS += -I$(BUILD_DIR)/userspace/public/include
CFLAGS += -I$(BUILD_DIR)/userspace/public/include/linux
CFLAGS += -I$(INC_BRCMDRIVER_PUB_PATH)/$(BRCM_BOARD)
# For now, enable HAPD_WDS only for router builds
# ifeq ($(BUILD_BRCM_HOSTAPD),y)
# CFLAGS += -DHAPD_WDS_DRAFT
# endif # BUILD_BRCM_HOSTAPD
# For now, enable HAPD_WDS_HOTPLUG only for router builds
ifeq ($(BUILD_BRCM_HOSTAPD),y)
CFLAGS += -DHAPD_WDS
endif # BUILD_BRCM_HOSTAPD
ifeq ($(strip $(BUILD_ETHWAN)),)
# GMAC builds do not support WAN
CFLAGS += -DNO_ETHWAN
endif
else
ifneq ($(CONFIG_STBAP),y)
CFLAGS += -I$(LINUXDIR)/include -I$(TOP)/busybox-1.x
endif # CONFIG_STBAP
endif # BCA_HNDROUTER

ifeq ($(BCA_HND_EAP),y)
CFLAGS += -DBCA_HND_EAP
endif

CFLAGS += -I. -I$(TOP)/shared -I$(SRCBASE)/include ${WLAN_StdIncPathA} -I$(SRCBASE)/../components/shared -I$(SRCBASE)/../components/wlioctl/include -I$(SRCBASE)/../components/proto/include -I$(SRCBASE)/common/include -Wall -Werror
else # 2.4
CFLAGS += -I. -I$(TOP)/shared -I$(TOP)/busybox -I$(SRCBASE)/include ${WLAN_StdIncPathA} -I$(SRCBASE)/../components/shared -I$(SRCBASE)/common/include -Wall -Werror
endif

CFLAGS += -I$(SRCBASE)/../components/math/include -I$(SRCBASE)/../components/math/src

CFLAGS += $(if $(WLAN_ComponentIncPathA),$(WLAN_ComponentIncPathA),$(addprefix -I,$(wildcard $(SRCBASE)/shared/bcmwifi/include)))
CFLAGS += -I$(TOP)/dpsta

#CFLAGS	+= -g -DDEBUG
CFLAGS	+= -s
ifeq ($(CONFIG_RTR_OPTIMIZE_SIZE),y)
CFLAGS  += -Os
else
CFLAGS  += -O2
endif

ifeq ($(strip $(BCA_CPEROUTER)),)
LDFLAGS	+= -L$(TOP)/nvram -L$(PLATFORMDIR)/install/nvram/usr/lib -lnvram
endif

LDFLAGS	+= -L$(TOP)/shared -L$(PLATFORMDIR)/install/shared/usr/lib -lshared \
	-L$(TOP)/libbcmcrypto -L$(INSTALLDIR)/libbcmcrypto/usr/lib -lbcmcrypto -lpthread
LDFLAGS += -L$(TOP)/libbcm -L$(INSTALLDIR)/libbcm/usr/lib -lbcm
LDFLAGS_N2H = -L$(TOP)/rc -L$(INSTALLDIR)/lib -lnv2hapdcfg
LDFLAGS += $(EXTRA_LDFLAGS)

ifeq ($(BUILD_WFA_APUT),1)
CFLAGS	+= -DCAPI_AP
endif

ifeq ($(CONFIG_WAPI_IAS),y)
CFLAGS += -DRC_BUILDTIME=$(shell date -d "now" +"%s")
endif

OBJS := rc.o init.o interface.o network.o services.o udhcpc.o http.o stats.o bcmutils.o mtd.o usb_hotplug.o pmon.o bcm_math.o

ifeq ($(CONFIG_NAT),y)
ifeq ($(strip $(BCA_CPEROUTER)),)
OBJS += firewall.o
LDFLAGS += -L$(TOP)/netconf -L$(PLATFORMDIR)/install/netconf/usr/lib -lnetconf
endif
endif

ifeq ($(CONFIG_NAT),y)
OBJS += ppp.o
endif

ifeq ($(BUILD_BRCM_HOSTAPD),y)
OBJS += hostapd_config.o
endif

ifeq ($(RWL),1)
CFLAGS += -DRWL_SOCKET
endif

vpath %.c $(TOP)/shared $(SRCBASE)/shared $(SRCBASE)/../components/math/src

ifeq ($(BUILD_BRCM_HOSTAPD),y)
CFLAGS += -I$(WPA_SUPPLICANT_DIR)/src/common
LDFLAGS += -L$(WPA_SUPPLICANT_DIR)/wpa_supplicant -L$(PLATFORMDIR)/install/wpa_supplicant/usr/lib -lwpa_client
WPS_PBCD := wps_pbcd
OBJS_WPS_PBCD := wps_pbcd.o
ALL = $(WPS_PBCD)
endif

ifneq ($(CMWIFI),)
CFLAGS += -I$(WL_CMWIFI_DIR)/libs/wlcsm/include
endif

ifeq ($(strip $(BCA_CPEROUTER)),)
ALL += rc
endif
LIBNV2HAPDCFG := libnv2hapdcfg.so
ALL += $(LIBNV2HAPDCFG)
CFLAGS += -fPIC
$(LIBNV2HAPDCFG): hostapd_config.o
	$(CC) $(LDFLAGS) -shared -o $@ $^

all: $(ALL)

clean:
	rm -f *.o rc .*.depend
	rm -f $(WPS_PBCD)
	rm -f *.so

install: all
ifeq ($(strip $(BCA_CPEROUTER)),)
	install -d $(INSTALLDIR)/sbin
	install rc $(INSTALLDIR)/sbin
	$(STRIP) $(INSTALLDIR)/sbin/rc
ifeq ($(BUILD_BRCM_HOSTAPD),y)
	install $(WPS_PBCD) $(INSTALLDIR)/sbin
	$(STRIP) $(INSTALLDIR)/sbin/$(WPS_PBCD)
endif
ifeq ($(strip $(BCA_HNDROUTER)),)
	cd $(INSTALLDIR)/sbin && ln -sf rc write_cfe
endif
	cd $(INSTALLDIR)/sbin && ln -sf rc write
	cd $(INSTALLDIR)/sbin && ln -sf rc init
	cd $(INSTALLDIR)/sbin && ln -sf rc stats
	cd $(INSTALLDIR)/sbin && ln -sf rc hotplug
	cd $(INSTALLDIR)/sbin && ln -sf rc preinit
	cd $(INSTALLDIR)/sbin && ln -sf rc erase
else
ifeq ($(strip $(CPEROUTER_HOSTAPD_BUILD)),y)
	install -d $(INSTALLDIR)/sbin
	install -d $(INSTALLDIR)/lib
	install  $(WPS_PBCD) $(INSTALLDIR)/sbin
	$(STRIP) $(INSTALLDIR)/sbin/$(WPS_PBCD)
endif
endif
	install -d $(INSTALLDIR)/lib
	install  $(LIBNV2HAPDCFG) $(INSTALLDIR)/lib
	$(STRIP) $(INSTALLDIR)/lib/$(LIBNV2HAPDCFG)

$(WPS_PBCD): $(OBJS_WPS_PBCD) $(LIBNV2HAPDCFG)
	$(CC) -o $@ $(OBJS_WPS_PBCD) $(LDFLAGS) $(LDFLAGS_N2H)

rc: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

%.o: .%.depend
.%.depend: %.c
	$(CC) $(CFLAGS) -M $< > $@

-include $(OBJS:%.o=.%.depend)
