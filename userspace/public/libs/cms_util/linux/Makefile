LIB  := oal.a

OBJS := oal_logging.o oal_readlog.o oal_memory.o oal_timestamp.o \
       oal_strconv.o oal_pid.o \
       oal_prctl.o oal_network.o oal_file.o oal_passwd.o


all install: $(LIB)

clean:
	rm -f *.o *.d $(LIB)


$(LIB) : $(OBJS)
	$(AR) -rcs $@ $(OBJS)


# Set VPATH because we want to compile in a seperate dir than source.
name := $(lastword $(MAKEFILE_LIST))
mdir := $(realpath $(dir $(name)))
VPATH=$(mdir)

# Generate and use dependencies.
CFLAGS += -MD
-include $(OBJS:%.o=%.d)
