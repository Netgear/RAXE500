/*
* <:copyright-BRCM:2013:DUAL/GPL:standard
* 
*    Copyright (c) 2013 Broadcom 
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


#ifndef _RDPA_CMD_LIST_H_
#define _RDPA_CMD_LIST_H_

/* TODO: Better to add details about how these values are determined/derived */
#if defined(BCM63158)
#define RDPA_CMD_LIST_PACKET_HEADER_MAX_LENGTH   98
#define RDPA_CMD_LIST_PACKET_BUFFER_OFFSET       0
#define RDPA_CMD_LIST_PACKET_HEADER_OFFSET       62
#define RDPA_CMD_LIST_UCAST_LIST_OFFSET          20
#define RDPA_CMD_LIST_HEADROOM                   RDPA_CMD_LIST_PACKET_HEADER_OFFSET
#define RDPA_CMD_LIST_BASIC_UCAST_LIST_SIZE      104
#elif defined(BCM63146) || defined(BCM4912) || defined(BCM6855)
#define RDPA_CMD_LIST_PACKET_HEADER_MAX_LENGTH   110
#define RDPA_CMD_LIST_PACKET_BUFFER_OFFSET       0
#define RDPA_CMD_LIST_PACKET_HEADER_OFFSET       2
#if defined(RDP_UFC)
#define RDPA_CMD_LIST_UCAST_LIST_OFFSET          24
#define RDPA_CMD_LIST_BASIC_UCAST_LIST_SIZE      100
#else
#define RDPA_CMD_LIST_UCAST_LIST_OFFSET          20
#define RDPA_CMD_LIST_BASIC_UCAST_LIST_SIZE      104
#endif
#define RDPA_CMD_LIST_HEADROOM                   RDPA_CMD_LIST_PACKET_HEADER_OFFSET
#else
#define RDPA_CMD_LIST_PACKET_HEADER_MAX_LENGTH   110
#define RDPA_CMD_LIST_PACKET_BUFFER_OFFSET       0
#define RDPA_CMD_LIST_PACKET_HEADER_OFFSET       18
#define RDPA_CMD_LIST_UCAST_LIST_OFFSET          24
#define RDPA_CMD_LIST_HEADROOM                   58
#define RDPA_CMD_LIST_BASIC_UCAST_LIST_SIZE      104
#endif

#if defined(CONFIG_BCM_RDPA_CNTXT_EXT_SUPPORT)
#define RDPA_CMD_LIST_UCAST_LIST_SIZE            (2*RDPA_CMD_LIST_BASIC_UCAST_LIST_SIZE)
#else
#define RDPA_CMD_LIST_UCAST_LIST_SIZE            (RDPA_CMD_LIST_BASIC_UCAST_LIST_SIZE)
#endif
#define RDPA_CMD_LIST_UCAST_LIST_SIZE_32         (RDPA_CMD_LIST_UCAST_LIST_SIZE / 4)

#define RDPA_CMD_LIST_MCAST_L2_LIST_OFFSET       0
#define RDPA_CMD_LIST_MCAST_L2_LIST_SIZE         64
#define RDPA_CMD_LIST_MCAST_L2_LIST_SIZE_32      (RDPA_CMD_LIST_MCAST_L2_LIST_SIZE / 4)

#define RDPA_CMD_LIST_MCAST_L3_LIST_OFFSET       52
#define RDPA_CMD_LIST_MCAST_L3_LIST_SIZE         20
#define RDPA_CMD_LIST_MCAST_L3_LIST_SIZE_32      (RDPA_CMD_LIST_MCAST_L3_LIST_SIZE / 4)


typedef struct {
    uint8_t *rdd_cmd_list_p;        /* Pointer to rdd cmd list array    - Input/Output */
    uint8_t *rdpa_cmd_list_p;       /* Pointer to rdpa cmd list array   - Input/Output */
    uint32_t rdd_cmd_list_max_size; /* Max size of rdd cmd list array   - Input */
    uint32_t rdpa_cmd_list_len;     /* Rdpa cmd list len                - Input */
    uint32_t rdpa_cmd_list_data_len;/* Rdpa cmd list data len           - Input */
    uint32_t rdd_cmd_list_offset;   /* Offset of cmd list array in rdd  - Input */
    uint32_t rdd_cmd_list_len_64;   /* Cmd list len in 64bit words      - Output */
    uint32_t rdd_q_bytes_cnt;       /* q_bytes_cnt for XPE              - Output */
    uint32_t spdsvc:1;              /* SpdSvc Flag                      - Input */
    uint32_t rsvd_bits:31;          /* Reserved bits                    - Input */
} cmd_list_update_params_t;

int rdpa_cmd_list_update_context(cmd_list_update_params_t *p, int overflow);

#endif /* _RDPA_CMD_LIST_H_ */
