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
#include "genutil_hexbinary.h"

static CmsRet hexret_to_cmsret(SINT32 hexRet)
{
   CmsRet ret;

   switch (hexRet)
   {
      case HEXRET_SUCCESS:
         ret = CMSRET_SUCCESS;
         break;

      case HEXRET_INTERNAL_ERROR:
         ret = CMSRET_INTERNAL_ERROR;
         break;

      case HEXRET_RESOURCE_EXCEEDED:
         ret = CMSRET_RESOURCE_EXCEEDED;
         break;

      case HEXRET_INVALID_ARGUMENTS:
         ret = CMSRET_INVALID_ARGUMENTS;
         break;

      case HEXRET_CONVERSION_ERROR:
         ret = CMSRET_CONVERSION_ERROR;
         break;

      default:
         ret = CMSRET_UNKNOWN_ERROR;
         break;
   }

   return ret;
}


CmsRet cmsUtl_binaryBufToHexStringStrict(const UINT8 *binaryBuf, UINT32 binaryBufLen, char **hexStr)
{
   /* reject if there is no input data to convert */
   if (binaryBuf == NULL || binaryBufLen == 0)
   {
      return CMSRET_INVALID_ARGUMENTS;
   }
   return (cmsUtl_binaryBufToHexString(binaryBuf, binaryBufLen, hexStr));
}

CmsRet cmsUtl_binaryBufToHexString(const UINT8 *binaryBuf, UINT32 binaryBufLen, char **hexStr)
{
   SINT32 hexRet;

   if (hexStr == NULL)
   {
      cmsLog_error("hexStr buffer is NULL");
      return CMSRET_INVALID_ARGUMENTS;
   }

   *hexStr = cmsMem_alloc((binaryBufLen*2)+1, ALLOC_ZEROIZE);
   if (*hexStr == NULL)
   {
      return CMSRET_RESOURCE_EXCEEDED;
   }

   /* genUtl_binaryBufToHexString does not allow 0 length binary buf, so
    * catch that case here and return with an empty string buf to
    * maintain original behavior. */
   if (binaryBufLen == 0)
   {
      return CMSRET_SUCCESS;
   }

   hexRet = genUtl_binaryBufToHexString(binaryBuf, binaryBufLen, *hexStr);
   if (hexRet != HEXRET_SUCCESS)
   {
      cmsMem_free(*hexStr);
      *hexStr = NULL;
      return hexret_to_cmsret(hexRet);
   }

   return CMSRET_SUCCESS;
}


CmsRet cmsUtl_hexStringToBinaryBufStrict(const char *hexStr, UINT8 **binaryBuf, UINT32 *binaryBufLen)
{
   /* reject if there is no input data */
   if (hexStr == NULL || hexStr[0] == '\0')
   {
      return CMSRET_INVALID_ARGUMENTS;
   }
   return (cmsUtl_hexStringToBinaryBuf(hexStr, binaryBuf, binaryBufLen));
}

CmsRet cmsUtl_hexStringToBinaryBuf(const char *hexStr, UINT8 **binaryBuf, UINT32 *binaryBufLen)
{
   UINT32 hexStrLen;
   SINT32 hexRet;

   if (hexStr == NULL || binaryBuf == NULL || binaryBufLen == NULL)
   {
      return CMSRET_INVALID_ARGUMENTS;
   }

   hexStrLen = strlen(hexStr);
   if (hexStrLen % 2 != 0)
   {
      cmsLog_error("hexStr must be an even number of characters");
      return CMSRET_INVALID_ARGUMENTS;
   }

   *binaryBuf = cmsMem_alloc(hexStrLen/2, ALLOC_ZEROIZE);
   if (*binaryBuf == NULL)
   {
      return CMSRET_RESOURCE_EXCEEDED;
   }

   /* genUtl_hexStringToBinaryBuf does not allow 0 length hexString, so
    * catch that case here and return with a "zero length buf" to
    * maintain original behavior. Caller still needs to free the buf. */
   if (hexStrLen == 0)
   {
      *binaryBufLen = 0;
      return CMSRET_SUCCESS;
   }

   hexRet = genUtl_hexStringToBinaryBuf(hexStr, *binaryBuf);
   if (hexRet != HEXRET_SUCCESS)
   {
      cmsMem_free(*binaryBuf);
      *binaryBuf = NULL;
      return hexret_to_cmsret(hexRet);
   }

   /* if we get here, we were successful, set length */
   *binaryBufLen = hexStrLen / 2;

   return CMSRET_SUCCESS;
}
