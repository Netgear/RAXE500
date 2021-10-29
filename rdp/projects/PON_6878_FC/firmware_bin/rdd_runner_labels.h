/* IMAGE 0 LABELS */
#ifndef IMAGE_0_CODE_ADDRESSES
#define IMAGE_0_CODE_ADDRESSES

#ifndef PC_ADDRESS_INST_IND

#define image_0_start_task_initialization_task        (0x50)
#define image_0_initialization_task        (0x50)
#define image_0_start_task_update_fifo_us_read_1st_wakeup_request        (0x1008)
#define image_0_update_fifo_us_read_1st_wakeup_request        (0x1008)
#define image_0_start_task_ghost_reporting_1st_wakeup_request        (0x26e0)
#define image_0_ghost_reporting_1st_wakeup_request        (0x26e0)
#define image_0_start_task_us_tx_task_1st_wakeup_request        (0xa70)
#define image_0_us_tx_task_1st_wakeup_request        (0xa70)
#define image_0_us_tx_task_wakeup_request        (0xa70)
#define image_0_start_task_epon_update_fifo_read_1st_wakeup_request        (0x12f0)
#define image_0_epon_update_fifo_read_1st_wakeup_request        (0x12f0)
#define image_0_start_task_epon_tx_task_wakeup_request        (0x2ae8)
#define image_0_epon_tx_task_wakeup_request        (0x2ae8)
#define image_0_start_task_budget_allocator_us_1st_wakeup_request        (0x1340)
#define image_0_budget_allocator_us_1st_wakeup_request        (0x1340)
#define image_0_start_task_debug_routine        (0x78)
#define image_0_debug_routine        (0x78)
#define image_0_start_task_gpon_control_wakeup_request        (0x650)
#define image_0_gpon_control_wakeup_request        (0x650)
#define image_0_start_task_ds_tx_task_wakeup_request        (0x1d08)
#define image_0_ds_tx_task_wakeup_request        (0x1d08)
#define image_0_start_task_ovl_budget_allocator_1st_wakeup_request        (0x23a8)
#define image_0_ovl_budget_allocator_1st_wakeup_request        (0x23a8)
#define image_0_start_task_flush_task_1st_wakeup_request        (0x2460)
#define image_0_flush_task_1st_wakeup_request        (0x2460)
#define image_0_start_task_update_fifo_ds_read_1st_wakeup_request        (0x1a08)
#define image_0_update_fifo_ds_read_1st_wakeup_request        (0x1a08)
#define image_0_start_task_budget_allocator_ds_1st_wakeup_request        (0x1f50)
#define image_0_budget_allocator_ds_1st_wakeup_request        (0x1f50)
#define image_0_start_task_service_queues_update_fifo_read_1st_wakeup_request        (0x2be0)
#define image_0_service_queues_update_fifo_read_1st_wakeup_request        (0x2be0)
#define image_0_start_task_service_queues_tx_task_wakeup_request        (0x2ee8)
#define image_0_service_queues_tx_task_wakeup_request        (0x2ee8)
#define image_0_start_task_budget_allocator_1st_wakeup_request        (0x3160)
#define image_0_budget_allocator_1st_wakeup_request        (0x3160)
#define image_0_debug_routine_handler        (0x4)

#else

#define image_0_start_task_initialization_task        (0x14)
#define image_0_initialization_task        (0x14)
#define image_0_start_task_update_fifo_us_read_1st_wakeup_request        (0x402)
#define image_0_update_fifo_us_read_1st_wakeup_request        (0x402)
#define image_0_start_task_ghost_reporting_1st_wakeup_request        (0x9b8)
#define image_0_ghost_reporting_1st_wakeup_request        (0x9b8)
#define image_0_start_task_us_tx_task_1st_wakeup_request        (0x29c)
#define image_0_us_tx_task_1st_wakeup_request        (0x29c)
#define image_0_us_tx_task_wakeup_request        (0x29c)
#define image_0_start_task_epon_update_fifo_read_1st_wakeup_request        (0x4bc)
#define image_0_epon_update_fifo_read_1st_wakeup_request        (0x4bc)
#define image_0_start_task_epon_tx_task_wakeup_request        (0xaba)
#define image_0_epon_tx_task_wakeup_request        (0xaba)
#define image_0_start_task_budget_allocator_us_1st_wakeup_request        (0x4d0)
#define image_0_budget_allocator_us_1st_wakeup_request        (0x4d0)
#define image_0_start_task_debug_routine        (0x1e)
#define image_0_debug_routine        (0x1e)
#define image_0_start_task_gpon_control_wakeup_request        (0x194)
#define image_0_gpon_control_wakeup_request        (0x194)
#define image_0_start_task_ds_tx_task_wakeup_request        (0x742)
#define image_0_ds_tx_task_wakeup_request        (0x742)
#define image_0_start_task_ovl_budget_allocator_1st_wakeup_request        (0x8ea)
#define image_0_ovl_budget_allocator_1st_wakeup_request        (0x8ea)
#define image_0_start_task_flush_task_1st_wakeup_request        (0x918)
#define image_0_flush_task_1st_wakeup_request        (0x918)
#define image_0_start_task_update_fifo_ds_read_1st_wakeup_request        (0x682)
#define image_0_update_fifo_ds_read_1st_wakeup_request        (0x682)
#define image_0_start_task_budget_allocator_ds_1st_wakeup_request        (0x7d4)
#define image_0_budget_allocator_ds_1st_wakeup_request        (0x7d4)
#define image_0_start_task_service_queues_update_fifo_read_1st_wakeup_request        (0xaf8)
#define image_0_service_queues_update_fifo_read_1st_wakeup_request        (0xaf8)
#define image_0_start_task_service_queues_tx_task_wakeup_request        (0xbba)
#define image_0_service_queues_tx_task_wakeup_request        (0xbba)
#define image_0_start_task_budget_allocator_1st_wakeup_request        (0xc58)
#define image_0_budget_allocator_1st_wakeup_request        (0xc58)
#define image_0_debug_routine_handler        (0x1)

#endif


#endif

/* IMAGE 1 LABELS */
#ifndef IMAGE_1_CODE_ADDRESSES
#define IMAGE_1_CODE_ADDRESSES

#ifndef PC_ADDRESS_INST_IND

#define image_1_start_task_initialization_task        (0x8)
#define image_1_initialization_task        (0x8)
#define image_1_start_task_processing_wakeup_request        (0xf50)
#define image_1_processing_wakeup_request        (0xf50)
#define image_1_start_task_debug_routine        (0x42c8)
#define image_1_debug_routine        (0x42c8)
#define image_1_start_task_cpu_rx_wakeup_request        (0x5218)
#define image_1_cpu_rx_wakeup_request        (0x5218)
#define image_1_start_task_cpu_recycle_wakeup_request        (0x5220)
#define image_1_cpu_recycle_wakeup_request        (0x5220)
#define image_1_start_task_interrupt_coalescing_1st_wakeup_request        (0x5420)
#define image_1_interrupt_coalescing_1st_wakeup_request        (0x5420)
#define image_1_start_task_cpu_rx_meter_budget_allocator_1st_wakeup_request        (0x5370)
#define image_1_cpu_rx_meter_budget_allocator_1st_wakeup_request        (0x5370)
#define image_1_start_task_cpu_rx_copy_wakeup_request        (0x4468)
#define image_1_cpu_rx_copy_wakeup_request        (0x4468)
#define image_1_start_task_spdsvc_gen_wakeup_request        (0x54d0)
#define image_1_spdsvc_gen_wakeup_request        (0x54d0)
#define image_1_start_task_common_reprocessing_wakeup_request        (0x6600)
#define image_1_common_reprocessing_wakeup_request        (0x6600)
#define image_1_debug_routine_handler        (0x4)
#define image_1_tcam_cmd_ic_submit        (0x33e8)
#define image_1_tcam_cmd_outer_tpid        (0x3430)
#define image_1_tcam_cmd_inner_tpid        (0x3448)
#define image_1_tcam_cmd_ethertype        (0x3460)
#define image_1_tcam_cmd_outer_vid        (0x3478)
#define image_1_tcam_cmd_inner_vid        (0x3490)
#define image_1_tcam_cmd_outer_pbit        (0x34a8)
#define image_1_tcam_cmd_inner_pbit        (0x34c0)
#define image_1_tcam_cmd_vlan_num        (0x34d8)
#define image_1_tcam_cmd_ip_protocol        (0x34f0)
#define image_1_tcam_cmd_l3_protocol        (0x3508)
#define image_1_tcam_cmd_tos        (0x3520)
#define image_1_tcam_cmd_network_layer        (0x3538)
#define image_1_tcam_cmd_ingress_port        (0x3558)
#define image_1_tcam_cmd_gem_flow        (0x3570)
#define image_1_tcam_cmd_ipv6_label        (0x3588)
#define image_1_tcam_cmd_src_ip        (0x35b8)
#define image_1_tcam_cmd_src_ipv6_masked        (0x3608)
#define image_1_tcam_cmd_dst_ip        (0x3640)
#define image_1_tcam_cmd_dst_ipv6_masked        (0x3690)
#define image_1_tcam_cmd_src_port        (0x36c8)
#define image_1_tcam_cmd_dst_port        (0x36e0)
#define image_1_tcam_cmd_src_mac        (0x36f8)
#define image_1_tcam_cmd_dst_mac        (0x3728)
#define image_1_tcam_cmd_generic_l2        (0x3758)
#define image_1_tcam_cmd_generic_l3        (0x3790)
#define image_1_tcam_cmd_generic_l4        (0x37c8)
#define image_1_start_task_c_cpu_rx_wakeup_request        (0x6784)
#define image_1_c_cpu_rx_wakeup_request        (0x6784)

#else

#define image_1_start_task_initialization_task        (0x2)
#define image_1_initialization_task        (0x2)
#define image_1_start_task_processing_wakeup_request        (0x3d4)
#define image_1_processing_wakeup_request        (0x3d4)
#define image_1_start_task_debug_routine        (0x10b2)
#define image_1_debug_routine        (0x10b2)
#define image_1_start_task_cpu_rx_wakeup_request        (0x1486)
#define image_1_cpu_rx_wakeup_request        (0x1486)
#define image_1_start_task_cpu_recycle_wakeup_request        (0x1488)
#define image_1_cpu_recycle_wakeup_request        (0x1488)
#define image_1_start_task_interrupt_coalescing_1st_wakeup_request        (0x1508)
#define image_1_interrupt_coalescing_1st_wakeup_request        (0x1508)
#define image_1_start_task_cpu_rx_meter_budget_allocator_1st_wakeup_request        (0x14dc)
#define image_1_cpu_rx_meter_budget_allocator_1st_wakeup_request        (0x14dc)
#define image_1_start_task_cpu_rx_copy_wakeup_request        (0x111a)
#define image_1_cpu_rx_copy_wakeup_request        (0x111a)
#define image_1_start_task_spdsvc_gen_wakeup_request        (0x1534)
#define image_1_spdsvc_gen_wakeup_request        (0x1534)
#define image_1_start_task_common_reprocessing_wakeup_request        (0x1980)
#define image_1_common_reprocessing_wakeup_request        (0x1980)
#define image_1_debug_routine_handler        (0x1)
#define image_1_tcam_cmd_ic_submit        (0xcfa)
#define image_1_tcam_cmd_outer_tpid        (0xd0c)
#define image_1_tcam_cmd_inner_tpid        (0xd12)
#define image_1_tcam_cmd_ethertype        (0xd18)
#define image_1_tcam_cmd_outer_vid        (0xd1e)
#define image_1_tcam_cmd_inner_vid        (0xd24)
#define image_1_tcam_cmd_outer_pbit        (0xd2a)
#define image_1_tcam_cmd_inner_pbit        (0xd30)
#define image_1_tcam_cmd_vlan_num        (0xd36)
#define image_1_tcam_cmd_ip_protocol        (0xd3c)
#define image_1_tcam_cmd_l3_protocol        (0xd42)
#define image_1_tcam_cmd_tos        (0xd48)
#define image_1_tcam_cmd_network_layer        (0xd4e)
#define image_1_tcam_cmd_ingress_port        (0xd56)
#define image_1_tcam_cmd_gem_flow        (0xd5c)
#define image_1_tcam_cmd_ipv6_label        (0xd62)
#define image_1_tcam_cmd_src_ip        (0xd6e)
#define image_1_tcam_cmd_src_ipv6_masked        (0xd82)
#define image_1_tcam_cmd_dst_ip        (0xd90)
#define image_1_tcam_cmd_dst_ipv6_masked        (0xda4)
#define image_1_tcam_cmd_src_port        (0xdb2)
#define image_1_tcam_cmd_dst_port        (0xdb8)
#define image_1_tcam_cmd_src_mac        (0xdbe)
#define image_1_tcam_cmd_dst_mac        (0xdca)
#define image_1_tcam_cmd_generic_l2        (0xdd6)
#define image_1_tcam_cmd_generic_l3        (0xde4)
#define image_1_tcam_cmd_generic_l4        (0xdf2)
#define image_1_start_task_c_cpu_rx_wakeup_request        (0x19e1)
#define image_1_c_cpu_rx_wakeup_request        (0x19e1)

#endif


#endif

/* IMAGE 2 LABELS */
#ifndef IMAGE_2_CODE_ADDRESSES
#define IMAGE_2_CODE_ADDRESSES

#ifndef PC_ADDRESS_INST_IND

#define image_2_start_task_initialization_task        (0x8)
#define image_2_initialization_task        (0x8)
#define image_2_start_task_processing_wakeup_request        (0x10f0)
#define image_2_processing_wakeup_request        (0x10f0)
#define image_2_start_task_cpu_tx_wakeup_request        (0x4468)
#define image_2_cpu_tx_wakeup_request        (0x4468)
#define image_2_cpu_tx_read_ring_indices        (0x4468)
#define image_2_start_task_debug_routine        (0x30)
#define image_2_debug_routine        (0x30)
#define image_2_start_task_cpu_recycle_wakeup_request        (0x4a40)
#define image_2_cpu_recycle_wakeup_request        (0x4a40)
#define image_2_start_task_interrupt_coalescing_1st_wakeup_request        (0x4b90)
#define image_2_interrupt_coalescing_1st_wakeup_request        (0x4b90)
#define image_2_start_task_dhd_tx_post_wakeup_request        (0x4c08)
#define image_2_dhd_tx_post_wakeup_request        (0x4c08)
#define image_2_start_task_dhd_tx_post_update_fifo_wakeup_request        (0x4c00)
#define image_2_dhd_tx_post_update_fifo_wakeup_request        (0x4c00)
#define image_2_start_task_dhd_tx_complete_wakeup_request        (0x4e58)
#define image_2_dhd_tx_complete_wakeup_request        (0x4e58)
#define image_2_start_task_dhd_rx_complete_wakeup_request        (0x4e60)
#define image_2_dhd_rx_complete_wakeup_request        (0x4e60)
#define image_2_start_task_dhd_timer_1st_wakeup_request        (0x4bf8)
#define image_2_dhd_timer_1st_wakeup_request        (0x4bf8)
#define image_2_start_task_timer_common_task_wakeup_request        (0x4cf8)
#define image_2_timer_common_task_wakeup_request        (0x4cf8)
#define image_2_debug_routine_handler        (0x4)
#define image_2_tcam_cmd_ic_submit        (0x3588)
#define image_2_tcam_cmd_outer_tpid        (0x35d0)
#define image_2_tcam_cmd_inner_tpid        (0x35e8)
#define image_2_tcam_cmd_ethertype        (0x3600)
#define image_2_tcam_cmd_outer_vid        (0x3618)
#define image_2_tcam_cmd_inner_vid        (0x3630)
#define image_2_tcam_cmd_outer_pbit        (0x3648)
#define image_2_tcam_cmd_inner_pbit        (0x3660)
#define image_2_tcam_cmd_vlan_num        (0x3678)
#define image_2_tcam_cmd_ip_protocol        (0x3690)
#define image_2_tcam_cmd_l3_protocol        (0x36a8)
#define image_2_tcam_cmd_tos        (0x36c0)
#define image_2_tcam_cmd_network_layer        (0x36d8)
#define image_2_tcam_cmd_ingress_port        (0x36f8)
#define image_2_tcam_cmd_gem_flow        (0x3710)
#define image_2_tcam_cmd_ipv6_label        (0x3728)
#define image_2_tcam_cmd_src_ip        (0x3758)
#define image_2_tcam_cmd_src_ipv6_masked        (0x37a8)
#define image_2_tcam_cmd_dst_ip        (0x37e0)
#define image_2_tcam_cmd_dst_ipv6_masked        (0x3830)
#define image_2_tcam_cmd_src_port        (0x3868)
#define image_2_tcam_cmd_dst_port        (0x3880)
#define image_2_tcam_cmd_src_mac        (0x3898)
#define image_2_tcam_cmd_dst_mac        (0x38c8)
#define image_2_tcam_cmd_generic_l2        (0x38f8)
#define image_2_tcam_cmd_generic_l3        (0x3930)
#define image_2_tcam_cmd_generic_l4        (0x3968)
#define image_2_start_task_c_cpu_tx_wakeup_request        (0x6744)
#define image_2_c_cpu_tx_wakeup_request        (0x6744)
#define image_2_start_task_c_dhd_rx_complete_wakeup_request        (0x531c)
#define image_2_c_dhd_rx_complete_wakeup_request        (0x531c)
#define image_2_start_task_c_dhd_timer_wakeup_request        (0x579c)
#define image_2_c_dhd_timer_wakeup_request        (0x579c)
#define image_2_start_task_c_dhd_tx_complete_wakeup_request        (0x55b8)
#define image_2_c_dhd_tx_complete_wakeup_request        (0x55b8)
#define image_2_start_task_c_dhd_tx_post_update_fifo_wakeup_request        (0x58c4)
#define image_2_c_dhd_tx_post_update_fifo_wakeup_request        (0x58c4)
#define image_2_start_task_c_dhd_tx_post_wakeup_request        (0x590c)
#define image_2_c_dhd_tx_post_wakeup_request        (0x590c)
#define image_2_start_task_cpu_tx_send_pd_to_recycle        (0x6b10)
#define image_2_cpu_tx_send_pd_to_recycle        (0x6b10)
#define image_2_start_task_dhd_send_entry_to_cpu_ring        (0x5238)
#define image_2_dhd_send_entry_to_cpu_ring        (0x5238)
#define image_2_start_task_dhd_tx_get_bq_index        (0x6678)
#define image_2_dhd_tx_get_bq_index        (0x6678)
#define image_2_start_task_dhd_tx_post_drop_packet        (0x65bc)
#define image_2_dhd_tx_post_drop_packet        (0x65bc)
#define image_2_start_task_dhd_tx_post_flow_ring_get_info        (0x63b0)
#define image_2_dhd_tx_post_flow_ring_get_info        (0x63b0)
#define image_2_start_task_dhd_tx_put_bq_index        (0x66e8)
#define image_2_dhd_tx_put_bq_index        (0x66e8)
#define image_2_start_task_memcpy_abs_to_fpm        (0x5060)
#define image_2_memcpy_abs_to_fpm        (0x5060)
#define image_2_start_task_memcpy_abs_to_sbpm        (0x4e6c)
#define image_2_memcpy_abs_to_sbpm        (0x4e6c)
#define image_2_start_task_tx_mirror        (0x6b74)
#define image_2_tx_mirror        (0x6b74)

#else

#define image_2_start_task_initialization_task        (0x2)
#define image_2_initialization_task        (0x2)
#define image_2_start_task_processing_wakeup_request        (0x43c)
#define image_2_processing_wakeup_request        (0x43c)
#define image_2_start_task_cpu_tx_wakeup_request        (0x111a)
#define image_2_cpu_tx_wakeup_request        (0x111a)
#define image_2_cpu_tx_read_ring_indices        (0x111a)
#define image_2_start_task_debug_routine        (0xc)
#define image_2_debug_routine        (0xc)
#define image_2_start_task_cpu_recycle_wakeup_request        (0x1290)
#define image_2_cpu_recycle_wakeup_request        (0x1290)
#define image_2_start_task_interrupt_coalescing_1st_wakeup_request        (0x12e4)
#define image_2_interrupt_coalescing_1st_wakeup_request        (0x12e4)
#define image_2_start_task_dhd_tx_post_wakeup_request        (0x1302)
#define image_2_dhd_tx_post_wakeup_request        (0x1302)
#define image_2_start_task_dhd_tx_post_update_fifo_wakeup_request        (0x1300)
#define image_2_dhd_tx_post_update_fifo_wakeup_request        (0x1300)
#define image_2_start_task_dhd_tx_complete_wakeup_request        (0x1396)
#define image_2_dhd_tx_complete_wakeup_request        (0x1396)
#define image_2_start_task_dhd_rx_complete_wakeup_request        (0x1398)
#define image_2_dhd_rx_complete_wakeup_request        (0x1398)
#define image_2_start_task_dhd_timer_1st_wakeup_request        (0x12fe)
#define image_2_dhd_timer_1st_wakeup_request        (0x12fe)
#define image_2_start_task_timer_common_task_wakeup_request        (0x133e)
#define image_2_timer_common_task_wakeup_request        (0x133e)
#define image_2_debug_routine_handler        (0x1)
#define image_2_tcam_cmd_ic_submit        (0xd62)
#define image_2_tcam_cmd_outer_tpid        (0xd74)
#define image_2_tcam_cmd_inner_tpid        (0xd7a)
#define image_2_tcam_cmd_ethertype        (0xd80)
#define image_2_tcam_cmd_outer_vid        (0xd86)
#define image_2_tcam_cmd_inner_vid        (0xd8c)
#define image_2_tcam_cmd_outer_pbit        (0xd92)
#define image_2_tcam_cmd_inner_pbit        (0xd98)
#define image_2_tcam_cmd_vlan_num        (0xd9e)
#define image_2_tcam_cmd_ip_protocol        (0xda4)
#define image_2_tcam_cmd_l3_protocol        (0xdaa)
#define image_2_tcam_cmd_tos        (0xdb0)
#define image_2_tcam_cmd_network_layer        (0xdb6)
#define image_2_tcam_cmd_ingress_port        (0xdbe)
#define image_2_tcam_cmd_gem_flow        (0xdc4)
#define image_2_tcam_cmd_ipv6_label        (0xdca)
#define image_2_tcam_cmd_src_ip        (0xdd6)
#define image_2_tcam_cmd_src_ipv6_masked        (0xdea)
#define image_2_tcam_cmd_dst_ip        (0xdf8)
#define image_2_tcam_cmd_dst_ipv6_masked        (0xe0c)
#define image_2_tcam_cmd_src_port        (0xe1a)
#define image_2_tcam_cmd_dst_port        (0xe20)
#define image_2_tcam_cmd_src_mac        (0xe26)
#define image_2_tcam_cmd_dst_mac        (0xe32)
#define image_2_tcam_cmd_generic_l2        (0xe3e)
#define image_2_tcam_cmd_generic_l3        (0xe4c)
#define image_2_tcam_cmd_generic_l4        (0xe5a)
#define image_2_start_task_c_cpu_tx_wakeup_request        (0x19d1)
#define image_2_c_cpu_tx_wakeup_request        (0x19d1)
#define image_2_start_task_c_dhd_rx_complete_wakeup_request        (0x14c7)
#define image_2_c_dhd_rx_complete_wakeup_request        (0x14c7)
#define image_2_start_task_c_dhd_timer_wakeup_request        (0x15e7)
#define image_2_c_dhd_timer_wakeup_request        (0x15e7)
#define image_2_start_task_c_dhd_tx_complete_wakeup_request        (0x156e)
#define image_2_c_dhd_tx_complete_wakeup_request        (0x156e)
#define image_2_start_task_c_dhd_tx_post_update_fifo_wakeup_request        (0x1631)
#define image_2_c_dhd_tx_post_update_fifo_wakeup_request        (0x1631)
#define image_2_start_task_c_dhd_tx_post_wakeup_request        (0x1643)
#define image_2_c_dhd_tx_post_wakeup_request        (0x1643)
#define image_2_start_task_cpu_tx_send_pd_to_recycle        (0x1ac4)
#define image_2_cpu_tx_send_pd_to_recycle        (0x1ac4)
#define image_2_start_task_dhd_send_entry_to_cpu_ring        (0x148e)
#define image_2_dhd_send_entry_to_cpu_ring        (0x148e)
#define image_2_start_task_dhd_tx_get_bq_index        (0x199e)
#define image_2_dhd_tx_get_bq_index        (0x199e)
#define image_2_start_task_dhd_tx_post_drop_packet        (0x196f)
#define image_2_dhd_tx_post_drop_packet        (0x196f)
#define image_2_start_task_dhd_tx_post_flow_ring_get_info        (0x18ec)
#define image_2_dhd_tx_post_flow_ring_get_info        (0x18ec)
#define image_2_start_task_dhd_tx_put_bq_index        (0x19ba)
#define image_2_dhd_tx_put_bq_index        (0x19ba)
#define image_2_start_task_memcpy_abs_to_fpm        (0x1418)
#define image_2_memcpy_abs_to_fpm        (0x1418)
#define image_2_start_task_memcpy_abs_to_sbpm        (0x139b)
#define image_2_memcpy_abs_to_sbpm        (0x139b)
#define image_2_start_task_tx_mirror        (0x1add)
#define image_2_tx_mirror        (0x1add)

#endif


#endif

/* COMMON LABELS */
#ifndef COMMON_CODE_ADDRESSES
#define COMMON_CODE_ADDRESSES

#define INVALID_LABEL_ADDRESS 0xFFFFFF

#ifndef PC_ADDRESS_INST_IND

#define TCAM_CMD_DST_IP_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3640, 0x37e0}
#define TCAM_CMD_DST_IPV6_MASKED_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3690, 0x3830}
#define TCAM_CMD_DST_MAC_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3728, 0x38c8}
#define TCAM_CMD_DST_PORT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x36e0, 0x3880}
#define TCAM_CMD_ETHERTYPE_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3460, 0x3600}
#define TCAM_CMD_GEM_FLOW_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3570, 0x3710}
#define TCAM_CMD_GENERIC_L2_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3758, 0x38f8}
#define TCAM_CMD_GENERIC_L3_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3790, 0x3930}
#define TCAM_CMD_GENERIC_L4_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x37c8, 0x3968}
#define TCAM_CMD_IC_SUBMIT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x33e8, 0x3588}
#define TCAM_CMD_INGRESS_PORT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3558, 0x36f8}
#define TCAM_CMD_INNER_PBIT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x34c0, 0x3660}
#define TCAM_CMD_INNER_TPID_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3448, 0x35e8}
#define TCAM_CMD_INNER_VID_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3490, 0x3630}
#define TCAM_CMD_IP_PROTOCOL_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x34f0, 0x3690}
#define TCAM_CMD_IPV6_LABEL_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3588, 0x3728}
#define TCAM_CMD_L3_PROTOCOL_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3508, 0x36a8}
#define TCAM_CMD_NETWORK_LAYER_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3538, 0x36d8}
#define TCAM_CMD_OUTER_PBIT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x34a8, 0x3648}
#define TCAM_CMD_OUTER_TPID_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3430, 0x35d0}
#define TCAM_CMD_OUTER_VID_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3478, 0x3618}
#define TCAM_CMD_SRC_IP_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x35b8, 0x3758}
#define TCAM_CMD_SRC_IPV6_MASKED_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3608, 0x37a8}
#define TCAM_CMD_SRC_MAC_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x36f8, 0x3898}
#define TCAM_CMD_SRC_PORT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x36c8, 0x3868}
#define TCAM_CMD_TOS_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x3520, 0x36c0}
#define TCAM_CMD_VLAN_NUM_ADDR_ARR {INVALID_LABEL_ADDRESS, 0x34d8, 0x3678}

#else

#define TCAM_CMD_DST_IP_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xd90, 0xdf8}
#define TCAM_CMD_DST_IPV6_MASKED_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xda4, 0xe0c}
#define TCAM_CMD_DST_MAC_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xdca, 0xe32}
#define TCAM_CMD_DST_PORT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xdb8, 0xe20}
#define TCAM_CMD_ETHERTYPE_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xd18, 0xd80}
#define TCAM_CMD_GEM_FLOW_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xd5c, 0xdc4}
#define TCAM_CMD_GENERIC_L2_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xdd6, 0xe3e}
#define TCAM_CMD_GENERIC_L3_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xde4, 0xe4c}
#define TCAM_CMD_GENERIC_L4_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xdf2, 0xe5a}
#define TCAM_CMD_IC_SUBMIT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xcfa, 0xd62}
#define TCAM_CMD_INGRESS_PORT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xd56, 0xdbe}
#define TCAM_CMD_INNER_PBIT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xd30, 0xd98}
#define TCAM_CMD_INNER_TPID_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xd12, 0xd7a}
#define TCAM_CMD_INNER_VID_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xd24, 0xd8c}
#define TCAM_CMD_IP_PROTOCOL_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xd3c, 0xda4}
#define TCAM_CMD_IPV6_LABEL_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xd62, 0xdca}
#define TCAM_CMD_L3_PROTOCOL_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xd42, 0xdaa}
#define TCAM_CMD_NETWORK_LAYER_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xd4e, 0xdb6}
#define TCAM_CMD_OUTER_PBIT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xd2a, 0xd92}
#define TCAM_CMD_OUTER_TPID_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xd0c, 0xd74}
#define TCAM_CMD_OUTER_VID_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xd1e, 0xd86}
#define TCAM_CMD_SRC_IP_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xd6e, 0xdd6}
#define TCAM_CMD_SRC_IPV6_MASKED_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xd82, 0xdea}
#define TCAM_CMD_SRC_MAC_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xdbe, 0xe26}
#define TCAM_CMD_SRC_PORT_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xdb2, 0xe1a}
#define TCAM_CMD_TOS_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xd48, 0xdb0}
#define TCAM_CMD_VLAN_NUM_ADDR_ARR {INVALID_LABEL_ADDRESS, 0xd36, 0xd9e}

#endif


#endif

