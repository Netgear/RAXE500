LIB := libbcm_dbus_strproto.so

all install: conditional_build

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common

ARCH                  := $(PROFILE_ARCH)
LIB_INSTALL_DIR       := $(BCM_FSBUILD_DIR)/private/lib
HEADER_INSTALL_DIR    := $(BCM_FSBUILD_DIR)/private/include
ALLOWED_INCLUDE_PATHS := -I . \
                         -I $(HEADER_INSTALL_DIR) \
                         -I$(BUILD_DIR)/userspace/public/include  \
                         -I$(BUILD_DIR)/userspace/public/include/$(OALDIR) \
                         -I$(BUILD_DIR)/userspace/private/include  \
                         -I$(BUILD_DIR)/userspace/private/include/$(OALDIR) \
                         -I$(BCM_FSBUILD_DIR)/public/include \
                         -I$(BCM_FSBUILD_DIR)/private/include

CFLAGS += -Werror -Wfatal-errors

CFLAGS += -I$(BCM_FSBUILD_DIR)/public/lib/glib-2.0/include
CFLAGS += -I$(BCM_FSBUILD_DIR)/public/include/glib-2.0
CFLAGS += -I$(BCM_FSBUILD_DIR)/public/include/gio-unix-2.0

LIBS += -L$(INSTALL_DIR)/lib -lpthread -lrt
LIBS += -L$(INSTALL_DIR)/lib/public -lexpat -lffi -ldbus-1
ifneq ($(findstring uclibc, $(TOOLCHAIN_PREFIX)),)
LIBS += -lintl
endif
LIBS += -lgio-2.0 -lglib-2.0 -lgmodule-2.0 -lgobject-2.0 -lgthread-2.0 -lz

export ARCH CFLAGS LIB_INSTALL_DIR HEADER_INSTALL_DIR

# Final location of LIB for system image.  Only the BRCM build system needs to
# know about this.
FINAL_LIB_INSTALL_DIR := $(INSTALL_DIR)/lib$(BCM_INSTALL_SUFFIX_DIR)

ifneq ($(strip $(BUILD_OPEN_PLATFORM)),)

COND_BUILD_DBUS_STRPROTO := 1

else

ifneq ($(and $(strip $(BUILD_BEEP)),$(strip $(BUILD_BRCM_CMS))),)

COND_BUILD_DBUS_STRPROTO := 1

endif

endif

ifeq ($(strip $(COND_BUILD_DBUS_STRPROTO)),1)
conditional_build:
	mkdir -p objs
	$(MAKE) -C objs -f ../Makefile install
	mkdir -p  $(FINAL_LIB_INSTALL_DIR)
	cp -p $(LIB_INSTALL_DIR)/$(LIB) $(FINAL_LIB_INSTALL_DIR)

else

conditional_build:
	@echo "Skipping $(LIB) (not configured)"

endif

clean:
	rm -f $(FINAL_LIB_INSTALL_DIR)/$(LIB)
	-mkdir -p objs
	-$(MAKE) -C objs -f ../Makefile clean
	rm -rf objs

CONSUMER_RELEASE_BINARYONLY_PREPARE: binaryonly_prepare

binaryonly_prepare:
	-mkdir -p objs
	-$(MAKE) -C objs -f ../Makefile binaryonly_prepare
	rm -rf objs

shell:
	@echo "Entering makefile debug shell (type exit to exit) >>>"
	@bash -i
	@echo "exiting debug shell."
