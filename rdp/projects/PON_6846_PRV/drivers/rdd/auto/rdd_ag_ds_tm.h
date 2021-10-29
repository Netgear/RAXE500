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


#ifndef _RDD_AG_DS_TM_H_
#define _RDD_AG_DS_TM_H_

typedef struct rdd_codel_drop_descriptor_s
{
    uint16_t max_seq_drops;
    uint16_t flush_task_wakeup_value;
    uint16_t flush_cfg_ptr;
    uint16_t flush_enable_ptr;
    uint8_t flush_enable;
} rdd_codel_drop_descriptor_t;

typedef struct rdd_scheduling_queue_descriptor_s
{
    uint8_t bbh_queue_index;
    bdmf_boolean block_type;
    bdmf_boolean codel_enable;
    bdmf_boolean codel_dropping;
    uint8_t scheduler_index;
    uint8_t queue_bit_mask;
} rdd_scheduling_queue_descriptor_t;

typedef struct rdd_codel_queue_descriptor_s
{
    uint16_t window_ts;
    uint16_t drop_interval;
} rdd_codel_queue_descriptor_t;

int rdd_ag_ds_tm_codel_drop_descriptor_get(rdd_codel_drop_descriptor_t *codel_drop_descriptor);
int rdd_ag_ds_tm_codel_drop_descriptor_set(rdd_codel_drop_descriptor_t *codel_drop_descriptor);
int rdd_ag_ds_tm_scheduling_queue_descriptor_get(uint32_t _entry, rdd_scheduling_queue_descriptor_t *scheduling_queue_descriptor);
int rdd_ag_ds_tm_scheduling_queue_descriptor_set(uint32_t _entry, rdd_scheduling_queue_descriptor_t *scheduling_queue_descriptor);
int rdd_ag_ds_tm_codel_queue_descriptor_get(uint32_t _entry, rdd_codel_queue_descriptor_t *codel_queue_descriptor);
int rdd_ag_ds_tm_codel_queue_descriptor_set(uint32_t _entry, rdd_codel_queue_descriptor_t *codel_queue_descriptor);
int rdd_ag_ds_tm_codel_num_queues_set(uint16_t bits);
int rdd_ag_ds_tm_codel_num_queues_get(uint16_t *bits);
int rdd_ag_ds_tm_codel_enable_table_set(uint32_t _entry, uint32_t bits);
int rdd_ag_ds_tm_codel_enable_table_get(uint32_t _entry, uint32_t *bits);
int rdd_ag_ds_tm_flush_cfg_cpu_table_enable_set(bdmf_boolean enable);
int rdd_ag_ds_tm_flush_cfg_cpu_table_enable_get(bdmf_boolean *enable);
int rdd_ag_ds_tm_flush_cfg_cpu_table_hw_flush_en_set(bdmf_boolean hw_flush_en);
int rdd_ag_ds_tm_flush_cfg_cpu_table_hw_flush_en_get(bdmf_boolean *hw_flush_en);
int rdd_ag_ds_tm_flush_cfg_cpu_table_flush_aggr_set(uint8_t flush_aggr);
int rdd_ag_ds_tm_flush_cfg_cpu_table_flush_aggr_get(uint8_t *flush_aggr);
int rdd_ag_ds_tm_flush_cfg_fw_table_enable_set(bdmf_boolean enable);
int rdd_ag_ds_tm_flush_cfg_fw_table_enable_get(bdmf_boolean *enable);
int rdd_ag_ds_tm_flush_cfg_fw_table_hw_flush_en_set(bdmf_boolean hw_flush_en);
int rdd_ag_ds_tm_flush_cfg_fw_table_hw_flush_en_get(bdmf_boolean *hw_flush_en);
int rdd_ag_ds_tm_flush_cfg_fw_table_flush_aggr_set(uint8_t flush_aggr);
int rdd_ag_ds_tm_flush_cfg_fw_table_flush_aggr_get(uint8_t *flush_aggr);
int rdd_ag_ds_tm_bb_destination_table_set(uint16_t bits);
int rdd_ag_ds_tm_bb_destination_table_get(uint16_t *bits);
int rdd_ag_ds_tm_tm_flow_cntr_table_set(uint32_t _entry, uint8_t cntr_id);
int rdd_ag_ds_tm_tm_flow_cntr_table_get(uint32_t _entry, uint8_t *cntr_id);
int rdd_ag_ds_tm_flush_aggregation_task_disable_set(uint8_t bits);
int rdd_ag_ds_tm_flush_aggregation_task_disable_get(uint8_t *bits);
int rdd_ag_ds_tm_first_queue_mapping_set(uint8_t bits);
int rdd_ag_ds_tm_first_queue_mapping_get(uint8_t *bits);
int rdd_ag_ds_tm_flush_cfg_current_table_enable_set(bdmf_boolean enable);
int rdd_ag_ds_tm_flush_cfg_current_table_enable_get(bdmf_boolean *enable);
int rdd_ag_ds_tm_flush_cfg_current_table_hw_flush_en_set(bdmf_boolean hw_flush_en);
int rdd_ag_ds_tm_flush_cfg_current_table_hw_flush_en_get(bdmf_boolean *hw_flush_en);
int rdd_ag_ds_tm_flush_cfg_current_table_flush_aggr_set(uint8_t flush_aggr);
int rdd_ag_ds_tm_flush_cfg_current_table_flush_aggr_get(uint8_t *flush_aggr);
int rdd_ag_ds_tm_codel_drop_descriptor_max_seq_drops_set(uint16_t max_seq_drops);
int rdd_ag_ds_tm_codel_drop_descriptor_max_seq_drops_get(uint16_t *max_seq_drops);
int rdd_ag_ds_tm_codel_drop_descriptor_flush_task_wakeup_value_set(uint16_t flush_task_wakeup_value);
int rdd_ag_ds_tm_codel_drop_descriptor_flush_task_wakeup_value_get(uint16_t *flush_task_wakeup_value);
int rdd_ag_ds_tm_codel_drop_descriptor_flush_cfg_ptr_set(uint16_t flush_cfg_ptr);
int rdd_ag_ds_tm_codel_drop_descriptor_flush_cfg_ptr_get(uint16_t *flush_cfg_ptr);
int rdd_ag_ds_tm_codel_drop_descriptor_flush_enable_ptr_set(uint16_t flush_enable_ptr);
int rdd_ag_ds_tm_codel_drop_descriptor_flush_enable_ptr_get(uint16_t *flush_enable_ptr);
int rdd_ag_ds_tm_codel_drop_descriptor_flush_enable_set(uint8_t flush_enable);
int rdd_ag_ds_tm_codel_drop_descriptor_flush_enable_get(uint8_t *flush_enable);
int rdd_ag_ds_tm_scheduling_queue_table_enable_set(uint32_t _entry, bdmf_boolean enable);
int rdd_ag_ds_tm_scheduling_queue_table_enable_get(uint32_t _entry, bdmf_boolean *enable);
int rdd_ag_ds_tm_scheduling_queue_table_rate_limit_enable_set(uint32_t _entry, bdmf_boolean rate_limit_enable);
int rdd_ag_ds_tm_scheduling_queue_table_rate_limit_enable_get(uint32_t _entry, bdmf_boolean *rate_limit_enable);
int rdd_ag_ds_tm_scheduling_queue_table_codel_enable_set(uint32_t _entry, bdmf_boolean codel_enable);
int rdd_ag_ds_tm_scheduling_queue_table_codel_enable_get(uint32_t _entry, bdmf_boolean *codel_enable);
int rdd_ag_ds_tm_scheduling_queue_table_codel_dropping_set(uint32_t _entry, bdmf_boolean codel_dropping);
int rdd_ag_ds_tm_scheduling_queue_table_codel_dropping_get(uint32_t _entry, bdmf_boolean *codel_dropping);
int rdd_ag_ds_tm_scheduling_queue_table_rate_limiter_index_set(uint32_t _entry, uint8_t rate_limiter_index);
int rdd_ag_ds_tm_scheduling_queue_table_rate_limiter_index_get(uint32_t _entry, uint8_t *rate_limiter_index);
int rdd_ag_ds_tm_scheduling_queue_table_quantum_number_set(uint32_t _entry, uint8_t quantum_number);
int rdd_ag_ds_tm_scheduling_queue_table_quantum_number_get(uint32_t _entry, uint8_t *quantum_number);

int rdd_ag_ds_tm_basic_scheduler_table_ds_quantum_number_set(uint32_t _entry, uint8_t quantum_number);
int rdd_ag_ds_tm_basic_scheduler_table_ds_quantum_number_get(uint32_t _entry, uint8_t *quantum_number);

#endif /* _RDD_AG_DS_TM_H_ */