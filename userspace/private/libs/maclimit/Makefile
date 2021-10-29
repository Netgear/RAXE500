LIB = libmaclimit.so
default: dynamic


libmaclimit.so:
	cp libmaclimit.so_$(PROFILE).save libmaclimit.so

dynamic: generic_private_lib_install

clean:
	rm -f libmaclimit.so

CURR_DIR := $(shell pwd)
BUILD_DIR := $(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR := $(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common
