SRC := paho.mqtt.c-1.3.4
LIB_DIR := paho-mqtt3

HEADERS = MQTTAsync.h MQTTClient.h MQTTClientPersistence.h \
		MQTTProperties.h MQTTReasonCodes.h MQTTSubscribeOpts.h \
		MQTTExportDeclarations.h

check_cmake: $(SRC).tar.gz
	@if [ ! -e $(LIB_DIR)/Makefile ]; then \
	    echo "Untarring original $(SRC) source and cmake"; \
		mkdir -p $(LIB_DIR); \
		( cd $(LIB_DIR); tar --strip-components=1 -xzf ../$(SRC).tar.gz; \
		(patch -p2 < ../$(SRC).patch); \
		cmake \
		-DCMAKE_BUILD_TYPE=Release \
		-DPAHO_BUILD_SHARED=TRUE \
		-DPAHO_WITH_SSL=TRUE \
		-DOPENSSL_ROOT_DIR=$(LIB_INSTALL_DIR) \
		-DOPENSSL_INCLUDE_DIR=$(INCLUDE_DIR) \
		-DPAHO_ENABLE_TESTING=FALSE \
		-DPAHO_BUILD_SAMPLES=FALSE \
		-DCMAKE_RANLIB=$(RANLIB) \
		-DCMAKE_STRIP=$(STRIP) \
		-DCMAKE_NM:FILEPATH=$(NM) \
		-DCMAKE_OBJCOPY:FILEPATH=$(OBJCOPY) \
		-DCMAKE_OBJDUMP=$(OBJDUMP) \
		-DCMAKE_LINKER:FILEPATH=$(LD) \
		-DCMAKE_AR:FILEPATH=$(AR) \
		-DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY \
		-DCMAKE_FIND_ROOT_PATH:PATH="$(ROOT_PATH)" \
		-DCMAKE_PREFIX_PATH:PATH="$(PREFIX)" \
		-DCMAKE_INSTALL_LIBDIR=$(LIB_INSTALL_DIR) \
		-DCMAKE_INSTALL_PREFIX:PATH=$(PREFIX) ); \
	fi

install: check_cmake
	if [ ! -d $(LIB_DIR)/.libs ]; then \
		make -C $(LIB_DIR)/lib; \
	    make -C $(LIB_DIR) install; \
	fi;

clean:
	rm -f $(LIB_INSTALL_DIR)/$(LIB)*.so*
	rm -f $(addprefix $(HEADER_INSTALL_DIR)/,$(HEADERS))
	rm -rf $(LIB_DIR)
