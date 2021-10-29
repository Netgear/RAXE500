LIB := libcms_util.so

all install: conditional_build


CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common


ARCH=$(PROFILE_ARCH)
LIB_INSTALL_DIR       := $(BCM_FSBUILD_DIR)/public/lib
HEADER_INSTALL_DIR    := $(BCM_FSBUILD_DIR)/public/include
ETC_INSTALL_DIR       := $(BCM_FSBUILD_DIR)/etc/cms_entity_info.d
ALLOWED_INCLUDE_PATHS := -I . \
                         -I$(BUILD_DIR)/userspace/public/include  \
                         -I$(BUILD_DIR)/userspace/public/include/$(OALDIR) \
                         -I$(BCM_FSBUILD_DIR)/public/include \
                         -I$(INC_BRCMSHARED_PUB_PATH)/$(BRCM_BOARD)  \
                         -I$(INC_BRCMDRIVER_PUB_PATH)/$(BRCM_BOARD)  \
                         -I$(INC_BRCMDRIVER_PRIV_PATH)/$(BRCM_BOARD) 

export ARCH CFLAGS LIB_INSTALL_DIR HEADER_INSTALL_DIR ETC_INSTALL_DIR \
       BUILD_DIR KERNEL_DIR BRCM_CHIP
export BUILD_BRCM_CMS


# Final location of LIB for system image.  Only the BRCM build system needs to
# know about this.
FINAL_LIB_INSTALL_DIR := $(INSTALL_DIR)/lib$(BCM_INSTALL_SUFFIX_DIR)
FINAL_ETC_INSTALL_DIR := $(INSTALL_DIR)/etc/cms_entity_info.d

EID_FILES := eid_bcm_base.txt \
             eid_bcm_mgmt.txt \
             symbol_table.txt


ifneq ($(strip $(BUILD_BRCM_CMS)),)
COND_BUILD_CMS_UTIL := 1
EID_FILES += eid_bcm_voip.txt eid_bcm_wifi.txt
endif
ifneq ($(strip $(BUILD_DISTRIBUTED_MDM)),)
COND_BUILD_CMS_UTIL := 1
endif
ifneq ($(strip $(BUILD_CMS_UTIL)),)
COND_BUILD_CMS_UTIL := 1
endif

ifeq ($(strip $(COND_BUILD_CMS_UTIL)),1)

conditional_build: clean_legacy
	mkdir -p objs
	$(MAKE) -C objs -f ../Makefile install
	mkdir -p $(FINAL_LIB_INSTALL_DIR)
	cp -p $(LIB_INSTALL_DIR)/$(LIB) $(FINAL_LIB_INSTALL_DIR)
	mkdir -p $(FINAL_ETC_INSTALL_DIR)
	install -p -m 444 -t $(FINAL_ETC_INSTALL_DIR) $(addprefix $(ETC_INSTALL_DIR)/,$(EID_FILES)) 

else

conditional_build:
	@echo "Skipping $(LIB) (not configured)"

endif


clean: clean_legacy
	rm -f $(FINAL_LIB_INSTALL_DIR)/$(LIB)
	rm -f $(addprefix $(FINAL_ETC_INSTALL_DIR)/,$(EID_FILES))
	-mkdir -p objs
	-$(MAKE) -C objs -f ../Makefile clean
	rm -rf objs

# delete objects left over from old Makefile. (Not needed for new directory
# which started with dual makefiles.)
clean_legacy:
	rm -f *.o *.d $(LIB)
	rm -f linux/*.o linux/*.d linux/oal.a


shell:
	@echo "Entering makefile debug shell (type exit to exit) >>>"
	@bash -i
	@echo "exiting debug shell."
