SRC := c-ares-1.16.1
LIB := libcares


ifeq ($(strip $(DESKTOP_LINUX)),y)
BCM_BLT32_FLAGS = CFLAGS='$(BCM_LD_FLAGS)'
TOOLCHAIN_PREFIX=$(PROFILE_ARCH)-none-linux
endif


check_config: $(SRC).tar.gz
	@if [ ! -e $(LIB)/Makefile ]; then \
	    echo "Untarring original $(SRC) source and configure"; \
		mkdir -p $(LIB); \
		( cd $(LIB); tar --strip-components=1 -xzf ../$(SRC).tar.gz; \
		./configure --prefix=$(BCM_FSBUILD_DIR)/public --host=$(TOOLCHAIN_PREFIX) $(BCM_BLT32_FLAGS) ); \
	fi

install: check_config
	if [ ! -d $(LIB)/.libs ]; then \
		make -C $(LIB)/lib; \
	    make -C $(LIB) install; \
	fi;

clean:
	-rm -rf $(LIB)
