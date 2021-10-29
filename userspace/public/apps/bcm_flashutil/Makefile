EXE := bcm_flashutil

OBJS := bcm_flashutil.o

LIBS := -lbcm_flashutil -lbcm_boardctl
LIBS += -lbcm_util -lsys_util -lgen_util -lrt

CFLAGS += -Werror -Wfatal-errors
CFLAGS += -I$(BUILD_DIR)/userspace/public/libs/bcm_flashutil


all: $(EXE)

install: all
	mkdir -p $(EXE_INSTALL_DIR)
	install -p -m 755 $(EXE) $(EXE_INSTALL_DIR)

clean:
	rm -f *.o *.d $(EXE)
	rm -f $(EXE_INSTALL_DIR)/$(EXE)


$(EXE): $(OBJS)
	$(CC) $(BCM_LD_FLAGS) -o $@ $(OBJS) -Wl,-rpath,$(CMS_LIB_RPATH) $(CMS_LIB_PATH) $(LIBS)

# Set VPATH because we want to compile in a seperate dir than source.
name := $(lastword $(MAKEFILE_LIST))
mdir := $(realpath $(dir $(name)))
VPATH=$(mdir)

# Generate and use dependencies.
CFLAGS += -MD
-include $(OBJS:%.o=%.d)

