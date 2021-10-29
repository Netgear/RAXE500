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


#ifndef _RDD_AG_PKTGEN_TX_H_
#define _RDD_AG_PKTGEN_TX_H_

int rdd_ag_pktgen_tx_pktgen_no_sbpm_hdrs_cntr_set(uint32_t bits);
int rdd_ag_pktgen_tx_pktgen_no_sbpm_hdrs_cntr_get(uint32_t *bits);
int rdd_ag_pktgen_tx_pktgen_fpm_ug_mgmt_set(uint32_t budget, uint32_t fpm_ug_cnt_dummy, uint32_t fpm_ug_cnt, uint32_t fpm_ug_cnt_reg_addr, uint32_t fpm_ug_threshold, uint8_t fpm_tokens_quantum);
int rdd_ag_pktgen_tx_pktgen_fpm_ug_mgmt_get(uint32_t *budget, uint32_t *fpm_ug_cnt_dummy, uint32_t *fpm_ug_cnt, uint32_t *fpm_ug_cnt_reg_addr, uint32_t *fpm_ug_threshold, uint8_t *fpm_tokens_quantum);
int rdd_ag_pktgen_tx_pktgen_curr_sbpm_hdr_ptr_set(uint16_t bits);
int rdd_ag_pktgen_tx_pktgen_curr_sbpm_hdr_ptr_get(uint16_t *bits);
int rdd_ag_pktgen_tx_pktgen_num_of_avail_sbpm_hdrs_set(uint16_t bits);
int rdd_ag_pktgen_tx_pktgen_num_of_avail_sbpm_hdrs_get(uint16_t *bits);
int rdd_ag_pktgen_tx_pktgen_sbpm_end_ptr_set(uint16_t bits);
int rdd_ag_pktgen_tx_pktgen_sbpm_end_ptr_get(uint16_t *bits);
int rdd_ag_pktgen_tx_pktgen_sbpm_hdr_bns_set(uint32_t _entry, uint16_t first_bn, uint8_t ext_idx);
int rdd_ag_pktgen_tx_pktgen_sbpm_hdr_bns_get(uint32_t _entry, uint16_t *first_bn, uint8_t *ext_idx);
int rdd_ag_pktgen_tx_pktgen_max_ut_pkts_set(uint32_t bits);
int rdd_ag_pktgen_tx_pktgen_max_ut_pkts_get(uint32_t *bits);
int rdd_ag_pktgen_tx_pktgen_ut_trigger_set(uint32_t bits);
int rdd_ag_pktgen_tx_pktgen_ut_trigger_get(uint32_t *bits);
int rdd_ag_pktgen_tx_pktgen_session_data_set(uint32_t bbmsg_sbpm_mcast_inc_req_0, uint32_t bbmsg_sbpm_mcast_inc_req_1, uint32_t ref_pd_0, uint32_t ref_pd_1, uint32_t ref_pd_2, uint32_t ref_pd_3);
int rdd_ag_pktgen_tx_pktgen_session_data_get(uint32_t *bbmsg_sbpm_mcast_inc_req_0, uint32_t *bbmsg_sbpm_mcast_inc_req_1, uint32_t *ref_pd_0, uint32_t *ref_pd_1, uint32_t *ref_pd_2, uint32_t *ref_pd_3);
int rdd_ag_pktgen_tx_pktgen_sbpm_exts_set(uint32_t _entry, uint8_t num_of_bns, uint16_t bn1);
int rdd_ag_pktgen_tx_pktgen_sbpm_exts_get(uint32_t _entry, uint8_t *num_of_bns, uint16_t *bn1);

#endif /* _RDD_AG_PKTGEN_TX_H_ */
