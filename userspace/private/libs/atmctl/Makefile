
name := $(lastword $(MAKEFILE_LIST))
mdir := $(realpath $(dir $(name)))
VPATH=$(mdir)


LIB = libatmctl.so

SAVED = $(LIB)_$(ARCH).shipped

OBJS = 

$(LIB) :

install : $(LIB)
	mkdir -p $(PREFIX)/lib/ 
	install $(LIB) $(PREFIX)/lib/ 
	# mkdir -p $(PREFIX)/include/ 
	# install $(LIB) $(PREFIX)/include/ 

clean: 
	rm -f *.o *.d $(LIB) $(PREFIX)/lib/$(LIB)
	-$(MAKE) -C $(OALDIR) -f $(mdir)/$(OALDIR)/Makefile clean

ifneq ($(wildcard $(mdir)*.c)$(wildcard $(mdir)/$(OALDIR)/*.c),)

$(LIB): $(OBJS)
	mkdir -p $(OALDIR)
	$(MAKE) -C $(OALDIR) -f $(mdir)/$(OALDIR)/Makefile
	$(CC) -shared $(BCM_LD_FLAGS) -Wl,--whole-archive,-soname,$(notdir $@) -o $@ $(OBJS) $(OALDIR)/oal.a -Wl,--no-whole-archive

else

$(LIB): ../$(SAVED)
	cat $< > $@

endif

binaryonly_prepare:
	-cp $(LIB) ../$(SAVED)
	-$(MAKE) -f $(name) clean
	rm -rf $(mdir)/*.c $(mdir)/*.h $(mdir)/$(OALDIR)


# Generate and use dependencies.
CFLAGS += -MD
-include $(OBJS:%.o=%.d)
