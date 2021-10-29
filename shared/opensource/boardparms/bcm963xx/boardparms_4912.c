#include "bp_defs.h"
#include "boardparms.h"
#include "bcmSpiRes.h"

#define BCM94912_PHY_BASE             0x1


static bp_elem_t g_bcm94915ref1d[] = {
  {bp_cpBoardId,               .u.cp = "94915REF1D"},
  {bp_usSerialLedData,         .u.us = BP_GPIO_16_AH},
  {bp_usSerialLedClk,          .u.us = BP_GPIO_17_AH},
  {bp_usSerialLedMask,         .u.us = BP_GPIO_18_AH},
  {bp_usGphyBaseAddress,       .u.us = BCM94912_PHY_BASE},
  {bp_ucPhyType0,              .u.uc = BP_ENET_NO_PHY}, // Runner
  {bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},
  {bp_ucPhyAddress,            .u.uc = 0x1e},
  {bp_ulPortMap,               .u.ul = 0xef},   // QGPHYs + 10GPHY + Serdes
  {bp_ulPhyId0,                .u.ul = (BCM94912_PHY_BASE + 0x00) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  {bp_ulPhyId1,                .u.ul = (BCM94912_PHY_BASE + 0x01) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  {bp_ulPhyId2,                .u.ul = (BCM94912_PHY_BASE + 0x02) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  {bp_ulPhyId3,                .u.ul = (BCM94912_PHY_BASE + 0x03) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},

  {bp_ulPhyId5,                .u.ul = 6 |  PHY_INTEGRATED_VALID | MAC_IF_SERDES},
  {bp_ulPhyId5,                .u.ul = 0x11 | PHY_EXTERNAL | PHY_TYPE_CL45GPHY},
  {bp_ulPhyId6,                .u.ul = 7 |  PHY_INTEGRATED_VALID | MAC_IF_SERDES},
  {bp_ulPhyId7,                .u.ul = 8 |  PHY_INTEGRATED_VALID | MAC_IF_SERDES},

  {bp_usIntfId,                .u.us = 0},
  {bp_usIntfType,              .u.us = BP_INTF_TYPE_I2C},
  {bp_usPortNum,               .u.us = 0},
  {bp_usGpioI2cSda,            .u.us = BP_GPIO_13_AH },
  {bp_usGpioI2cScl,            .u.us = BP_GPIO_12_AH },
  {bp_usIntfEnd},
  {bp_usIntfId,                .u.us = 1},
  {bp_usIntfType,              .u.us = BP_INTF_TYPE_I2C},
  {bp_usPortNum,               .u.us = 1},
  {bp_usGpioI2cSda,            .u.us = BP_GPIO_15_AH },
  {bp_usGpioI2cScl,            .u.us = BP_GPIO_14_AH },
  {bp_usIntfEnd},
  {bp_usIntfId,                .u.us = 2},
  {bp_usIntfType,              .u.us = BP_INTF_TYPE_SGMII},
  {bp_usPortNum,               .u.us = 5},
  {bp_usIntfEnd},
  {bp_usIntfId,                .u.us = 3},
  {bp_usIntfType,              .u.us = BP_INTF_TYPE_SGMII},
  {bp_usPortNum,               .u.us = 6},
  {bp_usIntfMgmtType,          .u.us = BP_INTF_MGMT_TYPE_I2C},
  {bp_usIntfMgmtBusNum,        .u.us = 0},
  {bp_usGpioSfpModDetect,      .u.us = BP_GPIO_4_AL},
  {bp_usSfpSigDetect,          .u.us = BP_GPIO_1_AL},
  {bp_usIntfEnd},
  {bp_usIntfId,                .u.us = 4},
  {bp_usIntfType,              .u.us = BP_INTF_TYPE_SGMII},
  {bp_usPortNum,               .u.us = 7},
  {bp_usIntfMgmtType,          .u.us = BP_INTF_MGMT_TYPE_I2C},
  {bp_usIntfMgmtBusNum,        .u.us = 1},
  {bp_usGpioSfpModDetect,      .u.us = BP_GPIO_5_AL},
  {bp_usSfpSigDetect,          .u.us = BP_GPIO_2_AL},
  {bp_usIntfEnd},

  {bp_usMiiMdc,                .u.us = BP_GPIO_54_AH},
  {bp_usMiiMdio,               .u.us = BP_GPIO_55_AH},
  {bp_ucDspType0,              .u.uc = BP_VOIP_DSP},
  {bp_ucDspAddress,            .u.uc = 0},
  {bp_last}
};

static bp_elem_t g_bcm94915ref1d_rgmii[] = {
  {bp_cpBoardId,               .u.cp = "94915REF1D_RGM"},
  {bp_usGphyBaseAddress,       .u.us = BCM94912_PHY_BASE},
  {bp_ucPhyType0,              .u.uc = BP_ENET_NO_PHY}, // Runner
  {bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},
  {bp_ucPhyAddress,            .u.uc = 0x1e},
  {bp_ulPortMap,               .u.ul = 0xff},   // QGPHYs + RGMII + 10GPHY + Serdes
  {bp_ulPhyId0,                .u.ul = (BCM94912_PHY_BASE + 0x00) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  {bp_ulPhyId1,                .u.ul = (BCM94912_PHY_BASE + 0x01) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  {bp_ulPhyId2,                .u.ul = (BCM94912_PHY_BASE + 0x02) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  {bp_ulPhyId3,                .u.ul = (BCM94912_PHY_BASE + 0x03) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  {bp_ulPhyId4,                .u.ul =  0x19 | PHY_INTEGRATED_VALID | MAC_IF_RGMII_1P8V | PHY_EXTERNAL},
  {bp_ulPhyId5,                .u.ul = 6 |  PHY_INTEGRATED_VALID | MAC_IF_SERDES},
  {bp_ulPhyId5,                .u.ul = 0x11 | PHY_EXTERNAL | PHY_TYPE_CL45GPHY},
  {bp_ulPhyId6,                .u.ul = 7 |  PHY_INTEGRATED_VALID | MAC_IF_SERDES},
  {bp_ulPhyId7,                .u.ul = 8 |  PHY_INTEGRATED_VALID | MAC_IF_SERDES},
  
  {bp_elemTemplate,            .u.bp_elemp = g_bcm94915ref1d},  
  {bp_last}  
};

static bp_elem_t g_bcm94915ref1d_p1b[] = {
  {bp_cpBoardId,               .u.cp = "94915REF1D_P1B"},
  {bp_elemTemplate,            .u.bp_elemp = g_bcm94915ref1d},  
  {bp_last}  
};

static bp_elem_t g_bcm94912ref1d[] = {
  {bp_cpBoardId,               .u.cp = "94912REF1D"},
  {bp_elemTemplate,            .u.bp_elemp = g_bcm94915ref1d},  
  {bp_last}   
};

static bp_elem_t g_bcm94912ref1d_rgmii[] = {
  {bp_cpBoardId,               .u.cp = "94912REF1D_RGM"},
  {bp_elemTemplate,            .u.bp_elemp = g_bcm94915ref1d_rgmii},  
  {bp_last}   
};

static bp_elem_t g_bcm94915sv[] = {
  {bp_cpBoardId,               .u.cp = "94915SV"},
  {bp_usSerialLedData,         .u.us = BP_GPIO_16_AH},
  {bp_usSerialLedClk,          .u.us = BP_GPIO_17_AH},
  {bp_usSerialLedMask,         .u.us = BP_GPIO_18_AH},
  {bp_usGphyBaseAddress,       .u.us = BCM94912_PHY_BASE},
  {bp_ucPhyType0,              .u.uc = BP_ENET_NO_PHY}, // Runner
  {bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},
  {bp_ucPhyAddress,            .u.uc = 0x1e},
  {bp_ulPortMap,               .u.ul = 0xef},   // QGPHYs + Serdes
  {bp_ulPhyId0,                .u.ul = (BCM94912_PHY_BASE + 0x00) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  {bp_ulPhyId1,                .u.ul = (BCM94912_PHY_BASE + 0x01) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  {bp_ulPhyId2,                .u.ul = (BCM94912_PHY_BASE + 0x02) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  {bp_ulPhyId3,                .u.ul = (BCM94912_PHY_BASE + 0x03) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},

  {bp_ulPhyId5,                .u.ul = 6 |  PHY_INTEGRATED_VALID | MAC_IF_SERDES},
  {bp_ulPhyId5,                .u.ul = 0x11 | PHY_EXTERNAL | PHY_TYPE_CL45GPHY},
  {bp_ulPhyId6,                .u.ul = 7 |  PHY_INTEGRATED_VALID | MAC_IF_SERDES},
  {bp_ulPhyId7,                .u.ul = 8 |  PHY_INTEGRATED_VALID | MAC_IF_SERDES},

  {bp_usIntfId,                .u.us = 0},
  {bp_usIntfType,              .u.us = BP_INTF_TYPE_I2C},
  {bp_usPortNum,               .u.us = 0},
  {bp_usGpioI2cSda,            .u.us = BP_GPIO_13_AH },
  {bp_usGpioI2cScl,            .u.us = BP_GPIO_12_AH },
  {bp_usIntfEnd},
  {bp_usIntfId,                .u.us = 1},
  {bp_usIntfType,              .u.us = BP_INTF_TYPE_I2C},
  {bp_usPortNum,               .u.us = 1},
  {bp_usGpioI2cSda,            .u.us = BP_GPIO_15_AH },
  {bp_usGpioI2cScl,            .u.us = BP_GPIO_14_AH },
  {bp_usIntfEnd},

  {bp_usIntfId,                .u.us = 2},
  {bp_usIntfType,              .u.us = BP_INTF_TYPE_SGMII},
  {bp_usPortNum,               .u.us = 5},
  {bp_usIntfEnd},
  {bp_usIntfId,                .u.us = 3},
  {bp_usIntfType,              .u.us = BP_INTF_TYPE_SGMII},
  {bp_usPortNum,               .u.us = 6},
  {bp_usIntfMgmtType,          .u.us = BP_INTF_MGMT_TYPE_I2C},
  {bp_usIntfMgmtBusNum,        .u.us = 0},
  {bp_usGpioSfpModDetect,      .u.us = BP_GPIO_4_AL},
  {bp_usSfpSigDetect,          .u.us = BP_GPIO_1_AL},
  {bp_usIntfEnd},
  {bp_usIntfId,                .u.us = 4},
  {bp_usIntfType,              .u.us = BP_INTF_TYPE_SGMII},
  {bp_usPortNum,               .u.us = 7},
  {bp_usIntfMgmtType,          .u.us = BP_INTF_MGMT_TYPE_I2C},
  {bp_usIntfMgmtBusNum,        .u.us = 1},
  {bp_usGpioSfpModDetect,      .u.us = BP_GPIO_5_AL},
  {bp_usSfpSigDetect,          .u.us = BP_GPIO_2_AL},
  {bp_usIntfEnd},

  {bp_usMiiMdc,                .u.us = BP_GPIO_54_AH},
  {bp_usMiiMdio,               .u.us = BP_GPIO_55_AH},
  
  {bp_usPcmSdin,               .u.us = BP_GPIO_8_AH},
  {bp_usPcmSdout,              .u.us = BP_GPIO_9_AH},
  {bp_usPcmClk,                .u.us = BP_GPIO_10_AH},
  {bp_usPcmFs,                 .u.us = BP_GPIO_11_AH},

  {bp_last}
};

static bp_elem_t g_bcm94915sv_gphy[] = {
  {bp_cpBoardId,               .u.cp = "94915SV_GPHY"},
  {bp_elemTemplate,            .u.bp_elemp = g_bcm94915sv},
  {bp_last}
};

static bp_elem_t g_bcm949418eap[] = {
  {bp_cpBoardId,               .u.cp = "949418EAP"},

  {bp_usGphyBaseAddress,       .u.us = BCM94912_PHY_BASE},
  {bp_ucPhyType0,              .u.uc = BP_ENET_NO_PHY}, // Runner
  {bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},
  {bp_ucPhyAddress,            .u.uc = 0x1e},
  {bp_ulPortMap,               .u.ul = 0x60},   // XGPHY, SGMII1
  {bp_ulPhyId5,                .u.ul =  6 |  PHY_INTEGRATED_VALID | MAC_IF_SERDES},
  {bp_ulPhyId5,                .u.ul = 17 | PHY_EXTERNAL | PHY_TYPE_CL45GPHY},
  {bp_ulPhyId6,                .u.ul =  7 |  PHY_INTEGRATED_VALID | MAC_IF_SERDES},
  {bp_ulPhyId6,                .u.ul = 31 | PHY_EXTERNAL | PHY_TYPE_CL45GPHY},

  {bp_usGpioPhyReset,          .u.us = BP_GPIO_3_AL},
  {bp_usMiiMdc,                .u.us = BP_GPIO_54_AH},
  {bp_usMiiMdio,               .u.us = BP_GPIO_55_AH},
  
  {bp_last}
};

bp_elem_t * g_BoardParms[] = {g_bcm94915sv, g_bcm94915sv_gphy, g_bcm94915ref1d, g_bcm94915ref1d_rgmii, g_bcm94915ref1d_p1b, g_bcm949418eap,
    g_bcm94912ref1d, g_bcm94912ref1d_rgmii, 0};
