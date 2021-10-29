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

#ifndef _USER_API_H_
#define _USER_API_H_

#include "rdpa_user_types.h"
#include "rdpa_user.h"

#ifdef PORT_OBJECT
#include "rdpa_port_user_ag.h"
#endif /* PORT_OBJECT */
#ifdef EGRESS_TM_OBJECT
#include "rdpa_egress_tm_user_ag.h"
#endif /* EGRESS_TM_OBJECT */
#ifdef TCONT_OBJECT
#include "rdpa_tcont_user_ag.h"
#endif /* TCONT_OBJECT */
#ifdef VLAN_OBJECT
#include "rdpa_vlan_user_ag.h"
#endif /* VLAN_OBJECT */
#ifdef VLAN_ACTION_OBJECT
#include "rdpa_vlan_action_user_ag.h"
#endif /* VLAN_ACTION_OBJECT */
#ifdef SYSTEM_OBJECT
#include "rdpa_system_user_ag.h"
#endif /* SYSTEM_OBJECT */
#ifdef IPTV_OBJECT
#include "rdpa_iptv_user_ag.h"
#endif /* IPTV_OBJECT */
#ifdef TC_TO_QUEUE_OBJECT
#include "rdpa_tc_to_queue_user_ag.h"
#endif /* TC_TO_QUEUE_OBJECT */
#ifdef LLID_OBJECT
#include "rdpa_llid_user_ag.h"
#endif /* LLID_OBJECT */
#ifdef INGRESS_CLASS_OBJECT
#include "rdpa_ingress_class_user_ag.h"
#endif /* INGRESS_CLASS_OBJECT */
#ifdef POLICER_OBJECT
#include "rdpa_policer_user_ag.h"
#endif /* POLICER_OBJECT */
#ifdef PBIT_TO_QUEUE_OBJECT
#include "rdpa_pbit_to_queue_user_ag.h"
#endif /* PBIT_TO_QUEUE_OBJECT */
#ifdef PBIT_TO_GEM_OBJECT
#include "rdpa_pbit_to_gem_user_ag.h"
#endif /* PBIT_TO_GEM_OBJECT */
#ifdef DSCP_TO_PBIT_OBJECT
#include "rdpa_dscp_to_pbit_user_ag.h"
#endif /* DSCP_TO_PBIT_OBJECT */
#ifdef UDPSPDTEST_OBJECT
#include "rdpa_udpspdtest_user_ag.h"
#endif /* UDPSPDTEST_OBJECT */
#include "bdmf_api.h"
#ifdef MLLID_OBJECT
#include "rdpa_mllid_user_ag.h"
#endif /* MLLID_OBJECT */

#endif /* _USER_API_H_ */
