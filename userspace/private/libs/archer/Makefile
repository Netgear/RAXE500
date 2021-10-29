
# Set VPATH because we want to compile in a seperate dir than source.
name := $(lastword $(MAKEFILE_LIST))
mdir := $(realpath $(dir $(name)))
VPATH=$(mdir)


LIB = libarcher.so

SAVED = $(LIB)_$(ARCH).shipped

OBJS = archer_api.o

BRCM_RELEASETAG ?= "unknown"
CFLAGS += -Werror -Wfatal-errors -DBRCM_RELEASETAG=\"$(BRCM_RELEASETAG)\"

$(LIB) :

install: $(LIB)
	mkdir -p $(LIB_INSTALL_DIR)
	install -p -t $(LIB_INSTALL_DIR) $(LIB)

clean:
	rm -f *.o *.d $(LIB)
	rm -f $(LIB_INSTALL_DIR)/$(LIB)

ifneq ($(wildcard $(mdir)/*.c),)

$(LIB): $(OBJS)
	$(CC) -shared $(BCM_LD_FLAGS) -Wl,--whole-archive,-soname,$(notdir $@) -o $@ $(OBJS) -Wl,--no-whole-archive

else

$(LIB): $(SAVED)
	cat $< > $@

endif

binaryonly_prepare:
	-cp $(LIB) $(mdir)/$(SAVED)
	-$(MAKE) -f $(name) clean
	rm -rf $(mdir)/*.c $(mdir)/*.h


# Generate and use dependencies.
CFLAGS += -MD
-include $(OBJS:%.o=%.d)
