SRC := libuv-v1.38.0
LIB_DIR := uv


ifeq ($(strip $(DESKTOP_LINUX)),y)
BCM_BLT32_FLAGS = CFLAGS='$(BCM_LD_FLAGS)'
TOOLCHAIN_PREFIX=$(PROFILE_ARCH)-none-linux
endif


check_config: $(SRC).tar.gz
	@if [ ! -e $(LIB_DIR)/Makefile ]; then \
	    echo "Untarring original $(SRC) source and configure"; \
		mkdir -p $(LIB_DIR); \
		( cd $(LIB_DIR); tar --strip-components=1 -xzf ../$(SRC).tar.gz; \
		./autogen.sh ; \
		./configure --prefix=$(BCM_FSBUILD_DIR)/public --host=$(TOOLCHAIN_PREFIX) $(BCM_BLT32_FLAGS) ); \
	fi

install: check_config
	if [ ! -d $(LIB_DIR)/.libs ]; then \
		make -C $(LIB_DIR)/lib; \
	    make -C $(LIB_DIR) install; \
	fi;

clean:
	rm -f $(LIB_INSTALL_DIR)/$(LIB).*
	rm -rf $(HEADER_INSTALL_DIR)/$(LIB_DIR)
	rm -rf $(LIB_DIR)
