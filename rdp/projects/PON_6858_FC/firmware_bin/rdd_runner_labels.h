/* IMAGE 0 LABELS */
#ifndef IMAGE_0_CODE_ADDRESSES
#define IMAGE_0_CODE_ADDRESSES

#ifndef PC_ADDRESS_INST_IND

#define image_0_start_task_initialization_task        (0x8)
#define image_0_initialization_task        (0x8)
#define image_0_start_task_flush_task_1st_wakeup_request        (0x88c)
#define image_0_flush_task_1st_wakeup_request        (0x88c)
#define image_0_start_task_budget_allocator_1st_wakeup_request        (0x994)
#define image_0_budget_allocator_1st_wakeup_request        (0x994)
#define image_0_start_task_debug_routine        (0x88)
#define image_0_debug_routine        (0x88)
#define image_0_start_task_ds_tx_task_wakeup_request        (0x458)
#define image_0_ds_tx_task_wakeup_request        (0x458)
#define image_0_start_task_update_fifo_ds_read_1st_wakeup_request        (0xd98)
#define image_0_update_fifo_ds_read_1st_wakeup_request        (0xd98)
#define image_0_start_task_buffer_cong_mgt_1st_wakeup_request        (0x10c8)
#define image_0_buffer_cong_mgt_1st_wakeup_request        (0x10c8)
#define image_0_debug_routine_handler        (0x4)

#else

#define image_0_start_task_initialization_task        (0x2)
#define image_0_initialization_task        (0x2)
#define image_0_start_task_flush_task_1st_wakeup_request        (0x223)
#define image_0_flush_task_1st_wakeup_request        (0x223)
#define image_0_start_task_budget_allocator_1st_wakeup_request        (0x265)
#define image_0_budget_allocator_1st_wakeup_request        (0x265)
#define image_0_start_task_debug_routine        (0x22)
#define image_0_debug_routine        (0x22)
#define image_0_start_task_ds_tx_task_wakeup_request        (0x116)
#define image_0_ds_tx_task_wakeup_request        (0x116)
#define image_0_start_task_update_fifo_ds_read_1st_wakeup_request        (0x366)
#define image_0_update_fifo_ds_read_1st_wakeup_request        (0x366)
#define image_0_start_task_buffer_cong_mgt_1st_wakeup_request        (0x432)
#define image_0_buffer_cong_mgt_1st_wakeup_request        (0x432)
#define image_0_debug_routine_handler        (0x1)

#endif


#endif

/* IMAGE 1 LABELS */
#ifndef IMAGE_1_CODE_ADDRESSES
#define IMAGE_1_CODE_ADDRESSES

#ifndef PC_ADDRESS_INST_IND

#define image_1_start_task_initialization_task        (0x8)
#define image_1_initialization_task        (0x8)
#define image_1_start_task_debug_routine        (0x48)
#define image_1_debug_routine        (0x48)
#define image_1_start_task_cpu_rx_wakeup_request        (0x2298)
#define image_1_cpu_rx_wakeup_request        (0x2298)
#define image_1_start_task_cpu_recycle_wakeup_request        (0x267c)
#define image_1_cpu_recycle_wakeup_request        (0x267c)
#define image_1_start_task_interrupt_coalescing_1st_wakeup_request        (0x27d0)
#define image_1_interrupt_coalescing_1st_wakeup_request        (0x27d0)
#define image_1_start_task_cpu_rx_meter_budget_allocator_1st_wakeup_request        (0x25bc)
#define image_1_cpu_rx_meter_budget_allocator_1st_wakeup_request        (0x25bc)
#define image_1_start_task_dhd_tx_post_wakeup_request        (0x5bc)
#define image_1_dhd_tx_post_wakeup_request        (0x5bc)
#define image_1_start_task_dhd_tx_post_update_fifo_wakeup_request        (0x564)
#define image_1_dhd_tx_post_update_fifo_wakeup_request        (0x564)
#define image_1_start_task_dhd_timer_1st_wakeup_request        (0x114)
#define image_1_dhd_timer_1st_wakeup_request        (0x114)
#define image_1_start_task_cpu_rx_copy_wakeup_request        (0x19b4)
#define image_1_cpu_rx_copy_wakeup_request        (0x19b4)
#define image_1_start_task_spdsvc_gen_wakeup_request        (0x2894)
#define image_1_spdsvc_gen_wakeup_request        (0x2894)
#define image_1_start_task_common_reprocessing_wakeup_request        (0x3a64)
#define image_1_common_reprocessing_wakeup_request        (0x3a64)
#define image_1_debug_routine_handler        (0x4)

#else

#define image_1_start_task_initialization_task        (0x2)
#define image_1_initialization_task        (0x2)
#define image_1_start_task_debug_routine        (0x12)
#define image_1_debug_routine        (0x12)
#define image_1_start_task_cpu_rx_wakeup_request        (0x8a6)
#define image_1_cpu_rx_wakeup_request        (0x8a6)
#define image_1_start_task_cpu_recycle_wakeup_request        (0x99f)
#define image_1_cpu_recycle_wakeup_request        (0x99f)
#define image_1_start_task_interrupt_coalescing_1st_wakeup_request        (0x9f4)
#define image_1_interrupt_coalescing_1st_wakeup_request        (0x9f4)
#define image_1_start_task_cpu_rx_meter_budget_allocator_1st_wakeup_request        (0x96f)
#define image_1_cpu_rx_meter_budget_allocator_1st_wakeup_request        (0x96f)
#define image_1_start_task_dhd_tx_post_wakeup_request        (0x16f)
#define image_1_dhd_tx_post_wakeup_request        (0x16f)
#define image_1_start_task_dhd_tx_post_update_fifo_wakeup_request        (0x159)
#define image_1_dhd_tx_post_update_fifo_wakeup_request        (0x159)
#define image_1_start_task_dhd_timer_1st_wakeup_request        (0x45)
#define image_1_dhd_timer_1st_wakeup_request        (0x45)
#define image_1_start_task_cpu_rx_copy_wakeup_request        (0x66d)
#define image_1_cpu_rx_copy_wakeup_request        (0x66d)
#define image_1_start_task_spdsvc_gen_wakeup_request        (0xa25)
#define image_1_spdsvc_gen_wakeup_request        (0xa25)
#define image_1_start_task_common_reprocessing_wakeup_request        (0xe99)
#define image_1_common_reprocessing_wakeup_request        (0xe99)
#define image_1_debug_routine_handler        (0x1)

#endif


#endif

/* IMAGE 2 LABELS */
#ifndef IMAGE_2_CODE_ADDRESSES
#define IMAGE_2_CODE_ADDRESSES

#ifndef PC_ADDRESS_INST_IND

#define image_2_start_task_initialization_task        (0x8)
#define image_2_initialization_task        (0x8)
#define image_2_start_task_ghost_reporting_1st_wakeup_request        (0x7ac)
#define image_2_ghost_reporting_1st_wakeup_request        (0x7ac)
#define image_2_start_task_cpu_tx_wakeup_request        (0xe84)
#define image_2_cpu_tx_wakeup_request        (0xe84)
#define image_2_cpu_tx_read_ring_indices        (0xe84)
#define image_2_start_task_budget_allocator_1st_wakeup_request        (0x1f80)
#define image_2_budget_allocator_1st_wakeup_request        (0x1f80)
#define image_2_start_task_debug_routine        (0x48)
#define image_2_debug_routine        (0x48)
#define image_2_start_task_cpu_recycle_wakeup_request        (0x15a4)
#define image_2_cpu_recycle_wakeup_request        (0x15a4)
#define image_2_start_task_interrupt_coalescing_1st_wakeup_request        (0x16f8)
#define image_2_interrupt_coalescing_1st_wakeup_request        (0x16f8)
#define image_2_start_task_dhd_tx_complete_wakeup_request        (0x114)
#define image_2_dhd_tx_complete_wakeup_request        (0x114)
#define image_2_start_task_dhd_rx_complete_wakeup_request        (0x45c)
#define image_2_dhd_rx_complete_wakeup_request        (0x45c)
#define image_2_start_task_timer_common_task_wakeup_request        (0x1770)
#define image_2_timer_common_task_wakeup_request        (0x1770)
#define image_2_start_task_service_queues_update_fifo_read_1st_wakeup_request        (0x18d0)
#define image_2_service_queues_update_fifo_read_1st_wakeup_request        (0x18d0)
#define image_2_start_task_service_queues_tx_task_wakeup_request        (0x1be0)
#define image_2_service_queues_tx_task_wakeup_request        (0x1be0)
#define image_2_debug_routine_handler        (0x4)

#else

#define image_2_start_task_initialization_task        (0x2)
#define image_2_initialization_task        (0x2)
#define image_2_start_task_ghost_reporting_1st_wakeup_request        (0x1eb)
#define image_2_ghost_reporting_1st_wakeup_request        (0x1eb)
#define image_2_start_task_cpu_tx_wakeup_request        (0x3a1)
#define image_2_cpu_tx_wakeup_request        (0x3a1)
#define image_2_cpu_tx_read_ring_indices        (0x3a1)
#define image_2_start_task_budget_allocator_1st_wakeup_request        (0x7e0)
#define image_2_budget_allocator_1st_wakeup_request        (0x7e0)
#define image_2_start_task_debug_routine        (0x12)
#define image_2_debug_routine        (0x12)
#define image_2_start_task_cpu_recycle_wakeup_request        (0x569)
#define image_2_cpu_recycle_wakeup_request        (0x569)
#define image_2_start_task_interrupt_coalescing_1st_wakeup_request        (0x5be)
#define image_2_interrupt_coalescing_1st_wakeup_request        (0x5be)
#define image_2_start_task_dhd_tx_complete_wakeup_request        (0x45)
#define image_2_dhd_tx_complete_wakeup_request        (0x45)
#define image_2_start_task_dhd_rx_complete_wakeup_request        (0x117)
#define image_2_dhd_rx_complete_wakeup_request        (0x117)
#define image_2_start_task_timer_common_task_wakeup_request        (0x5dc)
#define image_2_timer_common_task_wakeup_request        (0x5dc)
#define image_2_start_task_service_queues_update_fifo_read_1st_wakeup_request        (0x634)
#define image_2_service_queues_update_fifo_read_1st_wakeup_request        (0x634)
#define image_2_start_task_service_queues_tx_task_wakeup_request        (0x6f8)
#define image_2_service_queues_tx_task_wakeup_request        (0x6f8)
#define image_2_debug_routine_handler        (0x1)

#endif


#endif

/* IMAGE 3 LABELS */
#ifndef IMAGE_3_CODE_ADDRESSES
#define IMAGE_3_CODE_ADDRESSES

#ifndef PC_ADDRESS_INST_IND

#define image_3_start_task_initialization_task        (0x8)
#define image_3_initialization_task        (0x8)
#define image_3_start_task_update_fifo_us_read_1st_wakeup_request        (0x1c44)
#define image_3_update_fifo_us_read_1st_wakeup_request        (0x1c44)
#define image_3_start_task_us_tx_task_1st_wakeup_request        (0xdac)
#define image_3_us_tx_task_1st_wakeup_request        (0xdac)
#define image_3_us_tx_task_wakeup_request        (0xdac)
#define image_3_start_task_epon_update_fifo_read_1st_wakeup_request        (0x1f7c)
#define image_3_epon_update_fifo_read_1st_wakeup_request        (0x1f7c)
#define image_3_start_task_epon_tx_task_wakeup_request        (0x1548)
#define image_3_epon_tx_task_wakeup_request        (0x1548)
#define image_3_start_task_flush_task_1st_wakeup_request        (0x1648)
#define image_3_flush_task_1st_wakeup_request        (0x1648)
#define image_3_start_task_budget_allocator_1st_wakeup_request        (0x1730)
#define image_3_budget_allocator_1st_wakeup_request        (0x1730)
#define image_3_start_task_debug_routine        (0x88)
#define image_3_debug_routine        (0x88)
#define image_3_start_task_gpon_control_wakeup_request        (0x154)
#define image_3_gpon_control_wakeup_request        (0x154)
#define image_3_start_task_ovl_budget_allocator_1st_wakeup_request        (0x1a94)
#define image_3_ovl_budget_allocator_1st_wakeup_request        (0x1a94)
#define image_3_start_task_buffer_cong_mgt_1st_wakeup_request        (0x2040)
#define image_3_buffer_cong_mgt_1st_wakeup_request        (0x2040)
#define image_3_debug_routine_handler        (0x4)

#else

#define image_3_start_task_initialization_task        (0x2)
#define image_3_initialization_task        (0x2)
#define image_3_start_task_update_fifo_us_read_1st_wakeup_request        (0x711)
#define image_3_update_fifo_us_read_1st_wakeup_request        (0x711)
#define image_3_start_task_us_tx_task_1st_wakeup_request        (0x36b)
#define image_3_us_tx_task_1st_wakeup_request        (0x36b)
#define image_3_us_tx_task_wakeup_request        (0x36b)
#define image_3_start_task_epon_update_fifo_read_1st_wakeup_request        (0x7df)
#define image_3_epon_update_fifo_read_1st_wakeup_request        (0x7df)
#define image_3_start_task_epon_tx_task_wakeup_request        (0x552)
#define image_3_epon_tx_task_wakeup_request        (0x552)
#define image_3_start_task_flush_task_1st_wakeup_request        (0x592)
#define image_3_flush_task_1st_wakeup_request        (0x592)
#define image_3_start_task_budget_allocator_1st_wakeup_request        (0x5cc)
#define image_3_budget_allocator_1st_wakeup_request        (0x5cc)
#define image_3_start_task_debug_routine        (0x22)
#define image_3_debug_routine        (0x22)
#define image_3_start_task_gpon_control_wakeup_request        (0x55)
#define image_3_gpon_control_wakeup_request        (0x55)
#define image_3_start_task_ovl_budget_allocator_1st_wakeup_request        (0x6a5)
#define image_3_ovl_budget_allocator_1st_wakeup_request        (0x6a5)
#define image_3_start_task_buffer_cong_mgt_1st_wakeup_request        (0x810)
#define image_3_buffer_cong_mgt_1st_wakeup_request        (0x810)
#define image_3_debug_routine_handler        (0x1)

#endif


#endif

/* IMAGE 4 LABELS */
#ifndef IMAGE_4_CODE_ADDRESSES
#define IMAGE_4_CODE_ADDRESSES

#ifndef PC_ADDRESS_INST_IND

#define image_4_start_task_initialization_task        (0x8)
#define image_4_initialization_task        (0x8)
#define image_4_start_task_processing_wakeup_request        (0x14fc)
#define image_4_processing_wakeup_request        (0x14fc)
#define image_4_start_task_debug_routine        (0x5c)
#define image_4_debug_routine        (0x5c)
#define image_4_debug_routine_handler        (0x4)
#define image_4_processing_header_update_length_calc        (0x1e8c)
#define image_4_processing_layer2_header_copy_30_bytes_8_bytes_align        (0x1f74)
#define image_4_processing_layer2_header_copy_22_bytes_8_bytes_align        (0x1f7c)
#define image_4_processing_layer2_header_copy_14_bytes_8_bytes_align        (0x1f84)
#define image_4_processing_layer2_header_copy_30_bytes_4_bytes_align        (0x1f98)
#define image_4_processing_layer2_header_copy_22_bytes_4_bytes_align        (0x1fa8)
#define image_4_processing_layer2_header_copy_14_bytes_4_bytes_align        (0x1fb8)
#define image_4_processing_layer2_header_copy_26_bytes_8_bytes_align        (0x1fd8)
#define image_4_processing_layer2_header_copy_18_bytes_8_bytes_align        (0x1fe4)
#define image_4_processing_layer2_header_copy_18_bytes_4_bytes_align        (0x2008)
#define image_4_processing_layer2_header_copy_26_bytes_4_bytes_align        (0x2014)
#define image_4_tcam_cmd_ic_submit        (0x3900)
#define image_4_tcam_cmd_outer_tpid        (0x3944)
#define image_4_tcam_cmd_inner_tpid        (0x395c)
#define image_4_tcam_cmd_ethertype        (0x3974)
#define image_4_tcam_cmd_outer_vid        (0x398c)
#define image_4_tcam_cmd_inner_vid        (0x39a4)
#define image_4_tcam_cmd_outer_pbit        (0x39bc)
#define image_4_tcam_cmd_inner_pbit        (0x39d4)
#define image_4_tcam_cmd_vlan_num        (0x39ec)
#define image_4_tcam_cmd_ip_protocol        (0x3a04)
#define image_4_tcam_cmd_l3_protocol        (0x3a1c)
#define image_4_tcam_cmd_tos        (0x3a34)
#define image_4_tcam_cmd_network_layer        (0x3a4c)
#define image_4_tcam_cmd_ingress_port        (0x3a6c)
#define image_4_tcam_cmd_gem_flow        (0x3a84)
#define image_4_tcam_cmd_ipv6_label        (0x3a9c)
#define image_4_tcam_cmd_src_ip        (0x3ac8)
#define image_4_tcam_cmd_src_ipv6_masked        (0x3b1c)
#define image_4_tcam_cmd_dst_ip        (0x3b50)
#define image_4_tcam_cmd_dst_ipv6_masked        (0x3ba4)
#define image_4_tcam_cmd_src_port        (0x3bd8)
#define image_4_tcam_cmd_dst_port        (0x3bf0)
#define image_4_tcam_cmd_src_mac        (0x3c08)
#define image_4_tcam_cmd_dst_mac        (0x3c34)
#define image_4_tcam_cmd_generic_l2        (0x3c60)
#define image_4_tcam_cmd_generic_l3        (0x3c94)
#define image_4_tcam_cmd_generic_l4        (0x3cc8)

#else

#define image_4_start_task_initialization_task        (0x2)
#define image_4_initialization_task        (0x2)
#define image_4_start_task_processing_wakeup_request        (0x53f)
#define image_4_processing_wakeup_request        (0x53f)
#define image_4_start_task_debug_routine        (0x17)
#define image_4_debug_routine        (0x17)
#define image_4_debug_routine_handler        (0x1)
#define image_4_processing_header_update_length_calc        (0x7a3)
#define image_4_processing_layer2_header_copy_30_bytes_8_bytes_align        (0x7dd)
#define image_4_processing_layer2_header_copy_22_bytes_8_bytes_align        (0x7df)
#define image_4_processing_layer2_header_copy_14_bytes_8_bytes_align        (0x7e1)
#define image_4_processing_layer2_header_copy_30_bytes_4_bytes_align        (0x7e6)
#define image_4_processing_layer2_header_copy_22_bytes_4_bytes_align        (0x7ea)
#define image_4_processing_layer2_header_copy_14_bytes_4_bytes_align        (0x7ee)
#define image_4_processing_layer2_header_copy_26_bytes_8_bytes_align        (0x7f6)
#define image_4_processing_layer2_header_copy_18_bytes_8_bytes_align        (0x7f9)
#define image_4_processing_layer2_header_copy_18_bytes_4_bytes_align        (0x802)
#define image_4_processing_layer2_header_copy_26_bytes_4_bytes_align        (0x805)
#define image_4_tcam_cmd_ic_submit        (0xe40)
#define image_4_tcam_cmd_outer_tpid        (0xe51)
#define image_4_tcam_cmd_inner_tpid        (0xe57)
#define image_4_tcam_cmd_ethertype        (0xe5d)
#define image_4_tcam_cmd_outer_vid        (0xe63)
#define image_4_tcam_cmd_inner_vid        (0xe69)
#define image_4_tcam_cmd_outer_pbit        (0xe6f)
#define image_4_tcam_cmd_inner_pbit        (0xe75)
#define image_4_tcam_cmd_vlan_num        (0xe7b)
#define image_4_tcam_cmd_ip_protocol        (0xe81)
#define image_4_tcam_cmd_l3_protocol        (0xe87)
#define image_4_tcam_cmd_tos        (0xe8d)
#define image_4_tcam_cmd_network_layer        (0xe93)
#define image_4_tcam_cmd_ingress_port        (0xe9b)
#define image_4_tcam_cmd_gem_flow        (0xea1)
#define image_4_tcam_cmd_ipv6_label        (0xea7)
#define image_4_tcam_cmd_src_ip        (0xeb2)
#define image_4_tcam_cmd_src_ipv6_masked        (0xec7)
#define image_4_tcam_cmd_dst_ip        (0xed4)
#define image_4_tcam_cmd_dst_ipv6_masked        (0xee9)
#define image_4_tcam_cmd_src_port        (0xef6)
#define image_4_tcam_cmd_dst_port        (0xefc)
#define image_4_tcam_cmd_src_mac        (0xf02)
#define image_4_tcam_cmd_dst_mac        (0xf0d)
#define image_4_tcam_cmd_generic_l2        (0xf18)
#define image_4_tcam_cmd_generic_l3        (0xf25)
#define image_4_tcam_cmd_generic_l4        (0xf32)

#endif


#endif

/* COMMON LABELS */
#ifndef COMMON_CODE_ADDRESSES
#define COMMON_CODE_ADDRESSES

#define INVALID_LABEL_ADDRESS 0xFFFFFF

#ifndef PC_ADDRESS_INST_IND

#define TCAM_CMD_DST_IP_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x3b50}
#define TCAM_CMD_DST_IPV6_MASKED_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x3ba4}
#define TCAM_CMD_DST_MAC_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x3c34}
#define TCAM_CMD_DST_PORT_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x3bf0}
#define TCAM_CMD_ETHERTYPE_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x3974}
#define TCAM_CMD_GEM_FLOW_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x3a84}
#define TCAM_CMD_GENERIC_L2_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x3c60}
#define TCAM_CMD_GENERIC_L3_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x3c94}
#define TCAM_CMD_GENERIC_L4_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x3cc8}
#define TCAM_CMD_IC_SUBMIT_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x3900}
#define TCAM_CMD_INGRESS_PORT_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x3a6c}
#define TCAM_CMD_INNER_PBIT_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x39d4}
#define TCAM_CMD_INNER_TPID_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x395c}
#define TCAM_CMD_INNER_VID_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x39a4}
#define TCAM_CMD_IP_PROTOCOL_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x3a04}
#define TCAM_CMD_IPV6_LABEL_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x3a9c}
#define TCAM_CMD_L3_PROTOCOL_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x3a1c}
#define TCAM_CMD_NETWORK_LAYER_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x3a4c}
#define TCAM_CMD_OUTER_PBIT_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x39bc}
#define TCAM_CMD_OUTER_TPID_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x3944}
#define TCAM_CMD_OUTER_VID_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x398c}
#define TCAM_CMD_SRC_IP_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x3ac8}
#define TCAM_CMD_SRC_IPV6_MASKED_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x3b1c}
#define TCAM_CMD_SRC_MAC_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x3c08}
#define TCAM_CMD_SRC_PORT_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x3bd8}
#define TCAM_CMD_TOS_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x3a34}
#define TCAM_CMD_VLAN_NUM_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x39ec}

#else

#define TCAM_CMD_DST_IP_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xed4}
#define TCAM_CMD_DST_IPV6_MASKED_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xee9}
#define TCAM_CMD_DST_MAC_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xf0d}
#define TCAM_CMD_DST_PORT_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xefc}
#define TCAM_CMD_ETHERTYPE_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xe5d}
#define TCAM_CMD_GEM_FLOW_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xea1}
#define TCAM_CMD_GENERIC_L2_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xf18}
#define TCAM_CMD_GENERIC_L3_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xf25}
#define TCAM_CMD_GENERIC_L4_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xf32}
#define TCAM_CMD_IC_SUBMIT_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xe40}
#define TCAM_CMD_INGRESS_PORT_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xe9b}
#define TCAM_CMD_INNER_PBIT_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xe75}
#define TCAM_CMD_INNER_TPID_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xe57}
#define TCAM_CMD_INNER_VID_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xe69}
#define TCAM_CMD_IP_PROTOCOL_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xe81}
#define TCAM_CMD_IPV6_LABEL_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xea7}
#define TCAM_CMD_L3_PROTOCOL_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xe87}
#define TCAM_CMD_NETWORK_LAYER_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xe93}
#define TCAM_CMD_OUTER_PBIT_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xe6f}
#define TCAM_CMD_OUTER_TPID_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xe51}
#define TCAM_CMD_OUTER_VID_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xe63}
#define TCAM_CMD_SRC_IP_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xeb2}
#define TCAM_CMD_SRC_IPV6_MASKED_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xec7}
#define TCAM_CMD_SRC_MAC_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xf02}
#define TCAM_CMD_SRC_PORT_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xef6}
#define TCAM_CMD_TOS_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xe8d}
#define TCAM_CMD_VLAN_NUM_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xe7b}

#endif


#endif

