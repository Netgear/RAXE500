#include "bp_defs.h"
#include "boardparms.h"
#include "bcmSpiRes.h"

#ifdef _CFE_
#define MOVED_DT(x...) x
#else
#define MOVED_DT(x...)
#endif

static bp_elem_t g_bcm968560sv[] = {
  {bp_cpBoardId,               .u.cp = "968560SV"},
  MOVED_DT({bp_usI2sSdata,              .u.us = BP_GPIO_0_AH},)
  MOVED_DT({bp_usI2sSclk,               .u.us = BP_GPIO_1_AH},)
  MOVED_DT({bp_usI2sLrck,               .u.us = BP_GPIO_2_AH},)
  MOVED_DT({bp_usGpioWanSignalDetected, .u.us = BP_GPIO_8_AH},)
  MOVED_DT({bp_usGpioOpticalModuleTxPwrDown, .u.us = BP_GPIO_9_AH},)
  MOVED_DT({bp_usRogueOnuEn,            .u.us = BP_GPIO_10_AH},)
  MOVED_DT({bp_usExtIntrOpticalModulePresence, .u.us = BP_EXT_INTR_3 | BP_EXT_INTR_TYPE_IRQ_BOTH_EDGE | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_23_AL},)
  MOVED_DT({bp_usGpioPonTxEn,           .u.us = BP_GPIO_12_AH},)
  MOVED_DT({bp_usPonLbe,                .u.us = BP_GPIO_22_AL},)
  MOVED_DT({bp_usGpioI2cSda,            .u.us = BP_GPIO_19_AL},)
  MOVED_DT({bp_usGpioI2cScl,            .u.us = BP_GPIO_20_AL},)
  MOVED_DT({bp_usSerialLedData,         .u.us = BP_GPIO_26_AH},)
  MOVED_DT({bp_usSerialLedClk,          .u.us = BP_GPIO_27_AH},)
  MOVED_DT({bp_usSerialLedMask,         .u.us = BP_GPIO_28_AH},)
  MOVED_DT({bp_usMiiMdc,                .u.us = BP_GPIO_60_AH},)
  MOVED_DT({bp_usMiiMdio,               .u.us = BP_GPIO_61_AH},)
  MOVED_DT({bp_usPcmSdin,               .u.us = BP_GPIO_45_AH},)
  MOVED_DT({bp_usPcmSdout,              .u.us = BP_GPIO_46_AH},)
  MOVED_DT({bp_usPcmClk,                .u.us = BP_GPIO_47_AH},)
  MOVED_DT({bp_usPcmFs,                 .u.us = BP_GPIO_75_AH},)
  MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,               .u.ul = 0x3f},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0x01 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_3_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_4_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 0x02 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_5_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_6_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 0x03 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_7_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_8_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 0x04 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_9_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_10_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId4,                .u.ul = 0x07 | MAC_IF_RGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_TX_INTERNAL_DELAY},)
  MOVED_DT({bp_ulPhyId5,                .u.ul = 0x1e | MAC_IF_HSGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_12_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_13_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_14_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_15_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_16_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_17_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_18_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_19_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_20_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_21_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_22_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_23_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_24_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_25_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_26_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_27_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_28_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_29_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_30_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_31_AL},)
  MOVED_DT({bp_usUsbPwrFlt0,            .u.us = BP_GPIO_76_AL},)
  MOVED_DT({bp_usUsbPwrOn0,             .u.us = BP_GPIO_77_AL},)
  MOVED_DT({bp_usUsbPwrFlt1,            .u.us = BP_GPIO_78_AL},)
  MOVED_DT({bp_usUsbPwrOn1,             .u.us = BP_GPIO_79_AL},)
  MOVED_DT({bp_usUart1Sdin,             .u.us = BP_GPIO_24_AH},) 
  MOVED_DT({bp_usUart1Sdout,            .u.us = BP_GPIO_25_AH},) 
  MOVED_DT({bp_usGpioUart2Sdin,         .u.us = BP_GPIO_82_AH},)
  MOVED_DT({bp_usGpioUart2Sdout,        .u.us = BP_GPIO_83_AH},)        
  MOVED_DT({bp_ulMemoryConfig,          .u.ul = BP_DDR_SPEED_800_11_11_11 | BP_DDR_TOTAL_SIZE_512MB| BP_DDR_DEVICE_WIDTH_16 | BP_DDR_TOTAL_WIDTH_16BIT | BP_DDR_SSC_CONFIG_1},)
  MOVED_DT({bp_usSpiSlaveSelectNum,     .u.us = 1},)
  MOVED_DT({bp_usSpiSlaveSelectGpioNum, .u.us = BP_GPIO_81_AH},)
  {bp_ucDspType0,              .u.uc = BP_VOIP_DSP},
  {bp_ucDspAddress,            .u.uc = 0},
  {bp_last}
};

static bp_elem_t g_bcm968560sv_opt[] = {
  {bp_cpBoardId,                .u.cp = "968560SV_OPT"},
  MOVED_DT({bp_ucPhyType0,               .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,             .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,             .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,                .u.ul = 0x3f},)
  MOVED_DT({bp_ulPhyId0,                 .u.ul = 0x01 | MAC_IF_GMII},)
  MOVED_DT({bp_ulPhyId1,                 .u.ul = 0x02 | MAC_IF_GMII},)
  MOVED_DT({bp_ulPhyId2,                 .u.ul = 0x03 | MAC_IF_GMII},)
  MOVED_DT({bp_ulPhyId3,                 .u.ul = 0x04 | MAC_IF_GMII},)
  MOVED_DT({bp_ulPhyId4,                 .u.ul = 0x07 | MAC_IF_RGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPortFlags,              .u.ul = PORT_FLAG_TX_INTERNAL_DELAY},)
  MOVED_DT({bp_ulPhyId5,                 .u.ul = 0x06 | MAC_IF_SGMII},)
  {bp_elemTemplate,             .u.bp_elemp = g_bcm968560sv},
  {bp_last}
};

static bp_elem_t g_bcm968360bg[] = {
  {bp_cpBoardId,               .u.cp = "968360BG"},
  MOVED_DT({bp_usI2sSdata,              .u.us = BP_GPIO_0_AH},)
  MOVED_DT({bp_usI2sSclk,               .u.us = BP_GPIO_1_AH},)
  MOVED_DT({bp_usI2sLrck,               .u.us = BP_GPIO_2_AH},)
  MOVED_DT({bp_usGpioWanSignalDetected, .u.us = BP_GPIO_8_AH},)
  {bp_usGpioPmdReset,          .u.us = BP_GPIO_9_AL},
  MOVED_DT({bp_usExtIntrPmdAlarm,       .u.us = BP_EXT_INTR_2 | BP_EXT_INTR_TYPE_IRQ_HIGH_LEVEL | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_10_AH},)
  {bp_usGpioPonTxEn,           .u.us = BP_GPIO_12_AH},
  MOVED_DT({bp_usGpioI2cSda,            .u.us = BP_GPIO_19_AL},)
  MOVED_DT({bp_usGpioI2cScl,            .u.us = BP_GPIO_20_AL},)
  MOVED_DT({bp_usPonLbe,                .u.us = BP_GPIO_22_AL},)
  MOVED_DT({bp_InvSerdesTxPol,          .u.us = pmd_polarity_invert},)
  MOVED_DT({bp_usPmdMACEwakeEn,         .u.us = BP_GPIO_24_AH},)
  MOVED_DT({bp_usSerialLedData,         .u.us = BP_GPIO_26_AH},)
  MOVED_DT({bp_usSerialLedClk,          .u.us = BP_GPIO_27_AH},)
  MOVED_DT({bp_usSerialLedMask,         .u.us = BP_GPIO_28_AH},)
  MOVED_DT({bp_usMiiMdc,                .u.us = BP_GPIO_60_AH},)
  MOVED_DT({bp_usMiiMdio,               .u.us = BP_GPIO_61_AH},)
  MOVED_DT({bp_usPcmSdin,               .u.us = BP_GPIO_45_AH},)
  MOVED_DT({bp_usPcmSdout,              .u.us = BP_GPIO_46_AH},)
  MOVED_DT({bp_usPcmClk,                .u.us = BP_GPIO_47_AH},)
  MOVED_DT({bp_usPcmFs,                 .u.us = BP_GPIO_75_AH},)
  MOVED_DT({bp_usExtIntrSesBtnWireless, .u.us = BP_EXT_INTR_0 | BP_EXT_INTR_TYPE_IRQ_LOW_LEVEL | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_83_AL},)
  MOVED_DT({bp_usExtIntrResetToDefault, .u.us = BP_EXT_INTR_1 | BP_EXT_INTR_TYPE_IRQ_LOW_LEVEL | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_82_AL},)
  MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,               .u.ul = 0x3f},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0x01 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_3_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_4_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 0x02 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_5_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_6_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 0x03 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_7_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_8_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 0x04 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_9_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_10_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId4,                .u.ul = 0x07 | MAC_IF_RGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_TX_INTERNAL_DELAY},)
  MOVED_DT({bp_ulPhyId5,                .u.ul = 0x1e | MAC_IF_HSGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_0_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_1_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_2_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_12_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_13_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_14_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_15_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_16_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_17_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_18_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_19_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_20_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_21_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_22_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_23_AL},)
  MOVED_DT({bp_usUsbPwrFlt0,            .u.us = BP_GPIO_76_AL},)
  MOVED_DT({bp_usUsbPwrOn0,             .u.us = BP_GPIO_77_AL},)
  MOVED_DT({bp_usUsbPwrFlt1,            .u.us = BP_GPIO_78_AL},)
  MOVED_DT({bp_usUsbPwrOn1,             .u.us = BP_GPIO_79_AL},)
  MOVED_DT({bp_usGpioUart2Sdin,         .u.us = BP_GPIO_7_AH},)
  MOVED_DT({bp_usGpioUart2Sdout,        .u.us = BP_GPIO_25_AH},)
  MOVED_DT({bp_pmdFunc,                 .u.us = BP_PMD_APD_REG_DISABLED | BP_PMD_APD_TYPE_BOOST},)
  MOVED_DT({bp_ulMemoryConfig,          .u.ul = BP_DDR_SPEED_800_11_11_11 | BP_DDR_TOTAL_SIZE_512MB| BP_DDR_DEVICE_WIDTH_16 | BP_DDR_TOTAL_WIDTH_16BIT | BP_DDR_SSC_CONFIG_1},)
  MOVED_DT({bp_usSpiSlaveSelectNum,     .u.us = 1},)
  MOVED_DT({bp_usSpiSlaveSelectGpioNum, .u.us = BP_GPIO_81_AH},)
  {bp_ucDspType0,              .u.uc = BP_VOIP_DSP},
  {bp_ucDspAddress,            .u.uc = 0},
  {bp_last}
};

static bp_elem_t g_bcm968560ref[] = {
  {bp_cpBoardId,               .u.cp = "968560REF"},
  MOVED_DT({bp_usI2sSdata,              .u.us = BP_GPIO_0_AH},)
  MOVED_DT({bp_usI2sSclk,               .u.us = BP_GPIO_1_AH},)
  MOVED_DT({bp_usI2sLrck,               .u.us = BP_GPIO_2_AH},)
  MOVED_DT({bp_usGpioWanSignalDetected, .u.us = BP_GPIO_8_AH},)
  MOVED_DT({bp_usGpioPonTxEn,           .u.us = BP_GPIO_12_AH},)
  MOVED_DT({bp_usGpioI2cSda,            .u.us = BP_GPIO_19_AL},)
  MOVED_DT({bp_usGpioI2cScl,            .u.us = BP_GPIO_20_AL},)
  MOVED_DT({bp_usPonLbe,                .u.us = BP_GPIO_22_AL},)
  MOVED_DT({bp_usGpioOpticalModuleTxPwrDown, .u.us = BP_GPIO_9_AH},)
  MOVED_DT({bp_usExtIntrOpticalModulePresence, .u.us = BP_EXT_INTR_3 | BP_EXT_INTR_TYPE_IRQ_BOTH_EDGE | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_23_AL},)
  MOVED_DT({bp_usSerialLedData,         .u.us = BP_GPIO_26_AH},)
  MOVED_DT({bp_usSerialLedClk,          .u.us = BP_GPIO_27_AH},)
  MOVED_DT({bp_usSerialLedMask,         .u.us = BP_GPIO_28_AH},)
  MOVED_DT({bp_usPcmSdin,               .u.us = BP_GPIO_45_AH},)
  MOVED_DT({bp_usPcmSdout,              .u.us = BP_GPIO_46_AH},)
  MOVED_DT({bp_usPcmClk,                .u.us = BP_GPIO_47_AH},)
  MOVED_DT({bp_usPcmFs,                 .u.us = BP_GPIO_75_AH},)
  MOVED_DT({bp_usMiiMdc,                .u.us = BP_GPIO_60_AH},)
  MOVED_DT({bp_usMiiMdio,               .u.us = BP_GPIO_61_AH},)
  MOVED_DT({bp_usExtIntrSesBtnWireless, .u.us = BP_EXT_INTR_0 | BP_EXT_INTR_TYPE_IRQ_LOW_LEVEL | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_83_AL},)
  MOVED_DT({bp_usExtIntrResetToDefault, .u.us = BP_EXT_INTR_1 | BP_EXT_INTR_TYPE_IRQ_LOW_LEVEL | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_82_AL},)
  MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,               .u.ul = 0x3f},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0x01 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_3_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_4_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 0x02 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_5_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_6_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 0x03 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_7_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_8_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 0x04 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_9_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_10_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId4,                .u.ul = 0x07 | MAC_IF_RGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_TX_INTERNAL_DELAY},)
  MOVED_DT({bp_ulPhyId5,                .u.ul = 0x1e | MAC_IF_HSGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_SWAP_PAIR},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_0_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_1_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_2_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_12_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_13_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_14_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_15_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_16_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_17_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_18_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_19_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_20_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_21_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_22_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_23_AL},)
  MOVED_DT({bp_usUsbPwrFlt0,            .u.us = BP_GPIO_76_AL},)
  MOVED_DT({bp_usUsbPwrOn0,             .u.us = BP_GPIO_77_AL},)
  MOVED_DT({bp_usUsbPwrFlt1,            .u.us = BP_GPIO_78_AL},)
  MOVED_DT({bp_usUsbPwrOn1,             .u.us = BP_GPIO_79_AL},)
  MOVED_DT({bp_usGpioUart2Sdin,         .u.us = BP_GPIO_7_AH},)
  MOVED_DT({bp_usGpioUart2Sdout,        .u.us = BP_GPIO_25_AH},)
  MOVED_DT({bp_ulMemoryConfig,          .u.ul = BP_DDR_SPEED_1067_14_14_14 | BP_DDR_TOTAL_SIZE_512MB| BP_DDR_DEVICE_WIDTH_16 | BP_DDR_TOTAL_WIDTH_32BIT | BP_DDR_SSC_CONFIG_1},)
  MOVED_DT({bp_usSpiSlaveSelectNum,     .u.us = 1},)
  MOVED_DT({bp_usSpiSlaveSelectGpioNum, .u.us = BP_GPIO_81_AH},)
  {bp_ucDspType0,              .u.uc = BP_VOIP_DSP},
  {bp_ucDspAddress,            .u.uc = 0},
  {bp_last}
};

static bp_elem_t g_bcm968560ref_ng2[] = {
  {bp_cpBoardId,               .u.cp = "968560REF_NG2"},
  MOVED_DT({bp_usExtIntrOpticalModulePresence, .u.us = BP_EXT_INTR_3 | BP_EXT_INTR_TYPE_IRQ_BOTH_EDGE | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_21_AL},)
  {bp_elemTemplate,            .u.bp_elemp = g_bcm968560ref},
  {bp_last}
};

static bp_elem_t g_bcm968560bob[] = {
  {bp_cpBoardId,               .u.cp = "968560BOB"},
  MOVED_DT({bp_usExtIntrPmdAlarm,       .u.us = BP_EXT_INTR_2 | BP_EXT_INTR_TYPE_IRQ_HIGH_LEVEL | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_10_AH},)
  {bp_usGpioPmdReset,          .u.us = BP_GPIO_21_AL},
  MOVED_DT({bp_usExtIntrOpticalModulePresence, .u.us = BP_EXT_INTR_3 | BP_EXT_INTR_TYPE_IRQ_BOTH_EDGE | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_23_AH},)
  MOVED_DT({bp_usPmdMACEwakeEn,         .u.us = BP_GPIO_24_AH},)
  MOVED_DT({bp_InvSerdesTxPol,          .u.us = pmd_polarity_invert},)
  MOVED_DT({bp_pmdFunc,                 .u.us = BP_PMD_APD_TYPE_FLYBACK | BP_PMD_APD_REG_ENABLED},)
  {bp_elemTemplate,            .u.bp_elemp = g_bcm968560ref},
  {bp_last}
};

static bp_elem_t g_bcm968560bob_4g[] = {
  {bp_cpBoardId,               .u.cp = "968560BOB_4G"},
  MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,               .u.ul = 0x0f},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0x01 | MAC_IF_GMII},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 0x02 | MAC_IF_GMII},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 0x03 | MAC_IF_GMII},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 0x04 | MAC_IF_GMII},)
  {bp_elemTemplate,            .u.bp_elemp = g_bcm968560bob},
  {bp_last}
};

static bp_elem_t g_bcm968560bob_4gsff[] = {
  {bp_cpBoardId,               .u.cp = "968560BOB_4GSFF"},
  MOVED_DT({bp_InvSerdesTxPol,          .u.us = BP_NOT_DEFINED},)
  {bp_elemTemplate,            .u.bp_elemp = g_bcm968560bob_4g},
  {bp_last}
};

static bp_elem_t g_bcm968560ref_pci2[] = {
  {bp_cpBoardId,               .u.cp = "968560REF_PCI2"},
  MOVED_DT({bp_ulPciFlags,              .u.ul =  BP_PCI0_DUAL_LANE},)
  {bp_elemTemplate,            .u.bp_elemp = g_bcm968560ref},
  {bp_last}
};

static bp_elem_t g_bcm968560xsv21[] = {
  {bp_cpBoardId,               .u.cp = "968560XSV21"},
  MOVED_DT({bp_usI2sSdata,              .u.us = BP_GPIO_0_AH},)
  MOVED_DT({bp_usI2sSclk,               .u.us = BP_GPIO_1_AH},)
  MOVED_DT({bp_usI2sLrck,               .u.us = BP_GPIO_2_AH},)
  MOVED_DT({bp_usUart1Sdin,             .u.us = BP_GPIO_3_AH},)
  MOVED_DT({bp_usGpioUart2Cts,          .u.us = BP_GPIO_4_AH},)
  MOVED_DT({bp_usGpioUart2Rts,          .u.us = BP_GPIO_5_AH},)
  MOVED_DT({bp_usUart1Sdout,            .u.us = BP_GPIO_6_AH},)
  MOVED_DT({bp_usGpioI2cSda,            .u.us = BP_GPIO_19_AL},)
  MOVED_DT({bp_usGpioI2cScl,            .u.us = BP_GPIO_20_AL},)
  MOVED_DT({bp_usGpioUart2Sdout,        .u.us = BP_GPIO_25_AL},)
  MOVED_DT({bp_usSerialLedData,         .u.us = BP_GPIO_26_AH},)
  MOVED_DT({bp_usSerialLedClk,          .u.us = BP_GPIO_27_AH},)
  MOVED_DT({bp_usSerialLedMask,         .u.us = BP_GPIO_28_AH},)
  MOVED_DT({bp_usPcmSdin,               .u.us = BP_GPIO_45_AH},)
  MOVED_DT({bp_usPcmSdout,              .u.us = BP_GPIO_46_AH},)
  MOVED_DT({bp_usPcmClk,                .u.us = BP_GPIO_47_AH},)
  MOVED_DT({bp_usPcmFs,                 .u.us = BP_GPIO_75_AH},)
  MOVED_DT({bp_usMiiMdc,                .u.us = BP_GPIO_60_AH},)
  MOVED_DT({bp_usMiiMdio,               .u.us = BP_GPIO_61_AH},)
  MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,               .u.ul = 0x3f},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0x01 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_3_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_4_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 0x02 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_5_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_6_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 0x03 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_7_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_8_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 0x04 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_9_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_10_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId4,                .u.ul = 0x07 | MAC_IF_RGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_TX_INTERNAL_DELAY},)
  MOVED_DT({bp_ulPhyId5,                .u.ul = 0x1e | MAC_IF_HSGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_0_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_1_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_2_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_12_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_13_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_14_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_15_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_16_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_17_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_18_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_19_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_20_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_21_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_22_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_23_AL},)
  MOVED_DT({bp_usUsbPwrFlt0,            .u.us = BP_GPIO_76_AL},)
  MOVED_DT({bp_usUsbPwrOn0,             .u.us = BP_GPIO_77_AL},)
  MOVED_DT({bp_usUsbPwrFlt1,            .u.us = BP_GPIO_78_AL},)
  MOVED_DT({bp_usUsbPwrOn1,             .u.us = BP_GPIO_79_AL},)
  MOVED_DT({bp_ulMemoryConfig,          .u.ul = BP_DDR_SPEED_1067_14_14_14 | BP_DDR_TOTAL_SIZE_1024MB| BP_DDR_DEVICE_WIDTH_16 | BP_DDR_TOTAL_WIDTH_32BIT | BP_DDR_SSC_CONFIG_1},)
  {bp_ucDspType0,              .u.uc = BP_VOIP_DSP},
  {bp_ucDspAddress,            .u.uc = 0},
  {bp_last}
};

static bp_elem_t g_bcm968360bsff[] = {
  {bp_cpBoardId,               .u.cp = "968360BSFF"},
  MOVED_DT({bp_InvSerdesRxPol,          .u.us = BP_NOT_DEFINED},)
  MOVED_DT({bp_InvSerdesTxPol,          .u.us = BP_NOT_DEFINED},)
  {bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_24_AL},
  MOVED_DT({bp_usExtIntrOpticalModulePresence, .u.us = BP_EXT_INTR_3 | BP_EXT_INTR_TYPE_IRQ_BOTH_EDGE | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_23_AH},)
  MOVED_DT({bp_usExtIntrPmdAlarm,       .u.us = BP_EXT_INTR_NONE},)
  MOVED_DT({bp_usGpio_Intr,            .u.us = BP_GPIO_10_AH},)
  MOVED_DT({bp_usRogueOnuEn,            .u.us = BP_GPIO_10_AH},)
  MOVED_DT({bp_usGpioOpticalModuleTxPwrDown, .u.us = BP_GPIO_9_AH},)
  MOVED_DT({bp_pmdFunc,                 .u.us = BP_NOT_DEFINED},)
  {bp_elemTemplate,            .u.bp_elemp = g_bcm968360bg},
  {bp_last}
};

static bp_elem_t g_bcm968360bsfp[] = {
  {bp_cpBoardId,               .u.cp = "968360BSFP"},
  MOVED_DT({bp_InvSerdesRxPol,          .u.us = pmd_polarity_invert},)
  MOVED_DT({bp_usExtIntrOpticalModulePresence, .u.us = BP_EXT_INTR_3 | BP_EXT_INTR_TYPE_IRQ_BOTH_EDGE | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_23_AL},)
  {bp_elemTemplate,            .u.bp_elemp = g_bcm968360bsff},
  {bp_last}
};

static bp_elem_t g_bcm968360b_pci0x2[] = {
  {bp_cpBoardId,               .u.cp = "968360BG_PCI0X2"},
  MOVED_DT({bp_ulPciFlags,              .u.ul =  BP_PCI0_DUAL_LANE},)
  {bp_elemTemplate,            .u.bp_elemp = g_bcm968360bg},
  {bp_last}
};


static bp_elem_t g_bcm968360bsff_pci0x2[] = {
  {bp_cpBoardId,               .u.cp = "968360BSF_PCIX2"},
  MOVED_DT({bp_ulPciFlags,              .u.ul =  BP_PCI0_DUAL_LANE},)
  {bp_elemTemplate,            .u.bp_elemp = g_bcm968360bsff},
  {bp_last}
};

static bp_elem_t g_bcm968360b_4g[] = {
  {bp_cpBoardId,               .u.cp = "968360BG_4G"},
  MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,               .u.ul = 0x0f},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0x01 | MAC_IF_GMII},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 0x02 | MAC_IF_GMII},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 0x03 | MAC_IF_GMII},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 0x04 | MAC_IF_GMII},)
  {bp_elemTemplate,            .u.bp_elemp = g_bcm968360bg},
  {bp_last}
};

static bp_elem_t g_bcm968360b_4gsff[] = {
    {bp_cpBoardId,               .u.cp = "968360BG_4GSFF"},
    MOVED_DT({bp_InvSerdesTxPol,          .u.us = BP_NOT_DEFINED},)
    {bp_elemTemplate,            .u.bp_elemp = g_bcm968360b_4g},
    {bp_last}
};

static bp_elem_t g_bcm968360bg2[] = {
  {bp_cpBoardId,               .u.cp = "968360BG2"},
  MOVED_DT({bp_InvSerdesRxPol,          .u.us = pmd_use_def_polarity},)
  MOVED_DT({bp_InvSerdesTxPol,          .u.us = pmd_use_def_polarity},)
  MOVED_DT({bp_usExtIntrOpticalModulePresence, .u.us = BP_EXT_INTR_3 | BP_EXT_INTR_TYPE_IRQ_BOTH_EDGE | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_23_AH},)
  MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,               .u.ul = 0x3f},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0x01 | MAC_IF_GMII},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 0x02 | MAC_IF_GMII},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 0x03 | MAC_IF_GMII},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 0x04 | MAC_IF_GMII},)
  MOVED_DT({bp_ulPhyId4,                .u.ul = 0x07 | MAC_IF_RGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_TX_INTERNAL_DELAY},)
  MOVED_DT({bp_ulPhyId5,                .u.ul = 0x1e | MAC_IF_HSGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  {bp_elemTemplate,            .u.bp_elemp = g_bcm968360bsfp},
  {bp_last}
};

static bp_elem_t g_bcm94910ref[] = {
    {bp_cpBoardId,               .u.cp = "94910REF"},
    MOVED_DT({bp_usI2sSdata,              .u.us = BP_GPIO_0_AH},)
    MOVED_DT({bp_usI2sSclk,               .u.us = BP_GPIO_1_AH},)
    MOVED_DT({bp_usI2sLrck,               .u.us = BP_GPIO_2_AH},)
    MOVED_DT({bp_usGpioI2cSda,            .u.us = BP_GPIO_19_AL},)
    MOVED_DT({bp_usGpioI2cScl,            .u.us = BP_GPIO_20_AL},)
    MOVED_DT({bp_usSerialLedData,         .u.us = BP_GPIO_26_AH},)
    MOVED_DT({bp_usSerialLedClk,          .u.us = BP_GPIO_27_AH},)
    MOVED_DT({bp_usSerialLedMask,         .u.us = BP_GPIO_28_AH},)
    MOVED_DT({bp_usPcmSdin,               .u.us = BP_GPIO_45_AH},)
    MOVED_DT({bp_usPcmSdout,              .u.us = BP_GPIO_46_AH},)
    MOVED_DT({bp_usPcmClk,                .u.us = BP_GPIO_47_AH},)
    MOVED_DT({bp_usPcmFs,                 .u.us = BP_GPIO_75_AH},)
    MOVED_DT({bp_usMiiMdc,                .u.us = BP_GPIO_60_AH},)
    MOVED_DT({bp_usMiiMdio,               .u.us = BP_GPIO_61_AH},)
    MOVED_DT({bp_usExtIntrSesBtnWireless, .u.us = BP_EXT_INTR_0 | BP_EXT_INTR_TYPE_IRQ_LOW_LEVEL | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
    MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_83_AL},)
    MOVED_DT({bp_usExtIntrResetToDefault, .u.us = BP_EXT_INTR_1 | BP_EXT_INTR_TYPE_IRQ_LOW_LEVEL | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
    MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_82_AL},)
    MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_0_AL},)
    MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_1_AL},)
    MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_2_AL},)
    MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_12_AL},)
    MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_13_AL},)
    MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_14_AL},)
    MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_15_AL},)
    MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_16_AL},)
    MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_17_AL},)
    MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_18_AL},)
    MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_19_AL},)
    MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_20_AL},)
    MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_21_AL},)
    MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_22_AL},)
    MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_23_AL},)
    MOVED_DT({bp_usUsbPwrFlt0,            .u.us = BP_GPIO_76_AL},)
    MOVED_DT({bp_usUsbPwrOn0,             .u.us = BP_GPIO_77_AL},)
    MOVED_DT({bp_usUsbPwrFlt1,            .u.us = BP_GPIO_78_AL},)
    MOVED_DT({bp_usUsbPwrOn1,             .u.us = BP_GPIO_79_AL},)
    MOVED_DT({bp_usSpiSlaveSelectNum,     .u.us = 1},)
    MOVED_DT({bp_usSpiSlaveSelectGpioNum, .u.us = BP_GPIO_81_AH},)
    MOVED_DT({bp_ulMemoryConfig,          .u.ul = BP_DDR_SPEED_1067_14_14_14 | BP_DDR_TOTAL_SIZE_1024MB| BP_DDR_DEVICE_WIDTH_16 | BP_DDR_TOTAL_WIDTH_32BIT | BP_DDR_SSC_CONFIG_1},)
    {bp_ucDspType0,              .u.uc = BP_VOIP_DSP},
    {bp_ucDspAddress,            .u.uc = 0},
    MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
    MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
    MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
    MOVED_DT({bp_ulPortMap,               .u.ul = 0x12f},)
    MOVED_DT({bp_ulPhyId0,                .u.ul = 0x01 | MAC_IF_GMII},)
    MOVED_DT({bp_ulPhyId1,                .u.ul = 0x02 | MAC_IF_GMII},)
    MOVED_DT({bp_ulPhyId2,                .u.ul = 0x03 | MAC_IF_GMII},)
    MOVED_DT({bp_ulPhyId3,                .u.ul = 0x04 | MAC_IF_GMII},)
    MOVED_DT({bp_ulPhyId5,                .u.ul = 0x1e | MAC_IF_HSGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
    MOVED_DT({bp_ulPhyId8,                .u.ul = 0x1c | MAC_IF_XFI | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
    MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_DETECT | PORT_FLAG_WAN_ONLY},)
    {bp_last}
};

static bp_elem_t g_bcm94910ref_pci2[] = {
  {bp_cpBoardId,               .u.cp = "94910REF_PCI2"},
  MOVED_DT({bp_ulPciFlags,              .u.ul =  BP_PCI0_DUAL_LANE},)
  {bp_elemTemplate,            .u.bp_elemp = g_bcm94910ref},
  {bp_last}
};

bp_elem_t * g_BoardParms[] = {g_bcm968560sv, g_bcm968560sv_opt, g_bcm968360bg, g_bcm968360bsff, g_bcm968360b_pci0x2, g_bcm968360bsff_pci0x2, g_bcm968360bsfp, g_bcm968360b_4g, g_bcm968360b_4gsff, g_bcm968560ref, g_bcm968560xsv21, g_bcm968560ref_ng2, g_bcm968560ref_pci2, g_bcm968560bob, g_bcm968560bob_4g, g_bcm968560bob_4gsff, g_bcm968360bg2, g_bcm94910ref, g_bcm94910ref_pci2, 0};

