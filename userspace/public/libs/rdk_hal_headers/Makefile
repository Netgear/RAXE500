# This directory only installs header files.  No library.
LIB :=

OBJS :=

CFLAGS += -Werror -Wfatal-errors

# These are public API headers needed by other apps which want to call this lib.
# Private/internal headers should not be listed here.
HEADERS := ccsp_hal_ethsw.h dpoe_hal.h mta_hal.h \
           platform_hal.h vlan_hal.h \
           wifi_hal.h

all install:
	mkdir -p $(HEADER_INSTALL_DIR)
	install -p -m 444 -t $(HEADER_INSTALL_DIR) $(addprefix $(mdir)/,$(HEADERS))

clean:
	rm -f $(addprefix $(HEADER_INSTALL_DIR)/,$(HEADERS))



# Set VPATH because we want to compile in a seperate dir than source.
name := $(lastword $(MAKEFILE_LIST))
mdir := $(realpath $(dir $(name)))
VPATH=$(mdir)

# Generate and use dependencies.
CFLAGS += -MD
-include $(OBJS:%.o=%.d)
