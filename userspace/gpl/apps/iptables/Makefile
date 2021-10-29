
all dynamic install: conditional_build


#
# Set our CommEngine directory (by splitting the pwd into two words
# at /userspace and taking the first word only).
# Then include the common defines under CommEngine.
# You do not need to modify this part.
#
CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/make.common

APP = iptables-1.8.5

.PHONY: check_untar_patch_configure conditional_build

ifeq "$(BCM_LD_FLAGS)" "-m32"
BCM_HOST := i386
else
BCM_HOST := $(TOOLCHAIN_PREFIX)
endif
ifneq ($(strip $(BRCM_KERNEL_NETFILTER)),)
ifneq ($(strip $(BUILD_IPTABLES)),)
DO_IPV6:=
PF4_EXT:=icmp
PF6_EXT:=
PFX_EXT:=TCPMSS SKIPLOG addrtype blog mac_extend flowlabel u32
ifneq ($(strip $(BRCM_KERNEL_NF_FIREWALL)),)
PF4_EXT+=LOG
PFX_EXT+=length limit standard conntrack tcp udp NFQUEUE time
endif
ifneq ($(strip $(BRCM_KERNEL_NF_NAT)),)
PF4_EXT+=DNAT MASQUERADE REDIRECT SNAT
endif
ifneq ($(strip $(BRCM_KERNEL_NF_MANGLE)),)
PFX_EXT+=dscp DSCP tos TOS mac mark MARK
endif
ifneq ($(strip $(BUILD_IPV6)),)
PF6_EXT+=ah dst eui64 frag hbh hl icmp6 ipv6header mh rt HL LOG REJECT SNPT DNPT NETMAP
PFX_EXT+=owner policy sctp
DO_IPV6:=--enable-ipv6
endif
ifneq ($(strip $(BRCM_KERNEL_NF_LAYER7_FILTER)),)
PFX_EXT+=layer7 DC
endif
ifneq ($(strip $(BUILD_IPSET)),)
PFX_EXT+=set SET
endif
ifneq ($(strip $(BRCM_KERNEL_NF_FIREWALL_EXT)),)
PFX_EXT+=hashlimit
endif

ifneq ($(strip $(BRCM_KERNEL_NF_NAT_ALG_IPSEC)),)
BRCM_FLAG+= -DSUPPORT_IPSEC_PASSTHROUGH
endif
BRCM_FLAG+= -fPIC

check_untar_patch_configure: sanity_check
	@if [ ! -e $(APP)/INSTALL ]; then \
	    echo "Untarring original $(APP) source"; \
	    (tar xkjf $(APP).tar.bz2 2> /dev/null || true); \
	    echo "Applying patches to $(APP)"; \
            patch -p1 -b -s -d$(APP) < $(APP).patch; \
	    if [ $$? -ne 0 ]; then \
	         exit 1; \
	    fi; \
	    echo "Checking configure on $(APP)/configure"; \
	    if [ -e $(APP)/configure ]; then \
	        echo "Running configure on $(APP)"; \
	        echo "building to $(INSTALL_DIR)"; \
	        cd $(APP); \
	        export PKG_CONFIG_LIBDIR=$(INSTALL_DIR)/lib/gpl; \
	        ./configure --host=$(BCM_HOST) \
	        --target=$(TOOLCHAIN_PREFIX) \
	        --with-pfxmods="$(PFX_EXT)" \
            --with-pf4mods="$(PF4_EXT)" \
            --with-pf6mods="$(PF6_EXT)" \
            --with-xt-lock-name=/tmp/xtables.lock \
            $(DO_IPV6) \
            --disable-devel \
            --disable-shared \
            --disable-nftables \
            --prefix=$(BCM_FSBUILD_DIR)/gpl \
            --sbindir=$(INSTALL_DIR)/bin \
            --with-kbuild=$(TOOLCHAIN_INCLUDE_DIR) \
            LDFLAGS=-L$(INSTALL_DIR)/lib \
            CFLAGS='-s -Os -Wno-strict-aliasing $(BRCM_FLAG)';\
            autoreconf -i; \
	    fi; \
	fi

conditional_build: check_untar_patch_configure
	cd $(APP); $(MAKE) -j1 all install
	cp -urf $(APP)/include $(BCM_FSBUILD_DIR)/gpl/.
ifeq ($(APP),iptables-1.4.0)
	install -m 755 $(APP)/iptables $(INSTALL_DIR)/bin
ifneq ($(strip $(BUILD_IPV6)),)
	install -m 755 $(APP)/ip6tables $(INSTALL_DIR)/bin
endif
else
	install -m 755 $(APP)/iptables/xtables-legacy-multi $(INSTALL_DIR)/bin
	ln -sf xtables-legacy-multi $(INSTALL_DIR)/bin/iptables
ifneq ($(strip $(BUILD_IPV6)),)
	ln -sf xtables-legacy-multi $(INSTALL_DIR)/bin/ip6tables
endif
endif
else
conditional_build: sanity_check
	@echo "skipping $(APP) (not configured)"
endif
else
conditional_build: sanity_check
	@echo "skipping $(APP) (NETFILTER not configured)"
endif

# NOTE: make clean from within app does not do a proper job, so wiping out
# entire directory to ensure consistency.
clean:
	rm -rf $(APP)
	rm -f $(INSTALL_DIR)/bin/iptables
	rm -f $(INSTALL_DIR)/bin/ip6tables
	rm -f $(INSTALL_DIR)/bin/xtables-legacy-multi

# The next line is a hint to our release scripts
# GLOBAL_RELEASE_SCRIPT_CALL_DISTCLEAN
distclean: clean



