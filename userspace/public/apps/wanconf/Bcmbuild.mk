EXE := wanconf

all install: conditional_build


CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common

ARCH=$(PROFILE_ARCH)
EXE_INSTALL_DIR       := $(BCM_FSBUILD_DIR)/bin
ALLOWED_INCLUDE_PATHS := -I.\
                         -I $(BCM_FSBUILD_DIR)/public/include \
                         -I $(BCM_FSBUILD_DIR)/private/include \
                         -I $(BCM_FSBUILD_DIR)/public/lib/glib-2.0/include \
                         -I $(BCM_FSBUILD_DIR)/public/include/glib-2.0 \
                         -I $(BCM_FSBUILD_DIR)/public/include/gio-unix-2.0 \
                         -I$(BUILD_DIR)/userspace/public/include  \
                         -I$(BUILD_DIR)/userspace/public/include/$(OALDIR) \
                         -I$(BUILD_DIR)/userspace/private/include  \
                         -I$(BUILD_DIR)/userspace/private/include/$(OALDIR) \
                         -I$(INC_BRCMDRIVER_PUB_PATH)/$(BRCM_BOARD)  \
                         -I$(INC_BRCMSHARED_PUB_PATH)/$(BRCM_BOARD)

# RDPA part
ALLOWED_INCLUDE_PATHS += -I$(PROJECT_DIR)/rdp/drivers/rdpa_gpl/include/ \
                         -I$(PROJECT_DIR)/target/bdmf/framework \
                         -I$(PROJECT_DIR)/target/bdmf/system \
                         -I$(PROJECT_DIR)/target/bdmf/system/sim \
                         -I$(PROJECT_DIR)/target/rdpa_gpl \
                         -I$(PROJECT_DIR)/target/rdpa_user \
                         -I$(PROJECT_DIR)/target/rdpa_user/include \
                         -I$(PROJECT_DIR)/target/rdpa_user/include/autogen/autogen

ALLOWED_LIB_DIRS      := /lib:/lib/public

# Remove all mdm_cbk_* and bcm_*_hal libs from CMS_CORE_LIBS.  The local
# Makefile will specify which mdm_cbk and hal libs it needs.
MDM_CORE_LIBS := $(patsubst -lmdm_cbk_%,,$(CMS_CORE_LIBS))
MDM_CORE_LIBS := $(patsubst -lbcm_%_hal,,$(MDM_CORE_LIBS))

export ARCH CFLAGS BCM_LD_FLAGS CMS_LIB_PATH CMS_LIB_RPATH EXE_INSTALL_DIR
export WAN_AUTODETECT RELEASE_BUILD BRCM_PON_WAN_TYPE_AUTO_DETECT BUILD_PMD
export LINUX_VER_STR BUILD_BRCM_CMS BUILD_BRCM_BDK BUILD_DBUS BUILD_UBUS BUILD_BDK_SYSTEM_MANAGEMENT
export MDM_CORE_LIBS


# Final location of EXE for system image.  Only the BRCM build system needs to
# know about this.
FINAL_EXE_INSTALL_DIR := $(INSTALL_DIR)/bin

ifneq ($(strip $(WAN_AUTODETECT)),)

conditional_build: clean_legacy
	mkdir -p objs
	$(MAKE) -C objs -f ../Makefile install
	mkdir -p $(FINAL_EXE_INSTALL_DIR)
	cp -p $(EXE_INSTALL_DIR)/$(EXE) $(FINAL_EXE_INSTALL_DIR)

else

conditional_build:
	@echo "skipping $(LIB) (not configured)"

endif


clean: clean_legacy
	rm -f $(FINAL_EXE_INSTALL_DIR)/$(EXE)
	-mkdir -p objs
	-$(MAKE) -C objs -f ../Makefile clean
	rm -rf objs

# delete objects left over from old Makefile. (Not needed for new directory
# which started with dual makefiles.)
clean_legacy:
	rm -f *.o *.d $(EXE)


shell:
	@echo "Entering makefile debug shell (type exit to exit) >>>"
	@bash -i
	@echo "exiting debug shell."
