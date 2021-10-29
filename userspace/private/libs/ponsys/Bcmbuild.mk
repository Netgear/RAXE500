LIB = libponsys.so

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
                         -I $(BCM_FSBUILD_DIR)/public/include \
                         -I $(BUILD_DIR)/userspace/public/include  \
                         -I $(BUILD_DIR)/userspace/public/include/$(OALDIR) \
                         -I $(BUILD_DIR)/userspace/private/include  \
                         -I $(BUILD_DIR)/userspace/private/include/$(OALDIR) \
                         -I $(BUILD_DIR)/shared/opensource/include/bcm963xx \
                         -I $(BUILD_DIR)/shared/private/include/$(OALDIR) \
                         -I $(INC_BRCMDRIVER_PRIV_PATH)/$(BRCM_BOARD) \
                         -I $(INC_BRCMDRIVER_PUB_PATH)/$(BRCM_BOARD) \
                         -I $(PROJECT_DIR)/target/rdpa_user \
                         -I $(PROJECT_DIR)/target/bdmf/system \
                         -I $(PROJECT_DIR)/target/bdmf/system/sim \
                         -I $(BUILD_DIR)/rdp/drivers/bdmf/framework/ \
                         -I $(BUILD_DIR)/rdp/drivers/rdpa_gpl/include

export ARCH CFLAGS LIB_INSTALL_DIR HEADER_INSTALL_DIR DESKTOP_LINUX

# Final location of LIB for system image.  Only the BRCM build system needs to
# know about this.
FINAL_LIB_INSTALL_DIR := $(INSTALL_DIR)/lib$(BCM_INSTALL_SUFFIX_DIR)

ifneq ($(strip $(BUILD_RDPACTL)),)

conditional_build:
	mkdir -p objs
	$(MAKE) -C objs -f ../Makefile install
	mkdir -p $(FINAL_LIB_INSTALL_DIR)
	cp -p $(LIB_INSTALL_DIR)/$(LIB) $(FINAL_LIB_INSTALL_DIR)

else

conditional_build:
	@echo "Skipping $(LIB) (BUILD_RDPACTL not configured)"

endif


clean: clean_legacy
	rm -f $(FINAL_LIB_INSTALL_DIR)/$(LIB)
	-mkdir -p objs
	-$(MAKE) -C objs -f ../Makefile clean
	rm -rf objs

# delete objects left over from old Makefile. (Not needed for new directory
# which started with split makefiles.)
clean_legacy:
	rm -f *.o *.d $(LIB)


CONSUMER_RELEASE_BINARYONLY_PREPARE: binaryonly_prepare

binaryonly_prepare:
	-mkdir -p objs
	-$(MAKE) -C objs -f ../Makefile binaryonly_prepare
	rm -rf objs


shell:
	@echo "Entering makefile debug shell (type exit to exit) >>>"
	@bash -i
	@echo "exiting debug shell."
