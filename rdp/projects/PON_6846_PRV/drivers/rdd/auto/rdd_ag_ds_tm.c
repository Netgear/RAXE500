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


#include "rdd.h"

#include "rdd_ag_ds_tm.h"

int rdd_ag_ds_tm_codel_drop_descriptor_get(rdd_codel_drop_descriptor_t *codel_drop_descriptor)
{
    RDD_CODEL_DROP_DESCRIPTOR_MAX_SEQ_DROPS_READ_G(codel_drop_descriptor->max_seq_drops, RDD_DS_TM_CODEL_DROP_DESCRIPTOR_ADDRESS_ARR, 0);
    RDD_CODEL_DROP_DESCRIPTOR_FLUSH_TASK_WAKEUP_VALUE_READ_G(codel_drop_descriptor->flush_task_wakeup_value, RDD_DS_TM_CODEL_DROP_DESCRIPTOR_ADDRESS_ARR, 0);
    RDD_CODEL_DROP_DESCRIPTOR_FLUSH_CFG_PTR_READ_G(codel_drop_descriptor->flush_cfg_ptr, RDD_DS_TM_CODEL_DROP_DESCRIPTOR_ADDRESS_ARR, 0);
    RDD_CODEL_DROP_DESCRIPTOR_FLUSH_ENABLE_PTR_READ_G(codel_drop_descriptor->flush_enable_ptr, RDD_DS_TM_CODEL_DROP_DESCRIPTOR_ADDRESS_ARR, 0);
    RDD_CODEL_DROP_DESCRIPTOR_FLUSH_ENABLE_READ_G(codel_drop_descriptor->flush_enable, RDD_DS_TM_CODEL_DROP_DESCRIPTOR_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_codel_drop_descriptor_set(rdd_codel_drop_descriptor_t *codel_drop_descriptor)
{
    RDD_CODEL_DROP_DESCRIPTOR_MAX_SEQ_DROPS_WRITE_G(codel_drop_descriptor->max_seq_drops, RDD_DS_TM_CODEL_DROP_DESCRIPTOR_ADDRESS_ARR, 0);
    RDD_CODEL_DROP_DESCRIPTOR_FLUSH_TASK_WAKEUP_VALUE_WRITE_G(codel_drop_descriptor->flush_task_wakeup_value, RDD_DS_TM_CODEL_DROP_DESCRIPTOR_ADDRESS_ARR, 0);
    RDD_CODEL_DROP_DESCRIPTOR_FLUSH_CFG_PTR_WRITE_G(codel_drop_descriptor->flush_cfg_ptr, RDD_DS_TM_CODEL_DROP_DESCRIPTOR_ADDRESS_ARR, 0);
    RDD_CODEL_DROP_DESCRIPTOR_FLUSH_ENABLE_PTR_WRITE_G(codel_drop_descriptor->flush_enable_ptr, RDD_DS_TM_CODEL_DROP_DESCRIPTOR_ADDRESS_ARR, 0);
    RDD_CODEL_DROP_DESCRIPTOR_FLUSH_ENABLE_WRITE_G(codel_drop_descriptor->flush_enable, RDD_DS_TM_CODEL_DROP_DESCRIPTOR_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_scheduling_queue_descriptor_get(uint32_t _entry, rdd_scheduling_queue_descriptor_t *scheduling_queue_descriptor)
{
    if(!scheduling_queue_descriptor || _entry >= RDD_DS_TM_SCHEDULING_QUEUE_TABLE_SIZE)
         return BDMF_ERR_PARM;

    RDD_SCHEDULING_QUEUE_DESCRIPTOR_BBH_QUEUE_INDEX_READ_G(scheduling_queue_descriptor->bbh_queue_index, RDD_DS_TM_SCHEDULING_QUEUE_TABLE_ADDRESS_ARR, _entry);
    RDD_SCHEDULING_QUEUE_DESCRIPTOR_BLOCK_TYPE_READ_G(scheduling_queue_descriptor->block_type, RDD_DS_TM_SCHEDULING_QUEUE_TABLE_ADDRESS_ARR, _entry);
    RDD_SCHEDULING_QUEUE_DESCRIPTOR_CODEL_ENABLE_READ_G(scheduling_queue_descriptor->codel_enable, RDD_DS_TM_SCHEDULING_QUEUE_TABLE_ADDRESS_ARR, _entry);
    RDD_SCHEDULING_QUEUE_DESCRIPTOR_CODEL_DROPPING_READ_G(scheduling_queue_descriptor->codel_dropping, RDD_DS_TM_SCHEDULING_QUEUE_TABLE_ADDRESS_ARR, _entry);
    RDD_SCHEDULING_QUEUE_DESCRIPTOR_SCHEDULER_INDEX_READ_G(scheduling_queue_descriptor->scheduler_index, RDD_DS_TM_SCHEDULING_QUEUE_TABLE_ADDRESS_ARR, _entry);
    RDD_SCHEDULING_QUEUE_DESCRIPTOR_QUEUE_BIT_MASK_READ_G(scheduling_queue_descriptor->queue_bit_mask, RDD_DS_TM_SCHEDULING_QUEUE_TABLE_ADDRESS_ARR, _entry);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_scheduling_queue_descriptor_set(uint32_t _entry, rdd_scheduling_queue_descriptor_t *scheduling_queue_descriptor)
{
    if(!scheduling_queue_descriptor || _entry >= RDD_DS_TM_SCHEDULING_QUEUE_TABLE_SIZE || scheduling_queue_descriptor->bbh_queue_index >= 64 || scheduling_queue_descriptor->scheduler_index >= 128)
          return BDMF_ERR_PARM;

    RDD_SCHEDULING_QUEUE_DESCRIPTOR_BBH_QUEUE_INDEX_WRITE_G(scheduling_queue_descriptor->bbh_queue_index, RDD_DS_TM_SCHEDULING_QUEUE_TABLE_ADDRESS_ARR, _entry);
    RDD_SCHEDULING_QUEUE_DESCRIPTOR_BLOCK_TYPE_WRITE_G(scheduling_queue_descriptor->block_type, RDD_DS_TM_SCHEDULING_QUEUE_TABLE_ADDRESS_ARR, _entry);
    RDD_SCHEDULING_QUEUE_DESCRIPTOR_CODEL_ENABLE_WRITE_G(scheduling_queue_descriptor->codel_enable, RDD_DS_TM_SCHEDULING_QUEUE_TABLE_ADDRESS_ARR, _entry);
    RDD_SCHEDULING_QUEUE_DESCRIPTOR_CODEL_DROPPING_WRITE_G(scheduling_queue_descriptor->codel_dropping, RDD_DS_TM_SCHEDULING_QUEUE_TABLE_ADDRESS_ARR, _entry);
    RDD_SCHEDULING_QUEUE_DESCRIPTOR_SCHEDULER_INDEX_WRITE_G(scheduling_queue_descriptor->scheduler_index, RDD_DS_TM_SCHEDULING_QUEUE_TABLE_ADDRESS_ARR, _entry);
    RDD_SCHEDULING_QUEUE_DESCRIPTOR_QUEUE_BIT_MASK_WRITE_G(scheduling_queue_descriptor->queue_bit_mask, RDD_DS_TM_SCHEDULING_QUEUE_TABLE_ADDRESS_ARR, _entry);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_codel_queue_descriptor_get(uint32_t _entry, rdd_codel_queue_descriptor_t *codel_queue_descriptor)
{
    if(!codel_queue_descriptor || _entry >= RDD_DS_TM_CODEL_QUEUE_TABLE_SIZE)
         return BDMF_ERR_PARM;

    RDD_CODEL_QUEUE_DESCRIPTOR_WINDOW_TS_READ_G(codel_queue_descriptor->window_ts, RDD_DS_TM_CODEL_QUEUE_TABLE_ADDRESS_ARR, _entry);
    RDD_CODEL_QUEUE_DESCRIPTOR_DROP_INTERVAL_READ_G(codel_queue_descriptor->drop_interval, RDD_DS_TM_CODEL_QUEUE_TABLE_ADDRESS_ARR, _entry);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_codel_queue_descriptor_set(uint32_t _entry, rdd_codel_queue_descriptor_t *codel_queue_descriptor)
{
    if(!codel_queue_descriptor || _entry >= RDD_DS_TM_CODEL_QUEUE_TABLE_SIZE)
         return BDMF_ERR_PARM;

    RDD_CODEL_QUEUE_DESCRIPTOR_WINDOW_TS_WRITE_G(codel_queue_descriptor->window_ts, RDD_DS_TM_CODEL_QUEUE_TABLE_ADDRESS_ARR, _entry);
    RDD_CODEL_QUEUE_DESCRIPTOR_DROP_INTERVAL_WRITE_G(codel_queue_descriptor->drop_interval, RDD_DS_TM_CODEL_QUEUE_TABLE_ADDRESS_ARR, _entry);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_codel_num_queues_set(uint16_t bits)
{
    RDD_BYTES_2_BITS_WRITE_G(bits, RDD_CODEL_NUM_QUEUES_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_codel_num_queues_get(uint16_t *bits)
{
    RDD_BYTES_2_BITS_READ_G(*bits, RDD_CODEL_NUM_QUEUES_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_codel_enable_table_set(uint32_t _entry, uint32_t bits)
{
    if(_entry >= RDD_CODEL_ENABLE_TABLE_SIZE)
          return BDMF_ERR_PARM;

    RDD_BYTES_4_BITS_WRITE_G(bits, RDD_CODEL_ENABLE_TABLE_ADDRESS_ARR, _entry);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_codel_enable_table_get(uint32_t _entry, uint32_t *bits)
{
    if(_entry >= RDD_CODEL_ENABLE_TABLE_SIZE)
          return BDMF_ERR_PARM;

    RDD_BYTES_4_BITS_READ_G(*bits, RDD_CODEL_ENABLE_TABLE_ADDRESS_ARR, _entry);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_flush_cfg_cpu_table_enable_set(bdmf_boolean enable)
{
    RDD_FLUSH_CFG_ENTRY_ENABLE_WRITE_G(enable, RDD_DS_TM_FLUSH_CFG_CPU_TABLE_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_flush_cfg_cpu_table_enable_get(bdmf_boolean *enable)
{
    RDD_FLUSH_CFG_ENTRY_ENABLE_READ_G(*enable, RDD_DS_TM_FLUSH_CFG_CPU_TABLE_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_flush_cfg_cpu_table_hw_flush_en_set(bdmf_boolean hw_flush_en)
{
    RDD_FLUSH_CFG_ENTRY_HW_FLUSH_EN_WRITE_G(hw_flush_en, RDD_DS_TM_FLUSH_CFG_CPU_TABLE_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_flush_cfg_cpu_table_hw_flush_en_get(bdmf_boolean *hw_flush_en)
{
    RDD_FLUSH_CFG_ENTRY_HW_FLUSH_EN_READ_G(*hw_flush_en, RDD_DS_TM_FLUSH_CFG_CPU_TABLE_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_flush_cfg_cpu_table_flush_aggr_set(uint8_t flush_aggr)
{
    RDD_FLUSH_CFG_ENTRY_FLUSH_AGGR_WRITE_G(flush_aggr, RDD_DS_TM_FLUSH_CFG_CPU_TABLE_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_flush_cfg_cpu_table_flush_aggr_get(uint8_t *flush_aggr)
{
    RDD_FLUSH_CFG_ENTRY_FLUSH_AGGR_READ_G(*flush_aggr, RDD_DS_TM_FLUSH_CFG_CPU_TABLE_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_flush_cfg_fw_table_enable_set(bdmf_boolean enable)
{
    RDD_FLUSH_CFG_ENTRY_ENABLE_WRITE_G(enable, RDD_DS_TM_FLUSH_CFG_FW_TABLE_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_flush_cfg_fw_table_enable_get(bdmf_boolean *enable)
{
    RDD_FLUSH_CFG_ENTRY_ENABLE_READ_G(*enable, RDD_DS_TM_FLUSH_CFG_FW_TABLE_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_flush_cfg_fw_table_hw_flush_en_set(bdmf_boolean hw_flush_en)
{
    RDD_FLUSH_CFG_ENTRY_HW_FLUSH_EN_WRITE_G(hw_flush_en, RDD_DS_TM_FLUSH_CFG_FW_TABLE_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_flush_cfg_fw_table_hw_flush_en_get(bdmf_boolean *hw_flush_en)
{
    RDD_FLUSH_CFG_ENTRY_HW_FLUSH_EN_READ_G(*hw_flush_en, RDD_DS_TM_FLUSH_CFG_FW_TABLE_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_flush_cfg_fw_table_flush_aggr_set(uint8_t flush_aggr)
{
    RDD_FLUSH_CFG_ENTRY_FLUSH_AGGR_WRITE_G(flush_aggr, RDD_DS_TM_FLUSH_CFG_FW_TABLE_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_flush_cfg_fw_table_flush_aggr_get(uint8_t *flush_aggr)
{
    RDD_FLUSH_CFG_ENTRY_FLUSH_AGGR_READ_G(*flush_aggr, RDD_DS_TM_FLUSH_CFG_FW_TABLE_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_bb_destination_table_set(uint16_t bits)
{
    RDD_BYTES_2_BITS_WRITE_G(bits, RDD_DS_TM_BB_DESTINATION_TABLE_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_bb_destination_table_get(uint16_t *bits)
{
    RDD_BYTES_2_BITS_READ_G(*bits, RDD_DS_TM_BB_DESTINATION_TABLE_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_tm_flow_cntr_table_set(uint32_t _entry, uint8_t cntr_id)
{
    if(_entry >= RDD_DS_TM_TM_FLOW_CNTR_TABLE_SIZE)
          return BDMF_ERR_PARM;

    RDD_TM_FLOW_CNTR_ENTRY_CNTR_ID_WRITE_G(cntr_id, RDD_DS_TM_TM_FLOW_CNTR_TABLE_ADDRESS_ARR, _entry);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_tm_flow_cntr_table_get(uint32_t _entry, uint8_t *cntr_id)
{
    if(_entry >= RDD_DS_TM_TM_FLOW_CNTR_TABLE_SIZE)
          return BDMF_ERR_PARM;

    RDD_TM_FLOW_CNTR_ENTRY_CNTR_ID_READ_G(*cntr_id, RDD_DS_TM_TM_FLOW_CNTR_TABLE_ADDRESS_ARR, _entry);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_flush_aggregation_task_disable_set(uint8_t bits)
{
    RDD_BYTE_1_BITS_WRITE_G(bits, RDD_DS_TM_FLUSH_AGGREGATION_TASK_DISABLE_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_flush_aggregation_task_disable_get(uint8_t *bits)
{
    RDD_BYTE_1_BITS_READ_G(*bits, RDD_DS_TM_FLUSH_AGGREGATION_TASK_DISABLE_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_first_queue_mapping_set(uint8_t bits)
{
    RDD_BYTE_1_BITS_WRITE_G(bits, RDD_DS_TM_FIRST_QUEUE_MAPPING_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_first_queue_mapping_get(uint8_t *bits)
{
    RDD_BYTE_1_BITS_READ_G(*bits, RDD_DS_TM_FIRST_QUEUE_MAPPING_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_flush_cfg_current_table_enable_set(bdmf_boolean enable)
{
    RDD_FLUSH_CFG_ENTRY_ENABLE_WRITE_G(enable, RDD_DS_TM_FLUSH_CFG_CURRENT_TABLE_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_flush_cfg_current_table_enable_get(bdmf_boolean *enable)
{
    RDD_FLUSH_CFG_ENTRY_ENABLE_READ_G(*enable, RDD_DS_TM_FLUSH_CFG_CURRENT_TABLE_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_flush_cfg_current_table_hw_flush_en_set(bdmf_boolean hw_flush_en)
{
    RDD_FLUSH_CFG_ENTRY_HW_FLUSH_EN_WRITE_G(hw_flush_en, RDD_DS_TM_FLUSH_CFG_CURRENT_TABLE_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_flush_cfg_current_table_hw_flush_en_get(bdmf_boolean *hw_flush_en)
{
    RDD_FLUSH_CFG_ENTRY_HW_FLUSH_EN_READ_G(*hw_flush_en, RDD_DS_TM_FLUSH_CFG_CURRENT_TABLE_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_flush_cfg_current_table_flush_aggr_set(uint8_t flush_aggr)
{
    RDD_FLUSH_CFG_ENTRY_FLUSH_AGGR_WRITE_G(flush_aggr, RDD_DS_TM_FLUSH_CFG_CURRENT_TABLE_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_flush_cfg_current_table_flush_aggr_get(uint8_t *flush_aggr)
{
    RDD_FLUSH_CFG_ENTRY_FLUSH_AGGR_READ_G(*flush_aggr, RDD_DS_TM_FLUSH_CFG_CURRENT_TABLE_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_codel_drop_descriptor_max_seq_drops_set(uint16_t max_seq_drops)
{
    RDD_CODEL_DROP_DESCRIPTOR_MAX_SEQ_DROPS_WRITE_G(max_seq_drops, RDD_DS_TM_CODEL_DROP_DESCRIPTOR_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_codel_drop_descriptor_max_seq_drops_get(uint16_t *max_seq_drops)
{
    RDD_CODEL_DROP_DESCRIPTOR_MAX_SEQ_DROPS_READ_G(*max_seq_drops, RDD_DS_TM_CODEL_DROP_DESCRIPTOR_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_codel_drop_descriptor_flush_task_wakeup_value_set(uint16_t flush_task_wakeup_value)
{
    RDD_CODEL_DROP_DESCRIPTOR_FLUSH_TASK_WAKEUP_VALUE_WRITE_G(flush_task_wakeup_value, RDD_DS_TM_CODEL_DROP_DESCRIPTOR_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_codel_drop_descriptor_flush_task_wakeup_value_get(uint16_t *flush_task_wakeup_value)
{
    RDD_CODEL_DROP_DESCRIPTOR_FLUSH_TASK_WAKEUP_VALUE_READ_G(*flush_task_wakeup_value, RDD_DS_TM_CODEL_DROP_DESCRIPTOR_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_codel_drop_descriptor_flush_cfg_ptr_set(uint16_t flush_cfg_ptr)
{
    RDD_CODEL_DROP_DESCRIPTOR_FLUSH_CFG_PTR_WRITE_G(flush_cfg_ptr, RDD_DS_TM_CODEL_DROP_DESCRIPTOR_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_codel_drop_descriptor_flush_cfg_ptr_get(uint16_t *flush_cfg_ptr)
{
    RDD_CODEL_DROP_DESCRIPTOR_FLUSH_CFG_PTR_READ_G(*flush_cfg_ptr, RDD_DS_TM_CODEL_DROP_DESCRIPTOR_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_codel_drop_descriptor_flush_enable_ptr_set(uint16_t flush_enable_ptr)
{
    RDD_CODEL_DROP_DESCRIPTOR_FLUSH_ENABLE_PTR_WRITE_G(flush_enable_ptr, RDD_DS_TM_CODEL_DROP_DESCRIPTOR_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_codel_drop_descriptor_flush_enable_ptr_get(uint16_t *flush_enable_ptr)
{
    RDD_CODEL_DROP_DESCRIPTOR_FLUSH_ENABLE_PTR_READ_G(*flush_enable_ptr, RDD_DS_TM_CODEL_DROP_DESCRIPTOR_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_codel_drop_descriptor_flush_enable_set(uint8_t flush_enable)
{
    RDD_CODEL_DROP_DESCRIPTOR_FLUSH_ENABLE_WRITE_G(flush_enable, RDD_DS_TM_CODEL_DROP_DESCRIPTOR_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_codel_drop_descriptor_flush_enable_get(uint8_t *flush_enable)
{
    RDD_CODEL_DROP_DESCRIPTOR_FLUSH_ENABLE_READ_G(*flush_enable, RDD_DS_TM_CODEL_DROP_DESCRIPTOR_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_scheduling_queue_table_enable_set(uint32_t _entry, bdmf_boolean enable)
{
    if(_entry >= RDD_DS_TM_SCHEDULING_QUEUE_TABLE_SIZE)
         return BDMF_ERR_PARM;

    RDD_SCHEDULING_QUEUE_DESCRIPTOR_ENABLE_WRITE_G(enable, RDD_DS_TM_SCHEDULING_QUEUE_TABLE_ADDRESS_ARR, _entry);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_scheduling_queue_table_enable_get(uint32_t _entry, bdmf_boolean *enable)
{
    if(_entry >= RDD_DS_TM_SCHEDULING_QUEUE_TABLE_SIZE)
         return BDMF_ERR_PARM;

    RDD_SCHEDULING_QUEUE_DESCRIPTOR_ENABLE_READ_G(*enable, RDD_DS_TM_SCHEDULING_QUEUE_TABLE_ADDRESS_ARR, _entry);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_scheduling_queue_table_rate_limit_enable_set(uint32_t _entry, bdmf_boolean rate_limit_enable)
{
    if(_entry >= RDD_DS_TM_SCHEDULING_QUEUE_TABLE_SIZE)
         return BDMF_ERR_PARM;

    RDD_SCHEDULING_QUEUE_DESCRIPTOR_RATE_LIMIT_ENABLE_WRITE_G(rate_limit_enable, RDD_DS_TM_SCHEDULING_QUEUE_TABLE_ADDRESS_ARR, _entry);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_scheduling_queue_table_rate_limit_enable_get(uint32_t _entry, bdmf_boolean *rate_limit_enable)
{
    if(_entry >= RDD_DS_TM_SCHEDULING_QUEUE_TABLE_SIZE)
         return BDMF_ERR_PARM;

    RDD_SCHEDULING_QUEUE_DESCRIPTOR_RATE_LIMIT_ENABLE_READ_G(*rate_limit_enable, RDD_DS_TM_SCHEDULING_QUEUE_TABLE_ADDRESS_ARR, _entry);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_scheduling_queue_table_codel_enable_set(uint32_t _entry, bdmf_boolean codel_enable)
{
    if(_entry >= RDD_DS_TM_SCHEDULING_QUEUE_TABLE_SIZE)
         return BDMF_ERR_PARM;

    RDD_SCHEDULING_QUEUE_DESCRIPTOR_CODEL_ENABLE_WRITE_G(codel_enable, RDD_DS_TM_SCHEDULING_QUEUE_TABLE_ADDRESS_ARR, _entry);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_scheduling_queue_table_codel_enable_get(uint32_t _entry, bdmf_boolean *codel_enable)
{
    if(_entry >= RDD_DS_TM_SCHEDULING_QUEUE_TABLE_SIZE)
         return BDMF_ERR_PARM;

    RDD_SCHEDULING_QUEUE_DESCRIPTOR_CODEL_ENABLE_READ_G(*codel_enable, RDD_DS_TM_SCHEDULING_QUEUE_TABLE_ADDRESS_ARR, _entry);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_scheduling_queue_table_codel_dropping_set(uint32_t _entry, bdmf_boolean codel_dropping)
{
    if(_entry >= RDD_DS_TM_SCHEDULING_QUEUE_TABLE_SIZE)
         return BDMF_ERR_PARM;

    RDD_SCHEDULING_QUEUE_DESCRIPTOR_CODEL_DROPPING_WRITE_G(codel_dropping, RDD_DS_TM_SCHEDULING_QUEUE_TABLE_ADDRESS_ARR, _entry);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_scheduling_queue_table_codel_dropping_get(uint32_t _entry, bdmf_boolean *codel_dropping)
{
    if(_entry >= RDD_DS_TM_SCHEDULING_QUEUE_TABLE_SIZE)
         return BDMF_ERR_PARM;

    RDD_SCHEDULING_QUEUE_DESCRIPTOR_CODEL_DROPPING_READ_G(*codel_dropping, RDD_DS_TM_SCHEDULING_QUEUE_TABLE_ADDRESS_ARR, _entry);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_scheduling_queue_table_rate_limiter_index_set(uint32_t _entry, uint8_t rate_limiter_index)
{
    if(_entry >= RDD_DS_TM_SCHEDULING_QUEUE_TABLE_SIZE)
         return BDMF_ERR_PARM;

    RDD_SCHEDULING_QUEUE_DESCRIPTOR_RATE_LIMITER_INDEX_WRITE_G(rate_limiter_index, RDD_DS_TM_SCHEDULING_QUEUE_TABLE_ADDRESS_ARR, _entry);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_scheduling_queue_table_rate_limiter_index_get(uint32_t _entry, uint8_t *rate_limiter_index)
{
    if(_entry >= RDD_DS_TM_SCHEDULING_QUEUE_TABLE_SIZE)
         return BDMF_ERR_PARM;

    RDD_SCHEDULING_QUEUE_DESCRIPTOR_RATE_LIMITER_INDEX_READ_G(*rate_limiter_index, RDD_DS_TM_SCHEDULING_QUEUE_TABLE_ADDRESS_ARR, _entry);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_scheduling_queue_table_quantum_number_set(uint32_t _entry, uint8_t quantum_number)
{
    if(_entry >= RDD_DS_TM_SCHEDULING_QUEUE_TABLE_SIZE)
         return BDMF_ERR_PARM;

    RDD_SCHEDULING_QUEUE_DESCRIPTOR_QUANTUM_NUMBER_WRITE_G(quantum_number, RDD_DS_TM_SCHEDULING_QUEUE_TABLE_ADDRESS_ARR, _entry);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_scheduling_queue_table_quantum_number_get(uint32_t _entry, uint8_t *quantum_number)
{
    if(_entry >= RDD_DS_TM_SCHEDULING_QUEUE_TABLE_SIZE)
         return BDMF_ERR_PARM;

    RDD_SCHEDULING_QUEUE_DESCRIPTOR_QUANTUM_NUMBER_READ_G(*quantum_number, RDD_DS_TM_SCHEDULING_QUEUE_TABLE_ADDRESS_ARR, _entry);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_basic_scheduler_table_ds_quantum_number_set(uint32_t _entry, uint8_t quantum_number)
{
    if(_entry >= RDD_BASIC_SCHEDULER_TABLE_DS_SIZE)
         return BDMF_ERR_PARM;

    RDD_BASIC_SCHEDULER_DESCRIPTOR_QUANTUM_NUMBER_WRITE_G(quantum_number, RDD_BASIC_SCHEDULER_TABLE_DS_ADDRESS_ARR, _entry);

    return BDMF_ERR_OK;
}

int rdd_ag_ds_tm_basic_scheduler_table_ds_quantum_number_get(uint32_t _entry, uint8_t *quantum_number)
{
    if(_entry >= RDD_BASIC_SCHEDULER_TABLE_DS_SIZE)
         return BDMF_ERR_PARM;

    RDD_BASIC_SCHEDULER_DESCRIPTOR_QUANTUM_NUMBER_READ_G(*quantum_number, RDD_BASIC_SCHEDULER_TABLE_DS_ADDRESS_ARR, _entry);

    return BDMF_ERR_OK;
}
