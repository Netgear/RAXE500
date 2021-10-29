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


#include "rdd.h"

#include "rdd_ag_spdsvc_gen.h"

int rdd_ag_spdsvc_gen_udpspdtest_stream_tx_stat_table_set(uint32_t tx_packets_0, uint32_t tx_packets_1, uint32_t tx_drops_no_spbm, uint32_t first_ts, uint32_t last_ts_0, uint32_t last_ts_1, uint32_t iperf3_ts_sec, uint32_t iperf3_ts_usec, bdmf_boolean first_ts_set)
{
    RDD_UDPSPDTEST_STREAM_TX_STAT_TX_PACKETS_0_WRITE_G(tx_packets_0, RDD_UDPSPDTEST_STREAM_TX_STAT_TABLE_ADDRESS_ARR, 0);
    RDD_UDPSPDTEST_STREAM_TX_STAT_TX_PACKETS_1_WRITE_G(tx_packets_1, RDD_UDPSPDTEST_STREAM_TX_STAT_TABLE_ADDRESS_ARR, 0);
    RDD_UDPSPDTEST_STREAM_TX_STAT_TX_DROPS_NO_SPBM_WRITE_G(tx_drops_no_spbm, RDD_UDPSPDTEST_STREAM_TX_STAT_TABLE_ADDRESS_ARR, 0);
    RDD_UDPSPDTEST_STREAM_TX_STAT_FIRST_TS_WRITE_G(first_ts, RDD_UDPSPDTEST_STREAM_TX_STAT_TABLE_ADDRESS_ARR, 0);
    RDD_UDPSPDTEST_STREAM_TX_STAT_LAST_TS_0_WRITE_G(last_ts_0, RDD_UDPSPDTEST_STREAM_TX_STAT_TABLE_ADDRESS_ARR, 0);
    RDD_UDPSPDTEST_STREAM_TX_STAT_LAST_TS_1_WRITE_G(last_ts_1, RDD_UDPSPDTEST_STREAM_TX_STAT_TABLE_ADDRESS_ARR, 0);
    RDD_UDPSPDTEST_STREAM_TX_STAT_IPERF3_TS_SEC_WRITE_G(iperf3_ts_sec, RDD_UDPSPDTEST_STREAM_TX_STAT_TABLE_ADDRESS_ARR, 0);
    RDD_UDPSPDTEST_STREAM_TX_STAT_IPERF3_TS_USEC_WRITE_G(iperf3_ts_usec, RDD_UDPSPDTEST_STREAM_TX_STAT_TABLE_ADDRESS_ARR, 0);
    RDD_UDPSPDTEST_STREAM_TX_STAT_FIRST_TS_SET_WRITE_G(first_ts_set, RDD_UDPSPDTEST_STREAM_TX_STAT_TABLE_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

int rdd_ag_spdsvc_gen_udpspdtest_stream_tx_stat_table_get(uint32_t *tx_packets_0, uint32_t *tx_packets_1, uint32_t *tx_drops_no_spbm, uint32_t *first_ts, uint32_t *last_ts_0, uint32_t *last_ts_1, uint32_t *iperf3_ts_sec, uint32_t *iperf3_ts_usec, bdmf_boolean *first_ts_set)
{
    RDD_UDPSPDTEST_STREAM_TX_STAT_TX_PACKETS_0_READ_G(*tx_packets_0, RDD_UDPSPDTEST_STREAM_TX_STAT_TABLE_ADDRESS_ARR, 0);
    RDD_UDPSPDTEST_STREAM_TX_STAT_TX_PACKETS_1_READ_G(*tx_packets_1, RDD_UDPSPDTEST_STREAM_TX_STAT_TABLE_ADDRESS_ARR, 0);
    RDD_UDPSPDTEST_STREAM_TX_STAT_TX_DROPS_NO_SPBM_READ_G(*tx_drops_no_spbm, RDD_UDPSPDTEST_STREAM_TX_STAT_TABLE_ADDRESS_ARR, 0);
    RDD_UDPSPDTEST_STREAM_TX_STAT_FIRST_TS_READ_G(*first_ts, RDD_UDPSPDTEST_STREAM_TX_STAT_TABLE_ADDRESS_ARR, 0);
    RDD_UDPSPDTEST_STREAM_TX_STAT_LAST_TS_0_READ_G(*last_ts_0, RDD_UDPSPDTEST_STREAM_TX_STAT_TABLE_ADDRESS_ARR, 0);
    RDD_UDPSPDTEST_STREAM_TX_STAT_LAST_TS_1_READ_G(*last_ts_1, RDD_UDPSPDTEST_STREAM_TX_STAT_TABLE_ADDRESS_ARR, 0);
    RDD_UDPSPDTEST_STREAM_TX_STAT_IPERF3_TS_SEC_READ_G(*iperf3_ts_sec, RDD_UDPSPDTEST_STREAM_TX_STAT_TABLE_ADDRESS_ARR, 0);
    RDD_UDPSPDTEST_STREAM_TX_STAT_IPERF3_TS_USEC_READ_G(*iperf3_ts_usec, RDD_UDPSPDTEST_STREAM_TX_STAT_TABLE_ADDRESS_ARR, 0);
    RDD_UDPSPDTEST_STREAM_TX_STAT_FIRST_TS_SET_READ_G(*first_ts_set, RDD_UDPSPDTEST_STREAM_TX_STAT_TABLE_ADDRESS_ARR, 0);

    return BDMF_ERR_OK;
}

