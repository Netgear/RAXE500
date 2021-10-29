all install default: conditional_build


CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common





ifneq ($(strip $(BRCM_BUILD_DATA_TRANSITION_TOOL)),)

conditional_build: transition_unpacker
	mkdir -p $(INSTALL_DIR)/etc/init.d
	mkdir -p $(INSTALL_DIR)/etc/rc3.d
	install -m 755 scripts/transition_unpack $(INSTALL_DIR)/etc/init.d/transition_unpack
	install -m 755 transition_unpacker $(INSTALL_DIR)/bin/
	(cd $(INSTALL_DIR)/etc/rc3.d; rm -f S26transition_unpack; ln -s ../init.d/transition_unpack S26transition_unpack)

else

conditional_build:
	

endif

transition_unpacker : transition_unpacker.c

clean:
	rm -f $(INSTALL_DIR)/etc/rc3.d/S26transition_unpack
	rm -f $(INSTALL_DIR)/etc/init.d/transition_unpack
	rm -f transition_unpacker $(INSTALL_DIR)/bin/transition_unpacker


shell:
	bash -i

