#
# milli_httpd Makefile
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
# $Id: Makefile 782473 2019-12-20 01:21:46Z $
#

include $(TOP)/.config

ifneq (2_4,$(LINUX_VERSION))
CFLAGS += -DLINUX26
# No more Application Specific Port Forwards support after Linux 2.6.36
ifneq (2_6,$(LINUX_VERSION))
CFLAGS += -DAUTOFW_PORT_DEPRECATED
endif
endif

CFLAGS	+= -I. -I$(TOP)/shared -I$(TOP)/www/broadcom/cgi -I$(SRCBASE_SYS)/include -I$(SRCBASE)/include ${WLAN_StdIncPathA} -I$(SRCBASE_SYS)/../components/shared -I$(SRCBASE_SYS)/../components/wlioctl/include -I$(SRCBASE_SYS)/../components/proto/include -I$(SRCBASE_SYS)/common/include -Wall
CFLAGS	+= $(if $(WLAN_ComponentIncPathA),$(WLAN_ComponentIncPathA),$(addprefix -I,$(wildcard $(SRCBASE_SYS)/shared/bcmwifi/include)))
# For customer backwords compatibility, for now we keep CONFIG_WSCCMD here and can be removed when
# all customers migrate to new SDK (no CONFIG_WSCCMD in configration files).
ifneq ($(CONFIG_WPS)$(CONFIG_WSCCMD),)
CFLAGS += -I$(SRCBASE)/../components/apps/wps/brcm_apps/include
endif

ifneq ($(strip $(BCA_HNDROUTER)),)
CFLAGS += -I$(BUILD_DIR)/userspace/public/include
CFLAGS += -I$(BUILD_DIR)/userspace/public/include/linux
endif

#CFLAGS	+= -g -DDEBUG
CFLAGS	+= -s
LDFLAGS	+= -L$(TOP)/nvram -L$(INSTALLDIR)/nvram/usr/lib -lnvram -L$(TOP)/libbcmcrypto -L$(INSTALLDIR)/libbcmcrypto/usr/lib -lbcmcrypto -L$(TOP)/shared -L$(INSTALLDIR)/shared/usr/lib -lshared -lpthread
LDFLAGS += $(EXTRA_LDFLAGS)

OBJS := cgi.o ej.o httpd.o cgi_common.o cgi_radio.o cgi_index.o cgi_lan.o cgi_wan.o cgi_status.o cgi_filter.o cgi_forward.o cgi_media.o cgi_storage.o cgi_ssid.o cgi_security.o cgi_wps.o cgi_firmware.o cgi_internal.o
OBJS += cgi_cpuload.o
PRE_LINK_TARGETS :=
PRE_CLEAN_TARGETS :=
IQOS_GUI_SRC :=
PREBUILLT_OBJS :=
ifneq ($(LINUX_VERSION),2_6)
ifeq ($(CONFIG_TREND_IQOS),y)
ifneq ($(wildcard $(IQOS_DIR)),)
PRE_LINK_TARGETS += $(IQOS_DIR)/lib/libiqos_interface.a
PRE_CLEAN_TARGETS += iqos_interface_clean
IQOS_SRCS := $(IQOS_DIR)/src/interface
CFLAGS += -I$(IQOS_SRCS)
CFLAGS += -I$(SRCBASE)/../components/opensource/jsonc/installbin/include/json-c
LDFLAGS += -L$(TOP)/../../components/opensource/jsonc/installbin/lib
LDFLAGS += -L$(INSTALLDIR)/libjsonc/usr/lib -ljson-c
ifneq ($(wildcard $(TOP)/www/broadcom/cgi/cgi_iqos.c),)
	OBJS += cgi_iqos.o
	IQOS_GUI_SRC = cgi_iqos.c
else
	PREBUILLT_OBJS += cgi_iqos.o
endif
vpath %.c $(IQOS_SRCS)
endif # IQOS_DIR
endif # CONFIG_TREND_IQOS
endif

# Adding Passpoint_gui.c dependency
ifeq ($(CONFIG_HSPOT),y)
INCLUDE += -I$(PASSPOINT_DIR)/pkt
INCLUDE += -I$(PASSPOINT_ROUTER)/shared/
CFLAGS += $(INCLUDE)
vpath %.c $(PASSPOINT_ROUTER)/shared/
OBJS += passpoint_gui.o
endif

ifneq ($(LINUX_VERSION),2_6)
ifeq ($(CONFIG_VISUALIZATION),y)
OBJS += vis_gui.o
endif

ifeq ($(CONFIG_WBD),y)
OBJS += cgi_wbd.o
endif

endif

ifeq ($(CONFIG_EZC),y)
OBJS += ezc.o
endif

ifneq ($(CONFIG_AIRIQ),)
CFLAGS += -DWL_AIR_IQ
endif # CONFIG_AIRIQ

vpath %.c $(TOP)/shared
vpath %.c $(TOP)/www/broadcom/cgi
vpath %.o $(SRCBASE_ROUTER)/httpd/prebuilt

ifeq ($(CMWIFI),)
all: httpd libwifihttp.so
else
all: httpd
endif # CMWIFI

CFLAGS += -fPIC
NONE_LIB_OBJS := cgi.o httpd.o  cgi_index.o cgi_lan.o cgi_wan.o cgi_status.o cgi_filter.o cgi_forward.o cgi_firmware.o cgi_internal.o  iqos_gui.o
LIB_OBJS :=$(filter-out $(NONE_LIB_OBJS),$(OBJS))

libwifihttp.so: $(LIB_OBJS) $(PRE_LINK_TARGETS)
	$(CC) $(LDFLAGS) -shared -o $@ $^ -Wl,-Tlinkerscript.ld

clean: $(PRE_CLEAN_TARGETS)
	rm -f $(OBJS) *~ httpd *.so

cgi_iqos.o: $(IQOS_GUI_SRC)
ifneq ($(wildcard cgi_iqos.c),)
	$(CC) $(CFLAGS) -c -o $@ $<
endif

install: all
ifneq ($(CMWIFI),)
	install -D httpd $(INSTALL_DIR)/sbin/httpd_wlan
	$(STRIP) $(INSTALL_DIR)/sbin/httpd_wlan
else
ifeq ($(strip $(USE_EXTERNAL_HTTPD)),)
	install -D httpd $(INSTALLDIR)/usr/sbin/httpd
	$(STRIP) $(INSTALLDIR)/usr/sbin/httpd
else
	install -d $(INSTALLDIR)/lib
	install -m 755 libwifihttp.so $(INSTALLDIR)/lib
	$(STRIP) $(INSTALLDIR)/lib/libwifihttp.so
endif
endif # CMWIFI

ifeq ($(strip $(USE_EXTERNAL_HTTPD)),)
httpd: $(OBJS) $(PREBUILLT_OBJS) $(PRE_LINK_TARGETS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
else
httpd:
	@echo "external httpd is used"
endif

$(IQOS_DIR)/lib/libiqos_interface.a:
ifneq ($(wildcard $(IQOS_DIR)/src),)
	$(MAKE) -I$(IQOS_DIR)/tdts/include -C $(IQOS_DIR)/src/interface
endif

iqos_interface_clean:
ifneq ($(wildcard $(IQOS_DIR)/src),)
	$(MAKE) -C $(IQOS_DIR)/src/interface clean
endif
