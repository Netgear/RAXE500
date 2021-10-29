LIB = libwlmdm.so
default: dynamic


libwlmdm.so:
	cp libwlmdm.so_$(PROFILE).save libwlmdm.so

dynamic: generic_private_lib_install

clean:
	rm -f libwlmdm.so

CURR_DIR := $(shell pwd)
BUILD_DIR := $(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR := $(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common
