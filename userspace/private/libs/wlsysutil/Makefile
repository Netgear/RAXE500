
name := $(lastword $(MAKEFILE_LIST))
mdir := $(realpath $(dir $(name)))
VPATH=$(mdir)


LIB = libwlsysutil.so

SAVED = $(LIB)_$(ARCH).shipped

OBJS = wlsysutil.o

$(LIB) :

install : $(LIB)
	mkdir -p $(PREFIX)/lib/ 
	install $(LIB) $(PREFIX)/lib/ 

clean: 
	rm -f *.d *.o
	rm -f $(LIB) $(OBJS) $(PREFIX)/lib/$(LIB)
	rm -f $(PREFIX)/include/wlsysutil.h

ifneq ($(wildcard $(mdir)/*.c),)

$(LIB): $(OBJS)
	$(CC) -shared $(BCM_LD_FLAGS) -Wl,--whole-archive,-soname,$(notdir $@) -o $@ $(OBJS) -Wl,--no-whole-archive
	mkdir -p $(PREFIX)/include/ 
	install -m 644 wlsysutil.h $(PREFIX)/include/ 

else

$(LIB): $(SAVED)
	cat $< > $@

endif

binaryonly_prepare:
	-cp $(LIB) $(SAVED)
	-$(MAKE) -f $(name) clean
	rm -rf $(mdir)/*.c $(mdir)/*.h


# Generate and use dependencies.
CFLAGS += -MD
-include $(OBJS:%.o=%.d)
