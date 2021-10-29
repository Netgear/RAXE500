/*
   Copyright (c) 2014 Broadcom
   All Rights Reserved

    <:label-BRCM:2014:DUAL/GPL:standard
    
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

/*
 * rdpa_types.c
 *
 * RDPA types exposure to the management framework
 *
 *  Created on: Aug 16, 2012
 *      Author: igort
 */


#include "bdmf_dev.h"
#include "rdpa_types.h"
#include "rdpa_common.h"
#include "rdpa_ingress_class_basic.h"
#include "rdpa_filter.h"
#include "rdpa_mcast_basic.h"

#if defined(BCM_PON_XRDP)
#include "rdpa_ip_class.h"
#include "rdd_data_structures_auto.h"

#if defined(OPERATION_MODE_PRV)
#define FLOW_RESULT_VAL_TO_S NULL
#else
extern int l2l3_flow_result_wl_metadata_result_val_to_s(struct bdmf_object *mo, struct bdmf_attr *ad, const void *val, char *sbuf,
    uint32_t _size);

#define FLOW_RESULT_VAL_TO_S l2l3_flow_result_wl_metadata_result_val_to_s
#endif
#endif

/* rdpa_stat aggregate type */
struct bdmf_aggr_type rdpa_stat_aggr_type =
{
    .name = "rdpa_stat", .struct_name = "rdpa_stat_t",
    .help = "Packet and Byte Statistics",
    .size = sizeof(rdpa_stat_t),
    .fields = (struct bdmf_attr[]) {
        { .name = "packets", .help = "Packets", .size = sizeof(uint32_t), .flags = BDMF_ATTR_UNSIGNED,
          .type = bdmf_attr_number, .offset = offsetof(rdpa_stat_t, packets)
        },
        { .name = "bytes", .help = "Bytes", .size = sizeof(uint32_t), .flags = BDMF_ATTR_UNSIGNED,
          .type = bdmf_attr_number, .offset = offsetof(rdpa_stat_t, bytes)
        },
        BDMF_ATTR_LAST
    },
};
DECLARE_BDMF_AGGREGATE_TYPE(rdpa_stat_aggr_type);

/* stat type table */
const bdmf_attr_enum_table_t rdpa_stat_type_enum_table =
{
    .type_name = "stat_type", .help = "selects stat type (packets only or packets+bytes)",
    .values =
    {
        {"packets_only", rdpa_stat_packets_only},
        {"packets_and_bytes", rdpa_stat_packets_and_bytes},
        {NULL, 0}
    }
};

/* rdpa_stat_1way aggregate type */
struct bdmf_aggr_type rdpa_stat_1_way_aggr_type =
{
    .name = "rdpa_stat_1way", .struct_name = "rdpa_stat_1way_t",
    .help = "Passed and Discarded Packet and Byte Statistics",
    .size = sizeof(rdpa_stat_1way_t),
    .fields = (struct bdmf_attr[]) {
        { .name = "passed", .help = "Packets/bytes passed",
          .type = bdmf_attr_aggregate, .ts.aggr_type_name = "rdpa_stat",
          .offset = offsetof(rdpa_stat_1way_t, passed)
        },
        { .name = "discarded", .help = "Packets/bytes discarded",
          .type = bdmf_attr_aggregate, .ts.aggr_type_name = "rdpa_stat",
          .offset = offsetof(rdpa_stat_1way_t, discarded)
        },
        BDMF_ATTR_LAST
    }
};
DECLARE_BDMF_AGGREGATE_TYPE(rdpa_stat_1_way_aggr_type);

/* rdpa_stat_tx_rx aggregate type */
struct bdmf_aggr_type rdpa_stat_tx_rx_aggr_type = {
    .name = "rdpa_stat_tx_rx", .struct_name = "rdpa_stat_tx_rx_t",
    .help = "Passed and Discarded Packet and Byte Tx+Rx Statistics",
    .size = sizeof(rdpa_stat_tx_rx_t),
    .fields = (struct bdmf_attr[]) {
        { .name = "tx", .help = "Transmit",
          .type = bdmf_attr_aggregate, .ts.aggr_type_name = "rdpa_stat_1way",
          .offset = offsetof(rdpa_stat_tx_rx_t, tx)
        },
        { .name = "rx", .help = "Receive",
          .type = bdmf_attr_aggregate, .ts.aggr_type_name = "rdpa_stat_1way",
          .offset = offsetof(rdpa_stat_tx_rx_t, rx)
        },
        BDMF_ATTR_LAST
    }
};
DECLARE_BDMF_AGGREGATE_TYPE(rdpa_stat_tx_rx_aggr_type);

/* rdpa_stat_tx_rx_valid aggregate type */
struct bdmf_aggr_type rdpa_stat_tx_rx_valid_aggr_type = {
    .name = "rdpa_stat_tx_rx_valid", .struct_name = "rdpa_stat_tx_rx_valid_t",
    .help = "Passed Packet and Byte Tx+Rx Statistics",
    .size = sizeof(rdpa_stat_tx_rx_valid_t),
    .fields = (struct bdmf_attr[]) {
        { .name = "tx", .help = "Transmit",
          .type = bdmf_attr_aggregate, .ts.aggr_type_name = "rdpa_stat",
          .offset = offsetof(rdpa_stat_tx_rx_valid_t, tx)
        },
        { .name = "rx", .help = "Receive",
          .type = bdmf_attr_aggregate, .ts.aggr_type_name = "rdpa_stat",
          .offset = offsetof(rdpa_stat_tx_rx_valid_t, rx)
        },
        BDMF_ATTR_LAST
    }
};
DECLARE_BDMF_AGGREGATE_TYPE(rdpa_stat_tx_rx_valid_aggr_type);

/* rdpa_dir_index aggregate type */
struct bdmf_aggr_type rdpa_dir_index_aggr_type =
{
    .name = "rdpa_dir_index", .struct_name = "rdpa_dir_index_t",
    .help = "Traffic direction + index",
    .size = sizeof(rdpa_dir_index_t),
    .fields = (struct bdmf_attr[]) {
        { .name = "dir", .help = "Direction", .size = sizeof(rdpa_traffic_dir),
          .type = bdmf_attr_enum, .offset = offsetof(rdpa_dir_index_t, dir),
          .ts.enum_table = &rdpa_traffic_dir_enum_table
        },
        { .name = "index", .help = "Bytes", .size = sizeof(bdmf_index),
          .type = bdmf_attr_number, .offset = offsetof(rdpa_dir_index_t, index)
        },
        BDMF_ATTR_LAST
    }
};
DECLARE_BDMF_AGGREGATE_TYPE(rdpa_dir_index_aggr_type);

const bdmf_attr_enum_table_t rdpa_if_enum_table =
{
    .type_name = "rdpa_if", .help = "Interface",
    .values = {
        /** WAN ports */
        {"wan0", rdpa_if_wan0},
        {"wan1", rdpa_if_wan1},
        {"wan2", rdpa_if_wan2},

        /** LAN ports */
        {"lan0", rdpa_if_lan0},
        {"lan1", rdpa_if_lan1},
        {"lan2", rdpa_if_lan2},
        {"lan3", rdpa_if_lan3},
        {"lan4", rdpa_if_lan4},
        {"lan5", rdpa_if_lan5},
        {"lan6", rdpa_if_lan6},
        {"lan7", rdpa_if_lan7},
#if !defined(BCM_DSL_RDP) && !defined(BCM_DSL_XRDP)
        {"lan8", rdpa_if_lan8},
        {"lan9", rdpa_if_lan9},
        {"lan10", rdpa_if_lan10},
        {"lan11", rdpa_if_lan11},
        {"lan12", rdpa_if_lan12},
        {"lan13", rdpa_if_lan13},
        {"lan14", rdpa_if_lan14},
        {"lan15", rdpa_if_lan15},
        {"lan16", rdpa_if_lan16},
        {"lan17", rdpa_if_lan17},
        {"lan18", rdpa_if_lan18},
        {"lan19", rdpa_if_lan19},
        {"lan20", rdpa_if_lan20},
        {"lan21", rdpa_if_lan21},
#endif
#ifdef G9991
        {"lan22", rdpa_if_lan22},
        {"lan23", rdpa_if_lan23},
        {"lan24", rdpa_if_lan24},
        {"lan25", rdpa_if_lan25},
        {"lan26", rdpa_if_lan26},
        {"lan27", rdpa_if_lan27},
        {"lan28", rdpa_if_lan28},
        {"lan29", rdpa_if_lan29},
#endif

        /** Special ports */
        {"lag0", rdpa_if_lag0},
        {"lag1", rdpa_if_lag1},
        {"lag2", rdpa_if_lag2},
        {"lag3", rdpa_if_lag3},
        {"lag4", rdpa_if_lag4},

        /** Switch aggregate port */
        {"switch", rdpa_if_switch},

#ifndef XRDP
        /** WiFi physical ports */
        {"wlan0", rdpa_if_wlan0},
        {"wlan1", rdpa_if_wlan1},

        /** CPU (local termination) */
        {"cpu",  rdpa_if_cpu},

        /** WiFi logical ports (SSIDs) */
        {"ssid0", rdpa_if_ssid0},
        {"ssid1", rdpa_if_ssid1},
        {"ssid2", rdpa_if_ssid2},
        {"ssid3", rdpa_if_ssid3},
        {"ssid4", rdpa_if_ssid4},
        {"ssid5", rdpa_if_ssid5},
        {"ssid6", rdpa_if_ssid6},
        {"ssid7", rdpa_if_ssid7},
        {"ssid8", rdpa_if_ssid8},
        {"ssid9", rdpa_if_ssid9},
        {"ssid10", rdpa_if_ssid10},
        {"ssid11", rdpa_if_ssid11},
        {"ssid12", rdpa_if_ssid12},
        {"ssid13", rdpa_if_ssid13},
        {"ssid14", rdpa_if_ssid14},
        {"ssid15", rdpa_if_ssid15},
#else
        /** CPU (local termination) */
        {"cpu0", rdpa_if_cpu0},
        {"cpu1", rdpa_if_cpu1},
        {"cpu2", rdpa_if_cpu2},
        {"cpu3", rdpa_if_cpu3},
        {"wlan0", rdpa_if_wlan0},
        {"wlan1", rdpa_if_wlan1},
        {"wlan2", rdpa_if_wlan2},
#endif
        {"bond0", rdpa_if_bond0},
        {"bond1", rdpa_if_bond1},
        {"bond2", rdpa_if_bond2},

        {"any", rdpa_if_any},
        {"none", rdpa_if_none},
        {NULL, 0}
    }
};

const bdmf_attr_enum_table_t rdpa_lan_wan_if_enum_table =
{
    .type_name = "rdpa_if", .help = "Interface",
    .values = {
        /** WAN ports */
        {"wan0", rdpa_if_wan0},
        {"wan1", rdpa_if_wan1},
        {"wan2", rdpa_if_wan2},

        /** LAN ports */
        {"lan0", rdpa_if_lan0},
        {"lan1", rdpa_if_lan1},
        {"lan2", rdpa_if_lan2},
        {"lan3", rdpa_if_lan3},
        {"lan4", rdpa_if_lan4},
        {"lan5", rdpa_if_lan5},
        {"lan6", rdpa_if_lan6},
        {"lan7", rdpa_if_lan7},
#if !defined(BCM_DSL_RDP) && !defined(BCM_DSL_XRDP)
        {"lan8", rdpa_if_lan8},
        {"lan9", rdpa_if_lan9},
        {"lan10", rdpa_if_lan10},
        {"lan11", rdpa_if_lan11},
        {"lan12", rdpa_if_lan12},
        {"lan13", rdpa_if_lan13},
        {"lan14", rdpa_if_lan14},
        {"lan15", rdpa_if_lan15},
        {"lan16", rdpa_if_lan16},
        {"lan17", rdpa_if_lan17},
        {"lan18", rdpa_if_lan18},
        {"lan19", rdpa_if_lan19},
        {"lan20", rdpa_if_lan20},
        {"lan21", rdpa_if_lan21},
#endif
#ifdef G9991
        {"lan22", rdpa_if_lan22},
        {"lan23", rdpa_if_lan23},
        {"lan24", rdpa_if_lan24},
        {"lan25", rdpa_if_lan25},
        {"lan26", rdpa_if_lan26},
        {"lan27", rdpa_if_lan27},
        {"lan28", rdpa_if_lan28},
        {"lan29", rdpa_if_lan29},
#endif
        {NULL, 0}
    }
};

const bdmf_attr_enum_table_t rdpa_lan_or_cpu_if_enum_table =
{
    .type_name = "rdpa_if", .help = "Interface",
    .values = {
        /** LAN ports */
        {"lan0", rdpa_if_lan0},
        {"lan1", rdpa_if_lan1},
        {"lan2", rdpa_if_lan2},
        {"lan3", rdpa_if_lan3},
        {"lan4", rdpa_if_lan4}, 

#ifndef XRDP
        /** WiFi logical ports (SSIDs) */
        {"ssid0", rdpa_if_ssid0},
        {"ssid1", rdpa_if_ssid1},
        {"ssid2", rdpa_if_ssid2},
        {"ssid3", rdpa_if_ssid3},
        {"ssid4", rdpa_if_ssid4},
        {"ssid5", rdpa_if_ssid5},
        {"ssid6", rdpa_if_ssid6},
        {"ssid7", rdpa_if_ssid7},
        {"ssid8", rdpa_if_ssid8},
        {"ssid9", rdpa_if_ssid9},
        {"ssid10", rdpa_if_ssid10},
        {"ssid11", rdpa_if_ssid11},
        {"ssid12", rdpa_if_ssid12},
        {"ssid13", rdpa_if_ssid13},
        {"ssid14", rdpa_if_ssid14},
        {"ssid15", rdpa_if_ssid15},
#else
        {"lan5", rdpa_if_lan5}, 
        {"lan6", rdpa_if_lan6}, 
        {"lan7", rdpa_if_lan7},
#if !defined(BCM_DSL_RDP) && !defined(BCM_DSL_XRDP)
        {"lan8", rdpa_if_lan8}, 
        {"lan9", rdpa_if_lan9}, 
        {"lan10", rdpa_if_lan10}, 
        {"lan11", rdpa_if_lan11}, 
        {"lan12", rdpa_if_lan12}, 
        {"lan13", rdpa_if_lan13}, 
        {"lan14", rdpa_if_lan14}, 
        {"lan15", rdpa_if_lan15}, 
        {"lan16", rdpa_if_lan16}, 
        {"lan17", rdpa_if_lan17}, 
        {"lan18", rdpa_if_lan18}, 
        {"lan19", rdpa_if_lan19}, 
        {"lan20", rdpa_if_lan20}, 
        {"lan21", rdpa_if_lan21}, 
#endif
#ifdef G9991
        {"lan22", rdpa_if_lan22}, 
        {"lan23", rdpa_if_lan23}, 
        {"lan24", rdpa_if_lan24}, 
        {"lan25", rdpa_if_lan25}, 
        {"lan26", rdpa_if_lan26}, 
        {"lan27", rdpa_if_lan27}, 
        {"lan28", rdpa_if_lan28}, 
        {"lan29", rdpa_if_lan29}, 
#endif

        {"cpu0", rdpa_if_cpu0},
        {"cpu1", rdpa_if_cpu1},
        {"cpu2", rdpa_if_cpu2},
        {"cpu3", rdpa_if_cpu3},
        {"wlan0", rdpa_if_wlan0},
        {"wlan1", rdpa_if_wlan1},
        {"wlan2", rdpa_if_wlan2},
#endif
        {NULL, 0}
    }
};

const bdmf_attr_enum_table_t rdpa_lan_wan_wlan_if_enum_table =
{
    .type_name = "rdpa_if", .help = "Interface",
    .values = {
        /** WAN ports */
        {"wan0", rdpa_if_wan0},
        {"wan1", rdpa_if_wan1},
        {"wan2", rdpa_if_wan2},

        /** LAN ports */
        {"lan0", rdpa_if_lan0},
        {"lan1", rdpa_if_lan1},
        {"lan2", rdpa_if_lan2},
        {"lan3", rdpa_if_lan3},
        {"lan4", rdpa_if_lan4},
        {"lan5", rdpa_if_lan5},
        {"lan6", rdpa_if_lan6},
        {"lan7", rdpa_if_lan7},
#if !defined(BCM_DSL_RDP) && !defined(BCM_DSL_XRDP)
        {"lan8", rdpa_if_lan8},
        {"lan9", rdpa_if_lan9},
        {"lan10", rdpa_if_lan10},
        {"lan11", rdpa_if_lan11},
        {"lan12", rdpa_if_lan12},
        {"lan13", rdpa_if_lan13},
        {"lan14", rdpa_if_lan14},
        {"lan15", rdpa_if_lan15},
        {"lan16", rdpa_if_lan16},
        {"lan17", rdpa_if_lan17},
        {"lan18", rdpa_if_lan18},
        {"lan19", rdpa_if_lan19},
        {"lan20", rdpa_if_lan20},
        {"lan21", rdpa_if_lan21},
#endif
#ifdef G9991
        {"lan22", rdpa_if_lan22},
        {"lan23", rdpa_if_lan23},
        {"lan24", rdpa_if_lan24},
        {"lan25", rdpa_if_lan25},
        {"lan26", rdpa_if_lan26},
        {"lan27", rdpa_if_lan27},
        {"lan28", rdpa_if_lan28},
        {"lan29", rdpa_if_lan29},
#endif

        /** Switch port */
        {"switch", rdpa_if_switch},

        /** CPU ports */
#ifdef XRDP
        {"cpu0", rdpa_if_cpu0},
        {"cpu1", rdpa_if_cpu1},
        {"cpu2", rdpa_if_cpu2},
        {"cpu3", rdpa_if_cpu3},
#endif

        /** WLAN ports */
        {"wlan0", rdpa_if_wlan0},
        {"wlan1", rdpa_if_wlan1},
#ifdef XRDP
        {"wlan2", rdpa_if_wlan2},
#endif
        {NULL, 0}
    }
};

const bdmf_attr_enum_table_t rdpa_wlan_ssid_enum_table =
{
    .type_name = "wlan_ssid", .help = "WLAN mcast SSID",
    .values = {
        {"ssid_0", rdpa_wlan_ssid0},
        {"ssid_1", rdpa_wlan_ssid1},
        {"ssid_2", rdpa_wlan_ssid2},
        {"ssid_3", rdpa_wlan_ssid3},
        {"ssid_4", rdpa_wlan_ssid4},
        {"ssid_5", rdpa_wlan_ssid5},
        {"ssid_6", rdpa_wlan_ssid6},
        {"ssid_7", rdpa_wlan_ssid7},
        {"ssid_8", rdpa_wlan_ssid8},
        {"ssid_9", rdpa_wlan_ssid9},
        {"ssid_10", rdpa_wlan_ssid10},
        {"ssid_11", rdpa_wlan_ssid11},
        {"ssid_12", rdpa_wlan_ssid12},
        {"ssid_13", rdpa_wlan_ssid13},
        {"ssid_14", rdpa_wlan_ssid14},
        {"ssid_15", rdpa_wlan_ssid15},
        {NULL, 0}
    }
};

const bdmf_attr_enum_table_t rdpa_emac_enum_table =
{
    .type_name = "rdpa_emac", .help = "EMAC",
    .values = {
        {"emac0", rdpa_emac0},
        {"emac1", rdpa_emac1},
        {"emac2", rdpa_emac2},
        {"emac3", rdpa_emac3},
        {"emac4", rdpa_emac4},
        {"emac5", rdpa_emac5},
        {"emac6", rdpa_emac6},
        {"emac7", rdpa_emac7},
        {"max", rdpa_emac__num_of},
        {"none", rdpa_emac_none},
        {NULL, 0}
    }
};

const bdmf_attr_enum_table_t rdpa_wan_emac_enum_table =
{
    .type_name = "rdpa_emac", .help = "WAN EMAC",
    .values = {
        {"none", rdpa_emac_none},
        {"emac0", rdpa_emac0},
        {"emac1", rdpa_emac1},
        {"emac2", rdpa_emac2},
        {"emac3", rdpa_emac3},
        {"emac4", rdpa_emac4},
        {"emac5", rdpa_emac5},
        {NULL, 0}
    }
};

const bdmf_attr_enum_table_t rdpa_wan_type_enum_table =
{
    .type_name = "rdpa_wan_type", .help = "WAN interface type",
    .values = {
        {"not set", rdpa_wan_none},
        {"gpon", rdpa_wan_gpon},
        {"epon", rdpa_wan_epon},
        {"gbe", rdpa_wan_gbe},
        {"dsl", rdpa_wan_dsl},
#ifdef XRDP
        {"xgpon", rdpa_wan_xgpon},
        {"xepon", rdpa_wan_xepon},
#endif
        {NULL, 0}
    }
};

const bdmf_attr_enum_table_t rdpa_speed_type_enum_table =
{
    .type_name = "rdpa_speed_type", .help = "Active Ethernet Speed",
    .values = {
        {"not set", rdpa_speed_none},
        {"100m", rdpa_speed_100m},
        {"1g", rdpa_speed_1g},
        {"2.5g", rdpa_speed_2_5g},
        {"5g", rdpa_speed_5g},
        {"10g", rdpa_speed_10g},
        {NULL, 0}
    }
};

const bdmf_attr_enum_table_t rdpa_dos_attack_reason_enum_table =
{
    .type_name = "rdpa_dos_attack_reason", .help = "Dos attack reason",
    .values = {
        {"mac_sa_eq_da", rdpa_dos_reason_mac_sa_eq_da},       /* MAC SA==DA */
        {"ip_land", rdpa_dos_reason_ip_land},                 /* IPDA=IPSA in an IP(v4/v6) datagram */ 
        {"tcp_blat", rdpa_dos_reason_tcp_blat},               /* DPort=SPort in a TCP header */
        {"udp_blat", rdpa_dos_reason_udp_blat},               /* DPort=SPort in a UDP header */
        {"tcp_null_scan", rdpa_dos_reason_tcp_null_scan},     /* Seq_Num=0 & All TCP_FLAGs=0 */
        {"tcp_xmas_scan", rdpa_dos_reason_tcp_xmas_scan},     /* Seq_Num=0 & FIN=1 & URG=1 & PSH=1 */
        {"tcp_synfin_scan", rdpa_dos_reason_tcp_synfin_scan}, /* SYN=1 & FIN=1 */
        {"tcp_syn_error", rdpa_dos_reason_tcp_syn_error},     /* SYN=1 & ACK=0 & SRC_Port<1024 */
        {"tcp_short_hdr", rdpa_dos_reason_tcp_short_hdr},     /* Length of TCP header < MIN_TCP_Header_Size. */
        {"tcp_frag_error", rdpa_dos_reason_tcp_frag_error},   /* Fragment_Offset=1 in any fragment of a fragmented IP datagram carring part of TCP data */
        {"icmpv4_fragment", rdpa_dos_reason_icmpv4_fragment}, /* ICMPv4 protocol data unit carrier in a fragmented IPv4 datagram */
        {"icmpv6_fragment", rdpa_dos_reason_icmpv6_fragment}, /* ICMPv6 protocol data unit carrier in a fragmented IPv6 datagram */
        {"icmpv4_long_ping", rdpa_dos_reason_icmpv4_long_ping}, /* ICMPv4 Ping(Echo Request) > MAX_ICMPv4_Size + size of IPv4 header */
        {"icmpv6_long_ping", rdpa_dos_reason_icmpv6_long_ping}, /* ICMPv6 Ping(Echo Request) > MAX_ICMPv4_Size + size of IPv6 header */
        {NULL, 0}
    }
};

const bdmf_attr_enum_table_t rdpa_forward_action_enum_table =
{
    .type_name = "rdpa_forward_action", .help = "Forwarding action",
    .values = {
        {"acl", rdpa_forward_action_none},    /* used only for acl flows */
        {"forward", rdpa_forward_action_forward},
        {"drop", rdpa_forward_action_drop},
        {"host", rdpa_forward_action_host},
        {"flood", rdpa_forward_action_flood}, /* used only for bridge DAL */
        {"skip", rdpa_forward_action_skip},   /* used for generic filter for increment counter action only \XRDP_LIMITED */
#ifdef XRDP
        {"drop_low_pri", rdpa_forward_action_drop_low_pri},   /* drop only if ingress filters pass \XRDP_LIMITED */
#endif        
        {NULL, 0}
    }
};

const bdmf_attr_enum_table_t rdpa_filter_action_enum_table =
{
    .type_name = "rdpa_filter_action", .help = "Filter action",
    .values = {
        {"allow", rdpa_filter_action_allow},
        {"deny", rdpa_filter_action_deny},
        {NULL, 0}
    }
};

const bdmf_attr_enum_table_t rdpa_traffic_dir_enum_table =
{
    .type_name = "rdpa_traffic_dir", .help = "Traffic direction",
    .values = {
        {"ds", rdpa_dir_ds},        /**< Downstream */
        {"us", rdpa_dir_us},        /**< Upstream */
        {NULL, 0}
    }
};

const bdmf_attr_enum_table_t rdpa_port_frame_allow_enum_table =
{
    .type_name = "rdpa_port_frame_allow", .help = "Eligible frame type on port",
    .values = {
        {"any", rdpa_port_allow_any},           /**< Allow tagged and untagged frames */
        {"tagged", rdpa_port_allow_tagged},     /**< Allow tagged frames only */
        {"untagged", rdpa_port_allow_untagged}, /**< Allow untagged frames only */
        {NULL, 0}
    }
};

/* qos_methods type enum values */
const bdmf_attr_enum_table_t rdpa_qos_method_enum_table =
{
    .type_name = "rdpa_qos_method", .help = "QoS mapping method",
    .values = {
        {"pbit", rdpa_qos_method_pbit},
        {"flow", rdpa_qos_method_flow},
        {NULL, 0}
    }
};

/* forw_mode type enum values */
const bdmf_attr_enum_table_t rdpa_forward_mode_enum_table =
{
    .type_name = "rdpa_forwarding_mode", .help = "Forwarding mode",
    .values = {
        {"packet", rdpa_forwarding_mode_pkt},
        {"flow", rdpa_forwarding_mode_flow},
        {NULL, 0}
    }
};

/* class_mode type enum values */
const bdmf_attr_enum_table_t rdpa_classify_mode_enum_table =
{
    .type_name = "rdpa_classify_mode", .help = "Classification mode",
    .values = {
        {"packet", rdpa_classify_mode_pkt},
        {"flow", rdpa_classify_mode_flow},
        {NULL, 0}
    }
};

/* discard_prty type enum values */
const bdmf_attr_enum_table_t rdpa_disc_prty_enum_table =
{
    .type_name = "rdpa_discard_prty", .help = "Discard priority",
    .values = {
        {"low", rdpa_discard_prty_low},
        {"high", rdpa_discard_prty_high},
        {NULL, 0}
    }
};

/* Ingress Qos priority type enum values */
const bdmf_attr_enum_table_t rdpa_iq_priority_enum_table =
{
    .type_name = "rdpa_iq_priority", .help = "Ingress Qos priority",
    .values = {
        {"low", rdpa_iq_priority_low},
        {"high", rdpa_iq_priority_high},
        {NULL, 0}
    }
};

/* dest enum values */
const bdmf_attr_enum_table_t rdpa_flow_dest_enum_table =
{
    .type_name = "rdpa_flow_destination", .help = "Flow destination",
    .values = {
        {"not set", rdpa_flow_dest_none},
        {"iptv", rdpa_flow_dest_iptv},
        {"eth",  rdpa_flow_dest_eth},
        {"omci",  rdpa_flow_dest_omci},
        {NULL, 0}
    }
};

/* operation mode enum values */
const bdmf_attr_enum_table_t rdpa_operation_mode_enum_table =
{
    .type_name = "rdpa_operation_mode", .help = "System operation mode (FC/PRV)",
    .values = {
        {"prv", rdpa_method_prv},
        {"fc", rdpa_method_fc},
        {NULL, 0}
    }
};

/* ip version enum values */
const bdmf_attr_enum_table_t rdpa_ip_version_enum_table =
{
    .type_name = "ip_version", .help = "IP protocol version",
    .values = {
        {"other", RDPA_IC_L3_PROTOCOL_OTHER},
        {"ipv4", RDPA_IC_L3_PROTOCOL_IPV4},
        {"ipv6", RDPA_IC_L3_PROTOCOL_IPV6},
        {NULL, 0}
    }
};

/* ingress class result action vector */
const bdmf_attr_enum_table_t rdpa_ic_act_vect_enum_table =
{
    .type_name = "ic_action_vector", .help = "Vector of actions, relevant for ingress class only",
    .values =
    {
    	{"service_q", rdpa_ic_act_service_q},
        {"cpu_mirroring", rdpa_ic_act_cpu_mirroring},
        {"ttl", rdpa_ic_act_ttl},
        {NULL, 0}
    }
};

/* del command vector */
const bdmf_attr_enum_table_t rdpa_ic_dei_command_enum_table =
{
    .type_name = "dei_command", .help = "Set modification of dei bits in TCI",
    .values =
    {
        {"copy", RDPA_IC_DEI_COPY},
        {"clear", RDPA_IC_DEI_CLEAR},
        {"set", RDPA_IC_DEI_SET},
        {NULL, 0}
    }
};

/* del command vector */
const bdmf_attr_enum_table_t rdpa_bpm_buffer_size_enum_table =
{
    .type_name = "bpm_buffer_size", .help = "BPM buffer size",
    .values =
    {
        {"2K", RDPA_BPM_BUFFER_2K},
        {"2.5K", RDPA_BPM_BUFFER_2_5K},
        {"4K", RDPA_BPM_BUFFER_4K},
        {"16K", RDPA_BPM_BUFFER_16K},
        {NULL, 0}
    }
};

struct bdmf_aggr_type filter_ctrl_type =
{
    .name = "filter_ctrl", .struct_name = "rdpa_filter_ctrl_t",
    .help = "Filter control",
    .size = sizeof(rdpa_filter_ctrl_t),
    .fields = (struct bdmf_attr[])
    {
        { .name = "enabled", .help = "Enabled", .type = bdmf_attr_boolean,
            .size = sizeof(bdmf_boolean),
            .offset = offsetof(rdpa_filter_ctrl_t, enabled)
        },
        { .name = "action", .help = "Action (drop/trap)", .type = bdmf_attr_enum,
            .ts.enum_table = &rdpa_forward_action_enum_table,
            .size = sizeof(rdpa_forward_action),
            .offset = offsetof(rdpa_filter_ctrl_t, action)
        },
        BDMF_ATTR_LAST
    }
};
DECLARE_BDMF_AGGREGATE_TYPE(filter_ctrl_type);

/* 'rdpa_filter': Enumeration table */
const bdmf_attr_enum_table_t rdpa_filter_enum_table =
{
    .type_name = "rdpa_filter", .help = "Filter",
    .values =
    {
        { "dhcp", RDPA_FILTER_DHCP },
        { "igmp", RDPA_FILTER_IGMP },
        { "mld", RDPA_FILTER_MLD },
        { "icmpv6", RDPA_FILTER_ICMPV6 },
        { "etype_udef_0", RDPA_FILTER_ETYPE_UDEF_0 },
        { "etype_udef_1", RDPA_FILTER_ETYPE_UDEF_1 },
        { "etype_udef_2", RDPA_FILTER_ETYPE_UDEF_2 },
        { "etype_udef_3", RDPA_FILTER_ETYPE_UDEF_3 },
        { "etype_pppoe_d", RDPA_FILTER_ETYPE_PPPOE_D },
        { "etype_pppoe_s", RDPA_FILTER_ETYPE_PPPOE_S },
        { "etype_arp", RDPA_FILTER_ETYPE_ARP },
        { "etype_802_1x", RDPA_FILTER_ETYPE_802_1X },
        { "etype_802_1ag_cfm", RDPA_FILTER_ETYPE_802_1AG_CFM },
        { "etype_ptp_1588", RDPA_FILTER_ETYPE_PTP_1588 },
        { "l4_ptp_1588", RDPA_FILTER_L4_PTP_1588 },
        { "mcast_ipv4", RDPA_FILTER_MCAST_IPV4 },
        { "mcast_ipv6", RDPA_FILTER_MCAST_IPV6 },
        { "mcast_l2", RDPA_FILTER_MCAST_L2 },
        { "mcast", RDPA_FILTER_MCAST },
        { "bcast", RDPA_FILTER_BCAST },
        { "oui", RDPA_FILTER_MAC_ADDR_OUI },
        { "hdr_err", RDPA_FILTER_HDR_ERR },
        { "ip_frag", RDPA_FILTER_IP_FRAG },
        { "tpid", RDPA_FILTER_TPID },
        { "mac_spoofing", RDPA_FILTER_MAC_SPOOFING },
        { "ip_mcast_control", RDPA_FILTER_IP_MCAST_CONTROL },
        { "l2cp", RDPA_FILTER_L2CP },
        { NULL, 0 }
    }
};

const bdmf_attr_enum_table_t rdpa_protocol_filters_table =
{
    .type_name = "protocol_filters", .help = "Protocol Filters",
    .values = {
        {"ipv4", rdpa_proto_filter_ipv4},
        {"ipv6", rdpa_proto_filter_ipv6},
        {"pppoe", rdpa_proto_filter_pppoe},
        {"non_ip", rdpa_proto_filter_non_ip},
        {"any", rdpa_proto_filter_any},
        {NULL, 0}
    }
};

/* rdpa_tc type enum values */
const bdmf_attr_enum_table_t rdpa_tc_enum_table =
{
    .type_name = "rdpa_cpu_tc", .help = "CPU Traffic classes",
    .values = {
        {"TC0", rdpa_cpu_tc0},
        {"TC1", rdpa_cpu_tc1},
        {"TC2", rdpa_cpu_tc2},
        {"TC3", rdpa_cpu_tc3},
        {"TC4", rdpa_cpu_tc4},
        {"TC5", rdpa_cpu_tc5},
        {"TC6", rdpa_cpu_tc6},
        {"TC7", rdpa_cpu_tc7},
        {NULL, 0}
    }
};

/* mcast prefix filter method */
const bdmf_attr_enum_table_t rdpa_iptv_mcast_prefix_filter_enum_table =
{
    .type_name = "rdpa_mcast_filter_method", .help = "Multicast prefix filter method",
    .values = {
        {"none", rdpa_mcast_filter_method_none},
        {"mac", rdpa_mcast_filter_method_mac},
        {"ip", rdpa_mcast_filter_method_ip},
        {"mac_and_ip", rdpa_mcast_filter_method_mac_and_ip},
        {NULL, 0}
    }
};

/* mcast lookup method enum values */
const bdmf_attr_enum_table_t rdpa_iptv_lookup_method_enum_table =
{
    .type_name = "rdpa_iptv_lookup_method", .help = "IPTV lookup method",
    .values = {
        {"mac", iptv_lookup_method_mac},
        {"mac_vid", iptv_lookup_method_mac_vid},
        {"group_ip", iptv_lookup_method_group_ip},
        {"group_ip_src_ip", iptv_lookup_method_group_ip_src_ip},
        {"group_ip_src_ip_vid", iptv_lookup_method_group_ip_src_ip_vid},
        {NULL, 0}
    }
};

#if defined(BCM_PON_XRDP) && !defined(RDP_UFC)
/* same custom user reasons can't be use by IP_CLASS/INGRESS_CLASS in parallel */
static bdmf_attr_enum_table_t rdpa_ip_flow_trap_reason_enum_table =
{
    .type_name = "flow_trap_reason",
    .values =
    {
        {"no_trap", 0},
        {"conn_trap0", rdpa_cpu_rx_reason_udef_0},
        {"conn_trap1", rdpa_cpu_rx_reason_udef_1},
        {"conn_trap2", rdpa_cpu_rx_reason_udef_2},
        {"conn_trap3", rdpa_cpu_rx_reason_udef_3},
        {"conn_trap4", rdpa_cpu_rx_reason_udef_4},
        {"conn_trap5", rdpa_cpu_rx_reason_udef_5},
        {"conn_trap6", rdpa_cpu_rx_reason_udef_6},
        {"conn_trap7", rdpa_cpu_rx_reason_udef_7},
        {NULL, 0}
    }
};

/* Vlan offset enum values */
static bdmf_attr_enum_table_t rdpa_vlan_offset_enum_table =
{
    .type_name = "vlan_offset",
    .values =
    {
        {"offset_12", rdpa_vlan_offset_12},
        {"offset_16", rdpa_vlan_offset_16},
        {NULL, 0}
    }
};

/* PBIT remark action enum values */
static bdmf_attr_enum_table_t rdpa_pbit_action_enum_table =
{
    .type_name = "pbit_remark_action",
    .values =
    {
        {"dscp_copy", rdpa_pbit_act_dscp_copy},
        {"outer_copy", rdpa_pbit_act_outer_copy},
        {"inner_copy", rdpa_pbit_act_inner_copy},
        {NULL, 0}
    }
};

const bdmf_attr_enum_table_t rdpa_fc_act_vect_enum_table =
{
    .type_name = "fc_action_vector", .help = "Vector of actions, relevant for Flow Cache only",
    .values =
    {
        {"no_fwd", rdpa_fc_act_no_forward},
        {"ttl", rdpa_fc_act_ttl},
        {"dscp", rdpa_fc_act_dscp_remark},
        {"nat", rdpa_fc_act_nat},
        {"gre", rdpa_fc_act_gre_remark},
        {"opbit", rdpa_fc_act_opbit_remark},
        {"ipbit", rdpa_fc_act_ipbit_remark},
        {"dslite_tunnel", rdpa_fc_act_dslite_tunnel},
        {"gre_tunnel", rdpa_fc_act_gre_tunnel},
        {"pppoe", rdpa_fc_act_pppoe},
        {"service_q", rdpa_fc_act_service_q},
        {"llc_snap_set_len", rdpa_fc_act_llc_snap_set_len},
        {"spdsvc", rdpa_fc_act_spdsvc},
        {"pppoe_passthrough", rdpa_fc_act_pppoe_passthrough},
        {"mapt", rdpa_fc_act_mapt},
        {"vxlan_tunnel", rdpa_fc_act_vxlan_tunnel},
        {"spdt_gen", rdpa_fc_act_spdt_gen},
        {"skip_l2_hdr_copy", rdpa_fc_act_skip_l2_hdr_copy},
        {"skip_hdr_copy", rdpa_fc_act_skip_hdr_copy},
        {NULL, 0}
    }
};

struct bdmf_aggr_type mapt_cfg_type =
{
    .name = "mapt_cfg_t", .struct_name = "rdpa_mapt_t",
    .help = "MAP-T IPv6/4 translated header configuration",
    .size = sizeof(rdpa_mapt_t),
    .fields = (struct bdmf_attr[])
    {
        { .name = "tos_tc", .help = "Traffic Class(IPv6) / ToS(IPv4)", .size = sizeof(uint8_t),
            .type = bdmf_attr_number, .offset = offsetof(rdpa_mapt_t, tos_tc),
            .flags = BDMF_ATTR_UNSIGNED
        },
        { .name = "proto", .help = "Ip protocol", .size = sizeof(uint8_t),
            .type = bdmf_attr_number, .offset = offsetof(rdpa_mapt_t, proto),
            .flags = BDMF_ATTR_UNSIGNED
        },
        { .name = "src_ip", .help = "Ip source address", .size = sizeof(bdmf_ip_t),
            .type = bdmf_attr_ip_addr, .offset = offsetof(rdpa_mapt_t, src_ip),
        },
        { .name = "dst_ip", .help = "Ip destination address", .size = sizeof(bdmf_ip_t),
            .type = bdmf_attr_ip_addr, .offset = offsetof(rdpa_mapt_t, dst_ip),
        },
        { .name = "src_port", .help = "L4 source port", .size = sizeof(uint16_t),
            .type = bdmf_attr_number, .offset = offsetof(rdpa_mapt_t, src_port),
            .flags = BDMF_ATTR_UNSIGNED
        },
        { .name = "dst_port", .help = "L4 destination port", .size = sizeof(uint16_t),
            .type = bdmf_attr_number, .offset = offsetof(rdpa_mapt_t, dst_port),
            .flags = BDMF_ATTR_UNSIGNED
        },
        { .name = "l4csum", .help = "L4 checksum", .size = sizeof(uint16_t),
            .type = bdmf_attr_number, .offset = offsetof(rdpa_mapt_t, l4csum),
            .flags = BDMF_ATTR_UNSIGNED
        },
        { .name = "l3csum", .help = "L3 checksum", .size = sizeof(uint16_t),
            .type = bdmf_attr_number, .offset = offsetof(rdpa_mapt_t, l3csum),
            .flags = BDMF_ATTR_UNSIGNED
        },

        BDMF_ATTR_LAST
    },
};
DECLARE_BDMF_AGGREGATE_TYPE(mapt_cfg_type);

/* ip_flow_result aggregate type */
/* Although named ip_flow_result, the type is used both by ip_class and l2_class flows */
struct bdmf_aggr_type ip_flow_result_type =
{
    .name = "ip_flow_result", .struct_name = "rdpa_ip_flow_result_t",
    .help = "IP Flow Result",
    .size = sizeof(rdpa_ip_flow_result_t),
    .fields = (struct bdmf_attr[])
    {
        { .name = "qos_method", .help = "QoS classification method",
            .type = bdmf_attr_enum, .ts.enum_table = &rdpa_qos_method_enum_table,
            .size = sizeof(rdpa_qos_method), .offset = offsetof(rdpa_ip_flow_result_t , qos_method)
        },
        { .name = "action", .help = "Forwarding action",
            .type = bdmf_attr_enum, .ts.enum_table = &rdpa_forward_action_enum_table,
            .size = sizeof(rdpa_forward_action), .offset = offsetof(rdpa_ip_flow_result_t, action)
        },
        { .name = "trap_reason", .help = "Trap reason",
            .type = bdmf_attr_enum, .ts.enum_table = &rdpa_ip_flow_trap_reason_enum_table,
            .size = sizeof(rdpa_cpu_reason), .offset = offsetof(rdpa_ip_flow_result_t, trap_reason),
        },
        { .name = "dscp_value",
            .help = "DSCP value for IPv4, DSCP or TC for IPv6 (if ECN remarking is enabled globally through system options)",
            .size = sizeof(rdpa_dscp),
            .type = bdmf_attr_number, .offset = offsetof(rdpa_ip_flow_result_t, dscp_value),
            .flags = BDMF_ATTR_UNSIGNED
        },
        { .name = "nat_port", .help = "NAT port", .size = sizeof(uint16_t),
            .type = bdmf_attr_number, .offset = offsetof(rdpa_ip_flow_result_t, nat_port),
            .flags = BDMF_ATTR_UNSIGNED
        },
        { .name = "nat_ip", .help = "NAT IPv4/IPv6 address", .size = sizeof(bdmf_ip_t),
            .type = bdmf_attr_ip_addr, .offset = offsetof(rdpa_ip_flow_result_t, nat_ip)
        },
        { .name = "policer", .help = "Policer ID", .size = sizeof(bdmf_object_handle), .ts.ref_type_name = "policer",
            .type = bdmf_attr_object, .offset = offsetof(rdpa_ip_flow_result_t, policer_obj)
        },
        { .name = "port", .help = "Egress port",
            .type = bdmf_attr_enum, .ts.enum_table = &rdpa_if_enum_table,
            .size = sizeof(rdpa_if), .offset = offsetof(rdpa_ip_flow_result_t, port),
        },
        { .name = "ssid", .help = "If port is CPU vport for wlan",
            .size = sizeof(uint8_t),
            .type = bdmf_attr_number, .offset = offsetof(rdpa_ip_flow_result_t, ssid),
            .flags = BDMF_ATTR_UNSIGNED, .min_val = 0, .max_val = 15
        },
        { .name = "phys_port", .help = "physical port",
            .type = bdmf_attr_enum, .ts.enum_table = &rdpa_emac_enum_table,
            .size = sizeof(rdpa_emac), .offset = offsetof(rdpa_ip_flow_result_t, phy_port),
        },
        { .name = "queue_id", .help = "Egress queue id", .size = sizeof(uint32_t),
            .type = bdmf_attr_number, .offset = offsetof(rdpa_ip_flow_result_t, queue_id),
            .flags = BDMF_ATTR_UNSIGNED
        },
        { .name = "wan_flow", .help = "US gem flow or DSL status", .size = sizeof(uint8_t),
            .type = bdmf_attr_number, .offset = offsetof(rdpa_ip_flow_result_t, wan_flow),
            .flags = BDMF_ATTR_UNSIGNED
        },
        { .name = "ovid_offset", .help = "Outer VID offset",
            .type = bdmf_attr_enum, .ts.enum_table = &rdpa_vlan_offset_enum_table,
            .size = sizeof(rdpa_vlan_offset), .offset = offsetof(rdpa_ip_flow_result_t, ovid_offset),
        },
        { .name = "opbit_action", .help = "Packet based outer pbit remarking action",
            .type = bdmf_attr_enum, .ts.enum_table = &rdpa_pbit_action_enum_table,
            .size = sizeof(rdpa_pbit_remark_action), .offset = offsetof(rdpa_ip_flow_result_t, opbit_action),
        },
        { .name = "ipbit_action", .help = "Packet based inner pbit remarking action",
            .type = bdmf_attr_enum, .ts.enum_table = &rdpa_pbit_action_enum_table,
            .size = sizeof(rdpa_pbit_remark_action), .offset = offsetof(rdpa_ip_flow_result_t, ipbit_action),
        },
        { .name = "l2_offset", .help = "Offset of L2 header", .size = sizeof(int8_t),
            .type = bdmf_attr_number, .offset = offsetof(rdpa_ip_flow_result_t, l2_header_offset)
        },
        { .name = "l2_head_size", .help = "Size of L2 header in bytes", .size = sizeof(uint8_t),
            .type = bdmf_attr_number, .offset = offsetof(rdpa_ip_flow_result_t, l2_header_size),
            .flags = BDMF_ATTR_UNSIGNED
        },
        { .name = "l2_num_tags", .help = "L2 header number of tags", .size = sizeof(uint8_t),
            .type = bdmf_attr_number, .offset = offsetof(rdpa_ip_flow_result_t, l2_header_number_of_tags),
        },
        { .name = "action_vec", .help = "Vector of actions to perfrom on the flow",
            .size = sizeof(rdpa_fc_action_vec_t),
            .type = bdmf_attr_enum_mask, .ts.enum_table = &rdpa_fc_act_vect_enum_table,
            .offset = offsetof(rdpa_ip_flow_result_t, action_vec),
            .flags = BDMF_ATTR_UNSIGNED
        },
        { .name = "l2_header", .help = "L2 header in egress", .size = RDPA_L2_HEADER_SIZE,
            .type = bdmf_attr_buffer, .offset = offsetof(rdpa_ip_flow_result_t, l2_header)
        },
        { .name = "wl_metadata", .help = "WL Metadata (in use for some WiFi acceleration techniques)",
            .size = sizeof(uint32_t), .type = bdmf_attr_number, .flags = BDMF_ATTR_HEX_FORMAT,
            .offset = offsetof(rdpa_ip_flow_result_t, wl_metadata),
            .val_to_s = FLOW_RESULT_VAL_TO_S
        },
        { .name = "service_queue_id", .help = "service queue id", .size = sizeof(bdmf_index),
            .type = bdmf_attr_number, .offset = offsetof(rdpa_ip_flow_result_t, service_q_id),
            .flags = BDMF_ATTR_HAS_DISABLE, .disable_val = BDMF_INDEX_UNASSIGNED,
        },
        { .name = "drop_eligibility", .help = "Drop eligibility indicator[1:0] 00/01= disable 10=non drop eligible(WRED high priority), 11=drop eligible(WRED low priority)",
            .size = sizeof(uint8_t), .type = bdmf_attr_number, .offset = offsetof(rdpa_ip_flow_result_t, drop_eligibility),
            .flags = BDMF_ATTR_UNSIGNED
        },
        { .name = "tunnel", .help = "Tunnel ID", .size = sizeof(bdmf_object_handle), .ts.ref_type_name = "tunnel",
            .type = bdmf_attr_object, .offset = offsetof(rdpa_ip_flow_result_t, tunnel_obj),
        },
        { .name = "pathstat_idx", .help = "Path based Stat table index", .size = sizeof(uint32_t),
            .type = bdmf_attr_number, .offset = offsetof(rdpa_ip_flow_result_t, pathstat_idx),
            .flags = BDMF_ATTR_UNSIGNED
        },
        { .name = "max_pkt_len", .help = "Max packet lenth", .size = sizeof(uint16_t),
            .type = bdmf_attr_number, .offset = offsetof(rdpa_ip_flow_result_t, max_pkt_len),
            .flags = BDMF_ATTR_UNSIGNED
        },
        { .name = "is_tcpspdtest", .help = "Is Tcp Speed Test Flow", .size = sizeof(uint8_t),
            .type = bdmf_attr_number, .offset = offsetof(rdpa_ip_flow_result_t, is_tcpspdtest),
            .flags = BDMF_ATTR_UNSIGNED
        },
        { .name = "tcpspdtest_stream_id", .help = "Tcp Speed Test Stream Id", .size = sizeof(uint8_t),
            .type = bdmf_attr_number, .offset = offsetof(rdpa_ip_flow_result_t, tcpspdtest_stream_id),
            .flags = BDMF_ATTR_UNSIGNED
        },
        { .name = "tcpspdtest_is_upload", .help = "Tcp Speed Test action Download/Upload", .size = sizeof(bdmf_boolean),
            .type = bdmf_attr_boolean, .offset = offsetof(rdpa_ip_flow_result_t, tcpspdtest_is_upload),
            .flags = BDMF_ATTR_UNSIGNED
        },
        { .name = "is_df", .help = "DF (Don't Fragment Flag) in IPv4 header", .size = sizeof(uint8_t),
            .type = bdmf_attr_number, .offset = offsetof(rdpa_ip_flow_result_t, is_df),
        },
        { .name = "mapt_cfg", .help = "MAP-T IPv6/4 translated header", .type = bdmf_attr_aggregate,
            .ts.aggr_type_name = "mapt_cfg_t", .offset = offsetof(rdpa_ip_flow_result_t, mapt_cfg),
        },
        { .name = "clients_vector", .help = "Multicast clients vector", 
            .type = bdmf_attr_buffer, .offset = offsetof(rdpa_ip_flow_result_t, clients_vector),
            .size = (RDD_MASTER_MC_FLOW_CACHE_CONTEXT_ENTRY_CLIENT_IDX_VECTOR_NUMBER * sizeof(uint32_t)),
            .flags = BDMF_ATTR_HEX_FORMAT
        },

        BDMF_ATTR_LAST
    },
};
DECLARE_BDMF_AGGREGATE_TYPE(ip_flow_result_type);
#endif

