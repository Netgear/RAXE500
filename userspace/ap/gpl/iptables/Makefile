############################################################################################
#
#	Makefile For iptables
#
#	Author: Bob
#	Date: 2020.10.23
#	
#	Description:
#
#
#############################################################################################
include $(GPLTOPDIR)/config.mk
include $(GPLTOPDIR)/config.in

IPTABLES_VER := 1.6.2

PFX_EXT = TCPMSS SKIPLOG addrtype blog mac_extend flowlabel u32 length limit standard conntrack tcp udp NFQUEUE time dscp DSCP tos TOS mac mark MARK 
PFX_EXT += owner policy AUDIT CHECKSUM CLASSIFY cluster comment connbytes connlimit  connmark CONNMARK CONNSECMARK cpu dccp devgroup dscp DSCP exp 
PFX_EXT += hashlimit helper IDLETIMER iprange ipvs LED mac mark MARK multiport NFLOG NFQUEUE NOTRACK osf owner physdev pkttype policy quata rateest 
PFX_EXT += RATEEST recent sctp SECMARK set SET socket state statistic string tcpmss TCPOPTSTRIP TEE tos TOS TPROXY TRACE u32 BDCONNMARK
PF4_EXT = icmp LOG DNAT MASQUERADE REDIRECT SNAT ecn ECN MIRROR NETMAP realm SAME TRIGGER ttl TTL unclean REJECT
PF6_EXT = ah dst eui64 frag hbh hl icmp6 ipv6header mh rt HL LOG REJECT

all: iptables_config
	make -C iptables-$(IPTABLES_VER)

iptables_config:
	if [ ! -f ./iptables-$(IPTABLES_VER)/Makefile ];then \
	echo "Untarring original iptables-$(IPTABLES_VER) source"; \
	tar xkjf iptables-$(IPTABLES_VER).tar.bz2; \
	echo "Applying patches to iptables-$(IPTABLES_VER)"; \
	patch -p1 -d iptables-$(IPTABLES_VER) < iptables_onebyone.patch; \
	cp libxt_BDCONNMARK.c iptables-$(IPTABLES_VER)/extensions/ ; \
	cd iptables-$(IPTABLES_VER) && \
	(./configure --host=$(TARGET_PLATFORM_ARCH)-linux \
		--target=$(TARGET_PLATFORM_ARCH)-linux \
		--with-pfxmods="$(PFX_EXT)" \
        --with-pf4mods="$(PF4_EXT)" \
        --with-pf6mods="$(PF6_EXT)" \
        --with-xt-lock-name=/tmp/xtables.lock \
        --enable-ipv6 \
        --disable-devel \
        --disable-shared \
        --disable-nftables \
        --sbindir=$(INSTALL_DIR)/bin \
        ); \
	fi

clean:
	rm -f $(TARGETDIR)/bin/iptables
	rm -f $(TARGETDIR)/bin/iptables-restore
	rm -f $(TARGETDIR)/bin/iptables-save
	rm -f $(TARGETDIR)/bin/ip6tables
	rm -f $(TARGETDIR)/bin/ip6tables-restore
	rm -f $(TARGETDIR)/bin/ip6tables-save
	rm -f $(TARGETDIR)/bin/xtables-multi
		
install:
	install -d $(TARGETDIR)/bin
	install -m 755 iptables-$(IPTABLES_VER)/iptables/xtables-multi $(TARGETDIR)/bin
	ln -sf xtables-multi $(TARGETDIR)/bin/iptables
	ln -sf xtables-multi $(TARGETDIR)/bin/iptables-restore
	ln -sf xtables-multi $(TARGETDIR)/bin/iptables-save
	ln -sf xtables-multi $(TARGETDIR)/bin/ip6tables
	ln -sf xtables-multi $(TARGETDIR)/bin/ip6tables-restore
	ln -sf xtables-multi $(TARGETDIR)/bin/ip6tables-save
	
