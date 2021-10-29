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


#ifndef _RDD_AG_SPDSVC_GEN_H_
#define _RDD_AG_SPDSVC_GEN_H_

int rdd_ag_spdsvc_gen_udpspdtest_stream_tx_stat_table_set(uint32_t tx_packets_0, uint32_t tx_packets_1, uint32_t tx_drops_no_spbm, uint32_t first_ts, uint32_t last_ts_0, uint32_t last_ts_1, uint32_t iperf3_ts_sec, uint32_t iperf3_ts_usec, bdmf_boolean first_ts_set);
int rdd_ag_spdsvc_gen_udpspdtest_stream_tx_stat_table_get(uint32_t *tx_packets_0, uint32_t *tx_packets_1, uint32_t *tx_drops_no_spbm, uint32_t *first_ts, uint32_t *last_ts_0, uint32_t *last_ts_1, uint32_t *iperf3_ts_sec, uint32_t *iperf3_ts_usec, bdmf_boolean *first_ts_set);

#endif /* _RDD_AG_SPDSVC_GEN_H_ */
