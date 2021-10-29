
#***********************************************************************
#
#  Copyright (c) 2007-2013  Broadcom Corporation
#  All Rights Reserved
#
#***********************************************************************/


#
# In most cases, you only need to modify this first section.
#
EXE = boot_block_housekeeping
OBJS = boot_block_housekeeping.o 

all dynamic install: conditional_build

clean: generic_clean 
	rm -f $(INSTALL_DIR)/bin/$(EXE)
	rm -f $(INSTALL_DIR)/etc/init.d/boot_block_housekeeping.sh
	rm -f $(INSTALL_DIR)/etc/rc3.d/S45boot_block_housekeeping

binaryonly_dist_clean: clean generic_binaryonly_dist_clean
	rm -f Makefile.fullsrc


#
# Set our CommEngine directory (by splitting the pwd into two words
# at /userspace and taking the first word only).
# Then include the common defines under CommEngine.
#
CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/make.common


ifneq ($(strip $(BUILD_BOOT_BLOCK_HOUSEKEEPING)),)
conditional_build: $(EXE) generic_exe_install 
	mkdir -p $(INSTALL_DIR)/etc/init.d
	mkdir -p $(INSTALL_DIR)/etc/rc3.d
	install scripts/boot_block_housekeeping.sh $(INSTALL_DIR)/etc/init.d
	(cd $(INSTALL_DIR)/etc/rc3.d; rm -f S45boot_block_housekeeping; ln -s ../init.d/boot_block_housekeeping.sh S45boot_block_housekeeping)
else
conditional_build:
	@echo "Skipping $(EXE) (not configured)"
endif


#
# Private apps and libs are allowed to include header files from the
# private and public directories.
#
# WARNING: Do not modify this section unless you understand the
# license implications of what you are doing.

ALLOWED_INCLUDE_PATHS := -I.\
                         -I$(BUILD_DIR)/userspace/public/include  \
                         -I$(BUILD_DIR)/userspace/private/include  \
                         -I$(BUILD_DIR)/userspace/public/libs/bcm_flashutil \
            		 -I$(INC_BRCMDRIVER_PUB_PATH)/$(BRCM_BOARD)  \
                         -I$(INC_BRCMDRIVER_PRIV_PATH)/$(BRCM_BOARD) \
                         -I$(INC_BRCMSHARED_PUB_PATH)/$(BRCM_BOARD)

#
# Private apps and libs are allowed to link with libraries from the
# private and public directories.
#
# WARNING: Do not modify this section unless you understand the
# license implications of what you are doing.
#
ALLOWED_LIB_DIRS := /lib:/lib/private:/lib/public

# CFLAGS += -D
CFLAGS += -Werror
CFLAGS += -DCONFIG_BCM9$(BRCM_CHIP) 

ifneq ($(strip $(HOUSEKEEPING_BOOTBLOCK_TIME_UNTIL_NEXT_CHECK)),)
CFLAGS += -DHOUSEKEEPING_BOOTBLOCK_TIME_UNTIL_NEXT_CHECK=$(HOUSEKEEPING_BOOTBLOCK_TIME_UNTIL_NEXT_CHECK)
endif
ifneq ($(strip $(HOUSEKEEPING_BOOTBLOCK_CHECK_FILE_SYNC_TIME)),)
CFLAGS += -DHOUSEKEEPING_BOOTBLOCK_CHECK_FILE_SYNC_TIME=$(HOUSEKEEPING_BOOTBLOCK_CHECK_FILE_SYNC_TIME)
endif
ifneq ($(strip $(HOUSEKEEPING_BOOTBLOCK_CHECK_BOOT_COUNTS)),)
CFLAGS += -DHOUSEKEEPING_BOOTBLOCK_CHECK_BOOT_COUNTS=$(HOUSEKEEPING_BOOTBLOCK_CHECK_BOOT_COUNTS)
endif
ifneq ($(strip $(HOUSEKEEPING_BOOTBLOCK_TIME_UNTIL_NEXT_CHECK)),)
CFLAGS += -DHOUSEKEEPING_SLEEP_DURATION=$(HOUSEKEEPING_SLEEP_DURATION)
endif


#
# Implicit rule will make the .c into a .o
# Implicit rule is $(CC) -c $(CPPFLAGS) $(CFLAGS)
# See Section 10.2 of Gnu Make manual
# 
$(EXE): $(OBJS)
	$(CC) -o $@ $(OBJS) -Wl,-rpath,$(CMS_LIB_RPATH) $(CMS_LIB_PATH) $(LIBS) -lbcm_flashutil -lbcm_boardctl -lbcm_util -lsys_util -lgen_util


#
# Include the rule for making dependency files.
# The '-' in front of the second include suppresses
# error messages when make cannot find the .d files.
# It will just regenerate them.
# See Section 4.14 of Gnu Make.
#

include $(BUILD_DIR)/make.deprules

-include $(OBJS:.o=.d)


