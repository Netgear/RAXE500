/*
   Copyright (c) 2015 Broadcom
   All Rights Reserved

    <:label-BRCM:2015:DUAL/GPL:standard
    
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



/* This is an automated file. Do not edit its contents. */


#include "bdmf_shell.h"
#include "rdd_map_auto.h"
#include "rdd_runner_reg_dump.h"
#include "rdd_runner_reg_dump_addrs.h"
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DS_TM_PD_FIFO_TABLE =
{
	16,
	{
		{ dump_RDD_PROCESSING_TX_DESCRIPTOR, 0x0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SERVICE_QUEUES_PD_FIFO_TABLE =
{
	16,
	{
		{ dump_RDD_PROCESSING_TX_DESCRIPTOR, 0x800 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DS_TM_SCHEDULING_QUEUE_TABLE =
{
	8,
	{
		{ dump_RDD_SCHEDULING_QUEUE_DESCRIPTOR, 0xc00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT BASIC_SCHEDULER_TABLE_DS =
{
	16,
	{
		{ dump_RDD_BASIC_SCHEDULER_DESCRIPTOR, 0xe00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT IMG0_PACKET_BUFFER =
{
	512,
	{
		{ dump_RDD_PACKET_BUFFER, 0x1000 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT LAYER2_GRE_TUNNEL_TABLE =
{
	32,
	{
		{ dump_RDD_LAYER2_GRE_TUNNEL_ENTRY, 0x1800 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT COMPLEX_SCHEDULER_TABLE =
{
	64,
	{
		{ dump_RDD_COMPLEX_SCHEDULER_DESCRIPTOR, 0x1980 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT VPORT_CFG_TABLE =
{
	8,
	{
		{ dump_RDD_VPORT_CFG_ENTRY, 0x1a00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DS_TM_TM_FLOW_CNTR_TABLE =
{
	1,
	{
		{ dump_RDD_TM_FLOW_CNTR_ENTRY, 0x1b40 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT VPORT_TX_FLOW_TABLE =
{
	1,
	{
		{ dump_RDD_TX_FLOW_ENTRY, 0x1b80 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT GPE_COMMAND_PRIMITIVE_TABLE =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0x1bc0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT FW_POLICER_CBS =
{
	4,
	{
		{ dump_RDD_CBS_ENTRY, 0x1c00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT NATC_COUNTERS_GENERIC_CONTEXT =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x1d40 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SERVICE_QUEUES_COMPLEX_SCHEDULER_TABLE =
{
	64,
	{
		{ dump_RDD_COMPLEX_SCHEDULER_DESCRIPTOR, 0x1dc0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DSL_TX_FLOW_TABLE =
{
	1,
	{
		{ dump_RDD_TX_FLOW_ENTRY, 0x1e00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SERVICE_QUEUES_SCHEDULING_QUEUE_TABLE =
{
	8,
	{
		{ dump_RDD_SCHEDULING_QUEUE_DESCRIPTOR, 0x1f00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT BASIC_RATE_LIMITER_TABLE_DS =
{
	16,
	{
		{ dump_RDD_BASIC_RATE_LIMITER_DESCRIPTOR, 0x2000 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT RX_FLOW_TABLE =
{
	2,
	{
		{ dump_RDD_RX_FLOW_ENTRY, 0x2800 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT MUTEX_TABLE =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x28b4 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT TM_BBH_TX_WAKE_UP_DATA_TABLE =
{
	8,
	{
		{ dump_RDD_BBH_TX_WAKE_UP_DATA_ENTRY, 0x28b8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT LOOPBACK_QUEUE_TABLE =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x28c0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT IPTV_CONFIGURATION_TABLE =
{
	8,
	{
		{ dump_RDD_HW_IPTV_CONFIGURATION, 0x28e8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SPDSVC_ANALYZER_PARAMS_TABLE =
{
	16,
	{
		{ dump_RDD_SPDSVC_ANALYZER_PARAMS, 0x28f0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT FW_POLICER_BUDGET =
{
	2,
	{
		{ dump_RDD_FW_POLICER_BUDGET_ENTRY, 0x2900 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT FW_POLICER_BUDGET_REMAINDER =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x29a0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT INGRESS_FILTER_L2_REASON_TABLE =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x29f0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT INGRESS_FILTER_PROFILE_TABLE =
{
	8,
	{
		{ dump_RDD_INGRESS_FILTER_CTRL, 0x2a00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT REGISTERS_BUFFER =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2a80 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT VPORT_CFG_EX_TABLE =
{
	4,
	{
		{ dump_RDD_VPORT_CFG_EX_ENTRY, 0x2b00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DS_TM_FLUSH_CFG_CPU_TABLE =
{
	4,
	{
		{ dump_RDD_FLUSH_CFG_ENTRY, 0x2b7c },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT QUEUE_THRESHOLD_VECTOR =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2b80 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DS_TM_FLUSH_CFG_FW_TABLE =
{
	4,
	{
		{ dump_RDD_FLUSH_CFG_ENTRY, 0x2ba4 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT GENERAL_QUEUE_DYNAMIC_MNG_TABLE =
{
	8,
	{
		{ dump_RDD_QUEUE_DYNAMIC_MNG_ENTRY, 0x2ba8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DS_TM_FLUSH_DISPATCHER_CREDIT_TABLE =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2bb0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SERVICE_QUEUES_FLUSH_CFG_FW_TABLE =
{
	4,
	{
		{ dump_RDD_FLUSH_CFG_ENTRY, 0x2bbc },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT FLOW_BASED_ACTION_PTR_TABLE =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0x2bc0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT BUDGET_ALLOCATION_TIMER_VALUE =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0x2be2 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SERVICE_QUEUES_FLUSH_CFG_CURRENT_TABLE =
{
	4,
	{
		{ dump_RDD_FLUSH_CFG_ENTRY, 0x2be4 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT RX_FLOW_CONTEXT_DDR_ADDR =
{
	8,
	{
		{ dump_RDD_DEF_FLOW_CONTEXT_DDR_ADDR, 0x2be8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT FW_POLICER_VECTOR =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2bf0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SPDTEST_GEN_PARAM =
{
	4,
	{
		{ dump_RDD_SPDTEST_GEN_CFG, 0x2bfc },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT NATC_TBL_CFG =
{
	24,
	{
		{ dump_RDD_NATC_TBL_CONFIGURATION, 0x2c00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT FPM_POOL_NUMBER_MAPPING_TABLE =
{
	1,
	{
		{ dump_RDD_FPM_POOL_NUMBER, 0x2c60 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DS_TM_TM_ACTION_PTR_TABLE =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0x2c80 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DS_TM_BB_DESTINATION_TABLE =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0x2ca2 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT TIMER_COMMON_TIMER_VALUE =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0x2ca4 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SERVICE_QUEUES_BUDGET_ALLOCATION_TIMER_VALUE =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0x2ca6 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT NULL_BUFFER =
{
	8,
	{
		{ dump_RDD_BYTES_8, 0x2ca8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SERVICE_QUEUES_DISPATCHER_CREDIT_TABLE =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2cb0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT TASK_IDX =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2cbc },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT VLAN_ACTION_GPE_HANDLER_PTR_TABLE =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0x2cc0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT FORCE_DSCP =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2ce2 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CORE_ID_TABLE =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2ce3 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT EMAC_FLOW_CTRL_VECTOR =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2ce4 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SRAM_DUMMY_STORE =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2ce5 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT RX_MIRRORING_CONFIGURATION =
{
	2,
	{
		{ dump_RDD_MIRRORING_DESCRIPTOR, 0x2ce6 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT TIMER_COMMON_CTR_REP =
{
	8,
	{
		{ dump_RDD_BYTES_8, 0x2ce8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_FPM_THRESHOLDS =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2cf0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT RATE_LIMIT_OVERHEAD =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2cfc },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT INGRESS_QOS_DONT_DROP_RATIO =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2cfd },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DS_TM_FIRST_QUEUE_MAPPING =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2cfe },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT MULTI_FLOW_FLAG_TABLE =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2cff },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT UPDATE_FIFO_TABLE =
{
	4,
	{
		{ dump_RDD_UPDATE_FIFO_ENTRY, 0x2d00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT VPORT_TO_LOOKUP_PORT_MAPPING_TABLE =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2d20 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT EMAC_FLOW_CTRL_BUDGET =
{
	4,
	{
		{ dump_RDD_EMAC_FLOW_CTRL_BUDGET_ENTRY, 0x2d40 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DS_TM_SCHEDULING_QUEUE_AGING_VECTOR =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2d60 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_REDIRECT_MODE =
{
	1,
	{
		{ dump_RDD_CPU_REDIRECT_MODE_ENTRY, 0x2d74 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SERVICE_QUEUES_FIRST_QUEUE_MAPPING =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2d75 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DEBUG_PRINT_CORE_LOCK =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2d76 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT IPTV_CLASSIFICATION_CFG_TABLE =
{
	1,
	{
		{ dump_RDD_IPTV_CLASSIFICATION_CFG_ENTRY, 0x2d77 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SYSTEM_CONFIGURATION =
{
	5,
	{
		{ dump_RDD_SYSTEM_CONFIGURATION_ENTRY, 0x2d78 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT INGRESS_FILTER_1588_CFG =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2d7d },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT RX_MIRRORING_DIRECT_ENABLE =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2d7e },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SERVICE_QUEUES_UPDATE_FIFO_TABLE =
{
	4,
	{
		{ dump_RDD_UPDATE_FIFO_ENTRY, 0x2d80 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DEBUG_SCRATCHPAD =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2da0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SPDSVC_BBH_TX_PARAMS_TABLE =
{
	12,
	{
		{ dump_RDD_SPDSVC_BBH_TX_PARAMS, 0x2dd0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SERVICE_QUEUES_SCHEDULING_QUEUE_AGING_VECTOR =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2de0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT EMAC_FLOW_CTRL_BUDGET_REMAINDER =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2df8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DS_TM_BBH_QUEUE_TABLE =
{
	4,
	{
		{ dump_RDD_BBH_QUEUE_DESCRIPTOR, 0x2e00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DS_TM_SCHEDULING_AGGREGATION_CONTEXT_VECTOR =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2e20 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DS_TM_FLUSH_CFG_CURRENT_TABLE =
{
	4,
	{
		{ dump_RDD_FLUSH_CFG_ENTRY, 0x2e38 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SERVICE_QUEUES_RATE_LIMITER_VALID_TABLE =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2e40 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT FPM_GLOBAL_CFG =
{
	16,
	{
		{ dump_RDD_FPM_GLOBAL_CFG, 0x2e50 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT TCAM_IC_CFG_TABLE =
{
	16,
	{
		{ dump_RDD_TCAM_IC_CFG, 0x2e60 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT BITS_CALC_MASKS_TABLE =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2e80 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_FEED_RING_DESCRIPTOR_TABLE =
{
	16,
	{
		{ dump_RDD_CPU_RING_DESCRIPTOR, 0x2e90 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT RATE_LIMITER_VALID_TABLE_DS =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2ea0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DEBUG_PRINT_TABLE =
{
	16,
	{
		{ dump_RDD_DEBUG_PRINT_INFO, 0x2eb0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DS_TM_BBH_TX_EGRESS_COUNTER_TABLE =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2ec0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT TUNNELS_PARSING_CFG =
{
	8,
	{
		{ dump_RDD_TUNNELS_PARSING_CFG, 0x2ec8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT IPTV_CFG_TABLE =
{
	8,
	{
		{ dump_RDD_IPTV_CFG, 0x2ed0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT IPTV_DDR_CTX_TABLE_ADDRESS =
{
	8,
	{
		{ dump_RDD_IPTV_DDR_CTX_TABLE_ADDRESS, 0x2ed8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT MULTICAST_KEY_MASK =
{
	4,
	{
		{ dump_RDD_MULTICAST_KEY_MASK_ENTRY, 0x2ee0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT NAT_CACHE_CFG =
{
	7,
	{
		{ dump_RDD_NAT_CACHE_CFG, 0x2ee8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DS_TM_FLUSH_CFG_ENABLE_TABLE =
{
	2,
	{
		{ dump_RDD_FLUSH_CFG_ENABLE_ENTRY, 0x2ef0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SERVICE_QUEUES_FLUSH_CFG_ENABLE_TABLE =
{
	2,
	{
		{ dump_RDD_FLUSH_CFG_ENABLE_ENTRY, 0x2ef8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DS_TM_BBH_TX_ABS_COUNTER_TABLE =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2f00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT NAT_CACHE_KEY0_MASK =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2f08 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT NATC_L2_VLAN_KEY_MASK =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0x2f10 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT INGRESS_FILTER_CFG =
{
	2,
	{
		{ dump_RDD_INGRESS_FILTER_CFG, 0x2f18 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT NATC_L2_TOS_MASK =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2f20 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SERVICE_QUEUES_RATE_LIMITER_TABLE =
{
	16,
	{
		{ dump_RDD_BASIC_RATE_LIMITER_DESCRIPTOR, 0x3000 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT FC_FLOW_IP_ADDRESSES_TABLE =
{
	48,
	{
		{ dump_RDD_FC_FLOW_IP_ADDRESSES_ENTRY, 0x3d00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT RUNNER_PROFILING_TRACE_BUFFER =
{
	4,
	{
		{ dump_RDD_TRACE_EVENT, 0x3e00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT US_TM_PD_FIFO_TABLE_1 =
{
	16,
	{
		{ dump_RDD_PROCESSING_TX_DESCRIPTOR, 0x0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT US_TM_SCHEDULING_QUEUE_TABLE_1 =
{
	8,
	{
		{ dump_RDD_SCHEDULING_QUEUE_DESCRIPTOR, 0x800 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT BASIC_SCHEDULER_TABLE_US_1 =
{
	16,
	{
		{ dump_RDD_BASIC_SCHEDULER_DESCRIPTOR, 0xa00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DSL_TM_PD_FIFO_TABLE_1 =
{
	16,
	{
		{ dump_RDD_PROCESSING_TX_DESCRIPTOR, 0xc00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT LAYER2_GRE_TUNNEL_TABLE_1 =
{
	32,
	{
		{ dump_RDD_LAYER2_GRE_TUNNEL_ENTRY, 0xe00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT COMPLEX_SCHEDULER_TABLE_1 =
{
	64,
	{
		{ dump_RDD_COMPLEX_SCHEDULER_DESCRIPTOR, 0xf80 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT IMG1_PACKET_BUFFER_1 =
{
	512,
	{
		{ dump_RDD_PACKET_BUFFER, 0x1000 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_COMPLETE_COMMON_RADIO_DATA_1 =
{
	112,
	{
		{ dump_RDD_DHD_COMPLETE_COMMON_RADIO_ENTRY, 0x1800 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DIRECT_PROCESSING_PD_TABLE_1 =
{
	16,
	{
		{ dump_RDD_PROCESSING_RX_DESCRIPTOR, 0x1950 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT FPM_POOL_NUMBER_MAPPING_TABLE_1 =
{
	1,
	{
		{ dump_RDD_FPM_POOL_NUMBER, 0x1960 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_RX_COMPLETE_FLOW_RING_BUFFER_1 =
{
	32,
	{
		{ dump_RDD_DHD_RX_COMPLETE_DESCRIPTOR, 0x1980 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT VPORT_TO_LOOKUP_PORT_MAPPING_TABLE_1 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x19e0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT VPORT_CFG_TABLE_1 =
{
	8,
	{
		{ dump_RDD_VPORT_CFG_ENTRY, 0x1a00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT US_TM_ETH_TM_FLOW_CNTR_TABLE_1 =
{
	1,
	{
		{ dump_RDD_TM_FLOW_CNTR_ENTRY, 0x1b40 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_RX_POST_FLOW_RING_BUFFER_1 =
{
	32,
	{
		{ dump_RDD_DHD_RX_POST_DESCRIPTOR, 0x1b80 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT PFC_TX_STATUS_TABLE_1 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x1be0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT US_TM_DSL_TM_FLOW_CNTR_TABLE_1 =
{
	1,
	{
		{ dump_RDD_TM_FLOW_CNTR_ENTRY, 0x1c00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DSL_TX_FLOW_TABLE_1 =
{
	1,
	{
		{ dump_RDD_TX_FLOW_ENTRY, 0x1d00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_RX_COMPLETE_TASK_0_STACK_1 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x1e00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_RX_COMPLETE_TASK_1_STACK_1 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x1f00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT BASIC_RATE_LIMITER_TABLE_US_1 =
{
	16,
	{
		{ dump_RDD_BASIC_RATE_LIMITER_DESCRIPTOR, 0x2000 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_RX_COMPLETE_TASK_2_STACK_1 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2800 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_TX_COMPLETE_TASK_0_STACK_1 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2900 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_TX_COMPLETE_TASK_1_STACK_1 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2a00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_TX_COMPLETE_TASK_2_STACK_1 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2b00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT RX_FLOW_TABLE_1 =
{
	2,
	{
		{ dump_RDD_RX_FLOW_ENTRY, 0x2c00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT MUTEX_TABLE_1 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2cb4 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT TM_DSL_BBH_TX_WAKE_UP_DATA_TABLE_1 =
{
	8,
	{
		{ dump_RDD_BBH_TX_WAKE_UP_DATA_ENTRY, 0x2cb8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT VPORT_TX_FLOW_TABLE_1 =
{
	1,
	{
		{ dump_RDD_TX_FLOW_ENTRY, 0x2cc0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT INGRESS_FILTER_PROFILE_TABLE_1 =
{
	8,
	{
		{ dump_RDD_INGRESS_FILTER_CTRL, 0x2d00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT GPE_COMMAND_PRIMITIVE_TABLE_1 =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0x2d80 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT PFC_FRAME_1 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2dc0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT VPORT_CFG_EX_TABLE_1 =
{
	4,
	{
		{ dump_RDD_VPORT_CFG_EX_ENTRY, 0x2e00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT US_TM_FLUSH_CFG_CPU_TABLE_1 =
{
	4,
	{
		{ dump_RDD_FLUSH_CFG_ENTRY, 0x2e7c },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT NATC_COUNTERS_GENERIC_CONTEXT_1 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2e80 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT NATC_TBL_CFG_1 =
{
	24,
	{
		{ dump_RDD_NATC_TBL_CONFIGURATION, 0x2f00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT REGISTERS_BUFFER_1 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2f60 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT PFC_PD_TABLE_1 =
{
	16,
	{
		{ dump_RDD_PROCESSING_RX_DESCRIPTOR, 0x2fe0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT US_TM_BBH_QUEUE_TABLE_1 =
{
	4,
	{
		{ dump_RDD_BBH_QUEUE_DESCRIPTOR, 0x3000 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT US_TM_SCHEDULING_QUEUE_AGING_VECTOR_1 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x3060 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT US_TM_FLUSH_CFG_FW_TABLE_1 =
{
	4,
	{
		{ dump_RDD_FLUSH_CFG_ENTRY, 0x3074 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT TM_BBH_TX_WAKE_UP_DATA_TABLE_1 =
{
	8,
	{
		{ dump_RDD_BBH_TX_WAKE_UP_DATA_ENTRY, 0x3078 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_CPU_RECYCLE_RING_DESCRIPTOR_TABLE_1 =
{
	16,
	{
		{ dump_RDD_CPU_RING_DESCRIPTOR, 0x3080 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_HW_CFG_1 =
{
	16,
	{
		{ dump_RDD_DHD_HW_CONFIGURATION, 0x30b0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_TX_COMPLETE_FLOW_RING_BUFFER_1 =
{
	16,
	{
		{ dump_RDD_DHD_TX_COMPLETE_DESCRIPTOR, 0x30c0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SPDSVC_ANALYZER_PARAMS_TABLE_1 =
{
	16,
	{
		{ dump_RDD_SPDSVC_ANALYZER_PARAMS, 0x30f0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_RX_COMPLETE_DISPATCHER_CREDIT_TABLE_1 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x3100 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT INGRESS_FILTER_L2_REASON_TABLE_1 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x3130 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_TX_COMPLETE_DISPATCHER_CREDIT_TABLE_1 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x3140 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT OVERALL_RATE_LIMITER_TABLE_1 =
{
	16,
	{
		{ dump_RDD_OVERALL_RATE_LIMITER_DESCRIPTOR, 0x3170 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT LOOPBACK_QUEUE_TABLE_1 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x3180 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT IPTV_CONFIGURATION_TABLE_1 =
{
	8,
	{
		{ dump_RDD_HW_IPTV_CONFIGURATION, 0x31a8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT US_TM_FLUSH_DISPATCHER_CREDIT_TABLE_1 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x31b0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT US_TM_FLUSH_CFG_CURRENT_TABLE_1 =
{
	4,
	{
		{ dump_RDD_FLUSH_CFG_ENTRY, 0x31bc },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT QUEUE_THRESHOLD_VECTOR_1 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x31c0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SPDTEST_GEN_PARAM_1 =
{
	4,
	{
		{ dump_RDD_SPDTEST_GEN_CFG, 0x31e4 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT GENERAL_QUEUE_DYNAMIC_MNG_TABLE_1 =
{
	8,
	{
		{ dump_RDD_QUEUE_DYNAMIC_MNG_ENTRY, 0x31e8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DISPATCHER_CREDIT_TABLE_1 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x31f0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT BUDGET_ALLOCATION_TIMER_VALUE_1 =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0x31fc },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT US_TM_BB_DESTINATION_TABLE_1 =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0x31fe },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT FLOW_BASED_ACTION_PTR_TABLE_1 =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0x3200 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT PFC_PENDING_QUEUE_MAP_1 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x3222 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT TASK_IDX_1 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x3224 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT RX_FLOW_CONTEXT_DDR_ADDR_1 =
{
	8,
	{
		{ dump_RDD_DEF_FLOW_CONTEXT_DDR_ADDR, 0x3228 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_FPM_THRESHOLDS_1 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x3230 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DIRECT_PROCESSING_WAN_VIQ_EXCLUSIVE_1 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x323c },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT FORCE_DSCP_1 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x323d },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CORE_ID_TABLE_1 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x323e },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT PFC_TX_ENABLE_1 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x323f },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT US_TM_TM_ACTION_PTR_TABLE_1 =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0x3240 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_RX_POST_RING_SIZE_1 =
{
	2,
	{
		{ dump_RDD_DHD_RING_SIZE, 0x3262 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_RX_COMPLETE_RING_SIZE_1 =
{
	2,
	{
		{ dump_RDD_DHD_RING_SIZE, 0x3264 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_TX_COMPLETE_RING_SIZE_1 =
{
	2,
	{
		{ dump_RDD_DHD_RING_SIZE, 0x3266 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT NULL_BUFFER_1 =
{
	8,
	{
		{ dump_RDD_BYTES_8, 0x3268 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_DOORBELL_TX_COMPLETE_VALUE_1 =
{
	4,
	{
		{ dump_RDD_DHD_DOORBELL, 0x3270 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT RX_MIRRORING_CONFIGURATION_1 =
{
	2,
	{
		{ dump_RDD_MIRRORING_DESCRIPTOR, 0x327c },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT TX_MIRRORING_CONFIGURATION_1 =
{
	2,
	{
		{ dump_RDD_MIRRORING_DESCRIPTOR, 0x327e },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT VLAN_ACTION_GPE_HANDLER_PTR_TABLE_1 =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0x3280 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SRAM_DUMMY_STORE_1 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x32a2 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT RATE_LIMIT_OVERHEAD_1 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x32a3 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT INGRESS_QOS_DONT_DROP_RATIO_1 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x32a4 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT US_TM_DSL_BBH_TX_FIFO_SIZE_1 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x32a5 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT US_TM_FIRST_QUEUE_MAPPING_1 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x32a6 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT MAC_TYPE_1 =
{
	1,
	{
		{ dump_RDD_MAC_TYPE_ENTRY, 0x32a7 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_CPU_RECYCLE_INTERRUPT_ID_DDR_ADDR_1 =
{
	8,
	{
		{ dump_RDD_DDR_ADDRESS, 0x32a8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_DOORBELL_RX_COMPLETE_VALUE_1 =
{
	4,
	{
		{ dump_RDD_DHD_DOORBELL, 0x32b0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT MULTI_FLOW_FLAG_TABLE_1 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x32bc },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_REDIRECT_MODE_1 =
{
	1,
	{
		{ dump_RDD_CPU_REDIRECT_MODE_ENTRY, 0x32bd },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DEBUG_PRINT_CORE_LOCK_1 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x32be },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT IPTV_CLASSIFICATION_CFG_TABLE_1 =
{
	1,
	{
		{ dump_RDD_IPTV_CLASSIFICATION_CFG_ENTRY, 0x32bf },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT UPDATE_FIFO_TABLE_1 =
{
	4,
	{
		{ dump_RDD_UPDATE_FIFO_ENTRY, 0x32c0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DEBUG_SCRATCHPAD_1 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x32e0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_DOORBELL_RX_POST_VALUE_1 =
{
	4,
	{
		{ dump_RDD_DHD_DOORBELL, 0x3310 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT INGRESS_FILTER_1588_CFG_1 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x331c },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT RX_MIRRORING_DIRECT_ENABLE_1 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x331d },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT US_TM_SCHEDULING_AGGREGATION_CONTEXT_VECTOR_1 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x3320 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT MIRRORING_SCRATCH_1 =
{
	8,
	{
		{ dump_RDD_BYTES_8, 0x3338 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_FPM_REPLY_1 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x3340 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SYSTEM_CONFIGURATION_1 =
{
	5,
	{
		{ dump_RDD_SYSTEM_CONFIGURATION_ENTRY, 0x3358 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT TCAM_IC_CFG_TABLE_1 =
{
	16,
	{
		{ dump_RDD_TCAM_IC_CFG, 0x3360 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT US_TM_DSL_BBH_TX_EGRESS_COUNTER_TABLE_1 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x3380 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_CPU_INT_ID_1 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x3390 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT RATE_LIMITER_VALID_TABLE_US_1 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x33a0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SPDSVC_BBH_TX_PARAMS_TABLE_1 =
{
	12,
	{
		{ dump_RDD_SPDSVC_BBH_TX_PARAMS, 0x33b0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT US_TM_DSL_BBH_TX_ABS_COUNTER_TABLE_1 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x33c0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT TX_MIRRORING_DISPATCHER_CREDIT_TABLE_1 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x33d0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SPDSVC_WLAN_GEN_PARAMS_TABLE_1 =
{
	10,
	{
		{ dump_RDD_SPDSVC_WLAN_GEN_PARAMS, 0x33e0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT FPM_GLOBAL_CFG_1 =
{
	16,
	{
		{ dump_RDD_FPM_GLOBAL_CFG, 0x33f0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT BITS_CALC_MASKS_TABLE_1 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x3400 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_FEED_RING_DESCRIPTOR_TABLE_1 =
{
	16,
	{
		{ dump_RDD_CPU_RING_DESCRIPTOR, 0x3410 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DEBUG_PRINT_TABLE_1 =
{
	16,
	{
		{ dump_RDD_DEBUG_PRINT_INFO, 0x3420 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT TUNNELS_PARSING_CFG_1 =
{
	8,
	{
		{ dump_RDD_TUNNELS_PARSING_CFG, 0x3430 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT IPTV_CFG_TABLE_1 =
{
	8,
	{
		{ dump_RDD_IPTV_CFG, 0x3438 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT US_TM_BBH_TX_EGRESS_COUNTER_TABLE_1 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x3440 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT IPTV_DDR_CTX_TABLE_ADDRESS_1 =
{
	8,
	{
		{ dump_RDD_IPTV_DDR_CTX_TABLE_ADDRESS, 0x3448 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT NAT_CACHE_CFG_1 =
{
	7,
	{
		{ dump_RDD_NAT_CACHE_CFG, 0x3450 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT US_TM_FLUSH_CFG_ENABLE_TABLE_1 =
{
	2,
	{
		{ dump_RDD_FLUSH_CFG_ENABLE_ENTRY, 0x3458 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT MULTICAST_KEY_MASK_1 =
{
	4,
	{
		{ dump_RDD_MULTICAST_KEY_MASK_ENTRY, 0x3460 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT NAT_CACHE_KEY0_MASK_1 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x3468 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT NATC_L2_VLAN_KEY_MASK_1 =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0x3470 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT INGRESS_FILTER_CFG_1 =
{
	2,
	{
		{ dump_RDD_INGRESS_FILTER_CFG, 0x3478 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT US_TM_BBH_TX_ABS_COUNTER_TABLE_1 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x3480 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT NATC_L2_TOS_MASK_1 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x3488 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT FC_FLOW_IP_ADDRESSES_TABLE_1 =
{
	48,
	{
		{ dump_RDD_FC_FLOW_IP_ADDRESSES_ENTRY, 0x3d00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT RUNNER_PROFILING_TRACE_BUFFER_1 =
{
	4,
	{
		{ dump_RDD_TRACE_EVENT, 0x3e00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_TX_SCRATCHPAD_2 =
{
	8,
	{
		{ dump_RDD_BYTES_8, 0x0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT LAYER2_GRE_TUNNEL_TABLE_2 =
{
	32,
	{
		{ dump_RDD_LAYER2_GRE_TUNNEL_ENTRY, 0x800 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_TX_DBG_CNTRS_TABLE_2 =
{
	64,
	{
		{ dump_RDD_CPU_TX_DBG_CNTRS, 0x980 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT VPORT_CFG_TABLE_2 =
{
	8,
	{
		{ dump_RDD_VPORT_CFG_ENTRY, 0xa00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT VPORT_TX_FLOW_TABLE_2 =
{
	1,
	{
		{ dump_RDD_TX_FLOW_ENTRY, 0xb40 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT GPE_COMMAND_PRIMITIVE_TABLE_2 =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0xb80 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT LOOPBACK_QUEUE_TABLE_2 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0xbc0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT IPTV_CONFIGURATION_TABLE_2 =
{
	8,
	{
		{ dump_RDD_HW_IPTV_CONFIGURATION, 0xbe8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT BUFFER_ALLOC_REPLY_2 =
{
	8,
	{
		{ dump_RDD_BYTES_8, 0xbf0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_RECYCLE_SRAM_PD_FIFO_2 =
{
	16,
	{
		{ dump_RDD_PROCESSING_TX_DESCRIPTOR, 0xc00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DSL_TX_FLOW_TABLE_2 =
{
	1,
	{
		{ dump_RDD_TX_FLOW_ENTRY, 0xd00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT QM_QUEUE_TO_TX_FLOW_TABLE_PTR_TABLE_2 =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0xe00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_TX_TASK_STACK_2 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0xf00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT IMG2_PACKET_BUFFER_2 =
{
	512,
	{
		{ dump_RDD_PACKET_BUFFER, 0x1000 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT RX_FLOW_TABLE_2 =
{
	2,
	{
		{ dump_RDD_RX_FLOW_ENTRY, 0x2000 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT MUTEX_TABLE_2 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x20b4 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT GENERAL_QUEUE_DYNAMIC_MNG_TABLE_2 =
{
	8,
	{
		{ dump_RDD_QUEUE_DYNAMIC_MNG_ENTRY, 0x20b8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT QUEUE_THRESHOLD_VECTOR_2 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x20c0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SPDTEST_GEN_PARAM_2 =
{
	4,
	{
		{ dump_RDD_SPDTEST_GEN_CFG, 0x20e4 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT RX_FLOW_CONTEXT_DDR_ADDR_2 =
{
	8,
	{
		{ dump_RDD_DEF_FLOW_CONTEXT_DDR_ADDR, 0x20e8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_RECYCLE_INTERRUPT_COALESCING_TABLE_2 =
{
	16,
	{
		{ dump_RDD_CPU_INTERRUPT_COALESCING_ENTRY, 0x20f0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT INGRESS_FILTER_PROFILE_TABLE_2 =
{
	8,
	{
		{ dump_RDD_INGRESS_FILTER_CTRL, 0x2100 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT NATC_COUNTERS_GENERIC_CONTEXT_2 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2180 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT VPORT_CFG_EX_TABLE_2 =
{
	4,
	{
		{ dump_RDD_VPORT_CFG_EX_ENTRY, 0x2200 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT TASK_IDX_2 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x227c },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT REGISTERS_BUFFER_2 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2280 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT NATC_TBL_CFG_2 =
{
	24,
	{
		{ dump_RDD_NATC_TBL_CONFIGURATION, 0x2300 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT FPM_POOL_NUMBER_MAPPING_TABLE_2 =
{
	1,
	{
		{ dump_RDD_FPM_POOL_NUMBER, 0x2360 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT FLOW_BASED_ACTION_PTR_TABLE_2 =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0x2380 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT FORCE_DSCP_2 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x23a2 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CORE_ID_TABLE_2 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x23a3 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT RX_MIRRORING_CONFIGURATION_2 =
{
	2,
	{
		{ dump_RDD_MIRRORING_DESCRIPTOR, 0x23a4 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SRAM_DUMMY_STORE_2 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x23a6 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT INGRESS_QOS_DONT_DROP_RATIO_2 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x23a7 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT NULL_BUFFER_2 =
{
	8,
	{
		{ dump_RDD_BYTES_8, 0x23a8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SPDSVC_ANALYZER_PARAMS_TABLE_2 =
{
	16,
	{
		{ dump_RDD_SPDSVC_ANALYZER_PARAMS, 0x23b0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT VLAN_ACTION_GPE_HANDLER_PTR_TABLE_2 =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0x23c0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_TX_RECYCLE_RING_ID_2 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x23e2 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT MULTI_FLOW_FLAG_TABLE_2 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x23e3 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_REDIRECT_MODE_2 =
{
	1,
	{
		{ dump_RDD_CPU_REDIRECT_MODE_ENTRY, 0x23e4 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DEBUG_PRINT_CORE_LOCK_2 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x23e5 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT IPTV_CLASSIFICATION_CFG_TABLE_2 =
{
	1,
	{
		{ dump_RDD_IPTV_CLASSIFICATION_CFG_ENTRY, 0x23e6 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT INGRESS_FILTER_1588_CFG_2 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x23e7 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_RECYCLE_INTERRUPT_ID_DDR_ADDR_2 =
{
	8,
	{
		{ dump_RDD_DDR_ADDRESS, 0x23e8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT INGRESS_FILTER_L2_REASON_TABLE_2 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x23f0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT VPORT_TO_LOOKUP_PORT_MAPPING_TABLE_2 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2400 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DEBUG_SCRATCHPAD_2 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2420 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_TX_RING_DESCRIPTOR_TABLE_2 =
{
	16,
	{
		{ dump_RDD_CPU_RING_DESCRIPTOR, 0x2450 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_TX_EGRESS_DISPATCHER_CREDIT_TABLE_2 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2470 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT RX_MIRRORING_DIRECT_ENABLE_2 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x247c },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT RING_CPU_TX_DESCRIPTOR_DATA_TABLE_2 =
{
	16,
	{
		{ dump_RDD_RING_CPU_TX_DESCRIPTOR, 0x2480 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT TCAM_IC_CFG_TABLE_2 =
{
	16,
	{
		{ dump_RDD_TCAM_IC_CFG, 0x24a0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT BITS_CALC_MASKS_TABLE_2 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x24c0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_TX_INGRESS_DISPATCHER_CREDIT_TABLE_2 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x24d0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_FPM_THRESHOLDS_2 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x24e0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT FPM_GLOBAL_CFG_2 =
{
	16,
	{
		{ dump_RDD_FPM_GLOBAL_CFG, 0x24f0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_TX_SYNC_FIFO_TABLE_2 =
{
	8,
	{
		{ dump_RDD_CPU_TX_SYNC_FIFO_ENTRY, 0x2500 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_RECYCLE_RING_DESCRIPTOR_TABLE_2 =
{
	16,
	{
		{ dump_RDD_CPU_RING_DESCRIPTOR, 0x2510 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_RECYCLE_RING_INDEX_DDR_ADDR_TABLE_2 =
{
	8,
	{
		{ dump_RDD_DDR_ADDRESS, 0x2520 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_FEED_RING_DESCRIPTOR_TABLE_2 =
{
	16,
	{
		{ dump_RDD_CPU_RING_DESCRIPTOR, 0x2530 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_RX_INTERRUPT_ID_DDR_ADDR_2 =
{
	8,
	{
		{ dump_RDD_DDR_ADDRESS, 0x2540 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SYSTEM_CONFIGURATION_2 =
{
	5,
	{
		{ dump_RDD_SYSTEM_CONFIGURATION_ENTRY, 0x2548 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_RECYCLE_INTERRUPT_SCRATCH_2 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2550 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_RECYCLE_RING_INTERRUPT_COUNTER_TABLE_2 =
{
	8,
	{
		{ dump_RDD_CPU_RING_INTERRUPT_COUNTER_ENTRY, 0x2558 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DEBUG_PRINT_TABLE_2 =
{
	16,
	{
		{ dump_RDD_DEBUG_PRINT_INFO, 0x2560 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_TX_RING_INDICES_VALUES_TABLE_2 =
{
	4,
	{
		{ dump_RDD_CPU_TX_RING_INDICES, 0x2570 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT TUNNELS_PARSING_CFG_2 =
{
	8,
	{
		{ dump_RDD_TUNNELS_PARSING_CFG, 0x2578 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT MULTICAST_KEY_MASK_2 =
{
	4,
	{
		{ dump_RDD_MULTICAST_KEY_MASK_ENTRY, 0x2580 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT IPTV_CFG_TABLE_2 =
{
	8,
	{
		{ dump_RDD_IPTV_CFG, 0x2588 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT IPTV_DDR_CTX_TABLE_ADDRESS_2 =
{
	8,
	{
		{ dump_RDD_IPTV_DDR_CTX_TABLE_ADDRESS, 0x2590 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT NAT_CACHE_CFG_2 =
{
	7,
	{
		{ dump_RDD_NAT_CACHE_CFG, 0x2598 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_RECYCLE_SHADOW_RD_IDX_2 =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0x25a0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_RECYCLE_SHADOW_WR_IDX_2 =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0x25a8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_RECYCLE_NEXT_PTR_TABLE_2 =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0x25b0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT NAT_CACHE_KEY0_MASK_2 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x25b8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT NATC_L2_VLAN_KEY_MASK_2 =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0x25c0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT INGRESS_FILTER_CFG_2 =
{
	2,
	{
		{ dump_RDD_INGRESS_FILTER_CFG, 0x25c8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT NATC_L2_TOS_MASK_2 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x25d0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT FC_FLOW_IP_ADDRESSES_TABLE_2 =
{
	48,
	{
		{ dump_RDD_FC_FLOW_IP_ADDRESSES_ENTRY, 0x3d00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT RUNNER_PROFILING_TRACE_BUFFER_2 =
{
	4,
	{
		{ dump_RDD_TRACE_EVENT, 0x3e00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT TCPSPDTEST_STREAM_TABLE_3 =
{
	240,
	{
		{ dump_RDD_TCPSPDTEST_STREAM, 0x0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT VPORT_TX_FLOW_TABLE_3 =
{
	1,
	{
		{ dump_RDD_TX_FLOW_ENTRY, 0x3c0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_RX_SCRATCHPAD_3 =
{
	8,
	{
		{ dump_RDD_BYTES_8, 0x400 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT PKTGEN_TX_STREAM_TABLE_3 =
{
	136,
	{
		{ dump_RDD_PKTGEN_TX_STREAM_ENTRY, 0x800 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT FPM_POOL_NUMBER_MAPPING_TABLE_3 =
{
	1,
	{
		{ dump_RDD_FPM_POOL_NUMBER, 0xa20 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT GPE_COMMAND_PRIMITIVE_TABLE_3 =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0xa40 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_FEED_RING_CACHE_TABLE_3 =
{
	8,
	{
		{ dump_RDD_BYTES_8, 0xa80 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DSL_TX_FLOW_TABLE_3 =
{
	1,
	{
		{ dump_RDD_TX_FLOW_ENTRY, 0xb00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT LAYER2_GRE_TUNNEL_TABLE_3 =
{
	32,
	{
		{ dump_RDD_LAYER2_GRE_TUNNEL_ENTRY, 0xc00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DS_CPU_RX_METER_TABLE_3 =
{
	8,
	{
		{ dump_RDD_CPU_RX_METER_ENTRY, 0xd80 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT VPORT_CFG_TABLE_3 =
{
	8,
	{
		{ dump_RDD_VPORT_CFG_ENTRY, 0xe00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DS_CPU_REASON_TO_METER_TABLE_3 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0xf40 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT US_CPU_RX_METER_TABLE_3 =
{
	8,
	{
		{ dump_RDD_CPU_RX_METER_ENTRY, 0xf80 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT IMG3_PACKET_BUFFER_3 =
{
	512,
	{
		{ dump_RDD_PACKET_BUFFER, 0x1000 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_RECYCLE_SRAM_PD_FIFO_3 =
{
	16,
	{
		{ dump_RDD_PROCESSING_TX_DESCRIPTOR, 0x2000 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_RX_TASK_STACK_3 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2100 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_RX_COPY_TASK_STACK_3 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2200 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT RX_FLOW_TABLE_3 =
{
	2,
	{
		{ dump_RDD_RX_FLOW_ENTRY, 0x2300 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT MUTEX_TABLE_3 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x23b4 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT IPTV_CONFIGURATION_TABLE_3 =
{
	8,
	{
		{ dump_RDD_HW_IPTV_CONFIGURATION, 0x23b8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT US_CPU_REASON_TO_METER_TABLE_3 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x23c0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CSO_CONTEXT_TABLE_3 =
{
	152,
	{
		{ dump_RDD_CSO_CONTEXT_ENTRY, 0x2400 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT GENERAL_QUEUE_DYNAMIC_MNG_TABLE_3 =
{
	8,
	{
		{ dump_RDD_QUEUE_DYNAMIC_MNG_ENTRY, 0x2498 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT VPORT_TO_LOOKUP_PORT_MAPPING_TABLE_3 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x24a0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT IPV6_HOST_ADDRESS_CRC_TABLE_3 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x24c0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_RING_INTERRUPT_COUNTER_TABLE_3 =
{
	8,
	{
		{ dump_RDD_CPU_RING_INTERRUPT_COUNTER_ENTRY, 0x2500 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_RECYCLE_INTERRUPT_COALESCING_TABLE_3 =
{
	16,
	{
		{ dump_RDD_CPU_INTERRUPT_COALESCING_ENTRY, 0x2590 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_FEED_RING_RSV_TABLE_3 =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0x25a0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SPDSVC_GEN_PARAMS_TABLE_3 =
{
	60,
	{
		{ dump_RDD_SPDSVC_GEN_PARAMS, 0x25c0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT PKTGEN_NO_SBPM_HDRS_CNTR_3 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x25fc },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_RX_COPY_PD_FIFO_TABLE_3 =
{
	16,
	{
		{ dump_RDD_PROCESSING_TX_DESCRIPTOR, 0x2600 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_REASON_AND_VPORT_TO_METER_TABLE_3 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2680 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT RX_FLOW_CONTEXT_DDR_ADDR_3 =
{
	8,
	{
		{ dump_RDD_DEF_FLOW_CONTEXT_DDR_ADDR, 0x26f8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT INGRESS_FILTER_PROFILE_TABLE_3 =
{
	8,
	{
		{ dump_RDD_INGRESS_FILTER_CTRL, 0x2700 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT COMMON_REPROCESSING_PD_FIFO_TABLE_3 =
{
	16,
	{
		{ dump_RDD_PROCESSING_TX_DESCRIPTOR, 0x2780 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT LOOPBACK_QUEUE_TABLE_3 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x27c0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT NULL_BUFFER_3 =
{
	8,
	{
		{ dump_RDD_BYTES_8, 0x27e8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_RX_COPY_INT_SCRATCHPAD_3 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x27f0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT VPORT_CFG_EX_TABLE_3 =
{
	4,
	{
		{ dump_RDD_VPORT_CFG_EX_ENTRY, 0x2800 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SPDTEST_GEN_PARAM_3 =
{
	4,
	{
		{ dump_RDD_SPDTEST_GEN_CFG, 0x287c },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT NATC_COUNTERS_GENERIC_CONTEXT_3 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2880 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT NATC_TBL_CFG_3 =
{
	24,
	{
		{ dump_RDD_NATC_TBL_CONFIGURATION, 0x2900 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT REGISTERS_BUFFER_3 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2960 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT IPV4_HOST_ADDRESS_TABLE_3 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x29e0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT PKTGEN_SBPM_HDR_BNS_3 =
{
	2,
	{
		{ dump_RDD_PKTGEN_SBPM_HDR_BN, 0x2a00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT TX_ABS_RECYCLE_COUNTERS_3 =
{
	8,
	{
		{ dump_RDD_TX_ABS_RECYCLE_COUNTERS_ENTRY, 0x2a38 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT QUEUE_THRESHOLD_VECTOR_3 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2a40 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_RX_INTERRUPT_SCRATCH_3 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2a64 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_RECYCLE_INTERRUPT_ID_DDR_ADDR_3 =
{
	8,
	{
		{ dump_RDD_DDR_ADDRESS, 0x2a68 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_RX_PSRAM_GET_NEXT_SCRATCHPAD_3 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2a70 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT UDPSPDTEST_STREAM_RX_STAT_TABLE_3 =
{
	48,
	{
		{ dump_RDD_UDPSPDTEST_STREAM_RX_STAT, 0x2a80 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SPDSVC_ANALYZER_PARAMS_TABLE_3 =
{
	16,
	{
		{ dump_RDD_SPDSVC_ANALYZER_PARAMS, 0x2ab0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT FLOW_BASED_ACTION_PTR_TABLE_3 =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0x2ac0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT PKTGEN_CURR_SBPM_HDR_PTR_3 =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0x2ae2 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT PKTGEN_NUM_OF_AVAIL_SBPM_HDRS_3 =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0x2ae4 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT PKTGEN_SBPM_END_PTR_3 =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0x2ae6 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_RX_INTERRUPT_ID_DDR_ADDR_3 =
{
	8,
	{
		{ dump_RDD_DDR_ADDRESS, 0x2ae8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT INGRESS_FILTER_L2_REASON_TABLE_3 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2af0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_VPORT_TO_METER_TABLE_3 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2b00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SYSTEM_CONFIGURATION_3 =
{
	5,
	{
		{ dump_RDD_SYSTEM_CONFIGURATION_ENTRY, 0x2b28 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT FORCE_DSCP_3 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2b2d },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CORE_ID_TABLE_3 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2b2e },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SRAM_DUMMY_STORE_3 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2b2f },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_RX_COPY_DISPATCHER_CREDIT_TABLE_3 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2b30 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT PKTGEN_BAD_GET_NEXT_3 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2b3c },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT VLAN_ACTION_GPE_HANDLER_PTR_TABLE_3 =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0x2b40 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SPDSVC_TCPSPDTEST_COMMON_TABLE_3 =
{
	2,
	{
		{ dump_RDD_SPDSVC_TCPSPDTEST_COMMON_ENTRY, 0x2b62 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT PKTGEN_MAX_UT_PKTS_3 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2b64 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_RECYCLE_RING_INTERRUPT_COUNTER_TABLE_3 =
{
	8,
	{
		{ dump_RDD_CPU_RING_INTERRUPT_COUNTER_ENTRY, 0x2b68 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SPDSVC_GEN_DISPATCHER_CREDIT_TABLE_3 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2b70 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT PKTGEN_UT_TRIGGER_3 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2b7c },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT UDPSPDTEST_STREAM_TX_STAT_TABLE_3 =
{
	36,
	{
		{ dump_RDD_UDPSPDTEST_STREAM_TX_STAT, 0x2b80 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT TASK_IDX_3 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2ba4 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_FEED_RING_INDEX_DDR_ADDR_TABLE_3 =
{
	8,
	{
		{ dump_RDD_DDR_ADDRESS, 0x2ba8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT COMMON_REPROCESSING_DISPATCHER_CREDIT_TABLE_3 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2bb0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_FEED_RING_INTERRUPT_THRESHOLD_3 =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0x2bbc },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_FEED_RING_INTERRUPT_COUNTER_3 =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0x2bbe },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT UPDATE_FIFO_TABLE_3 =
{
	4,
	{
		{ dump_RDD_UPDATE_FIFO_ENTRY, 0x2bc0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT PKTGEN_FPM_UG_MGMT_3 =
{
	20,
	{
		{ dump_RDD_PKTGEN_FPM_UG_MGMT_ENTRY, 0x2be0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT RX_MIRRORING_CONFIGURATION_3 =
{
	2,
	{
		{ dump_RDD_MIRRORING_DESCRIPTOR, 0x2bf4 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT INGRESS_QOS_DONT_DROP_RATIO_3 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2bf6 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_TX_RECYCLE_RING_ID_3 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2bf7 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT TUNNELS_PARSING_CFG_3 =
{
	8,
	{
		{ dump_RDD_TUNNELS_PARSING_CFG, 0x2bf8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_RX_COPY_UPDATE_FIFO_TABLE_3 =
{
	4,
	{
		{ dump_RDD_UPDATE_FIFO_ENTRY, 0x2c00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT TCPSPDTEST_ENGINE_GLOBAL_TABLE_3 =
{
	20,
	{
		{ dump_RDD_TCPSPDTEST_ENGINE_GLOBAL_INFO, 0x2c20 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT MULTI_FLOW_FLAG_TABLE_3 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2c34 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_FEED_RING_CACHE_OFFSET_3 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2c35 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_REDIRECT_MODE_3 =
{
	1,
	{
		{ dump_RDD_CPU_REDIRECT_MODE_ENTRY, 0x2c36 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CSO_DISABLE_3 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2c37 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT IPTV_CFG_TABLE_3 =
{
	8,
	{
		{ dump_RDD_IPTV_CFG, 0x2c38 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT COMMON_REPROCESSING_UPDATE_FIFO_TABLE_3 =
{
	4,
	{
		{ dump_RDD_UPDATE_FIFO_ENTRY, 0x2c40 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT TCAM_IC_CFG_TABLE_3 =
{
	16,
	{
		{ dump_RDD_TCAM_IC_CFG, 0x2c60 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT PD_FIFO_TABLE_3 =
{
	16,
	{
		{ dump_RDD_PROCESSING_TX_DESCRIPTOR, 0x2c80 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_REASON_TO_TC_3 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2ca0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT TC_TO_CPU_RXQ_3 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2ce0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT EXC_TC_TO_CPU_RXQ_3 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2d20 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT BCM_SPDSVC_STREAM_RX_TS_TABLE_3 =
{
	12,
	{
		{ dump_RDD_SPDSVC_RX_TS_STAT, 0x2d60 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DEBUG_PRINT_CORE_LOCK_3 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2d6c },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT IPTV_CLASSIFICATION_CFG_TABLE_3 =
{
	1,
	{
		{ dump_RDD_IPTV_CLASSIFICATION_CFG_ENTRY, 0x2d6d },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT INGRESS_FILTER_1588_CFG_3 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2d6e },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT RX_MIRRORING_DIRECT_ENABLE_3 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2d6f },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_FPM_THRESHOLDS_3 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2d70 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT PKTGEN_SESSION_DATA_3 =
{
	24,
	{
		{ dump_RDD_PKTGEN_TX_PARAMS, 0x2d80 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DEBUG_SCRATCHPAD_3 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2d98 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT IPTV_DDR_CTX_TABLE_ADDRESS_3 =
{
	8,
	{
		{ dump_RDD_IPTV_DDR_CTX_TABLE_ADDRESS, 0x2dc8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT FPM_GLOBAL_CFG_3 =
{
	16,
	{
		{ dump_RDD_FPM_GLOBAL_CFG, 0x2dd0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_RECYCLE_RING_DESCRIPTOR_TABLE_3 =
{
	16,
	{
		{ dump_RDD_CPU_RING_DESCRIPTOR, 0x2de0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_RECYCLE_RING_INDEX_DDR_ADDR_TABLE_3 =
{
	8,
	{
		{ dump_RDD_DDR_ADDRESS, 0x2df0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT VPORT_TO_CPU_OBJ_3 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2e00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT NAT_CACHE_CFG_3 =
{
	7,
	{
		{ dump_RDD_NAT_CACHE_CFG, 0x2e28 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_INTERRUPT_COALESCING_TABLE_3 =
{
	16,
	{
		{ dump_RDD_CPU_INTERRUPT_COALESCING_ENTRY, 0x2e30 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT BITS_CALC_MASKS_TABLE_3 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2e40 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_FEED_RING_DESCRIPTOR_TABLE_3 =
{
	16,
	{
		{ dump_RDD_CPU_RING_DESCRIPTOR, 0x2e50 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_RECYCLE_INTERRUPT_SCRATCH_3 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2e60 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_RX_LOCAL_SCRATCH_3 =
{
	8,
	{
		{ dump_RDD_BYTES_8, 0x2e68 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT PKTGEN_SBPM_EXTS_3 =
{
	4,
	{
		{ dump_RDD_PKTGEN_SBPM_EXT, 0x2e78 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_RECYCLE_SHADOW_RD_IDX_3 =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0x2e88 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT PKTGEN_BBMSG_REPLY_SCRATCH_3 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2e90 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_RECYCLE_SHADOW_WR_IDX_3 =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0x2e98 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DEBUG_PRINT_TABLE_3 =
{
	16,
	{
		{ dump_RDD_DEBUG_PRINT_INFO, 0x2ea0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_RECYCLE_NEXT_PTR_TABLE_3 =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0x2eb0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT NAT_CACHE_KEY0_MASK_3 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2eb8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT MULTICAST_KEY_MASK_3 =
{
	4,
	{
		{ dump_RDD_MULTICAST_KEY_MASK_ENTRY, 0x2ec0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT NATC_L2_VLAN_KEY_MASK_3 =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0x2ec8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT INGRESS_FILTER_CFG_3 =
{
	2,
	{
		{ dump_RDD_INGRESS_FILTER_CFG, 0x2ed0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT NATC_L2_TOS_MASK_3 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2ed8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT PKTGEN_TX_STREAM_SCRATCH_TABLE_3 =
{
	8,
	{
		{ dump_RDD_PKTGEN_TX_STREAM_SCRATCH_ENTRY, 0x2f00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_RING_DESCRIPTORS_TABLE_3 =
{
	16,
	{
		{ dump_RDD_CPU_RING_DESCRIPTOR, 0x3000 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT FC_FLOW_IP_ADDRESSES_TABLE_3 =
{
	48,
	{
		{ dump_RDD_FC_FLOW_IP_ADDRESSES_ENTRY, 0x3d00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT RUNNER_PROFILING_TRACE_BUFFER_3 =
{
	4,
	{
		{ dump_RDD_TRACE_EVENT, 0x3e00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_FLOW_RING_CACHE_CTX_TABLE_4 =
{
	16,
	{
		{ dump_RDD_DHD_FLOW_RING_CACHE_CTX_ENTRY, 0x0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DSL_TX_FLOW_TABLE_4 =
{
	1,
	{
		{ dump_RDD_TX_FLOW_ENTRY, 0x300 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_AUX_INFO_CACHE_TABLE_4 =
{
	16,
	{
		{ dump_RDD_DHD_AUX_INFO_CACHE_ENTRY, 0x400 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_TIMER_TASK_STACK_4 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x700 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT WLAN_MCAST_DHD_STATION_TABLE_4 =
{
	8,
	{
		{ dump_RDD_WLAN_MCAST_DHD_STATION_ENTRY, 0x800 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT WLAN_MCAST_COPY_SCRATCHPAD_4 =
{
	8,
	{
		{ dump_RDD_BYTES_8, 0xa00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_POST_COMMON_RADIO_DATA_4 =
{
	144,
	{
		{ dump_RDD_DHD_POST_COMMON_RADIO_ENTRY, 0xc00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_FPM_POOL_NUMBER_MAPPING_TABLE_4 =
{
	1,
	{
		{ dump_RDD_FPM_POOL_NUMBER, 0xdb0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT VPORT_TX_FLOW_TABLE_4 =
{
	1,
	{
		{ dump_RDD_TX_FLOW_ENTRY, 0xdc0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT LAYER2_GRE_TUNNEL_TABLE_4 =
{
	32,
	{
		{ dump_RDD_LAYER2_GRE_TUNNEL_ENTRY, 0xe00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_BACKUP_INDEX_CACHE_4 =
{
	32,
	{
		{ dump_RDD_DHD_BACKUP_IDX_CACHE_TABLE, 0xf80 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT FPM_POOL_NUMBER_MAPPING_TABLE_4 =
{
	1,
	{
		{ dump_RDD_FPM_POOL_NUMBER, 0xfe0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT IMG4_PACKET_BUFFER_4 =
{
	512,
	{
		{ dump_RDD_PACKET_BUFFER, 0x1000 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT WLAN_MCAST_SSID_MAC_ADDRESS_TABLE_4 =
{
	8,
	{
		{ dump_RDD_WLAN_MCAST_SSID_MAC_ADDRESS_ENTRY, 0x2000 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_L2_HEADER_4 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2180 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT IPTV_CONFIGURATION_TABLE_4 =
{
	8,
	{
		{ dump_RDD_HW_IPTV_CONFIGURATION, 0x21c8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_HW_CFG_4 =
{
	16,
	{
		{ dump_RDD_DHD_HW_CONFIGURATION, 0x21d0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT VPORT_TO_LOOKUP_PORT_MAPPING_TABLE_4 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x21e0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT VPORT_CFG_TABLE_4 =
{
	8,
	{
		{ dump_RDD_VPORT_CFG_ENTRY, 0x2200 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT GPE_COMMAND_PRIMITIVE_TABLE_4 =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0x2340 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT NATC_COUNTERS_GENERIC_CONTEXT_4 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2380 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_TX_POST_TASK_0_STACK_4 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2400 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_TX_POST_TASK_1_STACK_4 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2500 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_TX_POST_TASK_2_STACK_4 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2600 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_TX_POST_UPDATE_FIFO_TASK_STACK_4 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2700 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_TX_POST_PD_FIFO_TABLE_4 =
{
	16,
	{
		{ dump_RDD_PROCESSING_TX_DESCRIPTOR, 0x2800 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT WLAN_MCAST_SSID_STATS_TABLE_4 =
{
	8,
	{
		{ dump_RDD_WLAN_MCAST_SSID_STATS_ENTRY, 0x28c0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT REGISTERS_BUFFER_4 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2a40 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT WLAN_MCAST_DHD_STATION_CTX_TABLE_4 =
{
	1,
	{
		{ dump_RDD_WLAN_MCAST_DHD_STATION_CTX_ENTRY, 0x2ac0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT RX_FLOW_TABLE_4 =
{
	2,
	{
		{ dump_RDD_RX_FLOW_ENTRY, 0x2b00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT MUTEX_TABLE_4 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2bb4 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT GENERAL_QUEUE_DYNAMIC_MNG_TABLE_4 =
{
	8,
	{
		{ dump_RDD_QUEUE_DYNAMIC_MNG_ENTRY, 0x2bb8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT WLAN_MCAST_DFT_LIST_ENTRY_SCRATCH_4 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2bc0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_TX_POST_FLOW_RING_BUFFER_4 =
{
	48,
	{
		{ dump_RDD_DHD_TX_POST_DESCRIPTOR, 0x2c00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DEBUG_SCRATCHPAD_4 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2c90 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_CODEL_BIAS_SLOPE_TABLE_4 =
{
	4,
	{
		{ dump_RDD_DHD_CODEL_BIAS_SLOPE, 0x2cc0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SPDTEST_GEN_PARAM_4 =
{
	4,
	{
		{ dump_RDD_SPDTEST_GEN_CFG, 0x2cec },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SPDSVC_ANALYZER_PARAMS_TABLE_4 =
{
	16,
	{
		{ dump_RDD_SPDSVC_ANALYZER_PARAMS, 0x2cf0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT INGRESS_FILTER_PROFILE_TABLE_4 =
{
	8,
	{
		{ dump_RDD_INGRESS_FILTER_CTRL, 0x2d00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT WLAN_MCAST_DFT_LIST_SIZE_4 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2d80 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT LOOPBACK_QUEUE_TABLE_4 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2dc0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT RX_FLOW_CONTEXT_DDR_ADDR_4 =
{
	8,
	{
		{ dump_RDD_DEF_FLOW_CONTEXT_DDR_ADDR, 0x2de8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT INGRESS_FILTER_L2_REASON_TABLE_4 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2df0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT VPORT_CFG_EX_TABLE_4 =
{
	4,
	{
		{ dump_RDD_VPORT_CFG_EX_ENTRY, 0x2e00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT TASK_IDX_4 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2e7c },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_CPU_RECYCLE_RING_DESCRIPTOR_TABLE_4 =
{
	16,
	{
		{ dump_RDD_CPU_RING_DESCRIPTOR, 0x2e80 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_FPM_THRESHOLDS_4 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2eb0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SPDSVC_WLAN_TXPOST_PARAMS_TABLE_4 =
{
	2,
	{
		{ dump_RDD_SPDSVC_WLAN_TXPOST_PARAMS, 0x2ebc },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT FORCE_DSCP_4 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2ebe },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CORE_ID_TABLE_4 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2ebf },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT WLAN_MCAST_SCRATCHPAD_4 =
{
	8,
	{
		{ dump_RDD_BYTES_8, 0x2ec0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT NULL_BUFFER_4 =
{
	8,
	{
		{ dump_RDD_BYTES_8, 0x2ee8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_CPU_INT_ID_4 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2ef0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT RX_MIRRORING_CONFIGURATION_4 =
{
	2,
	{
		{ dump_RDD_MIRRORING_DESCRIPTOR, 0x2efc },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SRAM_DUMMY_STORE_4 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2efe },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT INGRESS_QOS_DONT_DROP_RATIO_4 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2eff },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT NATC_TBL_CFG_4 =
{
	24,
	{
		{ dump_RDD_NATC_TBL_CONFIGURATION, 0x2f00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT TCAM_IC_CFG_TABLE_4 =
{
	16,
	{
		{ dump_RDD_TCAM_IC_CFG, 0x2f60 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT QUEUE_THRESHOLD_VECTOR_4 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2f80 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT MULTI_FLOW_FLAG_TABLE_4 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2fa4 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_REDIRECT_MODE_4 =
{
	1,
	{
		{ dump_RDD_CPU_REDIRECT_MODE_ENTRY, 0x2fa5 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DEBUG_PRINT_CORE_LOCK_4 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2fa6 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT IPTV_CLASSIFICATION_CFG_TABLE_4 =
{
	1,
	{
		{ dump_RDD_IPTV_CLASSIFICATION_CFG_ENTRY, 0x2fa7 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT MIRRORING_SCRATCH_4 =
{
	8,
	{
		{ dump_RDD_BYTES_8, 0x2fa8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_MCAST_DISPATCHER_CREDIT_TABLE_4 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2fb0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT INGRESS_FILTER_1588_CFG_4 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2fbc },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT RX_MIRRORING_DIRECT_ENABLE_4 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x2fbd },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT FLOW_BASED_ACTION_PTR_TABLE_4 =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0x2fc0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_CPU_RECYCLE_INTERRUPT_ID_DDR_ADDR_4 =
{
	8,
	{
		{ dump_RDD_DDR_ADDRESS, 0x2fe8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_MIRRORING_DISPATCHER_CREDIT_TABLE_4 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x2ff0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_FLOW_RING_CACHE_LKP_TABLE_4 =
{
	2,
	{
		{ dump_RDD_DHD_FLOW_RING_CACHE_LKP_ENTRY, 0x3000 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT FPM_GLOBAL_CFG_4 =
{
	16,
	{
		{ dump_RDD_FPM_GLOBAL_CFG, 0x3060 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT CPU_FEED_RING_DESCRIPTOR_TABLE_4 =
{
	16,
	{
		{ dump_RDD_CPU_RING_DESCRIPTOR, 0x3070 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT VLAN_ACTION_GPE_HANDLER_PTR_TABLE_4 =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0x3080 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_DOORBELL_TX_POST_VALUE_4 =
{
	4,
	{
		{ dump_RDD_DHD_DOORBELL, 0x30a8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DEBUG_PRINT_TABLE_4 =
{
	16,
	{
		{ dump_RDD_DEBUG_PRINT_INFO, 0x30f0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_TX_POST_UPDATE_FIFO_TABLE_4 =
{
	4,
	{
		{ dump_RDD_UPDATE_FIFO_ENTRY, 0x3100 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_BACKUP_BASE_ADDR_4 =
{
	8,
	{
		{ dump_RDD_BYTES_8, 0x3120 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT WLAN_MCAST_DFT_ADDR_4 =
{
	8,
	{
		{ dump_RDD_PHYS_ADDR_64_PTR, 0x3128 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT SYSTEM_CONFIGURATION_4 =
{
	5,
	{
		{ dump_RDD_SYSTEM_CONFIGURATION_ENTRY, 0x3130 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT TUNNELS_PARSING_CFG_4 =
{
	8,
	{
		{ dump_RDD_TUNNELS_PARSING_CFG, 0x3138 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_MCAST_UPDATE_FIFO_TABLE_4 =
{
	4,
	{
		{ dump_RDD_UPDATE_FIFO_ENTRY, 0x3140 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT MULTICAST_KEY_MASK_4 =
{
	4,
	{
		{ dump_RDD_MULTICAST_KEY_MASK_ENTRY, 0x3160 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT IPTV_CFG_TABLE_4 =
{
	8,
	{
		{ dump_RDD_IPTV_CFG, 0x3168 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT IPTV_DDR_CTX_TABLE_ADDRESS_4 =
{
	8,
	{
		{ dump_RDD_IPTV_DDR_CTX_TABLE_ADDRESS, 0x3170 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT NAT_CACHE_CFG_4 =
{
	7,
	{
		{ dump_RDD_NAT_CACHE_CFG, 0x3178 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT DHD_MCAST_PD_FIFO_TABLE_4 =
{
	16,
	{
		{ dump_RDD_PROCESSING_TX_DESCRIPTOR, 0x3180 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT NAT_CACHE_KEY0_MASK_4 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x31a0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT NATC_L2_VLAN_KEY_MASK_4 =
{
	2,
	{
		{ dump_RDD_BYTES_2, 0x31a8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT INGRESS_FILTER_CFG_4 =
{
	2,
	{
		{ dump_RDD_INGRESS_FILTER_CFG, 0x31b0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT NATC_L2_TOS_MASK_4 =
{
	1,
	{
		{ dump_RDD_BYTE_1, 0x31b8 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT BITS_CALC_MASKS_TABLE_4 =
{
	4,
	{
		{ dump_RDD_BYTES_4, 0x31c0 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT FC_FLOW_IP_ADDRESSES_TABLE_4 =
{
	48,
	{
		{ dump_RDD_FC_FLOW_IP_ADDRESSES_ENTRY, 0x3d00 },
		{ 0, 0 },
	}
};
#endif
#if defined BCM63146
static DUMP_RUNNERREG_STRUCT RUNNER_PROFILING_TRACE_BUFFER_4 =
{
	4,
	{
		{ dump_RDD_TRACE_EVENT, 0x3e00 },
		{ 0, 0 },
	}
};
#endif

TABLE_STRUCT RUNNER_TABLES[NUMBER_OF_TABLES] =
{
#if defined BCM63146
	{ "DS_TM_PD_FIFO_TABLE", 1, CORE_0_INDEX, &DS_TM_PD_FIFO_TABLE, 128, 1, 1 },
#endif
#if defined BCM63146
	{ "SERVICE_QUEUES_PD_FIFO_TABLE", 1, CORE_0_INDEX, &SERVICE_QUEUES_PD_FIFO_TABLE, 64, 1, 1 },
#endif
#if defined BCM63146
	{ "DS_TM_SCHEDULING_QUEUE_TABLE", 1, CORE_0_INDEX, &DS_TM_SCHEDULING_QUEUE_TABLE, 64, 1, 1 },
#endif
#if defined BCM63146
	{ "BASIC_SCHEDULER_TABLE_DS", 1, CORE_0_INDEX, &BASIC_SCHEDULER_TABLE_DS, 32, 1, 1 },
#endif
#if defined BCM63146
	{ "IMG0_PACKET_BUFFER", 1, CORE_0_INDEX, &IMG0_PACKET_BUFFER, 4, 1, 1 },
#endif
#if defined BCM63146
	{ "LAYER2_GRE_TUNNEL_TABLE", 1, CORE_0_INDEX, &LAYER2_GRE_TUNNEL_TABLE, 12, 1, 1 },
#endif
#if defined BCM63146
	{ "COMPLEX_SCHEDULER_TABLE", 1, CORE_0_INDEX, &COMPLEX_SCHEDULER_TABLE, 2, 1, 1 },
#endif
#if defined BCM63146
	{ "VPORT_CFG_TABLE", 1, CORE_0_INDEX, &VPORT_CFG_TABLE, 40, 1, 1 },
#endif
#if defined BCM63146
	{ "DS_TM_TM_FLOW_CNTR_TABLE", 1, CORE_0_INDEX, &DS_TM_TM_FLOW_CNTR_TABLE, 64, 1, 1 },
#endif
#if defined BCM63146
	{ "VPORT_TX_FLOW_TABLE", 1, CORE_0_INDEX, &VPORT_TX_FLOW_TABLE, 64, 1, 1 },
#endif
#if defined BCM63146
	{ "GPE_COMMAND_PRIMITIVE_TABLE", 1, CORE_0_INDEX, &GPE_COMMAND_PRIMITIVE_TABLE, 32, 1, 1 },
#endif
#if defined BCM63146
	{ "FW_POLICER_CBS", 1, CORE_0_INDEX, &FW_POLICER_CBS, 80, 1, 1 },
#endif
#if defined BCM63146
	{ "NATC_COUNTERS_GENERIC_CONTEXT", 1, CORE_0_INDEX, &NATC_COUNTERS_GENERIC_CONTEXT, 128, 1, 1 },
#endif
#if defined BCM63146
	{ "SERVICE_QUEUES_COMPLEX_SCHEDULER_TABLE", 1, CORE_0_INDEX, &SERVICE_QUEUES_COMPLEX_SCHEDULER_TABLE, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "DSL_TX_FLOW_TABLE", 1, CORE_0_INDEX, &DSL_TX_FLOW_TABLE, 256, 1, 1 },
#endif
#if defined BCM63146
	{ "SERVICE_QUEUES_SCHEDULING_QUEUE_TABLE", 1, CORE_0_INDEX, &SERVICE_QUEUES_SCHEDULING_QUEUE_TABLE, 32, 1, 1 },
#endif
#if defined BCM63146
	{ "BASIC_RATE_LIMITER_TABLE_DS", 1, CORE_0_INDEX, &BASIC_RATE_LIMITER_TABLE_DS, 128, 1, 1 },
#endif
#if defined BCM63146
	{ "RX_FLOW_TABLE", 1, CORE_0_INDEX, &RX_FLOW_TABLE, 90, 1, 1 },
#endif
#if defined BCM63146
	{ "MUTEX_TABLE", 1, CORE_0_INDEX, &MUTEX_TABLE, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "TM_BBH_TX_WAKE_UP_DATA_TABLE", 1, CORE_0_INDEX, &TM_BBH_TX_WAKE_UP_DATA_TABLE, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "LOOPBACK_QUEUE_TABLE", 1, CORE_0_INDEX, &LOOPBACK_QUEUE_TABLE, 40, 1, 1 },
#endif
#if defined BCM63146
	{ "IPTV_CONFIGURATION_TABLE", 1, CORE_0_INDEX, &IPTV_CONFIGURATION_TABLE, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "SPDSVC_ANALYZER_PARAMS_TABLE", 1, CORE_0_INDEX, &SPDSVC_ANALYZER_PARAMS_TABLE, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "FW_POLICER_BUDGET", 1, CORE_0_INDEX, &FW_POLICER_BUDGET, 80, 1, 1 },
#endif
#if defined BCM63146
	{ "FW_POLICER_BUDGET_REMAINDER", 1, CORE_0_INDEX, &FW_POLICER_BUDGET_REMAINDER, 80, 1, 1 },
#endif
#if defined BCM63146
	{ "INGRESS_FILTER_L2_REASON_TABLE", 1, CORE_0_INDEX, &INGRESS_FILTER_L2_REASON_TABLE, 16, 1, 1 },
#endif
#if defined BCM63146
	{ "INGRESS_FILTER_PROFILE_TABLE", 1, CORE_0_INDEX, &INGRESS_FILTER_PROFILE_TABLE, 16, 1, 1 },
#endif
#if defined BCM63146
	{ "REGISTERS_BUFFER", 1, CORE_0_INDEX, &REGISTERS_BUFFER, 32, 1, 1 },
#endif
#if defined BCM63146
	{ "VPORT_CFG_EX_TABLE", 1, CORE_0_INDEX, &VPORT_CFG_EX_TABLE, 31, 1, 1 },
#endif
#if defined BCM63146
	{ "DS_TM_FLUSH_CFG_CPU_TABLE", 1, CORE_0_INDEX, &DS_TM_FLUSH_CFG_CPU_TABLE, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "QUEUE_THRESHOLD_VECTOR", 1, CORE_0_INDEX, &QUEUE_THRESHOLD_VECTOR, 9, 1, 1 },
#endif
#if defined BCM63146
	{ "DS_TM_FLUSH_CFG_FW_TABLE", 1, CORE_0_INDEX, &DS_TM_FLUSH_CFG_FW_TABLE, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "GENERAL_QUEUE_DYNAMIC_MNG_TABLE", 1, CORE_0_INDEX, &GENERAL_QUEUE_DYNAMIC_MNG_TABLE, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "DS_TM_FLUSH_DISPATCHER_CREDIT_TABLE", 1, CORE_0_INDEX, &DS_TM_FLUSH_DISPATCHER_CREDIT_TABLE, 3, 1, 1 },
#endif
#if defined BCM63146
	{ "SERVICE_QUEUES_FLUSH_CFG_FW_TABLE", 1, CORE_0_INDEX, &SERVICE_QUEUES_FLUSH_CFG_FW_TABLE, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "FLOW_BASED_ACTION_PTR_TABLE", 1, CORE_0_INDEX, &FLOW_BASED_ACTION_PTR_TABLE, 17, 1, 1 },
#endif
#if defined BCM63146
	{ "BUDGET_ALLOCATION_TIMER_VALUE", 1, CORE_0_INDEX, &BUDGET_ALLOCATION_TIMER_VALUE, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "SERVICE_QUEUES_FLUSH_CFG_CURRENT_TABLE", 1, CORE_0_INDEX, &SERVICE_QUEUES_FLUSH_CFG_CURRENT_TABLE, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "RX_FLOW_CONTEXT_DDR_ADDR", 1, CORE_0_INDEX, &RX_FLOW_CONTEXT_DDR_ADDR, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "FW_POLICER_VECTOR", 1, CORE_0_INDEX, &FW_POLICER_VECTOR, 3, 1, 1 },
#endif
#if defined BCM63146
	{ "SPDTEST_GEN_PARAM", 1, CORE_0_INDEX, &SPDTEST_GEN_PARAM, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "NATC_TBL_CFG", 1, CORE_0_INDEX, &NATC_TBL_CFG, 4, 1, 1 },
#endif
#if defined BCM63146
	{ "FPM_POOL_NUMBER_MAPPING_TABLE", 1, CORE_0_INDEX, &FPM_POOL_NUMBER_MAPPING_TABLE, 32, 1, 1 },
#endif
#if defined BCM63146
	{ "DS_TM_TM_ACTION_PTR_TABLE", 1, CORE_0_INDEX, &DS_TM_TM_ACTION_PTR_TABLE, 17, 1, 1 },
#endif
#if defined BCM63146
	{ "DS_TM_BB_DESTINATION_TABLE", 1, CORE_0_INDEX, &DS_TM_BB_DESTINATION_TABLE, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "TIMER_COMMON_TIMER_VALUE", 1, CORE_0_INDEX, &TIMER_COMMON_TIMER_VALUE, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "SERVICE_QUEUES_BUDGET_ALLOCATION_TIMER_VALUE", 1, CORE_0_INDEX, &SERVICE_QUEUES_BUDGET_ALLOCATION_TIMER_VALUE, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "NULL_BUFFER", 1, CORE_0_INDEX, &NULL_BUFFER, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "SERVICE_QUEUES_DISPATCHER_CREDIT_TABLE", 1, CORE_0_INDEX, &SERVICE_QUEUES_DISPATCHER_CREDIT_TABLE, 3, 1, 1 },
#endif
#if defined BCM63146
	{ "TASK_IDX", 1, CORE_0_INDEX, &TASK_IDX, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "VLAN_ACTION_GPE_HANDLER_PTR_TABLE", 1, CORE_0_INDEX, &VLAN_ACTION_GPE_HANDLER_PTR_TABLE, 17, 1, 1 },
#endif
#if defined BCM63146
	{ "FORCE_DSCP", 1, CORE_0_INDEX, &FORCE_DSCP, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CORE_ID_TABLE", 1, CORE_0_INDEX, &CORE_ID_TABLE, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "EMAC_FLOW_CTRL_VECTOR", 1, CORE_0_INDEX, &EMAC_FLOW_CTRL_VECTOR, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "SRAM_DUMMY_STORE", 1, CORE_0_INDEX, &SRAM_DUMMY_STORE, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "RX_MIRRORING_CONFIGURATION", 1, CORE_0_INDEX, &RX_MIRRORING_CONFIGURATION, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "TIMER_COMMON_CTR_REP", 1, CORE_0_INDEX, &TIMER_COMMON_CTR_REP, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_FPM_THRESHOLDS", 1, CORE_0_INDEX, &DHD_FPM_THRESHOLDS, 3, 1, 1 },
#endif
#if defined BCM63146
	{ "RATE_LIMIT_OVERHEAD", 1, CORE_0_INDEX, &RATE_LIMIT_OVERHEAD, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "INGRESS_QOS_DONT_DROP_RATIO", 1, CORE_0_INDEX, &INGRESS_QOS_DONT_DROP_RATIO, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "DS_TM_FIRST_QUEUE_MAPPING", 1, CORE_0_INDEX, &DS_TM_FIRST_QUEUE_MAPPING, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "MULTI_FLOW_FLAG_TABLE", 1, CORE_0_INDEX, &MULTI_FLOW_FLAG_TABLE, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "UPDATE_FIFO_TABLE", 1, CORE_0_INDEX, &UPDATE_FIFO_TABLE, 8, 1, 1 },
#endif
#if defined BCM63146
	{ "VPORT_TO_LOOKUP_PORT_MAPPING_TABLE", 1, CORE_0_INDEX, &VPORT_TO_LOOKUP_PORT_MAPPING_TABLE, 32, 1, 1 },
#endif
#if defined BCM63146
	{ "EMAC_FLOW_CTRL_BUDGET", 1, CORE_0_INDEX, &EMAC_FLOW_CTRL_BUDGET, 8, 1, 1 },
#endif
#if defined BCM63146
	{ "DS_TM_SCHEDULING_QUEUE_AGING_VECTOR", 1, CORE_0_INDEX, &DS_TM_SCHEDULING_QUEUE_AGING_VECTOR, 5, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_REDIRECT_MODE", 1, CORE_0_INDEX, &CPU_REDIRECT_MODE, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "SERVICE_QUEUES_FIRST_QUEUE_MAPPING", 1, CORE_0_INDEX, &SERVICE_QUEUES_FIRST_QUEUE_MAPPING, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "DEBUG_PRINT_CORE_LOCK", 1, CORE_0_INDEX, &DEBUG_PRINT_CORE_LOCK, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "IPTV_CLASSIFICATION_CFG_TABLE", 1, CORE_0_INDEX, &IPTV_CLASSIFICATION_CFG_TABLE, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "SYSTEM_CONFIGURATION", 1, CORE_0_INDEX, &SYSTEM_CONFIGURATION, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "INGRESS_FILTER_1588_CFG", 1, CORE_0_INDEX, &INGRESS_FILTER_1588_CFG, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "RX_MIRRORING_DIRECT_ENABLE", 1, CORE_0_INDEX, &RX_MIRRORING_DIRECT_ENABLE, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "SERVICE_QUEUES_UPDATE_FIFO_TABLE", 1, CORE_0_INDEX, &SERVICE_QUEUES_UPDATE_FIFO_TABLE, 8, 1, 1 },
#endif
#if defined BCM63146
	{ "DEBUG_SCRATCHPAD", 1, CORE_0_INDEX, &DEBUG_SCRATCHPAD, 12, 1, 1 },
#endif
#if defined BCM63146
	{ "SPDSVC_BBH_TX_PARAMS_TABLE", 1, CORE_0_INDEX, &SPDSVC_BBH_TX_PARAMS_TABLE, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "SERVICE_QUEUES_SCHEDULING_QUEUE_AGING_VECTOR", 1, CORE_0_INDEX, &SERVICE_QUEUES_SCHEDULING_QUEUE_AGING_VECTOR, 5, 1, 1 },
#endif
#if defined BCM63146
	{ "EMAC_FLOW_CTRL_BUDGET_REMAINDER", 1, CORE_0_INDEX, &EMAC_FLOW_CTRL_BUDGET_REMAINDER, 8, 1, 1 },
#endif
#if defined BCM63146
	{ "DS_TM_BBH_QUEUE_TABLE", 1, CORE_0_INDEX, &DS_TM_BBH_QUEUE_TABLE, 8, 1, 1 },
#endif
#if defined BCM63146
	{ "DS_TM_SCHEDULING_AGGREGATION_CONTEXT_VECTOR", 1, CORE_0_INDEX, &DS_TM_SCHEDULING_AGGREGATION_CONTEXT_VECTOR, 5, 1, 1 },
#endif
#if defined BCM63146
	{ "DS_TM_FLUSH_CFG_CURRENT_TABLE", 1, CORE_0_INDEX, &DS_TM_FLUSH_CFG_CURRENT_TABLE, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "SERVICE_QUEUES_RATE_LIMITER_VALID_TABLE", 1, CORE_0_INDEX, &SERVICE_QUEUES_RATE_LIMITER_VALID_TABLE, 4, 1, 1 },
#endif
#if defined BCM63146
	{ "FPM_GLOBAL_CFG", 1, CORE_0_INDEX, &FPM_GLOBAL_CFG, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "TCAM_IC_CFG_TABLE", 1, CORE_0_INDEX, &TCAM_IC_CFG_TABLE, 2, 1, 1 },
#endif
#if defined BCM63146
	{ "BITS_CALC_MASKS_TABLE", 1, CORE_0_INDEX, &BITS_CALC_MASKS_TABLE, 4, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_FEED_RING_DESCRIPTOR_TABLE", 1, CORE_0_INDEX, &CPU_FEED_RING_DESCRIPTOR_TABLE, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "RATE_LIMITER_VALID_TABLE_DS", 1, CORE_0_INDEX, &RATE_LIMITER_VALID_TABLE_DS, 4, 1, 1 },
#endif
#if defined BCM63146
	{ "DEBUG_PRINT_TABLE", 1, CORE_0_INDEX, &DEBUG_PRINT_TABLE, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "DS_TM_BBH_TX_EGRESS_COUNTER_TABLE", 1, CORE_0_INDEX, &DS_TM_BBH_TX_EGRESS_COUNTER_TABLE, 8, 1, 1 },
#endif
#if defined BCM63146
	{ "TUNNELS_PARSING_CFG", 1, CORE_0_INDEX, &TUNNELS_PARSING_CFG, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "IPTV_CFG_TABLE", 1, CORE_0_INDEX, &IPTV_CFG_TABLE, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "IPTV_DDR_CTX_TABLE_ADDRESS", 1, CORE_0_INDEX, &IPTV_DDR_CTX_TABLE_ADDRESS, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "MULTICAST_KEY_MASK", 1, CORE_0_INDEX, &MULTICAST_KEY_MASK, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "NAT_CACHE_CFG", 1, CORE_0_INDEX, &NAT_CACHE_CFG, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "DS_TM_FLUSH_CFG_ENABLE_TABLE", 1, CORE_0_INDEX, &DS_TM_FLUSH_CFG_ENABLE_TABLE, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "SERVICE_QUEUES_FLUSH_CFG_ENABLE_TABLE", 1, CORE_0_INDEX, &SERVICE_QUEUES_FLUSH_CFG_ENABLE_TABLE, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "DS_TM_BBH_TX_ABS_COUNTER_TABLE", 1, CORE_0_INDEX, &DS_TM_BBH_TX_ABS_COUNTER_TABLE, 8, 1, 1 },
#endif
#if defined BCM63146
	{ "NAT_CACHE_KEY0_MASK", 1, CORE_0_INDEX, &NAT_CACHE_KEY0_MASK, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "NATC_L2_VLAN_KEY_MASK", 1, CORE_0_INDEX, &NATC_L2_VLAN_KEY_MASK, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "INGRESS_FILTER_CFG", 1, CORE_0_INDEX, &INGRESS_FILTER_CFG, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "NATC_L2_TOS_MASK", 1, CORE_0_INDEX, &NATC_L2_TOS_MASK, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "SERVICE_QUEUES_RATE_LIMITER_TABLE", 1, CORE_0_INDEX, &SERVICE_QUEUES_RATE_LIMITER_TABLE, 66, 1, 1 },
#endif
#if defined BCM63146
	{ "FC_FLOW_IP_ADDRESSES_TABLE", 1, CORE_0_INDEX, &FC_FLOW_IP_ADDRESSES_TABLE, 4, 1, 1 },
#endif
#if defined BCM63146
	{ "RUNNER_PROFILING_TRACE_BUFFER", 1, CORE_0_INDEX, &RUNNER_PROFILING_TRACE_BUFFER, 128, 1, 1 },
#endif
#if defined BCM63146
	{ "US_TM_PD_FIFO_TABLE", 1, CORE_1_INDEX, &US_TM_PD_FIFO_TABLE_1, 128, 1, 1 },
#endif
#if defined BCM63146
	{ "US_TM_SCHEDULING_QUEUE_TABLE", 1, CORE_1_INDEX, &US_TM_SCHEDULING_QUEUE_TABLE_1, 64, 1, 1 },
#endif
#if defined BCM63146
	{ "BASIC_SCHEDULER_TABLE_US", 1, CORE_1_INDEX, &BASIC_SCHEDULER_TABLE_US_1, 32, 1, 1 },
#endif
#if defined BCM63146
	{ "DSL_TM_PD_FIFO_TABLE", 1, CORE_1_INDEX, &DSL_TM_PD_FIFO_TABLE_1, 32, 1, 1 },
#endif
#if defined BCM63146
	{ "LAYER2_GRE_TUNNEL_TABLE", 1, CORE_1_INDEX, &LAYER2_GRE_TUNNEL_TABLE_1, 12, 1, 1 },
#endif
#if defined BCM63146
	{ "COMPLEX_SCHEDULER_TABLE", 1, CORE_1_INDEX, &COMPLEX_SCHEDULER_TABLE_1, 2, 1, 1 },
#endif
#if defined BCM63146
	{ "IMG1_PACKET_BUFFER", 1, CORE_1_INDEX, &IMG1_PACKET_BUFFER_1, 4, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_COMPLETE_COMMON_RADIO_DATA", 1, CORE_1_INDEX, &DHD_COMPLETE_COMMON_RADIO_DATA_1, 3, 1, 1 },
#endif
#if defined BCM63146
	{ "DIRECT_PROCESSING_PD_TABLE", 1, CORE_1_INDEX, &DIRECT_PROCESSING_PD_TABLE_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "FPM_POOL_NUMBER_MAPPING_TABLE", 1, CORE_1_INDEX, &FPM_POOL_NUMBER_MAPPING_TABLE_1, 32, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_RX_COMPLETE_FLOW_RING_BUFFER", 1, CORE_1_INDEX, &DHD_RX_COMPLETE_FLOW_RING_BUFFER_1, 3, 1, 1 },
#endif
#if defined BCM63146
	{ "VPORT_TO_LOOKUP_PORT_MAPPING_TABLE", 1, CORE_1_INDEX, &VPORT_TO_LOOKUP_PORT_MAPPING_TABLE_1, 32, 1, 1 },
#endif
#if defined BCM63146
	{ "VPORT_CFG_TABLE", 1, CORE_1_INDEX, &VPORT_CFG_TABLE_1, 40, 1, 1 },
#endif
#if defined BCM63146
	{ "US_TM_ETH_TM_FLOW_CNTR_TABLE", 1, CORE_1_INDEX, &US_TM_ETH_TM_FLOW_CNTR_TABLE_1, 64, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_RX_POST_FLOW_RING_BUFFER", 1, CORE_1_INDEX, &DHD_RX_POST_FLOW_RING_BUFFER_1, 3, 1, 1 },
#endif
#if defined BCM63146
	{ "PFC_TX_STATUS_TABLE", 1, CORE_1_INDEX, &PFC_TX_STATUS_TABLE_1, 8, 1, 1 },
#endif
#if defined BCM63146
	{ "US_TM_DSL_TM_FLOW_CNTR_TABLE", 1, CORE_1_INDEX, &US_TM_DSL_TM_FLOW_CNTR_TABLE_1, 256, 1, 1 },
#endif
#if defined BCM63146
	{ "DSL_TX_FLOW_TABLE", 1, CORE_1_INDEX, &DSL_TX_FLOW_TABLE_1, 256, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_RX_COMPLETE_TASK_0_STACK", 1, CORE_1_INDEX, &DHD_RX_COMPLETE_TASK_0_STACK_1, 256, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_RX_COMPLETE_TASK_1_STACK", 1, CORE_1_INDEX, &DHD_RX_COMPLETE_TASK_1_STACK_1, 256, 1, 1 },
#endif
#if defined BCM63146
	{ "BASIC_RATE_LIMITER_TABLE_US", 1, CORE_1_INDEX, &BASIC_RATE_LIMITER_TABLE_US_1, 128, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_RX_COMPLETE_TASK_2_STACK", 1, CORE_1_INDEX, &DHD_RX_COMPLETE_TASK_2_STACK_1, 256, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_TX_COMPLETE_TASK_0_STACK", 1, CORE_1_INDEX, &DHD_TX_COMPLETE_TASK_0_STACK_1, 256, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_TX_COMPLETE_TASK_1_STACK", 1, CORE_1_INDEX, &DHD_TX_COMPLETE_TASK_1_STACK_1, 256, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_TX_COMPLETE_TASK_2_STACK", 1, CORE_1_INDEX, &DHD_TX_COMPLETE_TASK_2_STACK_1, 256, 1, 1 },
#endif
#if defined BCM63146
	{ "RX_FLOW_TABLE", 1, CORE_1_INDEX, &RX_FLOW_TABLE_1, 90, 1, 1 },
#endif
#if defined BCM63146
	{ "MUTEX_TABLE", 1, CORE_1_INDEX, &MUTEX_TABLE_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "TM_DSL_BBH_TX_WAKE_UP_DATA_TABLE", 1, CORE_1_INDEX, &TM_DSL_BBH_TX_WAKE_UP_DATA_TABLE_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "VPORT_TX_FLOW_TABLE", 1, CORE_1_INDEX, &VPORT_TX_FLOW_TABLE_1, 64, 1, 1 },
#endif
#if defined BCM63146
	{ "INGRESS_FILTER_PROFILE_TABLE", 1, CORE_1_INDEX, &INGRESS_FILTER_PROFILE_TABLE_1, 16, 1, 1 },
#endif
#if defined BCM63146
	{ "GPE_COMMAND_PRIMITIVE_TABLE", 1, CORE_1_INDEX, &GPE_COMMAND_PRIMITIVE_TABLE_1, 32, 1, 1 },
#endif
#if defined BCM63146
	{ "PFC_FRAME", 1, CORE_1_INDEX, &PFC_FRAME_1, 64, 1, 1 },
#endif
#if defined BCM63146
	{ "VPORT_CFG_EX_TABLE", 1, CORE_1_INDEX, &VPORT_CFG_EX_TABLE_1, 31, 1, 1 },
#endif
#if defined BCM63146
	{ "US_TM_FLUSH_CFG_CPU_TABLE", 1, CORE_1_INDEX, &US_TM_FLUSH_CFG_CPU_TABLE_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "NATC_COUNTERS_GENERIC_CONTEXT", 1, CORE_1_INDEX, &NATC_COUNTERS_GENERIC_CONTEXT_1, 128, 1, 1 },
#endif
#if defined BCM63146
	{ "NATC_TBL_CFG", 1, CORE_1_INDEX, &NATC_TBL_CFG_1, 4, 1, 1 },
#endif
#if defined BCM63146
	{ "REGISTERS_BUFFER", 1, CORE_1_INDEX, &REGISTERS_BUFFER_1, 32, 1, 1 },
#endif
#if defined BCM63146
	{ "PFC_PD_TABLE", 1, CORE_1_INDEX, &PFC_PD_TABLE_1, 2, 1, 1 },
#endif
#if defined BCM63146
	{ "US_TM_BBH_QUEUE_TABLE", 1, CORE_1_INDEX, &US_TM_BBH_QUEUE_TABLE_1, 24, 1, 1 },
#endif
#if defined BCM63146
	{ "US_TM_SCHEDULING_QUEUE_AGING_VECTOR", 1, CORE_1_INDEX, &US_TM_SCHEDULING_QUEUE_AGING_VECTOR_1, 5, 1, 1 },
#endif
#if defined BCM63146
	{ "US_TM_FLUSH_CFG_FW_TABLE", 1, CORE_1_INDEX, &US_TM_FLUSH_CFG_FW_TABLE_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "TM_BBH_TX_WAKE_UP_DATA_TABLE", 1, CORE_1_INDEX, &TM_BBH_TX_WAKE_UP_DATA_TABLE_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_CPU_RECYCLE_RING_DESCRIPTOR_TABLE", 1, CORE_1_INDEX, &DHD_CPU_RECYCLE_RING_DESCRIPTOR_TABLE_1, 3, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_HW_CFG", 1, CORE_1_INDEX, &DHD_HW_CFG_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_TX_COMPLETE_FLOW_RING_BUFFER", 1, CORE_1_INDEX, &DHD_TX_COMPLETE_FLOW_RING_BUFFER_1, 3, 1, 1 },
#endif
#if defined BCM63146
	{ "SPDSVC_ANALYZER_PARAMS_TABLE", 1, CORE_1_INDEX, &SPDSVC_ANALYZER_PARAMS_TABLE_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_RX_COMPLETE_DISPATCHER_CREDIT_TABLE", 1, CORE_1_INDEX, &DHD_RX_COMPLETE_DISPATCHER_CREDIT_TABLE_1, 12, 1, 1 },
#endif
#if defined BCM63146
	{ "INGRESS_FILTER_L2_REASON_TABLE", 1, CORE_1_INDEX, &INGRESS_FILTER_L2_REASON_TABLE_1, 16, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_TX_COMPLETE_DISPATCHER_CREDIT_TABLE", 1, CORE_1_INDEX, &DHD_TX_COMPLETE_DISPATCHER_CREDIT_TABLE_1, 12, 1, 1 },
#endif
#if defined BCM63146
	{ "OVERALL_RATE_LIMITER_TABLE", 1, CORE_1_INDEX, &OVERALL_RATE_LIMITER_TABLE_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "LOOPBACK_QUEUE_TABLE", 1, CORE_1_INDEX, &LOOPBACK_QUEUE_TABLE_1, 40, 1, 1 },
#endif
#if defined BCM63146
	{ "IPTV_CONFIGURATION_TABLE", 1, CORE_1_INDEX, &IPTV_CONFIGURATION_TABLE_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "US_TM_FLUSH_DISPATCHER_CREDIT_TABLE", 1, CORE_1_INDEX, &US_TM_FLUSH_DISPATCHER_CREDIT_TABLE_1, 3, 1, 1 },
#endif
#if defined BCM63146
	{ "US_TM_FLUSH_CFG_CURRENT_TABLE", 1, CORE_1_INDEX, &US_TM_FLUSH_CFG_CURRENT_TABLE_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "QUEUE_THRESHOLD_VECTOR", 1, CORE_1_INDEX, &QUEUE_THRESHOLD_VECTOR_1, 9, 1, 1 },
#endif
#if defined BCM63146
	{ "SPDTEST_GEN_PARAM", 1, CORE_1_INDEX, &SPDTEST_GEN_PARAM_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "GENERAL_QUEUE_DYNAMIC_MNG_TABLE", 1, CORE_1_INDEX, &GENERAL_QUEUE_DYNAMIC_MNG_TABLE_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "DISPATCHER_CREDIT_TABLE", 1, CORE_1_INDEX, &DISPATCHER_CREDIT_TABLE_1, 3, 1, 1 },
#endif
#if defined BCM63146
	{ "BUDGET_ALLOCATION_TIMER_VALUE", 1, CORE_1_INDEX, &BUDGET_ALLOCATION_TIMER_VALUE_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "US_TM_BB_DESTINATION_TABLE", 1, CORE_1_INDEX, &US_TM_BB_DESTINATION_TABLE_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "FLOW_BASED_ACTION_PTR_TABLE", 1, CORE_1_INDEX, &FLOW_BASED_ACTION_PTR_TABLE_1, 17, 1, 1 },
#endif
#if defined BCM63146
	{ "PFC_PENDING_QUEUE_MAP", 1, CORE_1_INDEX, &PFC_PENDING_QUEUE_MAP_1, 2, 1, 1 },
#endif
#if defined BCM63146
	{ "TASK_IDX", 1, CORE_1_INDEX, &TASK_IDX_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "RX_FLOW_CONTEXT_DDR_ADDR", 1, CORE_1_INDEX, &RX_FLOW_CONTEXT_DDR_ADDR_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_FPM_THRESHOLDS", 1, CORE_1_INDEX, &DHD_FPM_THRESHOLDS_1, 3, 1, 1 },
#endif
#if defined BCM63146
	{ "DIRECT_PROCESSING_WAN_VIQ_EXCLUSIVE", 1, CORE_1_INDEX, &DIRECT_PROCESSING_WAN_VIQ_EXCLUSIVE_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "FORCE_DSCP", 1, CORE_1_INDEX, &FORCE_DSCP_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CORE_ID_TABLE", 1, CORE_1_INDEX, &CORE_ID_TABLE_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "PFC_TX_ENABLE", 1, CORE_1_INDEX, &PFC_TX_ENABLE_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "US_TM_TM_ACTION_PTR_TABLE", 1, CORE_1_INDEX, &US_TM_TM_ACTION_PTR_TABLE_1, 17, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_RX_POST_RING_SIZE", 1, CORE_1_INDEX, &DHD_RX_POST_RING_SIZE_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_RX_COMPLETE_RING_SIZE", 1, CORE_1_INDEX, &DHD_RX_COMPLETE_RING_SIZE_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_TX_COMPLETE_RING_SIZE", 1, CORE_1_INDEX, &DHD_TX_COMPLETE_RING_SIZE_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "NULL_BUFFER", 1, CORE_1_INDEX, &NULL_BUFFER_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_DOORBELL_TX_COMPLETE_VALUE", 1, CORE_1_INDEX, &DHD_DOORBELL_TX_COMPLETE_VALUE_1, 3, 1, 1 },
#endif
#if defined BCM63146
	{ "RX_MIRRORING_CONFIGURATION", 1, CORE_1_INDEX, &RX_MIRRORING_CONFIGURATION_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "TX_MIRRORING_CONFIGURATION", 1, CORE_1_INDEX, &TX_MIRRORING_CONFIGURATION_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "VLAN_ACTION_GPE_HANDLER_PTR_TABLE", 1, CORE_1_INDEX, &VLAN_ACTION_GPE_HANDLER_PTR_TABLE_1, 17, 1, 1 },
#endif
#if defined BCM63146
	{ "SRAM_DUMMY_STORE", 1, CORE_1_INDEX, &SRAM_DUMMY_STORE_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "RATE_LIMIT_OVERHEAD", 1, CORE_1_INDEX, &RATE_LIMIT_OVERHEAD_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "INGRESS_QOS_DONT_DROP_RATIO", 1, CORE_1_INDEX, &INGRESS_QOS_DONT_DROP_RATIO_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "US_TM_DSL_BBH_TX_FIFO_SIZE", 1, CORE_1_INDEX, &US_TM_DSL_BBH_TX_FIFO_SIZE_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "US_TM_FIRST_QUEUE_MAPPING", 1, CORE_1_INDEX, &US_TM_FIRST_QUEUE_MAPPING_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "MAC_TYPE", 1, CORE_1_INDEX, &MAC_TYPE_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_CPU_RECYCLE_INTERRUPT_ID_DDR_ADDR", 1, CORE_1_INDEX, &DHD_CPU_RECYCLE_INTERRUPT_ID_DDR_ADDR_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_DOORBELL_RX_COMPLETE_VALUE", 1, CORE_1_INDEX, &DHD_DOORBELL_RX_COMPLETE_VALUE_1, 3, 1, 1 },
#endif
#if defined BCM63146
	{ "MULTI_FLOW_FLAG_TABLE", 1, CORE_1_INDEX, &MULTI_FLOW_FLAG_TABLE_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_REDIRECT_MODE", 1, CORE_1_INDEX, &CPU_REDIRECT_MODE_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "DEBUG_PRINT_CORE_LOCK", 1, CORE_1_INDEX, &DEBUG_PRINT_CORE_LOCK_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "IPTV_CLASSIFICATION_CFG_TABLE", 1, CORE_1_INDEX, &IPTV_CLASSIFICATION_CFG_TABLE_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "UPDATE_FIFO_TABLE", 1, CORE_1_INDEX, &UPDATE_FIFO_TABLE_1, 8, 1, 1 },
#endif
#if defined BCM63146
	{ "DEBUG_SCRATCHPAD", 1, CORE_1_INDEX, &DEBUG_SCRATCHPAD_1, 12, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_DOORBELL_RX_POST_VALUE", 1, CORE_1_INDEX, &DHD_DOORBELL_RX_POST_VALUE_1, 3, 1, 1 },
#endif
#if defined BCM63146
	{ "INGRESS_FILTER_1588_CFG", 1, CORE_1_INDEX, &INGRESS_FILTER_1588_CFG_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "RX_MIRRORING_DIRECT_ENABLE", 1, CORE_1_INDEX, &RX_MIRRORING_DIRECT_ENABLE_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "US_TM_SCHEDULING_AGGREGATION_CONTEXT_VECTOR", 1, CORE_1_INDEX, &US_TM_SCHEDULING_AGGREGATION_CONTEXT_VECTOR_1, 5, 1, 1 },
#endif
#if defined BCM63146
	{ "MIRRORING_SCRATCH", 1, CORE_1_INDEX, &MIRRORING_SCRATCH_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_FPM_REPLY", 1, CORE_1_INDEX, &DHD_FPM_REPLY_1, 24, 1, 1 },
#endif
#if defined BCM63146
	{ "SYSTEM_CONFIGURATION", 1, CORE_1_INDEX, &SYSTEM_CONFIGURATION_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "TCAM_IC_CFG_TABLE", 1, CORE_1_INDEX, &TCAM_IC_CFG_TABLE_1, 2, 1, 1 },
#endif
#if defined BCM63146
	{ "US_TM_DSL_BBH_TX_EGRESS_COUNTER_TABLE", 1, CORE_1_INDEX, &US_TM_DSL_BBH_TX_EGRESS_COUNTER_TABLE_1, 16, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_CPU_INT_ID", 1, CORE_1_INDEX, &DHD_CPU_INT_ID_1, 3, 1, 1 },
#endif
#if defined BCM63146
	{ "RATE_LIMITER_VALID_TABLE_US", 1, CORE_1_INDEX, &RATE_LIMITER_VALID_TABLE_US_1, 4, 1, 1 },
#endif
#if defined BCM63146
	{ "SPDSVC_BBH_TX_PARAMS_TABLE", 1, CORE_1_INDEX, &SPDSVC_BBH_TX_PARAMS_TABLE_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "US_TM_DSL_BBH_TX_ABS_COUNTER_TABLE", 1, CORE_1_INDEX, &US_TM_DSL_BBH_TX_ABS_COUNTER_TABLE_1, 16, 1, 1 },
#endif
#if defined BCM63146
	{ "TX_MIRRORING_DISPATCHER_CREDIT_TABLE", 1, CORE_1_INDEX, &TX_MIRRORING_DISPATCHER_CREDIT_TABLE_1, 3, 1, 1 },
#endif
#if defined BCM63146
	{ "SPDSVC_WLAN_GEN_PARAMS_TABLE", 1, CORE_1_INDEX, &SPDSVC_WLAN_GEN_PARAMS_TABLE_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "FPM_GLOBAL_CFG", 1, CORE_1_INDEX, &FPM_GLOBAL_CFG_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "BITS_CALC_MASKS_TABLE", 1, CORE_1_INDEX, &BITS_CALC_MASKS_TABLE_1, 4, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_FEED_RING_DESCRIPTOR_TABLE", 1, CORE_1_INDEX, &CPU_FEED_RING_DESCRIPTOR_TABLE_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "DEBUG_PRINT_TABLE", 1, CORE_1_INDEX, &DEBUG_PRINT_TABLE_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "TUNNELS_PARSING_CFG", 1, CORE_1_INDEX, &TUNNELS_PARSING_CFG_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "IPTV_CFG_TABLE", 1, CORE_1_INDEX, &IPTV_CFG_TABLE_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "US_TM_BBH_TX_EGRESS_COUNTER_TABLE", 1, CORE_1_INDEX, &US_TM_BBH_TX_EGRESS_COUNTER_TABLE_1, 8, 1, 1 },
#endif
#if defined BCM63146
	{ "IPTV_DDR_CTX_TABLE_ADDRESS", 1, CORE_1_INDEX, &IPTV_DDR_CTX_TABLE_ADDRESS_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "NAT_CACHE_CFG", 1, CORE_1_INDEX, &NAT_CACHE_CFG_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "US_TM_FLUSH_CFG_ENABLE_TABLE", 1, CORE_1_INDEX, &US_TM_FLUSH_CFG_ENABLE_TABLE_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "MULTICAST_KEY_MASK", 1, CORE_1_INDEX, &MULTICAST_KEY_MASK_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "NAT_CACHE_KEY0_MASK", 1, CORE_1_INDEX, &NAT_CACHE_KEY0_MASK_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "NATC_L2_VLAN_KEY_MASK", 1, CORE_1_INDEX, &NATC_L2_VLAN_KEY_MASK_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "INGRESS_FILTER_CFG", 1, CORE_1_INDEX, &INGRESS_FILTER_CFG_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "US_TM_BBH_TX_ABS_COUNTER_TABLE", 1, CORE_1_INDEX, &US_TM_BBH_TX_ABS_COUNTER_TABLE_1, 8, 1, 1 },
#endif
#if defined BCM63146
	{ "NATC_L2_TOS_MASK", 1, CORE_1_INDEX, &NATC_L2_TOS_MASK_1, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "FC_FLOW_IP_ADDRESSES_TABLE", 1, CORE_1_INDEX, &FC_FLOW_IP_ADDRESSES_TABLE_1, 4, 1, 1 },
#endif
#if defined BCM63146
	{ "RUNNER_PROFILING_TRACE_BUFFER", 1, CORE_1_INDEX, &RUNNER_PROFILING_TRACE_BUFFER_1, 128, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_TX_SCRATCHPAD", 1, CORE_2_INDEX, &CPU_TX_SCRATCHPAD_2, 256, 1, 1 },
#endif
#if defined BCM63146
	{ "LAYER2_GRE_TUNNEL_TABLE", 1, CORE_2_INDEX, &LAYER2_GRE_TUNNEL_TABLE_2, 12, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_TX_DBG_CNTRS_TABLE", 1, CORE_2_INDEX, &CPU_TX_DBG_CNTRS_TABLE_2, 2, 1, 1 },
#endif
#if defined BCM63146
	{ "VPORT_CFG_TABLE", 1, CORE_2_INDEX, &VPORT_CFG_TABLE_2, 40, 1, 1 },
#endif
#if defined BCM63146
	{ "VPORT_TX_FLOW_TABLE", 1, CORE_2_INDEX, &VPORT_TX_FLOW_TABLE_2, 64, 1, 1 },
#endif
#if defined BCM63146
	{ "GPE_COMMAND_PRIMITIVE_TABLE", 1, CORE_2_INDEX, &GPE_COMMAND_PRIMITIVE_TABLE_2, 32, 1, 1 },
#endif
#if defined BCM63146
	{ "LOOPBACK_QUEUE_TABLE", 1, CORE_2_INDEX, &LOOPBACK_QUEUE_TABLE_2, 40, 1, 1 },
#endif
#if defined BCM63146
	{ "IPTV_CONFIGURATION_TABLE", 1, CORE_2_INDEX, &IPTV_CONFIGURATION_TABLE_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "BUFFER_ALLOC_REPLY", 1, CORE_2_INDEX, &BUFFER_ALLOC_REPLY_2, 2, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_RECYCLE_SRAM_PD_FIFO", 1, CORE_2_INDEX, &CPU_RECYCLE_SRAM_PD_FIFO_2, 16, 1, 1 },
#endif
#if defined BCM63146
	{ "DSL_TX_FLOW_TABLE", 1, CORE_2_INDEX, &DSL_TX_FLOW_TABLE_2, 256, 1, 1 },
#endif
#if defined BCM63146
	{ "QM_QUEUE_TO_TX_FLOW_TABLE_PTR_TABLE", 1, CORE_2_INDEX, &QM_QUEUE_TO_TX_FLOW_TABLE_PTR_TABLE_2, 128, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_TX_TASK_STACK", 1, CORE_2_INDEX, &CPU_TX_TASK_STACK_2, 256, 1, 1 },
#endif
#if defined BCM63146
	{ "IMG2_PACKET_BUFFER", 1, CORE_2_INDEX, &IMG2_PACKET_BUFFER_2, 8, 1, 1 },
#endif
#if defined BCM63146
	{ "RX_FLOW_TABLE", 1, CORE_2_INDEX, &RX_FLOW_TABLE_2, 90, 1, 1 },
#endif
#if defined BCM63146
	{ "MUTEX_TABLE", 1, CORE_2_INDEX, &MUTEX_TABLE_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "GENERAL_QUEUE_DYNAMIC_MNG_TABLE", 1, CORE_2_INDEX, &GENERAL_QUEUE_DYNAMIC_MNG_TABLE_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "QUEUE_THRESHOLD_VECTOR", 1, CORE_2_INDEX, &QUEUE_THRESHOLD_VECTOR_2, 9, 1, 1 },
#endif
#if defined BCM63146
	{ "SPDTEST_GEN_PARAM", 1, CORE_2_INDEX, &SPDTEST_GEN_PARAM_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "RX_FLOW_CONTEXT_DDR_ADDR", 1, CORE_2_INDEX, &RX_FLOW_CONTEXT_DDR_ADDR_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_RECYCLE_INTERRUPT_COALESCING_TABLE", 1, CORE_2_INDEX, &CPU_RECYCLE_INTERRUPT_COALESCING_TABLE_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "INGRESS_FILTER_PROFILE_TABLE", 1, CORE_2_INDEX, &INGRESS_FILTER_PROFILE_TABLE_2, 16, 1, 1 },
#endif
#if defined BCM63146
	{ "NATC_COUNTERS_GENERIC_CONTEXT", 1, CORE_2_INDEX, &NATC_COUNTERS_GENERIC_CONTEXT_2, 128, 1, 1 },
#endif
#if defined BCM63146
	{ "VPORT_CFG_EX_TABLE", 1, CORE_2_INDEX, &VPORT_CFG_EX_TABLE_2, 31, 1, 1 },
#endif
#if defined BCM63146
	{ "TASK_IDX", 1, CORE_2_INDEX, &TASK_IDX_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "REGISTERS_BUFFER", 1, CORE_2_INDEX, &REGISTERS_BUFFER_2, 32, 1, 1 },
#endif
#if defined BCM63146
	{ "NATC_TBL_CFG", 1, CORE_2_INDEX, &NATC_TBL_CFG_2, 4, 1, 1 },
#endif
#if defined BCM63146
	{ "FPM_POOL_NUMBER_MAPPING_TABLE", 1, CORE_2_INDEX, &FPM_POOL_NUMBER_MAPPING_TABLE_2, 32, 1, 1 },
#endif
#if defined BCM63146
	{ "FLOW_BASED_ACTION_PTR_TABLE", 1, CORE_2_INDEX, &FLOW_BASED_ACTION_PTR_TABLE_2, 17, 1, 1 },
#endif
#if defined BCM63146
	{ "FORCE_DSCP", 1, CORE_2_INDEX, &FORCE_DSCP_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CORE_ID_TABLE", 1, CORE_2_INDEX, &CORE_ID_TABLE_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "RX_MIRRORING_CONFIGURATION", 1, CORE_2_INDEX, &RX_MIRRORING_CONFIGURATION_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "SRAM_DUMMY_STORE", 1, CORE_2_INDEX, &SRAM_DUMMY_STORE_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "INGRESS_QOS_DONT_DROP_RATIO", 1, CORE_2_INDEX, &INGRESS_QOS_DONT_DROP_RATIO_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "NULL_BUFFER", 1, CORE_2_INDEX, &NULL_BUFFER_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "SPDSVC_ANALYZER_PARAMS_TABLE", 1, CORE_2_INDEX, &SPDSVC_ANALYZER_PARAMS_TABLE_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "VLAN_ACTION_GPE_HANDLER_PTR_TABLE", 1, CORE_2_INDEX, &VLAN_ACTION_GPE_HANDLER_PTR_TABLE_2, 17, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_TX_RECYCLE_RING_ID", 1, CORE_2_INDEX, &CPU_TX_RECYCLE_RING_ID_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "MULTI_FLOW_FLAG_TABLE", 1, CORE_2_INDEX, &MULTI_FLOW_FLAG_TABLE_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_REDIRECT_MODE", 1, CORE_2_INDEX, &CPU_REDIRECT_MODE_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "DEBUG_PRINT_CORE_LOCK", 1, CORE_2_INDEX, &DEBUG_PRINT_CORE_LOCK_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "IPTV_CLASSIFICATION_CFG_TABLE", 1, CORE_2_INDEX, &IPTV_CLASSIFICATION_CFG_TABLE_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "INGRESS_FILTER_1588_CFG", 1, CORE_2_INDEX, &INGRESS_FILTER_1588_CFG_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_RECYCLE_INTERRUPT_ID_DDR_ADDR", 1, CORE_2_INDEX, &CPU_RECYCLE_INTERRUPT_ID_DDR_ADDR_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "INGRESS_FILTER_L2_REASON_TABLE", 1, CORE_2_INDEX, &INGRESS_FILTER_L2_REASON_TABLE_2, 16, 1, 1 },
#endif
#if defined BCM63146
	{ "VPORT_TO_LOOKUP_PORT_MAPPING_TABLE", 1, CORE_2_INDEX, &VPORT_TO_LOOKUP_PORT_MAPPING_TABLE_2, 32, 1, 1 },
#endif
#if defined BCM63146
	{ "DEBUG_SCRATCHPAD", 1, CORE_2_INDEX, &DEBUG_SCRATCHPAD_2, 12, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_TX_RING_DESCRIPTOR_TABLE", 1, CORE_2_INDEX, &CPU_TX_RING_DESCRIPTOR_TABLE_2, 2, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_TX_EGRESS_DISPATCHER_CREDIT_TABLE", 1, CORE_2_INDEX, &CPU_TX_EGRESS_DISPATCHER_CREDIT_TABLE_2, 3, 1, 1 },
#endif
#if defined BCM63146
	{ "RX_MIRRORING_DIRECT_ENABLE", 1, CORE_2_INDEX, &RX_MIRRORING_DIRECT_ENABLE_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "RING_CPU_TX_DESCRIPTOR_DATA_TABLE", 1, CORE_2_INDEX, &RING_CPU_TX_DESCRIPTOR_DATA_TABLE_2, 2, 1, 1 },
#endif
#if defined BCM63146
	{ "TCAM_IC_CFG_TABLE", 1, CORE_2_INDEX, &TCAM_IC_CFG_TABLE_2, 2, 1, 1 },
#endif
#if defined BCM63146
	{ "BITS_CALC_MASKS_TABLE", 1, CORE_2_INDEX, &BITS_CALC_MASKS_TABLE_2, 4, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_TX_INGRESS_DISPATCHER_CREDIT_TABLE", 1, CORE_2_INDEX, &CPU_TX_INGRESS_DISPATCHER_CREDIT_TABLE_2, 3, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_FPM_THRESHOLDS", 1, CORE_2_INDEX, &DHD_FPM_THRESHOLDS_2, 3, 1, 1 },
#endif
#if defined BCM63146
	{ "FPM_GLOBAL_CFG", 1, CORE_2_INDEX, &FPM_GLOBAL_CFG_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_TX_SYNC_FIFO_TABLE", 1, CORE_2_INDEX, &CPU_TX_SYNC_FIFO_TABLE_2, 2, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_RECYCLE_RING_DESCRIPTOR_TABLE", 1, CORE_2_INDEX, &CPU_RECYCLE_RING_DESCRIPTOR_TABLE_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_RECYCLE_RING_INDEX_DDR_ADDR_TABLE", 1, CORE_2_INDEX, &CPU_RECYCLE_RING_INDEX_DDR_ADDR_TABLE_2, 2, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_FEED_RING_DESCRIPTOR_TABLE", 1, CORE_2_INDEX, &CPU_FEED_RING_DESCRIPTOR_TABLE_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_RX_INTERRUPT_ID_DDR_ADDR", 1, CORE_2_INDEX, &CPU_RX_INTERRUPT_ID_DDR_ADDR_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "SYSTEM_CONFIGURATION", 1, CORE_2_INDEX, &SYSTEM_CONFIGURATION_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_RECYCLE_INTERRUPT_SCRATCH", 1, CORE_2_INDEX, &CPU_RECYCLE_INTERRUPT_SCRATCH_2, 2, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_RECYCLE_RING_INTERRUPT_COUNTER_TABLE", 1, CORE_2_INDEX, &CPU_RECYCLE_RING_INTERRUPT_COUNTER_TABLE_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "DEBUG_PRINT_TABLE", 1, CORE_2_INDEX, &DEBUG_PRINT_TABLE_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_TX_RING_INDICES_VALUES_TABLE", 1, CORE_2_INDEX, &CPU_TX_RING_INDICES_VALUES_TABLE_2, 2, 1, 1 },
#endif
#if defined BCM63146
	{ "TUNNELS_PARSING_CFG", 1, CORE_2_INDEX, &TUNNELS_PARSING_CFG_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "MULTICAST_KEY_MASK", 1, CORE_2_INDEX, &MULTICAST_KEY_MASK_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "IPTV_CFG_TABLE", 1, CORE_2_INDEX, &IPTV_CFG_TABLE_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "IPTV_DDR_CTX_TABLE_ADDRESS", 1, CORE_2_INDEX, &IPTV_DDR_CTX_TABLE_ADDRESS_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "NAT_CACHE_CFG", 1, CORE_2_INDEX, &NAT_CACHE_CFG_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_RECYCLE_SHADOW_RD_IDX", 1, CORE_2_INDEX, &CPU_RECYCLE_SHADOW_RD_IDX_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_RECYCLE_SHADOW_WR_IDX", 1, CORE_2_INDEX, &CPU_RECYCLE_SHADOW_WR_IDX_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_RECYCLE_NEXT_PTR_TABLE", 1, CORE_2_INDEX, &CPU_RECYCLE_NEXT_PTR_TABLE_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "NAT_CACHE_KEY0_MASK", 1, CORE_2_INDEX, &NAT_CACHE_KEY0_MASK_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "NATC_L2_VLAN_KEY_MASK", 1, CORE_2_INDEX, &NATC_L2_VLAN_KEY_MASK_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "INGRESS_FILTER_CFG", 1, CORE_2_INDEX, &INGRESS_FILTER_CFG_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "NATC_L2_TOS_MASK", 1, CORE_2_INDEX, &NATC_L2_TOS_MASK_2, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "FC_FLOW_IP_ADDRESSES_TABLE", 1, CORE_2_INDEX, &FC_FLOW_IP_ADDRESSES_TABLE_2, 4, 1, 1 },
#endif
#if defined BCM63146
	{ "RUNNER_PROFILING_TRACE_BUFFER", 1, CORE_2_INDEX, &RUNNER_PROFILING_TRACE_BUFFER_2, 128, 1, 1 },
#endif
#if defined BCM63146
	{ "TCPSPDTEST_STREAM_TABLE", 1, CORE_3_INDEX, &TCPSPDTEST_STREAM_TABLE_3, 4, 1, 1 },
#endif
#if defined BCM63146
	{ "VPORT_TX_FLOW_TABLE", 1, CORE_3_INDEX, &VPORT_TX_FLOW_TABLE_3, 64, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_RX_SCRATCHPAD", 1, CORE_3_INDEX, &CPU_RX_SCRATCHPAD_3, 128, 1, 1 },
#endif
#if defined BCM63146
	{ "PKTGEN_TX_STREAM_TABLE", 1, CORE_3_INDEX, &PKTGEN_TX_STREAM_TABLE_3, 4, 1, 1 },
#endif
#if defined BCM63146
	{ "FPM_POOL_NUMBER_MAPPING_TABLE", 1, CORE_3_INDEX, &FPM_POOL_NUMBER_MAPPING_TABLE_3, 32, 1, 1 },
#endif
#if defined BCM63146
	{ "GPE_COMMAND_PRIMITIVE_TABLE", 1, CORE_3_INDEX, &GPE_COMMAND_PRIMITIVE_TABLE_3, 32, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_FEED_RING_CACHE_TABLE", 1, CORE_3_INDEX, &CPU_FEED_RING_CACHE_TABLE_3, 16, 1, 1 },
#endif
#if defined BCM63146
	{ "DSL_TX_FLOW_TABLE", 1, CORE_3_INDEX, &DSL_TX_FLOW_TABLE_3, 256, 1, 1 },
#endif
#if defined BCM63146
	{ "LAYER2_GRE_TUNNEL_TABLE", 1, CORE_3_INDEX, &LAYER2_GRE_TUNNEL_TABLE_3, 12, 1, 1 },
#endif
#if defined BCM63146
	{ "DS_CPU_RX_METER_TABLE", 1, CORE_3_INDEX, &DS_CPU_RX_METER_TABLE_3, 16, 1, 1 },
#endif
#if defined BCM63146
	{ "VPORT_CFG_TABLE", 1, CORE_3_INDEX, &VPORT_CFG_TABLE_3, 40, 1, 1 },
#endif
#if defined BCM63146
	{ "DS_CPU_REASON_TO_METER_TABLE", 1, CORE_3_INDEX, &DS_CPU_REASON_TO_METER_TABLE_3, 64, 1, 1 },
#endif
#if defined BCM63146
	{ "US_CPU_RX_METER_TABLE", 1, CORE_3_INDEX, &US_CPU_RX_METER_TABLE_3, 16, 1, 1 },
#endif
#if defined BCM63146
	{ "IMG3_PACKET_BUFFER", 1, CORE_3_INDEX, &IMG3_PACKET_BUFFER_3, 8, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_RECYCLE_SRAM_PD_FIFO", 1, CORE_3_INDEX, &CPU_RECYCLE_SRAM_PD_FIFO_3, 16, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_RX_TASK_STACK", 1, CORE_3_INDEX, &CPU_RX_TASK_STACK_3, 256, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_RX_COPY_TASK_STACK", 1, CORE_3_INDEX, &CPU_RX_COPY_TASK_STACK_3, 256, 1, 1 },
#endif
#if defined BCM63146
	{ "RX_FLOW_TABLE", 1, CORE_3_INDEX, &RX_FLOW_TABLE_3, 90, 1, 1 },
#endif
#if defined BCM63146
	{ "MUTEX_TABLE", 1, CORE_3_INDEX, &MUTEX_TABLE_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "IPTV_CONFIGURATION_TABLE", 1, CORE_3_INDEX, &IPTV_CONFIGURATION_TABLE_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "US_CPU_REASON_TO_METER_TABLE", 1, CORE_3_INDEX, &US_CPU_REASON_TO_METER_TABLE_3, 64, 1, 1 },
#endif
#if defined BCM63146
	{ "CSO_CONTEXT_TABLE", 1, CORE_3_INDEX, &CSO_CONTEXT_TABLE_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "GENERAL_QUEUE_DYNAMIC_MNG_TABLE", 1, CORE_3_INDEX, &GENERAL_QUEUE_DYNAMIC_MNG_TABLE_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "VPORT_TO_LOOKUP_PORT_MAPPING_TABLE", 1, CORE_3_INDEX, &VPORT_TO_LOOKUP_PORT_MAPPING_TABLE_3, 32, 1, 1 },
#endif
#if defined BCM63146
	{ "IPV6_HOST_ADDRESS_CRC_TABLE", 1, CORE_3_INDEX, &IPV6_HOST_ADDRESS_CRC_TABLE_3, 16, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_RING_INTERRUPT_COUNTER_TABLE", 1, CORE_3_INDEX, &CPU_RING_INTERRUPT_COUNTER_TABLE_3, 18, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_RECYCLE_INTERRUPT_COALESCING_TABLE", 1, CORE_3_INDEX, &CPU_RECYCLE_INTERRUPT_COALESCING_TABLE_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_FEED_RING_RSV_TABLE", 1, CORE_3_INDEX, &CPU_FEED_RING_RSV_TABLE_3, 16, 1, 1 },
#endif
#if defined BCM63146
	{ "SPDSVC_GEN_PARAMS_TABLE", 1, CORE_3_INDEX, &SPDSVC_GEN_PARAMS_TABLE_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "PKTGEN_NO_SBPM_HDRS_CNTR", 1, CORE_3_INDEX, &PKTGEN_NO_SBPM_HDRS_CNTR_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_RX_COPY_PD_FIFO_TABLE", 1, CORE_3_INDEX, &CPU_RX_COPY_PD_FIFO_TABLE_3, 8, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_REASON_AND_VPORT_TO_METER_TABLE", 1, CORE_3_INDEX, &CPU_REASON_AND_VPORT_TO_METER_TABLE_3, 120, 1, 1 },
#endif
#if defined BCM63146
	{ "RX_FLOW_CONTEXT_DDR_ADDR", 1, CORE_3_INDEX, &RX_FLOW_CONTEXT_DDR_ADDR_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "INGRESS_FILTER_PROFILE_TABLE", 1, CORE_3_INDEX, &INGRESS_FILTER_PROFILE_TABLE_3, 16, 1, 1 },
#endif
#if defined BCM63146
	{ "COMMON_REPROCESSING_PD_FIFO_TABLE", 1, CORE_3_INDEX, &COMMON_REPROCESSING_PD_FIFO_TABLE_3, 4, 1, 1 },
#endif
#if defined BCM63146
	{ "LOOPBACK_QUEUE_TABLE", 1, CORE_3_INDEX, &LOOPBACK_QUEUE_TABLE_3, 40, 1, 1 },
#endif
#if defined BCM63146
	{ "NULL_BUFFER", 1, CORE_3_INDEX, &NULL_BUFFER_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_RX_COPY_INT_SCRATCHPAD", 1, CORE_3_INDEX, &CPU_RX_COPY_INT_SCRATCHPAD_3, 4, 1, 1 },
#endif
#if defined BCM63146
	{ "VPORT_CFG_EX_TABLE", 1, CORE_3_INDEX, &VPORT_CFG_EX_TABLE_3, 31, 1, 1 },
#endif
#if defined BCM63146
	{ "SPDTEST_GEN_PARAM", 1, CORE_3_INDEX, &SPDTEST_GEN_PARAM_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "NATC_COUNTERS_GENERIC_CONTEXT", 1, CORE_3_INDEX, &NATC_COUNTERS_GENERIC_CONTEXT_3, 128, 1, 1 },
#endif
#if defined BCM63146
	{ "NATC_TBL_CFG", 1, CORE_3_INDEX, &NATC_TBL_CFG_3, 4, 1, 1 },
#endif
#if defined BCM63146
	{ "REGISTERS_BUFFER", 1, CORE_3_INDEX, &REGISTERS_BUFFER_3, 32, 1, 1 },
#endif
#if defined BCM63146
	{ "IPV4_HOST_ADDRESS_TABLE", 1, CORE_3_INDEX, &IPV4_HOST_ADDRESS_TABLE_3, 8, 1, 1 },
#endif
#if defined BCM63146
	{ "PKTGEN_SBPM_HDR_BNS", 1, CORE_3_INDEX, &PKTGEN_SBPM_HDR_BNS_3, 28, 1, 1 },
#endif
#if defined BCM63146
	{ "TX_ABS_RECYCLE_COUNTERS", 1, CORE_3_INDEX, &TX_ABS_RECYCLE_COUNTERS_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "QUEUE_THRESHOLD_VECTOR", 1, CORE_3_INDEX, &QUEUE_THRESHOLD_VECTOR_3, 9, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_RX_INTERRUPT_SCRATCH", 1, CORE_3_INDEX, &CPU_RX_INTERRUPT_SCRATCH_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_RECYCLE_INTERRUPT_ID_DDR_ADDR", 1, CORE_3_INDEX, &CPU_RECYCLE_INTERRUPT_ID_DDR_ADDR_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_RX_PSRAM_GET_NEXT_SCRATCHPAD", 1, CORE_3_INDEX, &CPU_RX_PSRAM_GET_NEXT_SCRATCHPAD_3, 4, 1, 1 },
#endif
#if defined BCM63146
	{ "UDPSPDTEST_STREAM_RX_STAT_TABLE", 1, CORE_3_INDEX, &UDPSPDTEST_STREAM_RX_STAT_TABLE_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "SPDSVC_ANALYZER_PARAMS_TABLE", 1, CORE_3_INDEX, &SPDSVC_ANALYZER_PARAMS_TABLE_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "FLOW_BASED_ACTION_PTR_TABLE", 1, CORE_3_INDEX, &FLOW_BASED_ACTION_PTR_TABLE_3, 17, 1, 1 },
#endif
#if defined BCM63146
	{ "PKTGEN_CURR_SBPM_HDR_PTR", 1, CORE_3_INDEX, &PKTGEN_CURR_SBPM_HDR_PTR_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "PKTGEN_NUM_OF_AVAIL_SBPM_HDRS", 1, CORE_3_INDEX, &PKTGEN_NUM_OF_AVAIL_SBPM_HDRS_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "PKTGEN_SBPM_END_PTR", 1, CORE_3_INDEX, &PKTGEN_SBPM_END_PTR_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_RX_INTERRUPT_ID_DDR_ADDR", 1, CORE_3_INDEX, &CPU_RX_INTERRUPT_ID_DDR_ADDR_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "INGRESS_FILTER_L2_REASON_TABLE", 1, CORE_3_INDEX, &INGRESS_FILTER_L2_REASON_TABLE_3, 16, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_VPORT_TO_METER_TABLE", 1, CORE_3_INDEX, &CPU_VPORT_TO_METER_TABLE_3, 40, 1, 1 },
#endif
#if defined BCM63146
	{ "SYSTEM_CONFIGURATION", 1, CORE_3_INDEX, &SYSTEM_CONFIGURATION_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "FORCE_DSCP", 1, CORE_3_INDEX, &FORCE_DSCP_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CORE_ID_TABLE", 1, CORE_3_INDEX, &CORE_ID_TABLE_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "SRAM_DUMMY_STORE", 1, CORE_3_INDEX, &SRAM_DUMMY_STORE_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_RX_COPY_DISPATCHER_CREDIT_TABLE", 1, CORE_3_INDEX, &CPU_RX_COPY_DISPATCHER_CREDIT_TABLE_3, 3, 1, 1 },
#endif
#if defined BCM63146
	{ "PKTGEN_BAD_GET_NEXT", 1, CORE_3_INDEX, &PKTGEN_BAD_GET_NEXT_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "VLAN_ACTION_GPE_HANDLER_PTR_TABLE", 1, CORE_3_INDEX, &VLAN_ACTION_GPE_HANDLER_PTR_TABLE_3, 17, 1, 1 },
#endif
#if defined BCM63146
	{ "SPDSVC_TCPSPDTEST_COMMON_TABLE", 1, CORE_3_INDEX, &SPDSVC_TCPSPDTEST_COMMON_TABLE_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "PKTGEN_MAX_UT_PKTS", 1, CORE_3_INDEX, &PKTGEN_MAX_UT_PKTS_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_RECYCLE_RING_INTERRUPT_COUNTER_TABLE", 1, CORE_3_INDEX, &CPU_RECYCLE_RING_INTERRUPT_COUNTER_TABLE_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "SPDSVC_GEN_DISPATCHER_CREDIT_TABLE", 1, CORE_3_INDEX, &SPDSVC_GEN_DISPATCHER_CREDIT_TABLE_3, 3, 1, 1 },
#endif
#if defined BCM63146
	{ "PKTGEN_UT_TRIGGER", 1, CORE_3_INDEX, &PKTGEN_UT_TRIGGER_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "UDPSPDTEST_STREAM_TX_STAT_TABLE", 1, CORE_3_INDEX, &UDPSPDTEST_STREAM_TX_STAT_TABLE_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "TASK_IDX", 1, CORE_3_INDEX, &TASK_IDX_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_FEED_RING_INDEX_DDR_ADDR_TABLE", 1, CORE_3_INDEX, &CPU_FEED_RING_INDEX_DDR_ADDR_TABLE_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "COMMON_REPROCESSING_DISPATCHER_CREDIT_TABLE", 1, CORE_3_INDEX, &COMMON_REPROCESSING_DISPATCHER_CREDIT_TABLE_3, 3, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_FEED_RING_INTERRUPT_THRESHOLD", 1, CORE_3_INDEX, &CPU_FEED_RING_INTERRUPT_THRESHOLD_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_FEED_RING_INTERRUPT_COUNTER", 1, CORE_3_INDEX, &CPU_FEED_RING_INTERRUPT_COUNTER_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "UPDATE_FIFO_TABLE", 1, CORE_3_INDEX, &UPDATE_FIFO_TABLE_3, 8, 1, 1 },
#endif
#if defined BCM63146
	{ "PKTGEN_FPM_UG_MGMT", 1, CORE_3_INDEX, &PKTGEN_FPM_UG_MGMT_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "RX_MIRRORING_CONFIGURATION", 1, CORE_3_INDEX, &RX_MIRRORING_CONFIGURATION_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "INGRESS_QOS_DONT_DROP_RATIO", 1, CORE_3_INDEX, &INGRESS_QOS_DONT_DROP_RATIO_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_TX_RECYCLE_RING_ID", 1, CORE_3_INDEX, &CPU_TX_RECYCLE_RING_ID_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "TUNNELS_PARSING_CFG", 1, CORE_3_INDEX, &TUNNELS_PARSING_CFG_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_RX_COPY_UPDATE_FIFO_TABLE", 1, CORE_3_INDEX, &CPU_RX_COPY_UPDATE_FIFO_TABLE_3, 8, 1, 1 },
#endif
#if defined BCM63146
	{ "TCPSPDTEST_ENGINE_GLOBAL_TABLE", 1, CORE_3_INDEX, &TCPSPDTEST_ENGINE_GLOBAL_TABLE_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "MULTI_FLOW_FLAG_TABLE", 1, CORE_3_INDEX, &MULTI_FLOW_FLAG_TABLE_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_FEED_RING_CACHE_OFFSET", 1, CORE_3_INDEX, &CPU_FEED_RING_CACHE_OFFSET_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_REDIRECT_MODE", 1, CORE_3_INDEX, &CPU_REDIRECT_MODE_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CSO_DISABLE", 1, CORE_3_INDEX, &CSO_DISABLE_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "IPTV_CFG_TABLE", 1, CORE_3_INDEX, &IPTV_CFG_TABLE_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "COMMON_REPROCESSING_UPDATE_FIFO_TABLE", 1, CORE_3_INDEX, &COMMON_REPROCESSING_UPDATE_FIFO_TABLE_3, 8, 1, 1 },
#endif
#if defined BCM63146
	{ "TCAM_IC_CFG_TABLE", 1, CORE_3_INDEX, &TCAM_IC_CFG_TABLE_3, 2, 1, 1 },
#endif
#if defined BCM63146
	{ "PD_FIFO_TABLE", 1, CORE_3_INDEX, &PD_FIFO_TABLE_3, 2, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_REASON_TO_TC", 1, CORE_3_INDEX, &CPU_REASON_TO_TC_3, 64, 1, 1 },
#endif
#if defined BCM63146
	{ "TC_TO_CPU_RXQ", 1, CORE_3_INDEX, &TC_TO_CPU_RXQ_3, 64, 1, 1 },
#endif
#if defined BCM63146
	{ "EXC_TC_TO_CPU_RXQ", 1, CORE_3_INDEX, &EXC_TC_TO_CPU_RXQ_3, 64, 1, 1 },
#endif
#if defined BCM63146
	{ "BCM_SPDSVC_STREAM_RX_TS_TABLE", 1, CORE_3_INDEX, &BCM_SPDSVC_STREAM_RX_TS_TABLE_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "DEBUG_PRINT_CORE_LOCK", 1, CORE_3_INDEX, &DEBUG_PRINT_CORE_LOCK_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "IPTV_CLASSIFICATION_CFG_TABLE", 1, CORE_3_INDEX, &IPTV_CLASSIFICATION_CFG_TABLE_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "INGRESS_FILTER_1588_CFG", 1, CORE_3_INDEX, &INGRESS_FILTER_1588_CFG_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "RX_MIRRORING_DIRECT_ENABLE", 1, CORE_3_INDEX, &RX_MIRRORING_DIRECT_ENABLE_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_FPM_THRESHOLDS", 1, CORE_3_INDEX, &DHD_FPM_THRESHOLDS_3, 3, 1, 1 },
#endif
#if defined BCM63146
	{ "PKTGEN_SESSION_DATA", 1, CORE_3_INDEX, &PKTGEN_SESSION_DATA_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "DEBUG_SCRATCHPAD", 1, CORE_3_INDEX, &DEBUG_SCRATCHPAD_3, 12, 1, 1 },
#endif
#if defined BCM63146
	{ "IPTV_DDR_CTX_TABLE_ADDRESS", 1, CORE_3_INDEX, &IPTV_DDR_CTX_TABLE_ADDRESS_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "FPM_GLOBAL_CFG", 1, CORE_3_INDEX, &FPM_GLOBAL_CFG_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_RECYCLE_RING_DESCRIPTOR_TABLE", 1, CORE_3_INDEX, &CPU_RECYCLE_RING_DESCRIPTOR_TABLE_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_RECYCLE_RING_INDEX_DDR_ADDR_TABLE", 1, CORE_3_INDEX, &CPU_RECYCLE_RING_INDEX_DDR_ADDR_TABLE_3, 2, 1, 1 },
#endif
#if defined BCM63146
	{ "VPORT_TO_CPU_OBJ", 1, CORE_3_INDEX, &VPORT_TO_CPU_OBJ_3, 40, 1, 1 },
#endif
#if defined BCM63146
	{ "NAT_CACHE_CFG", 1, CORE_3_INDEX, &NAT_CACHE_CFG_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_INTERRUPT_COALESCING_TABLE", 1, CORE_3_INDEX, &CPU_INTERRUPT_COALESCING_TABLE_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "BITS_CALC_MASKS_TABLE", 1, CORE_3_INDEX, &BITS_CALC_MASKS_TABLE_3, 4, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_FEED_RING_DESCRIPTOR_TABLE", 1, CORE_3_INDEX, &CPU_FEED_RING_DESCRIPTOR_TABLE_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_RECYCLE_INTERRUPT_SCRATCH", 1, CORE_3_INDEX, &CPU_RECYCLE_INTERRUPT_SCRATCH_3, 2, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_RX_LOCAL_SCRATCH", 1, CORE_3_INDEX, &CPU_RX_LOCAL_SCRATCH_3, 2, 1, 1 },
#endif
#if defined BCM63146
	{ "PKTGEN_SBPM_EXTS", 1, CORE_3_INDEX, &PKTGEN_SBPM_EXTS_3, 4, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_RECYCLE_SHADOW_RD_IDX", 1, CORE_3_INDEX, &CPU_RECYCLE_SHADOW_RD_IDX_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "PKTGEN_BBMSG_REPLY_SCRATCH", 1, CORE_3_INDEX, &PKTGEN_BBMSG_REPLY_SCRATCH_3, 2, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_RECYCLE_SHADOW_WR_IDX", 1, CORE_3_INDEX, &CPU_RECYCLE_SHADOW_WR_IDX_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "DEBUG_PRINT_TABLE", 1, CORE_3_INDEX, &DEBUG_PRINT_TABLE_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_RECYCLE_NEXT_PTR_TABLE", 1, CORE_3_INDEX, &CPU_RECYCLE_NEXT_PTR_TABLE_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "NAT_CACHE_KEY0_MASK", 1, CORE_3_INDEX, &NAT_CACHE_KEY0_MASK_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "MULTICAST_KEY_MASK", 1, CORE_3_INDEX, &MULTICAST_KEY_MASK_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "NATC_L2_VLAN_KEY_MASK", 1, CORE_3_INDEX, &NATC_L2_VLAN_KEY_MASK_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "INGRESS_FILTER_CFG", 1, CORE_3_INDEX, &INGRESS_FILTER_CFG_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "NATC_L2_TOS_MASK", 1, CORE_3_INDEX, &NATC_L2_TOS_MASK_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "PKTGEN_TX_STREAM_SCRATCH_TABLE", 1, CORE_3_INDEX, &PKTGEN_TX_STREAM_SCRATCH_TABLE_3, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_RING_DESCRIPTORS_TABLE", 1, CORE_3_INDEX, &CPU_RING_DESCRIPTORS_TABLE_3, 16, 1, 1 },
#endif
#if defined BCM63146
	{ "FC_FLOW_IP_ADDRESSES_TABLE", 1, CORE_3_INDEX, &FC_FLOW_IP_ADDRESSES_TABLE_3, 4, 1, 1 },
#endif
#if defined BCM63146
	{ "RUNNER_PROFILING_TRACE_BUFFER", 1, CORE_3_INDEX, &RUNNER_PROFILING_TRACE_BUFFER_3, 128, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_FLOW_RING_CACHE_CTX_TABLE", 1, CORE_4_INDEX, &DHD_FLOW_RING_CACHE_CTX_TABLE_4, 48, 1, 1 },
#endif
#if defined BCM63146
	{ "DSL_TX_FLOW_TABLE", 1, CORE_4_INDEX, &DSL_TX_FLOW_TABLE_4, 256, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_AUX_INFO_CACHE_TABLE", 1, CORE_4_INDEX, &DHD_AUX_INFO_CACHE_TABLE_4, 48, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_TIMER_TASK_STACK", 1, CORE_4_INDEX, &DHD_TIMER_TASK_STACK_4, 256, 1, 1 },
#endif
#if defined BCM63146
	{ "WLAN_MCAST_DHD_STATION_TABLE", 1, CORE_4_INDEX, &WLAN_MCAST_DHD_STATION_TABLE_4, 64, 1, 1 },
#endif
#if defined BCM63146
	{ "WLAN_MCAST_COPY_SCRATCHPAD", 1, CORE_4_INDEX, &WLAN_MCAST_COPY_SCRATCHPAD_4, 64, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_POST_COMMON_RADIO_DATA", 1, CORE_4_INDEX, &DHD_POST_COMMON_RADIO_DATA_4, 3, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_FPM_POOL_NUMBER_MAPPING_TABLE", 1, CORE_4_INDEX, &DHD_FPM_POOL_NUMBER_MAPPING_TABLE_4, 16, 1, 1 },
#endif
#if defined BCM63146
	{ "VPORT_TX_FLOW_TABLE", 1, CORE_4_INDEX, &VPORT_TX_FLOW_TABLE_4, 64, 1, 1 },
#endif
#if defined BCM63146
	{ "LAYER2_GRE_TUNNEL_TABLE", 1, CORE_4_INDEX, &LAYER2_GRE_TUNNEL_TABLE_4, 12, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_BACKUP_INDEX_CACHE", 1, CORE_4_INDEX, &DHD_BACKUP_INDEX_CACHE_4, 3, 1, 1 },
#endif
#if defined BCM63146
	{ "FPM_POOL_NUMBER_MAPPING_TABLE", 1, CORE_4_INDEX, &FPM_POOL_NUMBER_MAPPING_TABLE_4, 32, 1, 1 },
#endif
#if defined BCM63146
	{ "IMG4_PACKET_BUFFER", 1, CORE_4_INDEX, &IMG4_PACKET_BUFFER_4, 8, 1, 1 },
#endif
#if defined BCM63146
	{ "WLAN_MCAST_SSID_MAC_ADDRESS_TABLE", 1, CORE_4_INDEX, &WLAN_MCAST_SSID_MAC_ADDRESS_TABLE_4, 48, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_L2_HEADER", 1, CORE_4_INDEX, &DHD_L2_HEADER_4, 72, 1, 1 },
#endif
#if defined BCM63146
	{ "IPTV_CONFIGURATION_TABLE", 1, CORE_4_INDEX, &IPTV_CONFIGURATION_TABLE_4, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_HW_CFG", 1, CORE_4_INDEX, &DHD_HW_CFG_4, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "VPORT_TO_LOOKUP_PORT_MAPPING_TABLE", 1, CORE_4_INDEX, &VPORT_TO_LOOKUP_PORT_MAPPING_TABLE_4, 32, 1, 1 },
#endif
#if defined BCM63146
	{ "VPORT_CFG_TABLE", 1, CORE_4_INDEX, &VPORT_CFG_TABLE_4, 40, 1, 1 },
#endif
#if defined BCM63146
	{ "GPE_COMMAND_PRIMITIVE_TABLE", 1, CORE_4_INDEX, &GPE_COMMAND_PRIMITIVE_TABLE_4, 32, 1, 1 },
#endif
#if defined BCM63146
	{ "NATC_COUNTERS_GENERIC_CONTEXT", 1, CORE_4_INDEX, &NATC_COUNTERS_GENERIC_CONTEXT_4, 128, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_TX_POST_TASK_0_STACK", 1, CORE_4_INDEX, &DHD_TX_POST_TASK_0_STACK_4, 256, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_TX_POST_TASK_1_STACK", 1, CORE_4_INDEX, &DHD_TX_POST_TASK_1_STACK_4, 256, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_TX_POST_TASK_2_STACK", 1, CORE_4_INDEX, &DHD_TX_POST_TASK_2_STACK_4, 256, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_TX_POST_UPDATE_FIFO_TASK_STACK", 1, CORE_4_INDEX, &DHD_TX_POST_UPDATE_FIFO_TASK_STACK_4, 256, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_TX_POST_PD_FIFO_TABLE", 1, CORE_4_INDEX, &DHD_TX_POST_PD_FIFO_TABLE_4, 12, 1, 1 },
#endif
#if defined BCM63146
	{ "WLAN_MCAST_SSID_STATS_TABLE", 1, CORE_4_INDEX, &WLAN_MCAST_SSID_STATS_TABLE_4, 48, 1, 1 },
#endif
#if defined BCM63146
	{ "REGISTERS_BUFFER", 1, CORE_4_INDEX, &REGISTERS_BUFFER_4, 32, 1, 1 },
#endif
#if defined BCM63146
	{ "WLAN_MCAST_DHD_STATION_CTX_TABLE", 1, CORE_4_INDEX, &WLAN_MCAST_DHD_STATION_CTX_TABLE_4, 64, 1, 1 },
#endif
#if defined BCM63146
	{ "RX_FLOW_TABLE", 1, CORE_4_INDEX, &RX_FLOW_TABLE_4, 90, 1, 1 },
#endif
#if defined BCM63146
	{ "MUTEX_TABLE", 1, CORE_4_INDEX, &MUTEX_TABLE_4, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "GENERAL_QUEUE_DYNAMIC_MNG_TABLE", 1, CORE_4_INDEX, &GENERAL_QUEUE_DYNAMIC_MNG_TABLE_4, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "WLAN_MCAST_DFT_LIST_ENTRY_SCRATCH", 1, CORE_4_INDEX, &WLAN_MCAST_DFT_LIST_ENTRY_SCRATCH_4, 64, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_TX_POST_FLOW_RING_BUFFER", 1, CORE_4_INDEX, &DHD_TX_POST_FLOW_RING_BUFFER_4, 3, 1, 1 },
#endif
#if defined BCM63146
	{ "DEBUG_SCRATCHPAD", 1, CORE_4_INDEX, &DEBUG_SCRATCHPAD_4, 12, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_CODEL_BIAS_SLOPE_TABLE", 1, CORE_4_INDEX, &DHD_CODEL_BIAS_SLOPE_TABLE_4, 11, 1, 1 },
#endif
#if defined BCM63146
	{ "SPDTEST_GEN_PARAM", 1, CORE_4_INDEX, &SPDTEST_GEN_PARAM_4, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "SPDSVC_ANALYZER_PARAMS_TABLE", 1, CORE_4_INDEX, &SPDSVC_ANALYZER_PARAMS_TABLE_4, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "INGRESS_FILTER_PROFILE_TABLE", 1, CORE_4_INDEX, &INGRESS_FILTER_PROFILE_TABLE_4, 16, 1, 1 },
#endif
#if defined BCM63146
	{ "WLAN_MCAST_DFT_LIST_SIZE", 1, CORE_4_INDEX, &WLAN_MCAST_DFT_LIST_SIZE_4, 64, 1, 1 },
#endif
#if defined BCM63146
	{ "LOOPBACK_QUEUE_TABLE", 1, CORE_4_INDEX, &LOOPBACK_QUEUE_TABLE_4, 40, 1, 1 },
#endif
#if defined BCM63146
	{ "RX_FLOW_CONTEXT_DDR_ADDR", 1, CORE_4_INDEX, &RX_FLOW_CONTEXT_DDR_ADDR_4, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "INGRESS_FILTER_L2_REASON_TABLE", 1, CORE_4_INDEX, &INGRESS_FILTER_L2_REASON_TABLE_4, 16, 1, 1 },
#endif
#if defined BCM63146
	{ "VPORT_CFG_EX_TABLE", 1, CORE_4_INDEX, &VPORT_CFG_EX_TABLE_4, 31, 1, 1 },
#endif
#if defined BCM63146
	{ "TASK_IDX", 1, CORE_4_INDEX, &TASK_IDX_4, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_CPU_RECYCLE_RING_DESCRIPTOR_TABLE", 1, CORE_4_INDEX, &DHD_CPU_RECYCLE_RING_DESCRIPTOR_TABLE_4, 3, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_FPM_THRESHOLDS", 1, CORE_4_INDEX, &DHD_FPM_THRESHOLDS_4, 3, 1, 1 },
#endif
#if defined BCM63146
	{ "SPDSVC_WLAN_TXPOST_PARAMS_TABLE", 1, CORE_4_INDEX, &SPDSVC_WLAN_TXPOST_PARAMS_TABLE_4, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "FORCE_DSCP", 1, CORE_4_INDEX, &FORCE_DSCP_4, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CORE_ID_TABLE", 1, CORE_4_INDEX, &CORE_ID_TABLE_4, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "WLAN_MCAST_SCRATCHPAD", 1, CORE_4_INDEX, &WLAN_MCAST_SCRATCHPAD_4, 5, 1, 1 },
#endif
#if defined BCM63146
	{ "NULL_BUFFER", 1, CORE_4_INDEX, &NULL_BUFFER_4, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_CPU_INT_ID", 1, CORE_4_INDEX, &DHD_CPU_INT_ID_4, 3, 1, 1 },
#endif
#if defined BCM63146
	{ "RX_MIRRORING_CONFIGURATION", 1, CORE_4_INDEX, &RX_MIRRORING_CONFIGURATION_4, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "SRAM_DUMMY_STORE", 1, CORE_4_INDEX, &SRAM_DUMMY_STORE_4, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "INGRESS_QOS_DONT_DROP_RATIO", 1, CORE_4_INDEX, &INGRESS_QOS_DONT_DROP_RATIO_4, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "NATC_TBL_CFG", 1, CORE_4_INDEX, &NATC_TBL_CFG_4, 4, 1, 1 },
#endif
#if defined BCM63146
	{ "TCAM_IC_CFG_TABLE", 1, CORE_4_INDEX, &TCAM_IC_CFG_TABLE_4, 2, 1, 1 },
#endif
#if defined BCM63146
	{ "QUEUE_THRESHOLD_VECTOR", 1, CORE_4_INDEX, &QUEUE_THRESHOLD_VECTOR_4, 9, 1, 1 },
#endif
#if defined BCM63146
	{ "MULTI_FLOW_FLAG_TABLE", 1, CORE_4_INDEX, &MULTI_FLOW_FLAG_TABLE_4, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_REDIRECT_MODE", 1, CORE_4_INDEX, &CPU_REDIRECT_MODE_4, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "DEBUG_PRINT_CORE_LOCK", 1, CORE_4_INDEX, &DEBUG_PRINT_CORE_LOCK_4, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "IPTV_CLASSIFICATION_CFG_TABLE", 1, CORE_4_INDEX, &IPTV_CLASSIFICATION_CFG_TABLE_4, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "MIRRORING_SCRATCH", 1, CORE_4_INDEX, &MIRRORING_SCRATCH_4, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_MCAST_DISPATCHER_CREDIT_TABLE", 1, CORE_4_INDEX, &DHD_MCAST_DISPATCHER_CREDIT_TABLE_4, 3, 1, 1 },
#endif
#if defined BCM63146
	{ "INGRESS_FILTER_1588_CFG", 1, CORE_4_INDEX, &INGRESS_FILTER_1588_CFG_4, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "RX_MIRRORING_DIRECT_ENABLE", 1, CORE_4_INDEX, &RX_MIRRORING_DIRECT_ENABLE_4, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "FLOW_BASED_ACTION_PTR_TABLE", 1, CORE_4_INDEX, &FLOW_BASED_ACTION_PTR_TABLE_4, 17, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_CPU_RECYCLE_INTERRUPT_ID_DDR_ADDR", 1, CORE_4_INDEX, &DHD_CPU_RECYCLE_INTERRUPT_ID_DDR_ADDR_4, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_MIRRORING_DISPATCHER_CREDIT_TABLE", 1, CORE_4_INDEX, &DHD_MIRRORING_DISPATCHER_CREDIT_TABLE_4, 3, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_FLOW_RING_CACHE_LKP_TABLE", 1, CORE_4_INDEX, &DHD_FLOW_RING_CACHE_LKP_TABLE_4, 48, 1, 1 },
#endif
#if defined BCM63146
	{ "FPM_GLOBAL_CFG", 1, CORE_4_INDEX, &FPM_GLOBAL_CFG_4, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "CPU_FEED_RING_DESCRIPTOR_TABLE", 1, CORE_4_INDEX, &CPU_FEED_RING_DESCRIPTOR_TABLE_4, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "VLAN_ACTION_GPE_HANDLER_PTR_TABLE", 1, CORE_4_INDEX, &VLAN_ACTION_GPE_HANDLER_PTR_TABLE_4, 17, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_DOORBELL_TX_POST_VALUE", 1, CORE_4_INDEX, &DHD_DOORBELL_TX_POST_VALUE_4, 17, 1, 1 },
#endif
#if defined BCM63146
	{ "DEBUG_PRINT_TABLE", 1, CORE_4_INDEX, &DEBUG_PRINT_TABLE_4, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_TX_POST_UPDATE_FIFO_TABLE", 1, CORE_4_INDEX, &DHD_TX_POST_UPDATE_FIFO_TABLE_4, 8, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_BACKUP_BASE_ADDR", 1, CORE_4_INDEX, &DHD_BACKUP_BASE_ADDR_4, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "WLAN_MCAST_DFT_ADDR", 1, CORE_4_INDEX, &WLAN_MCAST_DFT_ADDR_4, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "SYSTEM_CONFIGURATION", 1, CORE_4_INDEX, &SYSTEM_CONFIGURATION_4, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "TUNNELS_PARSING_CFG", 1, CORE_4_INDEX, &TUNNELS_PARSING_CFG_4, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_MCAST_UPDATE_FIFO_TABLE", 1, CORE_4_INDEX, &DHD_MCAST_UPDATE_FIFO_TABLE_4, 8, 1, 1 },
#endif
#if defined BCM63146
	{ "MULTICAST_KEY_MASK", 1, CORE_4_INDEX, &MULTICAST_KEY_MASK_4, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "IPTV_CFG_TABLE", 1, CORE_4_INDEX, &IPTV_CFG_TABLE_4, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "IPTV_DDR_CTX_TABLE_ADDRESS", 1, CORE_4_INDEX, &IPTV_DDR_CTX_TABLE_ADDRESS_4, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "NAT_CACHE_CFG", 1, CORE_4_INDEX, &NAT_CACHE_CFG_4, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "DHD_MCAST_PD_FIFO_TABLE", 1, CORE_4_INDEX, &DHD_MCAST_PD_FIFO_TABLE_4, 2, 1, 1 },
#endif
#if defined BCM63146
	{ "NAT_CACHE_KEY0_MASK", 1, CORE_4_INDEX, &NAT_CACHE_KEY0_MASK_4, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "NATC_L2_VLAN_KEY_MASK", 1, CORE_4_INDEX, &NATC_L2_VLAN_KEY_MASK_4, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "INGRESS_FILTER_CFG", 1, CORE_4_INDEX, &INGRESS_FILTER_CFG_4, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "NATC_L2_TOS_MASK", 1, CORE_4_INDEX, &NATC_L2_TOS_MASK_4, 1, 1, 1 },
#endif
#if defined BCM63146
	{ "BITS_CALC_MASKS_TABLE", 1, CORE_4_INDEX, &BITS_CALC_MASKS_TABLE_4, 4, 1, 1 },
#endif
#if defined BCM63146
	{ "FC_FLOW_IP_ADDRESSES_TABLE", 1, CORE_4_INDEX, &FC_FLOW_IP_ADDRESSES_TABLE_4, 4, 1, 1 },
#endif
#if defined BCM63146
	{ "RUNNER_PROFILING_TRACE_BUFFER", 1, CORE_4_INDEX, &RUNNER_PROFILING_TRACE_BUFFER_4, 128, 1, 1 },
#endif
};
