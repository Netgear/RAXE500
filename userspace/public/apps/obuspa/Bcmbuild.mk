EXE := obuspa

all install: conditional_build


CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common


ARCH=$(PROFILE_ARCH)
EXE_INSTALL_DIR       := $(BCM_FSBUILD_DIR)/bin
LIB_INSTALL_DIR       := $(BCM_FSBUILD_DIR)/public/lib
HEADER_INSTALL_DIR    := $(BCM_FSBUILD_DIR)/public/include
ALLOWED_INCLUDE_PATHS := -I . \
                         -I $(BUILD_DIR)/userspace/public/include \
                         -I $(HEADER_INSTALL_DIR)

export ARCH CFLAGS BCM_LD_FLAGS BCM_FSBUILD_DIR
export EXE_INSTALL_DIR LIB_INSTALL_DIR HEADER_INSTALL_DIR
export DESKTOP_LINUX BUILD_LIBCURL_WITH_SSL BUILD_LIBCURL_WITH_HTTP2


# Final location of EXE for system image.  Only the BRCM build system needs to
# know about this.
FINAL_EXE_INSTALL_DIR := $(INSTALL_DIR)/bin

ifneq ($(strip $(BUILD_OBUSPA)),)
conditional_build:
	$(MAKE) -f Makefile install
	mkdir -p $(FINAL_EXE_INSTALL_DIR)
	cp -p $(EXE_INSTALL_DIR)/$(EXE) $(FINAL_EXE_INSTALL_DIR)
else
conditional_build:
	@echo "skipping $(EXE) (not configured)"
endif

clean:
	rm -f $(FINAL_EXE_INSTALL_DIR)/$(EXE)
	-$(MAKE) -f Makefile clean

shell:
	bash -i
