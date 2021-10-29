/***********************************************************************
 *
 *  Copyright (c) 2006-2007  Broadcom Corporation
 *  All Rights Reserved
 *
 * <:label-BRCM:2006:DUAL/GPL:standard
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


#include "cms.h"
#include "bdk.h"
#include "cms_mem.h"
#include "cms_msg.h"
#include "cms_log.h"
#include "cms_fil.h"
#include "cms_tms.h"
#include "oal.h"

#ifdef DESKTOP_LINUX
UINT16 desktopFakePid = 30;
#endif


#define CMS_MSG_PUTBACK_Q_IS_EMPTY(h) (((CmsMsgHandle *) (h))->putBackQueue == NULL)


/* message API functions go here */

CmsRet cmsMsg_initOnBus(CmsEntityId eid, UINT32 flags, const char *busName,
                        void **msgHandle)
{
   return oalMsg_initOnBus(eid, flags, busName, msgHandle);
}

CmsRet cmsMsg_initWithFlags(CmsEntityId eid, UINT32 flags, void **msgHandle)
{
   return cmsMsg_initOnBus(eid, flags, SMD_MESSAGE_ADDR, msgHandle);
}

CmsRet cmsMsg_init(CmsEntityId eid, void **msgHandle)
{
   const CmsEntityInfo *eInfo;

   if ((eInfo = cmsEid_getEntityInfo(eid)) == NULL)
   {
      cmsLog_error("Invalid eid %d", eid);
      return CMSRET_INVALID_ARGUMENTS;
   }

   return cmsMsg_initWithFlags(eid,
                               eInfo->flags & (EIF_MULTIPLE_INSTANCES|EIF_MULTIPLE_THREADS),
                               msgHandle);
}

void cmsMsg_cleanup(void **msgHandle)
{
   CmsMsgHandle *handle = (CmsMsgHandle *) *msgHandle;
   CmsMsgHeader *msg;

   /* free any queued up messages */
   while ((msg = handle->putBackQueue) != NULL)
   {
      handle->putBackQueue = (CmsMsgHeader *) msg->next;
      CMSMEM_FREE_BUF_AND_NULL_PTR(msg);
   }

   return oalMsg_cleanup(msgHandle);
}


CmsRet cmsMsg_send(void *msgHandle, const CmsMsgHeader *buf)
{
   CmsMsgHandle *handle = (CmsMsgHandle *) msgHandle;

#ifdef DESKTOP_LINUX
   if (handle->standalone)
   {
      /* just pretend to have sent the message */
      return CMSRET_SUCCESS;
   }
#endif

   return oalMsg_send(handle->commFd, buf);
}


CmsRet cmsMsg_sendReply(void *msgHandle, const CmsMsgHeader *msg, CmsRet retCode)
{
   CmsMsgHandle *handle = (CmsMsgHandle *) msgHandle;
   CmsMsgHeader replyMsg = EMPTY_MSG_HEADER;

   replyMsg.dst = msg->src;
   replyMsg.src = msg->dst;
   replyMsg.type = msg->type;

   replyMsg.flags_request = 0;
   replyMsg.flags_response = 1;
   replyMsg.flags_bounceIfNotRunning = msg->flags_bounceIfNotRunning;
   /* do we want to copy any other flags? */

   replyMsg.wordData = retCode;

   return oalMsg_send(handle->commFd, &replyMsg);
}


static void updateReceiveTimeout(const CmsTimestamp *startTs, UINT32 *timeout)
{
   if (timeout != NULL)
   {
      CmsTimestamp nowTs;
      UINT32 dec;
      UINT32 min=10;  // decrement a min of 10ms per call to ensure decrease.

      cmsTms_get(&nowTs);
      dec = cmsTms_deltaInMilliSeconds(&nowTs, startTs);
      dec = (dec < min) ? min : dec;
      if (*timeout <= dec)
         *timeout = 1;  // avoid 0, which might mean block indefinately.
      else
         *timeout -= dec;
   }
   return;
}



static CmsRet getReplyBuf(CmsMsgHandle *msgHandle, const CmsMsgHeader *req,
                          CmsMsgHeader **replyBuf, UINT32 *timeout)
{
   CmsMsgHeader *replyMsg = NULL;
   UBOOL8 doReceive = TRUE;
   CmsMsgType sentType;
   CmsEntityId sentDst;
   CmsRet ret = CMSRET_SUCCESS;

   sentType = req->type;
   sentDst = req->dst;

   while (doReceive)
   {
      CmsTimestamp startTs;
      cmsTms_get(&startTs);

      ret = oalMsg_receive(msgHandle->commFd, &replyMsg, timeout);
      if (ret != CMSRET_SUCCESS)
      {
         if ((timeout == NULL) ||
             ((timeout != NULL) && (ret != CMSRET_TIMED_OUT)))
         {
            cmsLog_error("error during get of reply, ret=%d", ret);
         }
         cmsMem_free(replyMsg);
         doReceive = FALSE;
      }
      else
      {
         if ((replyMsg->type == sentType) &&
             (replyMsg->src == sentDst) &&
             (replyMsg->flags_response))
         {
            if (*replyBuf != NULL)
            {
               // This is the older, dangerous interface.  Caller assumes he
               // knows the length of reply msg.  We just blindly copy the
               // reply into the given buffer.
               memcpy((*replyBuf), replyMsg, (sizeof(CmsMsgHeader) + replyMsg->dataLength));
               CMSMEM_FREE_BUF_AND_NULL_PTR(replyMsg);
            }
            else
            {
               // This is the newer, safe interface.  We give our allocated
               // response buffer to the caller.  Caller is responsible for
               // freeing it.
               *replyBuf = replyMsg;
            }
            doReceive = FALSE;
         }
         else if (replyMsg->type == CMS_MSG_INTERNAL_NOOP)
         {
            // Do not requeue this message.  Just drop it.  We will generate
            // another one at the bottom of this function if needed.
            updateReceiveTimeout(&startTs, timeout);
            CMSMEM_FREE_BUF_AND_NULL_PTR(replyMsg);
         }
         else
         {
            /* we got a mesage, but it was not the reply we were expecting.
             * Could be an event msg.  Push it back on the put-back queue and
             * keep trying to get the message we really want.
             */
            cmsLog_debug("Not the msg we wanted (0x%x from %d), put back 0x%x from %d (%d/%d/%d)",
                         sentType, sentDst, replyMsg->type, replyMsg->src,
                         replyMsg->flags_request, replyMsg->flags_response,
                         replyMsg->flags_event);
            cmsMsg_putBack(msgHandle, &replyMsg);
            replyMsg = NULL;
            updateReceiveTimeout(&startTs, timeout);
         }
      }
   }

   if (!CMS_MSG_PUTBACK_Q_IS_EMPTY(msgHandle))
   {
      cmsMsg_sendNoop(msgHandle);
   }

   return ret;
}

static CmsRet sendAndGetReply(CmsMsgHandle *msgHandle, const CmsMsgHeader *buf, UINT32 *timeout)
{
   CmsMsgHeader *replyMsg = NULL;
   CmsRet ret;

#ifdef DESKTOP_LINUX
   if (msgHandle->standalone)
   {
      CmsMsgHeader *msg = (CmsMsgHeader *) buf;

      /*
       * Standalone mode occurs during unittests.
       * Pretend to send out the message and get a successful reply.
       */
      if ((msg->type == CMS_MSG_START_APP) || (msg->type == CMS_MSG_RESTART_APP))
      {
         /* For the START_APP and RESTART_APP messages, the expected return value is the pid. */
         return desktopFakePid++;
      }
      else
      {
         return CMSRET_SUCCESS;
      }
   }
#endif

   ret = oalMsg_send(msgHandle->commFd, buf);
   if (ret != CMSRET_SUCCESS)
   {
      return ret;
   }

   ret = getReplyBuf(msgHandle, buf, &replyMsg, timeout);
   if (ret == CMSRET_SUCCESS)
   {
       ret = replyMsg->wordData;
       CMSMEM_FREE_BUF_AND_NULL_PTR(replyMsg);
   }

   return ret;
}

static CmsRet sendAndGetReplyBuf(CmsMsgHandle *msgHandle, const CmsMsgHeader *req,
                                CmsMsgHeader **replyBuf, UINT32 *timeout)
{
   CmsRet ret;

   ret = oalMsg_send(msgHandle->commFd, req);
   if (ret != CMSRET_SUCCESS)
   {
      return ret;
   }

   ret = getReplyBuf(msgHandle, req, replyBuf, timeout);

   return ret;
}


CmsRet cmsMsg_sendAndGetReply(void *msgHandle, const CmsMsgHeader *buf)
{
   return sendAndGetReply((CmsMsgHandle *)msgHandle, buf, NULL);
}


CmsRet cmsMsg_sendAndGetReplyWithTimeout(void *msgHandle,
                                         const CmsMsgHeader *buf,
                                         UINT32 timeoutMilliSeconds)
{
   UINT32 timeout = timeoutMilliSeconds;
   return sendAndGetReply((CmsMsgHandle *)msgHandle, buf, &timeout);
}

CmsRet cmsMsg_sendAndGetReplyBuf(void *msgHandle, const CmsMsgHeader *buf, CmsMsgHeader **replyBuf)
{
   return sendAndGetReplyBuf((CmsMsgHandle *)msgHandle, buf, replyBuf, NULL);
}

CmsRet cmsMsg_sendAndGetReplyBufWithTimeout(void *msgHandle, const CmsMsgHeader *buf,
                    CmsMsgHeader **replyBuf, UINT32 timeoutMilliSeconds)
{
   UINT32 timeout = timeoutMilliSeconds;

   return sendAndGetReplyBuf((CmsMsgHandle *)msgHandle, buf, replyBuf, &timeout);
}

/* This api should only be called after the requset CMS message has been sent.
 * It is intended to provide higher level of control for the calling process.
 * For example, if the calling process wants to enforce sequenceNumber in replyBuf
 * should match the sequenceNumber in request. Since "sequenceNumber" is optional
 * field in CmsMsgHeader, the check for its match is done outside of cms msg library itself.
 */
CmsRet cmsMsg_getReplyBufWithTimeout(void *msgHandle, const CmsMsgHeader *buf,
                          CmsMsgHeader **replyBuf, UINT32 timeoutMs)
{
    return getReplyBuf((CmsMsgHandle *)msgHandle, buf, replyBuf, &timeoutMs);
}



static CmsRet getPutbackMsg(void *msgHandle, CmsMsgHeader **buf)
{
   CmsRet ret = CMSRET_NO_MORE_INSTANCES;

   if (buf == NULL)
   {
      cmsLog_error("buf is NULL!");
      return CMSRET_INVALID_ARGUMENTS;
   }
   else
   {
      *buf = NULL;
   }

   if (!CMS_MSG_PUTBACK_Q_IS_EMPTY(msgHandle))
   {
      CmsMsgHandle *handle = (CmsMsgHandle *) msgHandle;

      *buf = handle->putBackQueue;
      handle->putBackQueue = (CmsMsgHeader *) (*buf)->next;
      (*buf)->next = 0L;

      ret = CMSRET_SUCCESS;
   }

   return ret;
}


CmsRet cmsMsg_receive(void *msgHandle, CmsMsgHeader **buf)
{
   CmsMsgHandle *handle = (CmsMsgHandle *) msgHandle;
   CmsRet ret;

#ifdef DESKTOP_LINUX
   if (handle->standalone)
   {
      /*
       * Hmm, this is a tricky situation.  Caller has told us to block until
       * we get a message, but since smd is not running, we will never get
       * a message.  Return INTERNAL_ERROR and let caller handle it?
       */
      cmsLog_error("cannot receive msg while in standalone (unittest) mode");
      *buf = NULL;
      return CMSRET_INTERNAL_ERROR;
   }
#endif

   /*
    * First check for any messages queued in the putback queue.  If nothing
    * there, then go to the real socket to read.
    */
   ret = getPutbackMsg(msgHandle, buf);
   if (ret == CMSRET_SUCCESS || ret == CMSRET_INVALID_ARGUMENTS)
   {
      return ret;
   }

   return oalMsg_receive(handle->commFd, buf, NULL);
}


CmsRet cmsMsg_receiveWithTimeout(void *msgHandle, CmsMsgHeader **buf, UINT32 timeoutMilliSeconds)
{
   CmsMsgHandle *handle = (CmsMsgHandle *) msgHandle;
   UINT32 timeout = timeoutMilliSeconds;
   CmsRet ret;

#ifdef DESKTOP_LINUX
   if (handle->standalone)
   {
      *buf = NULL;
      return CMSRET_TIMED_OUT;
   }
#endif

   /*
    * First check for any messages queued in the putback queue.  If nothing
    * there, then go to the real socket to read.
    */
   ret = getPutbackMsg(msgHandle, buf);
   if (ret == CMSRET_SUCCESS || ret == CMSRET_INVALID_ARGUMENTS)
   {
      return ret;
   }

   return oalMsg_receive(handle->commFd, buf, &timeout);
}


CmsRet cmsMsg_getEventHandle(const void *msgHandle, void *eventHandle)
{
   return (oalMsg_getEventHandle((CmsMsgHandle *) msgHandle, eventHandle));
}


CmsEntityId cmsMsg_getHandleEid(const void *msgHandle)
{
   const CmsMsgHandle *handle = (const CmsMsgHandle *) msgHandle;

   return (handle == NULL ? 0 : handle->eid);
}


const char *cmsMsg_getBusName(const void *msgHandle)
{
   const CmsMsgHandle *handle = (const CmsMsgHandle *) msgHandle;

   return (handle == NULL ? NULL : handle->busName);
}

CmsMsgHeader *cmsMsg_duplicate(const CmsMsgHeader *msg)
{
   UINT32 totalLen;
   void *newMsg;

   totalLen = sizeof(CmsMsgHeader) + msg->dataLength;
   newMsg = cmsMem_alloc(totalLen, 0);
   if (newMsg != NULL)
   {
      memcpy(newMsg, msg, totalLen);
   }

   return newMsg;
}


void cmsMsg_putBack(void *msgHandle, CmsMsgHeader **buf)
{
   CmsMsgHandle *handle = (CmsMsgHandle *) msgHandle;
   CmsMsgHeader *prevMsg;
   UINT32 len=1;

   (*buf)->next = 0L;

   /* put the new message at the end of the putBackQueue */
   if (handle->putBackQueue == NULL)
   {
      handle->putBackQueue = (*buf);
   }
   else
   {
      prevMsg = handle->putBackQueue;
      while (prevMsg->next != 0L)
      {
         prevMsg = (CmsMsgHeader *) prevMsg->next;
         len++;
      }

      prevMsg->next = (uint64_t)(*buf);
   }

   if (len > 1000)
      cmsLog_error("Putback queue is now %d, something wrong?", len);

   /* we've taken ownership of this msg, so null out caller's pointer */
   *buf = NULL;
   return;
}


void cmsMsg_sendNoop(void *msgHandle)
{
   CmsMsgHandle *handle = (CmsMsgHandle *) msgHandle;
   CmsMsgHeader noopMsg = EMPTY_MSG_HEADER;

   noopMsg.dst = handle->putBackQueue->dst;
   noopMsg.src = handle->putBackQueue->dst;
   noopMsg.type = CMS_MSG_INTERNAL_NOOP;

   noopMsg.flags_event = 1;

   oalMsg_send(handle->commFd, &noopMsg);

   return;
}


UBOOL8 cmsMsg_isServiceReady(void)
{
   return cmsFil_isFilePresent(SMD_MESSAGE_ADDR);
}


SINT32 cmsMsg_initUnixDomainServerSocket(const char *msgBusName, SINT32 backlog)
{
   return oalMsg_initUnixDomainServerSocket(msgBusName, backlog);
}


typedef struct {
   const char *compName;
   const char *busName;
} CompNameToBusName;

CompNameToBusName compNameToBusNameTable[] =
{
   {BDK_COMP_DEVINFO,  DEVINFO_MSG_BUS},
   {BDK_COMP_DSL,      DSL_MSG_BUS},
   {BDK_COMP_GPON,     GPON_MSG_BUS},
   {BDK_COMP_EPON,     EPON_MSG_BUS},
   {BDK_COMP_WIFI,     WIFI_MSG_BUS},
   {BDK_COMP_VOICE,    VOICE_MSG_BUS},
   {BDK_COMP_STORAGE,  STORAGE_MSG_BUS},
   {BDK_COMP_TR69,     TR69_MSG_BUS},
   {BDK_COMP_USP,      USP_MSG_BUS},
   {BDK_COMP_SYSMGMT,  SYSMGMT_MSG_BUS},
   {BDK_COMP_OPENPLAT, OPENPLAT_MSG_BUS},
   {"sys_directory",   SYS_DIRECTORY_MSG_BUS},
};

const char *cmsMsg_componentNameToBusName(const char *compName)
{
   UINT32 i;

   for (i=0; i < sizeof(compNameToBusNameTable)/sizeof(CompNameToBusName); i++)
   {
      if (!strcasecmp(compNameToBusNameTable[i].compName, compName))
      {
         return compNameToBusNameTable[i].busName;
      }
   }

   return NULL;
}

const char *cmsMsg_busNameToComponentName(const char *busName)
{
   UINT32 i;

   for (i=0; i < sizeof(compNameToBusNameTable)/sizeof(CompNameToBusName); i++)
   {
      if (!strcasecmp(compNameToBusNameTable[i].busName, busName))
      {
         return compNameToBusNameTable[i].compName;
      }
   }

   return NULL;
}
