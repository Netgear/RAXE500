/* IMAGE 0 LABELS */
#ifndef IMAGE_0_CODE_ADDRESSES
#define IMAGE_0_CODE_ADDRESSES

#ifndef PC_ADDRESS_INST_IND

#define image_0_start_task_initialization_task        (0x48)
#define image_0_initialization_task        (0x48)
#define image_0_start_task_update_fifo_us_read_1st_wakeup_request        (0x1100)
#define image_0_update_fifo_us_read_1st_wakeup_request        (0x1100)
#define image_0_start_task_ghost_reporting_1st_wakeup_request        (0x2a34)
#define image_0_ghost_reporting_1st_wakeup_request        (0x2a34)
#define image_0_start_task_us_tx_task_1st_wakeup_request        (0xa1c)
#define image_0_us_tx_task_1st_wakeup_request        (0xa1c)
#define image_0_us_tx_task_wakeup_request        (0xa1c)
#define image_0_start_task_epon_update_fifo_read_1st_wakeup_request        (0x1424)
#define image_0_epon_update_fifo_read_1st_wakeup_request        (0x1424)
#define image_0_start_task_epon_tx_task_wakeup_request        (0x2e4c)
#define image_0_epon_tx_task_wakeup_request        (0x2e4c)
#define image_0_start_task_budget_allocator_us_1st_wakeup_request        (0x1494)
#define image_0_budget_allocator_us_1st_wakeup_request        (0x1494)
#define image_0_start_task_debug_routine        (0x88)
#define image_0_debug_routine        (0x88)
#define image_0_start_task_gpon_control_wakeup_request        (0x5fc)
#define image_0_gpon_control_wakeup_request        (0x5fc)
#define image_0_start_task_ds_tx_task_wakeup_request        (0x1ed0)
#define image_0_ds_tx_task_wakeup_request        (0x1ed0)
#define image_0_start_task_ovl_budget_allocator_1st_wakeup_request        (0x26ec)
#define image_0_ovl_budget_allocator_1st_wakeup_request        (0x26ec)
#define image_0_start_task_flush_task_1st_wakeup_request        (0x289c)
#define image_0_flush_task_1st_wakeup_request        (0x289c)
#define image_0_start_task_update_fifo_ds_read_1st_wakeup_request        (0x1b88)
#define image_0_update_fifo_ds_read_1st_wakeup_request        (0x1b88)
#define image_0_start_task_budget_allocator_ds_1st_wakeup_request        (0x2268)
#define image_0_budget_allocator_ds_1st_wakeup_request        (0x2268)
#define image_0_start_task_service_queues_update_fifo_read_1st_wakeup_request        (0x2f4c)
#define image_0_service_queues_update_fifo_read_1st_wakeup_request        (0x2f4c)
#define image_0_start_task_service_queues_tx_task_wakeup_request        (0x3280)
#define image_0_service_queues_tx_task_wakeup_request        (0x3280)
#define image_0_start_task_budget_allocator_1st_wakeup_request        (0x3520)
#define image_0_budget_allocator_1st_wakeup_request        (0x3520)
#define image_0_debug_routine_handler        (0x4)

#else

#define image_0_start_task_initialization_task        (0x12)
#define image_0_initialization_task        (0x12)
#define image_0_start_task_update_fifo_us_read_1st_wakeup_request        (0x440)
#define image_0_update_fifo_us_read_1st_wakeup_request        (0x440)
#define image_0_start_task_ghost_reporting_1st_wakeup_request        (0xa8d)
#define image_0_ghost_reporting_1st_wakeup_request        (0xa8d)
#define image_0_start_task_us_tx_task_1st_wakeup_request        (0x287)
#define image_0_us_tx_task_1st_wakeup_request        (0x287)
#define image_0_us_tx_task_wakeup_request        (0x287)
#define image_0_start_task_epon_update_fifo_read_1st_wakeup_request        (0x509)
#define image_0_epon_update_fifo_read_1st_wakeup_request        (0x509)
#define image_0_start_task_epon_tx_task_wakeup_request        (0xb93)
#define image_0_epon_tx_task_wakeup_request        (0xb93)
#define image_0_start_task_budget_allocator_us_1st_wakeup_request        (0x525)
#define image_0_budget_allocator_us_1st_wakeup_request        (0x525)
#define image_0_start_task_debug_routine        (0x22)
#define image_0_debug_routine        (0x22)
#define image_0_start_task_gpon_control_wakeup_request        (0x17f)
#define image_0_gpon_control_wakeup_request        (0x17f)
#define image_0_start_task_ds_tx_task_wakeup_request        (0x7b4)
#define image_0_ds_tx_task_wakeup_request        (0x7b4)
#define image_0_start_task_ovl_budget_allocator_1st_wakeup_request        (0x9bb)
#define image_0_ovl_budget_allocator_1st_wakeup_request        (0x9bb)
#define image_0_start_task_flush_task_1st_wakeup_request        (0xa27)
#define image_0_flush_task_1st_wakeup_request        (0xa27)
#define image_0_start_task_update_fifo_ds_read_1st_wakeup_request        (0x6e2)
#define image_0_update_fifo_ds_read_1st_wakeup_request        (0x6e2)
#define image_0_start_task_budget_allocator_ds_1st_wakeup_request        (0x89a)
#define image_0_budget_allocator_ds_1st_wakeup_request        (0x89a)
#define image_0_start_task_service_queues_update_fifo_read_1st_wakeup_request        (0xbd3)
#define image_0_service_queues_update_fifo_read_1st_wakeup_request        (0xbd3)
#define image_0_start_task_service_queues_tx_task_wakeup_request        (0xca0)
#define image_0_service_queues_tx_task_wakeup_request        (0xca0)
#define image_0_start_task_budget_allocator_1st_wakeup_request        (0xd48)
#define image_0_budget_allocator_1st_wakeup_request        (0xd48)
#define image_0_debug_routine_handler        (0x1)

#endif


#endif

/* IMAGE 1 LABELS */
#ifndef IMAGE_1_CODE_ADDRESSES
#define IMAGE_1_CODE_ADDRESSES

#ifndef PC_ADDRESS_INST_IND

#define image_1_start_task_initialization_task        (0x8)
#define image_1_initialization_task        (0x8)
#define image_1_start_task_processing_wakeup_request        (0xfe0)
#define image_1_processing_wakeup_request        (0xfe0)
#define image_1_start_task_debug_routine        (0x441c)
#define image_1_debug_routine        (0x441c)
#define image_1_start_task_cpu_rx_wakeup_request        (0x535c)
#define image_1_cpu_rx_wakeup_request        (0x535c)
#define image_1_start_task_cpu_recycle_wakeup_request        (0x5680)
#define image_1_cpu_recycle_wakeup_request        (0x5680)
#define image_1_start_task_interrupt_coalescing_1st_wakeup_request        (0x5894)
#define image_1_interrupt_coalescing_1st_wakeup_request        (0x5894)
#define image_1_start_task_cpu_rx_meter_budget_allocator_1st_wakeup_request        (0x57d4)
#define image_1_cpu_rx_meter_budget_allocator_1st_wakeup_request        (0x57d4)
#define image_1_start_task_cpu_rx_copy_wakeup_request        (0x4528)
#define image_1_cpu_rx_copy_wakeup_request        (0x4528)
#define image_1_start_task_spdsvc_gen_wakeup_request        (0x5958)
#define image_1_spdsvc_gen_wakeup_request        (0x5958)
#define image_1_start_task_common_reprocessing_wakeup_request        (0x6b28)
#define image_1_common_reprocessing_wakeup_request        (0x6b28)
#define image_1_debug_routine_handler        (0x4)
#define image_1_tcam_cmd_ic_submit        (0x352c)
#define image_1_tcam_cmd_outer_tpid        (0x3570)
#define image_1_tcam_cmd_inner_tpid        (0x3588)
#define image_1_tcam_cmd_ethertype        (0x35a0)
#define image_1_tcam_cmd_outer_vid        (0x35b8)
#define image_1_tcam_cmd_inner_vid        (0x35d0)
#define image_1_tcam_cmd_outer_pbit        (0x35e8)
#define image_1_tcam_cmd_inner_pbit        (0x3600)
#define image_1_tcam_cmd_vlan_num        (0x3618)
#define image_1_tcam_cmd_ip_protocol        (0x3630)
#define image_1_tcam_cmd_l3_protocol        (0x3648)
#define image_1_tcam_cmd_tos        (0x3660)
#define image_1_tcam_cmd_network_layer        (0x3678)
#define image_1_tcam_cmd_ingress_port        (0x3698)
#define image_1_tcam_cmd_gem_flow        (0x36b0)
#define image_1_tcam_cmd_ipv6_label        (0x36c8)
#define image_1_tcam_cmd_src_ip        (0x36f4)
#define image_1_tcam_cmd_src_ipv6_masked        (0x3748)
#define image_1_tcam_cmd_dst_ip        (0x377c)
#define image_1_tcam_cmd_dst_ipv6_masked        (0x37d0)
#define image_1_tcam_cmd_src_port        (0x3804)
#define image_1_tcam_cmd_dst_port        (0x381c)
#define image_1_tcam_cmd_src_mac        (0x3834)
#define image_1_tcam_cmd_dst_mac        (0x3860)
#define image_1_tcam_cmd_generic_l2        (0x388c)
#define image_1_tcam_cmd_generic_l3        (0x38c0)
#define image_1_tcam_cmd_generic_l4        (0x38f4)

#else

#define image_1_start_task_initialization_task        (0x2)
#define image_1_initialization_task        (0x2)
#define image_1_start_task_processing_wakeup_request        (0x3f8)
#define image_1_processing_wakeup_request        (0x3f8)
#define image_1_start_task_debug_routine        (0x1107)
#define image_1_debug_routine        (0x1107)
#define image_1_start_task_cpu_rx_wakeup_request        (0x14d7)
#define image_1_cpu_rx_wakeup_request        (0x14d7)
#define image_1_start_task_cpu_recycle_wakeup_request        (0x15a0)
#define image_1_cpu_recycle_wakeup_request        (0x15a0)
#define image_1_start_task_interrupt_coalescing_1st_wakeup_request        (0x1625)
#define image_1_interrupt_coalescing_1st_wakeup_request        (0x1625)
#define image_1_start_task_cpu_rx_meter_budget_allocator_1st_wakeup_request        (0x15f5)
#define image_1_cpu_rx_meter_budget_allocator_1st_wakeup_request        (0x15f5)
#define image_1_start_task_cpu_rx_copy_wakeup_request        (0x114a)
#define image_1_cpu_rx_copy_wakeup_request        (0x114a)
#define image_1_start_task_spdsvc_gen_wakeup_request        (0x1656)
#define image_1_spdsvc_gen_wakeup_request        (0x1656)
#define image_1_start_task_common_reprocessing_wakeup_request        (0x1aca)
#define image_1_common_reprocessing_wakeup_request        (0x1aca)
#define image_1_debug_routine_handler        (0x1)
#define image_1_tcam_cmd_ic_submit        (0xd4b)
#define image_1_tcam_cmd_outer_tpid        (0xd5c)
#define image_1_tcam_cmd_inner_tpid        (0xd62)
#define image_1_tcam_cmd_ethertype        (0xd68)
#define image_1_tcam_cmd_outer_vid        (0xd6e)
#define image_1_tcam_cmd_inner_vid        (0xd74)
#define image_1_tcam_cmd_outer_pbit        (0xd7a)
#define image_1_tcam_cmd_inner_pbit        (0xd80)
#define image_1_tcam_cmd_vlan_num        (0xd86)
#define image_1_tcam_cmd_ip_protocol        (0xd8c)
#define image_1_tcam_cmd_l3_protocol        (0xd92)
#define image_1_tcam_cmd_tos        (0xd98)
#define image_1_tcam_cmd_network_layer        (0xd9e)
#define image_1_tcam_cmd_ingress_port        (0xda6)
#define image_1_tcam_cmd_gem_flow        (0xdac)
#define image_1_tcam_cmd_ipv6_label        (0xdb2)
#define image_1_tcam_cmd_src_ip        (0xdbd)
#define image_1_tcam_cmd_src_ipv6_masked        (0xdd2)
#define image_1_tcam_cmd_dst_ip        (0xddf)
#define image_1_tcam_cmd_dst_ipv6_masked        (0xdf4)
#define image_1_tcam_cmd_src_port        (0xe01)
#define image_1_tcam_cmd_dst_port        (0xe07)
#define image_1_tcam_cmd_src_mac        (0xe0d)
#define image_1_tcam_cmd_dst_mac        (0xe18)
#define image_1_tcam_cmd_generic_l2        (0xe23)
#define image_1_tcam_cmd_generic_l3        (0xe30)
#define image_1_tcam_cmd_generic_l4        (0xe3d)

#endif


#endif

/* IMAGE 2 LABELS */
#ifndef IMAGE_2_CODE_ADDRESSES
#define IMAGE_2_CODE_ADDRESSES

#ifndef PC_ADDRESS_INST_IND

#define image_2_start_task_initialization_task        (0x8)
#define image_2_initialization_task        (0x8)
#define image_2_start_task_processing_wakeup_request        (0x10ec)
#define image_2_processing_wakeup_request        (0x10ec)
#define image_2_start_task_cpu_tx_wakeup_request        (0x4528)
#define image_2_cpu_tx_wakeup_request        (0x4528)
#define image_2_cpu_tx_read_ring_indices        (0x4528)
#define image_2_start_task_debug_routine        (0x48)
#define image_2_debug_routine        (0x48)
#define image_2_start_task_cpu_recycle_wakeup_request        (0x4b44)
#define image_2_cpu_recycle_wakeup_request        (0x4b44)
#define image_2_start_task_interrupt_coalescing_1st_wakeup_request        (0x4c98)
#define image_2_interrupt_coalescing_1st_wakeup_request        (0x4c98)
#define image_2_start_task_dhd_tx_post_wakeup_request        (0x51b8)
#define image_2_dhd_tx_post_wakeup_request        (0x51b8)
#define image_2_start_task_dhd_tx_post_update_fifo_wakeup_request        (0x5160)
#define image_2_dhd_tx_post_update_fifo_wakeup_request        (0x5160)
#define image_2_start_task_dhd_tx_complete_wakeup_request        (0x6270)
#define image_2_dhd_tx_complete_wakeup_request        (0x6270)
#define image_2_start_task_dhd_rx_complete_wakeup_request        (0x65b8)
#define image_2_dhd_rx_complete_wakeup_request        (0x65b8)
#define image_2_start_task_dhd_timer_1st_wakeup_request        (0x4d10)
#define image_2_dhd_timer_1st_wakeup_request        (0x4d10)
#define image_2_start_task_timer_common_task_wakeup_request        (0x6110)
#define image_2_timer_common_task_wakeup_request        (0x6110)
#define image_2_debug_routine_handler        (0x4)
#define image_2_tcam_cmd_ic_submit        (0x3638)
#define image_2_tcam_cmd_outer_tpid        (0x367c)
#define image_2_tcam_cmd_inner_tpid        (0x3694)
#define image_2_tcam_cmd_ethertype        (0x36ac)
#define image_2_tcam_cmd_outer_vid        (0x36c4)
#define image_2_tcam_cmd_inner_vid        (0x36dc)
#define image_2_tcam_cmd_outer_pbit        (0x36f4)
#define image_2_tcam_cmd_inner_pbit        (0x370c)
#define image_2_tcam_cmd_vlan_num        (0x3724)
#define image_2_tcam_cmd_ip_protocol        (0x373c)
#define image_2_tcam_cmd_l3_protocol        (0x3754)
#define image_2_tcam_cmd_tos        (0x376c)
#define image_2_tcam_cmd_network_layer        (0x3784)
#define image_2_tcam_cmd_ingress_port        (0x37a4)
#define image_2_tcam_cmd_gem_flow        (0x37bc)
#define image_2_tcam_cmd_ipv6_label        (0x37d4)
#define image_2_tcam_cmd_src_ip        (0x3800)
#define image_2_tcam_cmd_src_ipv6_masked        (0x3854)
#define image_2_tcam_cmd_dst_ip        (0x3888)
#define image_2_tcam_cmd_dst_ipv6_masked        (0x38dc)
#define image_2_tcam_cmd_src_port        (0x3910)
#define image_2_tcam_cmd_dst_port        (0x3928)
#define image_2_tcam_cmd_src_mac        (0x3940)
#define image_2_tcam_cmd_dst_mac        (0x396c)
#define image_2_tcam_cmd_generic_l2        (0x3998)
#define image_2_tcam_cmd_generic_l3        (0x39cc)
#define image_2_tcam_cmd_generic_l4        (0x3a00)

#else

#define image_2_start_task_initialization_task        (0x2)
#define image_2_initialization_task        (0x2)
#define image_2_start_task_processing_wakeup_request        (0x43b)
#define image_2_processing_wakeup_request        (0x43b)
#define image_2_start_task_cpu_tx_wakeup_request        (0x114a)
#define image_2_cpu_tx_wakeup_request        (0x114a)
#define image_2_cpu_tx_read_ring_indices        (0x114a)
#define image_2_start_task_debug_routine        (0x12)
#define image_2_debug_routine        (0x12)
#define image_2_start_task_cpu_recycle_wakeup_request        (0x12d1)
#define image_2_cpu_recycle_wakeup_request        (0x12d1)
#define image_2_start_task_interrupt_coalescing_1st_wakeup_request        (0x1326)
#define image_2_interrupt_coalescing_1st_wakeup_request        (0x1326)
#define image_2_start_task_dhd_tx_post_wakeup_request        (0x146e)
#define image_2_dhd_tx_post_wakeup_request        (0x146e)
#define image_2_start_task_dhd_tx_post_update_fifo_wakeup_request        (0x1458)
#define image_2_dhd_tx_post_update_fifo_wakeup_request        (0x1458)
#define image_2_start_task_dhd_tx_complete_wakeup_request        (0x189c)
#define image_2_dhd_tx_complete_wakeup_request        (0x189c)
#define image_2_start_task_dhd_rx_complete_wakeup_request        (0x196e)
#define image_2_dhd_rx_complete_wakeup_request        (0x196e)
#define image_2_start_task_dhd_timer_1st_wakeup_request        (0x1344)
#define image_2_dhd_timer_1st_wakeup_request        (0x1344)
#define image_2_start_task_timer_common_task_wakeup_request        (0x1844)
#define image_2_timer_common_task_wakeup_request        (0x1844)
#define image_2_debug_routine_handler        (0x1)
#define image_2_tcam_cmd_ic_submit        (0xd8e)
#define image_2_tcam_cmd_outer_tpid        (0xd9f)
#define image_2_tcam_cmd_inner_tpid        (0xda5)
#define image_2_tcam_cmd_ethertype        (0xdab)
#define image_2_tcam_cmd_outer_vid        (0xdb1)
#define image_2_tcam_cmd_inner_vid        (0xdb7)
#define image_2_tcam_cmd_outer_pbit        (0xdbd)
#define image_2_tcam_cmd_inner_pbit        (0xdc3)
#define image_2_tcam_cmd_vlan_num        (0xdc9)
#define image_2_tcam_cmd_ip_protocol        (0xdcf)
#define image_2_tcam_cmd_l3_protocol        (0xdd5)
#define image_2_tcam_cmd_tos        (0xddb)
#define image_2_tcam_cmd_network_layer        (0xde1)
#define image_2_tcam_cmd_ingress_port        (0xde9)
#define image_2_tcam_cmd_gem_flow        (0xdef)
#define image_2_tcam_cmd_ipv6_label        (0xdf5)
#define image_2_tcam_cmd_src_ip        (0xe00)
#define image_2_tcam_cmd_src_ipv6_masked        (0xe15)
#define image_2_tcam_cmd_dst_ip        (0xe22)
#define image_2_tcam_cmd_dst_ipv6_masked        (0xe37)
#define image_2_tcam_cmd_src_port        (0xe44)
#define image_2_tcam_cmd_dst_port        (0xe4a)
#define image_2_tcam_cmd_src_mac        (0xe50)
#define image_2_tcam_cmd_dst_mac        (0xe5b)
#define image_2_tcam_cmd_generic_l2        (0xe66)
#define image_2_tcam_cmd_generic_l3        (0xe73)
#define image_2_tcam_cmd_generic_l4        (0xe80)

#endif


#endif

/* COMMON LABELS */
#ifndef COMMON_CODE_ADDRESSES
#define COMMON_CODE_ADDRESSES

#define INVALID_LABEL_ADDRESS 0xFFFFFF

#ifndef PC_ADDRESS_INST_IND

#define TCAM_CMD_DST_IP_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x377c, 0x3888}
#define TCAM_CMD_DST_IPV6_MASKED_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x37d0, 0x38dc}
#define TCAM_CMD_DST_MAC_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3860, 0x396c}
#define TCAM_CMD_DST_PORT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x381c, 0x3928}
#define TCAM_CMD_ETHERTYPE_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x35a0, 0x36ac}
#define TCAM_CMD_GEM_FLOW_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x36b0, 0x37bc}
#define TCAM_CMD_GENERIC_L2_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x388c, 0x3998}
#define TCAM_CMD_GENERIC_L3_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x38c0, 0x39cc}
#define TCAM_CMD_GENERIC_L4_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x38f4, 0x3a00}
#define TCAM_CMD_IC_SUBMIT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x352c, 0x3638}
#define TCAM_CMD_INGRESS_PORT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3698, 0x37a4}
#define TCAM_CMD_INNER_PBIT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3600, 0x370c}
#define TCAM_CMD_INNER_TPID_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3588, 0x3694}
#define TCAM_CMD_INNER_VID_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x35d0, 0x36dc}
#define TCAM_CMD_IP_PROTOCOL_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3630, 0x373c}
#define TCAM_CMD_IPV6_LABEL_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x36c8, 0x37d4}
#define TCAM_CMD_L3_PROTOCOL_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3648, 0x3754}
#define TCAM_CMD_NETWORK_LAYER_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3678, 0x3784}
#define TCAM_CMD_OUTER_PBIT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x35e8, 0x36f4}
#define TCAM_CMD_OUTER_TPID_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3570, 0x367c}
#define TCAM_CMD_OUTER_VID_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x35b8, 0x36c4}
#define TCAM_CMD_SRC_IP_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x36f4, 0x3800}
#define TCAM_CMD_SRC_IPV6_MASKED_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3748, 0x3854}
#define TCAM_CMD_SRC_MAC_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3834, 0x3940}
#define TCAM_CMD_SRC_PORT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3804, 0x3910}
#define TCAM_CMD_TOS_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3660, 0x376c}
#define TCAM_CMD_VLAN_NUM_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3618, 0x3724}

#else

#define TCAM_CMD_DST_IP_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xddf, 0xe22}
#define TCAM_CMD_DST_IPV6_MASKED_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xdf4, 0xe37}
#define TCAM_CMD_DST_MAC_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xe18, 0xe5b}
#define TCAM_CMD_DST_PORT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xe07, 0xe4a}
#define TCAM_CMD_ETHERTYPE_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xd68, 0xdab}
#define TCAM_CMD_GEM_FLOW_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xdac, 0xdef}
#define TCAM_CMD_GENERIC_L2_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xe23, 0xe66}
#define TCAM_CMD_GENERIC_L3_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xe30, 0xe73}
#define TCAM_CMD_GENERIC_L4_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xe3d, 0xe80}
#define TCAM_CMD_IC_SUBMIT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xd4b, 0xd8e}
#define TCAM_CMD_INGRESS_PORT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xda6, 0xde9}
#define TCAM_CMD_INNER_PBIT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xd80, 0xdc3}
#define TCAM_CMD_INNER_TPID_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xd62, 0xda5}
#define TCAM_CMD_INNER_VID_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xd74, 0xdb7}
#define TCAM_CMD_IP_PROTOCOL_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xd8c, 0xdcf}
#define TCAM_CMD_IPV6_LABEL_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xdb2, 0xdf5}
#define TCAM_CMD_L3_PROTOCOL_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xd92, 0xdd5}
#define TCAM_CMD_NETWORK_LAYER_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xd9e, 0xde1}
#define TCAM_CMD_OUTER_PBIT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xd7a, 0xdbd}
#define TCAM_CMD_OUTER_TPID_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xd5c, 0xd9f}
#define TCAM_CMD_OUTER_VID_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xd6e, 0xdb1}
#define TCAM_CMD_SRC_IP_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xdbd, 0xe00}
#define TCAM_CMD_SRC_IPV6_MASKED_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xdd2, 0xe15}
#define TCAM_CMD_SRC_MAC_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xe0d, 0xe50}
#define TCAM_CMD_SRC_PORT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xe01, 0xe44}
#define TCAM_CMD_TOS_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xd98, 0xddb}
#define TCAM_CMD_VLAN_NUM_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xd86, 0xdc9}

#endif


#endif

