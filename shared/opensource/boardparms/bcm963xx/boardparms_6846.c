#include "bp_defs.h"
#include "boardparms.h"
#include "bcmSpiRes.h"

#ifdef _CFE_
#define MOVED_DT(x...) x
#else
#define MOVED_DT(x...)
#endif

static bp_elem_t g_bcm968460sv[] = {
  {bp_cpBoardId,               .u.cp = "968460SV"},
  MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,               .u.ul = 0x1f},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0x01 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_0_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 0x02 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_1_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 0x03 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_2_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 0x04 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_SERIAL_GPIO_3_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId4,                .u.ul = 0x07 | MAC_IF_RGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_TX_INTERNAL_DELAY},)
  MOVED_DT({bp_usMiiMdc,                .u.us = BP_GPIO_54_AH},)
  MOVED_DT({bp_usMiiMdio,               .u.us = BP_GPIO_55_AH},)
  MOVED_DT({bp_ulMemoryConfig,          .u.ul = BP_DDR_SPEED_800_11_11_11 | BP_DDR_TOTAL_SIZE_256MB| BP_DDR_DEVICE_WIDTH_16 | BP_DDR_TOTAL_WIDTH_16BIT},)
  MOVED_DT({bp_usRogueOnuEn,            .u.us = BP_GPIO_14_AH},)
  MOVED_DT({bp_usPonLbe,                .u.us = BP_GPIO_67_AL},)
  MOVED_DT({bp_usGpioI2cScl,            .u.us = BP_GPIO_68_AH},)
  MOVED_DT({bp_usGpioI2cSda,            .u.us = BP_GPIO_69_AH},)
  MOVED_DT({bp_usUsbPwrFlt0,            .u.us = BP_GPIO_74_AL},)
  MOVED_DT({bp_usUsbPwrOn0,             .u.us = BP_GPIO_75_AL},)
  MOVED_DT({bp_usUsbPwrFlt1,            .u.us = BP_GPIO_76_AL},)
  MOVED_DT({bp_usUsbPwrOn1,             .u.us = BP_GPIO_77_AL},)
  {bp_ucDspType0,              .u.uc = BP_VOIP_DSP},
  {bp_ucDspAddress,            .u.uc = 0},
  MOVED_DT({bp_usGpioUart2Sdin,         .u.us = BP_GPIO_14_AH},)
  MOVED_DT({bp_usGpioUart2Sdout,        .u.us = BP_GPIO_15_AH},)
  {bp_last}
};

static bp_elem_t g_bcm968460ref[] = {
  {bp_cpBoardId,               .u.cp = "968460REF"},
  MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,               .u.ul = 0x0f},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0x01 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_GPIO_52_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 0x02 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_GPIO_53_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 0x03 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_GPIO_49_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 0x04 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_GPIO_44_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulMemoryConfig,          .u.ul = BP_DDR_SPEED_800_11_11_11 | BP_DDR_TOTAL_SIZE_256MB| BP_DDR_DEVICE_WIDTH_16 | BP_DDR_TOTAL_WIDTH_16BIT | BP_DDR_PCB_2LAYER},)
  MOVED_DT({bp_usGpioPonTxEn,           .u.us = BP_GPIO_40_AH},)
  MOVED_DT({bp_usRogueOnuEn,            .u.us = BP_GPIO_14_AH},)
  MOVED_DT({bp_usExtIntrSesBtnWireless, .u.us = BP_EXT_INTR_0 | BP_EXT_INTR_TYPE_IRQ_LOW_LEVEL | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_23_AL},)
  MOVED_DT({bp_usExtIntrResetToDefault, .u.us = BP_EXT_INTR_1 | BP_EXT_INTR_TYPE_IRQ_LOW_LEVEL | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_55_AL},)
  MOVED_DT({bp_usPonLbe,                .u.us = BP_GPIO_67_AL},)
  MOVED_DT({bp_usGpioI2cScl,            .u.us = BP_GPIO_68_AH},)
  MOVED_DT({bp_usGpioI2cSda,            .u.us = BP_GPIO_69_AH},)
  MOVED_DT({bp_usUsbPwrFlt0,            .u.us = BP_GPIO_74_AL},)
  MOVED_DT({bp_usUsbPwrOn0,             .u.us = BP_GPIO_75_AL},)
  MOVED_DT({bp_usUsbPwrFlt1,            .u.us = BP_GPIO_76_AL},)
  MOVED_DT({bp_usUsbPwrOn1,             .u.us = BP_GPIO_77_AL},)
  {bp_ucDspType0,              .u.uc = BP_VOIP_DSP},
  {bp_ucDspAddress,            .u.uc = 0},
  {bp_last}
};

static bp_elem_t g_bcm968460refp[] = {
  {bp_cpBoardId,               .u.cp = "968460REFP"},
  MOVED_DT({bp_InvSerdesRxPol,          .u.us = pmd_polarity_invert},)
  MOVED_DT({bp_InvSerdesTxPol,          .u.us = pmd_polarity_invert},)
  MOVED_DT({bp_usPmdMACEwakeEn,         .u.us = BP_GPIO_4_AH},)
  MOVED_DT({bp_usExtIntrPmdAlarm,       .u.us = BP_EXT_INTR_2 | BP_EXT_INTR_TYPE_IRQ_HIGH_LEVEL | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_14_AH},)
  MOVED_DT({bp_usGpioPmdReset,          .u.us = BP_GPIO_40_AL},)
  MOVED_DT({bp_usGpioPonTxEn,           .u.us = BP_GPIO_NONE},)
  MOVED_DT({bp_pmdFunc,                 .u.us = BP_PMD_APD_REG_DISABLED | BP_PMD_APD_TYPE_BOOST},)
  {bp_elemTemplate,            .u.bp_elemp = g_bcm968460ref},
  {bp_last}
};

static bp_elem_t g_bcm968461prw[] = {
  {bp_cpBoardId,               .u.cp = "968461PRW"},
  MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,               .u.ul = 0x0f},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0x01 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_GPIO_52_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 0x02 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_GPIO_53_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 0x03 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_GPIO_49_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 0x04 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_GPIO_44_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulMemoryConfig,          .u.ul = BP_DDR_SPEED_800_11_11_11 | BP_DDR_TOTAL_SIZE_256MB| BP_DDR_DEVICE_WIDTH_16 | BP_DDR_TOTAL_WIDTH_16BIT | BP_DDR_PCB_2LAYER},)
  MOVED_DT({bp_usExtIntrSesBtnWireless, .u.us = BP_EXT_INTR_0 | BP_EXT_INTR_TYPE_IRQ_LOW_LEVEL | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_23_AL},)
  MOVED_DT({bp_usExtIntrResetToDefault, .u.us = BP_EXT_INTR_1 | BP_EXT_INTR_TYPE_IRQ_LOW_LEVEL | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_55_AL},)
  MOVED_DT({bp_usPonLbe,                .u.us = BP_GPIO_67_AL},)
  MOVED_DT({bp_usGpioI2cScl,            .u.us = BP_GPIO_68_AH},)
  MOVED_DT({bp_usGpioI2cSda,            .u.us = BP_GPIO_69_AH},)
  MOVED_DT({bp_usUsbPwrFlt0,            .u.us = BP_GPIO_74_AL},)
  MOVED_DT({bp_usUsbPwrOn0,             .u.us = BP_GPIO_75_AL},)
  MOVED_DT({bp_usUsbPwrFlt1,            .u.us = BP_GPIO_76_AL},)
  MOVED_DT({bp_usUsbPwrOn1,             .u.us = BP_GPIO_77_AL},)
  {bp_usGpioLedSesWireless,    .u.us = BP_GPIO_5_AL},
  MOVED_DT({bp_InvSerdesRxPol,          .u.us = pmd_polarity_invert},)
  MOVED_DT({bp_InvSerdesTxPol,          .u.us = pmd_polarity_invert},)
  MOVED_DT({bp_usPmdMACEwakeEn,         .u.us = BP_GPIO_4_AH},)
  MOVED_DT({bp_usExtIntrPmdAlarm,       .u.us = BP_EXT_INTR_2 | BP_EXT_INTR_TYPE_IRQ_HIGH_LEVEL | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_14_AH},)
  MOVED_DT({bp_usGpioPmdReset,          .u.us = BP_GPIO_40_AL},)
  MOVED_DT({bp_pmdFunc,                 .u.us = BP_PMD_APD_REG_DISABLED | BP_PMD_APD_TYPE_BOOST},)
  {bp_ucDspType0,              .u.uc = BP_VOIP_DSP},
  {bp_ucDspAddress,            .u.uc = 0},
  {bp_last}
};

static bp_elem_t g_bcm968462ref[] = {
  {bp_cpBoardId,               .u.cp = "968462REF"},
  MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,               .u.ul = 0x0f},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0x01 | MAC_IF_MII},)
  MOVED_DT({bp_usOamIndex,              .u.us = 3},)
  MOVED_DT({bp_ucPhyDevName,            .u.cp = "eth3"},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_GPIO_52_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_FAE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_FAE},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 0x02 | MAC_IF_MII},)
  MOVED_DT({bp_usOamIndex,              .u.us = 2},)
  MOVED_DT({bp_ucPhyDevName,            .u.cp = "eth2"},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_GPIO_53_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_FAE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_FAE},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 0x03 | MAC_IF_MII},)
  MOVED_DT({bp_usOamIndex,              .u.us = 1},)
  MOVED_DT({bp_ucPhyDevName,            .u.cp = "eth1"},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_GPIO_49_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_FAE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_FAE},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 0x04 | MAC_IF_GMII},)
  MOVED_DT({bp_usOamIndex,              .u.us = 0},)
  MOVED_DT({bp_ucPhyDevName,            .u.cp = "eth0"},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_GPIO_44_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulMemoryConfig,          .u.ul = BP_DDR_SPEED_800_11_11_11 | BP_DDR_TOTAL_SIZE_512MB| BP_DDR_DEVICE_WIDTH_8 | BP_DDR_TOTAL_WIDTH_8BIT | BP_DDR_PCB_2LAYER},)
  MOVED_DT({bp_usExtIntrResetToDefault, .u.us = BP_EXT_INTR_1 | BP_EXT_INTR_TYPE_IRQ_LOW_LEVEL | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_45_AL},)
  MOVED_DT({bp_usGpioI2cScl,            .u.us = BP_GPIO_68_AH},)
  MOVED_DT({bp_usGpioI2cSda,            .u.us = BP_GPIO_69_AH},)
  {bp_usUsbPwrFlt1,            .u.us = BP_GPIO_76_AL},
  {bp_usUsbPwrOn1,             .u.us = BP_GPIO_77_AL},
  MOVED_DT({bp_usPonLbe,                .u.us = BP_GPIO_67_AL},)
  MOVED_DT({bp_InvSerdesRxPol,          .u.us = pmd_polarity_invert},)
  MOVED_DT({bp_InvSerdesTxPol,          .u.us = pmd_polarity_invert},)
  MOVED_DT({bp_usPmdMACEwakeEn,         .u.us = BP_GPIO_4_AH},)
  MOVED_DT({bp_usExtIntrPmdAlarm,       .u.us = BP_EXT_INTR_2 | BP_EXT_INTR_TYPE_IRQ_HIGH_LEVEL | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_14_AH},)
  MOVED_DT({bp_usGpioPmdReset,          .u.us = BP_GPIO_40_AL},)
  MOVED_DT({bp_pmdFunc,                 .u.us = BP_PMD_APD_REG_DISABLED | BP_PMD_APD_TYPE_BOOST},)
  MOVED_DT({bp_usRogueOnuEn,            .u.us = BP_GPIO_14_AH},)
  {bp_ucDspType0,              .u.uc = BP_VOIP_DSP},
  {bp_ucDspAddress,            .u.uc = 0},
  {bp_last}
};

static bp_elem_t g_bcm968462spw[] = {
  {bp_cpBoardId,               .u.cp = "968462SPW"},
  MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,               .u.ul = 0x0f},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0x01 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_GPIO_52_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 0x02 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_GPIO_53_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 0x03 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_GPIO_49_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 0x04 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_GPIO_44_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulMemoryConfig,          .u.ul = BP_DDR_SPEED_800_11_11_11 | BP_DDR_TOTAL_SIZE_512MB| BP_DDR_DEVICE_WIDTH_8 | BP_DDR_TOTAL_WIDTH_8BIT | BP_DDR_PCB_2LAYER},)
  MOVED_DT({bp_usExtIntrResetToDefault, .u.us = BP_EXT_INTR_1 | BP_EXT_INTR_TYPE_IRQ_LOW_LEVEL | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_45_AL},)
  MOVED_DT({bp_usGpioI2cScl,            .u.us = BP_GPIO_68_AH},)
  MOVED_DT({bp_usGpioI2cSda,            .u.us = BP_GPIO_69_AH},)
  {bp_usUsbPwrFlt1,            .u.us = BP_GPIO_76_AL},
  {bp_usUsbPwrOn1,             .u.us = BP_GPIO_77_AL},
  MOVED_DT({bp_usPonLbe,                .u.us = BP_GPIO_67_AL},)
  MOVED_DT({bp_InvSerdesRxPol,          .u.us = pmd_polarity_invert},)
  MOVED_DT({bp_InvSerdesTxPol,          .u.us = pmd_polarity_invert},)
  MOVED_DT({bp_usPmdMACEwakeEn,         .u.us = BP_GPIO_4_AH},)
  MOVED_DT({bp_usExtIntrPmdAlarm,       .u.us = BP_EXT_INTR_2 | BP_EXT_INTR_TYPE_IRQ_HIGH_LEVEL | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_14_AH},)
  MOVED_DT({bp_usGpioPmdReset,          .u.us = BP_GPIO_40_AL},)
  MOVED_DT({bp_pmdFunc,                 .u.us = BP_PMD_APD_REG_DISABLED | BP_PMD_APD_TYPE_BOOST},)
  MOVED_DT({bp_usRogueOnuEn,            .u.us = BP_GPIO_14_AH},)
  {bp_ucDspType0,              .u.uc = BP_VOIP_DSP},
  {bp_ucDspAddress,            .u.uc = 0},
  {bp_last}
};

static bp_elem_t g_bcm968462xsv[] = {
  {bp_cpBoardId,               .u.cp = "968462XSV"},
  MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,               .u.ul = 0x0f},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0x01 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_GPIO_52_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 0x02 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_GPIO_53_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 0x03 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_GPIO_49_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 0x04 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_GPIO_44_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulMemoryConfig,          .u.ul = BP_DDR_SPEED_800_11_11_11 | BP_DDR_TOTAL_SIZE_512MB| BP_DDR_DEVICE_WIDTH_8 | BP_DDR_TOTAL_WIDTH_8BIT},)
  MOVED_DT({bp_usGpioI2cScl,            .u.us = BP_GPIO_68_AH},)
  MOVED_DT({bp_usGpioI2cSda,            .u.us = BP_GPIO_69_AH},)
  {bp_usUsbPwrFlt1,            .u.us = BP_GPIO_76_AL},
  {bp_usUsbPwrOn1,             .u.us = BP_GPIO_77_AL},
  MOVED_DT({bp_usRogueOnuEn,            .u.us = BP_GPIO_14_AH},)
  MOVED_DT({bp_usPonLbe,                .u.us = BP_GPIO_67_AL},)
  MOVED_DT({bp_usGpioWanSignalDetected, .u.us = BP_GPIO_15_AH},)
  {bp_last}
};

static bp_elem_t g_bcm968460rgw[] = {
  {bp_cpBoardId,               .u.cp = "968460RGW"},
  MOVED_DT({bp_ulMemoryConfig,          .u.ul = BP_DDR_SPEED_800_11_11_11 | BP_DDR_TOTAL_SIZE_256MB| BP_DDR_DEVICE_WIDTH_16 | BP_DDR_TOTAL_WIDTH_16BIT},)
  MOVED_DT({bp_usPcmSdin,                .u.us = BP_GPIO_0_AH},)
  MOVED_DT({bp_usPcmSdout,               .u.us = BP_GPIO_1_AH},)
  MOVED_DT({bp_usPcmClk,                 .u.us = BP_GPIO_2_AH},)
  MOVED_DT({bp_usPcmFs,                  .u.us = BP_GPIO_3_AH},)
  MOVED_DT({bp_usExtIntrResetToDefault, .u.us = BP_EXT_INTR_0 | BP_EXT_INTR_TYPE_IRQ_LOW_LEVEL | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_19_AL},)
  MOVED_DT({bp_usGpioI2cScl,            .u.us = BP_GPIO_68_AH},)
  MOVED_DT({bp_usGpioI2cSda,            .u.us = BP_GPIO_69_AH},)
  MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,               .u.ul = 0x1f},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0x01 | MAC_IF_GMII},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 0x02 | MAC_IF_GMII},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 0x03 | MAC_IF_GMII},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 0x04 | MAC_IF_GMII},)
  MOVED_DT({bp_ulPhyId4,                .u.ul = 0x07 | MAC_IF_RGMII | PHY_EXTERNAL | PHY_INTEGRATED_VALID},)
  MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_TX_INTERNAL_DELAY},)
  MOVED_DT({bp_usMiiMdc,                .u.us = BP_GPIO_54_AH},)
  MOVED_DT({bp_usMiiMdio,               .u.us = BP_GPIO_55_AH},)
  MOVED_DT({bp_usPonLbe,                .u.us = BP_GPIO_67_AL},)
  MOVED_DT({bp_usUsbPwrFlt0,            .u.us = BP_GPIO_74_AL},)
  MOVED_DT({bp_usUsbPwrOn0,             .u.us = BP_GPIO_75_AL},)
  MOVED_DT({bp_usUsbPwrFlt1,            .u.us = BP_GPIO_76_AL},)
  MOVED_DT({bp_usUsbPwrOn1,             .u.us = BP_GPIO_77_AL},)
  MOVED_DT({bp_InvSerdesRxPol,          .u.us = pmd_polarity_invert},)
  MOVED_DT({bp_InvSerdesTxPol,          .u.us = pmd_polarity_invert},)
  MOVED_DT({bp_usPmdMACEwakeEn,         .u.us = BP_GPIO_4_AH},)
  MOVED_DT({bp_usExtIntrPmdAlarm,       .u.us = BP_EXT_INTR_2 | BP_EXT_INTR_TYPE_IRQ_HIGH_LEVEL | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_14_AH},)
  MOVED_DT({bp_usGpioPmdReset,          .u.us = BP_GPIO_40_AL},)
  MOVED_DT({bp_pmdFunc,                 .u.us = BP_PMD_APD_REG_DISABLED | BP_PMD_APD_TYPE_BOOST},)
  {bp_ucDspType0,               .u.uc =  BP_VOIP_DSP},
  {bp_ucDspAddress,             .u.uc =  0},
  {bp_last}
};

static bp_elem_t g_bcm968462egr[] = {
  {bp_cpBoardId,               .u.cp = "968462EGR"},
  MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_INTERNAL_PHY},)
  MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
  MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
  MOVED_DT({bp_ulPortMap,               .u.ul = 0x0f},)
  MOVED_DT({bp_ulPhyId0,                .u.ul = 0x01 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_GPIO_52_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId1,                .u.ul = 0x02 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_GPIO_53_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId2,                .u.ul = 0x03 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_GPIO_49_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulPhyId3,                .u.ul = 0x04 | MAC_IF_GMII},)
  MOVED_DT({bp_usNetLed0,               .u.us = BP_GPIO_44_AL},)
  MOVED_DT({bp_ulNetLedLink,            .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_ulNetLedActivity,        .u.ul = BP_NET_LED_SPEED_GBE},)
  MOVED_DT({bp_InvSerdesRxPol,          .u.us = pmd_polarity_invert},)
  MOVED_DT({bp_InvSerdesTxPol,          .u.us = pmd_polarity_invert},)
  MOVED_DT({bp_usPmdMACEwakeEn,         .u.us = BP_GPIO_4_AH},)
  MOVED_DT({bp_usExtIntrPmdAlarm,       .u.us = BP_EXT_INTR_2 | BP_EXT_INTR_TYPE_IRQ_HIGH_LEVEL | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_14_AH},)
  MOVED_DT({bp_usGpioPmdReset,          .u.us = BP_GPIO_40_AL},)
  MOVED_DT({bp_usPonLbe,                .u.us = BP_GPIO_67_AL},)
  MOVED_DT({bp_pmdFunc,                 .u.us = BP_PMD_APD_REG_DISABLED | BP_PMD_APD_TYPE_BOOST},)
  MOVED_DT({bp_usPcmSdin,                .u.us = BP_GPIO_0_AH},)
  MOVED_DT({bp_usPcmSdout,               .u.us = BP_GPIO_1_AH},)
  MOVED_DT({bp_usPcmClk,                 .u.us = BP_GPIO_2_AH},)
  MOVED_DT({bp_usPcmFs,                  .u.us = BP_GPIO_3_AH},)
  MOVED_DT({bp_ulMemoryConfig,           .u.ul = BP_DDR_SPEED_800_11_11_11 | BP_DDR_TOTAL_SIZE_128MB| BP_DDR_DEVICE_WIDTH_8 | BP_DDR_TOTAL_WIDTH_8BIT | BP_DDR_PCB_2LAYER},)
  MOVED_DT({bp_usExtIntrResetToDefault, .u.us = BP_EXT_INTR_1 | BP_EXT_INTR_TYPE_IRQ_LOW_LEVEL | BP_EXT_INTR_TYPE_IRQ_SENSE_EDGE},)
  MOVED_DT({bp_usGpio_Intr,             .u.us = BP_GPIO_45_AL},)
  MOVED_DT({bp_usGpioI2cScl,            .u.us = BP_GPIO_68_AH},)
  MOVED_DT({bp_usGpioI2cSda,            .u.us = BP_GPIO_69_AH},)
  {bp_usUsbPwrFlt1,            .u.us = BP_GPIO_76_AL},
  {bp_usUsbPwrOn1,             .u.us = BP_GPIO_77_AL},
  MOVED_DT({bp_usGpioLedUSB2,           .u.us = BP_SERIAL_GPIO_15_AL},)
  MOVED_DT({bp_usGpioVoip1Led,          .u.us = BP_SERIAL_GPIO_11_AL},)
  {bp_ucDspType0,              .u.uc = BP_VOIP_DSP},
  {bp_ucDspAddress,            .u.uc = 0},
  {bp_last}
};

bp_elem_t * g_BoardParms[] = {g_bcm968460sv, g_bcm968460ref, g_bcm968460refp, g_bcm968461prw, g_bcm968462ref, g_bcm968462spw, g_bcm968462xsv, g_bcm968460rgw, g_bcm968462egr, 0};

