LIB  := libbcm_boardctl.so

OBJS := 

CFLAGS += -Werror -Wfatal-errors

# These are public API headers needed by other apps which want to call this lib.
# Private/internal headers should not be listed here.
HEADERS := bcm_boardctl.h bcm_boarddriverctl.h

# OS abstraction layer
OALDIR := linux

all: $(LIB)

install: all
	mkdir -p $(LIB_INSTALL_DIR)
	install -t $(LIB_INSTALL_DIR) $(LIB)
	mkdir -p $(HEADER_INSTALL_DIR)
	install -p -m 444 -t $(HEADER_INSTALL_DIR) $(addprefix $(mdir)/,$(HEADERS))

clean:
	rm -f $(LIB) $(OBJS)
	rm -f $(OALDIR)/oal.a $(OALDIR)/*.o
	rm -f $(LIB_INSTALL_DIR)/$(LIB)
	rm -f $(addprefix $(HEADER_INSTALL_DIR)/,$(HEADERS))


$(LIB): $(OBJS) force_oal
	$(CC) -shared $(BCM_LD_FLAGS) -Wl,--whole-archive,-soname,$(notdir $@) \
	    -o $@ $(OBJS) $(OALDIR)/oal.a -Wl,--no-whole-archive

# At this level, we don't know if anything in oaldir has changed, so always go
# down into that dir and do a make.
force_oal:
	mkdir -p $(OALDIR)
	make -C $(OALDIR) -f $(mdir)/$(OALDIR)/Makefile


# Set VPATH because we want to compile in a seperate dir than source.
name := $(lastword $(MAKEFILE_LIST))
mdir := $(realpath $(dir $(name)))
VPATH=$(mdir)

-include $(OBJS:%.o=%.d)
