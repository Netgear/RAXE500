/*
* <:copyright-BRCM:2014:DUAL/GPL:standard
* 
*    Copyright (c) 2014 Broadcom 
*    All Rights Reserved
* 
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License, version 2, as published by
* the Free Software Foundation (the "GPL").
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* 
* A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
* writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
* Boston, MA 02111-1307, USA.
* 
* :>
*/

#ifndef _RDPA_SPDTEST_COMMON_H_
#define _RDPA_SPDTEST_COMMON_H_

#include "rdpa_types.h"

/** Minimum transmitted packet size for padding */
#define RDPA_SPDTEST_MIN_TX_PD_LEN 60


/** Speed Test Engine Reference Packet, includes both packet header and payload.\n
 * required by the Runner Speed Test Engine for TX tests.\n
 */
typedef struct
{
    uint16_t size;           /**< Reference packet size */
    void *data;              /**< Reference packet pointer */
    union {
        struct {
            uint16_t payload_offset; /**< Reference packet payload offset */
        } udp;
        uint16_t h;
    };
} rdpa_spdtest_ref_pkt_t;


/** UDP Speed Test RX parameters.\n
 */
typedef struct {
    bdmf_ip_t local_ip_addr;   /**< Local IP Address */
    uint16_t local_port_nbr;   /**< Local UDP Port Number */
    bdmf_ip_t remote_ip_addr;  /**< Remote IP Address */
    uint16_t remote_port_nbr;  /**< Remote UDP Port Number */
    bdmf_index flow_index[rdpa_if__number_of];  /**< Analyzer Unicast Flow Index */
} rdpa_udpspdtest_rx_params_t;

/** UDP Speed Test TX parameters.\n
 */
typedef struct {
    uint32_t kbps;           /**< Transmit Rate (Kbps) */
    uint32_t mbs;            /**< Maximum Burst Size (bytes) */
    uint64_t total_packets_to_send; /**< Total number of packets to send (optional) */
    bdmf_boolean iperf3_64bit_counters; /**< Iperf3 configuration: use 64-bit counters in UDP test packets */
} rdpa_udpspdtest_tx_params_t;

#endif /* _RDPA_SPDTEST_H_ */
