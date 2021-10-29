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
#define image_1_start_task_cpu_rx_wakeup_request        (0xcd4)
#define image_1_cpu_rx_wakeup_request        (0xcd4)
#define image_1_start_task_cpu_recycle_wakeup_request        (0x10b8)
#define image_1_cpu_recycle_wakeup_request        (0x10b8)
#define image_1_start_task_interrupt_coalescing_1st_wakeup_request        (0x120c)
#define image_1_interrupt_coalescing_1st_wakeup_request        (0x120c)
#define image_1_start_task_cpu_rx_meter_budget_allocator_1st_wakeup_request        (0xff8)
#define image_1_cpu_rx_meter_budget_allocator_1st_wakeup_request        (0xff8)
#define image_1_start_task_cpu_rx_copy_wakeup_request        (0x6a4)
#define image_1_cpu_rx_copy_wakeup_request        (0x6a4)
#define image_1_debug_routine_handler        (0x4)

#else

#define image_1_start_task_initialization_task        (0x2)
#define image_1_initialization_task        (0x2)
#define image_1_start_task_debug_routine        (0x12)
#define image_1_debug_routine        (0x12)
#define image_1_start_task_cpu_rx_wakeup_request        (0x335)
#define image_1_cpu_rx_wakeup_request        (0x335)
#define image_1_start_task_cpu_recycle_wakeup_request        (0x42e)
#define image_1_cpu_recycle_wakeup_request        (0x42e)
#define image_1_start_task_interrupt_coalescing_1st_wakeup_request        (0x483)
#define image_1_interrupt_coalescing_1st_wakeup_request        (0x483)
#define image_1_start_task_cpu_rx_meter_budget_allocator_1st_wakeup_request        (0x3fe)
#define image_1_cpu_rx_meter_budget_allocator_1st_wakeup_request        (0x3fe)
#define image_1_start_task_cpu_rx_copy_wakeup_request        (0x1a9)
#define image_1_cpu_rx_copy_wakeup_request        (0x1a9)
#define image_1_debug_routine_handler        (0x1)

#endif


#endif

/* IMAGE 2 LABELS */
#ifndef IMAGE_2_CODE_ADDRESSES
#define IMAGE_2_CODE_ADDRESSES

#ifndef PC_ADDRESS_INST_IND

#define image_2_start_task_initialization_task        (0x8)
#define image_2_initialization_task        (0x8)
#define image_2_start_task_ghost_reporting_1st_wakeup_request        (0x114)
#define image_2_ghost_reporting_1st_wakeup_request        (0x114)
#define image_2_start_task_cpu_tx_wakeup_request        (0x7ec)
#define image_2_cpu_tx_wakeup_request        (0x7ec)
#define image_2_cpu_tx_read_ring_indices        (0x7ec)
#define image_2_start_task_budget_allocator_1st_wakeup_request        (0x1884)
#define image_2_budget_allocator_1st_wakeup_request        (0x1884)
#define image_2_start_task_debug_routine        (0x48)
#define image_2_debug_routine        (0x48)
#define image_2_start_task_cpu_recycle_wakeup_request        (0xf0c)
#define image_2_cpu_recycle_wakeup_request        (0xf0c)
#define image_2_start_task_interrupt_coalescing_1st_wakeup_request        (0x1060)
#define image_2_interrupt_coalescing_1st_wakeup_request        (0x1060)
#define image_2_start_task_timer_common_task_wakeup_request        (0x10d8)
#define image_2_timer_common_task_wakeup_request        (0x10d8)
#define image_2_start_task_service_queues_update_fifo_read_1st_wakeup_request        (0x1238)
#define image_2_service_queues_update_fifo_read_1st_wakeup_request        (0x1238)
#define image_2_start_task_service_queues_tx_task_wakeup_request        (0x14f4)
#define image_2_service_queues_tx_task_wakeup_request        (0x14f4)
#define image_2_debug_routine_handler        (0x4)

#else

#define image_2_start_task_initialization_task        (0x2)
#define image_2_initialization_task        (0x2)
#define image_2_start_task_ghost_reporting_1st_wakeup_request        (0x45)
#define image_2_ghost_reporting_1st_wakeup_request        (0x45)
#define image_2_start_task_cpu_tx_wakeup_request        (0x1fb)
#define image_2_cpu_tx_wakeup_request        (0x1fb)
#define image_2_cpu_tx_read_ring_indices        (0x1fb)
#define image_2_start_task_budget_allocator_1st_wakeup_request        (0x621)
#define image_2_budget_allocator_1st_wakeup_request        (0x621)
#define image_2_start_task_debug_routine        (0x12)
#define image_2_debug_routine        (0x12)
#define image_2_start_task_cpu_recycle_wakeup_request        (0x3c3)
#define image_2_cpu_recycle_wakeup_request        (0x3c3)
#define image_2_start_task_interrupt_coalescing_1st_wakeup_request        (0x418)
#define image_2_interrupt_coalescing_1st_wakeup_request        (0x418)
#define image_2_start_task_timer_common_task_wakeup_request        (0x436)
#define image_2_timer_common_task_wakeup_request        (0x436)
#define image_2_start_task_service_queues_update_fifo_read_1st_wakeup_request        (0x48e)
#define image_2_service_queues_update_fifo_read_1st_wakeup_request        (0x48e)
#define image_2_start_task_service_queues_tx_task_wakeup_request        (0x53d)
#define image_2_service_queues_tx_task_wakeup_request        (0x53d)
#define image_2_debug_routine_handler        (0x1)

#endif


#endif

/* IMAGE 3 LABELS */
#ifndef IMAGE_3_CODE_ADDRESSES
#define IMAGE_3_CODE_ADDRESSES

#ifndef PC_ADDRESS_INST_IND

#define image_3_start_task_initialization_task        (0x8)
#define image_3_initialization_task        (0x8)
#define image_3_start_task_update_fifo_us_read_1st_wakeup_request        (0x1c5c)
#define image_3_update_fifo_us_read_1st_wakeup_request        (0x1c5c)
#define image_3_start_task_us_tx_task_1st_wakeup_request        (0xdac)
#define image_3_us_tx_task_1st_wakeup_request        (0xdac)
#define image_3_us_tx_task_wakeup_request        (0xdac)
#define image_3_start_task_epon_update_fifo_read_1st_wakeup_request        (0x1f40)
#define image_3_epon_update_fifo_read_1st_wakeup_request        (0x1f40)
#define image_3_start_task_epon_tx_task_wakeup_request        (0x1560)
#define image_3_epon_tx_task_wakeup_request        (0x1560)
#define image_3_start_task_flush_task_1st_wakeup_request        (0x1660)
#define image_3_flush_task_1st_wakeup_request        (0x1660)
#define image_3_start_task_budget_allocator_1st_wakeup_request        (0x1748)
#define image_3_budget_allocator_1st_wakeup_request        (0x1748)
#define image_3_start_task_debug_routine        (0x88)
#define image_3_debug_routine        (0x88)
#define image_3_start_task_gpon_control_wakeup_request        (0x154)
#define image_3_gpon_control_wakeup_request        (0x154)
#define image_3_start_task_ovl_budget_allocator_1st_wakeup_request        (0x1aac)
#define image_3_ovl_budget_allocator_1st_wakeup_request        (0x1aac)
#define image_3_debug_routine_handler        (0x4)

#else

#define image_3_start_task_initialization_task        (0x2)
#define image_3_initialization_task        (0x2)
#define image_3_start_task_update_fifo_us_read_1st_wakeup_request        (0x717)
#define image_3_update_fifo_us_read_1st_wakeup_request        (0x717)
#define image_3_start_task_us_tx_task_1st_wakeup_request        (0x36b)
#define image_3_us_tx_task_1st_wakeup_request        (0x36b)
#define image_3_us_tx_task_wakeup_request        (0x36b)
#define image_3_start_task_epon_update_fifo_read_1st_wakeup_request        (0x7d0)
#define image_3_epon_update_fifo_read_1st_wakeup_request        (0x7d0)
#define image_3_start_task_epon_tx_task_wakeup_request        (0x558)
#define image_3_epon_tx_task_wakeup_request        (0x558)
#define image_3_start_task_flush_task_1st_wakeup_request        (0x598)
#define image_3_flush_task_1st_wakeup_request        (0x598)
#define image_3_start_task_budget_allocator_1st_wakeup_request        (0x5d2)
#define image_3_budget_allocator_1st_wakeup_request        (0x5d2)
#define image_3_start_task_debug_routine        (0x22)
#define image_3_debug_routine        (0x22)
#define image_3_start_task_gpon_control_wakeup_request        (0x55)
#define image_3_gpon_control_wakeup_request        (0x55)
#define image_3_start_task_ovl_budget_allocator_1st_wakeup_request        (0x6ab)
#define image_3_ovl_budget_allocator_1st_wakeup_request        (0x6ab)
#define image_3_debug_routine_handler        (0x1)

#endif


#endif

/* IMAGE 4 LABELS */
#ifndef IMAGE_4_CODE_ADDRESSES
#define IMAGE_4_CODE_ADDRESSES

#ifndef PC_ADDRESS_INST_IND

#define image_4_start_task_initialization_task        (0x8)
#define image_4_initialization_task        (0x8)
#define image_4_start_task_processing_wakeup_request        (0x18a0)
#define image_4_processing_wakeup_request        (0x18a0)
#define image_4_start_task_debug_routine        (0x5c)
#define image_4_debug_routine        (0x5c)
#define image_4_debug_routine_handler        (0x4)
#define image_4_tcam_cmd_ic_submit        (0x2d28)
#define image_4_tcam_cmd_outer_tpid        (0x2d6c)
#define image_4_tcam_cmd_inner_tpid        (0x2d84)
#define image_4_tcam_cmd_ethertype        (0x2d9c)
#define image_4_tcam_cmd_outer_vid        (0x2db4)
#define image_4_tcam_cmd_inner_vid        (0x2dcc)
#define image_4_tcam_cmd_outer_pbit        (0x2de4)
#define image_4_tcam_cmd_inner_pbit        (0x2dfc)
#define image_4_tcam_cmd_vlan_num        (0x2e14)
#define image_4_tcam_cmd_ip_protocol        (0x2e2c)
#define image_4_tcam_cmd_l3_protocol        (0x2e44)
#define image_4_tcam_cmd_tos        (0x2e5c)
#define image_4_tcam_cmd_network_layer        (0x2e74)
#define image_4_tcam_cmd_ingress_port        (0x2e94)
#define image_4_tcam_cmd_gem_flow        (0x2eac)
#define image_4_tcam_cmd_ipv6_label        (0x2ec4)
#define image_4_tcam_cmd_src_ip        (0x2ef0)
#define image_4_tcam_cmd_src_ipv6_masked        (0x2f44)
#define image_4_tcam_cmd_dst_ip        (0x2f78)
#define image_4_tcam_cmd_dst_ipv6_masked        (0x2fcc)
#define image_4_tcam_cmd_src_port        (0x3000)
#define image_4_tcam_cmd_dst_port        (0x3018)
#define image_4_tcam_cmd_src_mac        (0x3030)
#define image_4_tcam_cmd_dst_mac        (0x305c)
#define image_4_tcam_cmd_generic_l2        (0x3088)
#define image_4_tcam_cmd_generic_l3        (0x30bc)
#define image_4_tcam_cmd_generic_l4        (0x30f0)
#define image_4_gpe_cmd_replace_16        (0x35cc)
#define image_4_gpe_cmd_replace_32        (0x35d4)
#define image_4_gpe_cmd_replace_bits_16        (0x35ec)
#define image_4_gpe_cmd_copy_bits_16        (0x360c)
#define image_4_gpe_cmd_skip_if        (0x363c)
#define image_4_gpe_vlan_action_cmd_drop        (0x365c)
#define image_4_gpe_vlan_action_cmd_dscp        (0x3660)
#define image_4_gpe_vlan_action_cmd_mac_hdr_copy        (0x36b0)

#else

#define image_4_start_task_initialization_task        (0x2)
#define image_4_initialization_task        (0x2)
#define image_4_start_task_processing_wakeup_request        (0x628)
#define image_4_processing_wakeup_request        (0x628)
#define image_4_start_task_debug_routine        (0x17)
#define image_4_debug_routine        (0x17)
#define image_4_debug_routine_handler        (0x1)
#define image_4_tcam_cmd_ic_submit        (0xb4a)
#define image_4_tcam_cmd_outer_tpid        (0xb5b)
#define image_4_tcam_cmd_inner_tpid        (0xb61)
#define image_4_tcam_cmd_ethertype        (0xb67)
#define image_4_tcam_cmd_outer_vid        (0xb6d)
#define image_4_tcam_cmd_inner_vid        (0xb73)
#define image_4_tcam_cmd_outer_pbit        (0xb79)
#define image_4_tcam_cmd_inner_pbit        (0xb7f)
#define image_4_tcam_cmd_vlan_num        (0xb85)
#define image_4_tcam_cmd_ip_protocol        (0xb8b)
#define image_4_tcam_cmd_l3_protocol        (0xb91)
#define image_4_tcam_cmd_tos        (0xb97)
#define image_4_tcam_cmd_network_layer        (0xb9d)
#define image_4_tcam_cmd_ingress_port        (0xba5)
#define image_4_tcam_cmd_gem_flow        (0xbab)
#define image_4_tcam_cmd_ipv6_label        (0xbb1)
#define image_4_tcam_cmd_src_ip        (0xbbc)
#define image_4_tcam_cmd_src_ipv6_masked        (0xbd1)
#define image_4_tcam_cmd_dst_ip        (0xbde)
#define image_4_tcam_cmd_dst_ipv6_masked        (0xbf3)
#define image_4_tcam_cmd_src_port        (0xc00)
#define image_4_tcam_cmd_dst_port        (0xc06)
#define image_4_tcam_cmd_src_mac        (0xc0c)
#define image_4_tcam_cmd_dst_mac        (0xc17)
#define image_4_tcam_cmd_generic_l2        (0xc22)
#define image_4_tcam_cmd_generic_l3        (0xc2f)
#define image_4_tcam_cmd_generic_l4        (0xc3c)
#define image_4_gpe_cmd_replace_16        (0xd73)
#define image_4_gpe_cmd_replace_32        (0xd75)
#define image_4_gpe_cmd_replace_bits_16        (0xd7b)
#define image_4_gpe_cmd_copy_bits_16        (0xd83)
#define image_4_gpe_cmd_skip_if        (0xd8f)
#define image_4_gpe_vlan_action_cmd_drop        (0xd97)
#define image_4_gpe_vlan_action_cmd_dscp        (0xd98)
#define image_4_gpe_vlan_action_cmd_mac_hdr_copy        (0xdac)

#endif


#endif

/* COMMON LABELS */
#ifndef COMMON_CODE_ADDRESSES
#define COMMON_CODE_ADDRESSES

#define INVALID_LABEL_ADDRESS 0xFFFFFF

#ifndef PC_ADDRESS_INST_IND

#define TCAM_CMD_DST_IP_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x2f78}
#define TCAM_CMD_DST_IPV6_MASKED_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x2fcc}
#define TCAM_CMD_DST_MAC_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x305c}
#define TCAM_CMD_DST_PORT_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x3018}
#define TCAM_CMD_ETHERTYPE_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x2d9c}
#define TCAM_CMD_GEM_FLOW_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x2eac}
#define TCAM_CMD_GENERIC_L2_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x3088}
#define TCAM_CMD_GENERIC_L3_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x30bc}
#define TCAM_CMD_GENERIC_L4_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x30f0}
#define TCAM_CMD_IC_SUBMIT_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x2d28}
#define TCAM_CMD_INGRESS_PORT_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x2e94}
#define TCAM_CMD_INNER_PBIT_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x2dfc}
#define TCAM_CMD_INNER_TPID_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x2d84}
#define TCAM_CMD_INNER_VID_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x2dcc}
#define TCAM_CMD_IP_PROTOCOL_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x2e2c}
#define TCAM_CMD_IPV6_LABEL_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x2ec4}
#define TCAM_CMD_L3_PROTOCOL_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x2e44}
#define TCAM_CMD_NETWORK_LAYER_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x2e74}
#define TCAM_CMD_OUTER_PBIT_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x2de4}
#define TCAM_CMD_OUTER_TPID_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x2d6c}
#define TCAM_CMD_OUTER_VID_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x2db4}
#define TCAM_CMD_SRC_IP_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x2ef0}
#define TCAM_CMD_SRC_IPV6_MASKED_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x2f44}
#define TCAM_CMD_SRC_MAC_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x3030}
#define TCAM_CMD_SRC_PORT_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x3000}
#define TCAM_CMD_TOS_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x2e5c}
#define TCAM_CMD_VLAN_NUM_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0x2e14}

#else

#define TCAM_CMD_DST_IP_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xbde}
#define TCAM_CMD_DST_IPV6_MASKED_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xbf3}
#define TCAM_CMD_DST_MAC_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xc17}
#define TCAM_CMD_DST_PORT_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xc06}
#define TCAM_CMD_ETHERTYPE_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xb67}
#define TCAM_CMD_GEM_FLOW_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xbab}
#define TCAM_CMD_GENERIC_L2_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xc22}
#define TCAM_CMD_GENERIC_L3_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xc2f}
#define TCAM_CMD_GENERIC_L4_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xc3c}
#define TCAM_CMD_IC_SUBMIT_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xb4a}
#define TCAM_CMD_INGRESS_PORT_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xba5}
#define TCAM_CMD_INNER_PBIT_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xb7f}
#define TCAM_CMD_INNER_TPID_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xb61}
#define TCAM_CMD_INNER_VID_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xb73}
#define TCAM_CMD_IP_PROTOCOL_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xb8b}
#define TCAM_CMD_IPV6_LABEL_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xbb1}
#define TCAM_CMD_L3_PROTOCOL_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xb91}
#define TCAM_CMD_NETWORK_LAYER_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xb9d}
#define TCAM_CMD_OUTER_PBIT_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xb79}
#define TCAM_CMD_OUTER_TPID_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xb5b}
#define TCAM_CMD_OUTER_VID_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xb6d}
#define TCAM_CMD_SRC_IP_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xbbc}
#define TCAM_CMD_SRC_IPV6_MASKED_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xbd1}
#define TCAM_CMD_SRC_MAC_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xc0c}
#define TCAM_CMD_SRC_PORT_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xc00}
#define TCAM_CMD_TOS_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xb97}
#define TCAM_CMD_VLAN_NUM_ADDR_ARR {INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, INVALID_LABEL_ADDRESS, 0xb85}

#endif


#endif

