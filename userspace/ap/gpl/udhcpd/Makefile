# udhcp makefile

include ../config.mk
include ../config.in

prefix=$(TARGETDIR)/usr
SBINDIR=$(INSTALLDIR)/sbin
USRSBINDIR=${prefix}/sbin
USRBINDIR=${prefix}/bin
USRSHAREDIR=${prefix}/share

# Uncomment this to get a shared binary. Call as udhcpd for the server,
# and udhcpc for the client
#COMBINED_BINARY=1

# Uncomment this for extra output and to compile with debugging symbols
#DEBUG=1

# Uncomment this to output messages to syslog, otherwise, messages go to stdout
#CFLAGS += -DSYSLOG

#CROSS_COMPILE=arm-uclibc-
#CC = $(CROSS_COMPILE)gcc
#LD = $(CROSS_COMPILE)gcc
LD = $(CC)
INSTALL = install

VER := 0.9.8


OBJS_SHARED = options.o socket.o packet.o pidfile.o arpping.o

ifeq ($(CONFIG_ONE_SSID),y)
OBJS_SHARED += md5.o
endif

DHCPD_OBJS = dhcpd.o files.o leases.o serverpacket.o reserveip.o
DHCPC_OBJS = dhcpc.o clientpacket.o script.o

ifdef COMBINED_BINARY
EXEC1 = udhcpd
OBJS1 = $(DHCPD_OBJS) $(DHCPC_OBJS) $(OBJS_SHARED) frontend.o
CFLAGS += -DCOMBINED_BINARY
else
EXEC1 = udhcpd
OBJS1 = $(DHCPD_OBJS) $(OBJS_SHARED)

EXEC2 = udhcpc
OBJS2 = $(DHCPC_OBJS) $(OBJS_SHARED)
endif

EXEC3 = dumpleases
OBJS3 = dumpleases.o

BOOT_PROGRAMS = udhcpc
DAEMONS = udhcpd
COMMANDS = dumpleases

ifdef SYSLOG
CFLAGS += -DSYSLOG
endif

ifeq ($(CONFIG_XDSL_PRODUCT),y)
CFLAGS += -W -Wall -Wstrict-prototypes -DVERSION='"$(VER)"' -I$(INC_BRCMCFM_PATH) -I$(INC_BRCMDRIVER_PUB_PATH)/$(BRCM_BOARD) -I$(INC_BRCMSHARED_PUB_PATH)/$(BRCM_BOARD)
CFLAGS += -D_XDSL_PRODUCT
else
CFLAGS += -W -Wall -Wstrict-prototypes -DVERSION='"$(VER)"'
endif

ifeq ($(PROFILE),MS60)
CFLAGS += -DMS60
CFLAGS += -DBRCM_EZ_MESH -DBRCM_EZ_MESH_SATELLITE
endif 
ifeq ($(PROFILE),MR60)
CFLAGS += -DMR60
CFLAGS += -DBRCM_EZ_MESH -DBRCM_EZ_MESH_BASE
endif 

ifeq ($(PROFILE),MS80)
CFLAGS += -DMS80
CFLAGS += -DBRCM_EZ_MESH -DBRCM_EZ_MESH_SATELLITE
endif 
ifeq ($(PROFILE),MR80)
CFLAGS += -DMR80
CFLAGS += -DBRCM_EZ_MESH -DBRCM_EZ_MESH_BASE
endif 

#ifeq ($(CONFIG_PORTTRUNKING_SUPPORT),y)
#CFLAGS += -DLINK_AGG_IP_MANIPULATION
#endif

ifdef DEBUG
CFLAGS += -g -DDEBUG
STRIP=true
else
CFLAGS += -O2 -fomit-frame-pointer
ifneq ($(CONFIG_XDSL_PRODUCT),y)
STRIP=$(CROSS_COMPILE)strip
endif
endif

ifeq ($(CONFIG_NEW_WANDETECT),y)
CFLAGS += -DNEW_WANDETECT
endif

ifeq ($(IPV6RD_ENABLE_FLAG),y)
CFLAGS += -DACOS_IPV6RD
endif

ifeq ($(BCA_HNDROUTER),y)
CFLAGS += -DIOCTL_NO_POINTER
endif

all: $(EXEC1) $(EXEC2) $(EXEC3)
	$(STRIP) --remove-section=.note --remove-section=.comment $(EXEC1) $(EXEC2) $(EXEC3)

$(OBJS1) $(OBJS2) $(OBJS3): *.h Makefile
$(EXEC1) $(EXEC2) $(EXEC3): Makefile

.c.o:
	$(CC) -c $(CFLAGS) $<

$(EXEC1): $(OBJS1)
	$(LD)  $(OBJS1) -o $(EXEC1)
#	$(LD) $(LDFLAGS) $(OBJS1) -o $(EXEC1)

$(EXEC2): $(OBJS2)
	$(LD)  $(OBJS2) -o $(EXEC2)
#	$(LD) $(LDFLAGS) $(OBJS2) -o $(EXEC2)

$(EXEC3): $(OBJS3)
	$(LD)  $(OBJS3) -o $(EXEC3)
#	$(LD) $(LDFLAGS) $(OBJS3) -o $(EXEC3)

ifeq ($(CONFIG_XDSL_PRODUCT),y)
install: all
	install -m 755 udhcpd $(INSTALL_DIR)/bin
	$(STRIP) $(INSTALL_DIR)/bin/udhcpd
	ln -sf udhcpd $(INSTALL_DIR)/bin/dhcpc
	ln -sf udhcpd $(INSTALL_DIR)/bin/dhcpd

clean:
	-rm -f udhcpd udhcpc dhcpc dhcpd dumpleases *.o core
else
install: all

	$(INSTALL) $(DAEMONS) $(USRSBINDIR)
#foxconn removed, water, 05/15/2009, dumplease is not necessary
#	$(INSTALL) $(COMMANDS) $(USRBINDIR)
ifdef COMBINED_BINARY
	cd $(USRSBINDIR) && ln -sf $(DAEMONS) $(BOOT_PROGRAMS)
else
	$(INSTALL) $(BOOT_PROGRAMS) $(USRSBINDIR)
endif

clean:
	-rm -f udhcpd udhcpc dumpleases *.o core
endif

