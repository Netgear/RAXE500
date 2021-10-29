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


/*!\file bcm_generic_hal_utils.c
 * \brief This file implements some common utility functions associated with
 *        the BCM Generic HAL.  These are so useful that some code which do
 *        not use the BCM Generic HAL will still want to call these functions.
 *
 */

#include <stdlib.h>         /* for NULL */

#include "bcm_generic_hal_utils.h"
#include "cms_mem.h"


void cmsUtl_freeParamInfoArray(BcmGenericParamInfo  **paramInfoArray,
                               UINT32                 numParamInfos)
{
   BcmGenericParamInfo *a;
   UINT32 i;

   if (paramInfoArray == NULL)
      return;

   a = *paramInfoArray;
   if (a == NULL)
      return;

   // Due to the way cmsMem_alloc works, we might have a weird scenario where
   // the numParamInfos is 0, but "a" points to a valid piece of memory that
   // still needs to be freed.  The algo below still works even if num == 0.
   for (i=0; i < numParamInfos; i++)
   {
      CMSMEM_FREE_BUF_AND_NULL_PTR(a[i].fullpath);
      CMSMEM_FREE_BUF_AND_NULL_PTR(a[i].type);
      CMSMEM_FREE_BUF_AND_NULL_PTR(a[i].value);
      CMSMEM_FREE_BUF_AND_NULL_PTR(a[i].profile);
   }

   // Free the array buffer and set to NULL.
   CMSMEM_FREE_BUF_AND_NULL_PTR(*paramInfoArray);
   return;
}


void cmsUtl_freeParamAttrArray(BcmGenericParamAttr  **paramAttrArray,
                               UINT32                 numParamAttrs)
{
   BcmGenericParamAttr *a;
   UINT32 i;

   if (paramAttrArray == NULL)
      return;

   a = *paramAttrArray;
   if (a == NULL)
      return;

   for (i=0; i < numParamAttrs; i++)
   {
      CMSMEM_FREE_BUF_AND_NULL_PTR(a[i].fullpath);
   }

   // Free the array buffer and set to NULL.
   CMSMEM_FREE_BUF_AND_NULL_PTR(*paramAttrArray);
   return;
}


/** Free all strings in the array, and then free the array itself.
 *  Set the pointer to NULL (that is why ***).
 */
void cmsUtl_freeArrayOfStrings(char ***array, UINT32 len)
{
   char **arrayOfStrings;
   UINT32 i;

   if (array == NULL)
      return;

   arrayOfStrings = *array;
   if (arrayOfStrings == NULL)
      return;

   // See comments in  bcm_generic_freeParamInfos about len == 0.
   for (i=0; i < len; i++)
   {
      CMSMEM_FREE_BUF_AND_NULL_PTR(arrayOfStrings[i]);
   }

   // Null out the pointer from the caller so it is not accidentally used.
   CMSMEM_FREE_BUF_AND_NULL_PTR(*array);
   return;
}


