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


/* OS dependent messaging functions go here */

#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <errno.h>
#include "../oal.h"
#include "cms_util.h"
#include "bcm_ulog.h"


CmsRet oalMsg_initOnBus(CmsEntityId eid, UINT32 flags, const char *busName,
                        void **msgHandle)
{
   CmsMsgHandle *handle;
   struct sockaddr_un serverAddr;
   SINT32 rc;

   
   if ((handle = (CmsMsgHandle *) cmsMem_alloc(sizeof(CmsMsgHandle), ALLOC_ZEROIZE)) == NULL)
   {
      cmsLog_error("could not allocate storage for msg handle");
      return CMSRET_RESOURCE_EXCEEDED;
   }

   /* store caller's eid */
   handle->eid = eid;
   strncpy(handle->busName, busName, sizeof(handle->busName)-1);

#ifdef DESKTOP_LINUX
   /*
    * Applications may be run without smd on desktop linux, so if we
    * don't see a socket for smd, don't bother connecting to it.
    * Hmm, is this feature actually used? (for unittests?)  Delete?
    */
   {
      struct stat statbuf;

      if ((rc = stat(handle->busName, &statbuf)) < 0)
      {
         handle->commFd = CMS_INVALID_FD;
         handle->standalone = TRUE;
         *msgHandle = (void *) handle;
         cmsLog_notice("no smd server socket detected, running in standalone mode.");
         return CMSRET_SUCCESS;
      }
   }
#endif /* DESKTOP_LINUX */


      /*
       * Create a unix domain socket.
       */
      handle->commFd = socket(AF_LOCAL, SOCK_STREAM, 0);
      if (handle->commFd < 0)
      {
         cmsLog_error("Could not create socket");
         cmsMem_free(handle);
         return CMSRET_INTERNAL_ERROR;
      }


      /*
       * Set close-on-exec, even though all apps should close their
       * fd's before fork and exec.
       */
      if ((rc = fcntl(handle->commFd, F_SETFD, FD_CLOEXEC)) != 0)
      {
         cmsLog_error("set close-on-exec failed, rc=%d errno=%d", rc, errno);
         close(handle->commFd);
         cmsMem_free(handle);
         return CMSRET_INTERNAL_ERROR;
      }


      /*
       * Connect to the message router.  In CMS classic, this is smd.  In BDK,
       * this will be the bcm_msgd for this component.
       */
      memset(&serverAddr, 0, sizeof(serverAddr));
      serverAddr.sun_family = AF_LOCAL;
      strncpy(serverAddr.sun_path, handle->busName, sizeof(serverAddr.sun_path));

      rc = connect(handle->commFd, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
      if (rc != 0)
      {
         // in BDK, the MD's will start bcm_msgd and immediately try to connect
         // to it.  The first attempt may fail, but the MD's will retry, so
         // don't complain too loudly here.
         cmsLog_debug("connect to %s failed, rc=%d errno=%d", handle->busName, rc, errno);
         close(handle->commFd);
         cmsMem_free(handle);
         return CMSRET_INTERNAL_ERROR;
      }
      else
      {
         cmsLog_debug("commFd=%d connected to smd", handle->commFd);
      }

      /* send a launched message to smd */
      {
         CmsRet ret;
         CmsMsgHeader launchMsg = EMPTY_MSG_HEADER;

         launchMsg.type = CMS_MSG_APP_LAUNCHED;

         if (flags & EIF_MULTIPLE_INSTANCES)
         {
            /* this entity can have multiple instances. Each instance may
             * have multiple threads. Set msg src to specific eid that
             * includes pid/tid.
             * Note that gettid returns the caller's thread id. In a
             * single-threaded process, the thread id is equal to the
             * process id as returned by getpid. In a multithreaded process,
             * all threads have the same pid, but each one has a uniqueue tid.
             */
            launchMsg.src = MAKE_SPECIFIC_EID(syscall(SYS_gettid), eid);
         }
         else if (flags & EIF_MULTIPLE_THREADS)
         {
            /* this is a single instance entity with multiple threads. */
            if (getpid() == syscall(SYS_gettid))
            {
               /* the caller is the main (root) thread.
                * set msg src to eid.
                */
               launchMsg.src = eid;
            }
            else
            {
               /* set msg src to specific eid that includes tid. */
               launchMsg.src = MAKE_SPECIFIC_EID(syscall(SYS_gettid), eid);
            }
         }
         else
         {
            /* this is a single instance, single thread entity.
             * set msg src to eid.
             */
            launchMsg.src = eid;
         }
         launchMsg.dst = EID_SMD;
         launchMsg.flags_event = 1;
         launchMsg.wordData = getpid();

         if ((ret = oalMsg_send(handle->commFd, &launchMsg)) != CMSRET_SUCCESS)
         {
            close(handle->commFd);
            cmsMem_free(handle);
            return CMSRET_INTERNAL_ERROR;
         }
         else
         {
            cmsLog_debug("sent LAUNCHED message to smd");
         }
      }

   /* successful, set handle pointer */
   *msgHandle = (void *) handle;

   return CMSRET_SUCCESS;
}


void oalMsg_cleanup(void **msgHandle)
{
   CmsMsgHandle *handle = (CmsMsgHandle *) *msgHandle;

   if (handle->commFd != CMS_INVALID_FD)
   {
      close(handle->commFd);
   }

   CMSMEM_FREE_BUF_AND_NULL_PTR((*msgHandle));

   return;
}


CmsRet oalMsg_getEventHandle(const CmsMsgHandle *msgHandle, void *eventHandle)
{
   SINT32 *fdPtr = (SINT32 *) eventHandle;

   *fdPtr = msgHandle->commFd;

   return CMSRET_SUCCESS;
}


CmsRet oalMsg_send(SINT32 fd, const CmsMsgHeader *buf)
{
   UINT32 totalLen;
   SINT32 rc;
   CmsRet ret=CMSRET_SUCCESS;

   totalLen = sizeof(CmsMsgHeader) + buf->dataLength;

   rc = write(fd, buf, totalLen);
   if (rc < 0)
   {
      if (errno == EPIPE)
      {
         /*
          * This could happen when smd tries to write to an app that
          * has exited.  Don't print out a scary error message.
          * Just return an error code and let upper layer app handle it.
          */
         cmsLog_debug("got EPIPE, dest app is dead");
         return CMSRET_DISCONNECTED;
      }
      else
      {
         cmsLog_error("write failed, errno=%d", errno);
         ret = CMSRET_INTERNAL_ERROR;
      }
   }
   else if (rc != (SINT32) totalLen)
   {
      cmsLog_error("unexpected rc %d, expected %u", rc, totalLen);
      ret = CMSRET_INTERNAL_ERROR;
   }

   return ret;
}


static CmsRet waitForDataAvailable(SINT32 fd, UINT32 timeout)
{
   struct timeval tv;
   fd_set readFds;
   SINT32 rc;

   FD_ZERO(&readFds);
   FD_SET(fd, &readFds);

   tv.tv_sec = timeout / MSECS_IN_SEC;
   tv.tv_usec = (timeout % MSECS_IN_SEC) * USECS_IN_MSEC;

   rc = select(fd+1, &readFds, NULL, NULL, &tv);
   if ((rc == 1) && (FD_ISSET(fd, &readFds)))
   {
      return CMSRET_SUCCESS;
   }
   else
   {
      return CMSRET_TIMED_OUT;
   }
}


CmsRet oalMsg_receive(SINT32 fd, CmsMsgHeader **buf, UINT32 *timeout)
{
   CmsMsgHeader *msg;
   SINT32 rc;
   CmsRet ret;

   if (buf == NULL)
   {
      cmsLog_error("buf is NULL!");
      return CMSRET_INVALID_ARGUMENTS;
   }
   else
   {
      *buf = NULL;
   }

   if (timeout)
   {
      if ((ret = waitForDataAvailable(fd, *timeout)) != CMSRET_SUCCESS)
      {
         return ret;
      }
   }

   /*
    * Read just the header in the first read.
    * Do not try to read more because we might get part of 
    * another message in the TCP socket.
    */
   msg = (CmsMsgHeader *) cmsMem_alloc(sizeof(CmsMsgHeader), ALLOC_ZEROIZE);
   if (msg == NULL)
   {
      cmsLog_error("alloc of msg header failed");
      return CMSRET_RESOURCE_EXCEEDED;
   }

   rc = read(fd, msg, sizeof(CmsMsgHeader));
   if ((rc == 0) ||
       ((rc == -1) && ((errno == 131) || (errno == ECONNRESET))))  /* new 2.6.21 kernel seems to give us this before rc==0 */
   {
      /* broken connection */
      cmsMem_free(msg);
      return CMSRET_DISCONNECTED;
   }
   else if (rc < 0 || rc != sizeof(CmsMsgHeader))
   {
      cmsLog_error("bad read, rc=%d errno=%d", rc, errno);
      cmsMem_free(msg);
      return CMSRET_INTERNAL_ERROR;
   }

   if (msg->dataLength > 0)
   {
      UINT32 totalReadSoFar=0;
      SINT32 totalRemaining=msg->dataLength;
      char *inBuf;
      CmsMsgHeader *newMsg;

      /* there is additional data in the message */
      newMsg = (CmsMsgHeader *) cmsMem_realloc(msg, sizeof(CmsMsgHeader) + msg->dataLength);
      if (newMsg == NULL)
      {
         cmsLog_error("realloc to %d bytes failed", sizeof(CmsMsgHeader) + msg->dataLength);
         cmsMem_free(msg);
         return CMSRET_RESOURCE_EXCEEDED;
      }

      /* orig msg was freed by cmsMem_realloc, so now msg can point to newMsg */
      msg = newMsg;
      newMsg = NULL;

      inBuf = (char *) (msg + 1);
      while (totalReadSoFar < msg->dataLength)
      {
         cmsLog_debug("reading segment: soFar=%u total=%d", totalReadSoFar, totalRemaining);
         if (timeout)
         {
            if ((ret = waitForDataAvailable(fd, *timeout)) != CMSRET_SUCCESS)
            {
               cmsMem_free(msg);
               return ret;
            }
         }

         rc = read(fd, inBuf, totalRemaining);
         if (rc <= 0)
         {
            cmsLog_error("bad data read, rc=%d errno=%d readSoFar=%d remaining=%d", rc, errno, totalReadSoFar, totalRemaining);
            cmsMem_free(msg);
            return CMSRET_INTERNAL_ERROR;
         }
         else
         {
            inBuf += rc;
            totalReadSoFar += rc;
            totalRemaining -= rc;
         }
      }
   }

   *buf = msg;

   return CMSRET_SUCCESS;
}


SINT32 oalMsg_initUnixDomainServerSocket(const char *msgBusName, SINT32 backlog)
{
   struct sockaddr_un serverAddr;
   SINT32 fd, rc;

   // Delete sockName in case previous instance exited without cleanup.
   // TODO: small danger another one is actually running.  Quick check? 
   unlink(msgBusName);

   if ((fd = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0)
   {
      bcmuLog_error("Could not create socket");
      return fd;
   }

   memset(&serverAddr, 0, sizeof(serverAddr));
   serverAddr.sun_family = AF_LOCAL;
   strncpy(serverAddr.sun_path, msgBusName, sizeof(serverAddr.sun_path));

   rc = bind(fd, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
   if (rc != 0)
   {
      bcmuLog_error("bind to %s failed, rc=%d errno=%d", msgBusName, rc, errno);
      close(fd);
      return -1;
   }

   rc = listen(fd, backlog);
   if (rc != 0)
   {
      bcmuLog_error("listen to %s failed, rc=%d errno=%d", msgBusName, rc, errno);
      close(fd);
      return -1;
   }

   bcmuLog_notice("message bus socket (%s) opened (fd=%d)", msgBusName, fd);

   return fd;
}
