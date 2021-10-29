/***********************************************************************
 *
 * <:copyright-BRCM:2019:DUAL/GPL:standard
 * 
 *    Copyright (c) 2019 Broadcom 
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
#include <net_port.h>
#include <user_api.h>

/* This function will override pktrunner default IPTV method lookup for SFU GPON modes */
int set_iptv_lookup_method(void)
{
    int rc = 0;
#if !defined(BCM_DSL_RDP) && !defined(BCM_DSL_XRDP)
    rdpa_system_init_cfg_t init_cfg;
    rdpa_system_cfg_t sys_cfg;
    bdmf_object_handle system_obj = 0, iptv_obj = 0;
    rdpa_iptv_lookup_method method = iptv_lookup_method_group_ip_src_ip;

    rc = rdpa_system_get(&system_obj);
    if (rc)
    {
        printf("%s %s Failed to get RDPA System object rc(%d)\n", __FILE__, __FUNCTION__, rc);
        goto Exit;
    }

    rc = rdpa_system_cfg_get(system_obj, &sys_cfg);
    if (rc)
    {
        printf("Failed to getting RDPA System cfg\n");
        goto Exit;
    }

    rc = rdpa_system_init_cfg_get(system_obj, &init_cfg);
    if (rc)
    {
        printf("Failed to getting RDPA System init cfg\n");
        goto Exit;
    }

    if (init_cfg.operation_mode != rdpa_method_prv)
        goto Exit;

    rc = rdpa_iptv_get(&iptv_obj);
    if (rc)
    {
        /* This is not an actual error, since IPTV object does not exist yet.
         * Method will be set later when pktrunner is loaded. */
        rc = 0;
        printf("Skip setting RDPA IPTV object\n");
        goto Exit;
    }

    rc = rdpa_iptv_lookup_method_set(iptv_obj, method);
    if (rc)
        printf("Failed to set RDPA IPTV lookup method %d\n", method);

Exit:
    if (iptv_obj)
        bdmf_put(iptv_obj);

    if (system_obj)
        bdmf_put(system_obj);

#endif
    return rc;
}

int set_car_mode(net_port_t *net_port __attribute__((unused)))
{
    int rc;
    rdpa_system_init_cfg_t init_cfg;
    rdpa_system_cfg_t sys_cfg;
    bdmf_object_handle system_obj = 0;

    rc = rdpa_system_get(&system_obj);
    if (rc)
    {
        printf("%s %s Failed to get RDPA System object rc(%d)\n", __FILE__, __FUNCTION__, rc);
        goto Exit;
    }

    rc = rdpa_system_cfg_get(system_obj, &sys_cfg);
    if (rc)
    {
        printf("Failed to getting RDPA System cfg\n");
        goto Exit;
    }

    rc = rdpa_system_init_cfg_get(system_obj, &init_cfg);
    if (rc)
    {
        printf("Failed to getting RDPA System init cfg\n");
        goto Exit;
    }

    if (init_cfg.operation_mode == rdpa_method_fc)
        sys_cfg.car_mode = 1;
    else
        sys_cfg.car_mode = 0;

    rc = rdpa_system_cfg_set(system_obj, &sys_cfg);
    if(rc)
        printf("%s %s Failed to set RDPA System car mode rc(%d)\n", __FILE__, __FUNCTION__, rc);

Exit:
    if (system_obj)
        bdmf_put(system_obj);

    return rc;
}

