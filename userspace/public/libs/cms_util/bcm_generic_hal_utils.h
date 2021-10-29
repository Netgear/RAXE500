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


#ifndef __BCM_GENERIC_HAL_UTILS_H__
#define __BCM_GENERIC_HAL_UTILS_H__

/*!\file bcm_generic_hal_utils.h
 * \brief Utility functions associated with the BCM Generic HAL.  These
 *        are so useful that some code which do not use the Generic HAL
 *        will still want to use these functions.
 *
 */

#include "number_defs.h"
#include "bcm_generic_hal_defs.h"


/** Free an array of BcmGenericParamInfo.
 *
 *  The array and its internal pointers are allocated with cmsMem_alloc,
 *  so it is important that code use this function to free instead of trying
 *  to free with the standard libc free.  This function will also null out
 *  the pointer to the array to prevent accidental use after free.
 *
 *  @param address of the array pointer.
 *  @param number of elements in the array.
 */
void cmsUtl_freeParamInfoArray(BcmGenericParamInfo  **paramInfoArray,
                               UINT32                 numParamInfos);


/** Free an array of BcmGenericParamAttr.
 *  Same comments as cmsUtl_freeParamInfos.
 */
void cmsUtl_freeParamAttrArray(BcmGenericParamAttr  **paramAttrArray,
                               UINT32                 numParamAttrs);


/** Free all strings in the array, and then free the array itself.
 *  Set the pointer to NULL (that is why ***).  Note that the array and the
 *  string buffers are allocated with cmsMem_alloc, so it is important that
 *  code use this function to free instead of trying to free with the standard
 *  libc free.
 */
void cmsUtl_freeArrayOfStrings(char ***array, UINT32 len);


#endif /* __BCM_GENERIC_HAL_UTILS__ */
