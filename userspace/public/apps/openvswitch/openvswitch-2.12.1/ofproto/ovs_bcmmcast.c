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

#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/icmp6.h>
#include "bcm_mcast.h"
#include "bcm_mcast_api_public.h"
#include "vswitch-idl.h"
#include "ovs_bcmmcast.h"
#include "poll-loop.h"
#include "openvswitch/vlog.h"

VLOG_DEFINE_THIS_MODULE(ovs_bcmmcast);

/* Data structure to store OVS<->MCPD related info */
typedef struct ovs_mcpd_info_struct{
    int  sock_conn;
    int  sock;
    char *sock_buff;
    t_ovs_mcpd_msg brcfg;
}t_ovs_mcpd_info;

t_ovs_mcpd_info g_ovs_mcpd_info;

bridge_manage_mcastsnoopentry_hook_t bridge_manage_mcastsnoopentry_hook = NULL;

/* Send message to MCPD */
int ovs_mcpd_send_msg(void *pBuf, int buf_size)
{
    return bcm_mcast_api_send_sock_msg(OVS_2_MCPD_SOCK_PORT, pBuf, buf_size);
}

/* Initialize the socket used for MCPD to OVS communtication */
int ovs_mcpd_socket_init(void)
{
    struct sockaddr_in sa;
    int             sd;
    int             flags;
    int             optval = 1;
    socklen_t       optlen = sizeof(optval);
  
    g_ovs_mcpd_info.sock_conn = -1;
    g_ovs_mcpd_info.sock = -1;

    g_ovs_mcpd_info.sock_buff = (char *)malloc(OVS_MCPD_RX_BUF_SIZE);
    if ( NULL == g_ovs_mcpd_info.sock_buff )
    {
       VLOG_ERR("%s mcpd socket buffer allocation failed\n", __func__);
    }

    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        VLOG_ERR("%s socket() error, %s", __func__, strerror(errno));
        return -1;
    }

    /* Allow reusing the socket immediately when application is restarted */
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &optval, optlen))
    {
        VLOG_ERR("%s setsockopt error %s", __func__, strerror(errno));
    }

    sa.sin_family      = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    sa.sin_port        = htons( (unsigned short)MCPD_2_OVS_SOCK_PORT);
    if((bind(sd, (struct sockaddr *)&sa, sizeof(sa))) == -1)
    {
        VLOG_ERR("%s bind() to port %d error, %s", __func__, 
                   MCPD_2_OVS_SOCK_PORT, strerror(errno));
        close(sd);
        return -1;
    }

    /* Support a maximum of 3 connections. MCPD is the only client
       today. Do not see a scenario that would require more than 1
       connection from MCPD but following MCPCTL model to support 3
       connections */
    if ((listen(sd, 3)) == -1)
    {
        VLOG_ERR("%s listen() to port %d error, %s", __func__, 
                   MCPD_2_OVS_SOCK_PORT, strerror(errno));
        close(sd);
        return -1;
    }

    flags = fcntl(sd, F_GETFL, 0);
    if(flags < 0)
    {
        VLOG_ERR("%s cannot retrieve socket flags. error=%s", 
                   __func__, strerror(errno));
    }
    if ( fcntl(sd, F_SETFL, flags | O_NONBLOCK) < 0 )
    {
        VLOG_ERR("%s cannot set socket to non-blocking. error=%s", 
                   __func__, strerror(errno));
    }

    g_ovs_mcpd_info.sock = sd;

    VLOG_INFO("%s successful\n", __func__);
    return(0);
}

/* Initialize objects */
int ovs_mcpd_init(void)
{
    ovs_mcpd_socket_init();

    g_ovs_mcpd_info.brcfg.msgtype = OVS_MCPD_MSG_BRIDGE_CONFIG_UPDATE;

    return 0;
}

/* Accept incoming connection request from MCPD */
int ovs_mcpd_socket_accept(void)
{
    struct sockaddr clientAddr;
    unsigned int sockAddrSize;
    int sd;
    int flags;

    if ( g_ovs_mcpd_info.sock_conn != -1 )
    {
        VLOG_ERR("%s Only one connection available\n", __func__);
        return -1;
    }

    sockAddrSize = sizeof(clientAddr);
    if ((sd = accept(g_ovs_mcpd_info.sock, (struct sockaddr *)&clientAddr, &sockAddrSize)) < 0) {
        if (errno != EAGAIN) {
            VLOG_ERR("%s accept connection failed. errno=%d %s\n", __func__, errno, strerror(errno));
        }
        return -1;
    }
  
    flags = fcntl(sd, F_GETFL, 0);
    if(flags < 0) {
        VLOG_ERR("%s cannot retrieve socket flags. errno=%d\n", __func__, errno);
    }
    if ( fcntl(sd, F_SETFL, flags | O_NONBLOCK) < 0 ) {
        VLOG_ERR("%s cannot set socket to non-blocking. errno=%d\n", __func__, errno);
    }

    g_ovs_mcpd_info.sock_conn = sd;

    return 0;
}

/* Receive incoming messages from MCPD and process them */
int ovs_mcpd_socket_receive(void)
{
    int length;
    int ret = 0;
  
    while (1) 
    {
        length = recv(g_ovs_mcpd_info.sock_conn, &g_ovs_mcpd_info.sock_buff[0],
                      OVS_MCPD_RX_BUF_SIZE, 0);
        if (length < 0) {
           if (errno != EAGAIN) {
               VLOG_ERR("%s recvfrom() error %d: %s\n", __func__, errno, strerror(errno));
               ret = -1;
           }
           break;
        }
        else if (length == 0) {
            /* socket has been closed */
            if (errno != EAGAIN) {
                VLOG_ERR("%s socket closed error %d: %s\n", __func__, errno, strerror(errno));
                ret = -1;
            }
            break;
        }
        else {
            t_ovs_mcpd_msg *pMsg;

            pMsg = (t_ovs_mcpd_msg *)&g_ovs_mcpd_info.sock_buff[0];
            switch (pMsg->msgtype)
            {
            case MCPD_OVS_MSG_MANAGE_SNOOP_ENTRY:
                    VLOG_DBG("%s MCPD_OVS_MSG_MANAGE_SNOOP_ENTRY addentry %d\n", 
                             __func__, pMsg->snoopentry.addentry);
                    if (bridge_manage_mcastsnoopentry_hook)
                        bridge_manage_mcastsnoopentry_hook(pMsg->snoopentry.brname,
                                                           pMsg->snoopentry.ipv4grp.s_addr,
                                                           &pMsg->snoopentry.ipv6grp,
                                                           pMsg->snoopentry.vlan,
                                                           pMsg->snoopentry.portname,
                                                           pMsg->snoopentry.is_igmp,
                                                           pMsg->snoopentry.ipv4src.s_addr,
                                                           &pMsg->snoopentry.ipv6src,
                                                           pMsg->snoopentry.addentry);
                    break;
                default:
                    VLOG_ERR("%s Unknown pMsg->msgtype %d\n", __func__, pMsg->msgtype);
                    break;
            }
        }
    }
    close(g_ovs_mcpd_info.sock_conn);
    g_ovs_mcpd_info.sock_conn = -1;
    return ret;
}

/* Add MCPD->OvS communication socket in the vswitchd
   socket poll list */
void ovs_mcpd_wait(void)
{
    poll_fd_wait(g_ovs_mcpd_info.sock, POLLIN);
}

/* Function called from vswitchd daemon when MCPD->OVS
   socket is ready for I/O */
int ovs_mcpd_comm_run(void)
{
    ovs_mcpd_socket_accept();

    if(g_ovs_mcpd_info.sock_conn != -1)
        ovs_mcpd_socket_receive();

    return 0;
}

/* Notify MCPD about a received IGMP/MLD packet and
   wait for a response. MCPD responds with OK or NOK.
   OK - OvS can continue processing this IGMP/MLD pkt
   NOK - OvS ignores this pkt
   */
void ovs_mcpd_notify(unsigned char *pkt,
                     int len,
                     int rxdev_ifi,
                     int to_acceldev_ifi,
                     uint32_t v4_src,
                     struct in6_addr *v6_src,
                     uint16_t vlan,
                     char *brname,
                     int l4offset,
                     int isigmp)
{
    int mcpdresp = MCPD_OVS_RESP_NOK;
    int     buf_size;
    t_ovs_mcpd_msg *pMcpdMsg;
    t_BCM_MCAST_PKT_INFO *pInfo;
    int igmp_mld_len;

    buf_size = sizeof(t_ovs_mcpd_msg) + len;
    VLOG_DBG("%s len %d sizeof(t_ovs_mcpd_msg) %d buf_size %d l4offset %d\n", 
              __func__, len, sizeof(t_ovs_mcpd_msg), buf_size, l4offset);
    pMcpdMsg = (t_ovs_mcpd_msg *)malloc(buf_size);
    if (pMcpdMsg == NULL) {
        VLOG_ERR("%s McpdMsg allocation failure\n", __func__);
        return;
    }
    pInfo = &(pMcpdMsg->pktInfo);

#define ETH_ALEN 6
    memcpy(pInfo->repMac, &pkt[ETH_ALEN], ETH_ALEN);
    pInfo->rxdev_ifi = rxdev_ifi;
    pInfo->to_acceldev_ifi = to_acceldev_ifi;

    if (isigmp) {
        pMcpdMsg->msgtype = OVS_MCPD_MSG_IGMP;
        pInfo->ipv4rep.s_addr = v4_src;
    }
    else {
        pMcpdMsg->msgtype = OVS_MCPD_MSG_MLD;
        memcpy(&pInfo->ipv6rep, v6_src, sizeof(*v6_src));
    }

    VLOG_DBG("%s src address is 0x%x vlan %d\n", __func__, pInfo->ipv4rep.s_addr, vlan);
    pInfo->lanppp = 0;
    if (vlan) {
        pInfo->tci = vlan;
    } else {
        pInfo->tci = 0;
    }
    pInfo->packetIndex = -1;
    igmp_mld_len = len - l4offset;
    buf_size -= l4offset;
    pInfo->data_len = igmp_mld_len;
    memcpy(&pInfo->pkt[0], (unsigned char *)(pkt + l4offset), igmp_mld_len);

    if (bcm_mcast_api_ifname_to_idx(brname, &(pInfo->parent_ifi)) == 0) 
    {
       if (ovs_mcpd_send_msg(pMcpdMsg, buf_size) == -1) 
       {
           VLOG_ERR("%s ovs_mcpd_send_msg_wait_rsp error pInfo->pkt[0](igmptype) 0x%x buf_size %d igmp_mld_len %d\n", 
                      __func__, pInfo->pkt[0], buf_size, igmp_mld_len);
       }
    } 
    else 
    {
        VLOG_ERR("%s bcm_mcast_api_ifname_to_idx() returned error brname %s\n",
                __func__, brname);
    }
    free(pMcpdMsg);

    VLOG_DBG("%s len %d mcpdresp %d l4offset %d igmp_mld_len %d buf_size %d sizeof(*pInfo) %d\n",
              __func__, len, mcpdresp, l4offset, igmp_mld_len, buf_size, sizeof(*pInfo));
    return;
}

/* Function called from the bridge on a bridge destroy.
   Notify MCPD with the updated OvS bridge/port info */
void ovs_mcpd_brcfg_brdelete(char *brname)
{
    int i;
    int j;
    int k;
    int ifIndex;
    t_ovs_mcpd_brcfg_info *pBrCfg = &(g_ovs_mcpd_info.brcfg.brcfgmsg);

    if (bcm_mcast_api_ifname_to_idx(brname, &ifIndex) == 0)
    {
        for (i = 0; i < pBrCfg->numbrs; i++) 
        {
            if (pBrCfg->ovs_br[i] == ifIndex) 
            {
                /* Found matching bridge, delete this entry
                   by adjusting the bridge and port array values */
                for (j = i; j < (pBrCfg->numbrs - 1); j++) 
                {
                    pBrCfg->ovs_br[j] = pBrCfg->ovs_br[j+1];
                    pBrCfg->numports[j] = pBrCfg->numports[j+1];

                    for (k = 0; k < pBrCfg->numports[j+1]; k++) 
                    {
                        pBrCfg->ovs_ports[j][k] = pBrCfg->ovs_ports[j+1][k];
                    }
                }

                /* Decrement the number of bridges */
                pBrCfg->numbrs--;
            }
        }
    
        if (pBrCfg->numbrs >= 0) 
        {
            if (ovs_mcpd_send_msg(&(g_ovs_mcpd_info.brcfg), 
                                  sizeof(g_ovs_mcpd_info.brcfg)) == -1)
            {
                VLOG_ERR("%s ovs_mcpd_send_msg returned Error\n", __func__);
            }
        }
        else
        {   
            VLOG_ERR("%s Num bridges(%d) negative\n", __func__, pBrCfg->numbrs);
        }
    }
}

/* Function called from the bridge on a bridge configuration
   change. Notify MCPD with the updated OvS bridge/port info */
void ovs_mcpd_brcfg_update(const struct ovsrec_open_vswitch *cfg)
{
    int i;
    int ifIndex;
    t_ovs_mcpd_brcfg_info *pBrCfg = &(g_ovs_mcpd_info.brcfg.brcfgmsg);

    pBrCfg->numbrs = 0;
    for (i = 0; i < MCPD_MAX_OVS_BRIDGES; i++) 
    {
        pBrCfg->numports[i] = 0;
    }

    if (cfg) {
        for (i = 0; i < cfg->n_bridges; i++) {
            const struct ovsrec_bridge *br_cfg = cfg->bridges[i];
            int j;

            if (bcm_mcast_api_ifname_to_idx(br_cfg->name, &ifIndex) == 0)
            {
                if (pBrCfg->numbrs < MCPD_MAX_OVS_BRIDGES) 
                {
                    pBrCfg->ovs_br[pBrCfg->numbrs] = ifIndex;
                }

                for (j = 0; j < br_cfg->n_ports; j++) {
                    struct ovsrec_port *port_cfg = br_cfg->ports[j];

                    if (bcm_mcast_api_ifname_to_idx(port_cfg->name, &ifIndex) == 0)
                    {
                        if (pBrCfg->numbrs < MCPD_MAX_OVS_BRIDGES &&
                            pBrCfg->numports[pBrCfg->numbrs] < MCPD_MAX_OVS_BRPORTS) 
                        {
                            pBrCfg->ovs_ports[pBrCfg->numbrs][pBrCfg->numports[pBrCfg->numbrs]] = ifIndex;
                        }
                        pBrCfg->numports[pBrCfg->numbrs]++;
                    }
                }
                pBrCfg->numbrs++;
            }
        }
    }
    if (pBrCfg->numbrs > 0) 
    {
        if (ovs_mcpd_send_msg(&(g_ovs_mcpd_info.brcfg), 
                              sizeof(g_ovs_mcpd_info.brcfg)) == -1)
        {
            VLOG_ERR("%s ovs_mcpd_send_msg returned Error\n", __func__);
        }
    }
}
#endif
