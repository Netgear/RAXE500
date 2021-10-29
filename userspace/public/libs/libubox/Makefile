#***********************************************************************
#
#  Copyright (c) 2019  Broadcom Limited
#  All Rights Reserved
#
#***********************************************************************/

LIB = libubox-2020-02-27-7da66430
CMAKE_BUILD_DIR = ubox-build

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

clean:
	-[ ! -e $(LIB) ] || $(MAKE) -C $(CMAKE_BUILD_DIR) clean;
	rm -rf $(INSTALL_DIR)/lib/libubox*
	rm -rf $(INSTALL_DIR)/lib/libblobmsg_json*
	rm -rf $(INSTALL_DIR)/lib/libjson_script.so
	rm -rf $(BCM_FSBUILD_DIR)/public/lib/libubox*
	rm -rf $(BCM_FSBUILD_DIR)/public/lib/libblobmsg_json*
	rm -rf $(BCM_FSBUILD_DIR)/public/lib/libjson_script.so
	rm -rf $(CURR_DIR)/$(CMAKE_BUILD_DIR)
	rm -rf $(LIB)


# The next line is a hint to to our release scripts
# GLOBAL_RELEASE_SCRIPT_CALL_DISTCLEAN
distclean: clean

include $(BUILD_DIR)/make.common

ifeq ($(strip $(DESKTOP_LINUX)),y)
BCM_BLT32_FLAGS := $(BCM_LD_FLAGS)
endif

ifeq ($(strip $(BUILD_LIBJSONC)),y)
LDFLAGS+=-L$(INSTALL_DIR)/lib -ljson-c
endif

.PHONY: build check_untar_cmake sanity_check conditional_build

ifneq ($(strip $(BUILD_LIBUBOX)),)
conditional_build: build
else
conditional_build:
	@echo "skipping $(LIB) (not configured)"
endif

check_untar_cmake: sanity_check
	@echo $(TOOLCHAIN_PREFIX)
	echo "Untarring original $(LIB) source and configure";
	(tar xfJ $(LIB).tar.xz 2> /dev/null || true);
	sed -i '1i\ADD_DEFINITIONS(-DJSONC)' $(LIB)/CMakeLists.txt
	mkdir -p $(CURR_DIR)/$(CMAKE_BUILD_DIR)
	export PKG_CONFIG_LIBDIR=$(BCM_FSBUILD_DIR)/public/lib
	export PKG_CONFIG_PATH=$(BCM_FSBUILD_DIR)/public/lib/pkgconfig
	cd $(CURR_DIR)/$(CMAKE_BUILD_DIR); \
	CFLAGS="-I$(BCM_FSBUILD_DIR)/public/include -I$(BCM_FSBUILD_DIR)/public/include/json-c" \
	cmake -DBUILD_LUA=off \
	-DCMAKE_RANLIB=$(RANLIB) \
	-DCMAKE_STRIP=$(STRIP) \
	-DCMAKE_NM:FILEPATH=$(NM) \
	-DCMAKE_OBJCOPY:FILEPATH=$(OBJCOPY) \
	-DCMAKE_OBJDUMP=$(OBJDUMP) \
	-DCMAKE_LINKER:FILEPATH=$(LD) \
	-DCMAKE_AR:FILEPATH=$(AR) \
	-DCMAKE_FIND_ROOT_PATH:PATH="$(BCM_FSBUILD_DIR);$(BCM_FSBUILD_DIR)/public" \
	-DCMAKE_PREFIX_PATH:PATH="$(BCM_FSBUILD_DIR)/public;$(BCM_FSBUILD_DIR)/public/include;$(BCM_FSBUILD_DIR)/public/lib" \
	-DCMAKE_LIBRARY_PATH:PATH="$(BCM_FSBUILD_DIR)/public/lib" \
	-DCMAKE_INSTALL_PREFIX:PATH=$(BCM_FSBUILD_DIR)/public \
	$(CURR_DIR)/$(LIB)


build: check_untar_cmake
	@echo "Building $(LIB)"
	cd $(CURR_DIR)/$(CMAKE_BUILD_DIR); $(MAKE) 
	cd $(CURR_DIR)/$(CMAKE_BUILD_DIR); $(MAKE) install
	#libblobmsg_json.a libblobmsg_json.so libjson_script.so libubox.a libubox.so
	chmod 755 $(BCM_FSBUILD_DIR)/public/lib/libubox.*;
	chmod 755 $(BCM_FSBUILD_DIR)/public/lib/libblobmsg_json.*;
	chmod 755 $(BCM_FSBUILD_DIR)/public/lib/libjson_script.*;
	cp -d $(BCM_FSBUILD_DIR)/public/lib/libubox.* $(INSTALL_DIR)/lib/
	cp -d $(BCM_FSBUILD_DIR)/public/lib/libblobmsg_json.* $(INSTALL_DIR)/lib/
	cp -d $(BCM_FSBUILD_DIR)/public/lib/libjson_script.so $(INSTALL_DIR)/lib/

