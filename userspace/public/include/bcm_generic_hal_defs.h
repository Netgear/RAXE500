/***********************************************************************
 *
 *  Copyright (c) 2020  Broadcom Ltd
 *  All Rights Reserved
 *
<:label-BRCM:2020:DUAL/GPL:standard

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
 *
 ************************************************************************/

#ifndef __BCM_GENERIC_HAL_DEFS_H__
#define __BCM_GENERIC_HAL_DEFS_H__

/*!\file bcm_generic_hal_defs.h
 * \brief Various generic hal defines that are used in various places in 
 *        CMS and BDK.  Not owned by any library, since many libraries
 *        use these defines.  Some users: CMS PHL, ZBus, DBus and UBus,
 *        bcm_generic_hal.
 *
 */

#include "os_defs.h"


// This struct is used by various flavors of getParameterNames and getParameterValues.
// If this struct is returned by getParmeterNames, the value field is not set.
typedef struct {
   char   *fullpath;
   char   *type;      // BBF types (MdmParamTypes) as defined in cms_mdm.h and mdm_binaryhelper.c
   char   *value;
   char   *profile;   // BBF profile, e.g. Device2_Baseline:1  Probably only TR69 cares about this.
   UBOOL8  writable;
   UBOOL8  isPassword;
   UINT32  errorCode;    // On return from setParameterValues, will be error code, or 0 if no errors.
} BcmGenericParamInfo;

// This struct is used by various flavors of getParameterAttributes and setParameterAttributes.
// See also MdmNodeAttributes in cms_mdm.h
typedef struct {
   char  *fullpath;
   UBOOL8 setAccess;   // only used during setAttr
   UINT16 access;      // access bitmask (set or get)
   UBOOL8 setNotif;    // only used during setAttr
   UINT16 notif;       // notification setting (set or get)
   UINT16 valueChanged;  // only used during getAttr: this param has notification enabled, and its value has changed.
   UBOOL8 setAltNotif;  // only used during setAttr (TODO: placeholder in struct, not implemented yet)
   UINT16 altNotif;     // TODO: placeholder in struct, not implemented yet
   UBOOL8 clearAltNotifValue;  // only used during setAttr (TODO: placeholder in struct, not implemented yet)
   UINT16 altNotifValue;       // 0 means no change (TODO: placeholder in struct, not implemented yet)
   UINT32 errorCode;   // On return from setParameterAttributes, will be error code, or 0 if no errors. (TODO: placeholder in struct, not implemented yet)
} BcmGenericParamAttr;


// These are the ops given to bcm_generic_databaseOp.
#define BCM_DATABASEOP_SAVECONFIG                     "saveConfig"
#define BCM_DATABASEOP_VALIDATECONFIG                 "validateConfig"
#define BCM_DATABASEOP_INVALIDATECONFIG               "invalidateConfig"
#define BCM_DATABASEOP_READCONFIG                     "readConfig"
#define BCM_DATABASEOP_WRITECONFIG                    "writeConfig"
#define BCM_DATABASEOP_READMDM                        "readMdm"
#define BCM_DATABASEOP_GET_NUM_VALUE_CHANGES          "getNumValuesChanges"
#define BCM_DATABASEOP_CLEAR_ALL_PARAM_VALUE_CHANGES  "clearAllParamValueChanges"
#define BCM_DATABASEOP_GET_CHANGED_PARAMS             "getChangedParams"
#define BCM_DATABASEOP_GET_ALT_NUM_VALUE_CHANGES          "getAltNumValuesChanges"
#define BCM_DATABASEOP_CLEAR_ALL_ALT_PARAM_VALUE_CHANGES  "clearAllAltParamValueChanges"
#define BCM_DATABASEOP_GET_ALT_CHANGED_PARAMS             "getAltChangedParams"


#endif /* __BCM_GENERIC_HAL_DEFS_H__ */
