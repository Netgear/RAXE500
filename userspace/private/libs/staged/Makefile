LIB = libstaged.so
default: dynamic


libstaged.so:
	cp libstaged.so_$(PROFILE).save libstaged.so

dynamic: generic_private_lib_install

clean:
	rm -f libstaged.so

CURR_DIR := $(shell pwd)
BUILD_DIR := $(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR := $(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common
