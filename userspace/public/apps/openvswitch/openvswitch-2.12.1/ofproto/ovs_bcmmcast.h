#if defined(CONFIG_BCM_OVS_MCAST)
/* 
 * <:label-BRCM:2019:NONE/NONE:standard
 * 
 :>  
 * Copyright 2019 Broadcom Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. 
 */

#ifndef __OVS_BCMMCAST_H__
#define __OVS_BCMMCAST_H__

#define OVS_MCPD_RX_BUF_SIZE 2048

typedef int (* bridge_manage_mcastsnoopentry_hook_t)
                                 (char *brname, ovs_be32 ip4,
                                  struct in6_addr *ip6, uint16_t vlan, 
                                  char *portname, int isip4,
                                  ovs_be32 ip4src, struct in6_addr *ip6src,
                                  int addentry);
extern bridge_manage_mcastsnoopentry_hook_t bridge_manage_mcastsnoopentry_hook;

/* Initialize objects */
int ovs_mcpd_init(void);

/* Initialize the socket used for MCPD to OVS communtication */
int ovs_mcpd_socket_init(void);

/* Accept incoming connection request from MCPD */
int ovs_mcpd_socket_accept(void);

/* Receive incoming messages from MCPD and process them */
int ovs_mcpd_socket_receive(void);

/* Function called from vswitchd daemon when MCPD->OVS
   socket is ready for I/O */
int ovs_mcpd_comm_run(void);

/* Add MCPD->OvS communication socket in the vswitchd
   socket poll list */
void ovs_mcpd_wait(void);

/* Function called from the bridge on a bridge destroy.
   Notify MCPD with the updated OvS bridge/port info */
void ovs_mcpd_brcfg_brdelete(char *brname);

/* Function called from the bridge on a bridge configuration
   change. Notify MCPD with the updated OvS bridge/port info */
void ovs_mcpd_brcfg_update(const struct ovsrec_open_vswitch *cfg);

/* Notify MCPD about a received IGMP/MLD packet and
   wait for a response. MCPD responds with OK or NOK.
   OK - OvS can continue processing this IGMP/MLD pkt
   NOK - OvS ignores this pkt
   */
void ovs_mcpd_notify(unsigned char *pkt,
                     int len,
                     int rxdev_ifi,
                     int to_acceldev_ifi,
                     uint32_t srcAddr,
                     struct in6_addr *v6_src,
                     uint16_t vlan,
                     char *brname,
                     int offset,
                     int isigmp);
#endif /* __OVS_BCMMCAST_H__ */
#endif /* defined(CONFIG_BCM_OVS_MCAST) */
