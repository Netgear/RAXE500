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
#define image_0_start_task_buffer_cong_mgt_1st_wakeup_request        (0x14cc)
#define image_0_buffer_cong_mgt_1st_wakeup_request        (0x14cc)
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
#define image_0_start_task_buffer_cong_mgt_1st_wakeup_request        (0x533)
#define image_0_buffer_cong_mgt_1st_wakeup_request        (0x533)
#define image_0_debug_routine_handler        (0x1)

#endif


#endif

/* IMAGE 1 LABELS */
#ifndef IMAGE_1_CODE_ADDRESSES
#define IMAGE_1_CODE_ADDRESSES

#ifndef PC_ADDRESS_INST_IND

#define image_1_start_task_initialization_task        (0x8)
#define image_1_initialization_task        (0x8)
#define image_1_start_task_processing_wakeup_request        (0x1010)
#define image_1_processing_wakeup_request        (0x1010)
#define image_1_start_task_debug_routine        (0x4588)
#define image_1_debug_routine        (0x4588)
#define image_1_start_task_cpu_rx_wakeup_request        (0x54c8)
#define image_1_cpu_rx_wakeup_request        (0x54c8)
#define image_1_start_task_cpu_recycle_wakeup_request        (0x58ac)
#define image_1_cpu_recycle_wakeup_request        (0x58ac)
#define image_1_start_task_interrupt_coalescing_1st_wakeup_request        (0x5a00)
#define image_1_interrupt_coalescing_1st_wakeup_request        (0x5a00)
#define image_1_start_task_cpu_rx_meter_budget_allocator_1st_wakeup_request        (0x57ec)
#define image_1_cpu_rx_meter_budget_allocator_1st_wakeup_request        (0x57ec)
#define image_1_start_task_cpu_rx_copy_wakeup_request        (0x4694)
#define image_1_cpu_rx_copy_wakeup_request        (0x4694)
#define image_1_start_task_spdsvc_gen_wakeup_request        (0x5ac4)
#define image_1_spdsvc_gen_wakeup_request        (0x5ac4)
#define image_1_start_task_common_reprocessing_wakeup_request        (0x6c94)
#define image_1_common_reprocessing_wakeup_request        (0x6c94)
#define image_1_debug_routine_handler        (0x4)
#define image_1_processing_header_update_length_calc        (0x19a0)
#define image_1_processing_layer2_header_copy_30_bytes_8_bytes_align        (0x1a88)
#define image_1_processing_layer2_header_copy_22_bytes_8_bytes_align        (0x1a90)
#define image_1_processing_layer2_header_copy_14_bytes_8_bytes_align        (0x1a98)
#define image_1_processing_layer2_header_copy_30_bytes_4_bytes_align        (0x1aac)
#define image_1_processing_layer2_header_copy_22_bytes_4_bytes_align        (0x1abc)
#define image_1_processing_layer2_header_copy_14_bytes_4_bytes_align        (0x1acc)
#define image_1_processing_layer2_header_copy_26_bytes_8_bytes_align        (0x1aec)
#define image_1_processing_layer2_header_copy_18_bytes_8_bytes_align        (0x1af8)
#define image_1_processing_layer2_header_copy_18_bytes_4_bytes_align        (0x1b1c)
#define image_1_processing_layer2_header_copy_26_bytes_4_bytes_align        (0x1b28)
#define image_1_tcam_cmd_ic_submit        (0x369c)
#define image_1_tcam_cmd_outer_tpid        (0x36e0)
#define image_1_tcam_cmd_inner_tpid        (0x36f8)
#define image_1_tcam_cmd_ethertype        (0x3710)
#define image_1_tcam_cmd_outer_vid        (0x3728)
#define image_1_tcam_cmd_inner_vid        (0x3740)
#define image_1_tcam_cmd_outer_pbit        (0x3758)
#define image_1_tcam_cmd_inner_pbit        (0x3770)
#define image_1_tcam_cmd_vlan_num        (0x3788)
#define image_1_tcam_cmd_ip_protocol        (0x37a0)
#define image_1_tcam_cmd_l3_protocol        (0x37b8)
#define image_1_tcam_cmd_tos        (0x37d0)
#define image_1_tcam_cmd_network_layer        (0x37e8)
#define image_1_tcam_cmd_ingress_port        (0x3808)
#define image_1_tcam_cmd_gem_flow        (0x3820)
#define image_1_tcam_cmd_ipv6_label        (0x3838)
#define image_1_tcam_cmd_src_ip        (0x3864)
#define image_1_tcam_cmd_src_ipv6_masked        (0x38b8)
#define image_1_tcam_cmd_dst_ip        (0x38ec)
#define image_1_tcam_cmd_dst_ipv6_masked        (0x3940)
#define image_1_tcam_cmd_src_port        (0x3974)
#define image_1_tcam_cmd_dst_port        (0x398c)
#define image_1_tcam_cmd_src_mac        (0x39a4)
#define image_1_tcam_cmd_dst_mac        (0x39d0)
#define image_1_tcam_cmd_generic_l2        (0x39fc)
#define image_1_tcam_cmd_generic_l3        (0x3a30)
#define image_1_tcam_cmd_generic_l4        (0x3a64)

#else

#define image_1_start_task_initialization_task        (0x2)
#define image_1_initialization_task        (0x2)
#define image_1_start_task_processing_wakeup_request        (0x404)
#define image_1_processing_wakeup_request        (0x404)
#define image_1_start_task_debug_routine        (0x1162)
#define image_1_debug_routine        (0x1162)
#define image_1_start_task_cpu_rx_wakeup_request        (0x1532)
#define image_1_cpu_rx_wakeup_request        (0x1532)
#define image_1_start_task_cpu_recycle_wakeup_request        (0x162b)
#define image_1_cpu_recycle_wakeup_request        (0x162b)
#define image_1_start_task_interrupt_coalescing_1st_wakeup_request        (0x1680)
#define image_1_interrupt_coalescing_1st_wakeup_request        (0x1680)
#define image_1_start_task_cpu_rx_meter_budget_allocator_1st_wakeup_request        (0x15fb)
#define image_1_cpu_rx_meter_budget_allocator_1st_wakeup_request        (0x15fb)
#define image_1_start_task_cpu_rx_copy_wakeup_request        (0x11a5)
#define image_1_cpu_rx_copy_wakeup_request        (0x11a5)
#define image_1_start_task_spdsvc_gen_wakeup_request        (0x16b1)
#define image_1_spdsvc_gen_wakeup_request        (0x16b1)
#define image_1_start_task_common_reprocessing_wakeup_request        (0x1b25)
#define image_1_common_reprocessing_wakeup_request        (0x1b25)
#define image_1_debug_routine_handler        (0x1)
#define image_1_processing_header_update_length_calc        (0x668)
#define image_1_processing_layer2_header_copy_30_bytes_8_bytes_align        (0x6a2)
#define image_1_processing_layer2_header_copy_22_bytes_8_bytes_align        (0x6a4)
#define image_1_processing_layer2_header_copy_14_bytes_8_bytes_align        (0x6a6)
#define image_1_processing_layer2_header_copy_30_bytes_4_bytes_align        (0x6ab)
#define image_1_processing_layer2_header_copy_22_bytes_4_bytes_align        (0x6af)
#define image_1_processing_layer2_header_copy_14_bytes_4_bytes_align        (0x6b3)
#define image_1_processing_layer2_header_copy_26_bytes_8_bytes_align        (0x6bb)
#define image_1_processing_layer2_header_copy_18_bytes_8_bytes_align        (0x6be)
#define image_1_processing_layer2_header_copy_18_bytes_4_bytes_align        (0x6c7)
#define image_1_processing_layer2_header_copy_26_bytes_4_bytes_align        (0x6ca)
#define image_1_tcam_cmd_ic_submit        (0xda7)
#define image_1_tcam_cmd_outer_tpid        (0xdb8)
#define image_1_tcam_cmd_inner_tpid        (0xdbe)
#define image_1_tcam_cmd_ethertype        (0xdc4)
#define image_1_tcam_cmd_outer_vid        (0xdca)
#define image_1_tcam_cmd_inner_vid        (0xdd0)
#define image_1_tcam_cmd_outer_pbit        (0xdd6)
#define image_1_tcam_cmd_inner_pbit        (0xddc)
#define image_1_tcam_cmd_vlan_num        (0xde2)
#define image_1_tcam_cmd_ip_protocol        (0xde8)
#define image_1_tcam_cmd_l3_protocol        (0xdee)
#define image_1_tcam_cmd_tos        (0xdf4)
#define image_1_tcam_cmd_network_layer        (0xdfa)
#define image_1_tcam_cmd_ingress_port        (0xe02)
#define image_1_tcam_cmd_gem_flow        (0xe08)
#define image_1_tcam_cmd_ipv6_label        (0xe0e)
#define image_1_tcam_cmd_src_ip        (0xe19)
#define image_1_tcam_cmd_src_ipv6_masked        (0xe2e)
#define image_1_tcam_cmd_dst_ip        (0xe3b)
#define image_1_tcam_cmd_dst_ipv6_masked        (0xe50)
#define image_1_tcam_cmd_src_port        (0xe5d)
#define image_1_tcam_cmd_dst_port        (0xe63)
#define image_1_tcam_cmd_src_mac        (0xe69)
#define image_1_tcam_cmd_dst_mac        (0xe74)
#define image_1_tcam_cmd_generic_l2        (0xe7f)
#define image_1_tcam_cmd_generic_l3        (0xe8c)
#define image_1_tcam_cmd_generic_l4        (0xe99)

#endif


#endif

/* IMAGE 2 LABELS */
#ifndef IMAGE_2_CODE_ADDRESSES
#define IMAGE_2_CODE_ADDRESSES

#ifndef PC_ADDRESS_INST_IND

#define image_2_start_task_initialization_task        (0x8)
#define image_2_initialization_task        (0x8)
#define image_2_start_task_processing_wakeup_request        (0x111c)
#define image_2_processing_wakeup_request        (0x111c)
#define image_2_start_task_cpu_tx_wakeup_request        (0x4694)
#define image_2_cpu_tx_wakeup_request        (0x4694)
#define image_2_cpu_tx_read_ring_indices        (0x4694)
#define image_2_start_task_debug_routine        (0x48)
#define image_2_debug_routine        (0x48)
#define image_2_start_task_cpu_recycle_wakeup_request        (0x4db0)
#define image_2_cpu_recycle_wakeup_request        (0x4db0)
#define image_2_start_task_interrupt_coalescing_1st_wakeup_request        (0x5064)
#define image_2_interrupt_coalescing_1st_wakeup_request        (0x5064)
#define image_2_start_task_timer_common_task_wakeup_request        (0x4f04)
#define image_2_timer_common_task_wakeup_request        (0x4f04)
#define image_2_debug_routine_handler        (0x4)
#define image_2_processing_header_update_length_calc        (0x1aac)
#define image_2_processing_layer2_header_copy_30_bytes_8_bytes_align        (0x1b94)
#define image_2_processing_layer2_header_copy_22_bytes_8_bytes_align        (0x1b9c)
#define image_2_processing_layer2_header_copy_14_bytes_8_bytes_align        (0x1ba4)
#define image_2_processing_layer2_header_copy_30_bytes_4_bytes_align        (0x1bb8)
#define image_2_processing_layer2_header_copy_22_bytes_4_bytes_align        (0x1bc8)
#define image_2_processing_layer2_header_copy_14_bytes_4_bytes_align        (0x1bd8)
#define image_2_processing_layer2_header_copy_26_bytes_8_bytes_align        (0x1bf8)
#define image_2_processing_layer2_header_copy_18_bytes_8_bytes_align        (0x1c04)
#define image_2_processing_layer2_header_copy_18_bytes_4_bytes_align        (0x1c28)
#define image_2_processing_layer2_header_copy_26_bytes_4_bytes_align        (0x1c34)
#define image_2_tcam_cmd_ic_submit        (0x37a8)
#define image_2_tcam_cmd_outer_tpid        (0x37ec)
#define image_2_tcam_cmd_inner_tpid        (0x3804)
#define image_2_tcam_cmd_ethertype        (0x381c)
#define image_2_tcam_cmd_outer_vid        (0x3834)
#define image_2_tcam_cmd_inner_vid        (0x384c)
#define image_2_tcam_cmd_outer_pbit        (0x3864)
#define image_2_tcam_cmd_inner_pbit        (0x387c)
#define image_2_tcam_cmd_vlan_num        (0x3894)
#define image_2_tcam_cmd_ip_protocol        (0x38ac)
#define image_2_tcam_cmd_l3_protocol        (0x38c4)
#define image_2_tcam_cmd_tos        (0x38dc)
#define image_2_tcam_cmd_network_layer        (0x38f4)
#define image_2_tcam_cmd_ingress_port        (0x3914)
#define image_2_tcam_cmd_gem_flow        (0x392c)
#define image_2_tcam_cmd_ipv6_label        (0x3944)
#define image_2_tcam_cmd_src_ip        (0x3970)
#define image_2_tcam_cmd_src_ipv6_masked        (0x39c4)
#define image_2_tcam_cmd_dst_ip        (0x39f8)
#define image_2_tcam_cmd_dst_ipv6_masked        (0x3a4c)
#define image_2_tcam_cmd_src_port        (0x3a80)
#define image_2_tcam_cmd_dst_port        (0x3a98)
#define image_2_tcam_cmd_src_mac        (0x3ab0)
#define image_2_tcam_cmd_dst_mac        (0x3adc)
#define image_2_tcam_cmd_generic_l2        (0x3b08)
#define image_2_tcam_cmd_generic_l3        (0x3b3c)
#define image_2_tcam_cmd_generic_l4        (0x3b70)

#else

#define image_2_start_task_initialization_task        (0x2)
#define image_2_initialization_task        (0x2)
#define image_2_start_task_processing_wakeup_request        (0x447)
#define image_2_processing_wakeup_request        (0x447)
#define image_2_start_task_cpu_tx_wakeup_request        (0x11a5)
#define image_2_cpu_tx_wakeup_request        (0x11a5)
#define image_2_cpu_tx_read_ring_indices        (0x11a5)
#define image_2_start_task_debug_routine        (0x12)
#define image_2_debug_routine        (0x12)
#define image_2_start_task_cpu_recycle_wakeup_request        (0x136c)
#define image_2_cpu_recycle_wakeup_request        (0x136c)
#define image_2_start_task_interrupt_coalescing_1st_wakeup_request        (0x1419)
#define image_2_interrupt_coalescing_1st_wakeup_request        (0x1419)
#define image_2_start_task_timer_common_task_wakeup_request        (0x13c1)
#define image_2_timer_common_task_wakeup_request        (0x13c1)
#define image_2_debug_routine_handler        (0x1)
#define image_2_processing_header_update_length_calc        (0x6ab)
#define image_2_processing_layer2_header_copy_30_bytes_8_bytes_align        (0x6e5)
#define image_2_processing_layer2_header_copy_22_bytes_8_bytes_align        (0x6e7)
#define image_2_processing_layer2_header_copy_14_bytes_8_bytes_align        (0x6e9)
#define image_2_processing_layer2_header_copy_30_bytes_4_bytes_align        (0x6ee)
#define image_2_processing_layer2_header_copy_22_bytes_4_bytes_align        (0x6f2)
#define image_2_processing_layer2_header_copy_14_bytes_4_bytes_align        (0x6f6)
#define image_2_processing_layer2_header_copy_26_bytes_8_bytes_align        (0x6fe)
#define image_2_processing_layer2_header_copy_18_bytes_8_bytes_align        (0x701)
#define image_2_processing_layer2_header_copy_18_bytes_4_bytes_align        (0x70a)
#define image_2_processing_layer2_header_copy_26_bytes_4_bytes_align        (0x70d)
#define image_2_tcam_cmd_ic_submit        (0xdea)
#define image_2_tcam_cmd_outer_tpid        (0xdfb)
#define image_2_tcam_cmd_inner_tpid        (0xe01)
#define image_2_tcam_cmd_ethertype        (0xe07)
#define image_2_tcam_cmd_outer_vid        (0xe0d)
#define image_2_tcam_cmd_inner_vid        (0xe13)
#define image_2_tcam_cmd_outer_pbit        (0xe19)
#define image_2_tcam_cmd_inner_pbit        (0xe1f)
#define image_2_tcam_cmd_vlan_num        (0xe25)
#define image_2_tcam_cmd_ip_protocol        (0xe2b)
#define image_2_tcam_cmd_l3_protocol        (0xe31)
#define image_2_tcam_cmd_tos        (0xe37)
#define image_2_tcam_cmd_network_layer        (0xe3d)
#define image_2_tcam_cmd_ingress_port        (0xe45)
#define image_2_tcam_cmd_gem_flow        (0xe4b)
#define image_2_tcam_cmd_ipv6_label        (0xe51)
#define image_2_tcam_cmd_src_ip        (0xe5c)
#define image_2_tcam_cmd_src_ipv6_masked        (0xe71)
#define image_2_tcam_cmd_dst_ip        (0xe7e)
#define image_2_tcam_cmd_dst_ipv6_masked        (0xe93)
#define image_2_tcam_cmd_src_port        (0xea0)
#define image_2_tcam_cmd_dst_port        (0xea6)
#define image_2_tcam_cmd_src_mac        (0xeac)
#define image_2_tcam_cmd_dst_mac        (0xeb7)
#define image_2_tcam_cmd_generic_l2        (0xec2)
#define image_2_tcam_cmd_generic_l3        (0xecf)
#define image_2_tcam_cmd_generic_l4        (0xedc)

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
#define image_3_start_task_epon_update_fifo_read_1st_wakeup_request        (0x1fac)
#define image_3_epon_update_fifo_read_1st_wakeup_request        (0x1fac)
#define image_3_start_task_epon_tx_task_wakeup_request        (0x1444)
#define image_3_epon_tx_task_wakeup_request        (0x1444)
#define image_3_start_task_budget_allocator_1st_wakeup_request        (0x15f8)
#define image_3_budget_allocator_1st_wakeup_request        (0x15f8)
#define image_3_start_task_debug_routine        (0x48)
#define image_3_debug_routine        (0x48)
#define image_3_start_task_gpon_control_wakeup_request        (0x5ac)
#define image_3_gpon_control_wakeup_request        (0x5ac)
#define image_3_start_task_ovl_budget_allocator_1st_wakeup_request        (0x195c)
#define image_3_ovl_budget_allocator_1st_wakeup_request        (0x195c)
#define image_3_start_task_flush_task_1st_wakeup_request        (0x1b0c)
#define image_3_flush_task_1st_wakeup_request        (0x1b0c)
#define image_3_start_task_update_fifo_us_read_1st_wakeup_request        (0x1c88)
#define image_3_update_fifo_us_read_1st_wakeup_request        (0x1c88)
#define image_3_start_task_buffer_cong_mgt_1st_wakeup_request        (0x201c)
#define image_3_buffer_cong_mgt_1st_wakeup_request        (0x201c)
#define image_3_debug_routine_handler        (0x4)

#else

#define image_3_start_task_initialization_task        (0x2)
#define image_3_initialization_task        (0x2)
#define image_3_start_task_us_tx_task_1st_wakeup_request        (0x35a)
#define image_3_us_tx_task_1st_wakeup_request        (0x35a)
#define image_3_us_tx_task_wakeup_request        (0x35a)
#define image_3_start_task_epon_update_fifo_read_1st_wakeup_request        (0x7eb)
#define image_3_epon_update_fifo_read_1st_wakeup_request        (0x7eb)
#define image_3_start_task_epon_tx_task_wakeup_request        (0x511)
#define image_3_epon_tx_task_wakeup_request        (0x511)
#define image_3_start_task_budget_allocator_1st_wakeup_request        (0x57e)
#define image_3_budget_allocator_1st_wakeup_request        (0x57e)
#define image_3_start_task_debug_routine        (0x12)
#define image_3_debug_routine        (0x12)
#define image_3_start_task_gpon_control_wakeup_request        (0x16b)
#define image_3_gpon_control_wakeup_request        (0x16b)
#define image_3_start_task_ovl_budget_allocator_1st_wakeup_request        (0x657)
#define image_3_ovl_budget_allocator_1st_wakeup_request        (0x657)
#define image_3_start_task_flush_task_1st_wakeup_request        (0x6c3)
#define image_3_flush_task_1st_wakeup_request        (0x6c3)
#define image_3_start_task_update_fifo_us_read_1st_wakeup_request        (0x722)
#define image_3_update_fifo_us_read_1st_wakeup_request        (0x722)
#define image_3_start_task_buffer_cong_mgt_1st_wakeup_request        (0x807)
#define image_3_buffer_cong_mgt_1st_wakeup_request        (0x807)
#define image_3_debug_routine_handler        (0x1)

#endif


#endif

/* IMAGE 4 LABELS */
#ifndef IMAGE_4_CODE_ADDRESSES
#define IMAGE_4_CODE_ADDRESSES

#ifndef PC_ADDRESS_INST_IND

#define image_4_start_task_initialization_task        (0x8)
#define image_4_initialization_task        (0x8)
#define image_4_start_task_processing_wakeup_request        (0x111c)
#define image_4_processing_wakeup_request        (0x111c)
#define image_4_start_task_debug_routine        (0x48)
#define image_4_debug_routine        (0x48)
#define image_4_start_task_dhd_tx_complete_wakeup_request        (0x4694)
#define image_4_dhd_tx_complete_wakeup_request        (0x4694)
#define image_4_start_task_dhd_rx_complete_wakeup_request        (0x49dc)
#define image_4_dhd_rx_complete_wakeup_request        (0x49dc)
#define image_4_debug_routine_handler        (0x4)
#define image_4_processing_header_update_length_calc        (0x1aac)
#define image_4_processing_layer2_header_copy_30_bytes_8_bytes_align        (0x1b94)
#define image_4_processing_layer2_header_copy_22_bytes_8_bytes_align        (0x1b9c)
#define image_4_processing_layer2_header_copy_14_bytes_8_bytes_align        (0x1ba4)
#define image_4_processing_layer2_header_copy_30_bytes_4_bytes_align        (0x1bb8)
#define image_4_processing_layer2_header_copy_22_bytes_4_bytes_align        (0x1bc8)
#define image_4_processing_layer2_header_copy_14_bytes_4_bytes_align        (0x1bd8)
#define image_4_processing_layer2_header_copy_26_bytes_8_bytes_align        (0x1bf8)
#define image_4_processing_layer2_header_copy_18_bytes_8_bytes_align        (0x1c04)
#define image_4_processing_layer2_header_copy_18_bytes_4_bytes_align        (0x1c28)
#define image_4_processing_layer2_header_copy_26_bytes_4_bytes_align        (0x1c34)
#define image_4_tcam_cmd_ic_submit        (0x37a8)
#define image_4_tcam_cmd_outer_tpid        (0x37ec)
#define image_4_tcam_cmd_inner_tpid        (0x3804)
#define image_4_tcam_cmd_ethertype        (0x381c)
#define image_4_tcam_cmd_outer_vid        (0x3834)
#define image_4_tcam_cmd_inner_vid        (0x384c)
#define image_4_tcam_cmd_outer_pbit        (0x3864)
#define image_4_tcam_cmd_inner_pbit        (0x387c)
#define image_4_tcam_cmd_vlan_num        (0x3894)
#define image_4_tcam_cmd_ip_protocol        (0x38ac)
#define image_4_tcam_cmd_l3_protocol        (0x38c4)
#define image_4_tcam_cmd_tos        (0x38dc)
#define image_4_tcam_cmd_network_layer        (0x38f4)
#define image_4_tcam_cmd_ingress_port        (0x3914)
#define image_4_tcam_cmd_gem_flow        (0x392c)
#define image_4_tcam_cmd_ipv6_label        (0x3944)
#define image_4_tcam_cmd_src_ip        (0x3970)
#define image_4_tcam_cmd_src_ipv6_masked        (0x39c4)
#define image_4_tcam_cmd_dst_ip        (0x39f8)
#define image_4_tcam_cmd_dst_ipv6_masked        (0x3a4c)
#define image_4_tcam_cmd_src_port        (0x3a80)
#define image_4_tcam_cmd_dst_port        (0x3a98)
#define image_4_tcam_cmd_src_mac        (0x3ab0)
#define image_4_tcam_cmd_dst_mac        (0x3adc)
#define image_4_tcam_cmd_generic_l2        (0x3b08)
#define image_4_tcam_cmd_generic_l3        (0x3b3c)
#define image_4_tcam_cmd_generic_l4        (0x3b70)

#else

#define image_4_start_task_initialization_task        (0x2)
#define image_4_initialization_task        (0x2)
#define image_4_start_task_processing_wakeup_request        (0x447)
#define image_4_processing_wakeup_request        (0x447)
#define image_4_start_task_debug_routine        (0x12)
#define image_4_debug_routine        (0x12)
#define image_4_start_task_dhd_tx_complete_wakeup_request        (0x11a5)
#define image_4_dhd_tx_complete_wakeup_request        (0x11a5)
#define image_4_start_task_dhd_rx_complete_wakeup_request        (0x1277)
#define image_4_dhd_rx_complete_wakeup_request        (0x1277)
#define image_4_debug_routine_handler        (0x1)
#define image_4_processing_header_update_length_calc        (0x6ab)
#define image_4_processing_layer2_header_copy_30_bytes_8_bytes_align        (0x6e5)
#define image_4_processing_layer2_header_copy_22_bytes_8_bytes_align        (0x6e7)
#define image_4_processing_layer2_header_copy_14_bytes_8_bytes_align        (0x6e9)
#define image_4_processing_layer2_header_copy_30_bytes_4_bytes_align        (0x6ee)
#define image_4_processing_layer2_header_copy_22_bytes_4_bytes_align        (0x6f2)
#define image_4_processing_layer2_header_copy_14_bytes_4_bytes_align        (0x6f6)
#define image_4_processing_layer2_header_copy_26_bytes_8_bytes_align        (0x6fe)
#define image_4_processing_layer2_header_copy_18_bytes_8_bytes_align        (0x701)
#define image_4_processing_layer2_header_copy_18_bytes_4_bytes_align        (0x70a)
#define image_4_processing_layer2_header_copy_26_bytes_4_bytes_align        (0x70d)
#define image_4_tcam_cmd_ic_submit        (0xdea)
#define image_4_tcam_cmd_outer_tpid        (0xdfb)
#define image_4_tcam_cmd_inner_tpid        (0xe01)
#define image_4_tcam_cmd_ethertype        (0xe07)
#define image_4_tcam_cmd_outer_vid        (0xe0d)
#define image_4_tcam_cmd_inner_vid        (0xe13)
#define image_4_tcam_cmd_outer_pbit        (0xe19)
#define image_4_tcam_cmd_inner_pbit        (0xe1f)
#define image_4_tcam_cmd_vlan_num        (0xe25)
#define image_4_tcam_cmd_ip_protocol        (0xe2b)
#define image_4_tcam_cmd_l3_protocol        (0xe31)
#define image_4_tcam_cmd_tos        (0xe37)
#define image_4_tcam_cmd_network_layer        (0xe3d)
#define image_4_tcam_cmd_ingress_port        (0xe45)
#define image_4_tcam_cmd_gem_flow        (0xe4b)
#define image_4_tcam_cmd_ipv6_label        (0xe51)
#define image_4_tcam_cmd_src_ip        (0xe5c)
#define image_4_tcam_cmd_src_ipv6_masked        (0xe71)
#define image_4_tcam_cmd_dst_ip        (0xe7e)
#define image_4_tcam_cmd_dst_ipv6_masked        (0xe93)
#define image_4_tcam_cmd_src_port        (0xea0)
#define image_4_tcam_cmd_dst_port        (0xea6)
#define image_4_tcam_cmd_src_mac        (0xeac)
#define image_4_tcam_cmd_dst_mac        (0xeb7)
#define image_4_tcam_cmd_generic_l2        (0xec2)
#define image_4_tcam_cmd_generic_l3        (0xecf)
#define image_4_tcam_cmd_generic_l4        (0xedc)

#endif


#endif

/* IMAGE 5 LABELS */
#ifndef IMAGE_5_CODE_ADDRESSES
#define IMAGE_5_CODE_ADDRESSES

#ifndef PC_ADDRESS_INST_IND

#define image_5_start_task_initialization_task        (0x8)
#define image_5_initialization_task        (0x8)
#define image_5_start_task_processing_wakeup_request        (0x1278)
#define image_5_processing_wakeup_request        (0x1278)
#define image_5_start_task_budget_allocator_1st_wakeup_request        (0x4f98)
#define image_5_budget_allocator_1st_wakeup_request        (0x4f98)
#define image_5_start_task_debug_routine        (0x47f0)
#define image_5_debug_routine        (0x47f0)
#define image_5_start_task_service_queues_update_fifo_read_1st_wakeup_request        (0x48fc)
#define image_5_service_queues_update_fifo_read_1st_wakeup_request        (0x48fc)
#define image_5_start_task_service_queues_tx_task_wakeup_request        (0x4bf8)
#define image_5_service_queues_tx_task_wakeup_request        (0x4bf8)
#define image_5_debug_routine_handler        (0x4)
#define image_5_processing_header_update_length_calc        (0x1c08)
#define image_5_processing_layer2_header_copy_30_bytes_8_bytes_align        (0x1cf0)
#define image_5_processing_layer2_header_copy_22_bytes_8_bytes_align        (0x1cf8)
#define image_5_processing_layer2_header_copy_14_bytes_8_bytes_align        (0x1d00)
#define image_5_processing_layer2_header_copy_30_bytes_4_bytes_align        (0x1d14)
#define image_5_processing_layer2_header_copy_22_bytes_4_bytes_align        (0x1d24)
#define image_5_processing_layer2_header_copy_14_bytes_4_bytes_align        (0x1d34)
#define image_5_processing_layer2_header_copy_26_bytes_8_bytes_align        (0x1d54)
#define image_5_processing_layer2_header_copy_18_bytes_8_bytes_align        (0x1d60)
#define image_5_processing_layer2_header_copy_18_bytes_4_bytes_align        (0x1d84)
#define image_5_processing_layer2_header_copy_26_bytes_4_bytes_align        (0x1d90)
#define image_5_tcam_cmd_ic_submit        (0x3904)
#define image_5_tcam_cmd_outer_tpid        (0x3948)
#define image_5_tcam_cmd_inner_tpid        (0x3960)
#define image_5_tcam_cmd_ethertype        (0x3978)
#define image_5_tcam_cmd_outer_vid        (0x3990)
#define image_5_tcam_cmd_inner_vid        (0x39a8)
#define image_5_tcam_cmd_outer_pbit        (0x39c0)
#define image_5_tcam_cmd_inner_pbit        (0x39d8)
#define image_5_tcam_cmd_vlan_num        (0x39f0)
#define image_5_tcam_cmd_ip_protocol        (0x3a08)
#define image_5_tcam_cmd_l3_protocol        (0x3a20)
#define image_5_tcam_cmd_tos        (0x3a38)
#define image_5_tcam_cmd_network_layer        (0x3a50)
#define image_5_tcam_cmd_ingress_port        (0x3a70)
#define image_5_tcam_cmd_gem_flow        (0x3a88)
#define image_5_tcam_cmd_ipv6_label        (0x3aa0)
#define image_5_tcam_cmd_src_ip        (0x3acc)
#define image_5_tcam_cmd_src_ipv6_masked        (0x3b20)
#define image_5_tcam_cmd_dst_ip        (0x3b54)
#define image_5_tcam_cmd_dst_ipv6_masked        (0x3ba8)
#define image_5_tcam_cmd_src_port        (0x3bdc)
#define image_5_tcam_cmd_dst_port        (0x3bf4)
#define image_5_tcam_cmd_src_mac        (0x3c0c)
#define image_5_tcam_cmd_dst_mac        (0x3c38)
#define image_5_tcam_cmd_generic_l2        (0x3c64)
#define image_5_tcam_cmd_generic_l3        (0x3c98)
#define image_5_tcam_cmd_generic_l4        (0x3ccc)

#else

#define image_5_start_task_initialization_task        (0x2)
#define image_5_initialization_task        (0x2)
#define image_5_start_task_processing_wakeup_request        (0x49e)
#define image_5_processing_wakeup_request        (0x49e)
#define image_5_start_task_budget_allocator_1st_wakeup_request        (0x13e6)
#define image_5_budget_allocator_1st_wakeup_request        (0x13e6)
#define image_5_start_task_debug_routine        (0x11fc)
#define image_5_debug_routine        (0x11fc)
#define image_5_start_task_service_queues_update_fifo_read_1st_wakeup_request        (0x123f)
#define image_5_service_queues_update_fifo_read_1st_wakeup_request        (0x123f)
#define image_5_start_task_service_queues_tx_task_wakeup_request        (0x12fe)
#define image_5_service_queues_tx_task_wakeup_request        (0x12fe)
#define image_5_debug_routine_handler        (0x1)
#define image_5_processing_header_update_length_calc        (0x702)
#define image_5_processing_layer2_header_copy_30_bytes_8_bytes_align        (0x73c)
#define image_5_processing_layer2_header_copy_22_bytes_8_bytes_align        (0x73e)
#define image_5_processing_layer2_header_copy_14_bytes_8_bytes_align        (0x740)
#define image_5_processing_layer2_header_copy_30_bytes_4_bytes_align        (0x745)
#define image_5_processing_layer2_header_copy_22_bytes_4_bytes_align        (0x749)
#define image_5_processing_layer2_header_copy_14_bytes_4_bytes_align        (0x74d)
#define image_5_processing_layer2_header_copy_26_bytes_8_bytes_align        (0x755)
#define image_5_processing_layer2_header_copy_18_bytes_8_bytes_align        (0x758)
#define image_5_processing_layer2_header_copy_18_bytes_4_bytes_align        (0x761)
#define image_5_processing_layer2_header_copy_26_bytes_4_bytes_align        (0x764)
#define image_5_tcam_cmd_ic_submit        (0xe41)
#define image_5_tcam_cmd_outer_tpid        (0xe52)
#define image_5_tcam_cmd_inner_tpid        (0xe58)
#define image_5_tcam_cmd_ethertype        (0xe5e)
#define image_5_tcam_cmd_outer_vid        (0xe64)
#define image_5_tcam_cmd_inner_vid        (0xe6a)
#define image_5_tcam_cmd_outer_pbit        (0xe70)
#define image_5_tcam_cmd_inner_pbit        (0xe76)
#define image_5_tcam_cmd_vlan_num        (0xe7c)
#define image_5_tcam_cmd_ip_protocol        (0xe82)
#define image_5_tcam_cmd_l3_protocol        (0xe88)
#define image_5_tcam_cmd_tos        (0xe8e)
#define image_5_tcam_cmd_network_layer        (0xe94)
#define image_5_tcam_cmd_ingress_port        (0xe9c)
#define image_5_tcam_cmd_gem_flow        (0xea2)
#define image_5_tcam_cmd_ipv6_label        (0xea8)
#define image_5_tcam_cmd_src_ip        (0xeb3)
#define image_5_tcam_cmd_src_ipv6_masked        (0xec8)
#define image_5_tcam_cmd_dst_ip        (0xed5)
#define image_5_tcam_cmd_dst_ipv6_masked        (0xeea)
#define image_5_tcam_cmd_src_port        (0xef7)
#define image_5_tcam_cmd_dst_port        (0xefd)
#define image_5_tcam_cmd_src_mac        (0xf03)
#define image_5_tcam_cmd_dst_mac        (0xf0e)
#define image_5_tcam_cmd_generic_l2        (0xf19)
#define image_5_tcam_cmd_generic_l3        (0xf26)
#define image_5_tcam_cmd_generic_l4        (0xf33)

#endif


#endif

/* IMAGE 6 LABELS */
#ifndef IMAGE_6_CODE_ADDRESSES
#define IMAGE_6_CODE_ADDRESSES

#ifndef PC_ADDRESS_INST_IND

#define image_6_start_task_initialization_task        (0x8)
#define image_6_initialization_task        (0x8)
#define image_6_start_task_processing_wakeup_request        (0x1010)
#define image_6_processing_wakeup_request        (0x1010)
#define image_6_start_task_debug_routine        (0x4588)
#define image_6_debug_routine        (0x4588)
#define image_6_start_task_dhd_tx_post_wakeup_request        (0x4b3c)
#define image_6_dhd_tx_post_wakeup_request        (0x4b3c)
#define image_6_start_task_dhd_tx_post_update_fifo_wakeup_request        (0x4ae4)
#define image_6_dhd_tx_post_update_fifo_wakeup_request        (0x4ae4)
#define image_6_start_task_dhd_timer_1st_wakeup_request        (0x4694)
#define image_6_dhd_timer_1st_wakeup_request        (0x4694)
#define image_6_debug_routine_handler        (0x4)
#define image_6_processing_header_update_length_calc        (0x19a0)
#define image_6_processing_layer2_header_copy_30_bytes_8_bytes_align        (0x1a88)
#define image_6_processing_layer2_header_copy_22_bytes_8_bytes_align        (0x1a90)
#define image_6_processing_layer2_header_copy_14_bytes_8_bytes_align        (0x1a98)
#define image_6_processing_layer2_header_copy_30_bytes_4_bytes_align        (0x1aac)
#define image_6_processing_layer2_header_copy_22_bytes_4_bytes_align        (0x1abc)
#define image_6_processing_layer2_header_copy_14_bytes_4_bytes_align        (0x1acc)
#define image_6_processing_layer2_header_copy_26_bytes_8_bytes_align        (0x1aec)
#define image_6_processing_layer2_header_copy_18_bytes_8_bytes_align        (0x1af8)
#define image_6_processing_layer2_header_copy_18_bytes_4_bytes_align        (0x1b1c)
#define image_6_processing_layer2_header_copy_26_bytes_4_bytes_align        (0x1b28)
#define image_6_tcam_cmd_ic_submit        (0x369c)
#define image_6_tcam_cmd_outer_tpid        (0x36e0)
#define image_6_tcam_cmd_inner_tpid        (0x36f8)
#define image_6_tcam_cmd_ethertype        (0x3710)
#define image_6_tcam_cmd_outer_vid        (0x3728)
#define image_6_tcam_cmd_inner_vid        (0x3740)
#define image_6_tcam_cmd_outer_pbit        (0x3758)
#define image_6_tcam_cmd_inner_pbit        (0x3770)
#define image_6_tcam_cmd_vlan_num        (0x3788)
#define image_6_tcam_cmd_ip_protocol        (0x37a0)
#define image_6_tcam_cmd_l3_protocol        (0x37b8)
#define image_6_tcam_cmd_tos        (0x37d0)
#define image_6_tcam_cmd_network_layer        (0x37e8)
#define image_6_tcam_cmd_ingress_port        (0x3808)
#define image_6_tcam_cmd_gem_flow        (0x3820)
#define image_6_tcam_cmd_ipv6_label        (0x3838)
#define image_6_tcam_cmd_src_ip        (0x3864)
#define image_6_tcam_cmd_src_ipv6_masked        (0x38b8)
#define image_6_tcam_cmd_dst_ip        (0x38ec)
#define image_6_tcam_cmd_dst_ipv6_masked        (0x3940)
#define image_6_tcam_cmd_src_port        (0x3974)
#define image_6_tcam_cmd_dst_port        (0x398c)
#define image_6_tcam_cmd_src_mac        (0x39a4)
#define image_6_tcam_cmd_dst_mac        (0x39d0)
#define image_6_tcam_cmd_generic_l2        (0x39fc)
#define image_6_tcam_cmd_generic_l3        (0x3a30)
#define image_6_tcam_cmd_generic_l4        (0x3a64)

#else

#define image_6_start_task_initialization_task        (0x2)
#define image_6_initialization_task        (0x2)
#define image_6_start_task_processing_wakeup_request        (0x404)
#define image_6_processing_wakeup_request        (0x404)
#define image_6_start_task_debug_routine        (0x1162)
#define image_6_debug_routine        (0x1162)
#define image_6_start_task_dhd_tx_post_wakeup_request        (0x12cf)
#define image_6_dhd_tx_post_wakeup_request        (0x12cf)
#define image_6_start_task_dhd_tx_post_update_fifo_wakeup_request        (0x12b9)
#define image_6_dhd_tx_post_update_fifo_wakeup_request        (0x12b9)
#define image_6_start_task_dhd_timer_1st_wakeup_request        (0x11a5)
#define image_6_dhd_timer_1st_wakeup_request        (0x11a5)
#define image_6_debug_routine_handler        (0x1)
#define image_6_processing_header_update_length_calc        (0x668)
#define image_6_processing_layer2_header_copy_30_bytes_8_bytes_align        (0x6a2)
#define image_6_processing_layer2_header_copy_22_bytes_8_bytes_align        (0x6a4)
#define image_6_processing_layer2_header_copy_14_bytes_8_bytes_align        (0x6a6)
#define image_6_processing_layer2_header_copy_30_bytes_4_bytes_align        (0x6ab)
#define image_6_processing_layer2_header_copy_22_bytes_4_bytes_align        (0x6af)
#define image_6_processing_layer2_header_copy_14_bytes_4_bytes_align        (0x6b3)
#define image_6_processing_layer2_header_copy_26_bytes_8_bytes_align        (0x6bb)
#define image_6_processing_layer2_header_copy_18_bytes_8_bytes_align        (0x6be)
#define image_6_processing_layer2_header_copy_18_bytes_4_bytes_align        (0x6c7)
#define image_6_processing_layer2_header_copy_26_bytes_4_bytes_align        (0x6ca)
#define image_6_tcam_cmd_ic_submit        (0xda7)
#define image_6_tcam_cmd_outer_tpid        (0xdb8)
#define image_6_tcam_cmd_inner_tpid        (0xdbe)
#define image_6_tcam_cmd_ethertype        (0xdc4)
#define image_6_tcam_cmd_outer_vid        (0xdca)
#define image_6_tcam_cmd_inner_vid        (0xdd0)
#define image_6_tcam_cmd_outer_pbit        (0xdd6)
#define image_6_tcam_cmd_inner_pbit        (0xddc)
#define image_6_tcam_cmd_vlan_num        (0xde2)
#define image_6_tcam_cmd_ip_protocol        (0xde8)
#define image_6_tcam_cmd_l3_protocol        (0xdee)
#define image_6_tcam_cmd_tos        (0xdf4)
#define image_6_tcam_cmd_network_layer        (0xdfa)
#define image_6_tcam_cmd_ingress_port        (0xe02)
#define image_6_tcam_cmd_gem_flow        (0xe08)
#define image_6_tcam_cmd_ipv6_label        (0xe0e)
#define image_6_tcam_cmd_src_ip        (0xe19)
#define image_6_tcam_cmd_src_ipv6_masked        (0xe2e)
#define image_6_tcam_cmd_dst_ip        (0xe3b)
#define image_6_tcam_cmd_dst_ipv6_masked        (0xe50)
#define image_6_tcam_cmd_src_port        (0xe5d)
#define image_6_tcam_cmd_dst_port        (0xe63)
#define image_6_tcam_cmd_src_mac        (0xe69)
#define image_6_tcam_cmd_dst_mac        (0xe74)
#define image_6_tcam_cmd_generic_l2        (0xe7f)
#define image_6_tcam_cmd_generic_l3        (0xe8c)
#define image_6_tcam_cmd_generic_l4        (0xe99)

#endif


#endif

/* IMAGE 7 LABELS */
#ifndef IMAGE_7_CODE_ADDRESSES
#define IMAGE_7_CODE_ADDRESSES

#ifndef PC_ADDRESS_INST_IND

#define image_7_start_task_initialization_task        (0x8)
#define image_7_initialization_task        (0x8)
#define image_7_start_task_processing_wakeup_request        (0x1010)
#define image_7_processing_wakeup_request        (0x1010)
#define image_7_start_task_debug_routine        (0x4588)
#define image_7_debug_routine        (0x4588)
#define image_7_debug_routine_handler        (0x4)
#define image_7_processing_header_update_length_calc        (0x19a0)
#define image_7_processing_layer2_header_copy_30_bytes_8_bytes_align        (0x1a88)
#define image_7_processing_layer2_header_copy_22_bytes_8_bytes_align        (0x1a90)
#define image_7_processing_layer2_header_copy_14_bytes_8_bytes_align        (0x1a98)
#define image_7_processing_layer2_header_copy_30_bytes_4_bytes_align        (0x1aac)
#define image_7_processing_layer2_header_copy_22_bytes_4_bytes_align        (0x1abc)
#define image_7_processing_layer2_header_copy_14_bytes_4_bytes_align        (0x1acc)
#define image_7_processing_layer2_header_copy_26_bytes_8_bytes_align        (0x1aec)
#define image_7_processing_layer2_header_copy_18_bytes_8_bytes_align        (0x1af8)
#define image_7_processing_layer2_header_copy_18_bytes_4_bytes_align        (0x1b1c)
#define image_7_processing_layer2_header_copy_26_bytes_4_bytes_align        (0x1b28)
#define image_7_tcam_cmd_ic_submit        (0x369c)
#define image_7_tcam_cmd_outer_tpid        (0x36e0)
#define image_7_tcam_cmd_inner_tpid        (0x36f8)
#define image_7_tcam_cmd_ethertype        (0x3710)
#define image_7_tcam_cmd_outer_vid        (0x3728)
#define image_7_tcam_cmd_inner_vid        (0x3740)
#define image_7_tcam_cmd_outer_pbit        (0x3758)
#define image_7_tcam_cmd_inner_pbit        (0x3770)
#define image_7_tcam_cmd_vlan_num        (0x3788)
#define image_7_tcam_cmd_ip_protocol        (0x37a0)
#define image_7_tcam_cmd_l3_protocol        (0x37b8)
#define image_7_tcam_cmd_tos        (0x37d0)
#define image_7_tcam_cmd_network_layer        (0x37e8)
#define image_7_tcam_cmd_ingress_port        (0x3808)
#define image_7_tcam_cmd_gem_flow        (0x3820)
#define image_7_tcam_cmd_ipv6_label        (0x3838)
#define image_7_tcam_cmd_src_ip        (0x3864)
#define image_7_tcam_cmd_src_ipv6_masked        (0x38b8)
#define image_7_tcam_cmd_dst_ip        (0x38ec)
#define image_7_tcam_cmd_dst_ipv6_masked        (0x3940)
#define image_7_tcam_cmd_src_port        (0x3974)
#define image_7_tcam_cmd_dst_port        (0x398c)
#define image_7_tcam_cmd_src_mac        (0x39a4)
#define image_7_tcam_cmd_dst_mac        (0x39d0)
#define image_7_tcam_cmd_generic_l2        (0x39fc)
#define image_7_tcam_cmd_generic_l3        (0x3a30)
#define image_7_tcam_cmd_generic_l4        (0x3a64)

#else

#define image_7_start_task_initialization_task        (0x2)
#define image_7_initialization_task        (0x2)
#define image_7_start_task_processing_wakeup_request        (0x404)
#define image_7_processing_wakeup_request        (0x404)
#define image_7_start_task_debug_routine        (0x1162)
#define image_7_debug_routine        (0x1162)
#define image_7_debug_routine_handler        (0x1)
#define image_7_processing_header_update_length_calc        (0x668)
#define image_7_processing_layer2_header_copy_30_bytes_8_bytes_align        (0x6a2)
#define image_7_processing_layer2_header_copy_22_bytes_8_bytes_align        (0x6a4)
#define image_7_processing_layer2_header_copy_14_bytes_8_bytes_align        (0x6a6)
#define image_7_processing_layer2_header_copy_30_bytes_4_bytes_align        (0x6ab)
#define image_7_processing_layer2_header_copy_22_bytes_4_bytes_align        (0x6af)
#define image_7_processing_layer2_header_copy_14_bytes_4_bytes_align        (0x6b3)
#define image_7_processing_layer2_header_copy_26_bytes_8_bytes_align        (0x6bb)
#define image_7_processing_layer2_header_copy_18_bytes_8_bytes_align        (0x6be)
#define image_7_processing_layer2_header_copy_18_bytes_4_bytes_align        (0x6c7)
#define image_7_processing_layer2_header_copy_26_bytes_4_bytes_align        (0x6ca)
#define image_7_tcam_cmd_ic_submit        (0xda7)
#define image_7_tcam_cmd_outer_tpid        (0xdb8)
#define image_7_tcam_cmd_inner_tpid        (0xdbe)
#define image_7_tcam_cmd_ethertype        (0xdc4)
#define image_7_tcam_cmd_outer_vid        (0xdca)
#define image_7_tcam_cmd_inner_vid        (0xdd0)
#define image_7_tcam_cmd_outer_pbit        (0xdd6)
#define image_7_tcam_cmd_inner_pbit        (0xddc)
#define image_7_tcam_cmd_vlan_num        (0xde2)
#define image_7_tcam_cmd_ip_protocol        (0xde8)
#define image_7_tcam_cmd_l3_protocol        (0xdee)
#define image_7_tcam_cmd_tos        (0xdf4)
#define image_7_tcam_cmd_network_layer        (0xdfa)
#define image_7_tcam_cmd_ingress_port        (0xe02)
#define image_7_tcam_cmd_gem_flow        (0xe08)
#define image_7_tcam_cmd_ipv6_label        (0xe0e)
#define image_7_tcam_cmd_src_ip        (0xe19)
#define image_7_tcam_cmd_src_ipv6_masked        (0xe2e)
#define image_7_tcam_cmd_dst_ip        (0xe3b)
#define image_7_tcam_cmd_dst_ipv6_masked        (0xe50)
#define image_7_tcam_cmd_src_port        (0xe5d)
#define image_7_tcam_cmd_dst_port        (0xe63)
#define image_7_tcam_cmd_src_mac        (0xe69)
#define image_7_tcam_cmd_dst_mac        (0xe74)
#define image_7_tcam_cmd_generic_l2        (0xe7f)
#define image_7_tcam_cmd_generic_l3        (0xe8c)
#define image_7_tcam_cmd_generic_l4        (0xe99)

#endif


#endif

/* COMMON LABELS */
#ifndef COMMON_CODE_ADDRESSES
#define COMMON_CODE_ADDRESSES

#define INVALID_LABEL_ADDRESS 0xFFFFFF

#ifndef PC_ADDRESS_INST_IND

#define TCAM_CMD_DST_IP_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x38ec, 0x39f8, INVALID_LABEL_ADDRESS, 0x39f8, 0x3b54, 0x38ec, 0x38ec}
#define TCAM_CMD_DST_IPV6_MASKED_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3940, 0x3a4c, INVALID_LABEL_ADDRESS, 0x3a4c, 0x3ba8, 0x3940, 0x3940}
#define TCAM_CMD_DST_MAC_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x39d0, 0x3adc, INVALID_LABEL_ADDRESS, 0x3adc, 0x3c38, 0x39d0, 0x39d0}
#define TCAM_CMD_DST_PORT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x398c, 0x3a98, INVALID_LABEL_ADDRESS, 0x3a98, 0x3bf4, 0x398c, 0x398c}
#define TCAM_CMD_ETHERTYPE_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3710, 0x381c, INVALID_LABEL_ADDRESS, 0x381c, 0x3978, 0x3710, 0x3710}
#define TCAM_CMD_GEM_FLOW_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3820, 0x392c, INVALID_LABEL_ADDRESS, 0x392c, 0x3a88, 0x3820, 0x3820}
#define TCAM_CMD_GENERIC_L2_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x39fc, 0x3b08, INVALID_LABEL_ADDRESS, 0x3b08, 0x3c64, 0x39fc, 0x39fc}
#define TCAM_CMD_GENERIC_L3_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3a30, 0x3b3c, INVALID_LABEL_ADDRESS, 0x3b3c, 0x3c98, 0x3a30, 0x3a30}
#define TCAM_CMD_GENERIC_L4_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3a64, 0x3b70, INVALID_LABEL_ADDRESS, 0x3b70, 0x3ccc, 0x3a64, 0x3a64}
#define TCAM_CMD_IC_SUBMIT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x369c, 0x37a8, INVALID_LABEL_ADDRESS, 0x37a8, 0x3904, 0x369c, 0x369c}
#define TCAM_CMD_INGRESS_PORT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3808, 0x3914, INVALID_LABEL_ADDRESS, 0x3914, 0x3a70, 0x3808, 0x3808}
#define TCAM_CMD_INNER_PBIT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3770, 0x387c, INVALID_LABEL_ADDRESS, 0x387c, 0x39d8, 0x3770, 0x3770}
#define TCAM_CMD_INNER_TPID_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x36f8, 0x3804, INVALID_LABEL_ADDRESS, 0x3804, 0x3960, 0x36f8, 0x36f8}
#define TCAM_CMD_INNER_VID_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3740, 0x384c, INVALID_LABEL_ADDRESS, 0x384c, 0x39a8, 0x3740, 0x3740}
#define TCAM_CMD_IP_PROTOCOL_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x37a0, 0x38ac, INVALID_LABEL_ADDRESS, 0x38ac, 0x3a08, 0x37a0, 0x37a0}
#define TCAM_CMD_IPV6_LABEL_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3838, 0x3944, INVALID_LABEL_ADDRESS, 0x3944, 0x3aa0, 0x3838, 0x3838}
#define TCAM_CMD_L3_PROTOCOL_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x37b8, 0x38c4, INVALID_LABEL_ADDRESS, 0x38c4, 0x3a20, 0x37b8, 0x37b8}
#define TCAM_CMD_NETWORK_LAYER_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x37e8, 0x38f4, INVALID_LABEL_ADDRESS, 0x38f4, 0x3a50, 0x37e8, 0x37e8}
#define TCAM_CMD_OUTER_PBIT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3758, 0x3864, INVALID_LABEL_ADDRESS, 0x3864, 0x39c0, 0x3758, 0x3758}
#define TCAM_CMD_OUTER_TPID_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x36e0, 0x37ec, INVALID_LABEL_ADDRESS, 0x37ec, 0x3948, 0x36e0, 0x36e0}
#define TCAM_CMD_OUTER_VID_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3728, 0x3834, INVALID_LABEL_ADDRESS, 0x3834, 0x3990, 0x3728, 0x3728}
#define TCAM_CMD_SRC_IP_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3864, 0x3970, INVALID_LABEL_ADDRESS, 0x3970, 0x3acc, 0x3864, 0x3864}
#define TCAM_CMD_SRC_IPV6_MASKED_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x38b8, 0x39c4, INVALID_LABEL_ADDRESS, 0x39c4, 0x3b20, 0x38b8, 0x38b8}
#define TCAM_CMD_SRC_MAC_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x39a4, 0x3ab0, INVALID_LABEL_ADDRESS, 0x3ab0, 0x3c0c, 0x39a4, 0x39a4}
#define TCAM_CMD_SRC_PORT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3974, 0x3a80, INVALID_LABEL_ADDRESS, 0x3a80, 0x3bdc, 0x3974, 0x3974}
#define TCAM_CMD_TOS_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x37d0, 0x38dc, INVALID_LABEL_ADDRESS, 0x38dc, 0x3a38, 0x37d0, 0x37d0}
#define TCAM_CMD_VLAN_NUM_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3788, 0x3894, INVALID_LABEL_ADDRESS, 0x3894, 0x39f0, 0x3788, 0x3788}

#else

#define TCAM_CMD_DST_IP_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xe3b, 0xe7e, INVALID_LABEL_ADDRESS, 0xe7e, 0xed5, 0xe3b, 0xe3b}
#define TCAM_CMD_DST_IPV6_MASKED_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xe50, 0xe93, INVALID_LABEL_ADDRESS, 0xe93, 0xeea, 0xe50, 0xe50}
#define TCAM_CMD_DST_MAC_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xe74, 0xeb7, INVALID_LABEL_ADDRESS, 0xeb7, 0xf0e, 0xe74, 0xe74}
#define TCAM_CMD_DST_PORT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xe63, 0xea6, INVALID_LABEL_ADDRESS, 0xea6, 0xefd, 0xe63, 0xe63}
#define TCAM_CMD_ETHERTYPE_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xdc4, 0xe07, INVALID_LABEL_ADDRESS, 0xe07, 0xe5e, 0xdc4, 0xdc4}
#define TCAM_CMD_GEM_FLOW_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xe08, 0xe4b, INVALID_LABEL_ADDRESS, 0xe4b, 0xea2, 0xe08, 0xe08}
#define TCAM_CMD_GENERIC_L2_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xe7f, 0xec2, INVALID_LABEL_ADDRESS, 0xec2, 0xf19, 0xe7f, 0xe7f}
#define TCAM_CMD_GENERIC_L3_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xe8c, 0xecf, INVALID_LABEL_ADDRESS, 0xecf, 0xf26, 0xe8c, 0xe8c}
#define TCAM_CMD_GENERIC_L4_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xe99, 0xedc, INVALID_LABEL_ADDRESS, 0xedc, 0xf33, 0xe99, 0xe99}
#define TCAM_CMD_IC_SUBMIT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xda7, 0xdea, INVALID_LABEL_ADDRESS, 0xdea, 0xe41, 0xda7, 0xda7}
#define TCAM_CMD_INGRESS_PORT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xe02, 0xe45, INVALID_LABEL_ADDRESS, 0xe45, 0xe9c, 0xe02, 0xe02}
#define TCAM_CMD_INNER_PBIT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xddc, 0xe1f, INVALID_LABEL_ADDRESS, 0xe1f, 0xe76, 0xddc, 0xddc}
#define TCAM_CMD_INNER_TPID_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xdbe, 0xe01, INVALID_LABEL_ADDRESS, 0xe01, 0xe58, 0xdbe, 0xdbe}
#define TCAM_CMD_INNER_VID_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xdd0, 0xe13, INVALID_LABEL_ADDRESS, 0xe13, 0xe6a, 0xdd0, 0xdd0}
#define TCAM_CMD_IP_PROTOCOL_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xde8, 0xe2b, INVALID_LABEL_ADDRESS, 0xe2b, 0xe82, 0xde8, 0xde8}
#define TCAM_CMD_IPV6_LABEL_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xe0e, 0xe51, INVALID_LABEL_ADDRESS, 0xe51, 0xea8, 0xe0e, 0xe0e}
#define TCAM_CMD_L3_PROTOCOL_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xdee, 0xe31, INVALID_LABEL_ADDRESS, 0xe31, 0xe88, 0xdee, 0xdee}
#define TCAM_CMD_NETWORK_LAYER_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xdfa, 0xe3d, INVALID_LABEL_ADDRESS, 0xe3d, 0xe94, 0xdfa, 0xdfa}
#define TCAM_CMD_OUTER_PBIT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xdd6, 0xe19, INVALID_LABEL_ADDRESS, 0xe19, 0xe70, 0xdd6, 0xdd6}
#define TCAM_CMD_OUTER_TPID_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xdb8, 0xdfb, INVALID_LABEL_ADDRESS, 0xdfb, 0xe52, 0xdb8, 0xdb8}
#define TCAM_CMD_OUTER_VID_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xdca, 0xe0d, INVALID_LABEL_ADDRESS, 0xe0d, 0xe64, 0xdca, 0xdca}
#define TCAM_CMD_SRC_IP_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xe19, 0xe5c, INVALID_LABEL_ADDRESS, 0xe5c, 0xeb3, 0xe19, 0xe19}
#define TCAM_CMD_SRC_IPV6_MASKED_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xe2e, 0xe71, INVALID_LABEL_ADDRESS, 0xe71, 0xec8, 0xe2e, 0xe2e}
#define TCAM_CMD_SRC_MAC_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xe69, 0xeac, INVALID_LABEL_ADDRESS, 0xeac, 0xf03, 0xe69, 0xe69}
#define TCAM_CMD_SRC_PORT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xe5d, 0xea0, INVALID_LABEL_ADDRESS, 0xea0, 0xef7, 0xe5d, 0xe5d}
#define TCAM_CMD_TOS_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xdf4, 0xe37, INVALID_LABEL_ADDRESS, 0xe37, 0xe8e, 0xdf4, 0xdf4}
#define TCAM_CMD_VLAN_NUM_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xde2, 0xe25, INVALID_LABEL_ADDRESS, 0xe25, 0xe7c, 0xde2, 0xde2}

#endif


#endif

