

all dynamic install: conditional_build

APP = ftpd
EID_FILE = eid_bcm_bftpd.txt

#
# Set our CommEngine directory (by splitting the pwd into two words
# at /userspace and taking the first word only).
# Then include the common defines under CommEngine.
# You do not need to modify this part.
#
CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/make.common



.PHONY: check_untar_patch_configure conditional_build check_versions ftpd

ifneq ($(strip $(BUILD_FTPD)),)
# Only for ftp image upgrading.
ifeq ($(strip $(BUILD_FTPD_STORAGE)),)
ifneq ($(strip $(BUILD_BRCM_CMS))$(strip $(BUILD_BRCM_BDK)),)
conditional_build: $(APP)

else
conditional_build: sanity_check
	@echo "skipping $(APP) (not configured)"

endif
endif
endif

ftpd: generic_eid_file_install
	(tar xkfj $(APP).tar.bz2 2> /dev/null || true)
	$(MAKE) -C $(APP) $(BUILD_FTPD)


clean: generic_eid_file_clean
	-make -C $(APP) clean

