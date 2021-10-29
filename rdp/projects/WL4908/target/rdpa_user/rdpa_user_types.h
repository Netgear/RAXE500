// <:copyright-BRCM:2013:DUAL/GPL:standard
// 
//    Copyright (c) 2013 Broadcom 
//    All Rights Reserved
// 
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License, version 2, as published by
// the Free Software Foundation (the "GPL").
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// 
// A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
// writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
// Boston, MA 02111-1307, USA.
// 
// :>

#ifndef _RDPA_USER_TYPES_H_
#define _RDPA_USER_TYPES_H_

#include <bdmf_interface.h>
#include <bdmf_system.h>
#include <rdpa_types.h>
#include <rdpa_config.h>

#include <rdpa_system.h>
#include <rdpa_port.h>
#include <rdpa_cpu_basic.h>
#include <rdpa_cpu.h>
#include <rdpa_ingress_class_basic.h>
#include <rdpa_ingress_class.h>
#include <rdpa_tcont.h>
#include <rdpa_llid.h>
#include <rdpa_mllid.h>
#include <rdpa_egress_tm.h>
#include <rdpa_vlan_action.h>
#include <rdpa_gem.h>
#include <rdpa_filter.h>
#if !defined(BCM_DSL_RDP) && !defined(BCM_DSL_XRDP) && !defined(RDP_UFC)
#include <rdpa_ip_class.h>
#endif

#include <rdpa_bridge.h>
#include <rdpa_vlan.h>
#include <rdpa_qos_mapper.h>
#include <rdpa_tm.h>
#include <rdpa_wlan_mcast.h>
#if defined(CONFIG_BCM_SPDSVC_SUPPORT) && (!defined(G9991) || defined(XRDP))
#include <rdpa_spdsvc.h>
#endif
#if defined(CONFIG_RUNNER_IPSEC)
#include <rdpa_ipsec.h>
#endif
#if defined(BCM_DSL_RDP) || defined(BCM_DSL_XRDP) || defined(RDP_UFC)
#include <rdpa_ucast.h>
#include <rdpa_mcast.h>
#include <rdpa_l2_ucast.h>
#if defined(BCM_DSL_RDP) || defined(BCM_DSL_XRDP)
#include <rdpa_xtm.h>
#endif
#else
#include <rdpa_iptv.h>
#endif /* BCM_DSL_RDP || BCM_DSL_XRDP */

#ifdef CONFIG_DHD_RUNNER
#include <rdpa_dhd_helper.h>
#endif

#if defined(CONFIG_BCM_UDPSPDTEST_SUPPORT) || defined(CONFIG_BCM_SPDTEST)
#include <rdpa_udpspdtest.h>
#endif

#endif /* _RDPA_USER_TYPES_H_ */
