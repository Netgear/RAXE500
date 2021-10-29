/*  *********************************************************************
    *  Broadcom Common Firmware Environment (CFE)
    *
    *  Board device initialization      File: bcm94710_devs.c
    *
    *  This is the "C" part of the board support package.  The
    *  routines to create and initialize the console, wire up
    *  device drivers, and do other customization live here.
    *
    *  Author:  Mitch Lichtenberg (mpl@broadcom.com)
    *
    *********************************************************************
    *
    *  XX Copyright 2000,2001
    *  Broadcom Corporation. All rights reserved.
    *
    *  BROADCOM PROPRIETARY AND CONFIDENTIAL
    *
    *  This software is furnished under license and may be used and
    *  copied only in accordance with the license.
    ********************************************************************* */
#include "lib_types.h"
#include "lib_printf.h"
#include "cfe_timer.h"
#include "cfe.h"
#include "bcm_map.h"
#include "bcm_hwdefs.h"
#include "bcmTag.h"
#include "dev_bcm63xx_flash.h"
#include "bcm63xx_util.h"
#include "flash_api.h"
#include "exception.h"
#include "shared_utils.h"
#include "btrm_if.h"
#include "bcm_otp.h"
#include "bcm63xx_dtb.h"
#include "bcm63xx_ipc.h"
#include "bcm63xx_common.h"

#if INC_EMMC_FLASH_DRIVER
#include "dev_emmcflash.h"
#endif
#if (INC_PMC_DRIVER==1)
#include "pmc_drv.h"
#include "BPCM.h"
#endif

#if !defined(_BCM960333_) && !defined(_BCM963268_) && !defined(_BCM96838_) && !defined(_BCM947189_)
#if !defined (_BCM96848_)
#include "bcm_misc_hw_init.h"
#endif
#include "bcm_gpio.h"
#include "bcm_pinmux.h"
#endif

#if !defined(_BCM963268_) && !defined(_BCM96838_)
#include "bcm_led.h"
#endif

#if defined (_BCM947189_)
#include "bcm_gpio.h"
#include "bcm_misc_hw_init.h"
#endif

#if defined (_BCM96848_)
#include "phys_common_drv.h"
#endif

/* Foxconn add, Laider Lai */
#include "bcm63xx_nvram.h"
#include "bcm_gpio.h"

#define GPIO_PWR_LED   26
#define GPIO_PWR_D_LED 27
#define GPIO_RST_BTN   21

void power_led_off(void);
void diagnostic_led_on(void);
/* Foxconn */

#if (INC_SPI_PROG_NAND==0)
static int checkForResetToDefaultHold( unsigned short rstToDfltIrq );
#endif

/* Foxconn added start , 1/29/2010 */
int verify_checksum(char *buf, unsigned long buf_len, unsigned long chksum);
/* Foxconn added end , 1/29/2010 */

void board_bootdevice_init(void); /* Foxconn modified */
/*  *********************************************************************
    *  Devices we're importing
    ********************************************************************* */

#if defined (_BCM96838_) || defined (_BCM963138_) || defined (_BCM963148_) || defined(_BCM96848_) || \
    defined (_BCM96858_) || defined (_BCM96846_)  || defined(_BCM96856_)
extern cfe_driver_t bcm63xx_uart;
#if (NONETWORK==0)
#if defined(_BCM96858_) || defined(_BCM96846_) || defined(_BCM96856_)
extern cfe_driver_t bcm6xxx_impl3_enet;
#else
extern cfe_driver_t bcm6xxx_impl2_enet;
#endif
#endif
#elif defined (_BCM96878_)
#if (NONETWORK==0)
extern cfe_driver_t bcm6xxx_impl3_enet;
#endif
#elif defined (_BCM947189_)
extern cfe_driver_t bcm63xx_uart;
#if (NONETWORK==0)
extern cfe_driver_t bcm47189_enet;
#endif
#else
extern cfe_driver_t bcm63xx_uart;
extern cfe_driver_t bcm63xx_enet;
#endif

/* add the second uart driver 63158 B0 */
#if defined (_BCM963158_) || defined(_BCM963178_) || defined(_BCM947622_) || defined(_BCM96878_)
extern cfe_driver_t pl011_uart;
#endif


#if defined(_BCM963381_)
extern void _cfe_flushcache(int, uint8_t *, uint8_t *);
void second_cpu_icache_fixup(void);

void second_cpu_icache_fixup(void)
{
    _cfe_flushcache(CFE_CACHE_INVAL_I,0,0);

    /* put the TP1 in sleep again for linux to bring up TP1 */
    asm(
"1: \n"
    "sync \n"
    "wait \n"
    "b  1b \n"
    "nop \n"
    ::);

    return;
}
#endif

/*  *********************************************************************
    *  board_console_init()
    *
    *  Add the console device and set it to be the primary
    *  console.
    *
    *  Input parameters:
    *      nothing
    *
    *  Return value:
    *      nothing
    ********************************************************************* */

void board_console_init(void)
{
    /* Add the serial port driver. */
#if defined (_BCM960333_)
    /* Minimum GPIO_MUX setup to enable GPIOs for UART
     * Map GPIOs 4 & 5 according to GPIO_MUX (default 4: UARTrx, 5: UARTtx)
     */
    GPIO->GPIOFuncMode |= 0x30;
#endif

#if defined (_BCM963158_)
    if( UtilGetChipRev() == 0xA0 )
        cfe_add_device(&bcm63xx_uart,0,0,0);
    else
        cfe_add_device(&pl011_uart,0,0,0);
#elif defined (_BCM963178_)  || defined(_BCM947622_) || defined(_BCM96878_)
        cfe_add_device(&pl011_uart,0,0,0);
#else
    cfe_add_device(&bcm63xx_uart,0,0,0);
#endif
    cfe_set_console( "uart0" );
}

#if defined(_BCM963138_) && !defined(CONFIG_BRCM_IKOS)
static void bump_nand_phase(int n) {
    int i;
    for (i = 0 ; i < n ; i++) {
        PROCMON->PMBM[1].wr_data = 0x081e000a;
        PROCMON->PMBM[1].ctrl = 0x8010a00d;
        while (PROCMON->PMBM[1].ctrl & 0x10000000) { };
        PROCMON->PMBM[1].wr_data = 0x001e000a;
        PROCMON->PMBM[1].ctrl = 0x8010a00d;
        while (PROCMON->PMBM[1].ctrl & 0x10000000) { };
    }
    printf(".");
}

static int check_nand_phase(int n) {
    unsigned long i;
    for (i = 0; (i >> n) < 10000 ; i++) {
        if (((NAND->NandRevision & ~0xff) != 0x80000700) 
           ||  ((NAND->NandCmdStart & ~0xff) == 0x80000700)) {
            printf("p%d.\n",i);
            return(1);
        }
    }
    return(0);
}
#endif

#if defined (CCI400_BASE)
/*this function is used to turn on CCI from secure mode 
  it also turns snooping enable for S3 (XRDP)*/
static void cci400_enable(void)
{
    uint32_t *cci_secure = (uint32_t*)(CCI400_PHYS_BASE + CCI400_SECURE_ACCESS_REG_OFFSET);
    uint32_t *cci_s3_snoop = (uint32_t*)(CCI400_PHYS_BASE + CCI400_S3_SNOOP_CTRL_REG_OFFSET);

    /*Enable access from E2 and below */
    *cci_secure = CCI400_SECURE_ACCESS_UNSECURE_ENABLE;

    /*config XRDP for CCI */
    *cci_s3_snoop |= CCI400_S3_SNOOP_CTRL_EN_SNOOP;
}

#endif

#if defined (CCI500_BASE)
/*this function is used to turn on CCI from secure mode 
  it also turns snooping enable for S5 CPU interface*/
static void cci500_enable(void)
{
    /*Enable access from E2 and below */
    CCI500->secr_acc |= SECURE_ACCESS_UNSECURE_ENABLE;
#if defined (_BCM963158_)
    /*disable snoop filter for 63158 A0 silicon issue*/    
    if( UtilGetChipRev() == 0xA0 )
        CCI500->ctrl_ovr |= CONTROL_OVERRIDE_SNOOP_FLT_DISABLE;
#endif

    /*enable snoop ini the cpu interface. This is done in the kernel early init so we can
      have coherent enable/disable through kernel configuration */
    /* CCI500->si[SLAVEINTF_A53_CLUSTER].snoop_ctrl |= SNOOP_CTRL_ENABLE_SNOOP; */

#if 0
    /* force share in s0 coherency port */
    CCI500->si[SLAVEINTF_COHERENCY_PORT].share_ovr &= ~SHARE_OVR_SHAREABLE_OVR_MASK;
    CCI500->si[SLAVEINTF_COHERENCY_PORT].share_ovr |= SHARE_OVR_SHAREABLE_OVR_SHR;

    /*turn on debug */
    CCI500->debug_ctrl |= DBG_CTRL_EN_INTF_MON;
    printf("CCI ctr_ovr 0x%p->0x%x,  s1 snoop_ctrl 0x%p->0x%x s0 share_ovr 0x%p->0x%x\n", 
      &CCI500->ctrl_ovr, CCI500->ctrl_ovr, &CCI500->si[SLAVEINTF_A53_CLUSTER].snoop_ctrl,
      CCI500->si[SLAVEINTF_A53_CLUSTER].snoop_ctrl, &CCI500->si[SLAVEINTF_COHERENCY_PORT].share_ovr, CCI500->si[SLAVEINTF_COHERENCY_PORT].share_ovr);
#endif
}
#endif

/*  *********************************************************************
    *  cfe_enable_ns_access()
    *
    *  Enable non-secure access to register blocks in case it has not
    *  been enabled in the CFEROM for some platforms. 
    *
    *  Input parameters:
    *      nothing
    *
    *  Return value:
    *      nothing
    ********************************************************************* */

static void cfe_enable_ns_access(void)
{
#if defined _BCM963158_ || defined _BCM947622_ || defined _BCM963178_
    BIUCFG->bac.bac_permission |= 0x33; // Linux access to BAC_CPU_THERM_TEMP
#endif
}

/* RGB LED control */
#define AL_AH_REG		0xFF803014
#define SW_INPUT_REG		0xFF803010  

#define LED_R1_REG		0xff803090	/* GPIO 7 */
#define LED_G1_REG		0xff803050	/* GPIO 3 */
#define LED_B1_REG		0xff8030b0	/* GPIO 9 */
#define LED_W_REG		0xff8031e0	/* GPIO 28 */

#define LED_Bit_MASK		0x10000288	/* GPIO 3, 7, 9 actvie */
#define LED_N_Bit_MASK		0xEFFFFD77	/* Negative Bit Mask, for set SW Ctrl Reg, SW LED act-Low */
#define LED_ACT_CFG		0xFF80301c	/* actvie register */
/* amber */
/* configure the color which you need by modify LED_x_BRIGHTNESS, 0x00~0xFF */
#define	LED_R_BRIGHTNESS	0x80
#define	LED_G_BRIGHTNESS	0x10
#define	LED_B_BRIGHTNESS	0x00

typedef enum led_mode	// bit 0-1
{
    STATE = 0,
    FADE = 1,
    PULSATE = 2
}LED_MODE;

typedef enum led_color 
{
    _LED_OFF = 0,
    _LED_RED = 1,
    _LED_GREEN,
    _LED_BLUE,
    _LED_WHITE,
    _LED_AMBER
}LED_COLOR;

typedef enum led_blink_freq	// bit 3-5
{
    BL_000 = 0,	// 0
    BL_001 = 1,	// 25		Hz
    BL_010,	// 12.5		Hz
    BL_011,	// 6.25 	Hz
    BL_100,	// 3.125	Hz
    BL_101,	// 1.5625	Hz
    BL_110,	// 0.78125	Hz
    BL_111 = 7, // 0.390625	Hz
}LED_BLINK_FREQ;

typedef struct led_color_cfg
{
    LED_COLOR index;
    uint32_t r_bright;
    uint32_t g_bright;
    uint32_t b_bright;
} LED_COLOR_CFG;

LED_COLOR_CFG color_table[] = 
{
    {_LED_OFF, 0, 0, 0},
    {_LED_RED, 0x80, 0, 0},
    {_LED_GREEN, 0, 0x80, 0},
    {_LED_BLUE, 0, 0, 0x80},
    {_LED_WHITE, 0x80, 0x80, 0x80},
    {_LED_AMBER, 0x80, 0x0f, 0}
};

void set_LED_cfg(LED_COLOR color, LED_MODE mode, LED_BLINK_FREQ freq, int RGB_LED, int repeat)
{
    volatile uint32_t *cled_al_ah_reg = (void *)(AL_AH_REG);
    volatile uint32_t *cled_input_reg = (void *)(SW_INPUT_REG);
    volatile uint32_t *cled_led_r1_reg = (void *)(LED_R1_REG);
    volatile uint32_t *cled_led_g1_reg = (void *)(LED_G1_REG);
    volatile uint32_t *cled_led_b1_reg = (void *)(LED_B1_REG);
    volatile uint32_t *cled_led_w_reg = (void *)(LED_W_REG);
    volatile uint32_t *cled_act_cfg = (void *)(LED_ACT_CFG);
    uint32_t val32;
    int i =0, color_table_size =0;//, found_color = 0;

    /* set LED_R(7), LED_G(3), LED_B(9), LED_W(28) as active low */
    val32 = *cled_al_ah_reg;
    val32 &= LED_N_Bit_MASK;
    *cled_al_ah_reg = val32;

    val32 = *cled_input_reg;
    val32 &= LED_N_Bit_MASK;
    *cled_input_reg = val32;

    if (color == _LED_OFF) {
        *cled_led_r1_reg = 0;
        *cled_led_g1_reg = 0;
        *cled_led_b1_reg = 0;
        *cled_led_w_reg = 0;
        
    }
    else if (!RGB_LED && color == _LED_WHITE) {
        *cled_led_w_reg = ((0x80 << 6) | (freq << 3) | mode);
    }
    else if (RGB_LED && mode == STATE) {
        color_table_size = sizeof(color_table)/sizeof(LED_COLOR_CFG);
        for (i=0; i<color_table_size; i++) {
            if (color == color_table[i].index) {
                *cled_led_r1_reg = ((repeat<<14) | (color_table[i].r_bright << 6) | 
                                   (freq << 3) | mode);
                *cled_led_g1_reg = ((repeat<<14) | (color_table[i].g_bright << 6) | 
                                   (freq << 3) | mode);
                *cled_led_b1_reg = ((repeat<<14) | (color_table[i].b_bright << 6) | 
                                   (freq << 3) | mode);
                //found_color =1;
printf("[%s][%d]R: %X, G: %X, B: %X\n", __func__, __LINE__, *cled_led_r1_reg, *cled_led_g1_reg, *cled_led_b1_reg);
                break;
            }
        }
    }
    else {
        printf("Unknown LED setting.\n");
        return;
    }
    /* active */
    *cled_act_cfg = LED_Bit_MASK;
}

/*  *********************************************************************
    *  board_device_init()
    *
    *  Initialize and add other devices.  Add everything you need
    *  for bootstrap here, like disk drives, flash memory, UARTs, etc
    *  BUT NOT network controllers.
    *
    *  Input parameters:
    *      nothing
    *
    *  Return value:
    *      nothing
    ********************************************************************* */

void board_device_init(void)
{
#if defined (_BCM963268_)
    unsigned int GPIOOverlays;
    const ETHERNET_MAC_INFO *EnetInfo;
    unsigned int DeviceOptions = 0;
#endif
#if defined (_BCM96838_)
    unsigned short MuxSel = 0;
    char    boardIdStr[BP_BOARD_ID_LEN];
#endif

#if defined(_BCM963138_) && !defined(CONFIG_BRCM_IKOS)
    int i = 0;
    if( UtilGetChipRev() == 0xa0 ) {
        printf("CHECKING NAND Phase\n");
        while (!check_nand_phase(0)) {
            bump_nand_phase(1);
            printf("B0");
            if (i++ > 60) {
                break;
            }
        }
        if (i <= 60) {
           while (check_nand_phase(4)) {
               bump_nand_phase(3);
               printf("B1");
           }
           bump_nand_phase(6);
           printf("B2");
        }
        printf("\n");
    }
#endif

#if !defined(_BCM960333_) && !defined(_BCM963268_) && !defined(_BCM96838_) && !defined(_BCM96848_) && !defined(_BCM947189_)
#if defined(_BCM963138_) || (INC_SPI_PROG_NAND==1)
    /* assume all the 63138 board does use nand pin for other function */ 
    bcm_init_pinmux_interface(BP_PINMUX_FNTYPE_NAND);    
#endif
#endif

#if (\
     (defined(_BCM94908_) || defined(_BCM96858_) || defined(_BCM93158_) || \
      defined(_BCM96846_) || defined(_BCM96856_) || defined(_BCM947622_)) && \
     INC_EMMC_FLASH_DRIVER )
    bcm_init_pinmux_interface(BP_PINMUX_FNTYPE_EMMC);    
#endif

    board_bootdevice_init();

#if defined (_BCM963268_)
    if( BpGetDeviceOptions(&DeviceOptions) == BP_SUCCESS ) {
        if(DeviceOptions&BP_DEVICE_OPTION_DISABLE_LED_INVERSION)
            MISC->miscLed_inv = 0;
    }

    /* If previously disabled, Robosw clock needs to be manually re-enabled on soft reboot */
    PERF->blkEnables |= ROBOSW250_CLK_EN;

    /* Turn off test bus */
    PERF->blkEnables &= ~TBUS_CLK_EN;

    /* Start with all HW LEDs disabled */
    LED->ledHWDis |= 0xFFFFFF;
    LED->ledMode = 0;

    EnetInfo = BpGetEthernetMacInfoArrayPtr();

    /* Enable HW to drive LEDs for Ethernet ports in use */
    if (EnetInfo[0].sw.port_map & (1 << 0)) {
        LED->ledHWDis &= ~(1 << LED_EPHY0_ACT);
        LED->ledHWDis &= ~(1 << LED_EPHY0_SPD);
    }
    if (EnetInfo[0].sw.port_map & (1 << 1)) {
        LED->ledHWDis &= ~(1 << LED_EPHY1_ACT);
        LED->ledHWDis &= ~(1 << LED_EPHY1_SPD);
    }
    if (EnetInfo[0].sw.port_map & (1 << 2)) {
        LED->ledHWDis &= ~(1 << LED_EPHY2_ACT);
        LED->ledHWDis &= ~(1 << LED_EPHY2_SPD);
    }
    if (EnetInfo[0].sw.port_map & (1 << 3)) {
        LED->ledHWDis &= ~(1 << LED_GPHY0_ACT);
        LED->ledHWDis &= ~(1 << LED_GPHY0_SPD0);
        LED->ledHWDis &= ~(1 << LED_GPHY0_SPD1);
        LED->ledLinkActSelLow |= ((1 << LED_GPHY0_SPD0) << LED_0_LINK_SHIFT);
        LED->ledLinkActSelLow |= ((1 << LED_GPHY0_SPD1) << LED_1_LINK_SHIFT);
        GPIO->RoboSWLEDControl |= LED_BICOLOR_SPD;
    }

    if( BpGetGPIOverlays(&GPIOOverlays) == BP_SUCCESS ) {
        if (GPIOOverlays & BP_OVERLAY_SERIAL_LEDS) {
            GPIO->GPIOMode |= (GPIO_MODE_SERIAL_LED_CLK | GPIO_MODE_SERIAL_LED_DATA);
            LED->ledInit |= LED_SERIAL_LED_EN;
        }
        /* Enable LED controller to drive GPIO when LEDs are connected to GPIO pins */
        if (GPIOOverlays & BP_OVERLAY_EPHY_LED_0) {
            GPIO->LEDCtrl |= (1 << LED_EPHY0_ACT);
            GPIO->LEDCtrl |= (1 << LED_EPHY0_SPD);
        }
        if (GPIOOverlays & BP_OVERLAY_EPHY_LED_1) {
            GPIO->LEDCtrl |= (1 << LED_EPHY1_ACT);
            GPIO->LEDCtrl |= (1 << LED_EPHY1_SPD);
        }
        if (GPIOOverlays & BP_OVERLAY_EPHY_LED_2) {
            GPIO->LEDCtrl |= (1 << LED_EPHY2_ACT);
            GPIO->LEDCtrl |= (1 << LED_EPHY2_SPD);
        }
        if (GPIOOverlays & BP_OVERLAY_GPHY_LED_0) {
            GPIO->LEDCtrl |= (1 << LED_GPHY0_ACT);
            GPIO->LEDCtrl |= (1 << LED_GPHY0_SPD0);
            GPIO->LEDCtrl |= (1 << LED_GPHY0_SPD1);
        }
        /* Enable HS SPI SS Pins */
        if (GPIOOverlays & BP_OVERLAY_HS_SPI_SSB4_EXT_CS) {
             GPIO->GPIOMode |= GPIO_MODE_HS_SPI_SS_4;
        }
        if (GPIOOverlays & BP_OVERLAY_HS_SPI_SSB5_EXT_CS) {
             GPIO->GPIOMode |= GPIO_MODE_HS_SPI_SS_5;
        }
        if (GPIOOverlays & BP_OVERLAY_HS_SPI_SSB6_EXT_CS) {
             GPIO->GPIOMode |= GPIO_MODE_HS_SPI_SS_6;
        }
        if (GPIOOverlays & BP_OVERLAY_HS_SPI_SSB7_EXT_CS) {
             GPIO->GPIOMode |= GPIO_MODE_HS_SPI_SS_7;
        }
    }

    {
        unsigned short PhyBaseAddr;
        /* clear the base address first. hw does not clear upon soft reset*/
        GPIO->RoboswEphyCtrl &= ~EPHY_PHYAD_BASE_ADDR_MASK;
        if( BpGetEphyBaseAddress(&PhyBaseAddr) == BP_SUCCESS ) {
            GPIO->RoboswEphyCtrl |= ((PhyBaseAddr >>3) & 0x3) << EPHY_PHYAD_BASE_ADDR_SHIFT;
        }

        /* clear the base address first. hw does not clear upon soft reset*/
        GPIO->RoboswGphyCtrl &= ~GPHY_PHYAD_BASE_ADDR_MASK;
        if( BpGetGphyBaseAddress(&PhyBaseAddr) == BP_SUCCESS ) {
            GPIO->RoboswGphyCtrl |= ((PhyBaseAddr >>3) & 0x3) << GPHY_PHYAD_BASE_ADDR_SHIFT;
        }
    }
#endif

#if defined (_BCM96838_)

    BpGetBoardId(boardIdStr);
    /* FIXME board dependency should be removed */
    if ((0 == strcmp(boardIdStr, "968380FTTDPS")) || (0 == strcmp(boardIdStr, "968380DP2")))
    {
        set_pinmux(0,5);
        set_pinmux(1,5);
        set_pinmux(2,5);
        set_pinmux(3,5);
        set_pinmux(4,5);
        set_pinmux(5,5);
        set_pinmux(6,5);
        set_pinmux(7,5);
        set_pinmux(8,5);
        set_pinmux(9,5);
        set_pinmux(12,5);
        set_pinmux(68,5);
        set_pinmux(69,5);
    }

    /* FIXME board dependency should be removed */
    if (0 == strcmp(boardIdStr, "968380DP2"))
    {
        set_pinmux(67,5);
    }

    /* FIXME board dependency should be removed */
    if ((0 == strcmp(boardIdStr, "965200F_CO")) ||
        (0 == strcmp(boardIdStr, "965200F_CPE")))
    {
        set_pinmux(0, 5);
        set_pinmux(1, 5);
        set_pinmux(8, 5);
        set_pinmux(10, 5);
        set_pinmux(11, 5);
        set_pinmux(12, 5);
        set_pinmux(13, 5);
        set_pinmux(14, 5);
        set_pinmux(15, 5);
        set_pinmux(67, 5);
    }

    /* set slic muxing */
    if( BpGetSlicInterfaces((unsigned short*)&MuxSel) == BP_SUCCESS )
    {
        if (MuxSel == BP_SLIC_GROUPC)
        {
            set_pinmux(PINMUX_CSLIC_CLK_PIN, PINMUX_SLICC_FUNC);
            set_pinmux(PINMUX_CSLIC_STDOUT_PIN, PINMUX_SLICC_FUNC);
            set_pinmux(PINMUX_CSLIC_STDIN_PIN, PINMUX_SLICC_FUNC);
            set_pinmux(PINMUX_CSLIC_ENABLE_PIN, PINMUX_SLICC_FUNC);
        }
        else if (MuxSel == BP_SLIC_GROUPD)
        {
            set_pinmux(PINMUX_DSLIC_CLK_PIN, PINMUX_SLICD_FUNC);
            set_pinmux(PINMUX_DSLIC_STDOUT_PIN, PINMUX_SLICD_FUNC);
            set_pinmux(PINMUX_DSLIC_STDIN_PIN, PINMUX_SLICD_FUNC);
            set_pinmux(PINMUX_DSLIC_ENABLE_PIN, PINMUX_SLICD_FUNC);
        }
    }

    /* set phy leds pinmuxing - FIXME board dependency should be removed*/
    if (strcmp(boardIdStr, "968380FTTDPS") &&
        strcmp(boardIdStr, "968380GWAN") &&
        strcmp(boardIdStr, "965200DPF"))
    {
        set_pinmux(PINMUX_EGPHY0_LED_PIN,PINMUX_EGPHY0_LED_FUNC);
        set_pinmux(PINMUX_EGPHY1_LED_PIN,PINMUX_EGPHY1_LED_FUNC);
        set_pinmux(PINMUX_EGPHY2_LED_PIN,PINMUX_EGPHY2_LED_FUNC);
        set_pinmux(PINMUX_EGPHY3_LED_PIN,PINMUX_EGPHY3_LED_FUNC);
    }

    /* Set simcard pinmuxing */
    if( BpGetSimInterfaces((unsigned short*)&MuxSel) == BP_SUCCESS ) 
    {   
        set_pinmux(PINMUX_SIM_DAT, PINMUX_SIM_FUNC);
        set_pinmux(PINMUX_SIM_CLK, PINMUX_SIM_FUNC);
        set_pinmux(PINMUX_SIM_PRESENCE, PINMUX_SIM_FUNC);

        if (MuxSel == BP_SIMCARD_GROUPA)
        {
            set_pinmux(PINMUX_SIM_A_VCC_EN_PIN, PINMUX_SIM_FUNC);
            set_pinmux(PINMUX_SIM_A_VCC_VOL_SEL_PIN, PINMUX_SIM_FUNC);
            set_pinmux(PINMUX_SIM_A_VCC_RST_PIN, PINMUX_SIM_FUNC);
            set_pinmux(PINMUX_SIM_A_VPP_EN_PIN, PINMUX_SIM_FUNC);
        }
        else if (MuxSel == BP_SIMCARD_GROUPA_OD)
        {
            set_pinmux(PINMUX_SIM_A_VCC_EN_PIN, PINMUX_GPIO_FUNC);
            set_pinmux(PINMUX_SIM_A_VCC_VOL_SEL_PIN, PINMUX_SIM_FUNC);
            set_pinmux(PINMUX_SIM_A_VCC_RST_PIN, PINMUX_GPIO_FUNC);
            set_pinmux(PINMUX_SIM_A_VPP_EN_PIN, PINMUX_SIM_FUNC);
        }
        else if (MuxSel == BP_SIMCARD_GROUPB)
        {
            set_pinmux(PINMUX_SIM_B_VCC_EN_PIN, PINMUX_SIM_FUNC);
            set_pinmux(PINMUX_SIM_B_VCC_VOL_SEL_PIN, PINMUX_SIM_FUNC);
            set_pinmux(PINMUX_SIM_B_VCC_RST_PIN, PINMUX_SIM_FUNC);
            set_pinmux(PINMUX_SIM_B_VPP_EN_PIN, PINMUX_SIM_FUNC);
        }
    }

    /* set serial leds pinmuxing */
    if ( BpGetSerialLEDMuxSel((unsigned short*)&MuxSel) == BP_SUCCESS )
    {
        if (MuxSel == BP_SERIAL_LED_MUX_GROUPA)
        {
            set_pinmux(PINMUX_SER_LED_CLK_A_PIN, PINMUX_SER_LED_A_FUNC);
            set_pinmux(PINMUX_SER_LED_DAT_A_PIN, PINMUX_SER_LED_A_FUNC);
        }
        else if (MuxSel == BP_SERIAL_LED_MUX_GROUPB)
        {
            set_pinmux(PINMUX_SER_LED_CLK_B_PIN, PINMUX_SER_LED_B_FUNC);
            set_pinmux(PINMUX_SER_LED_DAT_B_PIN, PINMUX_SER_LED_B_FUNC);
        }
        else if (MuxSel == BP_SERIAL_LED_MUX_GROUPC)
        {
            set_pinmux(PINMUX_SER_LED_CLK_C_PIN, PINMUX_SER_LED_C_FUNC);
            set_pinmux(PINMUX_SER_LED_DAT_C_PIN, PINMUX_SER_LED_C_FUNC);
        }
        set_pinmux(PINMUX_SER_LED_GATE_PIN, PINMUX_SER_LED_GATE_FUNC);
    }

    /* set rogue onu pinmuxing */
    if( BpGetRogueOnuEn((unsigned short*)&MuxSel) == BP_SUCCESS )
    {
        if (MuxSel)
            set_pinmux(PINMUX_ROGUE_ONU_PIN, PINMUX_ROGUE_ONU_FUNC);
    }

    /* set wan nco 10Mhz clock pinmuxing */
    if( BpGetWanNco10M((unsigned short*)&MuxSel) == BP_SUCCESS )
    {
        if (MuxSel)
            set_pinmux(PINMUX_WAN_NCO_10M_PIN, PINMUX_WAN_NCO_10M_FUNC);
    }

    /* set trx signal detect pinmuxing */
    if( BpGetTrxSignalDetect((unsigned short*)&MuxSel) == BP_SUCCESS )
    {
        if (MuxSel)
            set_pinmux(PINMUX_TRX_SD_PIN, PINMUX_TRX_SD_FUNC);
    }

    /* set E_WAKE pinmuxing */
    if( BpGetPmdMACEwakeEn((unsigned short*)&MuxSel) == BP_SUCCESS )
    {
        if (MuxSel)
            set_pinmux(PINMUX_EWAKE_GPON_MAC_PIN, PINMUX_EWAKE_GPON_MAC_FUNC);
    }

    /* Time Synchronization */

    /* 10/25MHz */
    if( BpGetTsync1025mhzPin((unsigned short*)&MuxSel) == BP_SUCCESS )
    {
        set_pinmux(BP_PIN_TSYNC_1025MHZ_11 & BP_GPIO_NUM_MASK, PINMUX_TSYNC_1025MHZ_11_FUNC);
    }

    /* 8KHz */
    if( BpGetTsync8khzPin((unsigned short*)&MuxSel) == BP_SUCCESS )
    {
        set_pinmux(BP_PIN_TSYNC_8KHZ_4 & BP_GPIO_NUM_MASK, PINMUX_TSYNC_8KHZ_4_FUNC);
    }

    /* 1PPS */
    if( BpGetTsync1ppsPin((unsigned short*)&MuxSel) == BP_SUCCESS )
    {
        if (MuxSel == BP_PIN_TSYNC_1PPS_6)
            set_pinmux(BP_PIN_TSYNC_1PPS_6 & BP_GPIO_NUM_MASK, PINMUX_TSYNC_1PPS_6_FUNC);
        else if (MuxSel == BP_PIN_TSYNC_1PPS_52)
            set_pinmux(BP_PIN_TSYNC_1PPS_52 & BP_GPIO_NUM_MASK, PINMUX_TSYNC_1PPS_52_FUNC);
    }

    /* MII interface */
    if( BpGetMiiInterfaceEn((unsigned short*)&MuxSel) == BP_SUCCESS )
    {
        set_pinmux(PINMUX_MII_RXCOL, PINMUX_MII_FUNC);
        set_pinmux(PINMUX_MII_RXCRS, PINMUX_MII_FUNC);
        set_pinmux(PINMUX_MII_RXER, PINMUX_MII_FUNC);
    }
   
    const ETHERNET_MAC_INFO    *pE;
    
    if ( (pE = BpGetEthernetMacInfoArrayPtr()) != NULL )
    {
        if (IsRgmiiDirect(pE[0].sw.phy_id[4]) || IsExtPhyId(pE[0].sw.phy_id[4]) || IsTMII(pE[0].sw.phy_id[4])  )
        {
          printf("Configuring RGMII pinpux\n");
          set_pinmux(PINMUX_RGMII_TXCLK_PIN, PINMUX_RGMII_FUNC);
          set_pinmux(PINMUX_RGMII_TXCTL_PIN, PINMUX_RGMII_FUNC);
          set_pinmux(PINMUX_RGMII_TXD_00_PIN, PINMUX_RGMII_FUNC);
          set_pinmux(PINMUX_RGMII_TXD_01_PIN, PINMUX_RGMII_FUNC);
          set_pinmux(PINMUX_RGMII_TXD_02_PIN, PINMUX_RGMII_FUNC);
          set_pinmux(PINMUX_RGMII_TXD_03_PIN, PINMUX_RGMII_FUNC);
          set_pinmux(PINMUX_RGMII_MDC, PINMUX_RGMII_FUNC);
        }
    }
    
    if (IsTMII(pE[0].sw.phy_id[4]))
    {
        set_pinmux(PINMUX_MII_TXER,  PINMUX_MII_FUNC);
    }

    /* set tx laser on out N pinmuxing */
    if( BpGetTxLaserOnOutN((unsigned short*)&MuxSel) == BP_SUCCESS )
    {
        if (MuxSel)
            set_pinmux(PINMUX_TX_LASER_ON_N_PIN, PINMUX_TX_LASER_ON_N_FUNC);
    }
   
    initGpioPinMux();

#endif

#if defined (_BCM963268_)
    LED->ledInit &= ~LED_FAST_INTV_MASK;
    LED->ledInit |= (LED_INTERVAL_20MS * 4) << LED_FAST_INTV_SHIFT;
#elif defined (_BCM960333_)
    bcm_common_led_init();
#elif defined (_BCM96838_)
    LED->ledInit &= ~LED_FAST_INTV_MASK;
    LED->ledInit |= (LED_INTERVAL_20MS * 2) << LED_FAST_INTV_SHIFT;
    LED->ledInit &= ~LED_SLOW_INTV_MASK;
    LED->ledInit |= (LED_INTERVAL_20MS * 8) << LED_SLOW_INTV_SHIFT;
    LED->ledInit |= LED_SERIAL_SHIFT_FRAME_POL | LED_SERIAL_LED_EN;
    /* in 968380GWAN board, EGPHY LEDs are shifted out serialy 
            FIXME board dependency should be removed*/
    if (!strcmp(boardIdStr, "968380GWAN"))
    {
        LED->ledSerialMuxSelect = 0x000000ff;
        LED->ledLinkActSelLow = 00410000;
        LED->ledHWDis = 0xfffffffc;
    }
    else
        /* Disable HW Link/Activity */
        LED->ledHWDis = 0xffffffff;
#elif defined (_BCM96848_)
    bcm_init_pinmux();
    bcm_common_led_init();
    bcm_common_gpio_init();
    phy_read_register(1, 2); // dummy read for mdio issue after reset.
#elif defined(_BCM947189_)
    bcm_misc_hw_init();
    bcm_common_gpio_init();
#else
#ifdef _BCM963381_
    if( (flash_get_flash_type() == FLASH_IFC_SPINAND) || bcm_otp_is_btrm_boot() )
        cfe_boot_second_cpu((unsigned long)second_cpu_icache_fixup);
#endif
#if defined (_BCM96858_) || defined(_BCM94908_) || defined(_BCM963158_) || defined(_BCM96856_) || defined(_BCM96846_) || defined (_BCM947622_) || defined (_BCM963178_) || defined (_BCM96878_)
    cfe_boot_second_cpu((unsigned long)CFG_BOOT_AREA_ADDR);
#endif

/* Boot secondary CPU if OP-TEE is **NOT** supported for BCM963138/BCM963148*/
#if (defined(_BCM963138_) || defined(_BCM963148_)) && !defined(BCM_OPTEE)
    cfe_boot_second_cpu((unsigned long)CFG_BOOT_AREA_ADDR);
#endif
    bcm_init_pinmux();
#if !defined(CONFIG_BRCM_IKOS)
    bcm_misc_hw_init();
#endif
    bcm_common_led_init();
#if !defined(CONFIG_BRCM_IKOS)
    bcm_common_gpio_init();
#endif
#endif

#if defined (CCI400_BASE)
    cci400_enable();
#endif
#if defined (CCI500_BASE)
    cci500_enable();
#endif
    cfe_enable_ns_access();

    power_led_off();
    setPowerOnLedOn();

}

/*  *********************************************************************
    *  board_bootdevice_init()
    *
    *  Initialize boot flash devices
    *
    *  Input parameters:
    *      nothing
    *
    *  Return value:
    *      nothing
    ********************************************************************* */
void board_bootdevice_init(void)
{

    /* Init raw flash devices - This call will only
     * initialize NAND/NOR flash devices which match 
     * the strap selections */
    kerSysFlashInit();
        
#if INC_EMMC_FLASH_DRIVER
    /* Initialization of EMMC devices is not supported by the flash API.
     * If the board is strapped to boot from eMMC, the flash API will simply
     * detect and report this strap setting. The following will initialize 
     * eMMC device. 
     */
#if !INC_NAND_FLASH_DRIVER    
    /* If we are building EMMC only cfe, initialize EMMC devices
     * only if we are strapped to boot from EMMC. Otherwise, if NAND is 
     * also included and we are strapped for NAND, initialize EMMC 
     * regardless. This is done to allow for NAND/EMMC coexistence
     */
    if ( flash_get_flash_type() == FLASH_IFC_UNSUP_EMMC )
#endif	    
    	emmc_init_dev();    
#endif

}

/*  *********************************************************************
    *  board_netdevice_init()
    *
    *  Do only net device initialization, due to a fact that we only need
    *  to initialize network device when we need it in CFE
    *
    *  Input parameters:
    *      nothing
    *
    *  Return value:
    *      nothing
    ********************************************************************* */
void board_netdevice_init(void)
{
#if defined(_BCM96838_) || defined(_BCM963138_) || defined(_BCM963148_) || defined(_BCM96848_) || \
    defined(_BCM96858_) || defined (_BCM96846_)|| defined (_BCM96878_) || defined(_BCM96856_)
#if (NONETWORK==0)
#if defined(_BCM96858_) || defined (_BCM96846_) || defined(_BCM96856_) || defined(_BCM96878_)
    cfe_add_device( &bcm6xxx_impl3_enet, 0, 0, 0);
#else
    cfe_add_device( &bcm6xxx_impl2_enet, 0, 0, 0);
#endif
#endif
#elif defined (_BCM947189_)
#if (NONETWORK==0)
    cfe_add_device( &bcm47189_enet, 0, 0, 0);
#endif
#else
    /* Add the ethernet driver. */
#if (NONETWORK==0)
    cfe_add_device( &bcm63xx_enet, 0, 0, 0);
#endif
#endif
}

int g_restore_default;

void check_restore_default(void)
{
#define Button_Reg 0xff800520	
#define Reset_GPIO 4	
	unsigned int *addr_button = (unsigned int*)Button_Reg;
	int i;
	
	g_restore_default=1;
	for( i=0; i<100; i++ )
	{
		if( ((*addr_button) & (1<<Reset_GPIO)) != 0 )  // Reset button not pressed
		{
			g_restore_default=0;
			break;
		}
			
        cfe_usleep(100000); //delay 100ms
	}
	
	if( g_restore_default==1 )
	{	
		setAllLedsOff();
		diagnostic_led_on();
		printf("===== Restore default =====\n");
		kerSysErasePsi();
	}
	return;
}

/*  *********************************************************************
    *  board_final_init()
    *
    *  Do any final initialization, such as adding commands to the
    *  user interface.
    *
    *  If you don't want a user interface, put the startup code here.
    *  This routine is called just before CFE starts its user interface.
    *
    *  Input parameters:
    *      int force_cfe.  Stop at cfe it is non zero
    *
    *  Return value:
    *      nothing
    ********************************************************************* */

void board_final_init(int force_cfe)
{
    unsigned short rstToDfltIrq;
    int breakIntoCfe = force_cfe;
    /* Foxconn add start by Cliff Wang, 03/23/2010 */
    int autoRun;
    /* Foxconn add end by Cliff Wang, 03/23/2010 */


#if (defined(_BCM963178_) || defined(_BCM947622_) || defined(_BCM96878_)) && defined(CONFIG_BRCM_IKOS)
#else
#if (INC_PMC_DRIVER==1)
    /* Do not boot PMC in safe mode */
    if ( BCM_MBOX_SAFEMODE_GET() )
        printf("** Bypassing PMC boot **\n");
    else
        pmc_init();
#endif
#endif
    setAllLedsOff();
    setPowerOnLedOn();


#if (INC_SPI_PROG_NAND==1)
    rstToDfltIrq = 1 ;
    if (bootInfo.runFrom != 'n')
        breakIntoCfe = rstToDfltIrq;
#else
    if( (bootInfo.runFrom == 'f') && getBootImageTag() == NULL ) {
        setBreakIntoCfeLed();
        printf("** Flash image not found. **\n\n");
        kerSysErasePsi();
        breakIntoCfe = 1;
    }

    if( BpGetResetToDefaultExtIntr( &rstToDfltIrq ) == BP_SUCCESS ) {
        if (checkForResetToDefaultHold( rstToDfltIrq )) {
            kerSysErasePsi();
            /* Reset the default bootline if the board IP address has changed. */
            if (strcmp(bootInfo.boardIp, DEFAULT_BOARD_IP) != 0) {
                setDefaultBootline();
            }
            breakIntoCfe = 1;
        }
    }
#endif

    check_restore_default();
    /* Foxconn add start by Cliff Wang, 03/23/2010 */
    //bcm63xx_run(breakIntoCfe);
    autoRun = bcm63xx_run_ex(breakIntoCfe, 0);
    setBreakIntoCfeLed();
    if (autoRun)
        ui_docommand("tftpd");
    /* Foxconn add end by Cliff Wang, 03/23/2010 */
}

/*  *********************************************************************
    * Miscellaneous Board Functions
    ********************************************************************* */

/*  *********************************************************************
    *  checkForResetToDefaultHold()
    *
    *  Determines if the user is holding the reset to default button.
    *
    *  Input parameters:
    *      Reset to default irq#
    *
    *  Return value:
    *      1 - break into the CFE, 0 - continue boot sequence
    ********************************************************************* */
#if (INC_SPI_PROG_NAND==0)
static int checkForResetToDefaultHold( unsigned short rstToDfltIrq )
{
#if defined(CONFIG_BRCM_IKOS)
    return 0;
#elif defined (_BCM960333_) || defined  (_BCM963381_) || defined (_BCM947189_)
    //FIXME to add GPIO support
    return 0;
#else
    const int nBreakIntoCfeDelay = 5;
    int ret = 0, i = 0, buttonPressed = 0;
#if !defined (_BCM96848_)
    int irqActiveHigh = 0;
#endif
#if defined (_BCM960333_) || defined (_BCM963268_)
    int irqShared = 0;
    unsigned short gpioPin = 0;
#endif
#if !defined (_BCM96838_)  && \
    !defined (_BCM94908_)  && \
    !defined (_BCM96858_)  && \
    !defined (_BCM963158_) && \
    !defined (_BCM963178_) && \
    !defined(_BCM947622_) && \
    !defined (_BCM96846_)  && \
    !defined (_BCM96878_)  && \
    !defined (_BCM96856_)
    uint32 irqBit;
    volatile uint32 *extIrqReg;
#endif
#if defined (_BCM96838_)  || \
    defined (_BCM94908_)  || \
    defined (_BCM96858_)  || \
    defined (_BCM963158_) || \
    defined (_BCM963178_) || \
    defined(_BCM947622_)   || \
    defined (_BCM96846_)  || \
    defined (_BCM96878_)  || \
    defined (_BCM96856_) 
    unsigned short gpioPin = 0;
    BpGetResetToDefaultExtIntrGpio(&gpioPin);
    if (gpioPin & BP_ACTIVE_LOW)
        irqActiveHigh = 0;
    else
        irqActiveHigh = 1;
#else
#if !defined (_BCM96848_)
    irqActiveHigh = IsExtIntrTypeActHigh(rstToDfltIrq);
#endif

#if defined (_BCM960333_) || defined (_BCM963268_)
    irqShared = IsExtIntrShared(rstToDfltIrq);
    if( irqShared )
    {
        BpGetResetToDefaultExtIntrGpio(&gpioPin);
        /* make it as input */
        GPIO->GPIODir &= ~GPIO_NUM_TO_MASK(gpioPin);
#if defined(_BCM963268_)
        /* enable PERIPH CTRL for pin 36 to 51 in 63268 */
        if( (gpioPin&BP_GPIO_NUM_MASK) >= 32 )
            GPIO->GPIOCtrl |= GPIO_NUM_TO_MASK(gpioPin-32);
#endif
    }
#endif

    rstToDfltIrq &= ~BP_EXT_INTR_FLAGS_MASK;
#if defined(_BCM963138_) || defined(_BCM963148_)
    extIrqReg = &PERF->ExtIrqStatus;
#elif defined(_BCM96848_)
    extIrqReg = &PERF->ExtIrqSts;
#else
    extIrqReg = &PERF->ExtIrqCfg;
#endif
    irqBit = 1 << (rstToDfltIrq + EI_STATUS_SHFT);

#endif

    /* Loop while the reset to default button is depressed. */
    while(1) {
#if defined (_BCM96848_)
        buttonPressed = (*extIrqReg & irqBit);
#else
        if( irqActiveHigh  ){
#if defined (_BCM96838_)
            buttonPressed = gpio_get_data(gpioPin&BP_GPIO_NUM_MASK);
#elif defined (_BCM94908_) || defined (_BCM96858_) || defined (_BCM963158_) || \
      defined (_BCM96846_) || defined (_BCM96856_) || defined (_BCM963178_) || defined(_BCM947622_) || defined(_BCM96878_)
            buttonPressed = bcm_gpio_get_data(gpioPin&BP_GPIO_NUM_MASK);
#else
            buttonPressed = (*extIrqReg & irqBit);
#endif
#if defined (_BCM960333_) || defined (_BCM963268_)
            if( irqShared && !(GPIO->GPIOio&GPIO_NUM_TO_MASK(gpioPin)) )
                buttonPressed = 0;
#endif
        }
        else{
#if defined (_BCM96838_)
            buttonPressed = !gpio_get_data(gpioPin&BP_GPIO_NUM_MASK);
#elif defined (_BCM94908_) || defined (_BCM96858_) || defined (_BCM963158_) || \
      defined (_BCM96846_) || defined (_BCM96856_) || defined (_BCM963178_) || defined(_BCM947622_) || defined(_BCM96878_)
            buttonPressed = !bcm_gpio_get_data(gpioPin&BP_GPIO_NUM_MASK);
#else
            buttonPressed = !(*extIrqReg & irqBit);
#endif
#if defined (_BCM960333_) || defined (_BCM963268_)
            if( irqShared && (GPIO->GPIOio&GPIO_NUM_TO_MASK(gpioPin)) )
                buttonPressed = 0;
#endif
        }
#endif

        if( buttonPressed ){
            if (i == nBreakIntoCfeDelay) {
                setBreakIntoCfeLed();
                printf("\n*** Break into CFE console ***\n\n");
                ret = 1;
                break;
            }
            cfe_sleep(CFE_HZ);
            i++;
        }
        else
            break;
    }

    return( ret );
#endif
}
#endif
/*  *********************************************************************
    *  setLed(led_gpio, led_state)
    *
    *  Turns on an LED.
    *
    *  Input parameters:
    *      LED purpose
    *      LED State
    *
    *  Return value:
    *      nothing
    ********************************************************************* */

void setLed (unsigned short led_gpio, unsigned short led_state)
{
#if defined(_BCM963268_) || defined(_BCM96838_) 
    unsigned short gpio_state;

    if (((led_gpio & BP_ACTIVE_LOW) && (led_state == LED_ON)) || 
        (!(led_gpio & BP_ACTIVE_LOW) && (led_state == LED_OFF)))
        gpio_state = 0;
    else
        gpio_state = 1;
#endif

    if ( (led_gpio&BP_GPIO_NUM_MASK) == BP_GPIO_NONE )
      return;

#if defined (_BCM963268_)
    /* Enable LED controller to drive this GPIO */
    if (!(led_gpio & BP_GPIO_SERIAL))
        GPIO->LEDCtrl |= GPIO_NUM_TO_MASK(led_gpio);
#endif

#if defined (_BCM963268_)
    LED->ledMode &= ~(LED_MODE_MASK << GPIO_NUM_TO_LED_MODE_SHIFT(led_gpio));
    if( gpio_state )
        LED->ledMode |= (LED_MODE_OFF << GPIO_NUM_TO_LED_MODE_SHIFT(led_gpio));
    else
        LED->ledMode |= (LED_MODE_ON << GPIO_NUM_TO_LED_MODE_SHIFT(led_gpio));

#elif defined(_BCM96838_)
    if ( (led_gpio&BP_LED_PIN) || (led_gpio&BP_GPIO_SERIAL) )
    {
        LED->ledMode &= ~(LED_MODE_MASK << GPIO_NUM_TO_LED_MODE_SHIFT(led_gpio));
        if( gpio_state )
            LED->ledMode |= (LED_MODE_OFF << GPIO_NUM_TO_LED_MODE_SHIFT(led_gpio));
        else
            LED->ledMode |= (LED_MODE_ON << GPIO_NUM_TO_LED_MODE_SHIFT(led_gpio));
    }
    else
    {
        unsigned short gpio_pin = led_gpio & BP_GPIO_NUM_MASK;
        gpio_set_dir(gpio_pin, 1);
        if( gpio_state )
            gpio_set_data(gpio_pin, 1);
        else
            gpio_set_data(gpio_pin, 0);
    }
#else
    bcm_led_driver_set(led_gpio, led_state);
#endif
}

/*  *********************************************************************
    *  setPinMuxGpio()
    *
    *  Set pin mux to GPIO function
    *
    *  Input parameters:
    *      unsigned short gpio pin number
    *      int i LED index in the bpLedList (used only in 6838)
    *
    *  Return value:
    *      nothing
    ********************************************************************* */

static void setPinMuxGpio(unsigned short gpio, int i)
{
#if defined (_BCM96838_)

    if( (gpio&BP_GPIO_NUM_MASK) == BP_GPIO_NONE )
        return;
    
    if( (gpio&BP_LED_PIN) == BP_LED_PIN )
    {   
        /* For parallel connected Leds to LED controller - set pinmux */
        unsigned short pinmux;
        if ( BpGetLedPinMuxGpio(i, &pinmux) == BP_SUCCESS )
        {
            pinmux &= BP_GPIO_NUM_MASK;
            if (LED_CTRL_FUNC[pinmux] != NO_PINMUX)
                set_pinmux(pinmux, LED_CTRL_FUNC[pinmux]);
        }
    }
    else if ( (gpio&BP_GPIO_SERIAL) != BP_GPIO_SERIAL )
        /* For LEDs connected directly to GPIO - set pinmux */
        set_pinmux(gpio&BP_GPIO_NUM_MASK, PINMUX_GPIO_FUNC);
#endif
    return;
}

/*  *********************************************************************
    *  initGpioPinMux()
    *
    *  Initialize the gpio pin mux register setting. On some chip like 6318, Certain
    *  gpio pin are muxed with other function and  are not default to gpio. so init
    *  code needs to set the mux to gpio if they are used by led or gpio boardparm
    *
    *
    *  Input parameters: none
    *
    *  Return value:
    *      nothing
    ********************************************************************* */
void initGpioPinMux(void)
{
    int i = 0,  rc;
    void* token = NULL;
    unsigned short gpio;

    /* walk through all the led bp */
    for(;;)
    {
        rc = BpGetLedGpio(i, &token, &gpio);
        if( rc == BP_MAX_ITEM_EXCEEDED )
            break;
        else if( rc == BP_SUCCESS )
            setPinMuxGpio(gpio, i);
        else
        {
            token = NULL;
            i++;
        }
    }

    /* walk through all the gpio bp */
    i = 0;
    token = NULL;
    for(;;)
    {
        rc = BpGetGpioGpio(i, &token, &gpio);
        if( rc == BP_MAX_ITEM_EXCEEDED )
            break;
        else if( rc == BP_SUCCESS )
            setPinMuxGpio(gpio,i);
        else
        {
            token = NULL;
            i++;
        }
    }

    return;
}

/*  *********************************************************************
    *  setAllLedsOff()
    *
    *  Turns off all board LEDs on init
    *
    *  Input parameters:
    *      LED purpose
    *
    *  Return value:
    *      nothing
    ********************************************************************* */

void setAllLedsOff(void)
{
#if defined(_BCM960333_) || defined(_BCM963268_) || defined(_BCM96838_)
    unsigned short gpio;
    int i = 0, rc;
    void* token = NULL;

    for(;;)
    {
        rc = BpGetLedGpio(i, &token, &gpio);
        if( rc == BP_MAX_ITEM_EXCEEDED )
            break;
        else if( rc == BP_SUCCESS )
        {
            setLed( gpio, LED_OFF );
        }
        else
    {
        token = NULL;
            i++;
    }
    }
#else
    set_LED_cfg(_LED_OFF, STATE, BL_000, 0, 0);
    //bcm_common_led_setAllSoftLedsOff();
#endif
    return;
}

/*  *********************************************************************
    *  setPowerOnLedOn()
    *
    *  Turns on the Power LED.
    *
    *  Input parameters:
    *      LED purpose
    *
    *  Return value:
    *      nothing
    ********************************************************************* */

void setPowerOnLedOn(void)
{
    set_LED_cfg(_LED_OFF, STATE, BL_000, 0, 0);
    set_LED_cfg(_LED_WHITE, STATE, BL_101, 0, 1);
}


/*  *********************************************************************
    *  setBreakIntoCfeLed()
    *
    *  Turns on the alarm LED.
    *
    *  Input parameters:
    *      LED purpose
    *
    *  Return value:
    *      nothing
    ********************************************************************* */

void setBreakIntoCfeLed(void)
{
    setAllLedsOff();
    set_LED_cfg(_LED_AMBER, STATE, BL_101, 1, 1);
#if 0
    unsigned short gpio;
    if( BpGetBootloaderStopLedGpio( &gpio ) == BP_SUCCESS ) {
        setLed( gpio, LED_ON );
        if( BpGetBootloaderPowerOnLedGpio( &gpio ) == BP_SUCCESS )
            setLed( gpio, LED_OFF );
    }
#endif
}

/*  *********************************************************************
    *  softReset()
    *
    *  Resets the board.
    *
    *  Input parameters:
    *      delay in second
    *
    *  Return value:
    *      nothing
    ********************************************************************* */

void softReset(unsigned int delay)
{
    printf( "\nResetting board in %d seconds...\n", delay );
    delay == 0?  bcm_cmn_wd_reset() : bcm_cmn_wd_set(delay, 1);
    while (1);
}

/* Foxconn added start , 1/29/2010 */
extern int nmrp_server_detected;
static unsigned long calculate_checksum (int action, char *s, int size)
{
    static unsigned long c0, c1;
    unsigned long checksum, b;
    int i;
    switch (action)
    {
        case 0:
            c0 = c1 = 0;
            break;

        case 1:
            for (i = 0; i < size; i++)
            {
                c0 += s[i] & 0xff;
                c1 += c0;
                /* check the lan/wan ports status, Foxconn added by EricHuang, 10/24/2006 */
                if ((i % 90000) == 0)
                {
                    printf(".");
//                    bcm_robo_check_link_status();
                }
            }
            break;

        case 2:
            b = (c0 & 65535) + ((c0 >> 16) & 65535);
            c0 = ((b >> 16) + b) & 65535;

            b = (c1 & 65535) + ((c1 >> 16) & 65535);
            c1 = ((b >> 16) + b) & 65535;

            checksum = ((c1 << 16) | c0);

            return checksum;
    }
    return 0;
}

int verify_checksum(char *buf, unsigned long buf_len, unsigned long chksum)
{
    unsigned long cal_chksum = 0;

    xprintf("Loading .");
    calculate_checksum (0, NULL, 0);
    calculate_checksum (1, buf, buf_len);
    cal_chksum = calculate_checksum (2, NULL, 0);
    printf("\n");


    if (cal_chksum != chksum) {
        xprintf("Image chksum: 0x%08X\n", chksum);
        xprintf("Calc  chksum: 0x%08X\n", cal_chksum);
        return -1;
    }

#ifdef _DEBUG
    //printf("verify_checksum calculate_checksum    = 0x%08X\n", cal_chksum);
    //printf("verify_checksum image_chksum          = 0x%08X\n", chksum);
#endif

    return 0;
}

int flash_region_num(uint16_t region_num)
{
    int res = CFE_OK;
    NVRAM_DATA nvramData;

    NVRAM_COPY_TO(&nvramData);

    *(unsigned short *)nvramData.szRegionNum = region_num;

    NVRAM_UPDATE(&nvramData);

    return res;
}

void enable_load_default_mode(void)
{
    NVRAM_DATA nvramData;

    // NMRP mode is finished, enable load default mode
    NVRAM_COPY_TO(&nvramData);
    nvramData.ucUnused2[1] = 'E';
    nvramData.ucUnused2[0] = 'N';
    printf("Enable load default mode\n");
    NVRAM_UPDATE(&nvramData);
}

void power_led_off(void)
{
    set_LED_cfg(_LED_OFF, STATE, BL_000, 0, 0);
}

void power_led_toggle(int state)
{
    if (state == 0)
    {
        power_led_off();
    }
    else
    {
        setPowerOnLedOn();
    }
}

void diagnostic_led_on(void)
{
    set_LED_cfg(_LED_OFF, STATE, BL_000, 0, 0);
    set_LED_cfg(_LED_AMBER, STATE, BL_000, 1, 1);
}

void diagnostic_led_off(void)
{
    set_LED_cfg(_LED_OFF, STATE, BL_000, 0, 0);
}

void diagnostic_led_toggle(void)
{
    static int led_state = 0;

    if (led_state == 0)
        led_state = 1;
    else
        led_state = 0;

    if (led_state == 0)
    {
        diagnostic_led_off();
    }
    else
    {
        diagnostic_led_on();
    }
}

void chk_rst_btn_event(void)
{
    if (bcm_gpio_get_data(GPIO_RST_BTN & BP_GPIO_NUM_MASK) == 0)
        enable_load_default_mode();
}
/* Foxconn added end , 1/29/2010 */
