/* IMAGE 0 LABELS */
#ifndef IMAGE_0_CODE_ADDRESSES
#define IMAGE_0_CODE_ADDRESSES

#ifndef PC_ADDRESS_INST_IND

#define image_0_start_task_initialization_task        (0x48)
#define image_0_initialization_task        (0x48)
#define image_0_start_task_update_fifo_ds_read_1st_wakeup_request        (0x1204)
#define image_0_update_fifo_ds_read_1st_wakeup_request        (0x1204)
#define image_0_start_task_ghost_reporting_1st_wakeup_request        (0xda4)
#define image_0_ghost_reporting_1st_wakeup_request        (0xda4)
#define image_0_start_task_budget_allocator_1st_wakeup_request        (0x7fc)
#define image_0_budget_allocator_1st_wakeup_request        (0x7fc)
#define image_0_start_task_debug_routine        (0x9c)
#define image_0_debug_routine        (0x9c)
#define image_0_start_task_ds_tx_task_wakeup_request        (0x46c)
#define image_0_ds_tx_task_wakeup_request        (0x46c)
#define image_0_start_task_flush_task_1st_wakeup_request        (0xc9c)
#define image_0_flush_task_1st_wakeup_request        (0xc9c)
#define image_0_debug_routine_handler        (0x4)

#else

#define image_0_start_task_initialization_task        (0x12)
#define image_0_initialization_task        (0x12)
#define image_0_start_task_update_fifo_ds_read_1st_wakeup_request        (0x481)
#define image_0_update_fifo_ds_read_1st_wakeup_request        (0x481)
#define image_0_start_task_ghost_reporting_1st_wakeup_request        (0x369)
#define image_0_ghost_reporting_1st_wakeup_request        (0x369)
#define image_0_start_task_budget_allocator_1st_wakeup_request        (0x1ff)
#define image_0_budget_allocator_1st_wakeup_request        (0x1ff)
#define image_0_start_task_debug_routine        (0x27)
#define image_0_debug_routine        (0x27)
#define image_0_start_task_ds_tx_task_wakeup_request        (0x11b)
#define image_0_ds_tx_task_wakeup_request        (0x11b)
#define image_0_start_task_flush_task_1st_wakeup_request        (0x327)
#define image_0_flush_task_1st_wakeup_request        (0x327)
#define image_0_debug_routine_handler        (0x1)

#endif


#endif

/* IMAGE 1 LABELS */
#ifndef IMAGE_1_CODE_ADDRESSES
#define IMAGE_1_CODE_ADDRESSES

#ifndef PC_ADDRESS_INST_IND

#define image_1_start_task_initialization_task        (0x8)
#define image_1_initialization_task        (0x8)
#define image_1_start_task_processing_wakeup_request        (0xb28)
#define image_1_processing_wakeup_request        (0xb28)
#define image_1_start_task_debug_routine        (0x3b90)
#define image_1_debug_routine        (0x3b90)
#define image_1_start_task_cpu_rx_wakeup_request        (0x481c)
#define image_1_cpu_rx_wakeup_request        (0x481c)
#define image_1_start_task_cpu_recycle_wakeup_request        (0x4e10)
#define image_1_cpu_recycle_wakeup_request        (0x4e10)
#define image_1_start_task_interrupt_coalescing_1st_wakeup_request        (0x4f64)
#define image_1_interrupt_coalescing_1st_wakeup_request        (0x4f64)
#define image_1_start_task_cpu_rx_meter_budget_allocator_1st_wakeup_request        (0x4b40)
#define image_1_cpu_rx_meter_budget_allocator_1st_wakeup_request        (0x4b40)
#define image_1_start_task_cpu_rx_copy_wakeup_request        (0x3c9c)
#define image_1_cpu_rx_copy_wakeup_request        (0x3c9c)
#define image_1_debug_routine_handler        (0x4)
#define image_1_tcam_cmd_ic_submit        (0x28d8)
#define image_1_tcam_cmd_outer_tpid        (0x291c)
#define image_1_tcam_cmd_inner_tpid        (0x2934)
#define image_1_tcam_cmd_ethertype        (0x294c)
#define image_1_tcam_cmd_outer_vid        (0x2964)
#define image_1_tcam_cmd_inner_vid        (0x297c)
#define image_1_tcam_cmd_outer_pbit        (0x2994)
#define image_1_tcam_cmd_inner_pbit        (0x29ac)
#define image_1_tcam_cmd_vlan_num        (0x29c4)
#define image_1_tcam_cmd_ip_protocol        (0x29dc)
#define image_1_tcam_cmd_l3_protocol        (0x29f4)
#define image_1_tcam_cmd_tos        (0x2a0c)
#define image_1_tcam_cmd_network_layer        (0x2a24)
#define image_1_tcam_cmd_ingress_port        (0x2a44)
#define image_1_tcam_cmd_gem_flow        (0x2a5c)
#define image_1_tcam_cmd_ipv6_label        (0x2a74)
#define image_1_tcam_cmd_src_ip        (0x2aa0)
#define image_1_tcam_cmd_src_ipv6_masked        (0x2af4)
#define image_1_tcam_cmd_dst_ip        (0x2b28)
#define image_1_tcam_cmd_dst_ipv6_masked        (0x2b7c)
#define image_1_tcam_cmd_src_port        (0x2bb0)
#define image_1_tcam_cmd_dst_port        (0x2bc8)
#define image_1_tcam_cmd_src_mac        (0x2be0)
#define image_1_tcam_cmd_dst_mac        (0x2c0c)
#define image_1_tcam_cmd_generic_l2        (0x2c38)
#define image_1_tcam_cmd_generic_l3        (0x2c6c)
#define image_1_tcam_cmd_generic_l4        (0x2ca0)
#define image_1_gpe_cmd_replace_16        (0x3294)
#define image_1_gpe_cmd_replace_32        (0x329c)
#define image_1_gpe_cmd_replace_bits_16        (0x32b4)
#define image_1_gpe_cmd_copy_bits_16        (0x32d4)
#define image_1_gpe_cmd_skip_if        (0x3304)
#define image_1_gpe_vlan_action_cmd_drop        (0x3324)
#define image_1_gpe_vlan_action_cmd_dscp        (0x3328)
#define image_1_gpe_vlan_action_cmd_mac_hdr_copy        (0x3378)

#else

#define image_1_start_task_initialization_task        (0x2)
#define image_1_initialization_task        (0x2)
#define image_1_start_task_processing_wakeup_request        (0x2ca)
#define image_1_processing_wakeup_request        (0x2ca)
#define image_1_start_task_debug_routine        (0xee4)
#define image_1_debug_routine        (0xee4)
#define image_1_start_task_cpu_rx_wakeup_request        (0x1207)
#define image_1_cpu_rx_wakeup_request        (0x1207)
#define image_1_start_task_cpu_recycle_wakeup_request        (0x1384)
#define image_1_cpu_recycle_wakeup_request        (0x1384)
#define image_1_start_task_interrupt_coalescing_1st_wakeup_request        (0x13d9)
#define image_1_interrupt_coalescing_1st_wakeup_request        (0x13d9)
#define image_1_start_task_cpu_rx_meter_budget_allocator_1st_wakeup_request        (0x12d0)
#define image_1_cpu_rx_meter_budget_allocator_1st_wakeup_request        (0x12d0)
#define image_1_start_task_cpu_rx_copy_wakeup_request        (0xf27)
#define image_1_cpu_rx_copy_wakeup_request        (0xf27)
#define image_1_debug_routine_handler        (0x1)
#define image_1_tcam_cmd_ic_submit        (0xa36)
#define image_1_tcam_cmd_outer_tpid        (0xa47)
#define image_1_tcam_cmd_inner_tpid        (0xa4d)
#define image_1_tcam_cmd_ethertype        (0xa53)
#define image_1_tcam_cmd_outer_vid        (0xa59)
#define image_1_tcam_cmd_inner_vid        (0xa5f)
#define image_1_tcam_cmd_outer_pbit        (0xa65)
#define image_1_tcam_cmd_inner_pbit        (0xa6b)
#define image_1_tcam_cmd_vlan_num        (0xa71)
#define image_1_tcam_cmd_ip_protocol        (0xa77)
#define image_1_tcam_cmd_l3_protocol        (0xa7d)
#define image_1_tcam_cmd_tos        (0xa83)
#define image_1_tcam_cmd_network_layer        (0xa89)
#define image_1_tcam_cmd_ingress_port        (0xa91)
#define image_1_tcam_cmd_gem_flow        (0xa97)
#define image_1_tcam_cmd_ipv6_label        (0xa9d)
#define image_1_tcam_cmd_src_ip        (0xaa8)
#define image_1_tcam_cmd_src_ipv6_masked        (0xabd)
#define image_1_tcam_cmd_dst_ip        (0xaca)
#define image_1_tcam_cmd_dst_ipv6_masked        (0xadf)
#define image_1_tcam_cmd_src_port        (0xaec)
#define image_1_tcam_cmd_dst_port        (0xaf2)
#define image_1_tcam_cmd_src_mac        (0xaf8)
#define image_1_tcam_cmd_dst_mac        (0xb03)
#define image_1_tcam_cmd_generic_l2        (0xb0e)
#define image_1_tcam_cmd_generic_l3        (0xb1b)
#define image_1_tcam_cmd_generic_l4        (0xb28)
#define image_1_gpe_cmd_replace_16        (0xca5)
#define image_1_gpe_cmd_replace_32        (0xca7)
#define image_1_gpe_cmd_replace_bits_16        (0xcad)
#define image_1_gpe_cmd_copy_bits_16        (0xcb5)
#define image_1_gpe_cmd_skip_if        (0xcc1)
#define image_1_gpe_vlan_action_cmd_drop        (0xcc9)
#define image_1_gpe_vlan_action_cmd_dscp        (0xcca)
#define image_1_gpe_vlan_action_cmd_mac_hdr_copy        (0xcde)

#endif


#endif

/* IMAGE 2 LABELS */
#ifndef IMAGE_2_CODE_ADDRESSES
#define IMAGE_2_CODE_ADDRESSES

#ifndef PC_ADDRESS_INST_IND

#define image_2_start_task_initialization_task        (0x8)
#define image_2_initialization_task        (0x8)
#define image_2_start_task_processing_wakeup_request        (0xc34)
#define image_2_processing_wakeup_request        (0xc34)
#define image_2_start_task_cpu_tx_wakeup_request        (0x3c9c)
#define image_2_cpu_tx_wakeup_request        (0x3c9c)
#define image_2_cpu_tx_read_ring_indices        (0x3c9c)
#define image_2_start_task_debug_routine        (0x48)
#define image_2_debug_routine        (0x48)
#define image_2_start_task_cpu_recycle_wakeup_request        (0x43b8)
#define image_2_cpu_recycle_wakeup_request        (0x43b8)
#define image_2_start_task_interrupt_coalescing_1st_wakeup_request        (0x487c)
#define image_2_interrupt_coalescing_1st_wakeup_request        (0x487c)
#define image_2_start_task_timer_common_task_wakeup_request        (0x450c)
#define image_2_timer_common_task_wakeup_request        (0x450c)
#define image_2_debug_routine_handler        (0x4)
#define image_2_tcam_cmd_ic_submit        (0x29e4)
#define image_2_tcam_cmd_outer_tpid        (0x2a28)
#define image_2_tcam_cmd_inner_tpid        (0x2a40)
#define image_2_tcam_cmd_ethertype        (0x2a58)
#define image_2_tcam_cmd_outer_vid        (0x2a70)
#define image_2_tcam_cmd_inner_vid        (0x2a88)
#define image_2_tcam_cmd_outer_pbit        (0x2aa0)
#define image_2_tcam_cmd_inner_pbit        (0x2ab8)
#define image_2_tcam_cmd_vlan_num        (0x2ad0)
#define image_2_tcam_cmd_ip_protocol        (0x2ae8)
#define image_2_tcam_cmd_l3_protocol        (0x2b00)
#define image_2_tcam_cmd_tos        (0x2b18)
#define image_2_tcam_cmd_network_layer        (0x2b30)
#define image_2_tcam_cmd_ingress_port        (0x2b50)
#define image_2_tcam_cmd_gem_flow        (0x2b68)
#define image_2_tcam_cmd_ipv6_label        (0x2b80)
#define image_2_tcam_cmd_src_ip        (0x2bac)
#define image_2_tcam_cmd_src_ipv6_masked        (0x2c00)
#define image_2_tcam_cmd_dst_ip        (0x2c34)
#define image_2_tcam_cmd_dst_ipv6_masked        (0x2c88)
#define image_2_tcam_cmd_src_port        (0x2cbc)
#define image_2_tcam_cmd_dst_port        (0x2cd4)
#define image_2_tcam_cmd_src_mac        (0x2cec)
#define image_2_tcam_cmd_dst_mac        (0x2d18)
#define image_2_tcam_cmd_generic_l2        (0x2d44)
#define image_2_tcam_cmd_generic_l3        (0x2d78)
#define image_2_tcam_cmd_generic_l4        (0x2dac)
#define image_2_gpe_cmd_replace_16        (0x33a0)
#define image_2_gpe_cmd_replace_32        (0x33a8)
#define image_2_gpe_cmd_replace_bits_16        (0x33c0)
#define image_2_gpe_cmd_copy_bits_16        (0x33e0)
#define image_2_gpe_cmd_skip_if        (0x3410)
#define image_2_gpe_vlan_action_cmd_drop        (0x3430)
#define image_2_gpe_vlan_action_cmd_dscp        (0x3434)
#define image_2_gpe_vlan_action_cmd_mac_hdr_copy        (0x3484)

#else

#define image_2_start_task_initialization_task        (0x2)
#define image_2_initialization_task        (0x2)
#define image_2_start_task_processing_wakeup_request        (0x30d)
#define image_2_processing_wakeup_request        (0x30d)
#define image_2_start_task_cpu_tx_wakeup_request        (0xf27)
#define image_2_cpu_tx_wakeup_request        (0xf27)
#define image_2_cpu_tx_read_ring_indices        (0xf27)
#define image_2_start_task_debug_routine        (0x12)
#define image_2_debug_routine        (0x12)
#define image_2_start_task_cpu_recycle_wakeup_request        (0x10ee)
#define image_2_cpu_recycle_wakeup_request        (0x10ee)
#define image_2_start_task_interrupt_coalescing_1st_wakeup_request        (0x121f)
#define image_2_interrupt_coalescing_1st_wakeup_request        (0x121f)
#define image_2_start_task_timer_common_task_wakeup_request        (0x1143)
#define image_2_timer_common_task_wakeup_request        (0x1143)
#define image_2_debug_routine_handler        (0x1)
#define image_2_tcam_cmd_ic_submit        (0xa79)
#define image_2_tcam_cmd_outer_tpid        (0xa8a)
#define image_2_tcam_cmd_inner_tpid        (0xa90)
#define image_2_tcam_cmd_ethertype        (0xa96)
#define image_2_tcam_cmd_outer_vid        (0xa9c)
#define image_2_tcam_cmd_inner_vid        (0xaa2)
#define image_2_tcam_cmd_outer_pbit        (0xaa8)
#define image_2_tcam_cmd_inner_pbit        (0xaae)
#define image_2_tcam_cmd_vlan_num        (0xab4)
#define image_2_tcam_cmd_ip_protocol        (0xaba)
#define image_2_tcam_cmd_l3_protocol        (0xac0)
#define image_2_tcam_cmd_tos        (0xac6)
#define image_2_tcam_cmd_network_layer        (0xacc)
#define image_2_tcam_cmd_ingress_port        (0xad4)
#define image_2_tcam_cmd_gem_flow        (0xada)
#define image_2_tcam_cmd_ipv6_label        (0xae0)
#define image_2_tcam_cmd_src_ip        (0xaeb)
#define image_2_tcam_cmd_src_ipv6_masked        (0xb00)
#define image_2_tcam_cmd_dst_ip        (0xb0d)
#define image_2_tcam_cmd_dst_ipv6_masked        (0xb22)
#define image_2_tcam_cmd_src_port        (0xb2f)
#define image_2_tcam_cmd_dst_port        (0xb35)
#define image_2_tcam_cmd_src_mac        (0xb3b)
#define image_2_tcam_cmd_dst_mac        (0xb46)
#define image_2_tcam_cmd_generic_l2        (0xb51)
#define image_2_tcam_cmd_generic_l3        (0xb5e)
#define image_2_tcam_cmd_generic_l4        (0xb6b)
#define image_2_gpe_cmd_replace_16        (0xce8)
#define image_2_gpe_cmd_replace_32        (0xcea)
#define image_2_gpe_cmd_replace_bits_16        (0xcf0)
#define image_2_gpe_cmd_copy_bits_16        (0xcf8)
#define image_2_gpe_cmd_skip_if        (0xd04)
#define image_2_gpe_vlan_action_cmd_drop        (0xd0c)
#define image_2_gpe_vlan_action_cmd_dscp        (0xd0d)
#define image_2_gpe_vlan_action_cmd_mac_hdr_copy        (0xd21)

#endif


#endif

/* IMAGE 3 LABELS */
#ifndef IMAGE_3_CODE_ADDRESSES
#define IMAGE_3_CODE_ADDRESSES

#ifndef PC_ADDRESS_INST_IND

#define image_3_start_task_initialization_task        (0x8)
#define image_3_initialization_task        (0x8)
#define image_3_start_task_us_tx_task_1st_wakeup_request        (0xd68)
#define image_3_us_tx_task_1st_wakeup_request        (0xd68)
#define image_3_us_tx_task_wakeup_request        (0xd68)
#define image_3_start_task_epon_update_fifo_read_1st_wakeup_request        (0x1f70)
#define image_3_epon_update_fifo_read_1st_wakeup_request        (0x1f70)
#define image_3_start_task_epon_tx_task_wakeup_request        (0x145c)
#define image_3_epon_tx_task_wakeup_request        (0x145c)
#define image_3_start_task_budget_allocator_1st_wakeup_request        (0x1610)
#define image_3_budget_allocator_1st_wakeup_request        (0x1610)
#define image_3_start_task_debug_routine        (0x48)
#define image_3_debug_routine        (0x48)
#define image_3_start_task_gpon_control_wakeup_request        (0x5ac)
#define image_3_gpon_control_wakeup_request        (0x5ac)
#define image_3_start_task_ovl_budget_allocator_1st_wakeup_request        (0x1974)
#define image_3_ovl_budget_allocator_1st_wakeup_request        (0x1974)
#define image_3_start_task_flush_task_1st_wakeup_request        (0x1b24)
#define image_3_flush_task_1st_wakeup_request        (0x1b24)
#define image_3_start_task_update_fifo_us_read_1st_wakeup_request        (0x1ca0)
#define image_3_update_fifo_us_read_1st_wakeup_request        (0x1ca0)
#define image_3_debug_routine_handler        (0x4)

#else

#define image_3_start_task_initialization_task        (0x2)
#define image_3_initialization_task        (0x2)
#define image_3_start_task_us_tx_task_1st_wakeup_request        (0x35a)
#define image_3_us_tx_task_1st_wakeup_request        (0x35a)
#define image_3_us_tx_task_wakeup_request        (0x35a)
#define image_3_start_task_epon_update_fifo_read_1st_wakeup_request        (0x7dc)
#define image_3_epon_update_fifo_read_1st_wakeup_request        (0x7dc)
#define image_3_start_task_epon_tx_task_wakeup_request        (0x517)
#define image_3_epon_tx_task_wakeup_request        (0x517)
#define image_3_start_task_budget_allocator_1st_wakeup_request        (0x584)
#define image_3_budget_allocator_1st_wakeup_request        (0x584)
#define image_3_start_task_debug_routine        (0x12)
#define image_3_debug_routine        (0x12)
#define image_3_start_task_gpon_control_wakeup_request        (0x16b)
#define image_3_gpon_control_wakeup_request        (0x16b)
#define image_3_start_task_ovl_budget_allocator_1st_wakeup_request        (0x65d)
#define image_3_ovl_budget_allocator_1st_wakeup_request        (0x65d)
#define image_3_start_task_flush_task_1st_wakeup_request        (0x6c9)
#define image_3_flush_task_1st_wakeup_request        (0x6c9)
#define image_3_start_task_update_fifo_us_read_1st_wakeup_request        (0x728)
#define image_3_update_fifo_us_read_1st_wakeup_request        (0x728)
#define image_3_debug_routine_handler        (0x1)

#endif


#endif

/* IMAGE 4 LABELS */
#ifndef IMAGE_4_CODE_ADDRESSES
#define IMAGE_4_CODE_ADDRESSES

#ifndef PC_ADDRESS_INST_IND

#define image_4_start_task_initialization_task        (0x8)
#define image_4_initialization_task        (0x8)
#define image_4_start_task_processing_wakeup_request        (0xc34)
#define image_4_processing_wakeup_request        (0xc34)
#define image_4_start_task_debug_routine        (0x48)
#define image_4_debug_routine        (0x48)
#define image_4_debug_routine_handler        (0x4)
#define image_4_tcam_cmd_ic_submit        (0x29e4)
#define image_4_tcam_cmd_outer_tpid        (0x2a28)
#define image_4_tcam_cmd_inner_tpid        (0x2a40)
#define image_4_tcam_cmd_ethertype        (0x2a58)
#define image_4_tcam_cmd_outer_vid        (0x2a70)
#define image_4_tcam_cmd_inner_vid        (0x2a88)
#define image_4_tcam_cmd_outer_pbit        (0x2aa0)
#define image_4_tcam_cmd_inner_pbit        (0x2ab8)
#define image_4_tcam_cmd_vlan_num        (0x2ad0)
#define image_4_tcam_cmd_ip_protocol        (0x2ae8)
#define image_4_tcam_cmd_l3_protocol        (0x2b00)
#define image_4_tcam_cmd_tos        (0x2b18)
#define image_4_tcam_cmd_network_layer        (0x2b30)
#define image_4_tcam_cmd_ingress_port        (0x2b50)
#define image_4_tcam_cmd_gem_flow        (0x2b68)
#define image_4_tcam_cmd_ipv6_label        (0x2b80)
#define image_4_tcam_cmd_src_ip        (0x2bac)
#define image_4_tcam_cmd_src_ipv6_masked        (0x2c00)
#define image_4_tcam_cmd_dst_ip        (0x2c34)
#define image_4_tcam_cmd_dst_ipv6_masked        (0x2c88)
#define image_4_tcam_cmd_src_port        (0x2cbc)
#define image_4_tcam_cmd_dst_port        (0x2cd4)
#define image_4_tcam_cmd_src_mac        (0x2cec)
#define image_4_tcam_cmd_dst_mac        (0x2d18)
#define image_4_tcam_cmd_generic_l2        (0x2d44)
#define image_4_tcam_cmd_generic_l3        (0x2d78)
#define image_4_tcam_cmd_generic_l4        (0x2dac)
#define image_4_gpe_cmd_replace_16        (0x33a0)
#define image_4_gpe_cmd_replace_32        (0x33a8)
#define image_4_gpe_cmd_replace_bits_16        (0x33c0)
#define image_4_gpe_cmd_copy_bits_16        (0x33e0)
#define image_4_gpe_cmd_skip_if        (0x3410)
#define image_4_gpe_vlan_action_cmd_drop        (0x3430)
#define image_4_gpe_vlan_action_cmd_dscp        (0x3434)
#define image_4_gpe_vlan_action_cmd_mac_hdr_copy        (0x3484)

#else

#define image_4_start_task_initialization_task        (0x2)
#define image_4_initialization_task        (0x2)
#define image_4_start_task_processing_wakeup_request        (0x30d)
#define image_4_processing_wakeup_request        (0x30d)
#define image_4_start_task_debug_routine        (0x12)
#define image_4_debug_routine        (0x12)
#define image_4_debug_routine_handler        (0x1)
#define image_4_tcam_cmd_ic_submit        (0xa79)
#define image_4_tcam_cmd_outer_tpid        (0xa8a)
#define image_4_tcam_cmd_inner_tpid        (0xa90)
#define image_4_tcam_cmd_ethertype        (0xa96)
#define image_4_tcam_cmd_outer_vid        (0xa9c)
#define image_4_tcam_cmd_inner_vid        (0xaa2)
#define image_4_tcam_cmd_outer_pbit        (0xaa8)
#define image_4_tcam_cmd_inner_pbit        (0xaae)
#define image_4_tcam_cmd_vlan_num        (0xab4)
#define image_4_tcam_cmd_ip_protocol        (0xaba)
#define image_4_tcam_cmd_l3_protocol        (0xac0)
#define image_4_tcam_cmd_tos        (0xac6)
#define image_4_tcam_cmd_network_layer        (0xacc)
#define image_4_tcam_cmd_ingress_port        (0xad4)
#define image_4_tcam_cmd_gem_flow        (0xada)
#define image_4_tcam_cmd_ipv6_label        (0xae0)
#define image_4_tcam_cmd_src_ip        (0xaeb)
#define image_4_tcam_cmd_src_ipv6_masked        (0xb00)
#define image_4_tcam_cmd_dst_ip        (0xb0d)
#define image_4_tcam_cmd_dst_ipv6_masked        (0xb22)
#define image_4_tcam_cmd_src_port        (0xb2f)
#define image_4_tcam_cmd_dst_port        (0xb35)
#define image_4_tcam_cmd_src_mac        (0xb3b)
#define image_4_tcam_cmd_dst_mac        (0xb46)
#define image_4_tcam_cmd_generic_l2        (0xb51)
#define image_4_tcam_cmd_generic_l3        (0xb5e)
#define image_4_tcam_cmd_generic_l4        (0xb6b)
#define image_4_gpe_cmd_replace_16        (0xce8)
#define image_4_gpe_cmd_replace_32        (0xcea)
#define image_4_gpe_cmd_replace_bits_16        (0xcf0)
#define image_4_gpe_cmd_copy_bits_16        (0xcf8)
#define image_4_gpe_cmd_skip_if        (0xd04)
#define image_4_gpe_vlan_action_cmd_drop        (0xd0c)
#define image_4_gpe_vlan_action_cmd_dscp        (0xd0d)
#define image_4_gpe_vlan_action_cmd_mac_hdr_copy        (0xd21)

#endif


#endif

/* IMAGE 5 LABELS */
#ifndef IMAGE_5_CODE_ADDRESSES
#define IMAGE_5_CODE_ADDRESSES

#ifndef PC_ADDRESS_INST_IND

#define image_5_start_task_initialization_task        (0x8)
#define image_5_initialization_task        (0x8)
#define image_5_start_task_processing_wakeup_request        (0xd90)
#define image_5_processing_wakeup_request        (0xd90)
#define image_5_start_task_budget_allocator_1st_wakeup_request        (0x474c)
#define image_5_budget_allocator_1st_wakeup_request        (0x474c)
#define image_5_start_task_debug_routine        (0x3df8)
#define image_5_debug_routine        (0x3df8)
#define image_5_start_task_service_queues_update_fifo_read_1st_wakeup_request        (0x4114)
#define image_5_service_queues_update_fifo_read_1st_wakeup_request        (0x4114)
#define image_5_start_task_service_queues_tx_task_wakeup_request        (0x43bc)
#define image_5_service_queues_tx_task_wakeup_request        (0x43bc)
#define image_5_debug_routine_handler        (0x4)
#define image_5_tcam_cmd_ic_submit        (0x2b40)
#define image_5_tcam_cmd_outer_tpid        (0x2b84)
#define image_5_tcam_cmd_inner_tpid        (0x2b9c)
#define image_5_tcam_cmd_ethertype        (0x2bb4)
#define image_5_tcam_cmd_outer_vid        (0x2bcc)
#define image_5_tcam_cmd_inner_vid        (0x2be4)
#define image_5_tcam_cmd_outer_pbit        (0x2bfc)
#define image_5_tcam_cmd_inner_pbit        (0x2c14)
#define image_5_tcam_cmd_vlan_num        (0x2c2c)
#define image_5_tcam_cmd_ip_protocol        (0x2c44)
#define image_5_tcam_cmd_l3_protocol        (0x2c5c)
#define image_5_tcam_cmd_tos        (0x2c74)
#define image_5_tcam_cmd_network_layer        (0x2c8c)
#define image_5_tcam_cmd_ingress_port        (0x2cac)
#define image_5_tcam_cmd_gem_flow        (0x2cc4)
#define image_5_tcam_cmd_ipv6_label        (0x2cdc)
#define image_5_tcam_cmd_src_ip        (0x2d08)
#define image_5_tcam_cmd_src_ipv6_masked        (0x2d5c)
#define image_5_tcam_cmd_dst_ip        (0x2d90)
#define image_5_tcam_cmd_dst_ipv6_masked        (0x2de4)
#define image_5_tcam_cmd_src_port        (0x2e18)
#define image_5_tcam_cmd_dst_port        (0x2e30)
#define image_5_tcam_cmd_src_mac        (0x2e48)
#define image_5_tcam_cmd_dst_mac        (0x2e74)
#define image_5_tcam_cmd_generic_l2        (0x2ea0)
#define image_5_tcam_cmd_generic_l3        (0x2ed4)
#define image_5_tcam_cmd_generic_l4        (0x2f08)
#define image_5_gpe_cmd_replace_16        (0x34fc)
#define image_5_gpe_cmd_replace_32        (0x3504)
#define image_5_gpe_cmd_replace_bits_16        (0x351c)
#define image_5_gpe_cmd_copy_bits_16        (0x353c)
#define image_5_gpe_cmd_skip_if        (0x356c)
#define image_5_gpe_vlan_action_cmd_drop        (0x358c)
#define image_5_gpe_vlan_action_cmd_dscp        (0x3590)
#define image_5_gpe_vlan_action_cmd_mac_hdr_copy        (0x35e0)

#else

#define image_5_start_task_initialization_task        (0x2)
#define image_5_initialization_task        (0x2)
#define image_5_start_task_processing_wakeup_request        (0x364)
#define image_5_processing_wakeup_request        (0x364)
#define image_5_start_task_budget_allocator_1st_wakeup_request        (0x11d3)
#define image_5_budget_allocator_1st_wakeup_request        (0x11d3)
#define image_5_start_task_debug_routine        (0xf7e)
#define image_5_debug_routine        (0xf7e)
#define image_5_start_task_service_queues_update_fifo_read_1st_wakeup_request        (0x1045)
#define image_5_service_queues_update_fifo_read_1st_wakeup_request        (0x1045)
#define image_5_start_task_service_queues_tx_task_wakeup_request        (0x10ef)
#define image_5_service_queues_tx_task_wakeup_request        (0x10ef)
#define image_5_debug_routine_handler        (0x1)
#define image_5_tcam_cmd_ic_submit        (0xad0)
#define image_5_tcam_cmd_outer_tpid        (0xae1)
#define image_5_tcam_cmd_inner_tpid        (0xae7)
#define image_5_tcam_cmd_ethertype        (0xaed)
#define image_5_tcam_cmd_outer_vid        (0xaf3)
#define image_5_tcam_cmd_inner_vid        (0xaf9)
#define image_5_tcam_cmd_outer_pbit        (0xaff)
#define image_5_tcam_cmd_inner_pbit        (0xb05)
#define image_5_tcam_cmd_vlan_num        (0xb0b)
#define image_5_tcam_cmd_ip_protocol        (0xb11)
#define image_5_tcam_cmd_l3_protocol        (0xb17)
#define image_5_tcam_cmd_tos        (0xb1d)
#define image_5_tcam_cmd_network_layer        (0xb23)
#define image_5_tcam_cmd_ingress_port        (0xb2b)
#define image_5_tcam_cmd_gem_flow        (0xb31)
#define image_5_tcam_cmd_ipv6_label        (0xb37)
#define image_5_tcam_cmd_src_ip        (0xb42)
#define image_5_tcam_cmd_src_ipv6_masked        (0xb57)
#define image_5_tcam_cmd_dst_ip        (0xb64)
#define image_5_tcam_cmd_dst_ipv6_masked        (0xb79)
#define image_5_tcam_cmd_src_port        (0xb86)
#define image_5_tcam_cmd_dst_port        (0xb8c)
#define image_5_tcam_cmd_src_mac        (0xb92)
#define image_5_tcam_cmd_dst_mac        (0xb9d)
#define image_5_tcam_cmd_generic_l2        (0xba8)
#define image_5_tcam_cmd_generic_l3        (0xbb5)
#define image_5_tcam_cmd_generic_l4        (0xbc2)
#define image_5_gpe_cmd_replace_16        (0xd3f)
#define image_5_gpe_cmd_replace_32        (0xd41)
#define image_5_gpe_cmd_replace_bits_16        (0xd47)
#define image_5_gpe_cmd_copy_bits_16        (0xd4f)
#define image_5_gpe_cmd_skip_if        (0xd5b)
#define image_5_gpe_vlan_action_cmd_drop        (0xd63)
#define image_5_gpe_vlan_action_cmd_dscp        (0xd64)
#define image_5_gpe_vlan_action_cmd_mac_hdr_copy        (0xd78)

#endif


#endif

/* IMAGE 6 LABELS */
#ifndef IMAGE_6_CODE_ADDRESSES
#define IMAGE_6_CODE_ADDRESSES

#ifndef PC_ADDRESS_INST_IND

#define image_6_start_task_initialization_task        (0x8)
#define image_6_initialization_task        (0x8)
#define image_6_start_task_processing_wakeup_request        (0xb28)
#define image_6_processing_wakeup_request        (0xb28)
#define image_6_start_task_debug_routine        (0x3b90)
#define image_6_debug_routine        (0x3b90)
#define image_6_debug_routine_handler        (0x4)
#define image_6_tcam_cmd_ic_submit        (0x28d8)
#define image_6_tcam_cmd_outer_tpid        (0x291c)
#define image_6_tcam_cmd_inner_tpid        (0x2934)
#define image_6_tcam_cmd_ethertype        (0x294c)
#define image_6_tcam_cmd_outer_vid        (0x2964)
#define image_6_tcam_cmd_inner_vid        (0x297c)
#define image_6_tcam_cmd_outer_pbit        (0x2994)
#define image_6_tcam_cmd_inner_pbit        (0x29ac)
#define image_6_tcam_cmd_vlan_num        (0x29c4)
#define image_6_tcam_cmd_ip_protocol        (0x29dc)
#define image_6_tcam_cmd_l3_protocol        (0x29f4)
#define image_6_tcam_cmd_tos        (0x2a0c)
#define image_6_tcam_cmd_network_layer        (0x2a24)
#define image_6_tcam_cmd_ingress_port        (0x2a44)
#define image_6_tcam_cmd_gem_flow        (0x2a5c)
#define image_6_tcam_cmd_ipv6_label        (0x2a74)
#define image_6_tcam_cmd_src_ip        (0x2aa0)
#define image_6_tcam_cmd_src_ipv6_masked        (0x2af4)
#define image_6_tcam_cmd_dst_ip        (0x2b28)
#define image_6_tcam_cmd_dst_ipv6_masked        (0x2b7c)
#define image_6_tcam_cmd_src_port        (0x2bb0)
#define image_6_tcam_cmd_dst_port        (0x2bc8)
#define image_6_tcam_cmd_src_mac        (0x2be0)
#define image_6_tcam_cmd_dst_mac        (0x2c0c)
#define image_6_tcam_cmd_generic_l2        (0x2c38)
#define image_6_tcam_cmd_generic_l3        (0x2c6c)
#define image_6_tcam_cmd_generic_l4        (0x2ca0)
#define image_6_gpe_cmd_replace_16        (0x3294)
#define image_6_gpe_cmd_replace_32        (0x329c)
#define image_6_gpe_cmd_replace_bits_16        (0x32b4)
#define image_6_gpe_cmd_copy_bits_16        (0x32d4)
#define image_6_gpe_cmd_skip_if        (0x3304)
#define image_6_gpe_vlan_action_cmd_drop        (0x3324)
#define image_6_gpe_vlan_action_cmd_dscp        (0x3328)
#define image_6_gpe_vlan_action_cmd_mac_hdr_copy        (0x3378)

#else

#define image_6_start_task_initialization_task        (0x2)
#define image_6_initialization_task        (0x2)
#define image_6_start_task_processing_wakeup_request        (0x2ca)
#define image_6_processing_wakeup_request        (0x2ca)
#define image_6_start_task_debug_routine        (0xee4)
#define image_6_debug_routine        (0xee4)
#define image_6_debug_routine_handler        (0x1)
#define image_6_tcam_cmd_ic_submit        (0xa36)
#define image_6_tcam_cmd_outer_tpid        (0xa47)
#define image_6_tcam_cmd_inner_tpid        (0xa4d)
#define image_6_tcam_cmd_ethertype        (0xa53)
#define image_6_tcam_cmd_outer_vid        (0xa59)
#define image_6_tcam_cmd_inner_vid        (0xa5f)
#define image_6_tcam_cmd_outer_pbit        (0xa65)
#define image_6_tcam_cmd_inner_pbit        (0xa6b)
#define image_6_tcam_cmd_vlan_num        (0xa71)
#define image_6_tcam_cmd_ip_protocol        (0xa77)
#define image_6_tcam_cmd_l3_protocol        (0xa7d)
#define image_6_tcam_cmd_tos        (0xa83)
#define image_6_tcam_cmd_network_layer        (0xa89)
#define image_6_tcam_cmd_ingress_port        (0xa91)
#define image_6_tcam_cmd_gem_flow        (0xa97)
#define image_6_tcam_cmd_ipv6_label        (0xa9d)
#define image_6_tcam_cmd_src_ip        (0xaa8)
#define image_6_tcam_cmd_src_ipv6_masked        (0xabd)
#define image_6_tcam_cmd_dst_ip        (0xaca)
#define image_6_tcam_cmd_dst_ipv6_masked        (0xadf)
#define image_6_tcam_cmd_src_port        (0xaec)
#define image_6_tcam_cmd_dst_port        (0xaf2)
#define image_6_tcam_cmd_src_mac        (0xaf8)
#define image_6_tcam_cmd_dst_mac        (0xb03)
#define image_6_tcam_cmd_generic_l2        (0xb0e)
#define image_6_tcam_cmd_generic_l3        (0xb1b)
#define image_6_tcam_cmd_generic_l4        (0xb28)
#define image_6_gpe_cmd_replace_16        (0xca5)
#define image_6_gpe_cmd_replace_32        (0xca7)
#define image_6_gpe_cmd_replace_bits_16        (0xcad)
#define image_6_gpe_cmd_copy_bits_16        (0xcb5)
#define image_6_gpe_cmd_skip_if        (0xcc1)
#define image_6_gpe_vlan_action_cmd_drop        (0xcc9)
#define image_6_gpe_vlan_action_cmd_dscp        (0xcca)
#define image_6_gpe_vlan_action_cmd_mac_hdr_copy        (0xcde)

#endif


#endif

/* IMAGE 7 LABELS */
#ifndef IMAGE_7_CODE_ADDRESSES
#define IMAGE_7_CODE_ADDRESSES

#ifndef PC_ADDRESS_INST_IND

#define image_7_start_task_initialization_task        (0x8)
#define image_7_initialization_task        (0x8)
#define image_7_start_task_processing_wakeup_request        (0xb28)
#define image_7_processing_wakeup_request        (0xb28)
#define image_7_start_task_debug_routine        (0x3b90)
#define image_7_debug_routine        (0x3b90)
#define image_7_debug_routine_handler        (0x4)
#define image_7_tcam_cmd_ic_submit        (0x28d8)
#define image_7_tcam_cmd_outer_tpid        (0x291c)
#define image_7_tcam_cmd_inner_tpid        (0x2934)
#define image_7_tcam_cmd_ethertype        (0x294c)
#define image_7_tcam_cmd_outer_vid        (0x2964)
#define image_7_tcam_cmd_inner_vid        (0x297c)
#define image_7_tcam_cmd_outer_pbit        (0x2994)
#define image_7_tcam_cmd_inner_pbit        (0x29ac)
#define image_7_tcam_cmd_vlan_num        (0x29c4)
#define image_7_tcam_cmd_ip_protocol        (0x29dc)
#define image_7_tcam_cmd_l3_protocol        (0x29f4)
#define image_7_tcam_cmd_tos        (0x2a0c)
#define image_7_tcam_cmd_network_layer        (0x2a24)
#define image_7_tcam_cmd_ingress_port        (0x2a44)
#define image_7_tcam_cmd_gem_flow        (0x2a5c)
#define image_7_tcam_cmd_ipv6_label        (0x2a74)
#define image_7_tcam_cmd_src_ip        (0x2aa0)
#define image_7_tcam_cmd_src_ipv6_masked        (0x2af4)
#define image_7_tcam_cmd_dst_ip        (0x2b28)
#define image_7_tcam_cmd_dst_ipv6_masked        (0x2b7c)
#define image_7_tcam_cmd_src_port        (0x2bb0)
#define image_7_tcam_cmd_dst_port        (0x2bc8)
#define image_7_tcam_cmd_src_mac        (0x2be0)
#define image_7_tcam_cmd_dst_mac        (0x2c0c)
#define image_7_tcam_cmd_generic_l2        (0x2c38)
#define image_7_tcam_cmd_generic_l3        (0x2c6c)
#define image_7_tcam_cmd_generic_l4        (0x2ca0)
#define image_7_gpe_cmd_replace_16        (0x3294)
#define image_7_gpe_cmd_replace_32        (0x329c)
#define image_7_gpe_cmd_replace_bits_16        (0x32b4)
#define image_7_gpe_cmd_copy_bits_16        (0x32d4)
#define image_7_gpe_cmd_skip_if        (0x3304)
#define image_7_gpe_vlan_action_cmd_drop        (0x3324)
#define image_7_gpe_vlan_action_cmd_dscp        (0x3328)
#define image_7_gpe_vlan_action_cmd_mac_hdr_copy        (0x3378)

#else

#define image_7_start_task_initialization_task        (0x2)
#define image_7_initialization_task        (0x2)
#define image_7_start_task_processing_wakeup_request        (0x2ca)
#define image_7_processing_wakeup_request        (0x2ca)
#define image_7_start_task_debug_routine        (0xee4)
#define image_7_debug_routine        (0xee4)
#define image_7_debug_routine_handler        (0x1)
#define image_7_tcam_cmd_ic_submit        (0xa36)
#define image_7_tcam_cmd_outer_tpid        (0xa47)
#define image_7_tcam_cmd_inner_tpid        (0xa4d)
#define image_7_tcam_cmd_ethertype        (0xa53)
#define image_7_tcam_cmd_outer_vid        (0xa59)
#define image_7_tcam_cmd_inner_vid        (0xa5f)
#define image_7_tcam_cmd_outer_pbit        (0xa65)
#define image_7_tcam_cmd_inner_pbit        (0xa6b)
#define image_7_tcam_cmd_vlan_num        (0xa71)
#define image_7_tcam_cmd_ip_protocol        (0xa77)
#define image_7_tcam_cmd_l3_protocol        (0xa7d)
#define image_7_tcam_cmd_tos        (0xa83)
#define image_7_tcam_cmd_network_layer        (0xa89)
#define image_7_tcam_cmd_ingress_port        (0xa91)
#define image_7_tcam_cmd_gem_flow        (0xa97)
#define image_7_tcam_cmd_ipv6_label        (0xa9d)
#define image_7_tcam_cmd_src_ip        (0xaa8)
#define image_7_tcam_cmd_src_ipv6_masked        (0xabd)
#define image_7_tcam_cmd_dst_ip        (0xaca)
#define image_7_tcam_cmd_dst_ipv6_masked        (0xadf)
#define image_7_tcam_cmd_src_port        (0xaec)
#define image_7_tcam_cmd_dst_port        (0xaf2)
#define image_7_tcam_cmd_src_mac        (0xaf8)
#define image_7_tcam_cmd_dst_mac        (0xb03)
#define image_7_tcam_cmd_generic_l2        (0xb0e)
#define image_7_tcam_cmd_generic_l3        (0xb1b)
#define image_7_tcam_cmd_generic_l4        (0xb28)
#define image_7_gpe_cmd_replace_16        (0xca5)
#define image_7_gpe_cmd_replace_32        (0xca7)
#define image_7_gpe_cmd_replace_bits_16        (0xcad)
#define image_7_gpe_cmd_copy_bits_16        (0xcb5)
#define image_7_gpe_cmd_skip_if        (0xcc1)
#define image_7_gpe_vlan_action_cmd_drop        (0xcc9)
#define image_7_gpe_vlan_action_cmd_dscp        (0xcca)
#define image_7_gpe_vlan_action_cmd_mac_hdr_copy        (0xcde)

#endif


#endif

/* COMMON LABELS */
#ifndef COMMON_CODE_ADDRESSES
#define COMMON_CODE_ADDRESSES

#define INVALID_LABEL_ADDRESS 0xFFFFFF

#ifndef PC_ADDRESS_INST_IND

#define TCAM_CMD_DST_IP_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x2b28, 0x2c34, INVALID_LABEL_ADDRESS, 0x2c34, 0x2d90, 0x2b28, 0x2b28}
#define TCAM_CMD_DST_IPV6_MASKED_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x2b7c, 0x2c88, INVALID_LABEL_ADDRESS, 0x2c88, 0x2de4, 0x2b7c, 0x2b7c}
#define TCAM_CMD_DST_MAC_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x2c0c, 0x2d18, INVALID_LABEL_ADDRESS, 0x2d18, 0x2e74, 0x2c0c, 0x2c0c}
#define TCAM_CMD_DST_PORT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x2bc8, 0x2cd4, INVALID_LABEL_ADDRESS, 0x2cd4, 0x2e30, 0x2bc8, 0x2bc8}
#define TCAM_CMD_ETHERTYPE_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x294c, 0x2a58, INVALID_LABEL_ADDRESS, 0x2a58, 0x2bb4, 0x294c, 0x294c}
#define TCAM_CMD_GEM_FLOW_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x2a5c, 0x2b68, INVALID_LABEL_ADDRESS, 0x2b68, 0x2cc4, 0x2a5c, 0x2a5c}
#define TCAM_CMD_GENERIC_L2_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x2c38, 0x2d44, INVALID_LABEL_ADDRESS, 0x2d44, 0x2ea0, 0x2c38, 0x2c38}
#define TCAM_CMD_GENERIC_L3_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x2c6c, 0x2d78, INVALID_LABEL_ADDRESS, 0x2d78, 0x2ed4, 0x2c6c, 0x2c6c}
#define TCAM_CMD_GENERIC_L4_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x2ca0, 0x2dac, INVALID_LABEL_ADDRESS, 0x2dac, 0x2f08, 0x2ca0, 0x2ca0}
#define TCAM_CMD_IC_SUBMIT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x28d8, 0x29e4, INVALID_LABEL_ADDRESS, 0x29e4, 0x2b40, 0x28d8, 0x28d8}
#define TCAM_CMD_INGRESS_PORT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x2a44, 0x2b50, INVALID_LABEL_ADDRESS, 0x2b50, 0x2cac, 0x2a44, 0x2a44}
#define TCAM_CMD_INNER_PBIT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x29ac, 0x2ab8, INVALID_LABEL_ADDRESS, 0x2ab8, 0x2c14, 0x29ac, 0x29ac}
#define TCAM_CMD_INNER_TPID_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x2934, 0x2a40, INVALID_LABEL_ADDRESS, 0x2a40, 0x2b9c, 0x2934, 0x2934}
#define TCAM_CMD_INNER_VID_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x297c, 0x2a88, INVALID_LABEL_ADDRESS, 0x2a88, 0x2be4, 0x297c, 0x297c}
#define TCAM_CMD_IP_PROTOCOL_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x29dc, 0x2ae8, INVALID_LABEL_ADDRESS, 0x2ae8, 0x2c44, 0x29dc, 0x29dc}
#define TCAM_CMD_IPV6_LABEL_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x2a74, 0x2b80, INVALID_LABEL_ADDRESS, 0x2b80, 0x2cdc, 0x2a74, 0x2a74}
#define TCAM_CMD_L3_PROTOCOL_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x29f4, 0x2b00, INVALID_LABEL_ADDRESS, 0x2b00, 0x2c5c, 0x29f4, 0x29f4}
#define TCAM_CMD_NETWORK_LAYER_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x2a24, 0x2b30, INVALID_LABEL_ADDRESS, 0x2b30, 0x2c8c, 0x2a24, 0x2a24}
#define TCAM_CMD_OUTER_PBIT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x2994, 0x2aa0, INVALID_LABEL_ADDRESS, 0x2aa0, 0x2bfc, 0x2994, 0x2994}
#define TCAM_CMD_OUTER_TPID_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x291c, 0x2a28, INVALID_LABEL_ADDRESS, 0x2a28, 0x2b84, 0x291c, 0x291c}
#define TCAM_CMD_OUTER_VID_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x2964, 0x2a70, INVALID_LABEL_ADDRESS, 0x2a70, 0x2bcc, 0x2964, 0x2964}
#define TCAM_CMD_SRC_IP_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x2aa0, 0x2bac, INVALID_LABEL_ADDRESS, 0x2bac, 0x2d08, 0x2aa0, 0x2aa0}
#define TCAM_CMD_SRC_IPV6_MASKED_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x2af4, 0x2c00, INVALID_LABEL_ADDRESS, 0x2c00, 0x2d5c, 0x2af4, 0x2af4}
#define TCAM_CMD_SRC_MAC_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x2be0, 0x2cec, INVALID_LABEL_ADDRESS, 0x2cec, 0x2e48, 0x2be0, 0x2be0}
#define TCAM_CMD_SRC_PORT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x2bb0, 0x2cbc, INVALID_LABEL_ADDRESS, 0x2cbc, 0x2e18, 0x2bb0, 0x2bb0}
#define TCAM_CMD_TOS_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x2a0c, 0x2b18, INVALID_LABEL_ADDRESS, 0x2b18, 0x2c74, 0x2a0c, 0x2a0c}
#define TCAM_CMD_VLAN_NUM_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x29c4, 0x2ad0, INVALID_LABEL_ADDRESS, 0x2ad0, 0x2c2c, 0x29c4, 0x29c4}

#else

#define TCAM_CMD_DST_IP_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xaca, 0xb0d, INVALID_LABEL_ADDRESS, 0xb0d, 0xb64, 0xaca, 0xaca}
#define TCAM_CMD_DST_IPV6_MASKED_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xadf, 0xb22, INVALID_LABEL_ADDRESS, 0xb22, 0xb79, 0xadf, 0xadf}
#define TCAM_CMD_DST_MAC_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xb03, 0xb46, INVALID_LABEL_ADDRESS, 0xb46, 0xb9d, 0xb03, 0xb03}
#define TCAM_CMD_DST_PORT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xaf2, 0xb35, INVALID_LABEL_ADDRESS, 0xb35, 0xb8c, 0xaf2, 0xaf2}
#define TCAM_CMD_ETHERTYPE_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xa53, 0xa96, INVALID_LABEL_ADDRESS, 0xa96, 0xaed, 0xa53, 0xa53}
#define TCAM_CMD_GEM_FLOW_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xa97, 0xada, INVALID_LABEL_ADDRESS, 0xada, 0xb31, 0xa97, 0xa97}
#define TCAM_CMD_GENERIC_L2_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xb0e, 0xb51, INVALID_LABEL_ADDRESS, 0xb51, 0xba8, 0xb0e, 0xb0e}
#define TCAM_CMD_GENERIC_L3_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xb1b, 0xb5e, INVALID_LABEL_ADDRESS, 0xb5e, 0xbb5, 0xb1b, 0xb1b}
#define TCAM_CMD_GENERIC_L4_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xb28, 0xb6b, INVALID_LABEL_ADDRESS, 0xb6b, 0xbc2, 0xb28, 0xb28}
#define TCAM_CMD_IC_SUBMIT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xa36, 0xa79, INVALID_LABEL_ADDRESS, 0xa79, 0xad0, 0xa36, 0xa36}
#define TCAM_CMD_INGRESS_PORT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xa91, 0xad4, INVALID_LABEL_ADDRESS, 0xad4, 0xb2b, 0xa91, 0xa91}
#define TCAM_CMD_INNER_PBIT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xa6b, 0xaae, INVALID_LABEL_ADDRESS, 0xaae, 0xb05, 0xa6b, 0xa6b}
#define TCAM_CMD_INNER_TPID_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xa4d, 0xa90, INVALID_LABEL_ADDRESS, 0xa90, 0xae7, 0xa4d, 0xa4d}
#define TCAM_CMD_INNER_VID_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xa5f, 0xaa2, INVALID_LABEL_ADDRESS, 0xaa2, 0xaf9, 0xa5f, 0xa5f}
#define TCAM_CMD_IP_PROTOCOL_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xa77, 0xaba, INVALID_LABEL_ADDRESS, 0xaba, 0xb11, 0xa77, 0xa77}
#define TCAM_CMD_IPV6_LABEL_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xa9d, 0xae0, INVALID_LABEL_ADDRESS, 0xae0, 0xb37, 0xa9d, 0xa9d}
#define TCAM_CMD_L3_PROTOCOL_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xa7d, 0xac0, INVALID_LABEL_ADDRESS, 0xac0, 0xb17, 0xa7d, 0xa7d}
#define TCAM_CMD_NETWORK_LAYER_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xa89, 0xacc, INVALID_LABEL_ADDRESS, 0xacc, 0xb23, 0xa89, 0xa89}
#define TCAM_CMD_OUTER_PBIT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xa65, 0xaa8, INVALID_LABEL_ADDRESS, 0xaa8, 0xaff, 0xa65, 0xa65}
#define TCAM_CMD_OUTER_TPID_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xa47, 0xa8a, INVALID_LABEL_ADDRESS, 0xa8a, 0xae1, 0xa47, 0xa47}
#define TCAM_CMD_OUTER_VID_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xa59, 0xa9c, INVALID_LABEL_ADDRESS, 0xa9c, 0xaf3, 0xa59, 0xa59}
#define TCAM_CMD_SRC_IP_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xaa8, 0xaeb, INVALID_LABEL_ADDRESS, 0xaeb, 0xb42, 0xaa8, 0xaa8}
#define TCAM_CMD_SRC_IPV6_MASKED_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xabd, 0xb00, INVALID_LABEL_ADDRESS, 0xb00, 0xb57, 0xabd, 0xabd}
#define TCAM_CMD_SRC_MAC_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xaf8, 0xb3b, INVALID_LABEL_ADDRESS, 0xb3b, 0xb92, 0xaf8, 0xaf8}
#define TCAM_CMD_SRC_PORT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xaec, 0xb2f, INVALID_LABEL_ADDRESS, 0xb2f, 0xb86, 0xaec, 0xaec}
#define TCAM_CMD_TOS_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xa83, 0xac6, INVALID_LABEL_ADDRESS, 0xac6, 0xb1d, 0xa83, 0xa83}
#define TCAM_CMD_VLAN_NUM_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xa71, 0xab4, INVALID_LABEL_ADDRESS, 0xab4, 0xb0b, 0xa71, 0xa71}

#endif


#endif

