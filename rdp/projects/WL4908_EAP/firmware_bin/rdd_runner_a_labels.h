#ifndef RUNNER_A_CODE_ADDRESSES
#define RUNNER_A_CODE_ADDRESSES

#define runner_a_start_task_initialization_task		(0x14)
#define runner_a_initialization_task		(0x14)
#define runner_a_start_task_timer_scheduler_set		(0x33BC)
#define runner_a_timer_scheduler_set		(0x33BC)
#define runner_a_start_task_cpu_rx_wakeup_request		(0x28F8)
#define runner_a_cpu_rx_wakeup_request		(0x28F8)
#define runner_a_start_task_flow_cache_wakeup_request		(0x13C8)
#define runner_a_flow_cache_wakeup_request		(0x13C8)
#define runner_a_start_task_cpu_tx_wakeup_request		(0x304C)
#define runner_a_cpu_tx_wakeup_request		(0x304C)
#define runner_a_start_task_policer_budget_allocator_1st_wakeup_request		(0x34D8)
#define runner_a_policer_budget_allocator_1st_wakeup_request		(0x34D8)
#define runner_a_start_task_wan_cpu_wakeup_request		(0x1FC)
#define runner_a_wan_cpu_wakeup_request		(0x1FC)
#define runner_a_start_task_wan_normal_wakeup_request		(0x3DC)
#define runner_a_wan_normal_wakeup_request		(0x3DC)
#define runner_a_start_task_debug_routine		(0x114)
#define runner_a_debug_routine		(0x114)
#define runner_a_start_task_free_skb_index_wakeup_request		(0x34A4)
#define runner_a_free_skb_index_wakeup_request		(0x34A4)
#define runner_a_free_skb_index_tx_abs_done		(0x34A4)
#define runner_a_start_task_ethwan2_normal_wakeup_request		(0x258)
#define runner_a_ethwan2_normal_wakeup_request		(0x258)
#define runner_a_gpe_sop_push_replace_ddr_sram_32		(0x1CEC)
#define runner_a_gpe_sop_push_replace_sram_32_64		(0x1D60)
#define runner_a_gpe_sop_push_replace_sram_64		(0x1D74)
#define runner_a_gpe_sop_push_replace_sram_64_32		(0x1D88)
#define runner_a_gpe_sop_pull_replace_ddr_sram_32		(0x1D9C)
#define runner_a_gpe_sop_pull_replace_sram_32_64		(0x1E10)
#define runner_a_gpe_sop_pull_replace_sram_64		(0x1E60)
#define runner_a_gpe_sop_pull_replace_sram_64_32		(0x1E9C)
#define runner_a_gpe_replace_pointer_32_ddr		(0x1EEC)
#define runner_a_gpe_replace_pointer_32_sram		(0x1F10)
#define runner_a_gpe_replace_pointer_64_sram		(0x1F34)
#define runner_a_gpe_replace_16		(0x1F58)
#define runner_a_gpe_replace_32		(0x1F8C)
#define runner_a_gpe_replace_bits_16		(0x1FB0)
#define runner_a_gpe_copy_add_16_cl		(0x1FDC)
#define runner_a_gpe_copy_add_16_sram		(0x1FE8)
#define runner_a_gpe_copy_bits_16_cl		(0x2030)
#define runner_a_gpe_copy_bits_16_sram		(0x203C)
#define runner_a_gpe_insert_16		(0x2084)
#define runner_a_gpe_delete_16		(0x20EC)
#define runner_a_gpe_decrement_8		(0x212C)
#define runner_a_gpe_apply_icsum_16		(0x2150)
#define runner_a_gpe_apply_icsum_nz_16		(0x2174)
#define runner_a_gpe_compute_csum_16_cl		(0x21B0)
#define runner_a_gpe_compute_csum_16_sram		(0x21BC)
#define runner_a_gpe_buffer_copy_16_sram		(0x21FC)
#define runner_a_gpe_buffer_copy_16_ddr		(0x2224)
#define runner_a_gpe_replace_add_packet_length_cl		(0x2250)
#define runner_a_ingress_classification_key_src_ip		(0x2458)
#define runner_a_ingress_classification_key_dst_ip		(0x249C)
#define runner_a_ingress_classification_key_ipv6_flow_label		(0x24E0)
#define runner_a_ingress_classification_key_generic_rule_1		(0x2504)
#define runner_a_ingress_classification_key_generic_rule_2		(0x250C)
#define runner_a_ingress_classification_key_outer_tpid		(0x2578)
#define runner_a_ingress_classification_key_inner_tpid		(0x2584)
#define runner_a_ingress_classification_key_src_port		(0x25A0)
#define runner_a_ingress_classification_key_dst_port		(0x25C0)
#define runner_a_ingress_classification_key_outer_vlan		(0x25E0)
#define runner_a_ingress_classification_key_inner_vlan		(0x25F8)
#define runner_a_ingress_classification_key_dst_mac		(0x2614)
#define runner_a_ingress_classification_key_src_mac		(0x261C)
#define runner_a_ingress_classification_key_ether_type		(0x2644)
#define runner_a_ingress_classification_key_ip_protocol		(0x2658)
#define runner_a_ingress_classification_key_dscp		(0x267C)
#define runner_a_ingress_classification_key_ssid		(0x2698)
#define runner_a_ingress_classification_key_ingress_port		(0x269C)
#define runner_a_ingress_classification_key_outer_pbits		(0x26AC)
#define runner_a_ingress_classification_key_inner_pbits		(0x26C4)
#define runner_a_ingress_classification_key_number_of_vlans		(0x26E0)
#define runner_a_ingress_classification_key_layer3_protocol		(0x26F0)
#define runner_a_cpu_rx_meter_budget_allocate		(0x343C)
#define runner_a_schedule_free_skb_index		(0x3488)

#endif