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


#ifndef _RDD_AG_PROCESSING_H_
#define _RDD_AG_PROCESSING_H_

typedef struct rdd_vport_cfg_entry_s
{
    bdmf_boolean congestion_flow_control;
    bdmf_boolean discard_prty;
    bdmf_boolean mcast_whitelist_skip;
    bdmf_boolean flow_control;
    bdmf_boolean sa_lookup_en;
    bdmf_boolean da_lookup_en;
    uint8_t sa_lookup_miss_action;
    uint8_t da_lookup_miss_action;
    uint8_t bridge_id;
    bdmf_boolean anti_spoofing_bypass;
    bdmf_boolean is_default_vid_set;
} rdd_vport_cfg_entry_t;

typedef struct rdd_emac_flow_ctrl_entry_s
{
    uint32_t rate;
    uint32_t max_burst_size;
    uint32_t threshold;
} rdd_emac_flow_ctrl_entry_t;

int rdd_ag_processing_vport_cfg_entry_get(uint32_t _entry, rdd_vport_cfg_entry_t *vport_cfg_entry);
int rdd_ag_processing_vport_cfg_entry_set(uint32_t _entry, rdd_vport_cfg_entry_t *vport_cfg_entry);
int rdd_ag_processing_emac_flow_ctrl_entry_get(uint32_t _entry, rdd_emac_flow_ctrl_entry_t *emac_flow_ctrl_entry);
int rdd_ag_processing_emac_flow_ctrl_entry_set(uint32_t _entry, rdd_emac_flow_ctrl_entry_t *emac_flow_ctrl_entry);
int rdd_ag_processing_vport_cfg_table_congestion_flow_control_set(uint32_t _entry, bdmf_boolean congestion_flow_control);
int rdd_ag_processing_vport_cfg_table_congestion_flow_control_get(uint32_t _entry, bdmf_boolean *congestion_flow_control);
int rdd_ag_processing_vport_cfg_table_mcast_whitelist_skip_set(uint32_t _entry, bdmf_boolean mcast_whitelist_skip);
int rdd_ag_processing_vport_cfg_table_mcast_whitelist_skip_get(uint32_t _entry, bdmf_boolean *mcast_whitelist_skip);
int rdd_ag_processing_vport_cfg_table_flow_control_set(uint32_t _entry, bdmf_boolean flow_control);
int rdd_ag_processing_vport_cfg_table_flow_control_get(uint32_t _entry, bdmf_boolean *flow_control);
int rdd_ag_processing_vport_cfg_table_egress_isolation_en_set(uint32_t _entry, bdmf_boolean egress_isolation_en);
int rdd_ag_processing_vport_cfg_table_egress_isolation_en_get(uint32_t _entry, bdmf_boolean *egress_isolation_en);
int rdd_ag_processing_vport_cfg_table_ingress_isolation_en_set(uint32_t _entry, bdmf_boolean ingress_isolation_en);
int rdd_ag_processing_vport_cfg_table_ingress_isolation_en_get(uint32_t _entry, bdmf_boolean *ingress_isolation_en);
int rdd_ag_processing_vport_cfg_table_bridge_id_set(uint32_t _entry, uint8_t bridge_id);
int rdd_ag_processing_vport_cfg_table_bridge_id_get(uint32_t _entry, uint8_t *bridge_id);
int rdd_ag_processing_vport_cfg_table_bridge_and_vlan_ingress_lookup_method_set(uint32_t _entry, bdmf_boolean bridge_and_vlan_ingress_lookup_method);
int rdd_ag_processing_vport_cfg_table_bridge_and_vlan_ingress_lookup_method_get(uint32_t _entry, bdmf_boolean *bridge_and_vlan_ingress_lookup_method);
int rdd_ag_processing_vport_cfg_table_bridge_and_vlan_egress_lookup_method_set(uint32_t _entry, bdmf_boolean bridge_and_vlan_egress_lookup_method);
int rdd_ag_processing_vport_cfg_table_bridge_and_vlan_egress_lookup_method_get(uint32_t _entry, bdmf_boolean *bridge_and_vlan_egress_lookup_method);
int rdd_ag_processing_vport_cfg_table_protocol_filters_dis_set(uint32_t _entry, uint8_t protocol_filters_dis);
int rdd_ag_processing_vport_cfg_table_protocol_filters_dis_get(uint32_t _entry, uint8_t *protocol_filters_dis);
int rdd_ag_processing_vport_cfg_table_egress_isolation_map_set(uint32_t _entry, uint32_t egress_isolation_map);
int rdd_ag_processing_vport_cfg_table_egress_isolation_map_get(uint32_t _entry, uint32_t *egress_isolation_map);
int rdd_ag_processing_policer_params_table_factor_bytes_set(uint32_t _entry, uint8_t factor_bytes);
int rdd_ag_processing_policer_params_table_factor_bytes_get(uint32_t _entry, uint8_t *factor_bytes);
int rdd_ag_processing_policer_params_table_dei_mode_set(uint32_t _entry, bdmf_boolean dei_mode);
int rdd_ag_processing_policer_params_table_dei_mode_get(uint32_t _entry, bdmf_boolean *dei_mode);
int rdd_ag_processing_policer_params_table_single_bucket_set(uint32_t _entry, bdmf_boolean single_bucket);
int rdd_ag_processing_policer_params_table_single_bucket_get(uint32_t _entry, bdmf_boolean *single_bucket);
int rdd_ag_processing_codel_enable_table_set(uint32_t _entry, uint32_t bits);
int rdd_ag_processing_codel_enable_table_get(uint32_t _entry, uint32_t *bits);
int rdd_ag_processing_emac_flow_ctrl_rate_set(uint32_t _entry, uint32_t rate);
int rdd_ag_processing_emac_flow_ctrl_rate_get(uint32_t _entry, uint32_t *rate);
int rdd_ag_processing_emac_flow_ctrl_max_burst_size_set(uint32_t _entry, uint32_t max_burst_size);
int rdd_ag_processing_emac_flow_ctrl_max_burst_size_get(uint32_t _entry, uint32_t *max_burst_size);
int rdd_ag_processing_emac_flow_ctrl_threshold_set(uint32_t _entry, uint32_t threshold);
int rdd_ag_processing_emac_flow_ctrl_threshold_get(uint32_t _entry, uint32_t *threshold);
int rdd_ag_processing_codel_num_queues_set(uint16_t bits);
int rdd_ag_processing_codel_num_queues_get(uint16_t *bits);
int rdd_ag_processing_vport_cfg_ex_table_set(uint32_t _entry, bdmf_boolean loopback_en, bdmf_boolean mirroring_en, uint8_t ingress_filter_profile, uint8_t port_mac_addr_idx, uint8_t emac_idx, uint8_t viq, bdmf_boolean rate_limit_unknown_da, bdmf_boolean rate_limit_broadcast, bdmf_boolean rate_limit_multicast, bdmf_boolean rate_limit_all_traffic, bdmf_boolean port_dbg_stat_en, uint8_t policer_idx);
int rdd_ag_processing_vport_cfg_ex_table_get(uint32_t _entry, bdmf_boolean *loopback_en, bdmf_boolean *mirroring_en, uint8_t *ingress_filter_profile, uint8_t *port_mac_addr_idx, uint8_t *emac_idx, uint8_t *viq, bdmf_boolean *rate_limit_unknown_da, bdmf_boolean *rate_limit_broadcast, bdmf_boolean *rate_limit_multicast, bdmf_boolean *rate_limit_all_traffic, bdmf_boolean *port_dbg_stat_en, uint8_t *policer_idx);
int rdd_ag_processing_vport_cfg_ex_table_loopback_en_set(uint32_t _entry, bdmf_boolean loopback_en);
int rdd_ag_processing_vport_cfg_ex_table_loopback_en_get(uint32_t _entry, bdmf_boolean *loopback_en);
int rdd_ag_processing_vport_cfg_ex_table_mirroring_en_set(uint32_t _entry, bdmf_boolean mirroring_en);
int rdd_ag_processing_vport_cfg_ex_table_mirroring_en_get(uint32_t _entry, bdmf_boolean *mirroring_en);
int rdd_ag_processing_vport_cfg_ex_table_ingress_filter_profile_set(uint32_t _entry, uint8_t ingress_filter_profile);
int rdd_ag_processing_vport_cfg_ex_table_ingress_filter_profile_get(uint32_t _entry, uint8_t *ingress_filter_profile);
int rdd_ag_processing_vport_cfg_ex_table_port_mac_addr_idx_set(uint32_t _entry, uint8_t port_mac_addr_idx);
int rdd_ag_processing_vport_cfg_ex_table_port_mac_addr_idx_get(uint32_t _entry, uint8_t *port_mac_addr_idx);
int rdd_ag_processing_vport_cfg_ex_table_emac_idx_set(uint32_t _entry, uint8_t emac_idx);
int rdd_ag_processing_vport_cfg_ex_table_emac_idx_get(uint32_t _entry, uint8_t *emac_idx);
int rdd_ag_processing_vport_cfg_ex_table_viq_set(uint32_t _entry, uint8_t viq);
int rdd_ag_processing_vport_cfg_ex_table_viq_get(uint32_t _entry, uint8_t *viq);
int rdd_ag_processing_vport_cfg_ex_table_rate_limit_unknown_da_set(uint32_t _entry, bdmf_boolean rate_limit_unknown_da);
int rdd_ag_processing_vport_cfg_ex_table_rate_limit_unknown_da_get(uint32_t _entry, bdmf_boolean *rate_limit_unknown_da);
int rdd_ag_processing_vport_cfg_ex_table_rate_limit_broadcast_set(uint32_t _entry, bdmf_boolean rate_limit_broadcast);
int rdd_ag_processing_vport_cfg_ex_table_rate_limit_broadcast_get(uint32_t _entry, bdmf_boolean *rate_limit_broadcast);
int rdd_ag_processing_vport_cfg_ex_table_rate_limit_multicast_set(uint32_t _entry, bdmf_boolean rate_limit_multicast);
int rdd_ag_processing_vport_cfg_ex_table_rate_limit_multicast_get(uint32_t _entry, bdmf_boolean *rate_limit_multicast);
int rdd_ag_processing_vport_cfg_ex_table_rate_limit_all_traffic_set(uint32_t _entry, bdmf_boolean rate_limit_all_traffic);
int rdd_ag_processing_vport_cfg_ex_table_rate_limit_all_traffic_get(uint32_t _entry, bdmf_boolean *rate_limit_all_traffic);
int rdd_ag_processing_vport_cfg_ex_table_port_dbg_stat_en_set(uint32_t _entry, bdmf_boolean port_dbg_stat_en);
int rdd_ag_processing_vport_cfg_ex_table_port_dbg_stat_en_get(uint32_t _entry, bdmf_boolean *port_dbg_stat_en);
int rdd_ag_processing_vport_cfg_ex_table_policer_idx_set(uint32_t _entry, uint8_t policer_idx);
int rdd_ag_processing_vport_cfg_ex_table_policer_idx_get(uint32_t _entry, uint8_t *policer_idx);
int rdd_ag_processing_ingress_packet_based_mapping_us_set(uint8_t us);
int rdd_ag_processing_ingress_packet_based_mapping_us_get(uint8_t *us);
int rdd_ag_processing_ingress_packet_based_mapping_ds_set(uint8_t ds);
int rdd_ag_processing_ingress_packet_based_mapping_ds_get(uint8_t *ds);
int rdd_ag_processing_vport_pbit_to_discard_prio_vector_set(uint32_t _entry, uint8_t bits);
int rdd_ag_processing_vport_pbit_to_discard_prio_vector_get(uint32_t _entry, uint8_t *bits);

#endif /* _RDD_AG_PROCESSING_H_ */
