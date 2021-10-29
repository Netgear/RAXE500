#include "bp_defs.h"
#include "boardparms.h"
#include "bcmSpiRes.h"

#ifdef _CFE_
#define MOVED_DT(x...) x
#else
#define MOVED_DT(x...)
#endif

static bp_elem_t g_bcm968580xsv[] = {
  {bp_cpBoardId,                .u.cp = "968580XSV"},
  MOVED_DT({bp_usPcmSdin,                .u.us = BP_GPIO_0_AH},)
  MOVED_DT({bp_usPcmSdout,               .u.us = BP_GPIO_1_AH},)
  MOVED_DT({bp_usPcmClk,                 .u.us = BP_GPIO_2_AH},)
  MOVED_DT({bp_usPcmFs,                  .u.us = BP_GPIO_3_AH},)
  MOVED_DT({bp_usGpioUart2Sdin,          .u.us = BP_GPIO_16_AH},)
  MOVED_DT({bp_usGpioUart2Sdout,         .u.us = BP_GPIO_17_AH},)
  MOVED_DT({bp_usGpioUart2Cts,           .u.us = BP_GPIO_18_AH},)
  MOVED_DT({bp_usGpioUart2Rts,           .u.us = BP_GPIO_19_AH},)
  MOVED_DT({bp_usGpioI2c2Scl,            .u.us = BP_GPIO_23_AH},)
  MOVED_DT({bp_usGpioI2c2Sda,            .u.us = BP_GPIO_24_AH},)
  MOVED_DT({bp_usSerialLedData,          .u.us = BP_GPIO_29_AH},)
  MOVED_DT({bp_usSerialLedClk,           .u.us = BP_GPIO_30_AH},)
  MOVED_DT({bp_usSerialLedMask,          .u.us = BP_GPIO_31_AH},)
  MOVED_DT({bp_usGpioPonTxEn,            .u.us = BP_GPIO_50_AH},)
  /* NOT USED IN THIS BOARD
  {bp_usSpiSlaveSelectNum,      .u.us = 2},
  {bp_usSpiSlaveSelectGpioNum,  .u.us = 51}, */
  MOVED_DT({bp_usSpiSlaveSelectNum,      .u.us = 3},)
  MOVED_DT({bp_usSpiSlaveSelectGpioNum,  .u.us = 13},)
  MOVED_DT({bp_usSpiSlaveSelectNum,      .u.us = 4},)
  MOVED_DT({bp_usSpiSlaveSelectGpioNum,  .u.us = 12},)
  MOVED_DT({bp_usSpiSlaveSelectNum,      .u.us = 5},)
  MOVED_DT({bp_usSpiSlaveSelectGpioNum,  .u.us = 11},)
  MOVED_DT({bp_usRogueOnuEn,             .u.us = BP_GPIO_56_AH},)
  /* NOT USED IN THIS BOARD 
  {bp_usI2sSdata,               .u.us = BP_GPIO_59_AH},
  {bp_usI2sSclk,                .u.us = BP_GPIO_60_AH},
  {bp_usI2sLrck,                .u.us = BP_GPIO_61_AH},*/
  MOVED_DT({bp_ulInterfaceEnable,        .u.ul = BP_PINMUX_FNTYPE_LPORT},)
  MOVED_DT({bp_usUsbPwrOn0,              .u.us = BP_GPIO_113_AH},)
  MOVED_DT({bp_usUsbPwrFlt0,             .u.us = BP_GPIO_114_AH},)
  MOVED_DT({bp_usUsbPwrOn1,              .u.us = BP_GPIO_115_AH},)
  MOVED_DT({bp_usUsbPwrFlt1,             .u.us = BP_GPIO_116_AH},)
  MOVED_DT({bp_usGpioLedOpticalLinkFail, .u.us = BP_SERIAL_GPIO_5_AL},)
  MOVED_DT({bp_usGpioLedGpon,            .u.us = BP_SERIAL_GPIO_8_AL},)
  MOVED_DT({bp_usGpioLedEpon,            .u.us = BP_SERIAL_GPIO_8_AL},)
  MOVED_DT({bp_usGpioLedWanData,         .u.us = BP_SERIAL_GPIO_2_AL},)
  MOVED_DT({bp_ucPhyType0,               .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,             .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,             .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,                .u.ul = 0x0f},)
  MOVED_DT({bp_ulPhyId0,                 .u.ul = 0x01 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,                .u.us = BP_SERIAL_GPIO_0_AL},)
  MOVED_DT({bp_ulNetLedLink,             .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,                .u.us = BP_SERIAL_GPIO_1_AL},)
  MOVED_DT({bp_ulNetLedActivity,         .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed3,                .u.us = BP_SERIAL_GPIO_21_AL},)
  MOVED_DT({bp_ulNetLedLink,             .u.ul = BP_NET_LED_SPEED_ALL},)
  MOVED_DT({bp_ulPhyId1,                 .u.ul = 0x02 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,                .u.us = BP_SERIAL_GPIO_3_AL},)
  MOVED_DT({bp_ulNetLedLink,             .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,                .u.us = BP_SERIAL_GPIO_4_AL},)
  MOVED_DT({bp_ulNetLedActivity,         .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed3,                .u.us = BP_SERIAL_GPIO_22_AL},)
  MOVED_DT({bp_ulNetLedLink,             .u.ul = BP_NET_LED_SPEED_ALL},)
  MOVED_DT({bp_ulPhyId2,                 .u.ul = 0x03 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,                .u.us = BP_SERIAL_GPIO_6_AL},)
  MOVED_DT({bp_ulNetLedLink,             .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,                .u.us = BP_SERIAL_GPIO_7_AL},)
  MOVED_DT({bp_ulNetLedActivity,         .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed3,                .u.us = BP_SERIAL_GPIO_23_AL},)
  MOVED_DT({bp_ulNetLedLink,             .u.ul = BP_NET_LED_SPEED_ALL},)
  MOVED_DT({bp_ulPhyId3,                 .u.ul = 0x04 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,                .u.us = BP_SERIAL_GPIO_9_AL},)
  MOVED_DT({bp_ulNetLedLink,             .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,                .u.us = BP_SERIAL_GPIO_10_AL},)
  MOVED_DT({bp_ulNetLedActivity,         .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed3,                .u.us = BP_SERIAL_GPIO_24_AL},)
  MOVED_DT({bp_ulNetLedLink,             .u.ul = BP_NET_LED_SPEED_ALL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_11_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_12_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_13_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_14_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_15_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_16_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_17_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_18_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_19_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_20_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_21_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_22_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_24_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_23_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_25_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_26_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_27_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_28_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_29_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_30_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_31_AL},)
  {bp_ucDspType0,               .u.uc = BP_VOIP_DSP},
  {bp_ucDspAddress,             .u.uc = 0},
  {bp_last}
};

static bp_elem_t g_bcm968580xsv_rgmii_phy[] = {
  {bp_cpBoardId,               .u.cp = "968580XSV_RGPHY"},
  MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,               .u.ul = 0x7f},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0x01 | MAC_IF_GMII},)
  MOVED_DT({bp_usOamIndex,              .u.us = 0},)
  MOVED_DT({bp_ucPhyDevName,            .u.cp = "eth0"},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 0x02 | MAC_IF_GMII},)
  MOVED_DT({bp_usOamIndex,              .u.us = 1},)
  MOVED_DT({bp_ucPhyDevName,            .u.cp = "eth1"},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 0x03 | MAC_IF_GMII},)
  MOVED_DT({bp_usOamIndex,              .u.us = 2},)
  MOVED_DT({bp_ucPhyDevName,            .u.cp = "eth2"},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 0x04 | MAC_IF_GMII},)
  MOVED_DT({bp_usOamIndex,              .u.us = 3},)
  MOVED_DT({bp_ucPhyDevName,            .u.cp = "eth3"},)
  MOVED_DT({bp_ulPhyId4,                .u.ul = 0x05 | MAC_IF_RGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_TX_INTERNAL_DELAY},)
  MOVED_DT({bp_usOamIndex,              .u.us = 6},)
  MOVED_DT({bp_ucPhyDevName,            .u.cp = "eth6"},)
  MOVED_DT({bp_ulPhyId5,                .u.ul = 0x07 | MAC_IF_RGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_TX_INTERNAL_DELAY},)
  MOVED_DT({bp_usOamIndex,              .u.us = 4},)
  MOVED_DT({bp_ucPhyDevName,            .u.cp = "eth4"},)
  MOVED_DT({bp_ulPhyId6,                .u.ul = 0x06 | MAC_IF_RGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_TX_INTERNAL_DELAY},)
  MOVED_DT({bp_usOamIndex,              .u.us = 5},)
  MOVED_DT({bp_ucPhyDevName,            .u.cp = "eth5"},)
  {bp_elemTemplate,            .u.bp_elemp = g_bcm968580xsv},
  {bp_last}
};

static bp_elem_t g_bcm968580xsv_sgmii_phy[] = {
  {bp_cpBoardId,               .u.cp = "968580XSV_SGPHY"},
  MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,               .u.ul = 0xff},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0x01 | MAC_IF_GMII},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 0x02 | MAC_IF_GMII},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 0x03 | MAC_IF_GMII},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 0x04 | MAC_IF_GMII},)
  MOVED_DT({bp_ulPhyId4,                .u.ul = 0x1c | MAC_IF_SGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPhyId5,                .u.ul = 0x1a | MAC_IF_SGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPhyId6,                .u.ul = 0x16 | MAC_IF_SGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPhyId7,                .u.ul = 0x1e | MAC_IF_SGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  {bp_elemTemplate,            .u.bp_elemp = g_bcm968580xsv},
  {bp_last}
};

static bp_elem_t g_bcm968580xsv_hsgmii_phy[] = {
  {bp_cpBoardId,               .u.cp = "968580XSV_HSG"},
  MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,               .u.ul = 0xff},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0x01 | MAC_IF_GMII},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 0x02 | MAC_IF_GMII},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 0x03 | MAC_IF_GMII},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 0x04 | MAC_IF_GMII},)
  MOVED_DT({bp_ulPhyId4,                .u.ul = 0x1c | MAC_IF_HSGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPhyId5,                .u.ul = 0x1a | MAC_IF_HSGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPhyId6,                .u.ul = 0x16 | MAC_IF_HSGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPhyId7,                .u.ul = 0x1e | MAC_IF_HSGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  {bp_elemTemplate,            .u.bp_elemp = g_bcm968580xsv},
  {bp_last}
};

static bp_elem_t g_bcm968580xsv_sgmii_opt[] = {
  {bp_cpBoardId,               .u.cp = "968580XSV_SGOPT"},
  MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,               .u.ul = 0xff},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0x01 | MAC_IF_GMII},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 0x02 | MAC_IF_GMII},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 0x03 | MAC_IF_GMII},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 0x04 | MAC_IF_GMII},)
  MOVED_DT({bp_ulPhyId4,                .u.ul = 0x04 | MAC_IF_SGMII},)
  MOVED_DT({bp_ulPhyId5,                .u.ul = 0x05 | MAC_IF_SGMII},)
  MOVED_DT({bp_ulPhyId6,                .u.ul = 0x06 | MAC_IF_SGMII},)
  MOVED_DT({bp_ulPhyId7,                .u.ul = 0x07 | MAC_IF_SGMII},)
  MOVED_DT({bp_usSFPSerdesSIGDET0,      .u.us = BP_GPIO_57_AH},)
  MOVED_DT({bp_usSFPSerdesSIGDET1,      .u.us = BP_GPIO_58_AH},)
  MOVED_DT({bp_usSFPSerdesSIGDET2,      .u.us = BP_GPIO_59_AH},)
  MOVED_DT({bp_usSFPSerdesSIGDET3,      .u.us = BP_GPIO_60_AH},)
  MOVED_DT({bp_usSFPSerdesMODDEF0,      .u.us = BP_GPIO_24_AH},)
  MOVED_DT({bp_usSFPSerdesMODDEF1,      .u.us = BP_GPIO_25_AH},)
  MOVED_DT({bp_usSFPSerdesMODDEF2,      .u.us = BP_GPIO_26_AH},)
  MOVED_DT({bp_usSFPSerdesMODDEF3,      .u.us = BP_GPIO_27_AH},)
  {bp_elemTemplate,            .u.bp_elemp = g_bcm968580xsv},
  {bp_last}
};

static bp_elem_t g_bcm968580xsv_xfi[] = {
  {bp_cpBoardId,               .u.cp = "968580XSV_XFI"},
  MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,               .u.ul = 0x3f},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0x01 | MAC_IF_GMII},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 0x02 | MAC_IF_GMII},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 0x03 | MAC_IF_GMII},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 0x04 | MAC_IF_GMII},)
  MOVED_DT({bp_ulPhyId4,                .u.ul = 0x04 | MAC_IF_XFI},)
  MOVED_DT({bp_ulPhyId5,                .u.ul = 0x07 | MAC_IF_RGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_TX_INTERNAL_DELAY},)
  MOVED_DT({bp_usGpioTxDis1,            .u.us = BP_GPIO_52_AH},)
  MOVED_DT({bp_usSFPSerdesSIGDET1,      .u.us = BP_GPIO_58_AH},)
  MOVED_DT({bp_usSFPSerdesMODDEF1,      .u.us = BP_GPIO_25_AH},)
  {bp_elemTemplate,            .u.bp_elemp = g_bcm968580xsv},
  {bp_last}
};

static bp_elem_t g_bcm968580xref[] = {
  {bp_cpBoardId,                .u.cp = "968580XREF"},
  MOVED_DT({bp_usPcmSdin,                .u.us = BP_GPIO_0_AH},)
  MOVED_DT({bp_usPcmSdout,               .u.us = BP_GPIO_1_AH},)
  MOVED_DT({bp_usPcmClk,                 .u.us = BP_GPIO_2_AH},)
  MOVED_DT({bp_usPcmFs,                  .u.us = BP_GPIO_3_AH},)
  MOVED_DT({bp_usGpioUart2Sdin,          .u.us = BP_GPIO_16_AH},)
  MOVED_DT({bp_usGpioUart2Sdout,         .u.us = BP_GPIO_17_AH},)
  MOVED_DT({bp_usGpioUart2Cts,           .u.us = BP_GPIO_18_AH},)
  MOVED_DT({bp_usGpioUart2Rts,           .u.us = BP_GPIO_19_AH},)
  MOVED_DT({bp_usGpioI2c2Scl,            .u.us = BP_GPIO_23_AH},)
  MOVED_DT({bp_usGpioI2c2Sda,            .u.us = BP_GPIO_24_AH},)
  MOVED_DT({bp_usSerialLedData,          .u.us = BP_GPIO_29_AH},)
  MOVED_DT({bp_usSerialLedClk,           .u.us = BP_GPIO_30_AH},)
  MOVED_DT({bp_usSerialLedMask,          .u.us = BP_GPIO_31_AH},)
  MOVED_DT({bp_usGpioWanSignalDetected,  .u.us = BP_GPIO_49_AH},)
  MOVED_DT({bp_usGpioPonTxEn,            .u.us = BP_GPIO_50_AH},)
  MOVED_DT({bp_usSpiSlaveSelectNum,      .u.us = 1},)
  MOVED_DT({bp_usSpiSlaveSelectGpioNum,  .u.us = 106},)
  /*NOT USED IN THIS BOARD
  {bp_usSpiSlaveSelectNum,      .u.us = 2},
  {bp_usSpiSlaveSelectGpioNum,  .u.us = 51}, */
  MOVED_DT({bp_usSpiSlaveSelectNum,      .u.us = 3},)
  MOVED_DT({bp_usSpiSlaveSelectGpioNum,  .u.us = 13},)
#ifdef CONFIG_BCM_TIME_SYNC_MODULE
  MOVED_DT({bp_usTsync1pps,              .u.us = BP_GPIO_11_AH | BP_NONGPIO_PIN},)
  MOVED_DT({bp_usGpioTsyncPonUnstable,   .u.us = BP_GPIO_12_AH},)
  MOVED_DT({bp_usUart1Sdin,              .u.us = BP_GPIO_27_AH},) 
  MOVED_DT({bp_usUart1Sdout,             .u.us = BP_GPIO_28_AH},) 
#else
  MOVED_DT({bp_usSpiSlaveSelectNum,      .u.us = 4},)
  MOVED_DT({bp_usSpiSlaveSelectGpioNum,  .u.us = 12},)
  MOVED_DT({bp_usSpiSlaveSelectNum,      .u.us = 5},)
  MOVED_DT({bp_usSpiSlaveSelectGpioNum,  .u.us = 11},)
#endif
  MOVED_DT({bp_usExtIntrOpticalModulePresence, .u.us = BP_EXT_INTR_3 | BP_EXT_INTR_TYPE_IRQ_BOTH_EDGE | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,              .u.us = BP_GPIO_32_AL},)
  MOVED_DT({bp_usGpioOpticalModuleTxPwrDown, .u.us = BP_GPIO_55_AH},)
  MOVED_DT({bp_usExtIntrSesBtnWireless,  .u.us = BP_EXT_INTR_4 | BP_EXT_INTR_TYPE_IRQ_LOW_LEVEL | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,              .u.us = BP_GPIO_53_AL},)
  MOVED_DT({bp_usExtIntrResetToDefault,  .u.us = BP_EXT_INTR_5 | BP_EXT_INTR_TYPE_IRQ_LOW_LEVEL | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,              .u.us = BP_GPIO_54_AL},)
  /*NOT USED IN THIS BOARD
  {bp_usI2sSdata,               .u.us = BP_GPIO_59_AH},
  {bp_usI2sSclk,                .u.us = BP_GPIO_60_AH},
  {bp_usI2sLrck,                .u.us = BP_GPIO_61_AH},*/
  MOVED_DT({bp_ulInterfaceEnable,        .u.ul = BP_PINMUX_FNTYPE_LPORT},)
  MOVED_DT({bp_usUsbPwrOn0,              .u.us = BP_GPIO_113_AL},)
  MOVED_DT({bp_usUsbPwrFlt0,             .u.us = BP_GPIO_114_AL},)
  MOVED_DT({bp_usUsbPwrOn1,              .u.us = BP_GPIO_115_AL},)
  MOVED_DT({bp_usUsbPwrFlt1,             .u.us = BP_GPIO_116_AL},)
  MOVED_DT({bp_usGpioLedOpticalLinkFail, .u.us = BP_SERIAL_GPIO_5_AL},)
  MOVED_DT({bp_usGpioLedGpon,            .u.us = BP_SERIAL_GPIO_8_AL},)
  MOVED_DT({bp_usGpioLedEpon,            .u.us = BP_SERIAL_GPIO_8_AL},)
  MOVED_DT({bp_usGpioLedWanData,         .u.us = BP_SERIAL_GPIO_2_AL},)
  MOVED_DT({bp_usGpioVoip1Led,           .u.us = BP_SERIAL_GPIO_11_AL},)
  MOVED_DT({bp_usGpioVoip2Led,           .u.us = BP_SERIAL_GPIO_14_AL},)
  MOVED_DT({bp_usGpioLedUSB,             .u.us = BP_SERIAL_GPIO_15_AL},)
  MOVED_DT({bp_usGpioLedUSB2,            .u.us = BP_SERIAL_GPIO_16_AL},)
  MOVED_DT({bp_usGpioLedSesWireless,     .u.us = BP_SERIAL_GPIO_17_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_12_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_13_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_18_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_19_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_20_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_21_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_22_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_23_AL},)
  MOVED_DT({bp_ucPhyType0,               .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,             .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,             .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,                .u.ul = 0x0f},)
  MOVED_DT({bp_ulPhyId0,                 .u.ul = 0x01 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,                .u.us = BP_SERIAL_GPIO_0_AL},)
  MOVED_DT({bp_ulNetLedLink,             .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,                .u.us = BP_SERIAL_GPIO_1_AL},)
  MOVED_DT({bp_ulNetLedActivity,         .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId1,                 .u.ul = 0x02 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,                .u.us = BP_SERIAL_GPIO_3_AL},)
  MOVED_DT({bp_ulNetLedLink,             .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,                .u.us = BP_SERIAL_GPIO_4_AL},)
  MOVED_DT({bp_ulNetLedActivity,         .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId2,                 .u.ul = 0x03 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,                .u.us = BP_SERIAL_GPIO_6_AL},)
  MOVED_DT({bp_ulNetLedLink,             .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,                .u.us = BP_SERIAL_GPIO_7_AL},)
  MOVED_DT({bp_ulNetLedActivity,         .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId3,                 .u.ul = 0x04 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,                .u.us = BP_SERIAL_GPIO_9_AL},)
  MOVED_DT({bp_ulNetLedLink,             .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,                .u.us = BP_SERIAL_GPIO_10_AL},)
  MOVED_DT({bp_ulNetLedActivity,         .u.ul = BP_NET_LED_SPEED_GBE},)
  {bp_ucDspType0,               .u.uc = BP_VOIP_DSP},
  {bp_ucDspAddress,             .u.uc = 0},
  {bp_last}
};


static bp_elem_t g_bcm968580xref_old[] = {
  {bp_cpBoardId,               .u.cp = "968580XREF_OLD"},
  MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,               .u.ul = 0x5f},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0x01 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_0_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_1_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 0x02 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_3_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_4_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 0x03 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_6_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_7_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 0x04 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_9_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_10_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId4,                .u.ul = 0x04 | MAC_IF_XFI},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_12_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_10G},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_13_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_10G},)
  MOVED_DT({bp_usGpioTxDis1,            .u.us = BP_GPIO_52_AH},)
  MOVED_DT({bp_usSFPSerdesSIGDET1,      .u.us = BP_GPIO_58_AH},)
  MOVED_DT({bp_usSFPSerdesMODDEF1,      .u.us = BP_GPIO_25_AH},)
  MOVED_DT({bp_ulPhyId6,                .u.ul = 0x19 | MAC_IF_RGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_TX_INTERNAL_DELAY},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_18_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_19_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  {bp_elemTemplate,            .u.bp_elemp = g_bcm968580xref},
  {bp_last}
};

static bp_elem_t g_bcm968580xref_opt[] = {
  {bp_cpBoardId,               .u.cp = "968580XREF_OPT"},
  MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,               .u.ul = 0x7f},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0x01 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_0_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_1_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 0x02 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_3_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_4_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 0x03 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_6_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_7_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 0x04 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_9_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_10_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId4,                .u.ul = 0x04 | MAC_IF_XFI},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_12_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_10G},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_13_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_10G},)
  MOVED_DT({bp_usGpioTxDis1,            .u.us = BP_GPIO_52_AH},)
  MOVED_DT({bp_usSFPSerdesSIGDET1,      .u.us = BP_GPIO_58_AH},)
  MOVED_DT({bp_usSFPSerdesMODDEF1,      .u.us = BP_GPIO_25_AH},)
  MOVED_DT({bp_ulPhyId5,                .u.ul = 0x18 | MAC_IF_RGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_TX_INTERNAL_DELAY},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_15_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_16_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId6,                .u.ul = 0x19 | MAC_IF_RGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_TX_INTERNAL_DELAY},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_18_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_19_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  {bp_elemTemplate,            .u.bp_elemp = g_bcm968580xref},
  {bp_last}
};

static bp_elem_t g_bcm968580xref_phy[] = {
  {bp_cpBoardId,               .u.cp = "968580XREF_PHY"},
  MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,               .u.ul = 0x7f},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0x01 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_0_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_1_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 0x02 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_3_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_4_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 0x03 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_6_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_7_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 0x04 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_9_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_10_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId4,                .u.ul = 0x1e | MAC_IF_XFI | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_SWAP_PAIR},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_12_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_10G},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_13_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_10G},)
  MOVED_DT({bp_ulPhyId5,                .u.ul = 0x18 | MAC_IF_RGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_TX_INTERNAL_DELAY},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_15_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_16_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId6,                .u.ul = 0x19 | MAC_IF_RGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_TX_INTERNAL_DELAY},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_18_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_19_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  {bp_elemTemplate,            .u.bp_elemp = g_bcm968580xref},
  {bp_last}
};

static bp_elem_t g_bcm968580wref[] = {
  {bp_cpBoardId,               .u.cp = "968580WREF"},
MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
MOVED_DT({bp_ulPortMap,               .u.ul = 0x17f},)
MOVED_DT({bp_ulPhyId0,                .u.ul = 0x01 | MAC_IF_GMII},)
MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_0_AL},)
MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_1_AL},)
MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
MOVED_DT({bp_ulPhyId1,                .u.ul = 0x02 | MAC_IF_GMII},)
MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_3_AL},)
MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_4_AL},)
MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
MOVED_DT({bp_ulPhyId2,                .u.ul = 0x03 | MAC_IF_GMII},)
MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_6_AL},)
MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_7_AL},)
MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
MOVED_DT({bp_ulPhyId3,                .u.ul = 0x04 | MAC_IF_GMII},)
MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_9_AL},)
MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_10_AL},)
MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
MOVED_DT({bp_ulPhyId4,                .u.ul = 0x1e | MAC_IF_XFI | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_SWAP_PAIR},)
MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_12_AL},)
MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_10G},)
MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_13_AL},)
MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_10G},)
MOVED_DT({bp_ulPhyId5,                .u.ul = 0x18 | MAC_IF_RGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_TX_INTERNAL_DELAY},)
MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_15_AL},)
MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_16_AL},)
MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
MOVED_DT({bp_ulPhyId6,                .u.ul = 0x19 | MAC_IF_RGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_TX_INTERNAL_DELAY},)
MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_18_AL},)
MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_19_AL},)
MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
MOVED_DT({bp_ulPhyId8,                .u.ul = 0x1b | MAC_IF_XFI | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_SWAP_PAIR | PORT_FLAG_DETECT | PORT_FLAG_WAN_ONLY},)
  MOVED_DT({bp_usRogueOnuEn,             .u.us = BP_GPIO_56_AH},)
  {bp_elemTemplate,            .u.bp_elemp = g_bcm968580xref},
  {bp_last}
};

static bp_elem_t g_bcm968580xref_pcix2[] = {
  {bp_cpBoardId,               .u.cp = "968580XREF_PCI2"},
  MOVED_DT({bp_ulPciFlags,              .u.ul =  BP_PCI0_DUAL_LANE},)
  {bp_elemTemplate,            .u.bp_elemp = g_bcm968580xref},
  {bp_last}
};

static bp_elem_t g_bcm968580xref_p400[] = {
  {bp_cpBoardId,                .u.cp = "968580XREF_P400"},
  MOVED_DT({bp_usPcmSdin,                .u.us = BP_GPIO_0_AH},)
  MOVED_DT({bp_usPcmSdout,               .u.us = BP_GPIO_1_AH},)
  MOVED_DT({bp_usPcmClk,                 .u.us = BP_GPIO_2_AH},)
  MOVED_DT({bp_usPcmFs,                  .u.us = BP_GPIO_3_AH},)
  MOVED_DT({bp_usGpioUart2Sdin,          .u.us = BP_GPIO_16_AH},)
  MOVED_DT({bp_usGpioUart2Sdout,         .u.us = BP_GPIO_17_AH},)
  MOVED_DT({bp_usGpioUart2Cts,           .u.us = BP_GPIO_18_AH},)
  MOVED_DT({bp_usGpioUart2Rts,           .u.us = BP_GPIO_19_AH},)
  MOVED_DT({bp_usGpioI2c2Scl,            .u.us = BP_GPIO_23_AH},)
  MOVED_DT({bp_usGpioI2c2Sda,            .u.us = BP_GPIO_24_AH},)
  MOVED_DT({bp_usSerialLedData,          .u.us = BP_GPIO_29_AH},)
  MOVED_DT({bp_usSerialLedClk,           .u.us = BP_GPIO_30_AH},)
  MOVED_DT({bp_usSerialLedMask,          .u.us = BP_GPIO_31_AH},)
  MOVED_DT({bp_usGpioWanSignalDetected,  .u.us = BP_GPIO_49_AH},)
  MOVED_DT({bp_usGpioPonTxEn,            .u.us = BP_GPIO_51_AH},)
  MOVED_DT({bp_usSpiSlaveSelectNum,      .u.us = 1},)
  MOVED_DT({bp_usSpiSlaveSelectGpioNum,  .u.us = 106},)
  MOVED_DT({bp_usSpiSlaveSelectNum,      .u.us = 3},)
  MOVED_DT({bp_usSpiSlaveSelectGpioNum,  .u.us = 13},)
#ifdef CONFIG_BCM_TIME_SYNC_MODULE
  MOVED_DT({bp_usTsync1pps,              .u.us = BP_GPIO_11_AH | BP_NONGPIO_PIN},)
  MOVED_DT({bp_usGpioTsyncPonUnstable,   .u.us = BP_GPIO_12_AH},)
  MOVED_DT({bp_usUart1Sdin,              .u.us = BP_GPIO_27_AH},)
  MOVED_DT({bp_usUart1Sdout,             .u.us = BP_GPIO_28_AH},)
#else
  MOVED_DT({bp_usSpiSlaveSelectNum,      .u.us = 4},)
  MOVED_DT({bp_usSpiSlaveSelectGpioNum,  .u.us = 12},)
  MOVED_DT({bp_usSpiSlaveSelectNum,      .u.us = 5},)
  MOVED_DT({bp_usSpiSlaveSelectGpioNum,  .u.us = 11},)
  MOVED_DT({bp_usGpioI2CMux,             .u.us = BP_GPIO_27_AH},)
#endif
  MOVED_DT({bp_usExtIntrOpticalModulePresence, .u.us = BP_EXT_INTR_3 | BP_EXT_INTR_TYPE_IRQ_BOTH_EDGE | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,              .u.us = BP_GPIO_32_AL},)
  MOVED_DT({bp_usGpioOpticalModuleTxPwrDown, .u.us = BP_GPIO_55_AH},)
  MOVED_DT({bp_usExtIntrSesBtnWireless,  .u.us = BP_EXT_INTR_4 | BP_EXT_INTR_TYPE_IRQ_LOW_LEVEL | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,              .u.us = BP_GPIO_53_AL},)
  MOVED_DT({bp_usExtIntrResetToDefault,  .u.us = BP_EXT_INTR_5 | BP_EXT_INTR_TYPE_IRQ_LOW_LEVEL | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,              .u.us = BP_GPIO_54_AL},)
  MOVED_DT({bp_usRogueOnuEn,             .u.us = BP_GPIO_56_AH},)
  /*NOT USED IN THIS BOARD
  {bp_usI2sSdata,               .u.us = BP_GPIO_59_AH},
  {bp_usI2sSclk,                .u.us = BP_GPIO_60_AH},
  {bp_usI2sLrck,                .u.us = BP_GPIO_61_AH},*/
  MOVED_DT({bp_ulInterfaceEnable,        .u.ul = BP_PINMUX_FNTYPE_LPORT},)
  MOVED_DT({bp_usUsbPwrOn0,              .u.us = BP_GPIO_113_AL},)
  MOVED_DT({bp_usUsbPwrFlt0,             .u.us = BP_GPIO_114_AL},)
  MOVED_DT({bp_usUsbPwrOn1,              .u.us = BP_GPIO_115_AL},)
  MOVED_DT({bp_usUsbPwrFlt1,             .u.us = BP_GPIO_116_AL},)
  MOVED_DT({bp_usGpioLedOpticalLinkFail, .u.us = BP_SERIAL_GPIO_5_AL},)
  MOVED_DT({bp_usGpioLedGpon,            .u.us = BP_SERIAL_GPIO_8_AL},)
  MOVED_DT({bp_usGpioLedEpon,            .u.us = BP_SERIAL_GPIO_8_AL},)
  MOVED_DT({bp_usGpioLedWanData,         .u.us = BP_SERIAL_GPIO_2_AL},)
  MOVED_DT({bp_usGpioVoip1Led,           .u.us = BP_SERIAL_GPIO_11_AL},)
  MOVED_DT({bp_usGpioVoip2Led,           .u.us = BP_SERIAL_GPIO_14_AL},)
  MOVED_DT({bp_usGpioLedUSB,             .u.us = BP_SERIAL_GPIO_15_AL},)
  MOVED_DT({bp_usGpioLedUSB2,            .u.us = BP_SERIAL_GPIO_16_AL},)
  MOVED_DT({bp_usGpioLedSesWireless,     .u.us = BP_SERIAL_GPIO_17_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_12_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_13_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_18_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_19_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_20_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_21_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_22_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_23_AL},)
  MOVED_DT({bp_ucPhyType0,               .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,             .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,             .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,                .u.ul = 0x0f},)
  MOVED_DT({bp_ulPhyId0,                 .u.ul = 0x01 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,                .u.us = BP_SERIAL_GPIO_0_AL},)
  MOVED_DT({bp_ulNetLedLink,             .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,                .u.us = BP_SERIAL_GPIO_1_AL},)
  MOVED_DT({bp_ulNetLedActivity,         .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId1,                 .u.ul = 0x02 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,                .u.us = BP_SERIAL_GPIO_3_AL},)
  MOVED_DT({bp_ulNetLedLink,             .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,                .u.us = BP_SERIAL_GPIO_4_AL},)
  MOVED_DT({bp_ulNetLedActivity,         .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId2,                 .u.ul = 0x03 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,                .u.us = BP_SERIAL_GPIO_6_AL},)
  MOVED_DT({bp_ulNetLedLink,             .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,                .u.us = BP_SERIAL_GPIO_7_AL},)
  MOVED_DT({bp_ulNetLedActivity,         .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId3,                 .u.ul = 0x04 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,                .u.us = BP_SERIAL_GPIO_9_AL},)
  MOVED_DT({bp_ulNetLedLink,             .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,                .u.us = BP_SERIAL_GPIO_10_AL},)
  MOVED_DT({bp_ulNetLedActivity,         .u.ul = BP_NET_LED_SPEED_GBE},)
  {bp_ucDspType0,               .u.uc = BP_VOIP_DSP},
  {bp_ucDspAddress,             .u.uc = 0},
  {bp_last}
};
static bp_elem_t g_bcm968580xref_p400_opt[] = {
  {bp_cpBoardId,               .u.cp = "968580XREFP4OPT"},
  MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,               .u.ul = 0x7f},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0x01 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_0_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_1_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 0x02 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_3_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_4_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 0x03 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_6_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_7_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 0x04 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_9_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_10_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId4,                .u.ul = 0x04 | MAC_IF_XFI},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_12_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_10G},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_13_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_10G},)
  MOVED_DT({bp_usGpioTxDis1,            .u.us = BP_GPIO_52_AH},)
  MOVED_DT({bp_usSFPSerdesSIGDET1,      .u.us = BP_GPIO_58_AH},)
  MOVED_DT({bp_usSFPSerdesMODDEF1,      .u.us = BP_GPIO_25_AH},)
  MOVED_DT({bp_ulPhyId5,                .u.ul = 0x18 | MAC_IF_RGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_TX_INTERNAL_DELAY},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_15_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_16_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId6,                .u.ul = 0x19 | MAC_IF_RGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_TX_INTERNAL_DELAY},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_18_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_19_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  {bp_elemTemplate,            .u.bp_elemp = g_bcm968580xref_p400},
  {bp_last}
};

static bp_elem_t g_bcm968580xref_p400_phy[] = {
  {bp_cpBoardId,               .u.cp = "968580XREFP4PHY"},
  MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,               .u.ul = 0x7f},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0x01 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_0_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_1_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 0x02 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_3_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_4_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 0x03 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_6_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_7_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 0x04 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_9_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_10_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId4,                .u.ul = 0x1e | MAC_IF_XFI | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_SWAP_PAIR},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_12_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_10G},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_13_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_10G},)
  MOVED_DT({bp_ulPhyId5,                .u.ul = 0x18 | MAC_IF_RGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_TX_INTERNAL_DELAY},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_15_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_16_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId6,                .u.ul = 0x19 | MAC_IF_RGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_TX_INTERNAL_DELAY},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_18_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_19_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  {bp_elemTemplate,            .u.bp_elemp = g_bcm968580xref_p400},
  {bp_last}
};

static bp_elem_t g_bcm968580xref_p500_phy[] = {
  {bp_cpBoardId,               .u.cp = "968580XREFP5PHY"},
  MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,               .u.ul = 0x7f},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0x01 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_0_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_1_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 0x02 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_3_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_4_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 0x03 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_6_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_7_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 0x04 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_9_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_10_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId4,                .u.ul = 0x1f | MAC_IF_XFI | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_SWAP_PAIR},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_12_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_10G},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_13_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_10G},)
  MOVED_DT({bp_ulPhyId5,                .u.ul = 0x18 | MAC_IF_RGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_TX_INTERNAL_DELAY},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_15_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_16_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId6,                .u.ul = 0x19 | MAC_IF_RGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_TX_INTERNAL_DELAY},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_18_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_19_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  {bp_elemTemplate,            .u.bp_elemp = g_bcm968580xref_p400},
  {bp_last}
};

static bp_elem_t g_bcm949508eapax[] = {
  {bp_cpBoardId,               .u.cp = "949508EAPAX"},
  MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,               .u.ul = 0x17},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0x0e | MAC_IF_XFI | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 0x02 | MAC_IF_GMII},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 0x03 | MAC_IF_GMII},)
  MOVED_DT({bp_ulPhyId4,                .u.ul = 0x0f | MAC_IF_XFI | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_usGpioPhyReset,          .u.us = BP_GPIO_4_AL},)
  {bp_elemTemplate,            .u.bp_elemp = g_bcm968580xref},
  {bp_last}
};

static bp_elem_t g_bcm949508eapax_pci2[] = {
  {bp_cpBoardId,               .u.cp = "949508EAPAX2"},
  MOVED_DT({bp_ulPciFlags,              .u.ul =  BP_PCI0_DUAL_LANE},)
  {bp_elemTemplate,            .u.bp_elemp = g_bcm949508eapax},
  {bp_last}
};

static bp_elem_t g_bcm968580xrnd[] = {
  {bp_cpBoardId,                .u.cp = "968580XRND"},
  MOVED_DT({bp_usPcmSdin,                .u.us = BP_GPIO_0_AH},)
  MOVED_DT({bp_usPcmSdout,               .u.us = BP_GPIO_1_AH},)
  MOVED_DT({bp_usPcmClk,                 .u.us = BP_GPIO_2_AH},)
  MOVED_DT({bp_usPcmFs,                  .u.us = BP_GPIO_3_AH},)
  MOVED_DT({bp_usGpioUart2Sdin,          .u.us = BP_GPIO_16_AH},)
  MOVED_DT({bp_usGpioUart2Sdout,         .u.us = BP_GPIO_17_AH},)
  MOVED_DT({bp_usGpioUart2Cts,           .u.us = BP_GPIO_18_AH},)
  MOVED_DT({bp_usGpioUart2Rts,           .u.us = BP_GPIO_19_AH},)
  MOVED_DT({bp_usGpioI2c2Scl,            .u.us = BP_GPIO_23_AH},)
  MOVED_DT({bp_usGpioI2c2Sda,            .u.us = BP_GPIO_24_AH},)
  MOVED_DT({bp_usUart1Sdin,              .u.us = BP_GPIO_27_AH},)
  MOVED_DT({bp_usUart1Sdout,             .u.us = BP_GPIO_28_AH},)
  MOVED_DT({bp_usSerialLedData,          .u.us = BP_GPIO_29_AH},)
  MOVED_DT({bp_usSerialLedClk,           .u.us = BP_GPIO_30_AH},)
  MOVED_DT({bp_usSerialLedMask,          .u.us = BP_GPIO_31_AH},)
  MOVED_DT({bp_usGpioWanSignalDetected,  .u.us = BP_GPIO_49_AH},)
  MOVED_DT({bp_usGpioPonTxEn,            .u.us = BP_GPIO_50_AH},)
  MOVED_DT({bp_usGpioTxDis1,             .u.us = BP_GPIO_52_AH},)
  MOVED_DT({bp_usSFPSerdesSIGDET1,       .u.us = BP_GPIO_58_AH},)
  MOVED_DT({bp_usSFPSerdesMODDEF1,       .u.us = BP_GPIO_25_AH},)
  {bp_usGpioPonMuxOe,           .u.us = BP_GPIO_53_AH},
  {bp_usGpioPonMux0,            .u.us = BP_GPIO_59_AH},
  {bp_usGpioPonMux1,            .u.us = BP_GPIO_60_AH},
  {bp_usSpiSlaveSelectNum,      .u.us = 2},
  {bp_usSpiSlaveSelectGpioNum,  .u.us = 51},
  {bp_usSpiSlaveSelectNum,      .u.us = 3},
  {bp_usSpiSlaveSelectGpioNum,  .u.us = 13},
  {bp_usSpiSlaveSelectNum,      .u.us = 4},
  {bp_usSpiSlaveSelectGpioNum,  .u.us = 12},
  {bp_usSpiSlaveSelectNum,      .u.us = 5},
  {bp_usSpiSlaveSelectGpioNum,  .u.us = 11},
  MOVED_DT({bp_usExtIntrOpticalModulePresence, .u.us = BP_EXT_INTR_3 | BP_EXT_INTR_TYPE_IRQ_BOTH_EDGE | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,              .u.us = BP_GPIO_61_AH},)
  MOVED_DT({bp_usRogueOnuEn,             .u.us = BP_GPIO_56_AH},)
  MOVED_DT({bp_ulInterfaceEnable,        .u.ul = BP_PINMUX_FNTYPE_LPORT},)
  MOVED_DT({bp_usGpioLedOpticalLinkFail, .u.us = BP_SERIAL_GPIO_5_AL},)
  MOVED_DT({bp_usGpioLedGpon,            .u.us = BP_SERIAL_GPIO_8_AL},)
  MOVED_DT({bp_usGpioLedEpon,            .u.us = BP_SERIAL_GPIO_8_AL},)
  MOVED_DT({bp_usGpioLedWanData,         .u.us = BP_SERIAL_GPIO_2_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_6_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_7_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_9_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_10_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_11_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_12_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_13_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_14_AL},)
  MOVED_DT({bp_usGpioLedReserved,        .u.us = BP_SERIAL_GPIO_15_AL},)
  MOVED_DT({bp_ucPhyType0,               .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,             .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,             .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,                .u.ul = 0x13},)
  MOVED_DT({bp_ulPhyId0,                 .u.ul = 0x01 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,                .u.us = BP_SERIAL_GPIO_0_AL},)
  MOVED_DT({bp_ulNetLedLink,             .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,                .u.us = BP_SERIAL_GPIO_1_AL},)
  MOVED_DT({bp_ulNetLedActivity,         .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId1,                 .u.ul = 0x02 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,                .u.us = BP_SERIAL_GPIO_3_AL},)
  MOVED_DT({bp_ulNetLedLink,             .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,                .u.us = BP_SERIAL_GPIO_4_AL},)
  MOVED_DT({bp_ulNetLedActivity,         .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId4,                 .u.ul = 0x04 | MAC_IF_XFI},)
  MOVED_DT({bp_usOamIndex,               .u.us = 2},)
  MOVED_DT({bp_usNetLed0,                .u.us = BP_SERIAL_GPIO_12_AL},)
  MOVED_DT({bp_ulNetLedLink,             .u.ul = BP_NET_LED_SPEED_10G},)
  MOVED_DT({bp_usNetLed1,                .u.us = BP_SERIAL_GPIO_13_AL},)
  MOVED_DT({bp_ulNetLedActivity,         .u.ul = BP_NET_LED_SPEED_10G},)
  {bp_ucDspAddress,             .u.uc = 0},
  MOVED_DT({bp_usI2cDefXponBus,          .u.us = 0x8},)
  {bp_last}
};

static bp_elem_t g_bcm955040sv[] = {
  {bp_cpBoardId,                .u.cp = "955040SV"},
  MOVED_DT({bp_usPcmSdin,                .u.us = BP_GPIO_0_AH},)
  MOVED_DT({bp_usPcmSdout,               .u.us = BP_GPIO_1_AH},)
  MOVED_DT({bp_usPcmClk,                 .u.us = BP_GPIO_2_AH},)
  MOVED_DT({bp_usPcmFs,                  .u.us = BP_GPIO_3_AH},)
  MOVED_DT({bp_usGpioUart2Sdin,          .u.us = BP_GPIO_16_AH},)
  MOVED_DT({bp_usGpioUart2Sdout,         .u.us = BP_GPIO_17_AH},)
  MOVED_DT({bp_usGpioUart2Cts,           .u.us = BP_GPIO_18_AH},)
  MOVED_DT({bp_usGpioUart2Rts,           .u.us = BP_GPIO_19_AH},)
  MOVED_DT({bp_usGpioI2c2Scl,            .u.us = BP_GPIO_23_AH},)
  MOVED_DT({bp_usGpioI2c2Sda,            .u.us = BP_GPIO_24_AH},)
  MOVED_DT({bp_usSerialLedData,          .u.us = BP_GPIO_29_AH},)
  MOVED_DT({bp_usSerialLedClk,           .u.us = BP_GPIO_30_AH},)
  MOVED_DT({bp_usSerialLedMask,          .u.us = BP_GPIO_31_AH},)
  MOVED_DT({bp_usGpioPonTxEn,            .u.us = BP_GPIO_50_AH},)
  {bp_usSpiSlaveSelectNum,      .u.us = 2},
  {bp_usSpiSlaveSelectGpioNum,  .u.us = 51},
  {bp_usSpiSlaveSelectNum,      .u.us = 3},
  {bp_usSpiSlaveSelectGpioNum,  .u.us = 13},
  {bp_usSpiSlaveSelectNum,      .u.us = 4},
  {bp_usSpiSlaveSelectGpioNum,  .u.us = 12},
  {bp_usSpiSlaveSelectNum,      .u.us = 5},
  {bp_usSpiSlaveSelectGpioNum,  .u.us = 11},
  MOVED_DT({bp_usRogueOnuEn,             .u.us = BP_GPIO_56_AH},)
  /*NOT USED IN THIS BOARD
  {bp_usI2sSdata,               .u.us = BP_GPIO_59_AH},
  {bp_usI2sSclk,                .u.us = BP_GPIO_60_AH},
  {bp_usI2sLrck,                .u.us = BP_GPIO_61_AH},*/
  MOVED_DT({bp_ulInterfaceEnable,        .u.ul = BP_PINMUX_FNTYPE_LPORT},)
  MOVED_DT({bp_usUsbPwrOn0,              .u.us = BP_GPIO_113_AH},)
  MOVED_DT({bp_usUsbPwrFlt0,             .u.us = BP_GPIO_114_AH},)
  MOVED_DT({bp_usUsbPwrOn1,              .u.us = BP_GPIO_115_AH},)
  MOVED_DT({bp_usUsbPwrFlt1,             .u.us = BP_GPIO_116_AH},)
  MOVED_DT({bp_usGpioLedOpticalLinkFail, .u.us = BP_SERIAL_GPIO_5_AL},)
  MOVED_DT({bp_usGpioLedGpon,            .u.us = BP_SERIAL_GPIO_8_AL},)
  MOVED_DT({bp_usGpioLedEpon,            .u.us = BP_SERIAL_GPIO_8_AL},)
  MOVED_DT({bp_usGpioLedWanData,         .u.us = BP_SERIAL_GPIO_2_AL},)
  MOVED_DT({bp_ucPhyType0,               .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,             .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,             .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,                .u.ul = 0x10},)
  MOVED_DT({bp_ulPhyId4,                 .u.ul = 0x05 | MAC_IF_RGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPortFlags,              .u.ul = PORT_FLAG_TX_INTERNAL_DELAY},)
  {bp_ucDspType0,               .u.uc = BP_VOIP_DSP},
  {bp_ucDspAddress,             .u.uc = 0},
  {bp_last}
};

static bp_elem_t g_bcm955040sv_hsgmii_phy[] = {
  {bp_cpBoardId,                .u.cp = "955040SV_HSG"},
  MOVED_DT({bp_ucPhyType0,               .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,             .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,             .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,                .u.ul = 0x1f},)
  MOVED_DT({bp_ulPhyId0,                 .u.ul = 0x1e | MAC_IF_HSGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPhyId1,                 .u.ul = 0x1c | MAC_IF_HSGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPhyId2,                 .u.ul = 0x1a | MAC_IF_HSGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPhyId3,                 .u.ul = 0x16 | MAC_IF_HSGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPhyId4,                 .u.ul = 0x05 | MAC_IF_RGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPortFlags,              .u.ul = PORT_FLAG_TX_INTERNAL_DELAY},)
  {bp_elemTemplate,             .u.bp_elemp = g_bcm955040sv},
  {bp_last}
};

static bp_elem_t g_bcm968580xpmd[] = {
  {bp_cpBoardId,               .u.cp = "968580XPMD"},
  MOVED_DT({bp_usExtIntrOpticalModulePresence, .u.us = BP_EXT_INTR_NONE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_NONE},)
  MOVED_DT({bp_InvSerdesTxPol,          .u.us = pmd_polarity_invert},)
  MOVED_DT({bp_usGpioOpticalModuleTxPwrDown, .u.us = BP_GPIO_NONE},)
  MOVED_DT({bp_usPmdMACEwakeEn,         .u.us = BP_GPIO_56_AH},)
  MOVED_DT({bp_usExtIntrPmdAlarm,       .u.us = BP_EXT_INTR_2 | BP_EXT_INTR_TYPE_IRQ_HIGH_LEVEL | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_32_AH},)
  MOVED_DT({bp_usGpioPmdReset,          .u.us = BP_GPIO_50_AL},)
  MOVED_DT({bp_usGpioPonTxEn,           .u.us = BP_GPIO_NONE},)
  MOVED_DT({bp_pmdFunc,                 .u.us = BP_PMD_APD_TYPE_FLYBACK | BP_PMD_APD_REG_ENABLED},)
  {bp_elemTemplate,            .u.bp_elemp = g_bcm968580xref_opt},
  {bp_last}
};

static bp_elem_t g_bcm955045dpu[] = {
  {bp_cpBoardId,                .u.cp = "955045DPU"},
  MOVED_DT({bp_usSpiSlaveBusNum,        .u.us = HS_SPI_BUS_NUM},)
  MOVED_DT({bp_usSpiSlaveMode,          .u.us = SPI_MODE_3},)
  MOVED_DT({bp_ulSpiSlaveCtrlState,     .u.ul = SPI_CONTROLLER_STATE_GATE_CLK_SSOFF},)
  MOVED_DT({bp_ulSpiSlaveMaxFreq,       .u.ul = 20000000},)
  MOVED_DT({bp_usSpiSlaveProtoRev,      .u.us = 2},)
  MOVED_DT({bp_usSpiSlaveSelectNum,     .u.us = 1},)
  MOVED_DT({bp_usGpioSpiSlaveReset,     .u.us = BP_GPIO_52_AL},)
  MOVED_DT({bp_usSpiSlaveSelectNum,     .u.us = 2},)
  MOVED_DT({bp_usGpioSpiSlaveReset,     .u.us = BP_GPIO_6_AL},)
  MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,               .u.ul = 0x2f},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0x00 | MAC_IF_HSGMII},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_ATTACHED},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 0x01 | MAC_IF_HSGMII},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_ATTACHED},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 0x02 | MAC_IF_HSGMII},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_ATTACHED},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 0x03 | MAC_IF_HSGMII},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_ATTACHED},)
  MOVED_DT({bp_ulPhyId5,                .u.ul = 0x07 | MAC_IF_RGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_TX_INTERNAL_DELAY | PORT_FLAG_MGMT},)
  MOVED_DT({bp_ulAttachedIdx,           .u.ul = 0},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 1},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 16 | ATTACHED_FLAG_CONTROL},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 18 | ATTACHED_FLAG_ES},)
  MOVED_DT({bp_ulAttachedIdx,           .u.ul = 1},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 2},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 3},)
  MOVED_DT({bp_ulAttachedIdx,           .u.ul = 2},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 4},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 5},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 17 | ATTACHED_FLAG_CONTROL},)
  MOVED_DT({bp_ulAttachedIdx,           .u.ul = 3},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 6},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 7},)
  MOVED_DT({bp_ulAttachedIdx,           .u.ul = -1},)
  MOVED_DT({bp_usI2cDefXponBus,         .u.us = 0x1},)
  MOVED_DT({bp_usExtIntrOpticalModulePresence, .u.us = BP_EXT_INTR_3 | BP_EXT_INTR_TYPE_IRQ_BOTH_EDGE | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_60_AL},)
  MOVED_DT({bp_usXdResetGpio,           .u.us = BP_GPIO_13_AH},)
  MOVED_DT({bp_cpXdResetName,           .u.cp = "DCXO_PWDN"},)
  MOVED_DT({bp_usXdResetReleaseOnInit,  .u.us = 1},)
  MOVED_DT({bp_usXdGpio,                .u.us = BP_GPIO_12_AH},)
  MOVED_DT({bp_usXdGpioInitValue,       .u.us = 0},)
  MOVED_DT({bp_cpXdGpioInfo,            .u.cp = "70M_CLK_SEL"},)
  MOVED_DT({bp_cpXdGpioInfoValue0,      .u.cp = "XO"},)
  MOVED_DT({bp_cpXdGpioInfoValue1,      .u.cp = "DCXO"},)
  {bp_elemTemplate,            .u.bp_elemp = g_bcm968580xref},
  {bp_last}
};

static bp_elem_t g_bcm955045dpu16[] = {
  {bp_cpBoardId,               .u.cp = "955045DPU16"},
  {bp_cpComment,               .u.cp = "16 line G.fast 106MHz mode"},
  MOVED_DT({bp_ulAttachedIdx,           .u.ul = 0},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 1},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 2},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 3},)
  MOVED_DT({bp_ulPhyId4,                .u.ul = 16 | ATTACHED_FLAG_CONTROL},)
  MOVED_DT({bp_ulPhyId5,                .u.ul = 18 | ATTACHED_FLAG_ES},)
  MOVED_DT({bp_ulAttachedIdx,           .u.ul = 1},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 4},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 5},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 6},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 7},)
  MOVED_DT({bp_ulAttachedIdx,           .u.ul = 2},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 8},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 9},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 10},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 11},)
  MOVED_DT({bp_ulPhyId4,                .u.ul = 17 | ATTACHED_FLAG_CONTROL},)
  MOVED_DT({bp_ulAttachedIdx,           .u.ul = 3},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 12},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 13},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 14},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 15},)
  MOVED_DT({bp_ulAttachedIdx,           .u.ul = -1},)
  {bp_elemTemplate,            .u.bp_elemp = g_bcm955045dpu},
  {bp_last}
};

static bp_elem_t g_bcm965450_424[] = {
  {bp_cpBoardId,               .u.cp = "965450_424"},
  {bp_cpComment,               .u.cp = "8 lines G.fast 424MHz"},
  MOVED_DT({bp_usSpiSlaveBusNum,        .u.us = HS_SPI_BUS_NUM},)
  MOVED_DT({bp_usSpiSlaveMode,          .u.us = SPI_MODE_3},)
  MOVED_DT({bp_ulSpiSlaveCtrlState,     .u.ul = SPI_CONTROLLER_STATE_GATE_CLK_SSOFF},)
  MOVED_DT({bp_ulSpiSlaveMaxFreq,       .u.ul = 20000000},)
  MOVED_DT({bp_usSpiSlaveProtoRev,      .u.us = 2},)
  MOVED_DT({bp_usSpiSlaveSelectNum,     .u.us = 1},)
  MOVED_DT({bp_usGpioSpiSlaveReset,     .u.us = BP_GPIO_52_AL},)
  MOVED_DT({bp_usTsync1pps,             .u.us = BP_GPIO_32_AH | BP_NONGPIO_PIN},)
  MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,               .u.ul = 0x2f},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0x00 | MAC_IF_HSGMII},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_ATTACHED},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 0x01 | MAC_IF_HSGMII},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_ATTACHED},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 0x02 | MAC_IF_HSGMII},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_ATTACHED},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 0x03 | MAC_IF_HSGMII},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_ATTACHED},)
  MOVED_DT({bp_ulPhyId5,                .u.ul = 0x01 | MAC_IF_RGMII_1P8V | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_MGMT},)
  MOVED_DT({bp_ulAttachedIdx,           .u.ul = 0},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 1},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 16 | ATTACHED_FLAG_CONTROL},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 18 | ATTACHED_FLAG_ES},)
  MOVED_DT({bp_ulAttachedIdx,           .u.ul = 1},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 2},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 3},)
  MOVED_DT({bp_ulAttachedIdx,           .u.ul = 2},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 4},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 5},)
  MOVED_DT({bp_ulAttachedIdx,           .u.ul = 3},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 6},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 7},)
  MOVED_DT({bp_ulAttachedIdx,           .u.ul = -1},)
  MOVED_DT({bp_usI2cDefXponBus,         .u.us = 0x1},)
  MOVED_DT({bp_usExtIntrOpticalModulePresence, .u.us = BP_EXT_INTR_3 | BP_EXT_INTR_TYPE_IRQ_BOTH_EDGE | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_60_AL},)
  MOVED_DT({bp_usXdResetGpio,           .u.us = BP_GPIO_13_AH},)
  MOVED_DT({bp_cpXdResetName,           .u.cp = "DCXO_PWDN"},)
  MOVED_DT({bp_usXdResetReleaseOnInit,  .u.us = 1},)
  MOVED_DT({bp_usXdResetGpio,           .u.us = BP_GPIO_55_AH},)
  MOVED_DT({bp_cpXdResetName,           .u.cp = "SFP_PD_RST"},)
  MOVED_DT({bp_usXdResetReleaseOnInit,  .u.us = 1},)
  MOVED_DT({bp_usXdGpio,                .u.us = BP_GPIO_5_AH},)
  MOVED_DT({bp_usXdGpioInitValue,       .u.us = 0},)
  MOVED_DT({bp_cpXdGpioInfo,            .u.cp = "70M_CLK_SEL"},)
  MOVED_DT({bp_cpXdGpioInfoValue0,      .u.cp = "EXT"},)
  MOVED_DT({bp_cpXdGpioInfoValue1,      .u.cp = "DCXO"},)
  MOVED_DT({bp_usXdGpio,                .u.us = BP_GPIO_6_AH},)
  MOVED_DT({bp_usXdGpioInitValue,       .u.us = 1},)
  MOVED_DT({bp_cpXdGpioInfo,            .u.cp = "CPE_DYING_GASP"},)
  MOVED_DT({bp_cpXdGpioInfoValue0,      .u.cp = "Trigger DSL DYING GASP message to CO"},)
  MOVED_DT({bp_cpXdGpioInfoValue1,      .u.cp = "OFF"},)
  {bp_elemTemplate,            .u.bp_elemp = g_bcm968580xref},
  {bp_last}
};

static bp_elem_t g_bcm965450_424_xfi[] = {
  {bp_cpBoardId,               .u.cp = "965450_424_XFI"},
  {bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},
  {bp_ucPhyAddress,            .u.uc = 0x0},
  {bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},
  {bp_ulPortMap,               .u.ul = 0x30},
  {bp_ulPhyId4,                .u.ul = 0x04 | MAC_IF_XFI},
  {bp_ulPortFlags,             .u.ul = PORT_FLAG_ATTACHED},
  {bp_ulPhyId5,                .u.ul = 0x01 | MAC_IF_RGMII_1P8V | PHY_EXTERNAL | PHY_INTEGRATED_VALID},
  {bp_ulPortFlags,             .u.ul = PORT_FLAG_MGMT},
  {bp_ulAttachedIdx,           .u.ul = 4},
  {bp_ulPhyId0,                .u.ul = 0},
  {bp_ulPhyId1,                .u.ul = 1},
  {bp_ulPhyId2,                .u.ul = 2},
  {bp_ulPhyId3,                .u.ul = 3},
  {bp_ulPhyId4,                .u.ul = 4},
  {bp_ulPhyId5,                .u.ul = 5},
  {bp_ulPhyId6,                .u.ul = 6},
  {bp_ulPhyId7,                .u.ul = 7},
  {bp_ulPhyId8,                .u.ul = 16 | ATTACHED_FLAG_CONTROL},
  {bp_ulPhyId9,                .u.ul = 18 | ATTACHED_FLAG_ES},
  {bp_ulAttachedIdx,           .u.ul = -1},
  {bp_elemTemplate,            .u.bp_elemp = g_bcm965450_424},
  {bp_last}
};

static bp_elem_t g_bcm965450_212[] = {
  {bp_cpBoardId,               .u.cp = "965450_212"},
  {bp_cpComment,               .u.cp = "16 lines G.fast 212MHz"},
  MOVED_DT({bp_ulAttachedIdx,           .u.ul = 0},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 1},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 2},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 3},)
  MOVED_DT({bp_ulPhyId4,                .u.ul = 16 | ATTACHED_FLAG_CONTROL},)
  MOVED_DT({bp_ulPhyId5,                .u.ul = 18 | ATTACHED_FLAG_ES},)
  MOVED_DT({bp_ulAttachedIdx,           .u.ul = 1},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 4},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 5},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 6},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 7},)
  MOVED_DT({bp_ulAttachedIdx,           .u.ul = 2},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 8},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 9},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 10},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 11},)
  MOVED_DT({bp_ulAttachedIdx,           .u.ul = 3},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 12},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 13},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 14},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 15},)
  MOVED_DT({bp_ulAttachedIdx,           .u.ul = -1},)
  {bp_elemTemplate,            .u.bp_elemp = g_bcm965450_424},
  {bp_last}
};

static bp_elem_t g_bcm965450_212_xfi[] = {
  {bp_cpBoardId,               .u.cp = "965450_212_XFI"},
  MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,               .u.ul = 0x31},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0x00 | MAC_IF_SGMII},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_ATTACHED},)
  MOVED_DT({bp_ulPhyId4,                .u.ul = 0x04 | MAC_IF_XFI},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_ATTACHED},)
  MOVED_DT({bp_ulPhyId5,                .u.ul = 0x01 | MAC_IF_RGMII_1P8V | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_MGMT},)
  MOVED_DT({bp_ulAttachedIdx,           .u.ul = 0},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 16 | ATTACHED_FLAG_CONTROL},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 18 | ATTACHED_FLAG_ES},)
  MOVED_DT({bp_ulAttachedIdx,           .u.ul = 4},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 1},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 2},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 3},)
  MOVED_DT({bp_ulPhyId4,                .u.ul = 4},)
  MOVED_DT({bp_ulPhyId5,                .u.ul = 5},)
  MOVED_DT({bp_ulPhyId6,                .u.ul = 6},)
  MOVED_DT({bp_ulPhyId7,                .u.ul = 7},)
  MOVED_DT({bp_ulPhyId8,                .u.ul = 8},)
  MOVED_DT({bp_ulPhyId9,                .u.ul = 9},)
  MOVED_DT({bp_ulPhyId10,                .u.ul = 10},)
  MOVED_DT({bp_ulPhyId11,                .u.ul = 11},)
  MOVED_DT({bp_ulPhyId12,                .u.ul = 12},)
  MOVED_DT({bp_ulPhyId13,                .u.ul = 13},)
  MOVED_DT({bp_ulPhyId14,                .u.ul = 14},)
  MOVED_DT({bp_ulPhyId15,                .u.ul = 15},)
  MOVED_DT({bp_ulAttachedIdx,           .u.ul = -1},)
  {bp_elemTemplate,            .u.bp_elemp = g_bcm965450_212},
  {bp_last}
};

bp_elem_t * g_BoardParms[] = {g_bcm968580xsv,           g_bcm968580xsv_rgmii_phy,   g_bcm968580xsv_sgmii_phy, 
                              g_bcm968580xsv_hsgmii_phy,g_bcm968580xsv_sgmii_opt,   g_bcm968580xsv_xfi, 
                              g_bcm968580xref,          g_bcm968580xref_old,        g_bcm968580xref_opt, 
                              g_bcm968580xref_phy,      g_bcm968580wref,            g_bcm968580xref_pcix2,
                              g_bcm968580xref_p400,     g_bcm968580xref_p400_opt,   g_bcm968580xref_p400_phy,
                              g_bcm968580xrnd,          g_bcm955040sv,              g_bcm955040sv_hsgmii_phy,   
                              g_bcm968580xpmd,          g_bcm955045dpu,             g_bcm949508eapax,
                              g_bcm949508eapax_pci2,    g_bcm955045dpu16,
                              g_bcm968580xref_p500_phy, g_bcm965450_424,            g_bcm965450_424_xfi, 
                              g_bcm965450_212,          g_bcm965450_212_xfi,
                              0};



