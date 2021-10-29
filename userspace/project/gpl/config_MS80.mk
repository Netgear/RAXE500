# Copyright 2020  Hon Hai Precision Ind. Co. Ltd.
#  All Rights Reserved.

TARGET_PLATFORM_ARCH := arm
TARGET_PLATFORM_CROSS_COMPILE_USER := arm-buildroot-linux-gnueabi-
TOOLCHAIN_PATH_USER := /opt/toolchains/crosstools-arm-gcc-5.5-linux-4.1-glibc-2.26-binutils-2.28.1/usr/bin
TOOLCHAIN_PATH_KERN := /opt/toolchains/crosstools-arm-gcc-5.5-linux-4.1-glibc-2.26-binutils-2.28.1/usr/bin
BCMDRIVER_WL_IMPL := impl63
KERNEL_PATH := $(BUILD_DIR)/kernel/linux-4.1
LIB_OPENSSLDIR := $(BUILD_DIR)/userspace/public/libs/openssl/openssl-1.1.1b

BCA_HNDROUTER=y
CONFIG_PPTP=n
#CONFIG_BPA=y
CONFIG_RIP=n
CONFIG_NEW_WANDETECT=y
CONFIG_SINGLE_PROCESS_PPPOE=n
MPOE_ENABLE_FLAG=n
CONFIG_STATIC_PPPOE=n
CONFIG_WGET=y
IPERF_ENABLE_FLAG=n
CONFIG_CLOUD_XAGENT_CONF=y
CONFIG_NETATALK_3_1_8=n
CONFIG_CIRCLE_PARENTAL_CONTROL=n
