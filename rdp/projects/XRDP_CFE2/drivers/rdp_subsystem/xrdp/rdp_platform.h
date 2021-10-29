/*
    <:copyright-BRCM:2015:DUAL/GPL:standard
    
       Copyright (c) 2015 Broadcom 
       All Rights Reserved
    
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License, version 2, as published by
    the Free Software Foundation (the "GPL").
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    
    A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
    writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
    
    :>
*/

#ifndef RDP_PLATFORM_H_INCLUDED
#define RDP_PLATFORM_H_INCLUDED

#ifdef __cplusplus
extern "C"
{
#endif


/* SBPM */
#define SBPM_BASE_ADDRESS                   0
#define SBPM_INIT_OFFSET                    (SBPM_MAX_BUFFER_NUMBER)
#define SBPM_MAX_NUM_OF_BNS                 (SBPM_MAX_BUFFER_NUMBER + 1)

typedef enum rnr_quad_id_e
{
    RNR_QUAD0_ID = 0,
    RNR_QUAD_ID_LAST = RNR_QUAD0_ID,
    NUM_OF_RNR_QUADS = RNR_QUAD_ID_LAST + 1,
} rnr_quad_id_e;

#if  defined(BCM6858)

#define RNR_FREQ_IN_MHZ 1000
#define UBUS_SLV_FREQ_IN_MHZ 500
#define NUM_OF_CORES_IN_QUAD 4

/* DISP CONGESTION */
#define DIS_REOR_CONGESTION_THRESHOLD    450
#define DIS_REOR_LINKED_LIST_BUFFER_NUM  1024
#define NUM_OF_PROCESSING_TASKS          96

/* BBH_RX */
#define DRV_BBH_RX_MAXIMAL_PACKET_HEADER_OFFSET 127

/* BBH_TX */
#define BBH_TX_DS_PD_FIFO_SIZE_0 8
#define BBH_TX_DS_PD_FIFO_SIZE_1 8

#define IS_WAN_TX_PORT(bbh_id) (bbh_id == BBH_ID_PON)
#define IS_SDMA(dma_id)     (dma_id > DMA1_ID)
#define IS_DMA(dma_id)      (dma_id <= DMA1_ID)

typedef enum dma_id_e
{
    DMA0_ID = 0,
    DMA1_ID,
    SDMA0_ID,
    SDMA1_ID,
    DMA_NUM,
    DMA_ID_FIRST = DMA0_ID,
} dma_id_e;

typedef enum bbh_id_e
{
    BBH_ID_0 = 0,  /* XLMAC1_0_RGMII */
    BBH_ID_1,      /* XLMAC0_1_2p5G  */
    BBH_ID_2,      /* XLMAC0_2_1G    */
    BBH_ID_3,      /* XLMAC0_3_1G    */
    BBH_ID_4,      /* XLMAC0_0_10G   */
    BBH_ID_5,      /* XLMAC1_1_RGMII */
    BBH_ID_6,      /* XLMAC1_2_RGMII */
    BBH_ID_7,      /* XLMAC1_3_RGMII */
    BBH_ID_PON,
    BBH_ID_NUM,
    BBH_ID_FIRST = BBH_ID_0,
    BBH_ID_LAST = BBH_ID_PON,
    BBH_ID_LAST_XLMAC = BBH_ID_7,
    BBH_ID_NULL = BBH_ID_NUM
} bbh_id_e;


/* SBPM */

#define SBPM_MAX_BUFFER_NUMBER              0xFFF


/**********************************************************************************************************/
#elif  defined(BCM6846)

#define RNR_FREQ_IN_MHZ 1400
#define UBUS_SLV_FREQ_IN_MHZ 466
#define NUM_OF_CORES_IN_QUAD 3

#define IS_WAN_TX_PORT(bbh_id) (bbh_id == BBH_TX_WAN_ID)
#define IS_WAN_RX_PORT(bbh_id) (bbh_id == BBH_ID_PON)
#define IS_SDMA(dma_id)  (dma_id > DMA0_ID)
#define IS_DMA(dma_id)   (dma_id <= DMA0_ID)


/* DISP CONGESTION */
#define DIS_REOR_CONGESTION_THRESHOLD    200
#define DIS_REOR_LINKED_LIST_BUFFER_NUM  512
#define NUM_OF_PROCESSING_TASKS          12

/* SBPM */

#define SBPM_MAX_BUFFER_NUMBER              0x5FF


/* BBH_RX */
#define DRV_BBH_RX_MAXIMAL_PACKET_HEADER_OFFSET 63
#define BBH_FREQUENCY (466770000) /* 466.77 MHz */

/* BBH_TX */
#define BBH_TX_DS_PD_FIFO_SIZE_0 7
#define BBH_TX_DS_PD_FIFO_SIZE_1 7

typedef enum bbh_id_e
{
    BBH_ID_0 = 0,
    BBH_ID_1,
    BBH_ID_2,
    BBH_ID_3,
    BBH_ID_4,
    BBH_ID_PON,
    BBH_ID_NUM,
    BBH_ID_FIRST = BBH_ID_0,
    BBH_ID_LAST = BBH_ID_PON,
    BBH_ID_NULL = BBH_ID_NUM
} bbh_id_e;

typedef enum dma_id_e
{
    DMA0_ID = 0,
    SDMA0_ID,
    DMA_NUM,
    DMA_ID_FIRST = DMA0_ID,
} dma_id_e;

/**********************************************************************************************************/

#elif  defined(BCM6878)
#define RNR_FREQ_IN_MHZ 1000
#define UBUS_SLV_FREQ_IN_MHZ 466
#define NUM_OF_CORES_IN_QUAD 3

#define IS_WAN_TX_PORT(bbh_id) (bbh_id == BBH_TX_WAN_ID)
#define IS_WAN_RX_PORT(bbh_id) (bbh_id == BBH_ID_PON)
#define IS_SDMA(dma_id)  (dma_id > DMA0_ID)
#define IS_DMA(dma_id)   (dma_id <= DMA0_ID)


/* DISP CONGESTION */
#define DIS_REOR_CONGESTION_THRESHOLD    100
#define DIS_REOR_LINKED_LIST_BUFFER_NUM  256
#define NUM_OF_PROCESSING_TASKS          12

/* BBH_RX */
#define DRV_BBH_RX_MAXIMAL_PACKET_HEADER_OFFSET 63
#define BBH_FREQUENCY (466770000) /* 466.77 MHz */

/* BBH_TX */
#define BBH_TX_DS_PD_FIFO_SIZE_0 7
#define BBH_TX_DS_PD_FIFO_SIZE_1 7

typedef enum bbh_id_e
{
    BBH_ID_0 = 0,
    BBH_ID_1,
    BBH_ID_2,
    BBH_ID_3,
    BBH_ID_4,
    BBH_ID_PON,
    BBH_ID_NUM,
    BBH_ID_FIRST = BBH_ID_0,
    BBH_ID_LAST = BBH_ID_PON,
    BBH_ID_NULL = BBH_ID_NUM
} bbh_id_e;

typedef enum dma_id_e
{
    DMA0_ID = 0,
    DMA_NUM,
    DMA_ID_FIRST = DMA0_ID,
} dma_id_e;

typedef enum bbh_tx_id_e
{
    BBH_TX_ID_LAN = 0,
    BBH_TX_ID_PON,
    BBH_TX_ID_NUM,
    BBH_TX_ID_FIRST = BBH_TX_ID_LAN,
    BBH_TX_ID_LAST = BBH_TX_ID_PON,
    BBH_TX_ID_NULL = BBH_TX_ID_NUM
} bbh_tx_id_e;

/* SBPM */
#define SBPM_MAX_BUFFER_NUMBER              0x3FF



#elif defined(BCM6856) 

#define RNR_FREQ_IN_MHZ 1400
#define UBUS_SLV_FREQ_IN_MHZ 500
#define NUM_OF_CORES_IN_QUAD 4

#define IS_WAN_TX_PORT(bbh_id) (bbh_id == BBH_TX_WAN_ID)
#define IS_SDMA(dma_id)  (dma_id > DMA0_ID)
#define IS_DMA(dma_id)   (dma_id <= DMA0_ID)

/* DISP CONGESTION */
#define DIS_REOR_CONGESTION_THRESHOLD    200
#define DIS_REOR_LINKED_LIST_BUFFER_NUM  1024
#define NUM_OF_PROCESSING_TASKS          32

/* SBPM */
#define SBPM_UG0_BN_THRESHOLD               0x3FF
#define SBPM_MAX_BUFFER_NUMBER              0x7FF

/* BBH_RX */
#define DRV_BBH_RX_MAXIMAL_PACKET_HEADER_OFFSET 63
#define BBH_FREQUENCY (500*1000000) /* 500 MHz */

/* BBH_TX */
#define BBH_TX_DS_PD_FIFO_SIZE_0 7
#define BBH_TX_DS_PD_FIFO_SIZE_1 7

typedef enum dma_id_e
{
    DMA0_ID = 0,
    DMA1_ID,
    SDMA0_ID,
    SDMA1_ID,
    DMA_NUM,
    DMA_ID_FIRST = DMA0_ID,
} dma_id_e;

typedef enum bbh_id_e
{
    BBH_ID_0 = 0,
    BBH_ID_1,
    BBH_ID_2,
    BBH_ID_3,
    BBH_ID_4,
    BBH_ID_5,
    BBH_ID_PON,
    BBH_ID_NUM,
    BBH_ID_FIRST = BBH_ID_0,
    BBH_ID_LAST = BBH_ID_PON,
    BBH_ID_NULL = BBH_ID_NUM
} bbh_id_e;

typedef enum bbh_tx_id_e
{
    BBH_TX_ID_LAN = 0,
    BBH_TX_ID_PON,
    BBH_TX_ID_NUM,
    BBH_TX_ID_FIRST = BBH_TX_ID_LAN,
    BBH_TX_ID_LAST = BBH_TX_ID_PON,
    BBH_TX_ID_NULL = BBH_TX_ID_NUM
} bbh_tx_id_e;

#elif defined(BCM63146) || defined(BCM4912)

#define RNR_FREQ_IN_MHZ 1250
#define UBUS_SLV_FREQ_IN_MHZ 560 /* TBD63146. Need for BCM63146 */
/* TBD below */
#define NUM_OF_CORES_IN_QUAD 3

/* IS_WAN_TX_PORT and IS_WAN_RX_PORT identify TX WAN ports for DSL only (NOT Ethernet) */
#define IS_WAN_TX_PORT(bbh_id) (bbh_id == BBH_TX_ID_DSL)
#define IS_WAN_RX_PORT(bbh_id) (bbh_id == BBH_ID_DSL)
/* IS_DS_WAN_PORT and IS_US_WAN_PORT identify ALL (DS/US respectively) WAN ports */
/* TBD63146. Identify Ethernet WAN */
#define IS_DS_WAN_PORT(bbh_id)  (bbh_id == BBH_ID_DSL)
#define IS_US_WAN_PORT(bbh_id) (bbh_id == BBH_TX_ID_DSL)

/* DISP CONGESTION */
#define DIS_REOR_CONGESTION_THRESHOLD     200
#define DIS_REOR_LINKED_LIST_BUFFER_NUM   512
#define NUM_OF_PROCESSING_TASKS           32

/* SBPM */
#define SBPM_MAX_BUFFER_NUMBER              0x17F
#define SBPM_UG0_BN_THRESHOLD               0x15F

/* BBH_RX */
#define DRV_BBH_RX_MAXIMAL_PACKET_HEADER_OFFSET 127
#define BBH_FREQUENCY (560*1000000) /* 560 MHz */ /* TBD63146. Need for BCM63146 */

/* BBH_TX */
#define BBH_TX_DS_PD_FIFO_SIZE_0 7
#define BBH_TX_DS_PD_FIFO_SIZE_1 7

#if defined(BCM63146)
typedef enum dma_id_e
{
    DMA0_ID = 0,
    DMA1_ID,
    DMA_NUM,
    DMA_ID_FIRST = DMA0_ID,
} dma_id_e;

typedef enum bbh_id_e
{
    /* RX */
    /* RX BBH_ID needs to be the same as DISP_REOR_VIQ defined in
     * project_data_structures.xml or else bbh_rx configuration will break */
    BBH_ID_0 = 0,
    BBH_ID_1,
    BBH_ID_2,
    BBH_ID_3,
    BBH_ID_4,
    BBH_ID_5,
    BBH_ID_6_2P5G,
    BBH_ID_7_5G,
    BBH_ID_DSL,
    BBH_ID_NUM,
    BBH_ID_FIRST = BBH_ID_0,
    BBH_ID_NUM_LAN = BBH_ID_7_5G,
    BBH_ID_LAST = BBH_ID_DSL,
    BBH_ID_NULL = BBH_ID_NUM,
} bbh_id_e;

typedef enum bbh_tx_id_e
{
    BBH_TX_ID_LAN = 0,
    BBH_ID_QM_COPY,
    BBH_TX_ID_DSL,
    BBH_TX_ID_NUM,
    BBH_TX_ID_FIRST = BBH_TX_ID_LAN,
    BBH_TX_ID_LAST = BBH_TX_ID_DSL,
    BBH_TX_ID_NULL = BBH_ID_NULL,
} bbh_tx_id_e;

#define SDMA_U_THRESH_IN_BBH_LAN_VALUE    ( 0x2 )
#define SDMA_U_THRESH_OUT_BBH_LAN_VALUE   ( 0x1 )

#define SDMA_U_THRESH_IN_BBH_5G_VALUE     ( 0x3 )
#define SDMA_U_THRESH_OUT_BBH_5G_VALUE    ( 0x2 )

#define SDMA_U_THRESH_IN_BBH_2P5G_VALUE   ( 0x3 )
#define SDMA_U_THRESH_OUT_BBH_2P5G_VALUE  ( 0x2 )

#define SDMA_U_THRESH_IN_BBH_DSL_VALUE    ( 0x6 )
#define SDMA_U_THRESH_OUT_BBH_DSL_VALUE   ( 0x4 )

#define SDMA_STRICT_PRI_RX_BBH_LAN_VALUE  ( 0x4 )
#define SDMA_STRICT_PRI_RX_BBH_5G_VALUE   ( 0x04 )
#define SDMA_STRICT_PRI_RX_BBH_2P5G_VALUE ( 0x04 )
#define SDMA_STRICT_PRI_RX_BBH_DSL_VALUE  ( 0x4 )

#define SDMA_STRICT_PRI_TX_BBH_LAN_VALUE  ( 0x04 )
#define DMA_STRICT_PRI_TX_BBH_LAN_VALUE   ( 0x08 )
#define SDMA_STRICT_PRI_TX_BBH_COPY_VALUE ( 0x04 )
#define DMA_STRICT_PRI_TX_BBH_COPY_VALUE  ( 0x08 )
#define SDMA_STRICT_PRI_TX_BBH_DSL_VALUE  ( 0x04 )
#define DMA_STRICT_PRI_TX_BBH_DSL_VALUE   ( 0x08 )

#define SDMA_RR_WEIGHT_RX_BBH_LAN_VALUE   ( 0x00 )
#define SDMA_RR_WEIGHT_RX_BBH_5G_VALUE    ( 0x00 )
#define SDMA_RR_WEIGHT_RX_BBH_2P5G_VALUE  ( 0x00 )
#define SDMA_RR_WEIGHT_RX_BBH_DSL_VALUE   ( 0x00 )

#define SDMA_RR_WEIGHT_TX_BBH_LAN_VALUE   ( 0x00 )
#define DMA_RR_WEIGHT_TX_BBH_LAN_VALUE    ( 0x00 )
#define SDMA_RR_WEIGHT_TX_BBH_COPY_VALUE  ( 0x00 )
#define DMA_RR_WEIGHT_TX_BBH_COPY_VALUE   ( 0x00 )
#define SDMA_RR_WEIGHT_TX_BBH_DSL_VALUE   ( 0x00 )
#define DMA_RR_WEIGHT_TX_BBH_DSL_VALUE    ( 0x00 )
#else /* if defined(BCM4912) */
typedef enum dma_id_e
{
    DMA0_ID = 0,
    DMA1_ID,
    DMA_COPY_ID,
    DMA_NUM,
    DMA_ID_FIRST = DMA0_ID,
} dma_id_e;

typedef enum bbh_id_e
{
    /* RX */
    /* RX BBH_ID needs to be the same as DISP_REOR_VIQ defined in
     * project_data_structures.xml or else bbh_rx configuration will break */
    BBH_ID_0 = 0,
    BBH_ID_1,
    BBH_ID_2,
    BBH_ID_3,
    BBH_ID_4,
    BBH_ID_5_10G,
    BBH_ID_6_5G,
    BBH_ID_7_10G,
    BBH_ID_NUM,
    BBH_ID_FIRST = BBH_ID_0,
    BBH_ID_NUM_LAN = BBH_ID_7_10G,
    BBH_ID_LAST = BBH_ID_7_10G,
    BBH_ID_NULL = BBH_ID_NUM,
    BBH_ID_DSL = BBH_ID_NULL,
} bbh_id_e;

typedef enum bbh_tx_id_e
{
    BBH_TX_ID_LAN = 0,
    /* BBH Queue 0-3: QGPHY
     * BBH Queue 4: RGMII
     * BBH Queue 5: XLMAC 0, port 0, 10Gbps */
    BBH_ID_QM_COPY,
    BBH_TX_ID_LAN_1,
    /* BBH Queue 0: XLMAC 0, port 2, 5Gbps
     * BBH Queue 1: XLMAC 2, port 0, 10Gbps */
    BBH_TX_ID_NUM,
    BBH_TX_ID_FIRST = BBH_TX_ID_LAN,
    BBH_TX_ID_LAST = BBH_TX_ID_LAN_1,
    BBH_TX_ID_NULL = BBH_ID_NULL,
    BBH_TX_ID_DSL = BBH_ID_NULL,
} bbh_tx_id_e;

typedef enum xlif_id_e
{
    XLIF_ID_CHANNEL_FIRST = 0,
    XLIF_ID_CHANNEL_0 = XLIF_ID_CHANNEL_FIRST,
    XLIF_ID_CHANNEL_1,
    XLIF_ID_CHANNEL_2,
    XLIF_ID_CHANNEL_3,
    XLIF_ID_CHANNEL_NUM,
} xlif_id_e;

#define SDMA_U_THRESH_IN_BBH_LAN_VALUE          ( 0x02 )
#define SDMA_U_THRESH_OUT_BBH_LAN_VALUE         ( 0x01 )

#define SDMA_U_THRESH_IN_BBH_5G_VALUE           ( 0x03 )
#define SDMA_U_THRESH_OUT_BBH_5G_VALUE          ( 0x02 )

#define SDMA_U_THRESH_IN_BBH_10G_VALUE          ( 0x06 )
#define SDMA_U_THRESH_OUT_BBH_10G_VALUE         ( 0x04 )

#define SDMA_STRICT_PRI_RX_BBH_LAN_VALUE        ( 0x04 )
#define SDMA_STRICT_PRI_RX_BBH_5G_VALUE         ( 0x04 )
#define SDMA_STRICT_PRI_RX_BBH_10G_VALUE        ( 0x04 )

#define SDMA_STRICT_PRI_TX_BBH_LAN_VALUE        ( 0x04 )
#define DMA_STRICT_PRI_TX_BBH_LAN_VALUE         ( 0x08 )
#define SDMA_STRICT_PRI_TX_BBH_LAN_1_VALUE      ( 0x04 )
#define DMA_STRICT_PRI_TX_BBH_LAN_1_VALUE       ( 0x08 )
#define SDMA_STRICT_PRI_TX_BBH_COPY_VALUE       ( 0x04 )
#define DMA_STRICT_PRI_TX_BBH_COPY_VALUE        ( 0x08 )

#define SDMA_RR_WEIGHT_RX_BBH_LAN_VALUE         ( 0x00 )
#define SDMA_RR_WEIGHT_RX_BBH_5G_VALUE          ( 0x00 )
#define SDMA_RR_WEIGHT_RX_BBH_10G_VALUE         ( 0x00 )

#define SDMA_RR_WEIGHT_TX_BBH_LAN_VALUE         ( 0x00 )
#define DMA_RR_WEIGHT_TX_BBH_LAN_VALUE          ( 0x00 )
#define SDMA_RR_WEIGHT_TX_BBH_LAN_1_VALUE       ( 0x00 )
#define DMA_RR_WEIGHT_TX_BBH_LAN_1_VALUE        ( 0x00 )
#define SDMA_RR_WEIGHT_TX_BBH_COPY_VALUE        ( 0x00 )
#define DMA_RR_WEIGHT_TX_BBH_COPY_VALUE         ( 0x00 )
#endif

typedef enum dma_target_e
{
    DMA_TARGET_DMA = 0,
    DMA_TARGET_SDMA,
    DMA_TARGET_NUM,
    DMA_TARGET_FIRST = DMA_TARGET_DMA,
} dma_target_e;

/* some change of definition for easy to compilation */
#define BBH_ID_PON  BBH_ID_DSL
#define BB_ID_RX_PON  BB_ID_RX_DSL
#define BB_ID_TX_PON_ETH_PD  BB_ID_TX_DSL
#define BB_ID_TX_PON_ETH_STAT  BB_ID_TX_DSL_STAT

#endif


#if !defined(BCM63146) && !defined(BCM4912)
#define DMA_U_THRESH_IN_BBH_LAN_VALUE     ( 0x3 )
#define DMA_U_THRESH_OUT_BBH_LAN_VALUE    ( 0x2 )
#define SDMA_U_THRESH_IN_BBH_LAN_VALUE    ( 0x3 )
#define SDMA_U_THRESH_OUT_BBH_LAN_VALUE   ( 0x2 )

#define DMA_U_THRESH_IN_BBH_PON_VALUE  ( 0x8 )
#define DMA_U_THRESH_OUT_BBH_PON_VALUE ( 0x6 )
#define SDMA_U_THRESH_IN_BBH_PON_VALUE    ( 0x8 )
#define SDMA_U_THRESH_OUT_BBH_PON_VALUE   ( 0x6 )

#define DMA_STRICT_PRI_RX_BBH_LAN_VALUE   ( 0x4 )
#define DMA_STRICT_PRI_TX_BBH_LAN_VALUE   ( 0x8 )
#define SDMA_STRICT_PRI_RX_BBH_LAN_VALUE  ( 0x4 )
#define SDMA_STRICT_PRI_TX_BBH_LAN_VALUE  ( 0x8 )

#define DMA_STRICT_PRI_RX_BBH_PON_VALUE   ( 0x4 )
#define DMA_STRICT_PRI_TX_BBH_PON_VALUE   ( 0x8 )
#define SDMA_STRICT_PRI_RX_BBH_PON_VALUE  ( 0x4 )
#define SDMA_STRICT_PRI_TX_BBH_PON_VALUE  ( 0x8 )

#define DMA_RR_WEIGHT_RX_BBH_LAN_VALUE    ( 0x0 )
#define DMA_RR_WEIGHT_TX_BBH_LAN_VALUE    ( 0x0 )

#define SDMA_RR_WEIGHT_RX_BBH_LAN_VALUE   ( 0x0 )
#define SDMA_RR_WEIGHT_TX_BBH_LAN_VALUE   ( 0x0 )

#define DMA_RR_WEIGHT_RX_BBH_PON_VALUE    ( 0x0 )
#define DMA_RR_WEIGHT_TX_BBH_PON_VALUE    ( 0x0 )
#define SDMA_RR_WEIGHT_RX_BBH_PON_VALUE   ( 0x0 )
#define SDMA_RR_WEIGHT_TX_BBH_PON_VALUE   ( 0x0 )
#endif

#endif
