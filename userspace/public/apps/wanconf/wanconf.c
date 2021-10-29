/***********************************************************************
 *
 * <:copyright-BRCM:2015:DUAL/GPL:standard
 * 
 *    Copyright (c) 2015 Broadcom 
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
 *
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>	 /* needed to define getpid() */
#include <errno.h>
#include <signal.h>
#include <string.h>
//#include <fcntl.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <bcmnet.h>
#include <sys/ioctl.h>
#include "cms.h"
#include "board.h"
#include <cms_mem.h>
#include <cms_msg.h>
#include <net_port.h>
#include <dirent.h>
#include <sched.h>
#include "cms_psp.h"
#include "wanconf.h"
#include <pmd.h>
#include <bcm/bcmswapitypes.h>

static int bcm_ifname_get(int unit, int port, char *name)
{
    int skfd, err = 0;
    struct ifreq ifr;
    struct ethswctl_data ethswctl;

    if ((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("socket open error\n");
        return -1;
    }

    strcpy(ifr.ifr_name, "bcmsw");
    if ((err = ioctl(skfd, SIOCGIFINDEX, &ifr)) < 0 )
    {
        printf("bcmsw interface does not exist");
        goto exit;
    }

    ifr.ifr_data = (void *)&ethswctl;

    ethswctl.op = ETHSWGETIFNAME;
    ethswctl.type = TYPE_GET;
    ethswctl.port = port;
    ethswctl.unit = unit;

    if ((err = ioctl(skfd, SIOCETHSWCTLOPS, &ifr)) < 0 )
    {
        printf("ioctl command return error! err=%d", err);
        goto exit;
    }

    if (ethswctl.ret_val != -1) {
       strcpy(name, ethswctl.ifname);
    }

exit:
    close(skfd);
    return err;
}

static int bcm_port(int is_add, struct net_port_t *net_port)
{
    struct ifreq ifr;
    int err, skfd;
    struct ethswctl_data ethswctl;

    memset(&ethswctl, 0x0, sizeof(struct ethswctl_data));
    memcpy(&ethswctl.net_port, net_port, sizeof(net_port_t));

    if ((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("socket open error\n");
        return -1;
    }

    strcpy(ifr.ifr_name, "bcmsw");
    if ((err = ioctl(skfd, SIOCGIFINDEX, &ifr)) < 0 )
    {
        printf("bcmsw interface does not exist");
        goto exit;
    }

    /* before deleting the interface, bring it down and wait until depended interfaces are down too.
     * if the interface is up and has vlan or ppp interfaces on top of it, the operation might get stuck
     * until the base interface is freed */
    if (!is_add)
    {
        int ret;
        char cmd[128];

        ret = bcm_ifname_get(0, net_port->port, ifr.ifr_name);
        if (ret)
        {
            printf("bcm_ifname_get(%d) failed: ret=%d\n", net_port->port, ret);
            goto exit;
        }

        sprintf(cmd, "ifconfig %s down", ifr.ifr_name);
        ret = system(cmd);
        if (ret)
        {
            printf("system command failed: '%s' ret=%d\n", cmd, ret);
            goto exit;
        }
        else
        {
            printf("system command: '%s'\n", cmd);
            sleep(1);
        }
    }

    ifr.ifr_data = (void *)&ethswctl;
    ethswctl.op = is_add ? ETHPORTCREATE : ETHPORTDELETE;
    err = ioctl(skfd, SIOCETHSWCTLOPS, &ifr);
    if (err < 0 )
        printf("Error %d bcmenet port\n", err);

    if (is_add)
        strncpy(net_port->ifname, ethswctl.net_port.ifname, IFNAMSIZ);

exit:
    close(skfd);
    return err;
}

int bcm_port_create(struct net_port_t *net_port)
{
    return bcm_port(1, net_port);
}

int bcm_port_delete(int port)
{
    struct net_port_t net_port =
    {
        .port = port,
    };

    return bcm_port(0, &net_port);
}
int rdpaCtl_time_sync_init(void);

#define WAN_CONF_ERROR -1
#define WAN_CONF_SUCCESS 0
#define LAUNCH_EPON_APP 1

#define GET_SCRATCHPAD_VALUE_AND_LENGTH(scratchpad_parameter, local_buffer, length_in_bytes) do     \
    {                                                                                               \
        length_in_bytes = cmsPsp_get(scratchpad_parameter, local_buffer, sizeof(local_buffer) - 1); \
        if (0 >= length_in_bytes)                                                                   \
        {                                                                                           \
            wc_log_err("unexpected data in %s, len=%d", scratchpad_parameter, length_in_bytes);     \
            return WAN_CONF_ERROR;                                                                  \
        }                                                                                           \
        local_buffer[length_in_bytes] = 0;                                                          \
    } while (0)

#if defined(BRCM_CMS_BUILD)
static CmsRet sendWanOpStateMsg(void *msgHandle, WanConfPhyType phyType, UBOOL8 opState);
#endif /* BRCM_CMS_BUILD */

int insmod_param(char *driver, char *param)
{
    char cmd[128];
    int ret;

    sprintf(cmd, "insmod %s %s", driver, param ? : "");
    ret = system(cmd);
    if (ret)
        wc_log_err("unable to load module: %s\n", cmd);

    return ret;
}

#if defined(BRCM_CMS_BUILD)
CmsRet smd_start_app(UINT32 wordData)
{
    void *msgBuf;
    CmsRet ret;
    CmsMsgHeader *msg;
    void *msgHandle;
    int pid;

    ret = cmsMsg_initWithFlags(EID_WANCONF, 0, &msgHandle);
    if (ret)
    {
        wc_log_err("message init failed ret=%d\n", ret);
        return ret;
    }

    msgBuf = cmsMem_alloc(sizeof(CmsMsgHeader), ALLOC_ZEROIZE);
    if (msgBuf == NULL)
    {
        wc_log_err("message allocation failed\n");
        return CMSRET_INTERNAL_ERROR;
    }

    msg = (CmsMsgHeader *)msgBuf;
    msg->src = EID_WANCONF;
    msg->dst = EID_SMD;
    msg->flags_event = FALSE;
    msg->type = CMS_MSG_START_APP;
    msg->wordData = wordData;
    msg->dataLength = 0;

    pid = (int)cmsMsg_sendAndGetReply(msgHandle, msg);
    if (pid == CMS_INVALID_PID)
    {
        CMSMEM_FREE_BUF_AND_NULL_PTR(msgBuf);
        cmsMsg_cleanup(&msgHandle);
        wc_log_err("Failed to start app\n");
        return CMSRET_INTERNAL_ERROR;
    }

    if (wordData == EID_EPON_APP)
    {
        ret = sendWanOpStateMsg(msgHandle, WANCONF_PHY_TYPE_EPON, TRUE);
    }
    else if (wordData == EID_OMCID)
    {
        ret = sendWanOpStateMsg(msgHandle, WANCONF_PHY_TYPE_GPON, TRUE);
    }

    CMSMEM_FREE_BUF_AND_NULL_PTR(msgBuf);
    cmsMsg_cleanup(&msgHandle);

    return ret;
}
#endif /* BRCM_CMS_BUILD */

int load_gpon_modules(int sub_type, int speed __attribute__((unused)))
{
    int rc;

    insmod("/lib/modules/"KERNELVER"/extra/gpontrx.ko");

    if (sub_type == NET_PORT_SUBTYPE_GPON)
        rc = insmod("/lib/modules/"KERNELVER"/extra/gponstack.ko");
    else
        rc = insmod("/lib/modules/"KERNELVER"/extra/ngponstack.ko");

    if (rc)
        return WAN_CONF_ERROR;

    rc = insmod("/lib/modules/"KERNELVER"/extra/bcmgpon.ko");
    rc = rc ? : system("cat /dev/rgs_logger &");

    return rc;
}

static int load_gpon_omci(void)
{
#if defined(BRCM_CMS_BUILD)
    if (smd_start_app(EID_OMCID) != CMSRET_SUCCESS)
    {
        wc_log_err("Failed to start omcid app\n");
        return -1;
    }
#endif /* BRCM_CMS_BUILD */

#if defined(BRCM_BDK_BUILD) && defined(SUPPORT_ZBUS) 
    zbus_out_wanConf("gpon", "GPON");
#if defined(SUPPORT_BDK_SYSTEM_MANAGEMENT)
    zbus_out_wanConf("sysmgmt", "GPON");
#endif /* SUPPORT_BDK_SYSTEM_MANAGEMENT */
#endif /* BRCM_BDK_BUILD & SUPPORT_ZBUS */

    return 0;
}

static int getPidByName(const char *name)
{
    DIR *dir;
    FILE *fp;
    struct dirent *dent;
    UBOOL8 found=FALSE;
    long pid;
    int  rc, p, i;
    int rval = CMS_INVALID_PID;
    char filename[BUFLEN_256];
    char processName[BUFLEN_256];
    char *endptr;

    if (NULL == (dir = opendir("/proc")))
    {
        wc_log_err("could not open /proc\n");
        return rval;
    }

    while ( (!found) && (NULL != (dent = readdir(dir))) )
    {
        /*
         * Each process has its own directory under /proc, the name of the
         * directory is the pid number.
         */
        if (DT_DIR != dent->d_type)
            continue;

        pid = strtol(dent->d_name, &endptr, 10);
        if (ERANGE != errno && endptr != dent->d_name)
        {
            snprintf(filename, sizeof(filename), "/proc/%ld/stat", pid);
            if (NULL == (fp = fopen(filename, "r")))
            {
                wc_log_err("could not open %s\n", filename);
            }
            else
            {
                /* Get the process name, format: 913 (consoled) */
                memset(processName, 0, sizeof(processName));
                rc = fscanf(fp, "%d (%s", &p, processName);
                fclose(fp);

                if (rc >= 2)
                {
                    i = strlen(processName);
                    if (i > 0)
                    {
                        /* strip out the trailing ) character */
                        if (')' == processName[i-1])
                            processName[i-1] = 0;
                    }
                }

                if (!strncmp(processName, name,strlen(name)))
                {
                    rval = pid;
                    found = TRUE;
                }
            }
        }
    }

    closedir(dir);

    return rval;
}

static int load_epon_modules(int launch_epon_app)
{
#define KTHREAD_NAME "EponMPCP"
    int pid;
    struct sched_param sp = { .sched_priority = 10 };
    char cmd[128];
    int rc;

    sprintf(cmd, "insmod /lib/modules/"KERNELVER"/extra/bcmepon.ko epon_usr_init=%d", launch_epon_app);
    printf("load_epon_modules: %s\n\n", cmd);
    rc = system(cmd);
    if (rc)
    {
        wc_log("Failed to load epon modules (rc=%d)\n", rc);
        return rc;
    }

    if (!launch_epon_app)
        return 0;

    pid = getPidByName(KTHREAD_NAME);
    if (pid > 0)
    {
        if (WAN_CONF_ERROR == sched_setscheduler(pid, SCHED_RR, &sp))
        {
            wc_log_err("failed to set kthread %s with scheduler RR\n", KTHREAD_NAME);
            return WAN_CONF_ERROR;
        }
    }
    else
    {
        wc_log_err("unable to find pid for kthread %s\n", KTHREAD_NAME);
    }

    return rc;
}

static int load_epon_app(void)
{
#if defined(BRCM_CMS_BUILD)
    if (smd_start_app(EID_EPON_APP) != CMSRET_SUCCESS)
    {
        wc_log_err("Failed to start eponapp\n");
        return WAN_CONF_ERROR;
    }
#endif /* BRCM_CMS_BUILD */

#if defined(BRCM_BDK_BUILD) && defined(SUPPORT_ZBUS) 
    zbus_out_wanConf("epon", "EPON");
#if defined(SUPPORT_BDK_SYSTEM_MANAGEMENT)
    zbus_out_wanConf("sysmgmt", "EPON");
#endif /* SUPPORT_BDK_SYSTEM_MANAGEMENT */
#endif /* BRCM_BDK_BUILD & SUPPORT_ZBUS */

    return 0;
}

#define MATCH_VALUE(_buf, _value) !strncasecmp(_buf, _value, strlen(_value))

static int get_scratchpad_wan_rate(struct net_port_t *net_port)
{
    char buf[16];
    int count;

    GET_SCRATCHPAD_VALUE_AND_LENGTH(RDPA_WAN_RATE_PSP_KEY, buf, count);

    if (MATCH_VALUE(buf, RDPA_WAN_RATE_10G RDPA_WAN_RATE_10G))
        net_port->speed = NET_PORT_SPEED_1010;
    else if (MATCH_VALUE(buf, RDPA_WAN_RATE_10G RDPA_WAN_RATE_2_5G))
        net_port->speed = NET_PORT_SPEED_1025;
    else if (MATCH_VALUE(buf, RDPA_WAN_RATE_10G RDPA_WAN_RATE_1G))
        net_port->speed = NET_PORT_SPEED_1001;
    else if (MATCH_VALUE(buf, RDPA_WAN_RATE_2G RDPA_WAN_RATE_2G))
        net_port->speed = NET_PORT_SPEED_0202;
    else if (MATCH_VALUE(buf, RDPA_WAN_RATE_2G RDPA_WAN_RATE_1G))
        net_port->speed = NET_PORT_SPEED_0201;
    else if (MATCH_VALUE(buf, RDPA_WAN_RATE_1G RDPA_WAN_RATE_1G))
        net_port->speed = NET_PORT_SPEED_0101;

    return 0;
}

static int get_scratchpad_wan_port(struct net_port_t *net_port)
{
    char buf[16];
    int count;

    net_port->port = NET_PORT_NONE;
    get_scratchpad_wan_rate(net_port);

    GET_SCRATCHPAD_VALUE_AND_LENGTH(RDPA_WAN_TYPE_PSP_KEY, buf, count);

    if (MATCH_VALUE(buf, RDPA_WAN_TYPE_VALUE_NONE))
    {
        return 0;
    }
    else if (MATCH_VALUE(buf, RDPA_WAN_TYPE_VALUE_EPON))
    {
        net_port->port = NET_PORT_EPON;
#if 0
    else if (!strcasecmp(buf, RDPA_WAN_TYPE_VALUE_EPON))
    {
        char wan_rate_buf[16];
#define RATE_STR_LEN 2
#define PSP_RATE_STR_LEN 4

        wan_type = rdpa_wan_epon;
        GET_SCRATCHPAD_VALUE_AND_LENGTH(RDPA_WAN_RATE_PSP_KEY, wan_rate_buf, count);
        if ((count >= PSP_RATE_STR_LEN) && (!strncasecmp(&wan_rate_buf[RATE_STR_LEN], RDPA_WAN_RATE_10G, RATE_STR_LEN)))
            wan_type = rdpa_wan_xepon;
    }
#endif
    }
    else if (MATCH_VALUE(buf, RDPA_WAN_TYPE_VALUE_GPON))
    {
        net_port->port = NET_PORT_GPON;
        net_port->sub_type = NET_PORT_SUBTYPE_GPON;
    }
    else if (MATCH_VALUE(buf, RDPA_WAN_TYPE_VALUE_XGS))
    {
        net_port->port = NET_PORT_GPON;
        net_port->sub_type = NET_PORT_SUBTYPE_XGS;
    }
    else if (MATCH_VALUE(buf, RDPA_WAN_TYPE_VALUE_XGPON1))
    {
        net_port->port = NET_PORT_GPON;
        net_port->sub_type= NET_PORT_SUBTYPE_XGPON;
    }
    else if (MATCH_VALUE(buf, RDPA_WAN_TYPE_VALUE_NGPON2))
    {
        net_port->port = NET_PORT_GPON;
        net_port->sub_type= NET_PORT_SUBTYPE_NGPON;
    }
    else if (MATCH_VALUE(buf, RDPA_WAN_TYPE_VALUE_GBE))
    {
        net_port->port = NET_PORT_LAST;
    }

    if (net_port->port == NET_PORT_NONE)
    {
        wc_log_err("No valid WAN type is set in scratchpad %s = %s\n", RDPA_WAN_TYPE_PSP_KEY, buf);
        return -1;
    }

    if (net_port->port != NET_PORT_LAST)
        return 0;

    GET_SCRATCHPAD_VALUE_AND_LENGTH(RDPA_WAN_OEMAC_PSP_KEY, buf, count);

    if (!strncasecmp(buf ,"EMAC",4) && (strlen(buf) == strlen("EMACX")))
        net_port->port = buf[4] - '0';
    else if (MATCH_VALUE(buf, RDPA_WAN_OEMAC_VALUE_EPONMAC))
        net_port->port = NET_PORT_EPON_AE;
    else if (MATCH_VALUE(buf, "NONE"))
        net_port->port = NET_PORT_NONE;
    else
    {
        wc_log_err("No valid GBE MAC is set in scratchpad %s = %s\n", RDPA_WAN_OEMAC_PSP_KEY, buf);
        return -1;
    }

    return 0;
}

/* TODO: Should call bcm_enet_driver_wan_interface_get instead */
int get_enet_wan_port(char *ifname, unsigned int len)
{
    struct ifreq ifr;
    int err, skfd;
    struct ethswctl_data ethswctl;

    memset(&ethswctl, 0x0, sizeof(struct ethswctl_data));
    if ((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("socket open error\n");
        return WAN_CONF_ERROR;
    }

    strcpy(ifr.ifr_name, "bcmsw");
    if ((err = ioctl(skfd, SIOCGIFINDEX, &ifr)) < 0 )
    {
        printf("bcmsw interface does not exist");
        goto exit;
    }

    ethswctl.up_len.uptr = ifname;
    ethswctl.up_len.len = len;
    ifr.ifr_data = (void *)&ethswctl;
    err = ioctl(skfd, SIOCGWANPORT, &ifr);
    if (err < 0 )
        printf("Error %d bcmenet gbe wan port\n", err);

exit:
    close(skfd);
    return err;
}

#if defined(BRCM_CMS_BUILD)
void gbeSendPostMdmMsg(void *msgHandle)
{
    char buf[sizeof(CmsMsgHeader) + sizeof(MdmPostActNodeInfo)] = {0};
    CmsRet cmsReturn;
    CmsMsgHeader *msgHdr = (CmsMsgHeader *)buf;
    MdmPostActNodeInfo *msgBody = (MdmPostActNodeInfo *)(msgHdr + 1);

    msgHdr->dst = EID_SSK;
    msgHdr->src = EID_WANCONF;
    msgHdr->type = CMS_MSG_MDM_POST_ACTIVATING;
    msgHdr->flags_event = 1;
    msgHdr->dataLength = sizeof(MdmPostActNodeInfo);
    msgBody->subType = MDM_POST_ACT_TYPE_FILTER;

    /* Attempt to send CMS response message & test result. */
    cmsReturn = cmsMsg_send(msgHandle, msgHdr);
    if (CMSRET_SUCCESS != cmsReturn)
    {
        wc_log_err("Send message failure, cmsResult: %d", cmsReturn);
    }
    else
    {
        wc_log_err("Sent Wanconf App Indication to SSK");
    }
}

static int sendEnablePort(char *if_name)
{
    char buf[sizeof(CmsMsgHeader) + IFNAMESIZ]={0};
    CmsMsgHeader *msg=(CmsMsgHeader *) buf;
    char *msg_ifname = (char *)(msg+1);
    CmsRet ret;
    void *msgHandle;

    if (strlen(if_name) > IFNAMESIZ -1)
        return WAN_CONF_ERROR;

    ret = cmsMsg_initWithFlags(EID_WANCONF, 0, &msgHandle);
    if (ret)
        return ret;

    msg->type = CMS_MSG_WAN_PORT_ENABLE;
    msg->src = EID_WANCONF;
    msg->dst = EID_SSK;
    msg->flags_event = 1;
    msg->dataLength = IFNAMESIZ;

    strcpy(msg_ifname, if_name);

    if (CMSRET_SUCCESS != (ret = cmsMsg_send(msgHandle, msg)))
    {
        cmsMsg_cleanup(&msgHandle);
        wc_log_err("could not send out CMS_MSG_WAN_PORT_ENABLE, ret=%d", ret);
        return WAN_CONF_ERROR;
    }
    gbeSendPostMdmMsg(msgHandle);

    cmsMsg_cleanup(&msgHandle);

    return WAN_CONF_SUCCESS;
}

static CmsRet sendWanOpStateMsg(void *msgHandle, WanConfPhyType phyType, UBOOL8 opState)
{
    CmsRet ret = CMSRET_SUCCESS;
    char buf[sizeof(CmsMsgHeader) + sizeof(WanConfPhyOpStateMsgBody)] = {0};
    CmsMsgHeader *msg = (CmsMsgHeader*)buf;
    WanConfPhyOpStateMsgBody *info;

    msg->type = CMS_MSG_WAN_PORT_SET_OPSTATE;
    msg->src = EID_WANCONF;
    msg->dst = EID_SSK;
    msg->flags_request = 0;
    msg->flags_response = 0;
    msg->flags_event = 1;
    msg->dataLength = sizeof(WanConfPhyOpStateMsgBody);
    msg->wordData = 0;

    info = (WanConfPhyOpStateMsgBody*)&(buf[sizeof(CmsMsgHeader)]);
    info->phyType = phyType;
    info->opState = opState;

    ret = cmsMsg_send(msgHandle, msg);
    if (CMSRET_SUCCESS != ret)
    {
        wc_log_err("cmsMsg_send(CMS_MSG_WAN_PORT_SET_OPSTATE) failed, ret=%d", ret);
    }

    return ret;
}
#endif

#if defined(CONFIG_BCM963158)
static int get_sfp_info(sfp_type_t *sfp_type)
{
    struct ifreq ifr;
    int err, skfd;
    struct ethctl_data ethctl;

    memset(&ethctl, 0x0, sizeof(struct ethctl_data));
    if ((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("socket open error\n");
        return WAN_CONF_ERROR;
    }

    strcpy(ifr.ifr_name, "bcmsw");
    if ((err = ioctl(skfd, SIOCGIFINDEX, &ifr)) < 0 )
    {
        printf("bcmsw interface does not exist");
        goto exit;
    }

    ethctl.op = ETHGETSFPINFO;
    ifr.ifr_data = (void *)&ethctl;
    err = ioctl(skfd, SIOCETHCTLOPS, &ifr);
    if (err < 0 )
        printf("Error %d bcmenet gbe wan port\n", err);

    *sfp_type = ethctl.ret_val;

exit:
    close(skfd);
    return err;

}

static int waiting_gpon_module(void)
{
    int err;
    sfp_type_t sfp_type = SFP_TYPE_NO_MODULE;

    for (;;) {
        if ((err = get_sfp_info(&sfp_type))) {
            return err;
        }

        /* If the module is KNOWN XPON SFP or 10GAE port is not defined in
            board parameters, thus this is GPON only board design
            we quit waiting */
        if (sfp_type == SFP_TYPE_XPON || sfp_type == SFP_TYPE_NOT_ETHERNET)
            break;

        sleep(1);
    }

    return 0;
}
#endif

static int get_detect_wan_port(void)
{
#if !defined(CONFIG_BCM963158)
    int rc;
    rdpa_wan_type wan_type = rdpa_wan_none;

    rc = try_wan_type_detect_and_set(&wan_type);
    if (DETECTION_ERROR == rc)
        return -1;

    return 0;
#endif
    return -1;
}

int set_car_mode(net_port_t *net_port);
void set_iptv_lookup_method(void);
#if defined(PMD_JSON_LIB)
extern int set_pmd_wan_type(net_port_t *net_port);
#endif

static int get_init_scratchpad_wan_type(char *buf, int count)
{
#define WAN_DEFAULT_TYPE "GPON"
    int ret;

    ret = cmsPsp_get(RDPA_WAN_TYPE_PSP_KEY, buf, count - 1);
    if (ret < 0)
    {
        wc_log("cmsPsp_get: unable to read RDPA_WAN_TYPE_PSP_KEY from scratchpad\n");
        return WAN_CONF_ERROR;
    }

    if (ret == 0)
    {
        if ((ret = cmsPsp_set(RDPA_WAN_TYPE_PSP_KEY, WAN_DEFAULT_TYPE, strlen(WAN_DEFAULT_TYPE))))
        {
            wc_log("cmsPsp_set: unable to set default key RDPA_WAN_TYPE_PSP_KEY in scratchpad\n");
            return WAN_CONF_ERROR;
        }

        strcpy(buf, WAN_DEFAULT_TYPE);
    }

    return WAN_CONF_SUCCESS;
}

int main(int argc __attribute__((unused)), char *argv[] __attribute__((unused)))
{
    pid_t childPid = fork();
    char buf[16] = {};
    int rc;
    struct net_port_t net_port =
    {
        .is_wan = 1,
    };

    if (childPid < 0) /* Failed to fork */
        return WAN_CONF_ERROR;

    if (childPid != 0) /* Father always exists */
        return WAN_CONF_SUCCESS;

#if defined(CONFIG_BCM963158)
    if (waiting_gpon_module()) /* Waiting GPON SFP module insertion for GPON/10GAE co-existence */
        return WAN_CONF_ERROR;
#endif

    if (get_init_scratchpad_wan_type(buf, sizeof(buf)))
        return WAN_CONF_ERROR;
    wc_log("cmsPsp_get: RdpaWanType=%s\n", buf);

    /* Must be loaded even when not in PMD module because of symbol dep. */
    /* Load here before PMD WAN type auto-detection may take place.      */
    if (insmod("/lib/modules/"KERNELVER"/extra/pmd.ko"))
        return WAN_CONF_ERROR;

    if (MATCH_VALUE(buf, RDPA_WAN_TYPE_VALUE_AUTO) && get_detect_wan_port()) /* This will update scratchpad */
        return WAN_CONF_ERROR;

    if (get_scratchpad_wan_port(&net_port))
        return WAN_CONF_ERROR;

    wc_log("net_port: port=%d sub_type=%d speed=%d\n", net_port.port, net_port.sub_type, net_port.speed);

#if defined(PMD_JSON_LIB)
    /* Must be called before loading laser_dev.ko, which creates /dev/laser for non PMD */
    if ((net_port.port == NET_PORT_GPON || net_port.port == NET_PORT_EPON) && set_pmd_wan_type(&net_port))
        return WAN_CONF_ERROR;
#endif

    /* Loaded here because of dependency of bcm_i2c_pon_optics_type_get */
    if (insmod("/lib/modules/"KERNELVER"/extra/laser_dev.ko"))
        return WAN_CONF_ERROR;

    switch (net_port.port)
    {
        case NET_PORT_GPON:
            if (load_gpon_modules(net_port.sub_type, net_port.speed))
                return WAN_CONF_ERROR;

            break;
        case NET_PORT_LAN_0 ... NET_PORT_LAN_7:
            /* Skip removal of LAN port if WAN already exists: RDP had to create wan in system object */
            {
                if (get_enet_wan_port(net_port.ifname, IFNAMESIZ))
                    return -1;

                if (net_port.ifname[0] != 0)
                    goto wan_port_already_exists;

                if ((rc = bcm_port_delete(net_port.port)))
                {
                    wc_log("Failed to delete LAN port (port=%d rc=%d)\n", net_port.port, rc);
                    return WAN_CONF_ERROR;
                }
            }
            /* Fallthrough */
        case NET_PORT_EPON:
        case NET_PORT_EPON_AE:
        case NET_PORT_NONE:
            if (load_epon_modules(net_port.port == NET_PORT_EPON ? LAUNCH_EPON_APP : 0))
                return WAN_CONF_ERROR;

            break;
        default:
            return WAN_CONF_ERROR;
    }

    if (net_port.port == NET_PORT_NONE)
        return WAN_CONF_SUCCESS;

    if ((rc = bcm_port_create(&net_port)))
    {
        wc_log("Failed to create port (port=%d sub_type=%d rc=%d)\n", net_port.port, net_port.sub_type, rc);
        return WAN_CONF_ERROR;
    }

wan_port_already_exists:
    switch (net_port.port)
    {
        case NET_PORT_GPON:
            set_iptv_lookup_method();
            if (load_gpon_omci())
                return WAN_CONF_ERROR;
            break;
        case NET_PORT_EPON:
            if (load_epon_app())
                return WAN_CONF_ERROR;
            break;
        case NET_PORT_LAN_0 ... NET_PORT_LAN_7:
        case NET_PORT_EPON_AE:
#if defined(BRCM_CMS_BUILD)
            /* Signal CMS LAN interface changed to WAN */
            sendEnablePort(net_port.ifname);
#endif
#if defined(BRCM_BDK_BUILD) && defined(SUPPORT_ZBUS) && defined(SUPPORT_BDK_SYSTEM_MANAGEMENT)
            zbus_out_wanConf("sysmgmt", net_port.ifname);
#endif
            break;
    }

    if ((rc = rdpaCtl_time_sync_init()))
    {
        wc_log("Failed to call rdpaCtl_time_sync_init ioctl (rc=%d)\n", rc);
        return WAN_CONF_ERROR;
    }

    return WAN_CONF_SUCCESS;
}

