#ifndef RUNNER_A_CODE_ADDRESSES
#define RUNNER_A_CODE_ADDRESSES

#define runner_a_start_task_initialization_task		(0x14)
#define runner_a_initialization_task		(0x14)
#define runner_a_start_task_timer_scheduler_set		(0x38AC)
#define runner_a_timer_scheduler_set		(0x38AC)
#define runner_a_start_task_cpu_rx_wakeup_request		(0x2670)
#define runner_a_cpu_rx_wakeup_request		(0x2670)
#define runner_a_start_task_flow_cache_wakeup_request		(0xE78)
#define runner_a_flow_cache_wakeup_request		(0xE78)
#define runner_a_start_task_cpu_tx_wakeup_request		(0x2E1C)
#define runner_a_cpu_tx_wakeup_request		(0x2E1C)
#define runner_a_start_task_policer_budget_allocator_1st_wakeup_request		(0x39E0)
#define runner_a_policer_budget_allocator_1st_wakeup_request		(0x39E0)
#define runner_a_start_task_wan_direct_wakeup_request		(0x18D4)
#define runner_a_wan_direct_wakeup_request		(0x18D4)
#define runner_a_start_task_wan_cpu_wakeup_request		(0x204)
#define runner_a_wan_cpu_wakeup_request		(0x204)
#define runner_a_start_task_wan_normal_wakeup_request		(0x3C8)
#define runner_a_wan_normal_wakeup_request		(0x3C8)
#define runner_a_start_task_downstream_multicast_wakeup_request		(0x335C)
#define runner_a_downstream_multicast_wakeup_request		(0x335C)
#define runner_a_start_task_debug_routine		(0x11C)
#define runner_a_debug_routine		(0x11C)
#define runner_a_start_task_free_skb_index_wakeup_request		(0x39AC)
#define runner_a_free_skb_index_wakeup_request		(0x39AC)
#define runner_a_free_skb_index_tx_abs_done		(0x39AC)
#define runner_a_start_task_dhd_tx_post		(0x4428)
#define runner_a_dhd_tx_post		(0x4428)
#define runner_a_start_task_dhd_tx_complete_wakeup_request		(0x484C)
#define runner_a_dhd_tx_complete_wakeup_request		(0x484C)
#define runner_a_dhd_tx_complete_check_next		(0x484C)
#define runner_a_start_task_ipsec_ds_wakeup_request		(0x3CE8)
#define runner_a_ipsec_ds_wakeup_request		(0x3CE8)
#define runner_a_start_task_ethwan2_normal_wakeup_request		(0x260)
#define runner_a_ethwan2_normal_wakeup_request		(0x260)
#define runner_a_gpe_sop_push_replace_ddr_sram_32		(0x1A90)
#define runner_a_gpe_sop_push_replace_sram_32_64		(0x1B04)
#define runner_a_gpe_sop_push_replace_sram_64		(0x1B18)
#define runner_a_gpe_sop_push_replace_sram_64_32		(0x1B2C)
#define runner_a_gpe_sop_pull_replace_ddr_sram_32		(0x1B40)
#define runner_a_gpe_sop_pull_replace_sram_32_64		(0x1BB4)
#define runner_a_gpe_sop_pull_replace_sram_64		(0x1C04)
#define runner_a_gpe_sop_pull_replace_sram_64_32		(0x1C40)
#define runner_a_gpe_replace_pointer_32_ddr		(0x1C90)
#define runner_a_gpe_replace_pointer_32_sram		(0x1CB4)
#define runner_a_gpe_replace_pointer_64_sram		(0x1CD8)
#define runner_a_gpe_replace_16		(0x1CFC)
#define runner_a_gpe_replace_32		(0x1D30)
#define runner_a_gpe_replace_bits_16		(0x1D54)
#define runner_a_gpe_copy_add_16_cl		(0x1D80)
#define runner_a_gpe_copy_add_16_sram		(0x1D8C)
#define runner_a_gpe_copy_bits_16_cl		(0x1DD4)
#define runner_a_gpe_copy_bits_16_sram		(0x1DE0)
#define runner_a_gpe_insert_16		(0x1E28)
#define runner_a_gpe_delete_16		(0x1E90)
#define runner_a_gpe_decrement_8		(0x1ED0)
#define runner_a_gpe_apply_icsum_16		(0x1EF4)
#define runner_a_gpe_apply_icsum_nz_16		(0x1F18)
#define runner_a_gpe_compute_csum_16_cl		(0x1F54)
#define runner_a_gpe_compute_csum_16_sram		(0x1F60)
#define runner_a_gpe_buffer_copy_16_sram		(0x1FA0)
#define runner_a_gpe_buffer_copy_16_ddr		(0x1FC8)
#define runner_a_ingress_classification_key_src_ip		(0x21D0)
#define runner_a_ingress_classification_key_dst_ip		(0x2214)
#define runner_a_ingress_classification_key_ipv6_flow_label		(0x2258)
#define runner_a_ingress_classification_key_generic_rule_1		(0x227C)
#define runner_a_ingress_classification_key_generic_rule_2		(0x2284)
#define runner_a_ingress_classification_key_outer_tpid		(0x22F0)
#define runner_a_ingress_classification_key_inner_tpid		(0x22FC)
#define runner_a_ingress_classification_key_src_port		(0x2318)
#define runner_a_ingress_classification_key_dst_port		(0x2338)
#define runner_a_ingress_classification_key_outer_vlan		(0x2358)
#define runner_a_ingress_classification_key_inner_vlan		(0x2370)
#define runner_a_ingress_classification_key_dst_mac		(0x238C)
#define runner_a_ingress_classification_key_src_mac		(0x2394)
#define runner_a_ingress_classification_key_ether_type		(0x23BC)
#define runner_a_ingress_classification_key_ip_protocol		(0x23D0)
#define runner_a_ingress_classification_key_dscp		(0x23F4)
#define runner_a_ingress_classification_key_ssid		(0x2410)
#define runner_a_ingress_classification_key_ingress_port		(0x2414)
#define runner_a_ingress_classification_key_outer_pbits		(0x2424)
#define runner_a_ingress_classification_key_inner_pbits		(0x243C)
#define runner_a_ingress_classification_key_number_of_vlans		(0x2458)
#define runner_a_ingress_classification_key_layer3_protocol		(0x2468)
#define runner_a_cpu_rx_meter_budget_allocate		(0x392C)
#define runner_a_dhd_tx_post_close_aggregation		(0x3978)
#define runner_a_schedule_free_skb_index		(0x3990)

#endif