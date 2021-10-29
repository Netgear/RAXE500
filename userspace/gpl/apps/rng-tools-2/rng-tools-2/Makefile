# rng-tools-2 makefile

LDFLAGS = -Wl,--allow-shlib-undefined

prefix=/usr
SBINDIR=/sbin
USRSBINDIR=${prefix}/sbin
USRBINDIR=${prefix}/bin
USRSHAREDIR=${prefix}/share

# Uncomment this to get a shared binary. Call as rngd for the server,
# and rngtest for the client
COMBINED_BINARY=1


INSTALL = install

VER := 0.9.6


OBJS_SHARED = fips.o
RNGTEST_OBJS = stats.o rngtest.o

# In Linux 4.19, we do not need rngd to feed entropy to kernel, so do not
# build it, do not start it.
# EXEC1 = rngd
# OBJS1 = $(RNGD_OBJS) $(OBJS_SHARED)
# RNGD_OBJS = rngd_entsource.o rngd_linux.o rngd.o

EXEC2 = rngtest
OBJS2 = $(RNGTEST_OBJS) $(OBJS_SHARED)

COMMANDS = rngtest

CFLAGS += -s -W -Wall -Wstrict-prototypes -DVERSION='"$(VER)"' -I$(INC_BRCMDRIVER_PUB_PATH)/$(BRCM_BOARD) -I$(INC_BRCMSHARED_PUB_PATH)/$(BRCM_BOARD) 
CFLAGS += -I$(TOOLCHAIN)/include
CFLAGS += $(BRCM_WERROR_CFLAGS)

LIBS=

#
# Use between the following 2 lines depending on whether you want verbose debug or not.
#
#CFLAGS += -g -DVERBOSE
CFLAGS += -Os -fomit-frame-pointer -DHAVE_CONFIG_H

# BRCM begin
all: $(EXEC2)
# BRCM end

$(OBJS2) : *.h Makefile
$(EXEC2) : Makefile

.c.o:
	$(CC) -c $(CFLAGS) $<



$(EXEC2): $(OBJS2)
	$(CC) $(LDFLAGS) $(OBJS2) -o $(EXEC2)

# BRCM begin
#install:
# BRCM end - Note that next line is inserted
install: rngtest
	install -m 755 rngtest $(INSTALL_DIR)/bin
	$(STRIP) $(INSTALL_DIR)/bin/rngtest


clean:
	-rm -f rngd rngtest *.o core

