/***********************************************************************
 *
 *  Copyright (c) 2018  Broadcom Corporation
 *  All Rights Reserved
 *
<:label-BRCM:2018:DUAL/GPL:standard

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

#include "cms.h"
#include "cms_util.h"
#include "genutil_base64.h"

static CmsRet b64ret_to_cmsret(SINT32 b64Ret)
{
   CmsRet ret;

   switch (b64Ret)
   {
      case B64RET_SUCCESS:
         ret = CMSRET_SUCCESS;
         break;

      case B64RET_INTERNAL_ERROR:
         ret = CMSRET_INTERNAL_ERROR;
         break;

      case B64RET_RESOURCE_EXCEEDED:
         ret = CMSRET_RESOURCE_EXCEEDED;
         break;

      case B64RET_INVALID_ARGUMENTS:
         ret = CMSRET_INVALID_ARGUMENTS;
         break;

      default:
         ret = CMSRET_UNKNOWN_ERROR;
         break;
   }

   return ret;
}


CmsRet cmsB64_encode(const unsigned char *src, UINT32 srclen, char **dest)
{
   UINT32 destBufLen;
   SINT32 b64ret;

   if (dest == NULL)
   {
      return CMSRET_INVALID_ARGUMENTS;
   }

   /* Allocate buffer for the output. */
   destBufLen = genUtl_b64EncodedBufferLength(srclen);
   *dest = cmsMem_alloc(destBufLen, ALLOC_ZEROIZE);
   if (*dest == NULL)
   {
      return CMSRET_RESOURCE_EXCEEDED;
   }

   /* genUtl_b64Encode requires at least some input data.  Catch that special
    * case here and return a null string buf to maintain original behavior. */
   if (srclen == 0)
   {
      return CMSRET_SUCCESS;
   }

   b64ret = genUtl_b64Encode(src, srclen, *dest, destBufLen);
   if (b64ret != B64RET_SUCCESS) {
      cmsMem_free(*dest);
      *dest = NULL;
      return (b64ret_to_cmsret(b64ret));
   }

   return CMSRET_SUCCESS;
}


CmsRet cmsB64_decode(const char *base64Str, UINT8 **binaryBuf, UINT32 *binaryBufLen)
{
   UINT32 base64StrLen;
   SINT32 b64ret;

   if (base64Str == NULL || binaryBuf == NULL || binaryBufLen == NULL)
   {
      return CMSRET_INVALID_ARGUMENTS;
   }

   /* Allocate buffer for the output. */
   base64StrLen = strlen(base64Str);
   *binaryBufLen = genUtl_b64DecodedBufferLength(base64StrLen);
   *binaryBuf = cmsMem_alloc(*binaryBufLen, ALLOC_ZEROIZE);
   if (*binaryBuf == NULL)
   {
      *binaryBufLen = 0;
      return CMSRET_RESOURCE_EXCEEDED;
   }

   /* catch special case where base64Str is an empty string.  Still return
    * a buffer which must be freed, but actual amount of data is 0. */
   if (base64StrLen == 0)
   {
      *binaryBufLen = 0;
      return CMSRET_SUCCESS;
   }

   b64ret = genUtl_b64Decode(base64Str, *binaryBuf, binaryBufLen);
   if (b64ret != B64RET_SUCCESS)
   {
      cmsMem_free(*binaryBuf);
      *binaryBuf = NULL;
      *binaryBufLen = 0;
      return (b64ret_to_cmsret(b64ret));
   }

   return CMSRET_SUCCESS;
}
