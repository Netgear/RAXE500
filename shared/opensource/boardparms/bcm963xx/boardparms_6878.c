#include "bp_defs.h"
#include "boardparms.h"
#include "bcmSpiRes.h"

#ifdef _CFE_
#define MOVED_DT(x...) x
#else
#define MOVED_DT(x...)
#endif

static bp_elem_t g_bcm968781ref[] = {
  {bp_cpBoardId,               .u.cp = "968781REF"},
  MOVED_DT({bp_InvSerdesRxPol,          .u.us = pmd_polarity_invert},)
  MOVED_DT({bp_InvSerdesTxPol,          .u.us = pmd_polarity_invert},)
  MOVED_DT({bp_usExtIntrResetToDefault, .u.us = BP_EXT_INTR_1 | BP_EXT_INTR_TYPE_IRQ_LOW_LEVEL | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_53_AL},)
  MOVED_DT({bp_usGpioI2cScl,            .u.us = BP_GPIO_62_AH},)
  MOVED_DT({bp_usGpioI2cSda,            .u.us = BP_GPIO_63_AH},)
  MOVED_DT({bp_ulMemoryConfig,          .u.ul = BP_DDR_SPEED_800_11_11_11 | BP_DDR_TOTAL_SIZE_256MB| BP_DDR_DEVICE_WIDTH_8 | BP_DDR_TOTAL_WIDTH_8BIT | BP_DDR_PCB_2LAYER},)
  MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,               .u.ul = 0x0f},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0x01 | MAC_IF_MII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_GPIO_50_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 0x02 | MAC_IF_MII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_GPIO_51_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 0x03 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_GPIO_47_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 0x04 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_GPIO_42_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usGpioLedWL0Act,         .u.us = BP_GPIO_45_AL},) /* wlan0 led */
  MOVED_DT({bp_usPonLbe,                .u.us = BP_GPIO_61_AH},)
  MOVED_DT({bp_usGpioPmdReset,          .u.us = BP_GPIO_11_AL},)
  MOVED_DT({bp_usPmdMACEwakeEn,         .u.us = BP_GPIO_4_AH},)
  MOVED_DT({bp_usExtIntrPmdAlarm,       .u.us = BP_EXT_INTR_2 | BP_EXT_INTR_TYPE_IRQ_HIGH_LEVEL | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_13_AH},)
  MOVED_DT({bp_pmdFunc,                 .u.us = BP_PMD_APD_TYPE_BOOST | BP_PMD_APD_REG_DISABLED},)
  MOVED_DT({bp_usUsbPwrFlt0,            .u.us = BP_GPIO_66_AL},)
  MOVED_DT({bp_usUsbPwrOn0,             .u.us = BP_GPIO_67_AL},)
  {bp_ucDspType0,              .u.uc = BP_VOIP_DSP},
  {bp_ucDspAddress,            .u.uc = 0},
  {bp_usUsbDis,                .u.us = 0},
  {bp_last}
};

static bp_elem_t g_bcm968781ref_4gphy[] = {
  {bp_cpBoardId,               .u.cp = "968781REF_4GPHY"},
  MOVED_DT({bp_InvSerdesRxPol,          .u.us = pmd_polarity_invert},)
  MOVED_DT({bp_InvSerdesTxPol,          .u.us = pmd_polarity_invert},)
  MOVED_DT({bp_usExtIntrResetToDefault, .u.us = BP_EXT_INTR_1 | BP_EXT_INTR_TYPE_IRQ_LOW_LEVEL | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_53_AL},)
  MOVED_DT({bp_usGpioI2cScl,            .u.us = BP_GPIO_62_AH},)
  MOVED_DT({bp_usGpioI2cSda,            .u.us = BP_GPIO_63_AH},)
  MOVED_DT({bp_ulMemoryConfig,          .u.ul = BP_DDR_SPEED_800_11_11_11 | BP_DDR_TOTAL_SIZE_256MB| BP_DDR_DEVICE_WIDTH_8 | BP_DDR_TOTAL_WIDTH_8BIT | BP_DDR_PCB_2LAYER},)
  MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,               .u.ul = 0x0f},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0x01 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_GPIO_50_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 0x02 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_GPIO_51_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 0x03 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_GPIO_47_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 0x04 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_GPIO_42_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usGpioLedWL0Act,         .u.us = BP_GPIO_45_AL},) /* wlan0 led */
  MOVED_DT({bp_usPonLbe,                .u.us = BP_GPIO_61_AH},)
  MOVED_DT({bp_usGpioPmdReset,          .u.us = BP_GPIO_11_AL},)
  MOVED_DT({bp_usPmdMACEwakeEn,         .u.us = BP_GPIO_4_AH},)
  MOVED_DT({bp_usExtIntrPmdAlarm,       .u.us = BP_EXT_INTR_2 | BP_EXT_INTR_TYPE_IRQ_HIGH_LEVEL | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_13_AH},)
  MOVED_DT({bp_pmdFunc,                 .u.us = BP_PMD_APD_TYPE_BOOST | BP_PMD_APD_REG_DISABLED},)
  MOVED_DT({bp_usUsbPwrFlt0,            .u.us = BP_GPIO_66_AL},)
  MOVED_DT({bp_usUsbPwrOn0,             .u.us = BP_GPIO_67_AL},)
  {bp_ucDspType0,              .u.uc = BP_VOIP_DSP},
  {bp_ucDspAddress,            .u.uc = 0},
  {bp_last}
};

static bp_elem_t g_bcm968781href[] = {
  {bp_cpBoardId,               .u.cp = "968781HREF"},
  {bp_elemTemplate,            .u.bp_elemp = g_bcm968781ref_4gphy},
  {bp_last}
};

static bp_elem_t g_bcm968781xsv[] = {
  {bp_cpBoardId,               .u.cp = "968781XSV"},
  MOVED_DT({bp_usGpioI2cScl,            .u.us = BP_GPIO_62_AH},)
  MOVED_DT({bp_usGpioI2cSda,            .u.us = BP_GPIO_63_AH},)
  MOVED_DT({bp_ulMemoryConfig,          .u.ul = BP_DDR_SPEED_800_11_11_11 | BP_DDR_TOTAL_SIZE_256MB| BP_DDR_DEVICE_WIDTH_8 | BP_DDR_TOTAL_WIDTH_8BIT},)
  MOVED_DT({bp_usMiiMdc,                .u.us = BP_GPIO_52_AH},)
  MOVED_DT({bp_usMiiMdio,               .u.us = BP_GPIO_53_AH},)
  MOVED_DT({bp_usRogueOnuEn,            .u.us = BP_GPIO_13_AH},)
  MOVED_DT({bp_usSerialLedData,         .u.us = BP_GPIO_9_AH},)
  MOVED_DT({bp_usSerialLedClk,          .u.us = BP_GPIO_8_AH},)
  MOVED_DT({bp_usSerialLedMask,         .u.us = BP_GPIO_7_AH},)
  MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,               .u.ul = 0x0f},)
  MOVED_DT({bp_usGpioLedAggregateAct,   .u.us = BP_SERIAL_GPIO_6_AL},)
  MOVED_DT({bp_usGpioLedAggregateLnk,   .u.us = BP_SERIAL_GPIO_7_AL},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0x01 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_8_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_9_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 0x02 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_10_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_11_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 0x03 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_12_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_13_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 0x04 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_14_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_15_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_0_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_1_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_2_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_3_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_3_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_4_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_5_AL},)
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
  MOVED_DT({bp_usPonLbe,                .u.us = BP_GPIO_61_AH},)
  MOVED_DT({bp_usUsbPwrFlt0,            .u.us = BP_GPIO_66_AL},)
  MOVED_DT({bp_usUsbPwrOn0,             .u.us = BP_GPIO_67_AL},)
  MOVED_DT({bp_usSpiSlaveSelectNum,     .u.us = 0},)
  MOVED_DT({bp_usSpiSlaveSelectGpioNum, .u.us = BP_GPIO_59_AH},)
  {bp_ucDspType0,              .u.uc = BP_VOIP_DSP},
  {bp_ucDspAddress,            .u.uc = 0},
  {bp_last}
};

static bp_elem_t g_bcm968781refs[] = {
  {bp_cpBoardId,               .u.cp = "968781REFS"},
  {bp_elemTemplate,            .u.bp_elemp = g_bcm968781ref},
  {bp_last}
};

static bp_elem_t g_bcm968782ref[] = {
  {bp_cpBoardId,               .u.cp = "968782REF"},
  MOVED_DT({bp_usPcmSdin,                .u.us = BP_GPIO_0_AH},)
  MOVED_DT({bp_usPcmSdout,               .u.us = BP_GPIO_1_AH},)
  MOVED_DT({bp_usPcmClk,                 .u.us = BP_GPIO_2_AH},)
  MOVED_DT({bp_usPcmFs,                  .u.us = BP_GPIO_3_AH},)
  MOVED_DT({bp_usGpioLedUSB2,            .u.us = BP_GPIO_8_AH},)
  MOVED_DT({bp_usGpioVoip1Led,           .u.us = BP_GPIO_41_AH},)
  MOVED_DT({bp_usGpioLedUSB,             .u.us = BP_GPIO_44_AH},)
  MOVED_DT({bp_usExtIntrResetToDefault, .u.us = BP_EXT_INTR_1 | BP_EXT_INTR_TYPE_IRQ_LOW_LEVEL | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_53_AL},)
  MOVED_DT({bp_usGpioI2cScl,            .u.us = BP_GPIO_62_AH},)
  MOVED_DT({bp_usGpioI2cSda,            .u.us = BP_GPIO_63_AH},)
  MOVED_DT({bp_ulMemoryConfig,          .u.ul = BP_DDR_SPEED_800_11_11_11 | BP_DDR_TOTAL_SIZE_512MB| BP_DDR_DEVICE_WIDTH_16 | BP_DDR_TOTAL_WIDTH_16BIT | BP_DDR_PCB_2LAYER},)
  MOVED_DT({bp_InvSerdesRxPol,          .u.us = pmd_polarity_invert},)
  MOVED_DT({bp_InvSerdesTxPol,          .u.us = pmd_polarity_invert},)
  MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,               .u.ul = 0x0f},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0x01 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_GPIO_50_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 0x02 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_GPIO_51_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 0x03 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_GPIO_47_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 0x04 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_GPIO_42_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usPmdMACEwakeEn,         .u.us = BP_GPIO_4_AH},)
  MOVED_DT({bp_usExtIntrPmdAlarm,       .u.us = BP_EXT_INTR_2 | BP_EXT_INTR_TYPE_IRQ_HIGH_LEVEL | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_12_AH},)
  MOVED_DT({bp_usGpioPmdReset,          .u.us = BP_GPIO_11_AL},)
  MOVED_DT({bp_pmdFunc,                 .u.us = BP_PMD_APD_TYPE_BOOST | BP_PMD_APD_REG_DISABLED},)
  MOVED_DT({bp_usPonLbe,                .u.us = BP_GPIO_61_AH},)
  MOVED_DT({bp_usGpioWlanReserved,      .u.us = BP_GPIO_16_AH},)
  MOVED_DT({bp_usGpioWlanReserved,      .u.us = BP_GPIO_17_AH},)
  MOVED_DT({bp_usGpioWlanReserved,      .u.us = BP_GPIO_22_AH},)
  MOVED_DT({bp_usGpioWlanReserved,      .u.us = BP_GPIO_23_AH},)
  {bp_ucDspType0,              .u.uc = BP_VOIP_DSP},
  {bp_ucDspAddress,            .u.uc = 0},
  {bp_last}
};

static bp_elem_t g_bcm968782ref2[] = {
  {bp_cpBoardId,               .u.cp = "968782REF2"},
  {bp_elemTemplate,            .u.bp_elemp = g_bcm968782ref},
  {bp_last}
};

static bp_elem_t g_bcm968782gref[] = {
  {bp_cpBoardId,               .u.cp = "968782GREF"},
  MOVED_DT({bp_usPcmSdin,                .u.us = BP_GPIO_0_AH},)
  MOVED_DT({bp_usPcmSdout,               .u.us = BP_GPIO_1_AH},)
  MOVED_DT({bp_usPcmClk,                 .u.us = BP_GPIO_2_AH},)
  MOVED_DT({bp_usPcmFs,                  .u.us = BP_GPIO_3_AH},)
  MOVED_DT({bp_usSerialLedData,          .u.us = BP_GPIO_9_AH},)
  MOVED_DT({bp_usSerialLedClk,           .u.us = BP_GPIO_8_AH},)
  MOVED_DT({bp_usSerialLedMask,          .u.us = BP_GPIO_7_AH},)
  MOVED_DT({bp_usMiiMdc,                 .u.us = BP_GPIO_52_AH},)
  MOVED_DT({bp_usMiiMdio,                .u.us = BP_GPIO_53_AH},)
  MOVED_DT({bp_usGpioI2cScl,             .u.us = BP_GPIO_62_AH},)
  MOVED_DT({bp_usGpioI2cSda,             .u.us = BP_GPIO_63_AH},)
  MOVED_DT({bp_ulMemoryConfig,          .u.ul = BP_DDR_SPEED_800_11_11_11 | BP_DDR_TOTAL_SIZE_512MB| BP_DDR_DEVICE_WIDTH_16 | BP_DDR_TOTAL_WIDTH_16BIT},)
  MOVED_DT({bp_usExtIntrResetToDefault, .u.us = BP_EXT_INTR_0 | BP_EXT_INTR_TYPE_IRQ_LOW_LEVEL | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_60_AL},)
  MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,               .u.ul = 0x1f},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0x01 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_8_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_9_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 0x02 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_10_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_11_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 0x03 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_12_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_13_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 0x04 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_14_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_15_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId4,                .u.ul = 0x07 | MAC_IF_RGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_TX_INTERNAL_DELAY},)
  MOVED_DT({bp_InvSerdesRxPol,          .u.us = pmd_polarity_invert},)
  MOVED_DT({bp_InvSerdesTxPol,          .u.us = pmd_polarity_invert},)
  MOVED_DT({bp_usPmdMACEwakeEn,         .u.us = BP_GPIO_4_AH},)
  MOVED_DT({bp_usExtIntrPmdAlarm,       .u.us = BP_EXT_INTR_1 | BP_EXT_INTR_TYPE_IRQ_HIGH_LEVEL | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_13_AH},)
  MOVED_DT({bp_usGpioPmdReset,          .u.us = BP_GPIO_10_AL},)
  MOVED_DT({bp_pmdFunc,                 .u.us = BP_PMD_APD_TYPE_BOOST | BP_PMD_APD_REG_DISABLED},)
  MOVED_DT({bp_usPonLbe,                .u.us = BP_GPIO_61_AH},)
  MOVED_DT({bp_usUsbPwrFlt0,            .u.us = BP_GPIO_66_AL},)
  MOVED_DT({bp_usUsbPwrOn0,             .u.us = BP_GPIO_67_AL},)
  MOVED_DT({bp_usUsbPwrFlt1,            .u.us = BP_GPIO_68_AL},)
  MOVED_DT({bp_usUsbPwrOn1,             .u.us = BP_GPIO_69_AL},)
  {bp_ucDspType0,              .u.uc = BP_VOIP_DSP},
  {bp_ucDspAddress,            .u.uc = 0},
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_16_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_17_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_18_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_19_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_20_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_21_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_22_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_23_AL},)
  {bp_last}
};

static bp_elem_t g_bcm968782gfm[] = {
  {bp_cpBoardId,               .u.cp = "968782GFM"},
  {bp_elemTemplate,            .u.bp_elemp = g_bcm968782gref},
  {bp_last}
};

static bp_elem_t g_bcm968782xsv[] = {
  {bp_cpBoardId,               .u.cp = "968782XSV"},
  MOVED_DT({bp_usGpioI2cScl,            .u.us = BP_GPIO_62_AH},)
  MOVED_DT({bp_usGpioI2cSda,            .u.us = BP_GPIO_63_AH},)
  MOVED_DT({bp_ulMemoryConfig,          .u.ul = BP_DDR_SPEED_800_11_11_11 | BP_DDR_TOTAL_SIZE_512MB| BP_DDR_DEVICE_WIDTH_16 | BP_DDR_TOTAL_WIDTH_16BIT},)
  MOVED_DT({bp_usMiiMdc,                .u.us = BP_GPIO_52_AH},)
  MOVED_DT({bp_usMiiMdio,               .u.us = BP_GPIO_53_AH},)
  MOVED_DT({bp_usRogueOnuEn,            .u.us = BP_GPIO_13_AH},)
  MOVED_DT({bp_usSerialLedData,         .u.us = BP_GPIO_9_AH},)
  MOVED_DT({bp_usSerialLedClk,          .u.us = BP_GPIO_8_AH},)
  MOVED_DT({bp_usSerialLedMask,         .u.us = BP_GPIO_7_AH},)
  MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,               .u.ul = 0x1f},)
  MOVED_DT({bp_usGpioLedAggregateAct,   .u.us = BP_SERIAL_GPIO_6_AL},)
  MOVED_DT({bp_usGpioLedAggregateLnk,   .u.us = BP_SERIAL_GPIO_7_AL},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0x01 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_8_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_9_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 0x02 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_10_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_11_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 0x03 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_12_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_13_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 0x04 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_14_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_usNetLed1,               .u.us = BP_SERIAL_GPIO_15_AL},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId4,                .u.ul = 0x07 | MAC_IF_RGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_TX_INTERNAL_DELAY},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_0_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_1_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_2_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_3_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_3_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_4_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_5_AL},)
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
  MOVED_DT({bp_usPonLbe,                .u.us = BP_GPIO_61_AH},)
  MOVED_DT({bp_usUsbPwrFlt0,            .u.us = BP_GPIO_66_AL},)
  MOVED_DT({bp_usUsbPwrOn0,             .u.us = BP_GPIO_67_AL},)
  MOVED_DT({bp_usGpioWlanReserved,      .u.us = BP_GPIO_16_AH},)
  MOVED_DT({bp_usGpioWlanReserved,      .u.us = BP_GPIO_17_AH},)
  MOVED_DT({bp_usGpioWlanReserved,      .u.us = BP_GPIO_22_AH},)
  MOVED_DT({bp_usGpioWlanReserved,      .u.us = BP_GPIO_23_AH},)
  MOVED_DT({bp_usSpiSlaveSelectNum,     .u.us = 0},)
  MOVED_DT({bp_usSpiSlaveSelectGpioNum, .u.us = BP_GPIO_59_AH},)
  {bp_ucDspType0,              .u.uc = BP_VOIP_DSP},
  {bp_ucDspAddress,            .u.uc = 0},
  // {bp_usGpioUart2Sdin,         .u.us = BP_GPIO_10_AH},
  // {bp_usGpioUart2Sdout,        .u.us = BP_GPIO_11_AH},
  // {bp_usGpioUart2Cts,          .u.us = BP_GPIO_9_AH},
  // {bp_usGpioUart2Rts,          .u.us = BP_GPIO_12_AH},
  {bp_last}
};

static bp_elem_t g_bcm968782sv2[] = {
  {bp_cpBoardId,               .u.cp = "968782SV2"},
  {bp_usGpioWanSignalDetected, .u.us = BP_GPIO_12_AH},
  {bp_usGpioWlanReserved,      .u.us = BP_GPIO_14_AH},
  {bp_usGpioWlanReserved,      .u.us = BP_GPIO_15_AH},
  {bp_usGpioWlanReserved,      .u.us = BP_GPIO_16_AH},
  {bp_usGpioWlanReserved,      .u.us = BP_GPIO_17_AH},
  {bp_usGpioWlanReserved,      .u.us = BP_GPIO_18_AH},
  {bp_usGpioWlanReserved,      .u.us = BP_GPIO_19_AH},
  {bp_usGpioWlanReserved,      .u.us = BP_GPIO_20_AH},
  {bp_usGpioWlanReserved,      .u.us = BP_GPIO_21_AH},
  {bp_usGpioWlanReserved,      .u.us = BP_GPIO_22_AH},
  {bp_usGpioWlanReserved,      .u.us = BP_GPIO_23_AH},
  {bp_usGpioWlanReserved,      .u.us = BP_GPIO_24_AH},
  {bp_usGpioWlanReserved,      .u.us = BP_GPIO_25_AH},
  {bp_elemTemplate,            .u.bp_elemp = g_bcm968782xsv},
  {bp_last}
};


static bp_elem_t g_bcm968780rfdvt[] = {
  {bp_cpBoardId,               .u.cp = "968780RFDVT"},
  MOVED_DT({bp_usGpioWlanReserved,      .u.us = BP_GPIO_0_AH},)
  MOVED_DT({bp_usGpioWlanReserved,      .u.us = BP_GPIO_1_AH},)
  MOVED_DT({bp_usGpioWlanReserved,      .u.us = BP_GPIO_2_AH},)
  MOVED_DT({bp_usGpioWlanReserved,      .u.us = BP_GPIO_3_AH},)
  MOVED_DT({bp_usGpioWlanReserved,      .u.us = BP_GPIO_14_AH},)
  MOVED_DT({bp_usGpioWlanReserved,      .u.us = BP_GPIO_15_AH},)
  MOVED_DT({bp_usGpioWlanReserved,      .u.us = BP_GPIO_16_AH},)
  MOVED_DT({bp_usGpioWlanReserved,      .u.us = BP_GPIO_17_AH},)
  MOVED_DT({bp_usGpioWlanReserved,      .u.us = BP_GPIO_20_AH},)
  MOVED_DT({bp_usGpioWlanReserved,      .u.us = BP_GPIO_21_AH},)
  MOVED_DT({bp_usGpioWlanReserved,      .u.us = BP_GPIO_22_AH},)
  MOVED_DT({bp_usGpioWlanReserved,      .u.us = BP_GPIO_23_AH},)
  MOVED_DT({bp_usGpioWlanReserved,      .u.us = BP_GPIO_44_AH},)
  {bp_usGpioI2cScl,            .u.us = BP_GPIO_62_AH},
  {bp_usGpioI2cSda,            .u.us = BP_GPIO_63_AH},
  MOVED_DT({bp_ulMemoryConfig,          .u.ul = BP_DDR_SPEED_800_11_11_11 | BP_DDR_TOTAL_SIZE_512MB| BP_DDR_DEVICE_WIDTH_16 | BP_DDR_TOTAL_WIDTH_16BIT},)
  {bp_usSpiSlaveSelectNum,     .u.us = 0},
  {bp_usSpiSlaveSelectGpioNum, .u.us = BP_GPIO_59_AH},
  {bp_usSpiSlaveSelectNum,     .u.us = 1},
  {bp_usSpiSlaveSelectGpioNum, .u.us = BP_GPIO_60_AH},
  MOVED_DT({bp_usUsbPwrFlt0,            .u.us = BP_GPIO_66_AL},)
  MOVED_DT({bp_usUsbPwrOn0,             .u.us = BP_GPIO_67_AL},)
  MOVED_DT({bp_usUsbPwrFlt1,            .u.us = BP_GPIO_68_AL},)
  MOVED_DT({bp_usUsbPwrOn1,             .u.us = BP_GPIO_69_AL},)
  MOVED_DT({bp_usExtIntrResetToDefault, .u.us = BP_EXT_INTR_0 | BP_EXT_INTR_TYPE_IRQ_LOW_LEVEL | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_53_AL},)
  MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,               .u.ul = 0x03},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0x01 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_GPIO_50_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 0x02 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_GPIO_51_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  {bp_ucDspType0,              .u.uc = BP_VOIP_DSP},
  {bp_ucDspAddress,            .u.uc = 0},
  {bp_last}
};

bp_elem_t * g_BoardParms[] = {g_bcm968781ref, g_bcm968781ref_4gphy, g_bcm968781href, g_bcm968781xsv, g_bcm968781refs, g_bcm968782ref, g_bcm968782xsv,g_bcm968780rfdvt, g_bcm968782gref, g_bcm968782ref2, g_bcm968782gfm, g_bcm968782sv2, 0};


