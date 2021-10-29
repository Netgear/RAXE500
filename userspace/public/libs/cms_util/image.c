/***********************************************************************
 * <:copyright-BRCM:2007:DUAL/GPL:standard
 * 
 *    Copyright (c) 2007 Broadcom 
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
 ************************************************************************/

/*!\file image.c
 * \brief Most or maybe all of the functionality in this file has been
 *        copied to bcm_imgutil.c.  Going forward, make fixes and enhancements
 *        in that file and redirect calls from this file to that file.
 *  
 */
 
#if defined(BRCM_CMS_BUILD) || defined (BRCM_BDK_BUILD) || defined(SUPPORT_BEEP)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/if.h>

#include "cms.h"
#include "bdk.h"
#include "cms_util.h"
#include "cms_msg.h"
#include "cms_boardcmds.h"
#include "cms_boardioctl.h"
#include "oal.h"
#include "bcmTag.h" /* in shared/opensource/include/bcm963xx, for FILE_TAG */
#include "board.h" /* in bcmdrivers/opensource/include/bcm963xx, for BCM_IMAGE_CFE */

#include "genutil_crc.h"
#include "bcm_flashutil.h"
#include "bcm_imgutil.h"


static CmsRet flashImage(char *imagePtr, UINT32 imageLen, CmsImageFormat format, UINT32 opts, int * noReboot);
static CmsRet sendConfigMsg(const char *imagePtr, UINT32 imageLen, void *msgHandle, CmsMsgType msgType);
static void dumpBcmFileTag(FILE_TAG *tagP);


// depending on the image type, do the brcm image or whole flash image
CmsRet flashImage(char *imagePtr, UINT32 imageLen, CmsImageFormat format, UINT32 opts, int * noReboot)
{
    FILE_TAG *pTag = (FILE_TAG *) imagePtr;
    int cfeSize=0, rootfsSize=0, kernelSize=0;
    unsigned long cfeAddr=0L, rootfsAddr=0L, kernelAddr=0L;
    UINT32 flags=0;
    CmsRet ret;

    cmsLog_notice("Entered: format=%d opts=0x%x", format, opts);
    if (format != CMS_IMAGE_FORMAT_FLASH && format != CMS_IMAGE_FORMAT_BROADCOM)
    {
       cmsLog_error("invalid image format %d", format);
       return CMSRET_INVALID_IMAGE;
    }

    if( opts & CMS_IMAGE_WR_OPT_PART1 )
    {
        *noReboot = (opts & CMS_IMAGE_WR_OPT_NO_REBOOT) ? FLASH_PART1_NO_REBOOT : FLASH_PART1_REBOOT;
    }
    else if( opts & CMS_IMAGE_WR_OPT_PART2 )
    {
        *noReboot = (opts & CMS_IMAGE_WR_OPT_NO_REBOOT) ? FLASH_PART2_NO_REBOOT : FLASH_PART2_REBOOT;
    }
    else
    {
        *noReboot = (opts & CMS_IMAGE_WR_OPT_NO_REBOOT) ? FLASH_PARTDEFAULT_NO_REBOOT : FLASH_PARTDEFAULT_REBOOT;
    }

    cmsLog_debug("noReboot=0x%x", *noReboot);

    if ((*noReboot & 0xFFFF) == 0)
    {
       /* we will reboot after the image write, sync FS first */
       sync();
    }

    if (format == CMS_IMAGE_FORMAT_FLASH)
    {
        printf("Flash whole image...\n");
        // Pass zero for the base address of whole image flash. It will be filled by kernel code
        // was sysFlashImageSet
        if ((ret = getFlashInfo(&flags)) == CMSRET_SUCCESS)
        {
           if(flags & FLASH_INFO_FLAG_NOR)
           { // NOR flash device
              ret = devCtl_boardIoctl(BOARD_IOCTL_FLASH_WRITE,
                                      BCM_IMAGE_WHOLE,
                                      imagePtr,
                                      imageLen-TOKEN_LEN,
                                      *noReboot, 0);
           }
           else
           { // NAND flash device
              if( flags & FLASH_INFO_FLAG_NAND )
                 ret = writeImageToNand( (unsigned char *)imagePtr, imageLen - TOKEN_LEN );
              else if( flags & FLASH_INFO_FLAG_EMMC )
                 cmsLog_error("Not implemented for EMMC!");
           }
        }

        *noReboot &= 0xFFFF;

        if (ret != CMSRET_SUCCESS)
        {
           cmsLog_error("Failed to flash whole image");
           return CMSRET_IMAGE_FLASH_FAILED;
        }
        else
        {
           return CMSRET_SUCCESS;
        }
    }

    /* this must be a broadcom format image */
    // check imageCrc
    cfeSize = rootfsSize = kernelSize = 0;

    // check cfe's existence
    cfeAddr = (unsigned long) strtoul((char *) pTag->cfeAddress, NULL, 10);
    cfeSize = atoi((char *) pTag->cfeLen);
    // check kernel existence
    kernelAddr = (unsigned long) strtoul((char *) pTag->kernelAddress, NULL, 10);
    kernelSize = atoi((char *) pTag->kernelLen);
    // check root filesystem existence
    rootfsAddr = (unsigned long) strtoul((char *) pTag->rootfsAddress, NULL, 10);
    rootfsSize = atoi((char *) pTag->rootfsLen);
    cmsLog_debug("cfeSize=%d kernelSize=%d rootfsSize=%d", cfeSize, kernelSize, rootfsSize);
    cmsLog_debug("cfeAddr=0x%lu rootfsAddr=0x%lu kernelAddr=0x%lu",
                  cfeAddr, rootfsAddr, kernelAddr);

    if (cfeAddr)
    {
        printf("Flashing CFE...\n");

        ret = devCtl_boardIoctl(BOARD_IOCTL_FLASH_WRITE,
                                BCM_IMAGE_CFE,
                                imagePtr+TAG_LEN,
                                cfeSize,
                                (int) cfeAddr, 0);
        if (ret != CMSRET_SUCCESS)
        {
            cmsLog_error("Failed to flash CFE");
            return CMSRET_IMAGE_FLASH_FAILED;
        }
    }

    if (rootfsAddr && kernelAddr)
    {
        char *tagFs = imagePtr;

        // tag is alway at the sector start of fs
        if (cfeAddr)
        {
            tagFs = imagePtr + cfeSize;       // will trash cfe memory, but cfe is already flashed
            memcpy(tagFs, imagePtr, TAG_LEN);
        }

        printf("Flashing root file system and kernel...\n");
        /* only the buf pointer and length is needed, the offset parameter
         * was present in the legacy code, but is not used. */
        ret = devCtl_boardIoctl(BOARD_IOCTL_FLASH_WRITE,
                                BCM_IMAGE_FS,
                                tagFs,
                                TAG_LEN+rootfsSize+kernelSize,
                                *noReboot, 0);
        if (ret != CMSRET_SUCCESS)
        {
            cmsLog_error("Failed to flash root file system and kernel");
            return CMSRET_IMAGE_FLASH_FAILED;
        }
    }

    *noReboot &= 0xFFFF;

    if (cfeAddr == 0 && rootfsAddr == 0 && kernelAddr == 0)
       cmsLog_error("Possible error: cfe, kernel, rootfs not flashed");

    return CMSRET_SUCCESS;
}


UINT32 cmsImg_getImageFlashSize(void)
{
   return (bcmImg_getImageFlashSize());
}


UINT32 cmsImg_getBroadcomImageTagSize(void)
{
   return (bcmImg_getBroadcomImageTagSize());
}


UINT32 cmsImg_getConfigFlashSize(void)
{
   return (bcmImg_getConfigFlashSize());
}


UINT32 cmsImg_getRealConfigFlashSize(void)
{
   return (bcmImg_getRealConfigFlashSize());
}


UBOOL8 cmsImg_willFitInFlash(UINT32 imageSize)
{
   return (bcmImg_willFitInFlash(imageSize));
}


UBOOL8 cmsImg_isBackupConfigFlashAvailable(void)
{
   return (bcmImg_isBackupConfigFlashAvailable());
}


UBOOL8 cmsImg_isConfigFileLikely(const char *buf)
{
   return (bcmImg_isConfigFileLikely(buf));
}


/** General entry point for writing the image.
 *  The image can be a flash image or a config file.
 *  This function will first determine the image type, which has the side
 *  effect of validating it.
 */
CmsRet cmsImg_writeImage(char *imagePtr, UINT32 imageLen, void *msgHandle)
{
   CmsImageFormat format;
   CmsRet ret;
   
   if ((format = cmsImg_validateImage(imagePtr, imageLen, msgHandle)) == CMS_IMAGE_FORMAT_INVALID)
   {
      ret = CMSRET_INVALID_IMAGE;
   }
   else
   {
      ret = cmsImg_writeValidatedImage(imagePtr, imageLen, format, msgHandle);
   }

   return ret;
}


CmsRet cmsImg_writeImage_noReboot(char *imagePtr, UINT32 imageLen, void *msgHandle)
{
   CmsImageFormat format;
   CmsRet ret;
   
   if ((format = cmsImg_validateImage(imagePtr, imageLen, msgHandle)) == CMS_IMAGE_FORMAT_INVALID)
   {
      ret = CMSRET_INVALID_IMAGE;
   }
   else
   {
      ret = cmsImg_writeValidatedImage_noReboot(imagePtr, imageLen, format, msgHandle);
   }

   return ret;
}


CmsRet cmsImg_writeValidatedImage(char *imagePtr, UINT32 imageLen,
                                  CmsImageFormat format, void *msgHandle)
{
   if (0xb0 & (UINT32)format)
   {
      /*
       * Prior to 4.14L.01 release, we had some overloaded bits in the
       * format enum.  They have been moved out to CMS_IMAGE_WR_OPT_XXX,
       * but leave some code here to detect old code and usage.
       * If you see this message and you are sure you are using the code
       * correctly, you can ignore this message or delete it.
       */
      cmsLog_error("suspicious old bits set in format enum, format=%d (0x%x)",
                   format, format);
   }

   return (cmsImg_writeValidatedImageEx(imagePtr, imageLen,
                                        format, msgHandle, 0));
}


CmsRet cmsImg_writeValidatedImage_noReboot(char *imagePtr, UINT32 imageLen,
                                  CmsImageFormat format, void *msgHandle)
{
   if (0xb0 & (UINT32)format)
   {
      /*
       * Prior to 4.14L.01 release, we had some overloaded bits in the
       * format enum.  They have been moved out to CMS_IMAGE_WR_OPT_XXX,
       * but leave some code here to detect old code and usage.
       * If you see this message and you are sure you are using the code
       * correctly, you can ignore this message or delete it.
       */
      cmsLog_error("suspicious old bits set in format enum, format=%d (0x%x)",
                   format, format);
   }

   return (cmsImg_writeValidatedImageEx(imagePtr, imageLen,
                                        format, msgHandle, CMS_IMAGE_WR_OPT_NO_REBOOT));
}


void cmsImg_reboot(void)
{
   sync();
   devCtl_boardIoctl(BOARD_IOCTL_MIPS_SOFT_RESET, 0, NULL, 0, 0, 0);
}


CmsRet cmsImg_writeValidatedImageEx(char *imagePtr, UINT32 imageLen,
                                    CmsImageFormat format, void *msgHandle,
                                    UINT32 opts)
{
   CmsRet ret=CMSRET_SUCCESS;
   int noReboot = 0;

   cmsLog_notice("Entered: format=0x%x opts=0x%x", format, opts);

   switch(format)
   {
   case CMS_IMAGE_FORMAT_BROADCOM:
   case CMS_IMAGE_FORMAT_FLASH:
      if ((ret = flashImage(imagePtr, imageLen, format, opts, &noReboot)) == CMSRET_SUCCESS)
      {
         printf("FirmwareSelect: image written, COMMIT the NEW_IMAGE in partition %s (noReboot=%d)\n",
                ((getBootPartition() == 1) ? "2" : "1"), noReboot);
         setBootImageState(BOOT_SET_NEW_IMAGE);
         if (noReboot == 0)
         {
            cmsImg_reboot();
         }
      }
      else
      {
         fprintf(stderr, "ERROR!!! Image could not be flashed.\n");
      }
      break;
      
   case CMS_IMAGE_FORMAT_XML_CFG:
      ret = sendConfigMsg(imagePtr, imageLen, msgHandle, CMS_MSG_WRITE_CONFIG_FILE);
      if (ret == CMSRET_SUCCESS)
      {
         /*
          * Emulate the behavior of the driver when a flash image is written.
          * When we write a config image, also request immediate reboot
          * because we don't want to let any other app save the config file
          * to flash, thus wiping out what we just written.
          */
         cmsLog_debug("config file download written, request reboot");
         cmsUtil_sendRequestRebootMsg(msgHandle);
      }
      break;
      
#ifdef DMP_X_BROADCOM_COM_MODSW_LINUXPFP_1
   case CMS_IMAGE_FORMAT_MODSW_LINUXPFP:
   {
      ret = cmsImg_writeValidatedLinuxPfp(imagePtr, imageLen, msgHandle);
      break;
   }
#endif

   default:
       cmsLog_error("Unrecognized image format=%d", format);
       ret = CMSRET_INVALID_IMAGE;
       break;
    }
   
   return ret;
}

CmsImageFormat cmsImg_validateImage(const char *imageBuf, UINT32 imageLen, void *msgHandle)
{
   cmsLog_notice("Entered: imageBuf=%p len=%d (msgHandle=%p)",
                 imageBuf, imageLen, msgHandle);

   if (imageBuf == NULL)
   {
      cmsLog_error("imageBuf is NULL");
      return CMS_IMAGE_FORMAT_INVALID;
   }
   
   if ((imageLen > CMS_CONFIG_FILE_DETECTION_LENGTH) &&
       cmsImg_isConfigFileLikely(imageBuf))
   {
      CmsRet ret;

      cmsLog_debug("possible CMS XML config file format detected");
      if (msgHandle == NULL)
      {
         cmsLog_error("CMS XML config file detected, but msgHandle is NULL!");
         return CMS_IMAGE_FORMAT_INVALID;
      }
      ret = sendConfigMsg(imageBuf, imageLen, msgHandle, CMS_MSG_VALIDATE_CONFIG_FILE);
      if (ret == CMSRET_SUCCESS)
      { 
         cmsLog_debug("CMS XML config format verified.");
         return CMS_IMAGE_FORMAT_XML_CFG;
      }
      else
      {
         // Not clear what should happen in this case:
         // Is the error due to just the sending of the config file msg?
         // Or is the config file not valid, and if not a valid config file,
         // should we return or try to validate it as an image file?
         // Preserve the exisitng logic and fall through.
         cmsLog_debug("sendConfigMsg ret=%d", ret);
      }
   }

   cmsLog_debug("not a config file");
   // Now do non-CMS processing in bcm_flashutil.
   return (bcmImg_validateImage(imageBuf, imageLen));
}


UINT32 cmsImg_countConfigSections(const char *buf)
{
    const char *delim = CMS_CONFIG_XML_TAG;
    char *pBegin, *pNext;
    UINT32 count=0;

    if (buf == NULL)
        return count;

    pBegin = strstr(buf, delim);
    while (pBegin != NULL)
    {
       count++;
       pNext= strstr((pBegin+strlen(delim)), delim);
       pBegin = pNext;
    }
    cmsLog_debug("Exit: sections=%d", count);
    return count;
}

const char* cmsImg_configTagToComponentName(const char *buf)
{
    const char *errorStr = "ERROR_INVALID_CONFIG";
    const char *componentName = NULL;
    UINT32 tagCount=0;
    UINT32 sectionCount;

    sectionCount = cmsImg_countConfigSections(buf);
    if (sectionCount == 0)
       goto detect_done;

    /* Search the entire config buf for a tag unique to that component.
     * In 504L02, all config files will have a "bdkConfigIdent" tag to
     * precisely identify which component this config file came from.  However,
     * to detect older config files (including old CMS monolithic config files)
     * which do not have this "bdkConfigIdent" tag, we have to use a heuristic
     * to look for a tag which is unique to that component.  Should be fine,
     * but there is a tiny possibility of misidentification.
     */
    if ((cmsUtl_strstr(buf, "bdkConfigIdent=\"sysmgmt\"") != NULL) ||
        (cmsUtl_strstr(buf, "<DNS>") != NULL))
    {
       /* DNS is required for Device2 */
       componentName = BDK_COMP_SYSMGMT;
       if (++tagCount > 1)
           goto detect_done;
    }

    if ((cmsUtl_strstr(buf, "bdkConfigIdent=\"wifi\"") != NULL) ||
        (cmsUtl_strstr(buf, "<WiFi>") != NULL))
    {
       componentName = BDK_COMP_WIFI;
       if (++tagCount > 1)
           goto detect_done;
    }

    if ((cmsUtl_strstr(buf, "bdkConfigIdent=\"devinfo\"") != NULL) ||
        (cmsUtl_strstr(buf, "<DeviceInfo>") != NULL))
    {
       componentName = BDK_COMP_DEVINFO;
       if (++tagCount > 1)
           goto detect_done;
    }

    else if ((cmsUtl_strstr(buf, "bdkConfigIdent=\"dsl\"") != NULL) ||
             (cmsUtl_strstr(buf, "<DSL>") != NULL) ||
             (cmsUtl_strstr(buf, "<FAST>") != NULL))
    {
       componentName = BDK_COMP_DSL;
       if (++tagCount > 1)
           goto detect_done;
    }

    if ((cmsUtl_strstr(buf, "bdkConfigIdent=\"voice\"") != NULL) ||
        (cmsUtl_strstr(buf, "<VoiceService>") != NULL))
    {
       componentName = BDK_COMP_VOICE;
       if (++tagCount > 1)
           goto detect_done;
    }

    if ((cmsUtl_strstr(buf, "bdkConfigIdent=\"gpon\"") != NULL) ||
        (cmsUtl_strstr(buf, "<OmciSystemCfg>") != NULL))
    {
       componentName = BDK_COMP_GPON;
       if (++tagCount > 1)
           goto detect_done;
    }

    if ((cmsUtl_strstr(buf, "bdkConfigIdent=\"epon\"") != NULL) ||
        (cmsUtl_strstr(buf, "<X_BROADCOM_COM_EPON>") != NULL))
    {
       componentName = BDK_COMP_EPON;
       if (++tagCount > 1)
           goto detect_done;
    }

    if ((cmsUtl_strstr(buf, "bdkConfigIdent=\"openplat\"") != NULL) ||
        (cmsUtl_strstr(buf, "<SoftwareModules>") != NULL))
    {
       componentName = BDK_COMP_OPENPLAT;
       if (++tagCount > 1)
           goto detect_done;
    }

detect_done:
    if (sectionCount == 0)
    {
        cmsLog_error("Could not find Config File section marker!");
        componentName = errorStr;
    }
    else if ((sectionCount == 1) && (tagCount == 1))
    {
        // This is just one section of a BDK config file, as fed to us by
        // remote_objd.  Do nothing in this block.  componentName is correct.
    }
    else if ((sectionCount == 1) && (tagCount > 1))
    {
        // CMS classic monolithic config file
        componentName = BDK_COMP_CMS_CLASSIC;
    }
    else if (sectionCount > 1)
    {
        // BDK concatentated config file, each component writes out a section
        componentName = "BDK_Concatenated_Config";
    }
    else
    {
       char debugBuf[1000]={0};
       int len=(int)strlen(buf);
       cmsUtl_strncpy(debugBuf, buf, sizeof(debugBuf)-1);
       if ((sectionCount == 1) && (tagCount == 0) && (len < 300))
       {
          // EPON module may have an "empty" config, so we cannot identify it.
          // If it is small and trivial looking (<300 bytes), it could be ok to
          // skip.  See Jira 39943.  A better solution is to put the component
          // name in one of the top level tags.
          printf("Skip trivial and non-identifiable config file (len=%d):\n%s\n\n", len, buf);
       }
       else
       {
          cmsLog_error("Unsupported config buf (sectionCount=%d tagCount=%d len=%d), first %d bytes: %s\n",
                        sectionCount, tagCount, len,
                        sizeof(debugBuf), debugBuf);
          componentName = errorStr;
       }
    }

    cmsLog_debug("Exit: componentName=%s", componentName);
    return componentName;
}


CmsRet sendConfigMsg(const char *imagePtr, UINT32 imageLen, void *msgHandle, CmsMsgType msgType)
{
   char *buf=NULL;
   char *body=NULL;
   CmsMsgHeader *msg;
   CmsRet ret;

   cmsLog_notice("Entered: msgType=0x%x imageLen=%d imagePtr=%p",
                 msgType, imageLen, imagePtr);

   if ((buf = cmsMem_alloc(sizeof(CmsMsgHeader) + imageLen, ALLOC_ZEROIZE)) == NULL)
   {
      cmsLog_error("failed to allocate %d bytes for msg 0x%x", sizeof(CmsMsgHeader) + imageLen, msgType);
      return CMSRET_RESOURCE_EXCEEDED;
   }

   msg = (CmsMsgHeader *) buf;
   body = (char *) (msg + 1);

   msg->type = msgType;
   msg->src = cmsMsg_getHandleEid(msgHandle);
   msg->dst = EID_SMD;
   msg->flags_request = 1;
   msg->dataLength = imageLen;

   memcpy(body, imagePtr, imageLen);

   ret = cmsMsg_sendAndGetReply(msgHandle, msg);

   cmsMem_free(buf);

   cmsLog_notice("Exit: ret=%d", ret);
   return ret;
}


CmsImageFormat cmsImg_ConfigFileValidate(UINT8 *imagePtr, UINT32 imageLen,
  void *usrDataP)
{
   CmsRet ret;

   ret = sendConfigMsg((char*)imagePtr, imageLen, usrDataP,
                       CMS_MSG_VALIDATE_CONFIG_FILE);
   if (ret == CMSRET_SUCCESS)
   {
      return CMS_IMAGE_FORMAT_XML_CFG;
   }

   return CMS_IMAGE_FORMAT_INVALID;
}


int cmsImg_ConfigFileWrite(UINT8 *imagePtr, UINT32 imageLen, void *usrDataP)
{
   CmsRet ret;

   ret = sendConfigMsg((char*)imagePtr, imageLen, usrDataP,
                       CMS_MSG_WRITE_CONFIG_FILE);
   if (ret == CMSRET_SUCCESS)
   {
      return 0;
   }

   return -1;
}


void cmsImg_sendLoadStartingMsg(void *msgHandle, const char *connIfName)
{
   CmsMsgHeader *msg;
   char *data;
   void *msgBuf;
   UINT32 msgDataLen=0;

   /* for the msg and the connIfName */
   if (connIfName)
   {
      msgDataLen = strlen(connIfName) + 1;
      msgBuf = cmsMem_alloc(sizeof(CmsMsgHeader) + msgDataLen, ALLOC_ZEROIZE);
   } 
   else
   {
      cmsLog_error("msg without connIfName");
      msgBuf = cmsMem_alloc(sizeof(CmsMsgHeader), ALLOC_ZEROIZE);
   }
   
   msg = (CmsMsgHeader *)msgBuf;
   msg->src = cmsMsg_getHandleEid(msgHandle);
   msg->dst = EID_SMD;
   msg->flags_request = 1;
   msg->type = CMS_MSG_LOAD_IMAGE_STARTING;

   if (connIfName)
   {
      data = (char *) (msg + 1);
      msg->dataLength = msgDataLen;
      memcpy(data, (char *)connIfName, msgDataLen);      
   }
   
   cmsLog_debug("connIfName=%s", connIfName);

   cmsMsg_sendAndGetReply(msgHandle, msg);

   CMSMEM_FREE_BUF_AND_NULL_PTR(msgBuf);

}


void cmsImg_sendLoadDoneMsg(void *msgHandle)
{
   CmsMsgHeader msg = EMPTY_MSG_HEADER;

   msg.type = CMS_MSG_LOAD_IMAGE_DONE;
   msg.src = cmsMsg_getHandleEid(msgHandle);
   msg.dst = EID_SMD;
   msg.flags_request = 1;
   
   cmsMsg_sendAndGetReply(msgHandle, &msg);
}


/* using a cmsUtil_ prefix because this can be used in non-upload scenarios */
void cmsUtil_sendRequestRebootMsg(void *msgHandle)
{
   CmsMsgHeader msg = EMPTY_MSG_HEADER;

   msg.type = CMS_MSG_REBOOT_SYSTEM;
   msg.src = cmsMsg_getHandleEid(msgHandle);
   msg.dst = EID_SMD;
   msg.flags_request = 1;

   cmsMsg_sendAndGetReply(msgHandle, &msg);
}


CmsRet cmsImg_saveIfNameFromSocket(SINT32 socketfd, char *connIfName)
{
   return (oal_saveIfNameFromSocket(socketfd,connIfName));
}


UBOOL8 cmsImg_isBcmTaggedImage(const char *imageBuf, UINT32 *imageSize)
{
   FILE_TAG *pTag = (FILE_TAG *)imageBuf;
   UINT32 crc;
   UINT32 tokenCrc, *pTokenCrc;
   UBOOL8 isBcmImg = FALSE;
         
   pTokenCrc = (UINT32 *)pTag->tagValidationToken;
   tokenCrc = *pTokenCrc;
#ifdef DESKTOP_LINUX
    /* assume desktop is running on little endien intel, but the CRC has been
     * written for big endien mips, so swap before compare.
     * FIXME! this part of code might not work with Little-endian target system.
     */
    tokenCrc = htonl(tokenCrc);
#endif

   /* check tag validate token to see if it is a valid bcmTag */
   crc = CRC_INITIAL_VALUE;
   crc = genUtl_getCrc32((UINT8 *) pTag, (UINT32)TAG_LEN-TOKEN_LEN, crc);
   if (crc == tokenCrc)
   {
      isBcmImg = TRUE;
      *imageSize = (UINT32) atoi((char *)pTag->totalImageLen) + TAG_LEN;
      cmsLog_debug("It's a broadcom tagged image with length %d", *imageSize );
   }
   else
   {
      cmsLog_debug("token crc failed, this is not a valid broadcom image");
   }

   
   return isBcmImg;
   
}

CmsImageFormat cmsImg_parseImgHdr(UINT8 *bufP, UINT32 bufLen)
{
   CmsImageFormat result = CMS_IMAGE_FORMAT_INVALID;

   if (bufP == NULL)
   {
       cmsLog_debug("NULL bufP pointer.");
       result = CMS_IMAGE_FORMAT_INVALID;
   }
   else if ((bufLen > CMS_CONFIG_FILE_DETECTION_LENGTH) &&
     cmsImg_isConfigFileLikely((char*)bufP))
   {
       cmsLog_debug("Detected possible CMS XML config file format");
       result = CMS_IMAGE_FORMAT_XML_CFG;
   }
#ifdef SUPPORT_MODSW_LINUXPFP
   else if (cmsImg_isModSwLinuxPfp((UINT8*)bufP, bufLen))
   {
       cmsLog_debug("Detected Modular Software Linux PFP format.");
       result = CMS_IMAGE_FORMAT_MODSW_LINUXPFP;
   }
#endif
   else if ((bufLen > sizeof(FILE_TAG)) &&
            (bcmImg_verifyBroadcomFileTag((FILE_TAG*)bufP, 0, bufLen) == CMSRET_SUCCESS))
   {
       cmsLog_debug("Detected Broadcom image format.");
       result = CMS_IMAGE_FORMAT_BROADCOM;
       if (cmsLog_getLevel() == LOG_LEVEL_DEBUG)
       {
           dumpBcmFileTag((FILE_TAG*)bufP);
       }
   }

   return result;
}

/* This function retrieves software version number from an image version string
 * which starts with software version followed by image type, build number and
 * build time info.
 * e.g. given the image version string "5041pre1GW_BEEP_BEEPREINSTALL2131745",
 * the output software version number will be "5.04L.01pre1".
 *
 * The parser is based on the following assumptions:
 * (1) Software version string is made up of digits and/or lowercase letters,
 *    e.g. "5041pre1", and is terminated by the uppercase image type letter,
 *    e.g. G, O, H, B, D.
 * (2) The first single digit is the major version number, e.g. "5"
 * (3) The second and third digits are the minor version number, e.g. "04"
 * (4) The remaining characters are the build number suffix, e.g. "1pre1"
 */
void cmsImg_parseImageVersionForSwVersion(const char *imageVersion,
                                          char *swVersion, SINT32 len)
{
   char buf[BUFLEN_256+1] = {0};
   SINT32 length, i, j;

   length = strlen(imageVersion);

   for (i = 0, j = 0; i < length; i++)
   {
      /* Software version string is terminated by the uppercase image type
       * letter.
       */
      if (isupper(imageVersion[i]))
         break;

      buf[j++] = imageVersion[i];
      if (i == 0)
      {
         buf[j++] = '.';   /* major version delimiter */
      }
      else if (i == 2)
      {
         buf[j++] = 'L';   /* L for linux */
         buf[j++] = '.';   /* minor version number delimiter */

         /* The forth character shall be the build number. If the fifth
          * character is also a digit, then the forth and the fifth
          * characters are the build number. Otherwise, insert '0'
          * before the forth character to make the two-digit build number.
          */
         if (!isdigit(imageVersion[4]))
            buf[j++] = '0';
      }
   }
   buf[j] = '\0';

   cmsUtl_strncpy(swVersion, buf, len);

   cmsLog_debug("swVersion=%s", swVersion);
}

CmsRet cmsImg_getIdent(SINT32 part, char *key, char *value, SINT32 len)
{
   char keystr[BUFLEN_256+1] = {0};
   char *p1, *p2;
   int start = 0, end = 50;

   *value = '\0';

   cmsLog_debug("Enter: part=%d key=%s", part, key);

   if (IS_EMPTY_STRING(key))
   {
      cmsLog_error("key not specified");
      return CMSRET_INVALID_ARGUMENTS;
   }

   if (bcmFlash_getIdent(part, &start, &end, key, keystr, sizeof(keystr)-1) <= 0)
   {
      cmsLog_notice("key %s not found", key);
      return CMSRET_INVALID_ARGUMENTS;
   }
   cmsLog_debug("keystr=%s", keystr);

   /* e.g. key="imageversion" --> keystr="$imageversion: 5041GW_PURE1812211109 $"
    *      key="imgversion"   --> keystr="$imgversion: 5.04L.01 $"
    */
   if ((p1 = strchr(keystr, ':')) == NULL)
   {
      cmsLog_error("Invalid key string %s", keystr);
      return CMSRET_INTERNAL_ERROR;
   }

   p1++;
   while (*p1 != '\0' && isspace(*p1))
      p1++;

   p2 = p1;
   while (*p2 != '\0' && *p2 != '$' && !isspace(*p2))
      p2++;
   *p2 = '\0';

   cmsUtl_strncpy(value, p1, len);

   cmsLog_debug("value=%s", value);

   return CMSRET_SUCCESS;
}

CmsRet cmsImg_getFirmwareNameVersion(SINT32 part,
                                     char *fullVersionBuf,
                                     char *shortVersionBuf)
{
   CmsRet ret;

   if (fullVersionBuf == NULL)
   {
      cmsLog_error("fullVersionBuf must be provided");
      return CMSRET_INVALID_ARGUMENTS;
   }

   ret = cmsImg_getIdent(part, "imageversion", fullVersionBuf, IMAGE_VERSION_TAG_SIZE);
   if (ret != CMSRET_SUCCESS)
   {
      return ret;
   }

   // If not provided, then we are done.
   if (shortVersionBuf == NULL)
      return ret;

   cmsImg_getIdent(part, "imgversion", shortVersionBuf, IMAGE_VERSION_TAG_SIZE);
   if (IS_EMPTY_STRING(shortVersionBuf))
   {
      /* "imgversion" key may not exist in older version of firmware image.
       * As a workaround, parse for software version from the "imageversion"
       * key string.
       */  
      cmsImg_parseImageVersionForSwVersion(fullVersionBuf,
                                   shortVersionBuf, IMAGE_VERSION_TAG_SIZE);
   }

   return ret;
}



static void dumpBcmFileTag(FILE_TAG *tagP)
{
    printf("--------------------------------------------------------------\n");
    printf("Broadcom image tag:\n");
    printf("\ttagVersion: %02x %02x %02x %02x\n",
      tagP->tagVersion[0], tagP->tagVersion[1],
      tagP->tagVersion[2], tagP->tagVersion[3]);
    printf("\tendian: %s\n", tagP->bigEndian);
    printf("\ttotalImageLen: %s\n", tagP->totalImageLen);
    printf("\tcfeAddress: %s\n", tagP->cfeAddress);
    printf("\tcfeLen: %s\n", tagP->cfeLen);
    printf("\trootfsAddress: %s\n", tagP->rootfsAddress);
    printf("\trootfsLen: %s\n", tagP->rootfsLen);
    printf("\tkernelAddress: %s\n", tagP->kernelAddress);
    printf("\tkernelLen: %s\n", tagP->kernelLen);
    printf("\timageSequence: %s\n", tagP->imageSequence);
    printf("\timageVersion: %s\n", tagP->imageVersion);
    printf("--------------------------------------------------------------\n");
}

#ifdef SUPPORT_TR69C_AUTONOMOUS_TRANSFER_COMPLETE

CmsRet cmsImg_storeImageTransferStats(const CmsImageTransferStats *pStats)
{
   CmsRet ret = CMSRET_SUCCESS;

   if (pStats == NULL)
   {
      ret = cmsPsp_set(CMS_IMAGE_TRANSFER_STATS_PSP_KEY, NULL, 0);
   }
   else if ((ret = cmsPsp_set(CMS_IMAGE_TRANSFER_STATS_PSP_KEY, pStats, sizeof(CmsImageTransferStats))) != CMSRET_SUCCESS)
   {
      cmsLog_error("set of CMS_IMAGE_TRANSFER_STATS_PSP_KEY failed, ret=%d", ret);
   }
   return (ret);
}

CmsRet cmsImg_clearImageTransferStats(void)
{
   return(cmsPsp_set(CMS_IMAGE_TRANSFER_STATS_PSP_KEY, NULL, 0));
}

CmsRet cmsImg_getImageTransferStats(CmsImageTransferStats *pStats)
{
   int byteRead = 0;
   CmsRet ret = CMSRET_INTERNAL_ERROR;

   if (pStats != NULL)
   {
      byteRead = cmsPsp_get(CMS_IMAGE_TRANSFER_STATS_PSP_KEY, pStats, sizeof(CmsImageTransferStats));
   }
   if (byteRead > 0)
   {
      ret = CMSRET_SUCCESS;
   }
   return (ret);
}

void cmsImg_sendAutonomousTransferCompleteMsg(void *msgHandle, const CmsImageTransferStats *pStats)
{
   char buf[sizeof(CmsMsgHeader) + sizeof(AutonomousTransferCompleteMsgBody)]={0};
   CmsMsgHeader *msg = (CmsMsgHeader *) buf; 
   AutonomousTransferCompleteMsgBody *body = (AutonomousTransferCompleteMsgBody*)(msg+1);

   msg->type = CMS_MSG_AUTONOMOUS_TRANSFER_COMPLETE;
   msg->src = cmsMsg_getHandleEid(msgHandle);
   msg->dst = EID_TR69C;
   msg->flags_event = 1;
   msg->dataLength = sizeof(AutonomousTransferCompleteMsgBody);

   body->isDownload = pStats->isDownload;
   body->fileType = pStats->fileType;
   body->fileSize = pStats->fileSize;
   body->faultCode = pStats->faultCode;
   strcpy(body->faultStr,pStats->faultStr);
   body->startTime = pStats->startTime;
   body->completeTime = pStats->completeTime;
   
   cmsMsg_send(msgHandle, msg);
}

#endif /* SUPPORT_TR69C_AUTONOMOUS_TRANSFER_COMPLETE */
#endif /* BRCM_CMS_BUILD || BRCM_BDK_BUILD  */
