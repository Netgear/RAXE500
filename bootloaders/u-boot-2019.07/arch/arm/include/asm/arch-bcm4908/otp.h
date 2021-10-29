/* SPDX-License-Identifier: GPL-2.0+
 *
 *  Copyright 2020 Broadcom Ltd.
 */

#ifndef _4908_OTP_H
#define _4908_OTP_H

#define JTAG_OTP_BASE    0xff800e00

/* row 14 */
#define OTP_CPU_CORE_CFG_ROW			14
#define OTP_CPU_CORE_CFG_SHIFT			14
#define OTP_CPU_CORE_CFG_MASK			0x3

#define OTP_SGMII_DISABLE_ROW			14
#define OTP_SGMII_DISABLE_SHIFT			17
#define OTP_SGMII_DISABLE_MASK			0x1

/* row 17 */
#define OTP_BRCM_BTRM_BOOT_ENABLE_ROW           17
#define OTP_BRCM_BTRM_BOOT_ENABLE_SHIFT         3
#define OTP_BRCM_BTRM_BOOT_ENABLE_MASK          0x1

/* row 18 */
#define OTP_CUST_BTRM_BOOT_ENABLE_ROW           18
#define OTP_CUST_BTRM_BOOT_ENABLE_SHIFT         15
#define OTP_CUST_BTRM_BOOT_ENABLE_MASK          0x7

/* row 24 */
#define OTP_CUST_MFG_MRKTID_ROW                 24
#define OTP_CUST_MFG_MRKTID_SHIFT               0
#define OTP_CUST_MFG_MRKTID_MASK                0xffff

/* A row initializer that maps actual row number with mask and shift to a feature name;
 * this allows to use features vs. rows for common functionality, 
 * such as secure boot handling frequency, chipid and so on 
 * prevent ifdef dependencies when used outside of arch directories for common among SoCs logic
 * */
#define	DEFINE_OTP_MAP_ROW_INITLR(__VV__)									\
	static otp_hw_cmn_row_t __VV__[ ] = {									\
	{OTP_MAP_BRCM_BTRM_BOOT_ENABLE, OTP_BRCM_BTRM_BOOT_ENABLE_ROW, OTP_BRCM_BTRM_BOOT_ENABLE_MASK, OTP_BRCM_BTRM_BOOT_ENABLE_SHIFT, 1},\
	{OTP_MAP_CUST_BTRM_BOOT_ENABLE, OTP_CUST_BTRM_BOOT_ENABLE_ROW, OTP_CUST_BTRM_BOOT_ENABLE_MASK, OTP_CUST_BTRM_BOOT_ENABLE_SHIFT, 1},\
	{OTP_MAP_CUST_MFG_MRKTID, OTP_CUST_MFG_MRKTID_ROW, OTP_CUST_MFG_MRKTID_MASK, OTP_CUST_MFG_MRKTID_SHIFT, 1},				\
	{OTP_MAP_CPU_CORE_CFG, OTP_CPU_CORE_CFG_ROW, OTP_CPU_CORE_CFG_MASK, OTP_CPU_CORE_CFG_SHIFT, 1},				\
	{OTP_MAP_SGMII_DISABLE, OTP_SGMII_DISABLE_ROW, OTP_SGMII_DISABLE_MASK, OTP_SGMII_DISABLE_SHIFT, 1},				\
	}

#endif