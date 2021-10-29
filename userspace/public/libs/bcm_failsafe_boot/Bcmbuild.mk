#***********************************************************************
#
#  Copyright (c) 2019  Broadcom Corporation
#  All Rights Reserved
#
#***********************************************************************
default:all

mdir := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common
all: install  

ifneq ($(strip $(BCM_FAILSAFE_BOOT)),)
install: $(INSTALL_DIR)/etc/rc3.d/failsafe_check.sh
else
install:
endif
$(INSTALL_DIR)/etc/rc3.d/failsafe_check.sh:
	mkdir -p $(INSTALL_DIR)/etc/init.d
	mkdir -p $(INSTALL_DIR)/etc/rc3.d
	install -m 0755 -t $(INSTALL_DIR)/etc/init.d/ $(mdir)/failsafe_check.sh
	install -m 0755 -t $(INSTALL_DIR)/etc/init.d/ $(mdir)/failsafe_finish.sh
	(cd $(INSTALL_DIR)/etc/rc3.d; rm -f S1failsafe_check; ln -s ../init.d/failsafe_check.sh S1failsafe_check)
	(cd $(INSTALL_DIR)/etc/rc3.d; rm -f S89failsafe_finish; ln -s ../init.d/failsafe_finish.sh S89failsafe_finish)

clean:
ifneq ($(strip $(BCM_FAILSAFE_BOOT)),)
	rm -f $(INSTALL_DIR)/etc/rc3.d/S89failsafe_finish
	rm -f $(INSTALL_DIR)/etc/init.d/failsafe_finish.sh
	rm -f $(INSTALL_DIR)/etc/rc3.d/S1failsafe_check
	rm -f $(INSTALL_DIR)/etc/init.d/failsafe_check.sh
else
clean:
endif




