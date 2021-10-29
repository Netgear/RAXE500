LIB = libblogctl.so
default: dynamic


libblogctl.so:
	cp libblogctl.so_$(PROFILE).save libblogctl.so

dynamic: generic_private_lib_install

clean:
	rm -f libblogctl.so

CURR_DIR := $(shell pwd)
BUILD_DIR := $(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR := $(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common
