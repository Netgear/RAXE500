#
# Broadcom Linux Router Makefile
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
# $Id: Makefile 789555 2020-07-31 02:28:45Z $
#

ifneq (true,$(parallel))
  .NOTPARALLEL:
endif

include .config
include .config.plt

CPU ?=
MAKE_ARGS ?=

LINUX_VERSION ?= 2_6

ARCH = mips
PLT ?= mipsel

# Get ARCH from PLT argument
ifneq ($(findstring arm,$(PLT)),)
ARCH := arm
endif

ifeq ($(CONFIG_STBAP),y)
ARCH := $(PLT)
endif

# uClibc wrapper
ifeq ($(CONFIG_UCLIBC),y)
PLATFORM := $(PLT)-uclibc
else ifeq ($(CONFIG_GLIBC),y)
PLATFORM := $(PLT)-glibc
else
PLATFORM := $(PLT)
endif
TARGET_PLT ?= $(PLT)-linux

# Source bases
export PLATFORM LIBDIR USRLIBDIR LINUX_VERSION

export BCM_KVERSIONSTRING := $(subst _,.,$(LINUX_VERSION))

WLAN_ComponentsInUse := avs awd bcmcrypto bcmdrv bcmwifi clm clm-api ppr math
include ../../src/makefiles/WLAN_Common.mk

export SRCBASE := $(WLAN_SrcBaseA)
export BASEDIR := $(WLAN_TreeBaseA)
export TOP = $(CURDIR)
export WLAN_ComponentSrcDirsA
export WLAN_ComponentIncPathA
export WLAN_StdSrcDirsA
export WLAN_StdIncPathA

ifeq (2_6_36,$(LINUX_VERSION))
export	LINUXDIR := $(BASEDIR)/components/opensource/linux/linux-2.6.36
export	KBUILD_VERBOSE := 1
export	BUILD_MFG := 0
SUBMAKE_SETTINGS = SRCBASE=$(SRCBASE) BASEDIR=$(BASEDIR)
else ifeq (2_6,$(LINUX_VERSION))
export	LINUXDIR := $(BASEDIR)/components/opensource/linux/linux-2.6
export	KBUILD_VERBOSE := 1
export	BUILD_MFG := 0
SUBMAKE_SETTINGS = SRCBASE=$(SRCBASE) BASEDIR=$(BASEDIR)
else
ifeq ($(CMWIFI),)
ifeq ($(CONFIG_STBAP),y)
export  LINUXDIR := $(LINUX)
else
export	LINUXDIR ?= $(SRCBASE)/linux/linux
endif # CONFIG_STBAP
endif # CMWIFI
SUBMAKE_SETTINGS  ?= SRCBASE=$(SRCBASE)
endif

# top level "sys" src path which has all files from trunk.
export SRCBASE_ROUTER := $(abspath $(BASEDIR)/components/router)
export SRCBASE_MAIN   := $(abspath $(SRCBASE))

ifeq (2_6,$(LINUX_VERSION))
export SRCBASE_SYS    := $(abspath $(SRCBASE))
else
export SRCBASE_SYS    := $(abspath $(SRCBASE)/../../sys/src)
endif

# Opensource bases
OPENSOURCE_BASE_DIR := $(BASEDIR)/components/opensource
DNSMASQ_DIR := $(OPENSOURCE_BASE_DIR)/dnsmasq
LIBNFNETLINK_DIR := $(OPENSOURCE_BASE_DIR)/netfilter/libnfnetlink
LIBMNL_DIR := $(OPENSOURCE_BASE_DIR)/netfilter/libmnl
LIBNETFILTER_QUEUE_DIR := $(OPENSOURCE_BASE_DIR)/netfilter/libnetfilter_queue
LIBNETFILTER_CONNTRACK_DIR := $(OPENSOURCE_BASE_DIR)/netfilter/libnetfilter_conntrack
LIBSQLITE3_DIR := $(OPENSOURCE_BASE_DIR)/sqlite3
LIBJSONC_DIR := $(OPENSOURCE_BASE_DIR)/jsonc
LIBXML2_DIR := $(OPENSOURCE_BASE_DIR)/libxml2
RPCAPD_DIR := $(OPENSOURCE_BASE_DIR)/libpcap_winpcap

ifeq ($(CONFIG_STBAP),y)
export LIBNL_DIR := $(STBAP_LIBNL_PATH)
export PKG_CONFIG_PATH := $(LIBNL_DIR)/lib/pkgconfig:$(PKG_CONFIG_PATH)
else
export LIBNL_DIR := $(abspath $(OPENSOURCE_BASE_DIR)/router_tools/libnl)
export PKG_CONFIG_PATH := $(LIBNL_DIR)/install/lib/pkgconfig:$(PKG_CONFIG_PATH)
endif # CONFIG_STBAP
HOSTAPD_DIR := $(OPENSOURCE_BASE_DIR)/router_tools/hostapd
export WPA_SUPPLICANT_DIR := $(OPENSOURCE_BASE_DIR)/router_tools/wpa_supplicant
IW_DIR := $(OPENSOURCE_BASE_DIR)/router_tools/iw

#Visualization Directories
VISSOURCE_BASE_DIR := $(BASEDIR)/components/apps/visualization
VISDCOLL_DIR := $(VISSOURCE_BASE_DIR)/datacollector
VISDCON_DIR := $(VISSOURCE_BASE_DIR)/dataconcentrator

ifeq ($(CONFIG_STBAP),y)

# ebtables should build for STBAP
export EBTABLES_DIR := $(BASEDIR)/components/opensource/ebtables

# Opensource base for openssl
ifeq ($(PLT),arm)
export ARCH=arm
export MACHINE=arm
else
export ARCH=aarch64
export MACHINE=aarch64
endif
export OPENSSL_DIR := $(STBAP_OPENSSL_PATH)

NICTARGET ?= debug
ifeq ($(WLTEST),1)
NICTARGET := debug
else
ifeq ($(NO_BCMINTERNAL),1)
NICTARGET := nodebug
endif
endif # WLTEST

# Emf, igs and dpsta Driver Directories
export EMFSOURCE_DIR := $(BASEDIR)/components/router/emf/emf

export EMF_EXTRA_SYMBOLS=$(EMFSOURCE_DIR)/../igs/obj-igs-$(LINUXVER)/Module.symvers
export KBUILD_EXTRA_SYMBOLS=${BASEDIR}/Module.symvers

# DHD Driver  Directories
export DHDSOURCE_DIR := $(BASEDIR)/../sys/src/dhd/linux
ifeq ($(PLT),arm)
export DHD_STBAP_NONSECDMA_TARGET := dhd-stbap-armv7l
export DHD_STBAP_SECDMA_TARGET := dhd-stbap-secdma-armv7l
else
export DHD_STBAP_NONSECDMA_TARGET := dhd-stbap-armv8
export DHD_STBAP_SECDMA_TARGET := dhd-stbap-secdma-armv8
endif

# NIC Driver  Directories
export NICSOURCE_DIR := $(BASEDIR)/src/wl/linux
ifeq ($(PLT),arm)
export NIC_STBAP_SECDMA_TARGET := $(NICTARGET)-stbap-secdma-armv7l
export NIC_STBAP_NONSECDMA_TARGET := $(NICTARGET)-stbap-armv7l
else
export NIC_STBAP_SECDMA_TARGET := $(NICTARGET)-stbap-secdma-armv8
export NIC_STBAP_NONSECDMA_TARGET := $(NICTARGET)-stbap-armv8
endif
endif # CONFIG_STBAP

export WBDSOURCE_DIR := $(BASEDIR)/components/apps/wbd2
export IEEE1905SOURCE_DIR := $(BASEDIR)/components/apps/ieee1905

# WPS Directories
export WPSSOURCE_DIR := $(BASEDIR)/components/apps/wps

# MoCa Directories
MOCA_BASE_DIR := $(BASEDIR)/components/apps/moca

# appeventd Directories
APPEVENTD_BASE_DIR := $(BASEDIR)/components/apps/appeventd

# BSD Directories
BSD_BASE_DIR := $(BASEDIR)/components/apps/bsd

# Passpoint Directories
export PASSPOINT_DIR := $(abspath $(SRCBASE_SYS)/../components/apps/hspot)
export PASSPOINT_ROUTER := $(abspath $(PASSPOINT_DIR)/router)
export PASSPOINT_AP_DIR := $(abspath $(PASSPOINT_ROUTER)/hspot_ap)
# Air-IQ Directory
export AIRIQ_DIR := $(BASEDIR)/components/apps/airiq

#
# Cross-compile environment variables
#

# Build platform
export BUILD := i386-pc-linux-gnu
export HOSTCC := gcc

ifeq ($(PLATFORM),mipsel)
export CROSS_COMPILE := mipsel-linux-
export CONFIGURE := ./configure mipsel-linux --build=$(BUILD)
export TOOLCHAIN := $(shell cd $(dir $(shell which $(CROSS_COMPILE)gcc))/../mipsel-linux && pwd -P)
endif

ifeq ($(PLATFORM),mipsel-uclibc)
ifeq (2_6_36,$(LINUX_VERSION))
export CROSS_COMPILE := mipsel-uclibc-linux-2.6.36-
CFLAGS += -D__EXPORTED_HEADERS__ -fPIC
else ifeq (2_6,$(LINUX_VERSION))
export CROSS_COMPILE := mipsel-uclibc-linux26-
else
export CROSS_COMPILE := mipsel-uclibc-
endif
export CONFIGURE := ./configure mipsel-linux --build=$(BUILD)
export TOOLCHAIN := $(shell cd $(dir $(shell which $(CROSS_COMPILE)gcc))/.. && pwd -P)
endif

ifeq ($(PLATFORM),mipsel-glibc)
ifeq (2_6_36,$(LINUX_VERSION))
export CROSS_COMPILE := mipsel-glibc-linux-2.6.36-
export TOOLCHAIN := $(shell cd $(dir $(shell which $(CROSS_COMPILE)gcc))/.. && pwd -P)
CFLAGS += -D__EXPORTED_HEADERS__ -fPIC
else ifeq (2_6,$(LINUX_VERSION))
export CROSS_COMPILE := mipsel-linux-linux26-
else
export CROSS_COMPILE := mipsel-linux-
export CONFIGURE := ./configure mipsel-linux --build=$(BUILD)
export TOOLCHAIN := $(shell cd $(dir $(shell which $(CROSS_COMPILE)gcc))/../mipsel-linux && pwd -P)
endif
endif

ifeq ($(PLATFORM),arm-uclibc)
export CROSS_COMPILE := arm-brcm-linux-uclibcgnueabi-
export CONFIGURE := ./configure arm-linux --build=$(BUILD)
export TOOLCHAIN := $(shell cd $(dir $(shell which $(CROSS_COMPILE)gcc))/.. && pwd -P)
export CFLAGS += -fno-strict-aliasing
SUBMAKE_SETTINGS += ARCH=$(ARCH)
EXTRA_LDFLAGS := -lgcc_s
endif

ifeq ($(PLATFORM),arm-glibc)
ifneq ($(CONFIG_STBAP),y)
export CROSS_COMPILE ?= arm-buildroot-linux-gnueabi-
else
export CROSS_COMPILE ?= arm-linux-
endif # CONFIG_STBAP
export CONFIGURE := ./configure arm-linux --build=$(BUILD)
export TOOLCHAIN := $(shell cd $(dir $(shell which $(CROSS_COMPILE)gcc))/../.. && pwd -P)
export CFLAGS += -fno-strict-aliasing
SUBMAKE_SETTINGS += ARCH=$(ARCH)
EXTRA_LDFLAGS := -lgcc_s
endif

ifneq ($(strip $(BCA_HNDROUTER)),)
export CFLAGS += -DBCA_HNDROUTER $(USR_CFLAGS)
ifneq ($(strip $(BUILD_MCAST_PROXY)),)
CFLAGS += -DMCPD_PROXY
endif

ifeq ($(PROFILE_ARCH),aarch64)
export EXTRA_LDFLAGS := -lgcc_s $(USR_LDFLAGS) -Wl,-rpath=/usr/lib
else
export EXTRA_LDFLAGS := -lgcc_s $(USR_LDFLAGS)
endif
endif

ifeq ($(PLATFORM),arm64-glibc)
export CROSS_COMPILE ?= aarch64-linux-gnu-
export CONFIGURE := ./configure  aarch64-linux-gnu --build=$(BUILD)
export TOOLCHAIN := $(shell cd $(dir $(shell which $(CROSS_COMPILE)gcc))/../.. && pwd -P)
export CFLAGS += -fno-strict-aliasing
export CFLAGS += -Wno-pointer-to-int-cast
export CFLAGS += -Wno-int-to-pointer-cast
SUBMAKE_SETTINGS += ARCH=$(ARCH)
EXTRA_LDFLAGS := -lgcc_s
TARGET_PLT=aarch64-linux-gnu
endif

ifeq ($(CONFIG_STBAP),y)
ifeq ($(PLT),arm64)
export CFLAGS += -DBN64_BIT
endif
endif

ifeq ($(CONFIG_RTR_OPTIMIZE_SIZE),y)
export CFLAGS += -Os
export OPTCFLAGS = -Os
else
export CFLAGS += -O2
export OPTCFLAGS = -O2
endif

#look at driver configuration
WLCFGDIR=$(SRCBASE)/wl/config

ifeq ($(CONFIG_NVRAM),y)
export CFLAGS += -DBCMNVRAM
endif

ifeq ($(CONFIG_BCMWPA2),y)
export CFLAGS += -DBCMWPA2
endif

# BCA_HNDROUTER mfg flag
ifneq ($(BUILD_HND_MFG),)
export BUILD_MFG := 1
export WLTEST := 1
endif

ifeq ($(CONFIG_DHDAP),y)
export CONFIG_DHDAP
export CFLAGS += -D__CONFIG_DHDAP__

export SRCBASE_DHD := $(SRCBASE)/../../sys/src

ifeq ($(CONFIG_LBR_AGGR), y)
export CONFIG_LBR_AGGR
endif

# include toplevel Make for firmware
include Makefile.fw
endif

ifeq ($(CONFIG_RSDB),y)
export CFLAGS += -D__CONFIG_RSDB__
endif

ifeq ($(CONFIG_GMAC3),y)
export CFLAGS += -D__CONFIG_GMAC3__
endif

ifeq ($(CONFIG_STBAP),y)
export CFLAGS += -D__CONFIG_STBAP__
endif

ifeq ($(PLT),arm)
ifeq ($(call wlan_version_ge,$(BCM_KVERSIONSTRING),2.6.36),TRUE)
ifeq ($(CONFIG_VISUALIZATION),y)
export CFLAGS += -D__CONFIG_VISUALIZATION__ -DCONFIG_VISUALIZATION_ENABLED
endif
endif

ifeq ($(CONFIG_PORT_BONDING),y)
export CFLAGS += -DPORT_BONDING
endif
endif

ifeq ($(CONFIG_NFC),y)
# WPS_NFC
export CFLAGS += -D__CONFIG_NFC__
endif

ifeq ($(CONFIG_EMF),y)
export CFLAGS += -D__CONFIG_EMF__
export CONFIG_EMF_ENABLED := $(CONFIG_EMF)
endif

ifeq ($(CONFIG_IGMP_PROXY),y)
export CFLAGS += -D__CONFIG_IGMP_PROXY__
endif

ifeq ($(CONFIG_WL_ACI),y)
export CFLAGS += -D__CONFIG_WL_ACI__
endif

ifeq ($(CONFIG_TRAFFIC_MGMT_RSSI_POLICY),y)
export CFLAGS += -DTRAFFIC_MGMT_RSSI_POLICY
endif

ifneq ($(and $(NO_BCMINTERNAL), $(NO_BCMDBG)),)
obj-$(CONFIG_UTELNETD) += utelnetd
endif

MNL_CFLAGS:="-I$(LIBMNL_DIR)/install/include"
MNL_LIBS:="-L$(LIBMNL_DIR)/install/lib -lmnl"

NFNETLINK_CFLAGS:="-I$(LIBNFNETLINK_DIR)/install/include"
NFNETLINK_LIBS:="-L$(LIBNFNETLINK_DIR)/install/lib -lnfnetlink"

NETFILTER_CONNTRACK_CFLAGS:="-I$(LIBNETFILTER_CONNTRACK_DIR)/install/include"
NETFILTER_CONNTRACK_LIBS:="-L$(LIBNETFILTER_CONNTRACK_DIR)/install/lib -lnetfilter_conntrack"

NETFILTER_QUEUE_CFLAGS:="-I$(LIBNETFILTER_QUEUE_DIR)/install/include"
NETFILTER_QUEUE_LIBS:="-L$(LIBNETFILTER_QUEUE_DIR)/install/lib -lnetfilter_queue"

ifeq ($(CONFIG_SOUND),y)
export CFLAGS += -D__CONFIG_SOUND__
endif

ifeq ($(CONFIG_VOIP),y)
export CFLAGS += -DBCMVOIP
endif

ifeq ($(CONFIG_SQUASHFS), y)
ROOT_IMG := target.squashfs
else
ROOT_IMG := target.cramfs
endif

ifeq ($(CONFIG_WAPI),y)
export CFLAGS += -DBCMWAPI_WAI -DBCMWAPI_WPI
endif

ifeq ($(CONFIG_PHYMON_UTILITY),y)
export CFLAGS += -DPHYMON
endif

ifneq (2_4,$(LINUX_VERSION))
CRAMFSDIR := cramfs
else
CRAMFSDIR := $(LINUXDIR)/scripts/cramfs
endif

ifeq ($(CONFIG_BCMDCS),y)
export CFLAGS += -DBCM_DCS
endif
ifeq ($(CONFIG_EXTACS),y)
export CFLAGS += -DEXT_ACS
endif

ifeq ($(CONFIG_BCMESCAND),y)
ifneq ($(filter y,$(CONFIG_EAP) $(BUILD_HND_EAP)),)
export CFLAGS += -DBCM_ESCAND
endif
endif

ifeq ($(CONFIG_BCMBSD),y)
export CFLAGS += -DBCM_BSD
endif

ifeq ($(CONFIG_BCMSSD),y)
export CFLAGS += -DBCM_SSD
endif

ifeq ($(CMWIFI_RDKB),y)
# only used by RDKB or RDKM builds
export CFLAGS += -DBCM_ECBD
export BCM_ECBD := 1
endif

ifeq ($(CONFIG_BCMEVENTD),y)
export CFLAGS += -DBCM_EVENTD
endif

ifeq ($(CONFIG_TOAD),y)
export CFLAGS += -D__CONFIG_TOAD__
endif

ifeq ($(CONFIG_BCM_APPEVENTD),y)
export CFLAGS += -DBCM_APPEVENTD
endif

ifeq ($(CONFIG_BCM_MEVENTD),y)
export CFLAGS += -DBCM_MEVENTD
endif

ifeq ($(CONFIG_MFP),y)
export CFLAGS += -DMFP
endif

ifeq ($(CONFIG_BCMDRSDBD),y)
export CFLAGS += -DBCM_DRSDBD
endif

ifeq ($(CONFIG_MOCA),y)
export CFLAGS += -DBCM_MOCA
endif

ifeq ($(CONFIG_BCM_CEVENT),y)
obj-y += cevent_app
CEVENT_APP_DIR := $(BASEDIR)/components/apps/cevent_app
export CFLAGS += -D__CONFIG_BCM_CEVENT__ -DBCM_CEVENT
export CONFIG_BCM_CEVENT
endif

ifeq ($(CONFIG_HSPOT),y)
export CFLAGS += -DNAS_GTK_PER_STA -DHSPOT_OSEN
ifeq ($(strip $(BCA_CPEROUTER)),)
export ICONPATH := /www/hspot
else
export ICONPATH := /webs/wlrouter/hspot
endif
export CFLAGS += -DICONPATH=\"$(ICONPATH)\"
endif

ifeq ($(CONFIG_WBD),y)
export CFLAGS += -DBCM_WBD -DMULTIAP
endif

ifeq ($(CONFIG_SIGMA),y)
export CFLAGS += -D__CONFIG_SIGMA__
endif

ifeq ($(CONFIG_MINI_ROUTER), y)
export CFLAGS += -D__CONFIG_ROUTER_MINI__
endif

ifeq ($(CONFIG_WNM), y)
export CFLAGS += -DWLWNM
endif

ifeq ($(call wlan_version_ge,$(BCM_KVERSIONSTRING),2.6.36),TRUE)
ifeq ($(CONFIG_BCMASPMD),y)
export CFLAGS += -DBCM_ASPMD
endif # ASPMD
endif # wlan_version_ge

ifeq ($(CONFIG_FBT),y)
export	CFLAGS += -DWLHOSTFBT
endif # CONFIG_FBT

ifeq ($(BUILD_HND_EAP),y)
export	CFLAGS += -DBCM_SKB_FREE_OFFLOAD
endif

ifneq ($(CMWIFI),)
export CC ?= $(CROSS_COMPILE)gcc
else
export CC := $(CROSS_COMPILE)gcc
endif # CMWIFI
export AR := $(CROSS_COMPILE)ar
export AS := $(CROSS_COMPILE)as
export LD := $(CROSS_COMPILE)ld
export NM := $(CROSS_COMPILE)nm
export RANLIB := $(CROSS_COMPILE)ranlib
export STRIP := $(CROSS_COMPILE)strip
export SIZE := $(CROSS_COMPILE)size

ifneq ("$(LINUX_VERSION)","2_4")
export MKSYM := $(shell which $(TOP)/misc/mksym.pl)
endif

#
# Install and target directories
#

export PLATFORMDIR := $(TOP)/$(PLATFORM)
export INSTALLDIR := $(PLATFORMDIR)/install
export TARGETDIR := $(PLATFORMDIR)/target
ifeq ($(CONFIG_STBAP),y)
export INSTALL_DIR := $(PLATFORMDIR)/install
export STBAP_TARGET_DIR := $(PLATFORMDIR)/stbap_binaries_${LINUXVER}
endif # CONFIG_STBAP

define STRIP_DEBUG_SYMBOLS
	@dbgsymf=$(basename $(1))_dbgsym$(suffix $(1)); \
	if [ ! -e "$${dbgsymf}" -o "$(1)" -nt "$${dbgsymf}" ]; then \
	   echo "#- $0"; \
	   ls -ln $1 | awk '{printf "Orig  size: %10d bytes, %s\n",$$5,$$NF}'; \
	   cp -p -v $1 $$dbgsymf; $(STRIP) -d $(1); touch $$dbgsymf; \
	   ls -ln $1 | awk '{printf "Strip size: %10d bytes, %s\n",$$5,$$NF}'; \
	fi
endef

# DHD AP support (PCIe full dongle)
obj-$(CONFIG_DHDAP) += dhd
# for bca builds, dongle is built directly
ifeq ($(CONFIG_STBAP),y)
ifneq ($(FW_CLEAN),n)
obj-$(CONFIG_DHDAP) += pciefd
endif
else
ifeq ($(strip $(BCA_HNDROUTER)),)
obj-$(CONFIG_DHDAP) += pciefd
endif
endif # CONFIG_STBAP

#PCIe FD images in target file system
ifneq ($(BCM_EXTFDIMAGE_PATH),)
obj-$(CONFIG_DHDAP) += extfdimg
endif

# always build libbcmcrypto
obj-y += libbcmcrypto

#
# Configuration
#

#ifdef BCMSOUND
obj-$(CONFIG_SIGMA) += sigma
obj-$(CONFIG_SALSA) += salsa
obj-$(CONFIG_LIBZ) += libz
obj-$(CONFIG_LIBID3TAG) += libid3tag
obj-$(CONFIG_LIBMAD) += libmad
obj-$(CONFIG_MADPLAY) += madplay
obj-$(CONFIG_APLAY) += alsa-utils/aplay
#endif // endif
obj-$(CONFIG_RPCAPD) += rpcapd
obj-$(CONFIG_NVRAM) += nvram
obj-$(CONFIG_SHARED) += shared
obj-$(CONFIG_LIBBCM) += libbcm
ifeq ($(CONFIG_STBAP),y)
obj-$(CONFIG_LIBBCMSTB) += libbcmstb
endif
obj-$(CONFIG_BSHARED) += bshared
ifeq ($(RDK_BUILD),)
obj-$(CONFIG_HTTPD) += httpd
endif
obj-$(CONFIG_RC) += rc
ifeq ($(PLT),arm)
ifeq ($(call wlan_version_ge,$(BCM_KVERSIONSTRING),2.6.36),TRUE)
ifeq ($(CMWIFI_RDKB),)
obj-$(CONFIG_VISUALIZATION) += libsqlite3 libjsonc libxml2 visdcoll visdcon
else
obj-$(CONFIG_VISUALIZATION) += libjsonc visdcoll visdcon
endif # CMWIFI_RDKB
ifneq ($(wildcard $(IQOS_DIR)),)
obj-$(CONFIG_TREND_IQOS) += libjsonc
endif
endif
endif
obj-$(CONFIG_WBD) += libjsonc ebtables ieee1905 wbd
obj-$(CONFIG_WWW) += www
obj-$(CONFIG_GLIBC) += lib
obj-$(CONFIG_UCLIBC) += lib
obj-$(CONFIG_WLCONF) += wlconf
obj-$(CONFIG_DNSMASQ) += dnsmasq
ifeq ($(strip $(BCA_HNDROUTER)),)
obj-$(CONFIG_BRIDGE) += bridge
obj-$(CONFIG_BUSYBOX) += busybox
obj-$(CONFIG_IPTABLES) += iptables
obj-$(CONFIG_LIBIPT) += iptables
endif
# Build only for kernel >= 2.6.36.
ifeq ($(call wlan_version_ge,$(BCM_KVERSIONSTRING),2.6.36),TRUE)
obj-$(CONFIG_LIBSTDCPP) += libstdcpp
obj-$(CONFIG_LIBFLOW) += libflow
obj-$(CONFIG_LIBMNL) += libmnl
obj-$(CONFIG_LIBNFNETLINK) += libnfnetlink
obj-$(CONFIG_LIBNETFILTER_CONNTRACK) += libnetfilter_conntrack
obj-$(CONFIG_LIBNETFILTER_QUEUE) += libnetfilter_queue
endif
obj-$(CONFIG_HSPOT) += hspot_ap
obj-$(CONFIG_WAPI) += wapi/wapid
obj-$(CONFIG_WAPI_IAS) += wapi/as
obj-$(CONFIG_EZC) += ezc
obj-$(CONFIG_NETCONF) += netconf
obj-$(CONFIG_NTP) += ntpclient
obj-$(CONFIG_PPP) += ppp
obj-$(CONFIG_UDHCPD) += udhcpd
obj-$(CONFIG_IGD) += igd
obj-$(CONFIG_LIBUPNP) += libupnp
obj-$(CONFIG_FFMPEG) += ffmpeg
obj-$(CONFIG_DLNA_DMR) += dlna/dmr
obj-$(CONFIG_DLNA_DMS) += dlna/dms
obj-$(CONFIG_DEBUG_MONITOR) +=  debug_monitor
ifeq ($(strip $(BCA_HNDROUTER)),)
obj-$(CONFIG_SAMBA) += samba
else
# samba source is in userspace/gpl/apps/samba for BCA_HNDROUTER
obj-$(CONFIG_SAMBA) += samba/genpasswd
endif
obj-$(CONFIG_UTILS) += utils
obj-$(CONFIG_WLEXE) += wlexe
obj-$(CONFIG_ETC) += etc
obj-$(CONFIG_VLAN) += vlan
obj-$(CONFIG_NFC) += nfc
obj-$(CONFIG_EMF) += emf
obj-$(CONFIG_EMF) += igs
obj-$(CONFIG_IGMP_PROXY) += igmp
obj-$(CONFIG_WL_ACI) += aci
ifeq (2_6,$(LINUX_VERSION))
obj-y += udev
obj-y += hotplug2
endif
obj-$(CONFIG_LLD2D) += lltd/wrt54g-linux
#if defined(PHYMON)
obj-$(CONFIG_PHYMON_UTILITY) += phymon
#endif // endif
#if defined(EXT_ACS)
obj-$(CONFIG_EXTACS) += acsdv2
ACSDV2_DIR := $(BASEDIR)/components/apps/acsdv2
#endif // endif
#if defined(BCM_ESCAND)
ESCAND_DIR := $(BASEDIR)/components/apps/escand
ifneq ($(filter y,$(CONFIG_EAP) $(BUILD_HND_EAP)),)
obj-$(CONFIG_BCMESCAND) += escand
endif
#endif // endif
#if defined(BCM_BSD)
obj-$(CONFIG_BCMBSD) += bsd
#endif // endif
#if defined(BCM_SSD)
obj-$(CONFIG_BCMSSD) += ssd
#endif // endif
#if defined(BCM_ECBD)
ifeq ($(CMWIFI_RDKB),y)
obj-y += ecbd
endif
#endif // endif
#if defined(BCM_EVENTD)
obj-$(CONFIG_BCMEVENTD) += eventd
#endif // endif
#if defined(TOAD)
obj-$(CONFIG_TOAD) += toad
#endif // endif
#if defined(BCM_APPEVENTD)
obj-$(CONFIG_BCM_APPEVENTD) += appeventd
#endif // endif
#if defined(BCM_MEVENTD)
obj-$(CONFIG_BCM_MEVENTD) += meventd
#endif // endif
#if defined(BCM_DRSDBD)
obj-$(CONFIG_BCMDRSDBD) += drsdbd
#endif // endif
#if defined(BCM_MOCA)
obj-$(CONFIG_MOCA) += moca
#endif // endif

obj-$(CONFIG_VMSTAT) += vmstat
ifeq ($(PLT),arm)
ifeq ($(call wlan_version_ge,$(BCM_KVERSIONSTRING),2.6.36),TRUE)
obj-$(CONFIG_MPSTAT) += mpstat
obj-$(CONFIG_TASKSET) += taskset
endif
endif

ifeq ($(strip $(BCA_HNDROUTER)),)
ifeq ($(LINUX_VERSION),2_6_36)
IPROUTE2_DIR = iproute2-3.1.0
IPUTILS_DIR = iputils-s20101006
else
IPROUTE2_DIR = iproute2
IPUTILS_DIR = iputils
endif
obj-$(CONFIG_RADVD) += radvd
ifneq (,${CONFIG_IPROUTE2})
  obj-y += iproute2
endif
obj-$(CONFIG_IPUTILS) += iputils
ifneq (,$(filter y,${CONFIG_DHCPV6S} ${CONFIG_DHCPV6C}))
  obj-y += dhcp6
endif
#obj-$(CONFIG_DHCPV6S) += dhcp6s
#obj-$(CONFIG_DHCPV6C) += dhcp6c
endif # BCA_HNDROUTER

ifeq ($(CONFIG_STBAP),y)
obj-$(BUILD_BRCM_HOSTAPD) += openssl
endif
obj-$(BUILD_BRCM_HOSTAPD) += libnl
obj-$(BUILD_BRCM_HOSTAPD) += hostapd
obj-$(BUILD_BRCM_HOSTAPD) += wpa_supplicant
obj-$(BUILD_BRCM_HOSTAPD) += iw

# BUZZZ tools: function call tracing, performance monitoring, event history
obj-$(CONFIG_BUZZZ) += buzzz

# CONFIG_EAP is an Enterprise AP indication exported to
# other non-driver related modules, for example, the
# CLI command parser (wl.exe)
ifeq ($(CONFIG_EAP),y)
export CONFIG_EAP
export CFLAGS += -DWL_EAP_AP
# export CFLAGS += -DWL_PROXDETECT
ifeq ($(CONFIG_BCMESCAND),y)
export CFLAGS += -DWL_EAP_SCAN_TX
export CFLAGS += -DWL_EAP_SCAN_BEACON_DELAY
export CFLAGS += -DWL_EAP_DATA_SNOOP
endif
obj-y += tpdump
else
# BCM949408EAP platform builds (linux 4.1.x) require different check
ifeq ($(BUILD_HND_EAP),y)
export CONFIG_EAP := y
export CFLAGS += -DWL_EAP_AP
# export CFLAGS += -DWL_PROXDETECT
ifeq ($(CONFIG_BCMESCAND),y)
export CFLAGS += -DWL_EAP_SCAN_TX
export CFLAGS += -DWL_EAP_SCAN_BEACON_DELAY
export CFLAGS += -DWL_EAP_DATA_SNOOP
endif
endif
endif

ifeq ($(BUILD_BRCM_AIRIQ),y)
export CONFIG_AIRIQ := y
endif
ifneq ($(RDK_BUILD),y)
obj-$(CONFIG_AIRIQ) += airiq
endif

ifeq ($(BUILD_BRCM_HOSTAPD),y)
export CFLAGS += -DCONFIG_HOSTAPD
endif

#Air-IQ
airiq:
ifneq ($(CONFIG_AIRIQ),)
ifneq ($(wildcard $(AIRIQ_DIR)),)
	+$(MAKE) -C $(AIRIQ_DIR)
endif
endif

airiq-install:
ifneq ($(CONFIG_AIRIQ),)
ifneq ($(wildcard $(AIRIQ_DIR)),)
	+$(MAKE) -C $(AIRIQ_DIR) install INSTALLDIR=$(INSTALLDIR)/airiq
endif
endif
airiq-clean:
ifneq ($(CONFIG_AIRIQ),)
ifneq ($(wildcard $(AIRIQ_DIR)),)
	+$(MAKE) -C $(AIRIQ_DIR) clean
endif
endif
ifeq ($(BUILD_RDKWIFI),y)
export CONFIG_WLDM := y
obj-y += ecbd
export CFLAGS += -DBCM_ECBD
export BCM_ECBD := 1
endif

# WL Data Model lib and app
ifeq ($(BUILD_BRCM_WLDM),y)
export CONFIG_WLDM := y
endif

obj-$(CONFIG_WLDM) += wldm
WLDM_DIR := $(BASEDIR)/components/apps/wldm

.PHONY: wldm wldm-install wldm-clean
wldm:
ifneq ($(wildcard $(WLDM_DIR)),)
	$(MAKE) -C $(WLDM_DIR)
endif

wldm-install:
ifneq ($(wildcard $(WLDM_DIR)),)
	+$(MAKE) -C $(WLDM_DIR) install INSTALLDIR=$(INSTALLDIR)
endif

wldm-clean:
ifneq ($(wildcard $(WLDM_DIR)),)
	+$(MAKE) -C $(WLDM_DIR) clean
endif
# End of WL Data Model

# always build eap dispatcher
obj-y += eapd/linux
.PHONY: eapd eapd-clean eapd-install
eapd:
	+$(MAKE) -C eapd/linux
eapd-clean:
	+$(MAKE) -C eapd/linux clean
eapd-install:
	+$(MAKE) -C eapd/linux install

ifeq ($(CONFIG_VOIP),y)
obj-y += voipd
endif

#if 4908 or 47622 based platform
ifneq (,$(filter 4908 47622, $(BRCM_CHIP)))
export CONFIG_CSIMOND := y
endif
#endif 4908/47622

ifeq ($(CONFIG_CSIMOND),y)
obj-y += csimond
endif

ifeq ($(call wlan_version_ge,$(BCM_KVERSIONSTRING),2.6.36),TRUE)
obj-$(CONFIG_BCMASPMD) += aspmd
endif

obj-clean := $(foreach obj,$(obj-y) $(obj-n),$(obj)-clean)
obj-install := $(foreach obj,$(obj-y),$(obj)-install)

# separate the libraries which need to be built first
ifeq ($(CMWIFI_RDKB),)
obj-prelibs =$(filter nvram libbcmcrypto shared bshared netconf libupnp libz libid3tag ffmpeg libbcm nfc libsqlite3 libjsonc libxml2 pciefd, $(obj-y))
else
obj-prelibs =$(filter nvram libbcmcrypto shared bshared netconf libupnp libz libbcm nfc libjsonc pciefd, $(obj-y))
endif # CMWIFI_RDKB

ifeq ($(CONFIG_STBAP),y)
obj-prelibs += $(filter libbcmstb , $(obj-y))
endif

# remaining libraries that are built next
obj-postlibs := $(filter-out $(obj-prelibs), $(obj-y))

ifneq (2_4,$(LINUX_VERSION))
ifneq ($(shell grep "CONFIG_BLK_DEV_INITRD=y" $(LINUXDIR)/.config),)
ifeq ($(shell grep "CONFIG_BLK_DEV_RAM=y" $(LINUXDIR)/.config),)
export BUILD_MFG := 1
export WLTEST := 1
endif
endif
endif

ifneq ($(WLTEST),1)
ifneq ($(shell grep "CONFIG_EMBEDDED_RAMDISK=y" $(LINUXDIR)/.config),)
export WLTEST := 1
endif
endif

ifeq ($(WLTEST),1)
export CFLAGS += -DWLTEST
endif

ifeq ($(CONFIG_WLEXE),y)
export CONFIG_WLEXE
export RWL ?= 0
endif

ifeq ($(CONFIG_STBAP),y)
export CFLAGS	+= -DSTBAP
export CFLAGS += -D__CONFIG_STBAP__
export CFLAGS	+= -DSTB
export STB := 1
endif # CONFIG_STBAP

export BUILD_WLAN_TESTBED_AP

#
# Basic rules
#

# Following export values will be used in wl/config/wlconfig_apdef
export CONFIG_MFP
export CONFIG_HSPOT
export CONFIG_WNM
export CONFIG_FBT
export CONFIG_MBO_AP

all: version check_kernel_config obj_prelibs
	+$(MAKE) $(MAKE_ARGS) post_preplibs

obj_prelibs:
	+$(MAKE) parallel=true $(MAKE_ARGS) ARCH=$(ARCH) $(obj-prelibs)

obj_postlibs:
	+$(MAKE) parallel=true $(MAKE_ARGS) ARCH=$(ARCH) $(obj-postlibs)

post_preplibs:  obj_postlibs
        # Also build kernel

ifeq ($(CONFIG_STBAP),y)
	# skip building kernel
	@true
	+$(MAKE) $(MAKE_ARGS) stbap-modules
else
ifneq ($(strip $(BCA_HNDROUTER)),)
	# skip building kernel
	@true
else
ifneq (2_4,$(LINUX_VERSION))
ifneq ($(BUILD_MFG), 1)
ifdef ECLOUD_BUILD_ID
	# EMAKE build
	$(MAKE) -C ${LINUXDIR} $(SUBMAKE_SETTINGS) zImage
else # ECLOUD_BUILD_ID
	# GMAKE build
	(echo '.NOTPARALLEL:' ; cat ${LINUXDIR}/Makefile) |\
	    $(MAKE) -C ${LINUXDIR} -f - $(SUBMAKE_SETTINGS) zImage
endif # ECLOUD_BUILD_ID
	+$(MAKE) CONFIG_SQUASHFS=$(CONFIG_SQUASHFS) -C $(SRCBASE_ROUTER)/compressed ARCH=$(ARCH)
endif # BUILD_MFG
else  # LINUXDIR
ifdef ECLOUD_BUILD_ID
	# EMAKE build
	$(if $(shell grep "CONFIG_EMBEDDED_RAMDISK=y" ${LINUXDIR}/.config),,\
	  $(MAKE) -C ${LINUXDIR} $(SUBMAKE_SETTINGS) zImage)
else # ECLOUD_BUILD_ID
	# GMAKE build
	$(if $(shell grep "CONFIG_EMBEDDED_RAMDISK=y" ${LINUXDIR}/.config),, \
	  (echo '.NOTPARALLEL:' ; cat ${LINUXDIR}/Makefile) | \
	  $(MAKE) -C ${LINUXDIR} -f - $(SUBMAKE_SETTINGS) zImage \
	 )
endif # ECLOUD_BUILD_ID
endif # LINUXDIR
ifdef ECLOUD_BUILD_ID
	# EMAKE build
ifeq (2_6_36,$(LINUX_VERSION))
	# The each Kernel modules xx.mod.c need the symbols in vmlinux,
	# So how can we do the make modules before the vmlinux is ready?
	$(if $(shell grep "CONFIG_MODULES=y" ${LINUXDIR}/.config), \
	  $(MAKE) -C ${LINUXDIR} modules)
else
	$(if $(shell grep "CONFIG_MODULES=y" ${LINUXDIR}/.config), \
	  $(MAKE) -C ${LINUXDIR} -f - $(SUBMAKE_SETTINGS) MFG_WAR=1 zImage ; \
	  $(MAKE) -C ${LINUXDIR} modules)
endif
else # ECLOUD_BUILD_ID
	# GMAKE build
ifeq (2_6_36,$(LINUX_VERSION))
	# The each Kernel modules xx.mod.c need the symbols in vmlinux,
	# So how can we do the make modules before the vmlinux is ready?
	$(if $(shell grep "CONFIG_MODULES=y" ${LINUXDIR}/.config), \
	  (echo '.NOTPARALLEL:' ; cat ${LINUXDIR}/Makefile) | $(MAKE) -C ${LINUXDIR} -f - $(SUBMAKE_SETTINGS) MFG_WAR=1 zImage ; \
	  (echo '.NOTPARALLEL:' ; cat ${LINUXDIR}/Makefile) | $(MAKE) -C ${LINUXDIR} -f - ARCH=$(ARCH) modules)
else
	$(if $(shell grep "CONFIG_MODULES=y" ${LINUXDIR}/.config), \
	  (echo '.NOTPARALLEL:' ; cat ${LINUXDIR}/Makefile) | $(MAKE) -C ${LINUXDIR} -f - ARCH=$(ARCH) modules \
	 )
endif
endif # ECLOUD_BUILD_ID
	# Preserve the debug versions of these and strip for release
	$(call STRIP_DEBUG_SYMBOLS,$(LINUXDIR)/vmlinux)
ifneq (2_4,$(LINUX_VERSION))
	$(call STRIP_DEBUG_SYMBOLS,$(LINUXDIR)/drivers/net/wl/wl.ko)
	$(call STRIP_DEBUG_SYMBOLS,$(LINUXDIR)/drivers/net/et/et.ko)
	$(call STRIP_DEBUG_SYMBOLS,$(LINUXDIR)/drivers/net/ctf/ctf.ko)
	$(call STRIP_DEBUG_SYMBOLS,$(LINUXDIR)/drivers/net/dhd/dhd.ko)
	$(call STRIP_DEBUG_SYMBOLS,$(LINUXDIR)/drivers/net/bcm57xx/bcm57xx.ko)
	$(call STRIP_DEBUG_SYMBOLS,$(LINUXDIR)/drivers/net/emf/emf.ko)
	$(call STRIP_DEBUG_SYMBOLS,$(LINUXDIR)/drivers/net/igs/igs.ko)
	$(call STRIP_DEBUG_SYMBOLS,$(LINUXDIR)/drivers/net/dpsta/dpsta.ko)
	$(call STRIP_DEBUG_SYMBOLS,$(LINUXDIR)/drivers/connector/cn.ko)
	$(call STRIP_DEBUG_SYMBOLS,$(LINUXDIR)/drivers/scsi/scsi_wait_scan.ko)
	$(call STRIP_DEBUG_SYMBOLS,$(LINUXDIR)/drivers/usb/host/xhci-hcd.ko)
	$(call STRIP_DEBUG_SYMBOLS,$(LINUXDIR)/drivers/usb/host/ehci-hcd.ko)
	$(call STRIP_DEBUG_SYMBOLS,$(LINUXDIR)/drivers/usb/host/ohci-hcd.ko)
	$(call STRIP_DEBUG_SYMBOLS,$(LINUXDIR)/lib/libcrc32c.ko)
	$(call STRIP_DEBUG_SYMBOLS,$(LINUXDIR)/net/sched/sch_tbf.ko)
	$(call STRIP_DEBUG_SYMBOLS,$(LINUXDIR)/net/sched/sch_hfsc.ko)
else # LINUXDIR
	$(call STRIP_DEBUG_SYMBOLS,$(LINUXDIR)/drivers/net/wl/wl.o)
	$(call STRIP_DEBUG_SYMBOLS,$(LINUXDIR)/drivers/net/et/et.o)
	$(call STRIP_DEBUG_SYMBOLS,$(LINUXDIR)/drivers/net/ctf/ctf.o)
	$(call STRIP_DEBUG_SYMBOLS,$(LINUXDIR)/drivers/net/dhd/dhd.ko)
	$(call STRIP_DEBUG_SYMBOLS,$(LINUXDIR)/drivers/net/bcm57xx/bcm57xx.o)
	$(call STRIP_DEBUG_SYMBOLS,$(LINUXDIR)/drivers/net/emf/emf.o)
	$(call STRIP_DEBUG_SYMBOLS,$(LINUXDIR)/drivers/net/igs/igs.o)
endif # LINUXDIR
endif # BCA_HNDROUTER
endif # CONFIG_STBAP

# well, we should always be able to use the BOM, but right now, the last build step on
# the build machine doesn't have it, so we don't rerun this is the file already
# exists

.PHONY: version

version:  $(SRCBASE)/include/epivers.h

EPIVERS_OPTION =

$(SRCBASE)/include/epivers.h:
	$(MAKE) -C $(SRCBASE)/include $(EPIVERS_OPTION)
	$(MAKE) -C $(SRCBASE_SYS)/include
ifeq ($(CONFIG_DHDAP),y)
	$(MAKE) -C $(SRCBASE_DHD)/include
	$(MAKE) pciefd-version
endif

ifeq ($(CONFIG_STBAP),y)
nvram: shared

apps: obj_prelibs obj_postlibs

apps_config:
	$(MAKE) -C config
	./config/conf -o ./config/Config

apps_clean: $(obj-clean)
	rm -rf $(TARGETDIR)
endif # CONFIG_STBAP

router-clean: $(obj-clean) config-clean
ifeq ($(CONFIG_STBAP),y)
	+$(MAKE) $(MAKE_ARGS) stbap-modules-clean
	rm  $(BASEDIR)/components/router/.config.plt
endif # CONFIG_STBAP
	rm -rf $(TARGETDIR)
	rm -f $(PLATFORMDIR)/linux.trx $(PLATFORMDIR)/linux-gzip.trx
	rm -f $(PLATFORMDIR)/vmlinuz $(PLATFORMDIR)/vmlinuz-gzip
	rm -f $(PLATFORMDIR)/target.cramfs $(PLATFORMDIR)/target.squashfs
	rm -rf $(INSTALLDIR)/busybox

clean: router-clean
ifeq ($(strip $(BCA_HNDROUTER)),)
ifneq ($(CONFIG_STBAP),y)
	@echo cleaning LINUXDIR = $(LINUXDIR)
ifneq (2_4,$(LINUX_VERSION))
	# we need to pass some conf file for cleaning 2.6. The kbuild clean doesn't seem to
	# to load the .config the way our wl Makefile is expecting.
	$(MAKE) CONFIG_WL_CONF=wlconfig_lx_router_ap -C $(LINUXDIR) $(SUBMAKE_SETTINGS) clean
	$(MAKE) -C $(SRCBASE_ROUTER)/compressed ARCH=$(ARCH) clean
else
	$(MAKE) -C $(LINUXDIR) $(SUBMAKE_SETTINGS) clean
endif
	$(MAKE) -C $(BASEDIR)/components/cfe/build/broadcom/bcm947xx ARCH=$(ARCH) clean
endif # CONFIG_STBAP
endif # BCA_HNDROUTER
	[ ! -f $(SRCBASE)/tools/misc/Makefile ] || $(MAKE) -C $(SRCBASE)/tools/misc clean

distclean mrproper: clean
	rm -f .config .config.plt $(LINUXDIR)/.config

ifneq ($(CONFIG_STBAP),y)
install package: $(filter-out lib-install,$(obj-install)) check_kernel_config
else
apps_install: $(filter-out lib-install,$(obj-install))
endif
	# Install binaries into target directory
ifneq ($(strip $(BCA_HNDROUTER)),)
	for dir in $(wildcard $(patsubst %,$(INSTALLDIR)/%,$(obj-y))) ; do \
	    (cd $${dir} && tar --exclude="*_saved-*" -cpf - .) | (cd $(INSTALL_DIR) && tar xpf -) \
	done
else
	install -d $(TARGETDIR)
	for dir in $(wildcard $(patsubst %,$(INSTALLDIR)/%,$(obj-y))) ; do \
	    (cd $${dir} && tar cpf - .) | (cd $(TARGETDIR) && tar xpf -) \
	done

ifeq ($(CONFIG_STBAP),y)
install package: apps_install stbap-modules-install check_kernel_config
endif
	# optimize the crypto library by removing unneeded symbols
	$(if $(wildcard libbcmcrypto),+$(MAKE) -C libbcmcrypto optimize)
ifneq ("$(CONFIG_WAPI)$(CONFIG_WAPI_IAS)","")
	# optimize the OPENSSL library by removing unneeded symbols
#	[ ! -d wapi/wapid ] || $(MAKE) -C wapi/wapid optimize
endif
ifneq ($(CONFIG_STBAP),y)
	# Install (and possibly optimize) C library
	+$(MAKE) lib-install
	# Install modules into filesystem
	if grep -q "CONFIG_MODULES=y" $(LINUXDIR)/.config ; then \
	    $(MAKE) -C $(LINUXDIR) $(SUBMAKE_SETTINGS) \
		modules_install DEPMOD=/bin/true INSTALL_MOD_PATH=$(TARGETDIR) ; \
	fi
ifneq (2_4,$(LINUX_VERSION))
	find $(TARGETDIR) -name "wl_*.ko" | xargs rm -rf
else # Linux 2.4
	find $(TARGETDIR) -name "wl_*.o" | xargs rm -rf
endif
	# Prepare filesystem
	cd $(TARGETDIR) && $(TOP)/misc/rootprep.sh

ifeq ($(CONFIG_SQUASHFS), y)
ifeq (2_6_36,$(LINUX_VERSION))
	+$(MAKE) -C squashfs-4.2 mksquashfs
	squashfs-4.2/mksquashfs $(TARGETDIR) $(PLATFORMDIR)/$(ROOT_IMG) -noappend -all-root
else
	# Make sure mksquashfs 3.0 is used
	+$(MAKE) -C squashfs mksquashfs
	squashfs/mksquashfs $(TARGETDIR) $(PLATFORMDIR)/$(ROOT_IMG) -noappend -all-root
endif
else # CONFIG_SQUASHFS
	# Make sure mkcramfs-2.0 is used
	+$(MAKE) -C $(CRAMFSDIR) mkcramfs
	# '-u 0 -g 0' will set the uid and gid of all the files to 0 (root)
	# These options are currently only available on our version of mkcramfs
	$(CRAMFSDIR)/mkcramfs -u 0 -g 0 $(TARGETDIR) $(PLATFORMDIR)/$(ROOT_IMG)
endif # CONFIG_SQUASHFS

ifneq (2_4,$(LINUX_VERSION))
	# Package kernel and filesystem
ifeq ($(BUILD_MFG), 1)
	cd $(TARGETDIR) ; \
	find . | cpio -o -H newc | gzip > $(LINUXDIR)/usr/initramfs_data.cpio.gz
	ls -l $(LINUXDIR)/usr/initramfs_data.cpio.gz
ifdef ECLOUD_BUILD_ID
	# EMAKE build
	$(MAKE) -C ${LINUXDIR} $(SUBMAKE_SETTINGS) zImage
	+$(MAKE) CONFIG_SQUASHFS=$(CONFIG_SQUASHFS) -C $(SRCBASE_ROUTER)/compressed ARCH=$(ARCH)
else # ECLOUD_BUILD_ID
	# GMAKE build
	(echo '.NOTPARALLEL:' ; cat ${LINUXDIR}/Makefile) | $(MAKE) -C ${LINUXDIR} -f - $(SUBMAKE_SETTINGS) zImage
	+$(MAKE) CONFIG_SQUASHFS=$(CONFIG_SQUASHFS) -C $(SRCBASE_ROUTER)/compressed ARCH=$(ARCH)
endif # ECLOUD_BUILD_ID
else # BUILD_MFG
	cp $(SRCBASE_ROUTER)/compressed/vmlinuz $(PLATFORMDIR)/
	trx -o $(PLATFORMDIR)/linux.trx $(PLATFORMDIR)/vmlinuz $(PLATFORMDIR)/$(ROOT_IMG)
	cp $(SRCBASE_ROUTER)/compressed/vmlinuz-gzip $(PLATFORMDIR)/
	trx -o $(PLATFORMDIR)/linux-gzip.trx $(PLATFORMDIR)/vmlinuz-gzip $(PLATFORMDIR)/$(ROOT_IMG)
endif # BUILD_MFG
	# Pad self-booting Linux to a 64 KB boundary
	cp $(SRCBASE_ROUTER)/compressed/zImage $(PLATFORMDIR)/
else # LINUXDIR
ifdef ECLOUD_BUILD_ID
	# EMAKE build
	# Package kernel and filesystem
	$(if $(shell grep "CONFIG_EMBEDDED_RAMDISK=y" ${LINUXDIR}/.config), \
		cp ${PLATFORMDIR}/${ROOT_IMG} ${LINUXDIR}/arch/mips/ramdisk/$${CONFIG_EMBEDDED_RAMDISK_IMAGE} ; \
		$(MAKE) -C ${LINUXDIR} $(SUBMAKE_SETTINGS) zImage \
		, \
		cp $(LINUXDIR)/arch/mips/brcm-boards/bcm947xx/compressed/vmlinuz $(PLATFORMDIR)/ ; \
		trx -o $(PLATFORMDIR)/linux.trx $(PLATFORMDIR)/vmlinuz $(PLATFORMDIR)/$(ROOT_IMG) ; \
		cp $(LINUXDIR)/arch/mips/brcm-boards/bcm947xx/compressed/vmlinuz-gzip $(PLATFORMDIR)/ ; \
		trx -o $(PLATFORMDIR)/linux-gzip.trx $(PLATFORMDIR)/vmlinuz-gzip $(PLATFORMDIR)/$(ROOT_IMG) ; \
	)
else # ECLOUD_BUILD_ID
	# GMAKE build
	# Package kernel and filesystem
	$(if $(shell grep "CONFIG_EMBEDDED_RAMDISK=y" ${LINUXDIR}/.config), \
		cp ${PLATFORMDIR}/${ROOT_IMG} ${LINUXDIR}/arch/mips/ramdisk/$${CONFIG_EMBEDDED_RAMDISK_IMAGE} ; \
		(echo '.NOTPARALLEL:' ; cat ${LINUXDIR}/Makefile) | \
		$(MAKE) -C ${LINUXDIR} -f - $(SUBMAKE_SETTINGS) zImage \
		, \
		cp $(LINUXDIR)/arch/mips/brcm-boards/bcm947xx/compressed/vmlinuz $(PLATFORMDIR)/ ; \
		trx -o $(PLATFORMDIR)/linux.trx $(PLATFORMDIR)/vmlinuz $(PLATFORMDIR)/$(ROOT_IMG) ; \
		cp $(LINUXDIR)/arch/mips/brcm-boards/bcm947xx/compressed/vmlinuz-gzip $(PLATFORMDIR)/ ; \
		trx -o $(PLATFORMDIR)/linux-gzip.trx $(PLATFORMDIR)/vmlinuz-gzip $(PLATFORMDIR)/$(ROOT_IMG) ; \
	)
endif # ECLOUD_BUILD_ID
	# Pad self-booting Linux to a 64 KB boundary
	cp $(LINUXDIR)/arch/mips/brcm-boards/bcm947xx/compressed/zImage $(PLATFORMDIR)/
endif # LINUXDIR
	dd conv=sync bs=64k < $(PLATFORMDIR)/zImage > $(PLATFORMDIR)/linux.bin
	# Append filesystem to self-booting Linux
	cat $(PLATFORMDIR)/$(ROOT_IMG) >> $(PLATFORMDIR)/linux.bin
endif # BCA_HNDROUTER
endif # CONFIG_STBAP

# sibling target is specific for release build scripts used. src/router/misc/toplevel-release.mk will do it.
# Create linux-ap.trx or linux-apsta.trx and copy to $(SIBLING_INSTALL_DIR) foldir.
sibling:
ifneq ($(BUILD_MFG), 1)
	if grep -q "CONFIG_WL_USE_AP=y" $(LINUXDIR)/.config ; then \
                cp $(LINUXDIR)/.config $(LINUXDIR)/.config.chk ; \
                sed -e "s/CONFIG_WL_USE_AP=y/# CONFIG_WL_USE_AP is not set/g" $(LINUXDIR)/.config.chk | \
                sed -e "s/# CONFIG_WL_USE_APSTA is not set/CONFIG_WL_USE_APSTA=y/g" > \
                $(LINUXDIR)/.config ; \
                rm -f $(LINUXDIR)/.config.chk ; \
		cp $(SIBLING_INSTALL_DIR)/linux.trx $(SIBLING_INSTALL_DIR)/linux-ap.trx ; \
		$(MAKE) oldconfig ; \
		$(MAKE) wps-clean ; \
		$(MAKE) all ; \
		$(MAKE) install ; \
		cp $(PLATFORMDIR)/linux.trx $(SIBLING_INSTALL_DIR)/linux-apsta.trx ; \
		cp $(PLATFORMDIR)/linux-gzip.trx $(SIBLING_INSTALL_DIR)/linux-apsta-gzip.trx ; \
	elif grep -q "CONFIG_WL_USE_APSTA=y" $(LINUXDIR)/.config ; then \
                cp $(LINUXDIR)/.config $(LINUXDIR)/.config.chk ; \
                sed -e "s/CONFIG_WL_USE_APSTA=y/# CONFIG_WL_USE_APSTA is not set/g" $(LINUXDIR)/.config.chk | \
                sed -e "s/# CONFIG_WL_USE_AP is not set/CONFIG_WL_USE_AP=y/g" > \
                $(LINUXDIR)/.config ; \
                rm -f $(LINUXDIR)/.config.chk ; \
		cp $(SIBLING_INSTALL_DIR)/linux.trx $(SIBLING_INSTALL_DIR)/linux-apsta.trx ; \
		$(MAKE) oldconfig ; \
		$(MAKE) wps-clean ; \
		$(MAKE) all ; \
		$(MAKE) install ; \
		cp $(PLATFORMDIR)/linux.trx $(SIBLING_INSTALL_DIR)/linux-ap.trx ; \
		cp $(PLATFORMDIR)/linux-gzip.trx $(SIBLING_INSTALL_DIR)/linux-ap-gzip.trx ; \
	fi
endif
	# MFG will not build TRX
	@true;

#
# Configuration rules
#

conf mconf:
	+$(MAKE) -C config LINUXDIR=${LINUXDIR}
	@LINUXDIR=${LINUXDIR} ./config/$@ ./config/Config
ifeq ($(strip $(BCA_HNDROUTER)),)
ifneq ($(CONFIG_STBAP),y)
	# Also configure kernel
	+$(MAKE) LINUXDIR=${LINUXDIR} k$@
endif # CONFIG_STBAP
endif

oldconf: .config
	+$(MAKE) -C config LINUXDIR=${LINUXDIR}
	@LINUXDIR=${LINUXDIR} ./config/conf -o ./config/Config
ifeq ($(strip $(BCA_HNDROUTER)),)
ifneq ($(CONFIG_STBAP),y)
	# Also configure kernel
	+$(MAKE) LINUXDIR=${LINUXDIR} k$@
endif # CONFIG_STBAP
endif

kconf:
	+$(MAKE) -C $(LINUXDIR) $(SUBMAKE_SETTINGS) config

kmconf: check_kernel_config
	+$(MAKE) -C $(LINUXDIR) $(SUBMAKE_SETTINGS) menuconfig

koldconf: check_kernel_config
	+$(MAKE) -C $(LINUXDIR) $(SUBMAKE_SETTINGS) oldconfig

# Convenience
config: conf

menuconfig: mconf

oldconfig: oldconf

# Platform file
.config.plt:
	@echo "PLT=$(PLT)" > $@
	@echo "LINUX_VERSION=$(LINUX_VERSION)" >> $@

# Default configurations
.config:
ifneq ($(CONFIG_STBAP),y)
ifneq (2_4,$(LINUX_VERSION))
ifneq ($(strip $(BCA_CPEROUTER)),)
	cp config/defconfig-4.1-vista-dhdap-bca-cperouter $@
else
ifneq ($(strip $(BCA_HNDROUTER)),)
ifneq ($(strip $(BUILD_HND_NIC)),)
	cp config/defconfig-4.1-vista-router $@
else
ifneq ($(strip $(CMWIFI)),)
	cp config/defconfig-cmwifi-dhdap $@
else
	cp config/defconfig-4.1-vista-dhdap-router $@
endif # CMWIFI
endif
else
	cp config/defconfig-2.6 $@
endif
endif
else
	cp config/defconfig $@
endif
	+$(MAKE) SRCBASE=$(SRCBASE) LINUXDIR=$(LINUXDIR) oldconfig
endif # CONFIG_STBAP

ifneq ($(CONFIG_STBAP),y)
$(LINUXDIR)/.config:
ifeq ($(CMWIFI),)
ifneq (2_4,$(LINUX_VERSION))
	cp $(LINUXDIR)/arch/$(ARCH)/defconfig-2.6-bcm947xx $@
else
	cp $(LINUXDIR)/arch/$(ARCH)/defconfig-bcm947xx $@
endif
endif # not CMWIFI
endif # CONFIG_STBAP

ifneq ($(CONFIG_STBAP),y)
# Overwrite Kernel .config
check_kernel_config: $(LINUXDIR)/.config
endif # CONFIG_STBAP
ifeq ($(strip $(BCA_HNDROUTER)),)
ifneq ($(CONFIG_STBAP),y)
	cp $(LINUXDIR)/.config $(LINUXDIR)/.config.tmp
endif # CONFIG_STBAP
ifeq ($(CONFIG_SQUASHFS), y)
	if ! grep -q "CONFIG_SQUASHFS=y" $(LINUXDIR)/.config ; then \
		cp $(LINUXDIR)/.config $(LINUXDIR)/.config.chk ; \
		sed -e "s/CONFIG_CRAMFS=y/# CONFIG_CRAMFS is not set/g" $(LINUXDIR)/.config.chk | \
		sed -e "s/# CONFIG_SQUASHFS is not set/CONFIG_SQUASHFS=y/g" > \
		$(LINUXDIR)/.config ; \
		rm -f $(LINUXDIR)/.config.chk ; \
	fi
endif
ifeq ($(CONFIG_CRAMFS), y)
	if ! grep -q "CONFIG_CRAMFS=y" $(LINUXDIR)/.config ; then \
		cp $(LINUXDIR)/.config $(LINUXDIR)/.config.chk ; \
		sed -e "s/CONFIG_SQUASHFS=y/# CONFIG_SQUASHFS is not set/g" $(LINUXDIR)/.config.chk | \
		sed -e "s/# CONFIG_CRAMFS is not set/CONFIG_CRAMFS=y/g" > \
		$(LINUXDIR)/.config ; \
		rm -f $(LINUXDIR)/.config.chk ; \
	fi
endif
ifeq ($(CONFIG_SHRINK_MEMORY), y)
	if ! grep -q "CONFIG_SHRINKMEM=y" $(LINUXDIR)/.config ; then \
		cp $(LINUXDIR)/.config $(LINUXDIR)/.config.chk ; \
		sed -e "s/# CONFIG_SHRINKMEM is not set/CONFIG_SHRINKMEM=y/g" $(LINUXDIR)/.config.chk > \
		$(LINUXDIR)/.config ; \
		rm -f $(LINUXDIR)/.config.chk ; \
	fi
else
	if grep -q "CONFIG_SHRINKMEM=y" $(LINUXDIR)/.config ; then \
		cp $(LINUXDIR)/.config $(LINUXDIR)/.config.chk ; \
		sed -e "s/CONFIG_SHRINKMEM=y/# CONFIG_SHRINKMEM is not set/g" $(LINUXDIR)/.config.chk > \
		$(LINUXDIR)/.config ; \
		rm -f $(LINUXDIR)/.config.chk ; \
	fi
endif
	# Do not override CONFIG_PLAT_MUX_CONSOLE if CONFIG_NFC is disable, others may enable it.
ifeq ($(CONFIG_NFC), y)
	if ! grep -q "CONFIG_PLAT_MUX_CONSOLE=y" $(LINUXDIR)/.config ; then \
		cp $(LINUXDIR)/.config $(LINUXDIR)/.config.chk ; \
		sed -e "s/# CONFIG_PLAT_MUX_CONSOLE is not set/CONFIG_PLAT_MUX_CONSOLE=y/g" $(LINUXDIR)/.config.chk > \
		$(LINUXDIR)/.config ; \
		rm -f $(LINUXDIR)/.config.chk ; \
	fi
endif
	# Make kernel config again if changed
	$(if $(shell [ ! cmp ${LINUXDIR}/.config ${LINUXDIR}/.config.tmp >/dev/null 2>&1 ] && echo "y"), \
	+$(MAKE) -C ${LINUXDIR} $(SUBMAKE_SETTINGS) oldconfig < /dev/null \
	)
endif # BCA_HNDROUTER

#
# Overrides
#

ifneq (2_4,$(LINUX_VERSION))
ifeq ($(CONFIG_BUSYBOX),y)
# See if .config has changed. This file gets a new date stamp at each build,
# so we always have exactly 4 lines of diffs. Anything else, we rebuild.
ifeq ($(strip $(BCA_HNDROUTER)),)
ifeq ($(CMWIFI),)
BB_DIFF_LINES=$(shell diff busybox-1.x/.config busybox-1.x/configs/bbconfig-$(CONFIG_BUSYBOX_CONFIG) | wc -l)
endif
endif

busybox-1.x/.config:
	cd $(@D) && cp configs/bbconfig-$(CONFIG_BUSYBOX_CONFIG) .config && $(MAKE) oldconfig

busybox-1.x/Config.h: dummy
	cd $(@D) && rm -f Config.h && ln -sf include/autoconf.h Config.h && touch Config.h
ifneq ($(BB_DIFF_LINES),4)
	cd $(@D) && cp configs/bbconfig-$(CONFIG_BUSYBOX_CONFIG) .config && $(MAKE) clean
endif

.PHONY: busybox
busybox: busybox-1.x/.config busybox-1.x/Config.h
	+$(MAKE) -C busybox-1.x ARCH=$(ARCH) INSTALL

busybox-install:
	+$(MAKE) -C busybox-1.x ARCH=$(ARCH) CONFIG_PREFIX=$(INSTALLDIR)/busybox install

busybox-clean:
	+$(MAKE) -C busybox-1.x ARCH=$(ARCH) clean
endif

ifeq ($(BUILD_BRCM_HOSTAPD),y)
rc: netconf nvram shared wpa_supplicant
else
rc: netconf nvram shared
endif
	+$(MAKE) LINUXDIR=$(LINUXDIR) EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)" -C rc all

ifneq ($(CONFIG_BUSYBOX),)
ifeq ($(strip $(BCA_HNDROUTER)),)
rc: busybox-1.x/Config.h
endif # BCA_HNDROUTER
endif
else #linux-2.6
ifeq ($(CONFIG_BUSYBOX),y)
CURBBCFG := $(CONFIG_BUSYBOX_CONFIG).h
OLDBBCFG := $(shell basename $$(readlink busybox/Config.h) 2> /dev/null)
busybox/Config.h: dummy
ifneq ($(OLDBBCFG),$(CURBBCFG))
	cd busybox && rm -f Config.h && ln -sf configs/$(CURBBCFG) Config.h && touch Config.h
endif

busybox: busybox/Config.h
	+$(MAKE) -C busybox STRIPTOOL=$(STRIP)

busybox-install:
	+$(MAKE) -C busybox STRIPTOOL=$(STRIP) PREFIX=$(INSTALLDIR)/busybox install
endif

rc: netconf nvram shared
ifneq ($(CONFIG_BUSYBOX),)
rc: busybox/Config.h
endif
endif #linux-2.6

rc-install: rc
	+$(MAKE) LINUXDIR=$(LINUXDIR) INSTALLDIR=$(INSTALLDIR)/rc -C rc install

lib-install:
	+$(MAKE) LX_VERS=$(LINUX_VERSION) INSTALLDIR=$(INSTALLDIR)/lib ARCH=$(ARCH) -C lib install

www:
	+$(MAKE) -C www/$(CONFIG_VENDOR) INSTALLDIR=$(INSTALLDIR)/www

www-install:
ifneq ($(CONFIG_STBAP),y)
	+$(MAKE) -C www/$(CONFIG_VENDOR) install INSTALLDIR=$(INSTALLDIR)/www
else
	+$(MAKE) -C www/$(CONFIG_VENDOR) install INSTALLDIR=$(INSTALLDIR)/www PLT=arm
endif

www-clean:
	+$(MAKE) -C www/$(CONFIG_VENDOR) clean INSTALLDIR=$(INSTALLDIR)/www

ebtables:
ifeq ($(CONFIG_WBD), y)
ifneq ($(wildcard $(EBTABLES_DIR)),)
	+$(MAKE) -C $(EBTABLES_DIR) CROSS=$(CROSS_COMPILE) EXTRA_LDFLAGS=$(EXTRA_LDFLAGS)
endif
endif

ebtables-install:
ifeq ($(CONFIG_WBD), y)
ifneq ($(wildcard $(EBTABLES_DIR)),)
	+$(MAKE) -C $(EBTABLES_DIR) CROSS=$(CROSS_COMPILE) install
endif
endif

ebtables-clean:
ifeq ($(CONFIG_WBD), y)
ifneq ($(wildcard $(EBTABLES_DIR)),)
	+$(MAKE) -C $(EBTABLES_DIR) CROSS=$(CROSS_COMPILE) clean
endif
endif

ieee1905:
ifeq ($(CONFIG_WBD), y)
ifneq ($(wildcard $(IEEE1905SOURCE_DIR)),)
	+$(MAKE) -C $(IEEE1905SOURCE_DIR) CROSS=$(CROSS_COMPILE) EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)"
endif
endif

ieee1905-install:
ifeq ($(CONFIG_WBD), y)
ifneq ($(wildcard $(IEEE1905SOURCE_DIR)),)
	+$(MAKE) -C $(IEEE1905SOURCE_DIR) CROSS=$(CROSS_COMPILE) install
endif
endif

ieee1905-clean:
ifeq ($(CONFIG_WBD), y)
ifneq ($(wildcard $(IEEE1905SOURCE_DIR)),)
	+$(MAKE) -C $(IEEE1905SOURCE_DIR) CROSS=$(CROSS_COMPILE) clean
endif
endif

wbd: ieee1905
ifeq ($(CONFIG_WBD), y)
ifneq ($(wildcard $(WBDSOURCE_DIR)),)
	+$(MAKE) -C $(WBDSOURCE_DIR) CROSS=$(CROSS_COMPILE) EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)"

endif
endif

wbd-install:
ifeq ($(CONFIG_WBD),y)
	+$(MAKE) -C $(WBDSOURCE_DIR) CROSS=$(CROSS_COMPILE) install
endif

wbd-clean:
ifeq ($(CONFIG_WBD),y)
ifneq ($(wildcard $(WBDSOURCE_DIR)),)
	+$(MAKE) -C $(WBDSOURCE_DIR) CROSS=$(CROSS_COMPILE) clean
endif
endif

#if defined(PREBUILD)
ifeq ($(WLTEST),1)
BMAC_FIRM =ag-nodis-mfgtest
else
BMAC_FIRM =ag-nodis
endif

ifneq ($(CONFIG_DHDAP),)
extfdimg : $(SRCBASE_DHD)/dongle/bin
ifneq ($(BCM_EXTFDIMAGE_PATH),)
#pciefd should make the images into dhd shared
#check if extfdimages are present in the dhd
	@true
endif

extfdimg-clean :
ifneq ($(BCM_EXTFDIMAGE_PATH),)
	rm -r $(INSTALLDIR)/extfdimg/$(BCM_EXTFDIMAGE_PATH)
endif

extfdimg-install :
ifneq ($(strip $(BUILD_HND_NIC)),)
# Not building pcie dongle fw images shortens build time
else
ifeq ($(CMWIFI),)
ifneq ($(BCM_EXTFDIMAGE_PATH),)
	install -d $(INSTALLDIR)/extfdimg/$(BCM_EXTFDIMAGE_PATH)
	cp -r $(SRCBASE_DHD)/dongle/bin/* $(INSTALLDIR)/extfdimg/$(BCM_EXTFDIMAGE_PATH)
endif
endif
endif
endif

.PHONY: dhd
dhd:
ifeq ($(CONFIG_DHDAP),y)
ifneq ($(wildcard $(SRCBASE_DHD)/dhd/exe),)
ifeq ($(CMWIFI_RDKB),)
	$(MAKE) TARGET_PREFIX=$(CROSS_COMPILE) -C $(SRCBASE_DHD)/dhd/exe
else
	$(MAKE) -C $(SRCBASE_DHD)/dhd/exe CC="$(CC)"
endif # CMWIFI_RDKB
endif
endif

.PHONY: dhd-clean
dhd-clean :
ifneq ($(wildcard $(SRCBASE_DHD)/dhd/exe),)
	$(MAKE) TARGET_PREFIX=$(CROSS_COMPILE) -C $(SRCBASE_DHD)/dhd/exe clean
endif

.PHONY: dhd-install
dhd-install :
ifeq ($(strip $(BUILD_HND_NIC)),)
ifeq ($(CONFIG_DHDAP),y)
ifneq ($(wildcard $(SRCBASE_DHD)/dhd/exe),)
	install -d $(INSTALLDIR)/dhd/usr/sbin
	install $(SRCBASE_DHD)/dhd/exe/dhd$(TARGETARCH) $(INSTALLDIR)/dhd/usr/sbin/dhd
	$(STRIP) $(INSTALLDIR)/dhd/usr/sbin/dhd
endif
endif
endif

ifeq ($(CONFIG_DEBUG_MONITOR),y)
.PHONY: debug_monitor
debug_monitor: shared nvram
	+$(MAKE) LINUXDIR=$(LINUXDIR) EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)" -C dhd_monitor debug_monitor

.PHONY: debug_monitor-install
debug_monitor-install: debug_monitor
	+$(MAKE) -C dhd_monitor INSTALLDIR=$(INSTALLDIR)/debug_monitor install

.PHONY: debug_monitor-clean
debug_monitor-clean:
	+$(MAKE) -C dhd_monitor clean
endif

bridge:
ifneq (2_4,$(LINUX_VERSION))
	+$(MAKE) -C bridge-1.x
else
	+$(MAKE) -C bridge brctl/brctl
endif

bridge-install:
ifneq (2_4,$(LINUX_VERSION))
	install -D bridge-1.x/brctl/brctl $(INSTALLDIR)/bridge/usr/sbin/brctl
else
	install -D bridge/brctl/brctl $(INSTALLDIR)/bridge/usr/sbin/brctl
endif
	$(STRIP) $(INSTALLDIR)/bridge/usr/sbin/brctl

bridge-clean:
ifneq (2_4,$(LINUX_VERSION))
	-$(MAKE) -C bridge-1.x clean
else
	-$(MAKE) -C bridge clean
endif

ifeq ($(call wlan_version_ge,$(BCM_KVERSIONSTRING),2.6.36),TRUE)
dnsmasq:
	+$(MAKE) -C $(DNSMASQ_DIR)

dnsmasq-install:
	install -D $(DNSMASQ_DIR)/src/dnsmasq $(INSTALLDIR)/dnsmasq/usr/sbin/dnsmasq
	$(STRIP) $(INSTALLDIR)/dnsmasq/usr/sbin/dnsmasq
else
dnsmasq-install:
	install -D dnsmasq/dnsmasq $(INSTALLDIR)/dnsmasq/usr/sbin/dnsmasq
	$(STRIP) $(INSTALLDIR)/dnsmasq/usr/sbin/dnsmasq
endif

libstdcpp:
	# So that generic rule does not take precedence
	@:

libstdcpp-install:
	install -D $(TOOLCHAIN)/usr/lib/libstdc++.so.6.0.14 $(INSTALLDIR)/libstdcpp/usr/lib/libstdc++.so.6.0.14
	+$(STRIP) $(INSTALLDIR)/libstdcpp/usr/lib/libstdc++.so.6.0.14

$(LIBMNL_DIR)/Makefile:
	cd $(LIBMNL_DIR) && \
	 CC=$(CC) ./configure \
	    --target=$(PLATFORM)-linux \
	    --host=$(PLATFORM)-linux \
	    --build=`/bin/arch`-linux \
	    --with-kernel=$(LINUXDIR) \
	    --prefix=$(LIBMNL_DIR)/install

libmnl: $(LIBMNL_DIR)/Makefile
	+$(MAKE) -C $(LIBMNL_DIR)
	+$(MAKE) -C $(LIBMNL_DIR) install-strip

libmnl-install:
	install -d $(INSTALLDIR)/libmnl/usr/lib
	cp -rf $(LIBMNL_DIR)/install/lib/* $(INSTALLDIR)/libmnl/usr/lib
	$(RM) -r $(INSTALLDIR)/libmnl/usr/lib/pkgconfig
	$(RM) $(INSTALLDIR)/libmnl/usr/lib/*.la

libmnl-clean:
	+$(MAKE) -C $(LIBMNL_DIR) KERNEL_DIR=$(LINUXDIR) clean

$(LIBNFNETLINK_DIR)/Makefile:
	cd $(LIBNFNETLINK_DIR) && \
	 CC=$(CC) ./configure \
	    --target=$(PLATFORM)-linux \
	    --host=$(PLATFORM)-linux \
	    --build=`/bin/arch`-linux \
	    --with-kernel=$(LINUXDIR) \
	    --prefix=$(LIBNFNETLINK_DIR)/install

libnfnetlink: $(LIBNFNETLINK_DIR)/Makefile
	+$(MAKE) -C $(LIBNFNETLINK_DIR)
	+$(MAKE) -C $(LIBNFNETLINK_DIR) install-strip

libnfnetlink-install:
	install -d $(INSTALLDIR)/libnfnetlink/usr/lib
	cp -rf $(LIBNFNETLINK_DIR)/install/lib/* $(INSTALLDIR)/libnfnetlink/usr/lib
	$(RM) -r $(INSTALLDIR)/libnfnetlink/usr/lib/pkgconfig
	$(RM) $(INSTALLDIR)/libnfnetlink/usr/lib/*.la

libnfnetlink-clean:
	+$(MAKE) -C $(LIBNFNETLINK_DIR) KERNEL_DIR=$(LINUXDIR) clean

$(LIBNETFILTER_CONNTRACK_DIR)/Makefile:
	cd $(LIBNETFILTER_CONNTRACK_DIR) && \
	 CC=$(CC) ./configure \
	    --target=$(PLATFORM)-linux \
	    --host=$(PLATFORM)-linux \
	    --build=`/bin/arch`-linux \
	    --with-kernel=$(LINUXDIR) \
	    --prefix=$(LIBNETFILTER_CONNTRACK_DIR)/install \
        LIBNFNETLINK_CFLAGS=$(NFNETLINK_CFLAGS) LIBNFNETLINK_LIBS=$(NFNETLINK_LIBS) \
        LIBMNL_CFLAGS=$(MNL_CFLAGS) LIBMNL_LIBS=$(MNL_LIBS)

libnetfilter_conntrack: libmnl libnfnetlink $(LIBNETFILTER_CONNTRACK_DIR)/Makefile
	+$(MAKE) -C $(LIBNETFILTER_CONNTRACK_DIR)
	+$(MAKE) -C $(LIBNETFILTER_CONNTRACK_DIR) install-strip

libnetfilter_conntrack-install:
	install -d $(INSTALLDIR)/libnetfilter_conntrack/usr/lib
	cp -rf $(LIBNETFILTER_CONNTRACK_DIR)/install/lib/* $(INSTALLDIR)/libnetfilter_conntrack/usr/lib
	$(RM) -r $(INSTALLDIR)/libnetfilter_conntrack/usr/lib/pkgconfig
	$(RM) $(INSTALLDIR)/libnetfilter_conntrack/usr/lib/*.la

libnetfilter_conntrack-clean:
	+$(MAKE) -C $(LIBNETFILTER_CONNTRACK_DIR) KERNEL_DIR=$(LINUXDIR) clean

$(LIBNETFILTER_QUEUE_DIR)/Makefile:
	cd $(LIBNETFILTER_QUEUE_DIR) && \
	 CC=$(CC) ./configure \
	    --target=$(PLATFORM)-linux \
	    --host=$(PLATFORM)-linux \
	    --build=`/bin/arch`-linux \
	    --with-kernel=$(LINUXDIR) \
	    --prefix=$(LIBNETFILTER_QUEUE_DIR)/install \
        LIBNFNETLINK_CFLAGS=$(NFNETLINK_CFLAGS) LIBNFNETLINK_LIBS=$(NFNETLINK_LIBS) \
        LIBMNL_CFLAGS=$(MNL_CFLAGS) LIBMNL_LIBS=$(MNL_LIBS)

libnetfilter_queue: libmnl libnfnetlink $(LIBNETFILTER_QUEUE_DIR)/Makefile
	+$(MAKE) -C $(LIBNETFILTER_QUEUE_DIR)
	+$(MAKE) -C $(LIBNETFILTER_QUEUE_DIR) install-strip

libnetfilter_queue-install:
	install -d $(INSTALLDIR)/libnetfilter_queue/usr/lib
	cp -rf $(LIBNETFILTER_QUEUE_DIR)/install/lib/* $(INSTALLDIR)/libnetfilter_queue/usr/lib
	$(RM) -r $(INSTALLDIR)/libnetfilter_queue/usr/lib/pkgconfig
	$(RM) $(INSTALLDIR)/libnetfilter_queue/usr/lib/*.la

libnetfilter_queue-clean:
	+$(MAKE) -C $(LIBNETFILTER_QUEUE_DIR) KERNEL_DIR=$(LINUXDIR) clean

LIBFLOW_DIR := $(BASEDIR)/components/shared/libflow

libflow: libnetfilter_conntrack
	$(MAKE) -C $(LIBFLOW_DIR) LIBMNL_CFLAGS=$(MNL_CFLAGS) LIBMNL_LIBS=$(MNL_LIBS) \
	    LIBNFNETLINK_CFLAGS=$(NFNETLINK_CFLAGS) LIBNFNETLINK_LIBS=$(NFNETLINK_LIBS) \
	    LIBNETFILTER_CONNTRACK_CFLAGS=$(NETFILTER_CONNTRACK_CFLAGS) LIBNETFILTER_CONNTRACK_LIBS=$(NETFILTER_CONNTRACK_LIBS)

libflow-install:
	install -d $(INSTALLDIR)/libflow/usr/lib
	install -D $(LIBFLOW_DIR)/libflow.so $(INSTALLDIR)/libflow/usr/lib
	$(STRIP) $(INSTALLDIR)/libflow/usr/lib/libflow.so

libflow-clean:
	$(MAKE) -C $(LIBFLOW_DIR) clean

ifeq ($(CONFIG_VISUALIZATION),y)
$(LIBSQLITE3_DIR)/Makefile:
	cd $(LIBSQLITE3_DIR) && \
	 CC=$(CC) ./configure \
            --target=$(PLATFORM)-linux \
            --host=$(PLATFORM)-linux \
            --enable-static=no \
            --prefix=$(LIBSQLITE3_DIR)/installbin

libsqlite3: $(LIBSQLITE3_DIR)/Makefile
	+$(MAKE) -C $(LIBSQLITE3_DIR)
	+$(MAKE) -C $(LIBSQLITE3_DIR) install-strip
else
libsqlite3:
endif

libsqlite3-install:
ifeq ($(CONFIG_VISUALIZATION),y)
	install -d $(INSTALLDIR)/libsqlite3/usr/lib
	cp -rf $(LIBSQLITE3_DIR)/installbin/lib/libsqlite3.so* $(INSTALLDIR)/libsqlite3/usr/lib
endif

libsqlite3-clean:
ifeq ($(CONFIG_VISUALIZATION),y)
	+$(MAKE) -C $(LIBSQLITE3_DIR) distclean
endif

$(LIBJSONC_DIR)/configure:
	cd $(LIBJSONC_DIR) && \
         autoreconf --force --install

$(LIBJSONC_DIR)/Makefile: $(LIBJSONC_DIR)/configure
	cd $(LIBJSONC_DIR) && \
         CC="$(CC)" ./configure \
            --target=$(TARGET_PLT) \
            --host=$(TARGET_PLT) \
            --disable-oldname-compat \
            --enable-static=no \
            --prefix=$(LIBJSONC_DIR)/installbin

libjsonc: $(LIBJSONC_DIR)/Makefile
	+$(MAKE) -C $(LIBJSONC_DIR)
	+$(MAKE) -C $(LIBJSONC_DIR) install-strip

libjsonc-install:
	install -d $(INSTALLDIR)/libjsonc/usr/lib
	cp -rf $(LIBJSONC_DIR)/installbin/lib/libjson-c.so* $(INSTALLDIR)/libjsonc/usr/lib

libjsonc-clean: $(LIBJSONC_DIR)/Makefile
	+$(MAKE) -C $(LIBJSONC_DIR) distclean

ifeq ($(CONFIG_VISUALIZATION),y)
$(LIBXML2_DIR)/Makefile:
	cd $(LIBXML2_DIR) && \
         CC=$(CC) ./configure \
            --target=$(PLATFORM)-linux \
            --host=$(PLATFORM)-linux \
            --with-python=no \
            --without-zlib \
            --without-ftp \
            --without-http \
            --without-html \
            --without-debug \
            --without-docbook \
            --without-catalog \
            --without-schemas \
            --enable-static=no \
            --prefix=$(LIBXML2_DIR)/installbin

libxml2: $(LIBXML2_DIR)/Makefile
	+$(MAKE) -C $(LIBXML2_DIR)
	+$(MAKE) -C $(LIBXML2_DIR) install-strip
else
libxml2:
endif

libxml2-install:
ifeq ($(CONFIG_VISUALIZATION),y)
	install -d $(INSTALLDIR)/libxml2/usr/lib
	cp -rf $(LIBXML2_DIR)/installbin/lib/libxml2.so* $(INSTALLDIR)/libxml2/usr/lib
endif

libxml2-clean:
ifeq ($(CONFIG_VISUALIZATION),y)
	+$(MAKE) -C $(LIBXML2_DIR) distclean
endif

hspot_ap:
ifeq ($(CONFIG_HSPOT),y)
	+$(MAKE) -C $(PASSPOINT_AP_DIR) EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)"
endif
hspot_ap-install:
ifeq ($(CONFIG_HSPOT),y)
	+$(MAKE) -C $(PASSPOINT_AP_DIR) INSTALLDIR=$(INSTALLDIR)/hspot_ap install EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)"
endif
hspot_ap-clean:
ifeq ($(CONFIG_HSPOT),y)
	+$(MAKE) -C $(PASSPOINT_AP_DIR) clean EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)"
endif

moca:
ifeq ($(CONFIG_MOCA),y)
	+$(MAKE) -C $(MOCA_BASE_DIR) EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)"
endif
moca-install:
ifeq ($(CONFIG_MOCA),y)
	+$(MAKE) -C $(MOCA_BASE_DIR) INSTALLDIR=$(INSTALLDIR)/moca install EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)"
endif
moca-clean:
ifeq ($(CONFIG_MOCA),y)
	+$(MAKE) -C $(MOCA_BASE_DIR) clean EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)"
endif

appeventd:
ifeq ($(CONFIG_BCM_APPEVENTD),y)
	+$(MAKE) -C $(APPEVENTD_BASE_DIR) EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)"
endif
appeventd-install:
ifeq ($(CONFIG_BCM_APPEVENTD),y)
	+$(MAKE) -C $(APPEVENTD_BASE_DIR) INSTALLDIR=$(INSTALLDIR)/appeventd install EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)"
endif
appeventd-clean:
ifeq ($(CONFIG_BCM_APPEVENTD),y)
	+$(MAKE) -C $(APPEVENTD_BASE_DIR) clean EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)"
endif

bsd:
ifeq ($(CONFIG_BCMBSD),y)
	+$(MAKE) -C $(BSD_BASE_DIR) EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)"
endif
bsd-install:
ifeq ($(CONFIG_BCMBSD),y)
	+$(MAKE) -C $(BSD_BASE_DIR) INSTALLDIR=$(INSTALLDIR)/bsd install EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)"
endif
bsd-clean:
ifeq ($(CONFIG_BCMBSD),y)
	+$(MAKE) -C $(BSD_BASE_DIR) clean EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)"
endif

#if defined(EXT_ACS)
ifeq ($(CONFIG_EXTACS),y)
acsdv2:
	+$(MAKE) -C $(ACSDV2_DIR) EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)" all
acsdv2-install: acsdv2
	+$(MAKE) -C $(ACSDV2_DIR) INSTALLDIR=$(INSTALLDIR)/acsdv2 install
acsdv2-clean:
	+$(MAKE) -C $(ACSDV2_DIR) EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)" clean
endif
#endif // endif

#if defined(BCM_ESCAND)
ifeq ($(CONFIG_BCMESCAND),y)
ifneq ($(filter y,$(CONFIG_EAP) $(BUILD_HND_EAP)),)
escand:
	+$(MAKE) -C $(ESCAND_DIR) EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)" all
escand-install: escand
	+$(MAKE) -C $(ESCAND_DIR) INSTALLDIR=$(INSTALLDIR)/escand install
escand-clean:
	+$(MAKE) -C $(ESCAND_DIR) EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)" clean
endif
endif
#endif // endif

ifeq ($(CONFIG_CSIMOND),y)
CSIMOND_DIR := $(BASEDIR)/components/router/csimond
csimond:
	+$(MAKE) -C $(CSIMOND_DIR) EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)" all
csimond-install: csimond
	+$(MAKE) -C $(CSIMOND_DIR) INSTALLDIR=$(INSTALLDIR)/csimond install
csimond-clean:
	+$(MAKE) -C $(CSIMOND_DIR) EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)" clean
endif

ifeq ($(CONFIG_BCM_CEVENT),y)
cevent_app:
	+$(MAKE) -C $(CEVENT_APP_DIR) all EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)"
cevent_app-install: cevent_app
	+$(MAKE) -C $(CEVENT_APP_DIR) install EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)" INSTALLDIR=$(INSTALLDIR)/cevent_app
cevent_app-clean:
	+$(MAKE) -C $(CEVENT_APP_DIR) clean EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)"
endif

ifeq ($(CONFIG_VISUALIZATION),y)
ifneq ($(wildcard $(VISDCOLL_DIR)),)
$(VISDCOLL_DIR)/configure:
	cd $(VISDCOLL_DIR) && \
	autoreconf --force --install

$(VISDCOLL_DIR)/Makefile: $(VISDCOLL_DIR)/configure
	cd $(VISDCOLL_DIR) && \
         CC="$(CC)" ./configure \
            --target=$(PLATFORM)-linux \
            --host=$(PLATFORM)-linux \
            --prefix=$(VISSOURCE_BASE_DIR)/installbin

visdcoll: $(VISDCOLL_DIR)/Makefile
	+$(MAKE) -C $(VISDCOLL_DIR) EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)"
	+$(MAKE) -C $(VISDCOLL_DIR) install-strip
endif
else
visdcoll:
endif

visdcoll-install:
ifeq ($(CONFIG_VISUALIZATION),y)
	install -d $(INSTALLDIR)/visdcoll/usr/sbin
	cp -rf $(VISSOURCE_BASE_DIR)/installbin/bin/vis-datacollector $(INSTALLDIR)/visdcoll/usr/sbin
endif

visdcoll-clean: $(VISDCOLL_DIR)/Makefile
ifeq ($(CONFIG_VISUALIZATION),y)
ifneq ($(wildcard $(VISDCOLL_DIR)),)
	+$(MAKE) -C $(VISDCOLL_DIR) distclean
endif
endif

ifeq ($(CONFIG_VISUALIZATION),y)
ifneq ($(wildcard $(VISDCON_DIR)),)
$(VISDCON_DIR)/configure:
	cd $(VISDCON_DIR) && \
	autoreconf --force --install

$(VISDCON_DIR)/Makefile: $(VISDCON_DIR)/configure
	cd $(VISDCON_DIR) && \
         CC="$(CC)" ./configure \
            --target=$(PLATFORM)-linux \
            --host=$(PLATFORM)-linux \
            --prefix=$(VISSOURCE_BASE_DIR)/installbin

visdcon: $(VISDCON_DIR)/Makefile
	+$(MAKE) -C $(VISDCON_DIR) EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)"
	+$(MAKE) -C $(VISDCON_DIR) install-strip
endif
else
visdcon:
endif

visdcon-install:
ifeq ($(CONFIG_VISUALIZATION),y)
	install -d $(INSTALLDIR)/visdcon/usr/sbin
	cp -rf $(VISSOURCE_BASE_DIR)/installbin/bin/vis-dcon $(INSTALLDIR)/visdcon/usr/sbin
endif

visdcon-clean: $(VISDCON_DIR)/Makefile
ifeq ($(CONFIG_VISUALIZATION),y)
ifneq ($(wildcard $(VISDCON_DIR)),)
	+$(MAKE) -C $(VISDCON_DIR) distclean
endif
endif

ifeq ($(CONFIG_LIBNFNETLINK),y)
# iptables will try to link with netfilter's libs if enabled.
IPTABLES_DEPS := libnfnetlink
endif

ifeq ($(CONFIG_IPV6),y)
DOIPV6=1
else
DOIPV6=0
endif

ifeq ($(CONFIG_STBAP),y)
iptables: $(IPTABLES_DEPS)
	# using iptables from STB's bsp
	@true

iptables-install:
	# using iptables from STB's bsp
	@true

iptables-clean:
	# using iptables from STB's bsp
	@true

else ifneq ($(strip $(BCA_HNDROUTER)),)
iptables: $(IPTABLES_DEPS)
	# using iptables from 4.1 bsp
	@true

iptables-install:
	# using iptables from 4.1 bsp
	@true

iptables-clean:
	# using iptables from 4.1 bsp
	@true

else ifeq ($(call wlan_version_ge,$(BCM_KVERSIONSTRING),2.6.36),TRUE)
iptables: $(IPTABLES_DEPS)
	+$(MAKE) -C iptables-1.4.12 BINDIR=/usr/sbin LIBDIR=/usr/lib \
	    KERNEL_DIR=$(LINUXDIR)

iptables-install:
ifeq ($(CONFIG_IPTABLES),y)
	install -d $(INSTALLDIR)/iptables/usr/lib/iptables
	install iptables-1.4.12/src/extensions/*.so $(INSTALLDIR)/iptables/usr/lib/iptables
	$(STRIP) $(INSTALLDIR)/iptables/usr/lib/iptables/*.so
	install -d $(INSTALLDIR)/iptables/usr/sbin
	cp -rf iptables-1.4.12/src/install/sbin/* $(INSTALLDIR)/iptables/usr/sbin
	install -d $(INSTALLDIR)/iptables/usr/lib
	cp -rf iptables-1.4.12/src/install/lib/* $(INSTALLDIR)/iptables/usr/lib
	$(RM) -r $(INSTALLDIR)/iptables/usr/lib/pkgconfig
	$(RM) $(INSTALLDIR)/iptables/usr/lib/*.la
else
	# So that generic rule does not take precedence
	@true
endif
iptables-clean:
	-$(MAKE) -C iptables-1.4.12 KERNEL_DIR=$(LINUXDIR) clean

else ifeq (2_6,$(LINUX_VERSION))
iptables:
	+$(MAKE) -C iptables-1.x BINDIR=/usr/sbin LIBDIR=/usr/lib \
	    KERNEL_DIR=$(LINUXDIR) DO_IPV6=$(DOIPV6)

iptables-install:
ifeq ($(CONFIG_IPTABLES),y)
	install -d $(INSTALLDIR)/iptables/usr/lib/iptables
	install iptables-1.x/extensions/*.so $(INSTALLDIR)/iptables/usr/lib/iptables
	$(STRIP) $(INSTALLDIR)/iptables/usr/lib/iptables/*.so
	install -D iptables-1.x/iptables $(INSTALLDIR)/iptables/usr/sbin/iptables
	$(STRIP) $(INSTALLDIR)/iptables/usr/sbin/iptables
	install -D iptables-1.x/iptables-restore $(INSTALLDIR)/iptables/usr/sbin/iptables-restore # BCMQOS
	$(STRIP) $(INSTALLDIR)/iptables/usr/sbin/iptables-restore # BCMQOS
else
	# So that generic rule does not take precedence
	@true
endif
iptables-clean:
	-$(MAKE) -C iptables-1.x KERNEL_DIR=$(LINUXDIR) DO_IPV6=$(DOIPV6) clean
else # linux-2.6
iptables:
	+$(MAKE) -C iptables BINDIR=/usr/sbin LIBDIR=/usr/lib \
	    KERNEL_DIR=$(LINUXDIR)
#DO_IPV6=1

iptables-install:
ifeq ($(CONFIG_IPTABLES),y)
	install -d $(INSTALLDIR)/iptables/usr/lib/iptables
	install iptables/extensions/*.so $(INSTALLDIR)/iptables/usr/lib/iptables
	$(STRIP) $(INSTALLDIR)/iptables/usr/lib/iptables/*.so
	install -D iptables/iptables $(INSTALLDIR)/iptables/usr/sbin/iptables
	$(STRIP) $(INSTALLDIR)/iptables/usr/sbin/iptables
else
	# So that generic rule does not take precedence
	@true
endif
iptables-clean:
	-$(MAKE) -C iptables KERNEL_DIR=$(LINUXDIR) DO_IPV6=$(DOIPV6) clean

endif # linux-2.6

netconf: iptables
ifeq ($(CONFIG_NETCONF),y)
	+$(MAKE) LINUXDIR=$(LINUXDIR) -C netconf

else
	# In case of "Prerequisite 'iptables' is newer than target 'netconf'"
	@true
endif

ntpclient-install:
	install -D ntpclient/ntpclient $(INSTALLDIR)/ntpclient/usr/sbin/ntpclient
	$(STRIP) $(INSTALLDIR)/ntpclient/usr/sbin/ntpclient

ppp :
	+$(MAKE) -C ppp/pppoecd $* INSTALLDIR=$(INSTALLDIR)/ppp

ppp-install:
	+$(MAKE) -C ppp/pppoecd install INSTALLDIR=$(INSTALLDIR)/ppp

ppp-clean:
	+$(MAKE) -C ppp/pppoecd clean INSTALLDIR=$(INSTALLDIR)/ppp

udhcpd-install:
	install -D udhcpd/udhcpd $(INSTALLDIR)/udhcpd/usr/sbin/udhcpd
	$(STRIP) $(INSTALLDIR)/udhcpd/usr/sbin/udhcpd
	cd $(INSTALLDIR)/udhcpd/usr/sbin && ln -sf udhcpd udhcpc

wlconf: nvram shared

vlan:
	+$(MAKE) -C vlan CROSS=$(CROSS_COMPILE) STRIPTOOL=$(STRIP)

vlan-install:
	+$(MAKE) install -C vlan \
	    CROSS=$(CROSS_COMPILE) STRIPTOOL=$(STRIP) INSTALLDIR=$(INSTALLDIR)

vlan-clean:
	$(MAKE) -C vlan clean

buzzz:
	+$(MAKE) -C buzzz CROSS=$(CROSS_COMPILE) STRIPTOOL=$(STRIP)

buzzz-install:
	install -d $(TARGETDIR)
	+$(MAKE) -C buzzz CROSS=$(CROSS_COMPILE) STRIPTOOL=$(STRIP)  INSTALLDIR=$(INSTALLDIR) install

buzzz-clean:
	$(MAKE) -C buzzz clean

emf:
ifeq ($(CMWIFI_RDKB),)
	+$(MAKE) -C emf/emfconf EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)" CROSS=$(CROSS_COMPILE)
else
	+$(MAKE) -C emf/emfconf EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)" CROSS=$(CROSS_COMPILE) CC="$(CC)"
endif

emf-install:
ifeq ($(CONFIG_EMF),y)
	install -d $(TARGETDIR)
	+$(MAKE) install -C emf/emfconf \
	    CROSS=$(CROSS_COMPILE) INSTALLDIR=$(INSTALLDIR)

endif

emf-clean:
	$(MAKE) -C emf/emfconf clean

igs:
ifeq ($(CMWIFI_RDKB),)
	+$(MAKE) -C emf/igsconf EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)" CROSS=$(CROSS_COMPILE)
else
	+$(MAKE) -C emf/igsconf EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)" CROSS=$(CROSS_COMPILE) CC="$(CC)"
endif # CMWIFI_RDKB

igs-install:
ifeq ($(CONFIG_EMF),y)
	install -d $(TARGETDIR)
	+$(MAKE) -C emf/igsconf CROSS=$(CROSS_COMPILE) INSTALLDIR=$(INSTALLDIR) install

endif

igs-clean:
	$(MAKE) -C emf/igsconf clean

igmp:
	$(MAKE) -C igmp EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)" CROSS=$(CROSS_COMPILE)

igmp-install:
ifeq ($(CONFIG_IGMP_PROXY),y)
	install -d $(TARGETDIR)
	+$(MAKE) -C igmp CROSS=$(CROSS_COMPILE) INSTALLDIR=$(INSTALLDIR) install
endif

igmp-clean:
	$(MAKE) -C igmp clean

ifeq ($(CONFIG_STBAP),y)
stbap-modules: emfdriver igsdriver dhddriver nicdriver

stbap-modules-clean: emfdriver-clean igsdriver-clean dhddriver-clean nicdriver-clean

emfdriver:
	+$(MAKE) -C $(EMFSOURCE_DIR) STBLINUX=1 ARCH=${PLT} WLARCH=${PLT} LINUXDIR=${LINUX} LINUXVER=${LINUX_VERSION} EXTRA_LDFLAGS= CONFIG_STBAP=y

igsdriver:
	+$(MAKE) -C $(EMFSOURCE_DIR)/../igs STBLINUX=1 ARCH=${PLT} WLARCH=${PLT} LINUXVER=${LINUX_VERSION} EXTRA_LDFLAGS= CONFIG_STBAP=y

nicdriver:
	+$(MAKE) -C $(NICSOURCE_DIR) ARCH=${PLT} WLARCH=${PLT} LINUXVER=${LINUX_VERSION} EXTRA_LDFLAGS=  STBLINUX=1 ${NIC_STBAP_SECDMA_TARGET}
	+$(MAKE) -C $(NICSOURCE_DIR) ARCH=${PLT} WLARCH=${PLT} LINUXVER=${LINUX_VERSION} EXTRA_LDFLAGS=  STBLINUX=1 ${NIC_STBAP_NONSECDMA_TARGET}

dhddriver:
	cat ${EMF_EXTRA_SYMBOLS}  > ${KBUILD_EXTRA_SYMBOLS}
	+$(MAKE) -C $(DHDSOURCE_DIR) -f Makefile.stbap LINUXVER=$(LINUXVER) CONFIG_STBAP=y LINUXDIR=$(LINUXDIR) CROSS_COMPILE=$(CROSS_COMPILE) ARCH=${PLT} EXTRA_LDFLAGS= $(DHD_STBAP_NONSECDMA_TARGET)
	+$(MAKE) -C $(DHDSOURCE_DIR) -f Makefile.stbap LINUXVER=$(LINUXVER) CONFIG_STBAP=y LINUXDIR=$(LINUXDIR) CROSS_COMPILE=$(CROSS_COMPILE) ARCH=${PLT} EXTRA_LDFLAGS= $(DHD_STBAP_SECDMA_TARGET)

emfdriver-clean:
	+$(MAKE) -C $(EMFSOURCE_DIR) ARCH=${PLT} WLARCH=${PLT} LINUXVER=${LINUX_VERSION} EXTRA_LDFLAGS= clean

igsdriver-clean:
	+$(MAKE) -C $(EMFSOURCE_DIR)/../igs ARCH=${PLT} WLARCH=${PLT} LINUXVER=${LINUX_VERSION} EXTRA_LDFLAGS=  clean

nicdriver-clean:
	+$(MAKE) -C $(NICSOURCE_DIR) clean

dhddriver-clean:
	$(MAKE) -C $(DHDSOURCE_DIR) -f Makefile.stbap clean

stbap-modules-install:
	install -d $(TARGETDIR)/lib/modules
	cp $(EMFSOURCE_DIR)/obj-emf-$(LINUX_VERSION)/emf.ko $(TARGETDIR)/lib/modules
	cp $(EMFSOURCE_DIR)/../igs/obj-igs-$(LINUX_VERSION)/igs.ko $(TARGETDIR)/lib/modules
	cp $(NICSOURCE_DIR)/obj-$(NIC_STBAP_SECDMA_TARGET)-$(LINUX_VERSION)/wl.ko $(TARGETDIR)/lib/modules/wl_secdma.ko
	cp $(NICSOURCE_DIR)/obj-$(NIC_STBAP_NONSECDMA_TARGET)-$(LINUX_VERSION)/wl.ko $(TARGETDIR)/lib/modules
	cp $(DHDSOURCE_DIR)/$(DHD_STBAP_SECDMA_TARGET)-$(LINUX_VERSION)/dhd.ko $(TARGETDIR)/lib/modules/dhd_secdma.ko
	cp $(DHDSOURCE_DIR)/$(DHD_STBAP_NONSECDMA_TARGET)-$(LINUX_VERSION)/dhd.ko $(TARGETDIR)/lib/modules
endif # CONFIG_STBAP

# NFC
nfc:
ifneq (,$(and $(filter y,$(CONFIG_NFC)),$(wildcard nfc/Makefile)))
	+$(MAKE) -C nfc EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)"
else
	# Prevent to use generic rules"
	@true
endif

nfc-install:
ifeq ($(CONFIG_NFC),y)
	+$(if $(wildcard nfc/Makefile), \
	    $(MAKE) -C nfc INSTALLDIR=$(INSTALLDIR) EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)" install \
	   , \
	    @true \
	  )
else
	# Prevent to use generic rules"
	@true
endif

nfc-clean:
ifeq ($(CONFIG_NFC),y)
	[ ! -f nfc/Makefile ] || $(MAKE) -C nfc clean
else
	# Prevent to use generic rules"
	@true
endif

ifneq (2_4,$(LINUX_VERSION))
udev:
	+$(MAKE) -C udev CROSS_COMPILE=$(CROSS_COMPILE)

udev-install:
	install -d $(TARGETDIR)
	+$(MAKE) install-udevtrigger -C udev \
	    CROSS_COMPILE=$(CROSS_COMPILE) DESTDIR=$(INSTALLDIR) prefix=/udev

udev-clean:
	$(MAKE) -C udev clean

hotplug2:
	+$(MAKE) -C hotplug2 CROSS_COMPILE=$(CROSS_COMPILE)

hotplug2-install:
	install -d $(TARGETDIR)
	install -d $(INSTALLDIR)/hotplug2
	+$(MAKE) install -C hotplug2 \
	    CROSS_COMPILE=$(CROSS_COMPILE) PREFIX=$(INSTALLDIR)

hotplug2-clean:
	$(MAKE) -C hotplug2 clean
endif

IPROUTE2_SUBDIRS := lib

ifeq (2_4,$(LINUX_VERSION))
UCLIBC_IPV6=$(TOP)/lib/mipsel-uclibc/libc.so.0
ifeq ($(CONFIG_IPROUTE2),y)
IPROUTE2_SUBDIRS += ip
endif
endif

radvd/Makefile: radvd/configure radvd/Makefile.in
	[ ! -d $(@D) ] || (cd $(@D) && CC=$(CC) ./configure --host=$(PLT)-linux)

.PHONY: radvd
radvd: radvd/Makefile flex dummy
	+$(if $(wildcard $@/),$(MAKE) -C radvd CC=${CC} radvd_LDADD="-L../flex -lfl",@true)

.PHONY: radvd-install
radvd-install:
	[ ! -d radvd ] || install -D -m 755 radvd/radvd $(INSTALLDIR)/radvd/usr/sbin/radvd
	[ ! -d radvd ] || $(STRIP) $(INSTALLDIR)/radvd/usr/sbin/radvd

.PHONY: radvd-clean
radvd-clean:
	[ ! -f radvd/Makefile ] || $(MAKE) -C radvd distclean

flex/Makefile: flex/configure flex/Makefile.in
	[ ! -d $(@D) ] || (cd $(@D) && CC=$(CC) ./configure --host=$(PLT)-linux)

.PHONY: flex
flex: flex/Makefile dummy
	+$(if $(wildcard $@/),$(MAKE) libfl.a -C $@ CC=${CC} RANLIB=${RANLIB},@true)

iproute2:
ifneq ($(LINUX_VERSION),2_6_36)
	+$(if $(wildcard $@/), \
	    $(MAKE) -C $@ KERNEL_INCLUDE=$(LINUXDIR)/include CC=$(CC) \
	         AR=$(AR) SUBDIRS="$(IPROUTE2_SUBDIRS)" LDLIBS="$(UCLIBC_IPV6)" \
	   , \
	    @true \
	  )
else
	+$(if $(wildcard $(IPROUTE2_DIR)/), \
	    $(MAKE) -C $(IPROUTE2_DIR) KERNEL_INCLUDE=$(LINUXDIR)/include CC=$(CC) \
	         AR=$(AR) SUBDIRS="$(IPROUTE2_SUBDIRS)" LDLIBS="$(UCLIBC_IPV6)" \
	   , \
	    @true \
	  )

endif

iproute2-clean:
	[ ! -d $(IPROUTE2_DIR) ] || $(MAKE) -C $(IPROUTE2_DIR) clean

iproute2-install:
ifeq ($(CONFIG_IPROUTE2),y)
	[ ! -d $(IPROUTE2_DIR) ] || install -D -m 755 $(IPROUTE2_DIR)/ip/ip $(INSTALLDIR)/iproute2/usr/sbin/ip
	[ ! -d $(IPROUTE2_DIR) ] || $(STRIP) $(INSTALLDIR)/iproute2/usr/sbin/ip
endif

iputils:
ifneq ($(LINUX_VERSION),2_6_36)
	[ ! -d $@ ] || [ -f $@/include-glibc/bits/socket.h ] || ( \
	    cd $@/include-glibc/bits && ln -s ../socketbits.h socket.h \
	)
endif
	+$(if $(wildcard $(IPUTILS_DIR)), \
	    $(MAKE) -C $(IPUTILS_DIR) KERNEL_INCLUDE=$(LINUXDIR)/include \
	    LINUX_VERSION=$(LINUX_VERSION) CC=$(CC) LDLIBS= \
	   , \
	    @true \
	  )

iputils-clean:
	[ ! -d $(IPUTILS_DIR) ] || $(MAKE) -C $(IPUTILS_DIR) clean

iputils-install:
	[ ! -d $(IPUTILS_DIR) ] || install -D -m 755 $(IPUTILS_DIR)/ping6 $(INSTALLDIR)/iputils/usr/sbin/ping6
	[ ! -d $(IPUTILS_DIR) ] || $(STRIP) $(INSTALLDIR)/iputils/usr/sbin/ping6
	[ ! -d $(IPUTILS_DIR) ] || install -D -m 755 $(IPUTILS_DIR)/traceroute6 $(INSTALLDIR)/iputils/usr/sbin/traceroute6
	[ ! -d $(IPUTILS_DIR) ] || $(STRIP) $(INSTALLDIR)/iputils/usr/sbin/traceroute6
	[ ! -d $(IPUTILS_DIR) ] || install -D -m 755 $(IPUTILS_DIR)/tracepath6 $(INSTALLDIR)/iputils/usr/sbin/tracepath6
	[ ! -d $(IPUTILS_DIR) ] || $(STRIP) $(INSTALLDIR)/iputils/usr/sbin/tracepath6

dhcp6: dummy
#dhcp6s dhcp6c: dummy
	[ ! -d dhcp6 ] || [ -f dhcp6/Makefile ] || ( \
	    cd dhcp6 && CC="$(CC)" ./configure CC=gcc CFLAGS+="-I../shared" \
	)
ifeq ($(CONFIG_UCLIBC),y)
	+$(if $(wildcard dhcp6/), \
	    $(MAKE) dhcp6s dhcp6c -C dhcp6 CC=$(CC) LIBS="$(UCLIBC_IPV6)" \
	   , \
	    @true \
	  )
else
	+$(if $(wildcard dhcp6/), \
	    $(MAKE) dhcp6s dhcp6c -C dhcp6 CC=$(CC) \
	        LIBS="-lresolv -L../libbcmcrypto -lbcmcrypto \
	              -L$(INSTALLDIR)/libbcmcrypto/usr/lib" \
	   , \
	    @true \
	  )
endif

dhcp6-install: $(if $(filter y,${CONFIG_DHCPV6S}),dhcp6s-install) \
               $(if $(filter y,${CONFIG_DHCPV6C}),dhcp6c-install)
	@true # Keep this rule from falling through to generic and failing.

dhcp6s-install: dhcp6
	[ ! -d dhcp6 ] || install -D -m 755 dhcp6/dhcp6s $(INSTALLDIR)/dhcp6s/usr/sbin/dhcp6s
	[ ! -d dhcp6 ] || $(STRIP) $(INSTALLDIR)/dhcp6s/usr/sbin/dhcp6s

dhcp6c-install: dhcp6
	[ ! -d dhcp6 ] || install -D -m 755 dhcp6/dhcp6c $(INSTALLDIR)/dhcp6c/usr/sbin/dhcp6c
	[ ! -d dhcp6 ] || $(STRIP) $(INSTALLDIR)/dhcp6c/usr/sbin/dhcp6c

dhcp6s-clean dhcp6c-clean:
#dhcp6s-clean dhcp6c-clean:
dhcp6-clean:

tpdump: dummy

tpdump-install:
ifeq ($(CONFIG_EAP),y)
	echo "Installing src/tools/scripts/tpdump to /usr/sbin/tpdump"
	install -d $(TARGETDIR)/usr/sbin
	install -D $(SRCBASE)/tools/scripts/tpdump $(TARGETDIR)/usr/sbin/tpdump
endif

.PHONY: wlexe
wlexe:
ifeq ($(CONFIG_WLEXE),y)
ifneq ($(wildcard $(SRCBASE_SYS)/wl/exe/GNUmakefile),)
ifneq ($(CONFIG_STBAP),y)
ifeq ($(ARCH),arm)
ifeq ($(CMWIFI_RDKB),)
	$(MAKE) TARGETARCH=arm_le ARCH_SFX="" TARGET_PREFIX=$(CROSS_COMPILE) -C $(SRCBASE_SYS)/wl/exe
else
	$(MAKE) TARGETARCH=arm_le ARCH_SFX="" TARGET_PREFIX=$(CROSS_COMPILE) -C $(SRCBASE_SYS)/wl/exe CC="$(CC)"
endif # CMWIFI_RDKB
else
	$(MAKE) TARGETARCH=mips ARCH_SFX="" TARGET_PREFIX=$(CROSS_COMPILE) -C $(SRCBASE_SYS)/wl/exe
endif
	[ -d wlexe ] || install -d wlexe
	install $(SRCBASE_SYS)/wl/exe/wl wlexe
ifeq ($(ARCH),arm)
	install $(SRCBASE_SYS)/wl/exe/socket_noasd/arm_le/wl_server_socket wlexe
else
	install $(SRCBASE_SYS)/wl/exe/socket_noasd/mips/wl_server_socket wlexe
endif
else
ifeq ($(PLT),arm)
	$(MAKE) TARGETARCH=arm_le ARCH_SFX="" TARGET_PREFIX=$(CROSS_COMPILE) -C $(SRCBASE_SYS)/wl/exe
else
	$(MAKE) TARGETARCH=${PLT} CC=$(CROSS_COMPILE)gcc -C $(SRCBASE_SYS)/wl/exe
endif
	[ -d wlexe ] || install -d wlexe
ifeq ($(PLT),arm)
	install $(SRCBASE_SYS)/wl/exe/wl wlexe
else
	install $(SRCBASE_SYS)/wl/exe/wlarm64 wlexe
endif

ifeq ($(PLT),arm)
	install $(SRCBASE_SYS)/wl/exe/socket_noasd/arm_le/wl_server_socket wlexe
else
	install $(SRCBASE_SYS)/wl/exe/socket_noasd/arm64/wl_server_socketarm64 wlexe
endif
endif # CONFIG_STBAP

endif
endif

.PHONY: wlexe-clean
wlexe-clean:
ifneq ($(wildcard $(SRCBASE_SYS)/wl/exe/GNUmakefile),)
	$(MAKE) TARGET_PREFIX=$(CROSS_COMPILE) -C $(SRCBASE_SYS)/wl/exe clean
	$(RM) wlexe/wl
	$(RM) wlexe/wl_server_socket
endif

.PHONY: wlexe-install
wlexe-install:
ifeq ($(CONFIG_WLEXE),y)
ifneq ($(wildcard $(SRCBASE_SYS)/wl/exe/GNUmakefile),)
ifneq ($(CONFIG_STBAP),y)
	[ ! -d wlexe ] || install -D -m 755 wlexe/wl $(INSTALLDIR)/wlexe/usr/sbin/wl
	[ ! -d wlexe ] || install -D -m 755 wlexe/wl_server_socket $(INSTALLDIR)/wlexe/usr/sbin/wl_server_socket
else
ifeq ($(PLT),arm)
	[ ! -d wlexe ] || install -D -m 755 wlexe/wl $(INSTALLDIR)/wlexe/usr/sbin/wl
	[ ! -d wlexe ] || install -D -m 755 wlexe/wl_server_socket $(INSTALLDIR)/wlexe/usr/sbin/wl_server_socket
else
	[ ! -d wlexe ] || install -D -m 755 wlexe/wlarm64 $(INSTALLDIR)/wlexe/usr/sbin/wl
	[ ! -d wlexe ] || install -D -m 755 wlexe/wl_server_socketarm64 $(INSTALLDIR)/wlexe/usr/sbin/wl_server_socket
endif
endif # CONFIG_STBAP
	[ ! -d wlexe ] || $(STRIP) $(INSTALLDIR)/wlexe/usr/sbin/wl
	[ ! -d wlexe ] || $(STRIP) $(INSTALLDIR)/wlexe/usr/sbin/wl_server_socket
endif
endif

ifneq ($(CONFIG_STBAP),y)
$(LIBNL_DIR)/Makefile:
	(cd $(LIBNL_DIR); \
		autoreconf --force --install && \
		./configure \
		CC="$(CC)" \
		--host=$(TARGET_PLT) \
		--build=$(BUILD) \
		--prefix=$(LIBNL_DIR)/install)

libnl: $(LIBNL_DIR)/Makefile
	+$(MAKE) -C $(LIBNL_DIR)
	+$(MAKE) -C $(LIBNL_DIR) install

libnl-install:
	install -d $(INSTALL_DIR)/lib/gpl
	cp -rf $(LIBNL_DIR)/install/lib/* $(INSTALL_DIR)/lib/gpl/
	find $(INSTALL_DIR)/lib/gpl -name "libnl*.la" -delete
	find $(INSTALL_DIR)/lib/gpl -name "libnl*.pc" -delete
	find $(INSTALL_DIR)/lib/gpl -name "libnl*.a" -delete

libnl-clean:
	+$(MAKE) -C $(LIBNL_DIR) distclean
endif # CONFIG_STBAP

$(HOSTAPD_DIR)/hostapd/.config: $(HOSTAPD_DIR)/hostapd/brcm.config
	cp $(HOSTAPD_DIR)/hostapd/brcm.config $(HOSTAPD_DIR)/hostapd/.config;

hostapd: libnl $(HOSTAPD_DIR)/hostapd/.config
	+$(MAKE) -C $(HOSTAPD_DIR)/hostapd

hostapd-install:
	install -d $(INSTALLDIR)/hostapd/usr/sbin
	cp -rf $(HOSTAPD_DIR)/hostapd/hostapd $(INSTALLDIR)/hostapd/usr/sbin
	cp -rf $(HOSTAPD_DIR)/hostapd/hostapd_cli $(INSTALLDIR)/hostapd/usr/sbin

hostapd-clean:
	+$(MAKE) -C $(HOSTAPD_DIR)/hostapd clean

$(WPA_SUPPLICANT_DIR)/wpa_supplicant/.config: $(WPA_SUPPLICANT_DIR)/wpa_supplicant/brcm.config
	cp $(WPA_SUPPLICANT_DIR)/wpa_supplicant/brcm.config $(WPA_SUPPLICANT_DIR)/wpa_supplicant/.config;

wpa_supplicant: libnl $(WPA_SUPPLICANT_DIR)/wpa_supplicant/.config
	+$(MAKE) -C $(WPA_SUPPLICANT_DIR)/wpa_supplicant

wpa_supplicant-install:
	install -d $(INSTALLDIR)/wpa_supplicant/usr/sbin
	install -d $(INSTALLDIR)/wpa_supplicant/usr/lib
	cp -rf $(WPA_SUPPLICANT_DIR)/wpa_supplicant/wpa_supplicant $(INSTALLDIR)/wpa_supplicant/usr/sbin
	cp -rf $(WPA_SUPPLICANT_DIR)/wpa_supplicant/wpa_cli $(INSTALLDIR)/wpa_supplicant/usr/sbin
	cp -rf $(WPA_SUPPLICANT_DIR)/wpa_supplicant/libwpa_client.so $(INSTALLDIR)/wpa_supplicant/usr/lib

wpa_supplicant-clean:
	+$(MAKE) -C $(WPA_SUPPLICANT_DIR)/wpa_supplicant clean

iw: libnl
	+$(MAKE) -C $(IW_DIR)

iw-install:
	install -d $(INSTALLDIR)/iw/usr/sbin
	cp -rf $(IW_DIR)/iw $(INSTALLDIR)/iw/usr/sbin

pcienic:
	cd $(BASEDIR)/../../../../../../kernel/linux-4.1; \
	make -j8 ARCH=arm64 \
	CROSS_COMPILE=aarch64-buildroot-linux-gnu- \
	M=../../bcmdrivers/broadcom/net/wl/bcm94908/main/components/router/hnd_wl \
	EXTRA_CFLAGS="$(shell cat $(BASEDIR)/components/router/hnd_wl/wlflags.tmp)" \
	OSLLX=1 SIUTILS=1 \
	modules

#
# Generic rules
#

%:
	+$(if $(wildcard $*/),$(MAKE) -C $* EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)" ARCH=$(ARCH),@true)

%-clean:
	[ ! -d $* ] || $(MAKE) INSTALLDIR=$(INSTALLDIR)/$* -C $* clean

%-install:
	+$(if $(wildcard $*/), \
	  $(MAKE) -C $* install EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)" INSTALLDIR=$(INSTALLDIR)/$*, \
	  @true \
	 )

$(obj-y) $(obj-n) $(obj-clean) $(obj-install): dummy

ifeq ($(CONFIG_STBAP),y)
stbap-pkg:
	install -d $(STBAP_TARGET_DIR)/etc
	install -d $(STBAP_TARGET_DIR)/etc/init.d
	install -d $(STBAP_TARGET_DIR)/sbin
	install -d $(STBAP_TARGET_DIR)/usr/sbin
	install -d $(STBAP_TARGET_DIR)/bin
	install -d $(STBAP_TARGET_DIR)/usr/lib
	install -d $(STBAP_TARGET_DIR)/lib/firmware/brcm
ifeq ($(PLT),arm64)
	install -d $(STBAP_TARGET_DIR)/lib64
	cp -r ${BASEDIR}/components/router/${PLT}-glibc/target/usr/lib/* ${STBAP_TARGET_DIR}/lib64
	cp ${BASEDIR}/components/router/${PLT}-glibc/target/lib/libnv2hapdcfg.so ${STBAP_TARGET_DIR}/lib64
else
	install -d $(STBAP_TARGET_DIR)/lib
	cp -r ${BASEDIR}/components/router/${PLT}-glibc/target/usr/lib/* ${STBAP_TARGET_DIR}/lib
endif
	cp $(BASEDIR)/src/shared/nvram/stbap_nvram.txt ${STBAP_TARGET_DIR}/etc/nvrams_ap_default.txt
	cp $(BASEDIR)/src/shared/nvram/stbap_nvram_non7278.txt ${STBAP_TARGET_DIR}/etc/nvrams_ap_default_non7278.txt
	cp ${BASEDIR}/components/router/rc/stbap_init.sh ${STBAP_TARGET_DIR}/sbin/stbap_init.sh
	chmod 777 ${STBAP_TARGET_DIR}/sbin/stbap_init.sh
	cp ${BASEDIR}/components/router/rc/stbap_init_non7278.sh ${STBAP_TARGET_DIR}/sbin/stbap_init_non7278.sh
	chmod 777 ${STBAP_TARGET_DIR}/sbin/stbap_init_non7278.sh
	cp ${BASEDIR}/components/router/rc/stbhotplug ${STBAP_TARGET_DIR}/sbin
	chmod 777 ${STBAP_TARGET_DIR}/sbin/stbhotplug
	cp ${BASEDIR}/components/router/rc/stbap_rcS ${STBAP_TARGET_DIR}/etc/init.d/rcS
	chmod 755 ${STBAP_TARGET_DIR}/etc/init.d/rcS
	cp ${BASEDIR}/components/router/${PLT}-glibc/target/sbin/rc ${STBAP_TARGET_DIR}/sbin
	cp ${BASEDIR}/components/router/${PLT}-glibc/target/sbin/wps_pbcd ${STBAP_TARGET_DIR}/sbin
	cp ${BASEDIR}/components/router/${PLT}-glibc/target/usr/sbin/* ${STBAP_TARGET_DIR}/usr/sbin
	cp ${BASEDIR}/components/router/${PLT}-glibc/target/bin/* ${STBAP_TARGET_DIR}/bin
	cp -r ${BASEDIR}/components/router/${PLT}-glibc/target/lib ${STBAP_TARGET_DIR}/
ifeq ($(WLTEST),1)
	if [ ! -e ${STBAP_TARGET_DIR}/lib/firmware/brcm/*.bin ]; then \
		cp ${BASEDIR}/firmware_mfg/* $(STBAP_TARGET_DIR)/lib/firmware/brcm; \
	fi
else
	if [ ! -e ${STBAP_TARGET_DIR}/lib/firmware/brcm/*.bin ]; then \
		cp ${BASEDIR}/firmware_prd/* $(STBAP_TARGET_DIR)/lib/firmware/brcm; \
	fi
endif
	cp -r ${BASEDIR}/components/router/${PLT}-glibc/target/www ${STBAP_TARGET_DIR}/
	tar -C ${PLT}-glibc/stbap_binaries_${LINUXVER} -czf target_stbap_binaries_${PLT}_${LINUXVER}.tar.gz ./
	mv target_stbap_binaries_${PLT}_${LINUXVER}.tar.gz ${PLT}-glibc/
	rm -rf ${STBAP_TARGET_DIR}
endif # CONFIG_STBAP

.PHONY: all clean distclean mrproper install package check_kernel_config
.PHONY: conf mconf oldconf kconf kmconf config menuconfig oldconfig
.PHONY: dummy netconf
.PHONY: libstdcpp libstdcpp-install
.PHONY: libmnl libmnl-install libmnl-clean
.PHONY: libnfnetlink libnfnetlink-install libnfnetlink-clean
.PHONY: libnetfilter_conntrack libnetfilter_conntrack-install libnetfilter_conntrack-clean
.PHONY: libnetfilter_queue libnetfilter_queue-install libnetfilter_queue-clean
.PHONY: libflow libflow-install libflow-clean
ifeq ($(CONFIG_VISUALIZATION),y)
.PHONY: libsqlite3 libsqlite3-install libsqlite3-clean
.PHONY: libjsonc libjsonc-install libjsonc-clean
.PHONY: libxml2 libxml2-install libxml2-clean
.PHONY: visdcoll visdcoll-install visdcoll-clean
.PHONY: visdcon visdcon-install visdcon-clean
endif
ifeq ($(CONFIG_HSPOT),y)
.PHONY: hspot_ap hspot_ap-install hspot_ap-clean
endif
ifeq ($(BUILD_BRCM_HOSTAPD),y)
.PHONY: libnl libnl-install libnl-clean
.PHONY: hostapd hostapd-install hostapd-clean
.PHONY: wpa_supplicant wpa_supplicant-install wpa_supplicant-clean
.PHONY: iw iw-install
.PHONY: openssl-install
endif
.PHONY: moca moca-install moca-clean
.PHONY: appeventd appeventd-install appeventd-clean
.PHONY: bsd bsd-install bsd-clean
.PHONY: wbd wbd-install wbd-clean
.PHONY: ieee1905 ieee1905-install ieee1905-clean
.PHONY: airiq airiq-install airiq-clean
.PHONY: ebtables ebtables-install ebtables-clean
.PHONY: stbap-modules stbap-modules-install stbap-modules-clean
.PHONY: stbap-pkg
