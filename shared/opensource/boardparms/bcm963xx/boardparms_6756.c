#include "bp_defs.h"
#include "boardparms.h"
#include "bcmSpiRes.h"

#define MOVED_DT(x...)

#define BCM96756_PHY_BASE             0x8

static bp_elem_t g_bcm96756sv[] = {
    {bp_cpBoardId,               .u.cp = "96756SV"},

    MOVED_DT({bp_usGphyBaseAddress,       .u.us = BCM96756_PHY_BASE},) 
    MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_NO_PHY},)
    MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
    MOVED_DT({bp_ulPortMap,               .u.ul = 0x23},)
    MOVED_DT({bp_ulPhyId0,                .u.ul = (BCM96756_PHY_BASE + 0x00) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},)
    MOVED_DT({bp_ulPhyId1,                .u.ul = 0x19 |  PHY_INTEGRATED_VALID | MAC_IF_RGMII_1P8V | PHY_EXTERNAL},)
    MOVED_DT({bp_ulPhyId5,                .u.ul = 6 | PHY_INTEGRATED_VALID | MAC_IF_SERDES},)

    {bp_usIntfId,                .u.us = 0},
    {bp_usIntfType,              .u.us = BP_INTF_TYPE_SGMII},
    {bp_usPortNum,               .u.us = 0},
    {bp_usIntfMgmtType,          .u.us = BP_INTF_MGMT_TYPE_I2C},
    {bp_usIntfMgmtBusNum,        .u.us = 0},
    {bp_usGpioSfpModDetect,      .u.us = BP_GPIO_9_AL},
    {bp_usSfpSigDetect,          .u.us = BP_GPIO_26_AL },
    {bp_usIntfEnd},

    {bp_usIntfId,                .u.us = 1},
    {bp_usIntfType,              .u.us = BP_INTF_TYPE_I2C},
    {bp_usPortNum,               .u.us = 0},
    {bp_usIntfEnd},

    {bp_last}
};

static bp_elem_t g96756ref1[] = {
    {bp_cpBoardId,               .u.cp = "96756REF1"},

    MOVED_DT({bp_usGphyBaseAddress,       .u.us = BCM96756_PHY_BASE},)
    MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_NO_PHY},)
    MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
    MOVED_DT({bp_ulPortMap,               .u.ul = 0x03},)
    MOVED_DT({bp_ulPhyId0,                .u.ul = (BCM96756_PHY_BASE + 0x00) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},)
    MOVED_DT({bp_ulPhyId1,                .u.ul = RGMII_DIRECT_3P3V | EXTSW_CONNECTED},)
    MOVED_DT({bp_ulPortFlags,             .u.ul = PORT_FLAG_TX_INTERNAL_DELAY | PORT_FLAG_RX_INTERNAL_DELAY},)

    MOVED_DT({bp_ucPhyType1,              .u.uc = BP_ENET_EXTERNAL_SWITCH},)
    MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
    MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MDIO},)
    MOVED_DT({bp_ulPortMap,               .u.ul = 0x0f},)
    MOVED_DT({bp_ulPhyId0,                .u.ul = BP_PHY_ID_0 | CONNECTED_TO_EXTERN_SW},)
    MOVED_DT({bp_ulPhyId1,                .u.ul = BP_PHY_ID_1 | CONNECTED_TO_EXTERN_SW},)
    MOVED_DT({bp_ulPhyId2,                .u.ul = BP_PHY_ID_2 | CONNECTED_TO_EXTERN_SW},)
    MOVED_DT({bp_ulPhyId3,                .u.ul = BP_PHY_ID_3 | CONNECTED_TO_EXTERN_SW},)

    {bp_last}
};

static bp_elem_t g96756ref1_sg[] = {
    {bp_cpBoardId,               .u.cp = "96756REF1_SG"},

    MOVED_DT({bp_usGphyBaseAddress,       .u.us = BCM96756_PHY_BASE},)
    MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_NO_PHY},)
    MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
    MOVED_DT({bp_ulPortMap,               .u.ul = 0x21},)
    MOVED_DT({bp_ulPhyId0,                .u.ul = (BCM96756_PHY_BASE + 0x00) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},)
    MOVED_DT({bp_ulPhyId5,                .u.ul = 6 |  PHY_INTEGRATED_VALID | MAC_IF_SERDES | EXTSW_CONNECTED},)

    MOVED_DT({bp_ucPhyType1,              .u.uc = BP_ENET_EXTERNAL_SWITCH},)
    MOVED_DT({bp_ucPhyAddress,            .u.uc = 0x0},)
    MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MDIO},)
    MOVED_DT({bp_ulPortMap,               .u.ul = 0x0f},)
    MOVED_DT({bp_ulPhyId0,                .u.ul = BP_PHY_ID_0 | CONNECTED_TO_EXTERN_SW},)
    MOVED_DT({bp_ulPhyId1,                .u.ul = BP_PHY_ID_1 | CONNECTED_TO_EXTERN_SW},)
    MOVED_DT({bp_ulPhyId2,                .u.ul = BP_PHY_ID_2 | CONNECTED_TO_EXTERN_SW},)
    MOVED_DT({bp_ulPhyId3,                .u.ul = BP_PHY_ID_3 | CONNECTED_TO_EXTERN_SW},)

    {bp_last}
};

static bp_elem_t g_bcm96756rfdvt[] = {
    {bp_cpBoardId,               .u.cp = "96756RF_DVT"},

    MOVED_DT({bp_usGphyBaseAddress,       .u.us = BCM96756_PHY_BASE},) 
    MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_NO_PHY},)
    MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
    MOVED_DT({bp_ulPortMap,               .u.ul = 0x21},)
    MOVED_DT({bp_ulPhyId0,                .u.ul = (BCM96756_PHY_BASE + 0x00) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},)
    MOVED_DT({bp_ulPhyId5,                .u.ul = 6 | PHY_INTEGRATED_VALID | MAC_IF_SERDES},)
    MOVED_DT({bp_ulPhyId5,                .u.ul = 0x1e | PHY_EXTERNAL | PHY_TYPE_CL45GPHY},)

    {bp_last}
};

static bp_elem_t g_bcm947623eap6l[] = {
    {bp_cpBoardId,               .u.cp = "947623EAP6L"},

    MOVED_DT({bp_usGphyBaseAddress,       .u.us = BCM96756_PHY_BASE},)
    MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_NO_PHY},)
    MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
    MOVED_DT({bp_ulPortMap,               .u.ul = 0x21},)
    MOVED_DT({bp_ulPhyId0,                .u.ul = (BCM96756_PHY_BASE + 0x00) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},)
    MOVED_DT({bp_ulPhyId5,                .u.ul = 6 | PHY_INTEGRATED_VALID | MAC_IF_SERDES},)
    MOVED_DT({bp_ulPhyId5,                .u.ul = 0x1f | PHY_EXTERNAL | PHY_TYPE_CL45GPHY},)

    {bp_last}
};

static bp_elem_t g_bcm947623eap6ll[] = {
    {bp_cpBoardId,               .u.cp = "947623EAP6LL"},

    MOVED_DT({bp_usGphyBaseAddress,       .u.us = BCM96756_PHY_BASE},)
    MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_NO_PHY},)
    MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
    MOVED_DT({bp_ulPortMap,               .u.ul = 0x21},)
    MOVED_DT({bp_ulPhyId0,                .u.ul = (BCM96756_PHY_BASE + 0x00) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},)
    MOVED_DT({bp_ulPhyId5,                .u.ul = 6 | PHY_INTEGRATED_VALID | MAC_IF_SERDES},)
    MOVED_DT({bp_ulPhyId5,                .u.ul = 0x1f | PHY_EXTERNAL | PHY_TYPE_CL45GPHY},)

    {bp_last}
};

static bp_elem_t g_bcm96757sv[] = {
    {bp_cpBoardId,               .u.cp = "96757SV"},

    MOVED_DT({bp_usGphyBaseAddress,       .u.us = BCM96756_PHY_BASE},)
    MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_NO_PHY},)
    MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
    MOVED_DT({bp_ulPortMap,               .u.ul = 0x61},)
    MOVED_DT({bp_ulPhyId0,                .u.ul = (BCM96756_PHY_BASE + 0x00) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},)
    MOVED_DT({bp_ulPhyId5,                .u.ul = 6 | PHY_INTEGRATED_VALID | MAC_IF_SERDES},)
    MOVED_DT({bp_ulPhyId6,                .u.ul = 7 | PHY_INTEGRATED_VALID | MAC_IF_SERDES},)

    {bp_usIntfId,                .u.us = 0},
    {bp_usIntfType,              .u.us = BP_INTF_TYPE_SGMII},
    {bp_usPortNum,               .u.us = 0},
    {bp_usIntfMgmtType,          .u.us = BP_INTF_MGMT_TYPE_I2C},
    {bp_usIntfMgmtBusNum,        .u.us = 0},
    {bp_usGpioSfpModDetect,      .u.us = BP_GPIO_9_AL},
    {bp_usSfpSigDetect,          .u.us = BP_GPIO_26_AL },
    {bp_usIntfEnd},

    {bp_usIntfId,                .u.us = 1},
    {bp_usIntfType,              .u.us = BP_INTF_TYPE_SGMII},
    {bp_usPortNum,               .u.us = 1},
    {bp_usIntfMgmtType,          .u.us = BP_INTF_MGMT_TYPE_I2C},
    {bp_usIntfMgmtBusNum,        .u.us = 1},
    {bp_usGpioSfpModDetect,      .u.us = BP_GPIO_66_AL},
    {bp_usSfpSigDetect,          .u.us = BP_GPIO_67_AL },
    {bp_usIntfEnd},

    {bp_usIntfId,                .u.us = 2},
    {bp_usIntfType,              .u.us = BP_INTF_TYPE_I2C},
    {bp_usPortNum,               .u.us = 0},
    {bp_usIntfEnd},

    {bp_usIntfId,                .u.us = 3},
    {bp_usIntfType,              .u.us = BP_INTF_TYPE_I2C},
    {bp_usPortNum,               .u.us = 1},
    {bp_usIntfEnd},
    
    {bp_last}
};

static bp_elem_t g_bcm96757ref1t[] = {
    {bp_cpBoardId,               .u.cp = "96757REF1T"},

    MOVED_DT({bp_usGphyBaseAddress,       .u.us = BCM96756_PHY_BASE},)
    MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_NO_PHY},)
    MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
    MOVED_DT({bp_ulPortMap,               .u.ul = 0x61},)
    MOVED_DT({bp_ulPhyId0,                .u.ul = (BCM96756_PHY_BASE + 0x00) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},)
    MOVED_DT({bp_ulPhyId5,                .u.ul = 6 | PHY_INTEGRATED_VALID | MAC_IF_SERDES},)
    MOVED_DT({bp_ulPhyId5,                .u.ul = 0x1f | PHY_EXTERNAL | PHY_TYPE_CL45GPHY},)
    MOVED_DT({bp_ulPhyId6,                .u.ul = 7 | PHY_INTEGRATED_VALID | MAC_IF_SERDES},)
    MOVED_DT({bp_ulPhyId6,                .u.ul = 0x1e | PHY_EXTERNAL | PHY_TYPE_CL45GPHY},)

    {bp_last}
};

static bp_elem_t g_bcm96757ref2t[] = {
    {bp_cpBoardId,               .u.cp = "96757REF2T"},

    MOVED_DT({bp_usGphyBaseAddress,       .u.us = BCM96756_PHY_BASE},)
    MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_NO_PHY},)
    MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
    MOVED_DT({bp_ulPortMap,               .u.ul = 0x61},)
    MOVED_DT({bp_ulPhyId0,                .u.ul = (BCM96756_PHY_BASE + 0x00) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},)
    MOVED_DT({bp_ulPhyId5,                .u.ul = 6 | PHY_INTEGRATED_VALID | MAC_IF_SERDES},)
    MOVED_DT({bp_ulPhyId5,                .u.ul = 0x1f | PHY_EXTERNAL | PHY_TYPE_CL45GPHY},)
    MOVED_DT({bp_ulPhyId6,                .u.ul = 7 | PHY_INTEGRATED_VALID | MAC_IF_SERDES},)
    MOVED_DT({bp_ulPhyId6,                .u.ul = 0x1e | PHY_EXTERNAL | PHY_TYPE_CL45GPHY},)

    {bp_last}
};

static bp_elem_t g_bcm96756rfdvt_fake[] = {
    {bp_cpBoardId,               .u.cp = "96756RFDVT_FAKE"},

    MOVED_DT({bp_usGphyBaseAddress,       .u.us = BCM96756_PHY_BASE},) 
    MOVED_DT({bp_ucPhyType0,              .u.uc = BP_ENET_NO_PHY},)
    MOVED_DT({bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},)
    MOVED_DT({bp_ulPortMap,               .u.ul = 0x3},)
    MOVED_DT({bp_ulPhyId0,                .u.ul = (BCM96756_PHY_BASE + 0x00) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},)
    MOVED_DT({bp_ulPhyId1,                .u.ul = 0x19 |  PHY_INTEGRATED_VALID | MAC_IF_RGMII_1P8V | PHY_EXTERNAL},)
    {bp_last}
};


bp_elem_t * g_BoardParms[] = {g_bcm96756sv, g96756ref1, g96756ref1_sg, g_bcm96756rfdvt, g_bcm947623eap6l, g_bcm947623eap6ll,
                              g_bcm96757sv, g_bcm96757ref1t, g_bcm96757ref2t, g_bcm96756rfdvt_fake, 0};
