/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2019 Broadcom Ltd.
 */

#include <common.h>
#include <asm/arch/cpu.h>
#include <asm/arch/ddr.h>
#include <asm/arch/ubus4.h>
#include <spl.h>
#include "bcm_otp.h"
#if defined(CONFIG_BCMBCA_PMC)
#include "pmc_drv.h"
#include "asm/arch/BPCM.h"
#include "clk_rst.h"
#endif

int arch_cpu_init(void)
{
#if defined(CONFIG_SPL_BUILD) && !defined(CONFIG_TPL_BUILD)
    u32 frq = COUNTER_FREQUENCY;

    // set arch timer frequency
    asm volatile("mcr p15, 0, %0, c14, c0, 0" : : "r" (frq));

    // enable system timer
    BIUCFG->TSO_CNTCR |= 1;

    /* enable unalgined access */    
    set_cr(get_cr() & ~CR_A);
#endif

    return 0;
}

void boost_cpu_clock(void)
{
    int stat = 0;
    uint32_t chipid;
    PLL_CTRL_REG ctrl_reg;

    chipid =  (((PERF->RevID & CHIP_ID_MASK) >> (CHIP_ID_SHIFT-CHIP_ID_LC_SIZE)) | (TOP->OtpChipidLC && CHIP_ID_LC_MASK));
    if ((chipid==0x68552c) || (chipid==0x682520) || (chipid==0x685500))
    {
        stat = ReadBPCMRegister(PMB_ADDR_BIU_PLL, PLLBPCMRegOffset(resets), &ctrl_reg.Reg32);
        ctrl_reg.Bits.byp_wait = 1;
        stat |= WriteBPCMRegister(PMB_ADDR_BIU_PLL, PLLBPCMRegOffset(resets), ctrl_reg.Reg32);

        stat |= pll_vco_config(PMB_ADDR_BIU_PLL ,60, 1);
        stat |= pll_ch_freq_set(PMB_ADDR_BIU_PLL, 0, 2);

        stat = ReadBPCMRegister(PMB_ADDR_BIU_PLL, PLLBPCMRegOffset(resets), &ctrl_reg.Reg32);
        ctrl_reg.Bits.byp_wait = 0;
        stat |= WriteBPCMRegister(PMB_ADDR_BIU_PLL, PLLBPCMRegOffset(resets), ctrl_reg.Reg32);
    }
    else if (chipid==685520)
        stat = pll_ch_freq_set(PMB_ADDR_BIU_PLL, 0, 2);

    if (stat)
        printf("Error: failed to set cpu clock\n");

    UBUS4CLK->ClockCtrl = UBUS4_CLK_BYPASS_MASK;
}

#if !defined(CONFIG_SPL_BUILD)
void print_chipinfo(void)
{
	unsigned int cpu_speed, rdp_speed, nr_cores;
	unsigned int chipId = (PERF->RevID & CHIP_ID_MASK) >> CHIP_ID_SHIFT;
	unsigned int revId = PERF->RevID & REV_ID_MASK;

    unsigned int chipIdLC = (TOP->OtpChipidLC && CHIP_ID_LC_MASK);
    if (chipIdLC)
        chipId = (chipId << CHIP_ID_LC_SIZE) | chipIdLC;

    printf("Chip ID: BCM%X_%X\n",chipId,revId);

    pll_ch_freq_get(PMB_ADDR_BIU_PLL, 0, &cpu_speed);
    if (bcm_otp_get_nr_cpus(&nr_cores))
        printf("Error: failed to read cores from OTP\n");
    else
    {
        if (nr_cores)
            printf("ARM Cortex A7 Dual Core: %dMHz", cpu_speed);
        else
            printf("ARM Cortex A7 Triple Core: %dMHz", cpu_speed);
    }

    get_rdp_freq(& rdp_speed);
    printf("RDP Freq: %dMHz\n", rdp_speed);
}
#endif

int bcmbca_get_boot_device(void)
{
	if ((MISC->miscStrapBus&MISC_STRAP_BUS_BOOT_SEL_NAND_MASK) == MISC_STRAP_BUS_BOOT_NAND)
		return BOOT_DEVICE_NAND;

	if ((MISC->miscStrapBus&MISC_STRAP_BUS_BOOT_SEL_MASK) == MISC_STRAP_BUS_BOOT_SPI_NAND)
		return BOOT_DEVICE_SPI;

	printf("Error: boot_sel straps are not set correctly\n");

	return BOOT_DEVICE_NONE;
}

#if !defined(CONFIG_TPL_ATF)
void boot_secondary_cpu(unsigned long vector)
{
    uint32_t cpu = 1;
    uint32_t nr_cpus;
    ARM_CONTROL_REG ctrl_reg;
    uint64_t rvbar = vector;
    int stat;

    printf("boot secondary cpu from 0x%lx\n", vector);

    *(volatile uint32_t*)(BOOTLUT_BASE+0x20) = vector;

    if ( bcm_otp_get_nr_cpus(&nr_cpus) )
        return;

    nr_cpus = MAX_NUM_OF_CPU-nr_cpus;

    while (cpu < nr_cpus)
    {
        BIUCFG->cluster[0].rvbar_addr[cpu] = rvbar;

        stat = PowerOnDevice(PMB_ADDR_ORION_CPU0 + cpu);
        if (stat != kPMC_NO_ERROR)
            printf("failed to power on secondary cpu %d - sts %d\n", cpu, stat);	

        stat = ReadBPCMRegister(PMB_ADDR_BIU_BPCM, ARMBPCMRegOffset(arm_control), &ctrl_reg.Reg32);
        ctrl_reg.Bits.cpu_reset_n &= ~(0x1 << cpu);
        stat |= WriteBPCMRegister(PMB_ADDR_BIU_BPCM, ARMBPCMRegOffset(arm_control), ctrl_reg.Reg32);
        if (stat != kPMC_NO_ERROR)
            printf("failed to boot secondary cpu %d - sts %d\n", cpu, stat);
        cpu++;
    }

    return;
}
#endif

