EXE = dhcp6s dhcp6c
dynamic: generic_exe_install

clean:
	rm -f dhcp6s dhcp6c


dhcp6s:
	cp dhcp6s_$(PROFILE).save dhcp6s

dhcp6c:
	cp dhcp6c_$(PROFILE).save dhcp6c

CURR_DIR := $(shell pwd)
BUILD_DIR := $(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR := $(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common
