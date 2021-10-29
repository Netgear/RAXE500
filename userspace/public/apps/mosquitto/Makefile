

all dynamic install: conditional_build

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

unexport CROSS_COMPILE

APP = mosquitto
APP_BEEP = mosquitto_beep
BEEP_PATCH = $(APP_BEEP).patch

.PHONY: check_untar_patch_configure conditional_build check_versions

ifneq ($(strip $(BUILD_MOSQUITTO)),)
conditional_build: $(APP)
else 
ifneq ($(strip $(BUILD_BEEP_MOSQUITTO)),)
conditional_build: $(APP_BEEP)
else
conditional_build: sanity_check
	@echo "skipping $(APP) (not configured)"
endif
endif

mosquitto: sanity_check
	(tar xkfz mosquitto-1.6.2.tar.gz 2> /dev/null || true)
	$(MAKE) -C ./mosquitto-1.6.2 -j 1
	cd mosquitto-1.6.2 ; make install
	install -m 755 ./mosquitto-1.6.2/client/mosquitto_pub  $(INSTALL_DIR)/bin/mosquitto_pub
	install -m 755 ./mosquitto-1.6.2/client/mosquitto_sub  $(INSTALL_DIR)/bin/mosquitto_sub
	install -m 755 ./mosquitto-1.6.2/client/mosquitto_rr   $(INSTALL_DIR)/bin/mosquitto_rr
	install -m 755 ./mosquitto-1.6.2/src/mosquitto  $(INSTALL_DIR)/bin/mosquitto
	if [ -e ./mosquitto-1.6.2/src/mosquitto_passwd ];then install -m 755 ./mosquitto-1.6.2/src/mosquitto_passwd  $(INSTALL_DIR)/bin/mosquitto_passwd;fi;
	cp -d $(BCM_FSBUILD_DIR)/public/lib/libmosquitto* $(INSTALL_DIR)/lib$(BCM_INSTALL_SUFFIX_DIR)

check_patch_file: $(APP_BEEP).patch

mosquitto_beep: sanity_check check_patch_file
	test -s $(APP_BEEP)/Makefile || patch -s -p0 < $(BEEP_PATCH); chmod 755 $(APP_BEEP)/bin/*.sh;
	(tar xkfz mosquitto-1.6.2.tar.gz 2> /dev/null || true)
	$(MAKE) -C ./mosquitto-1.6.2 -j 1
	$(MAKE) -C ./$(APP_BEEP)

clean:
	make -C mosquitto-1.6.2 clean ; make -C mosquitto-1.6.2 uninstall
	rm -f $(INSTALL_DIR)/bin/mosquitto_pub
	rm -f $(INSTALL_DIR)/bin/mosquitto_sub
	rm -f $(INSTALL_DIR)/bin/mosquitto_rr
	rm -f $(INSTALL_DIR)/bin/mosquitto
	if [ -e ./mosquitto-1.6.2/src/mosquitto_passwd ];then rm -f  $(INSTALL_DIR)/bin/mosquitto_passwd;fi;
	rm -f $(INSTALL_DIR)/lib$(BCM_INSTALL_SUFFIX_DIR)/libmosquitto*
	if [ -f ./$(APP_BEEP)/Makefile ];then $(MAKE) -C ./$(APP_BEEP) clean;fi;
