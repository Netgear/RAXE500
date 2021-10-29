EXE  = wdtctl

OBJS = wdtctl.o


all install : $(EXE)
	mkdir -p $(PREFIX)/bin
	install -p -m 755 $(EXE) $(PREFIX)/bin


$(EXE): $(OBJS)
	$(CC) $(BCM_LD_FLAGS) -o $@ $(OBJS) -Wl,-rpath,$(LIBS)

clean: 
	-rm -f *.o *.d $(EXE) $(PREFIX)/bin/$(EXE)


# Set VPATH because we want to compile in a seperate dir than source.
name := $(lastword $(MAKEFILE_LIST))
mdir := $(realpath $(dir $(name)))
VPATH=$(mdir)

# Generate and use dependencies.
CFLAGS += -MD
-include $(OBJS:%.o=%.d)
