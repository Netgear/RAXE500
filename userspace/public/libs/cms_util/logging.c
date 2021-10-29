/***********************************************************************
 *
<:copyright-BRCM:2006:DUAL/GPL:standard

   Copyright (c) 2006 Broadcom 
   All Rights Reserved

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


#include <fcntl.h>      /* open */
#include "cms.h"
#include "cms_log.h"
#include "cms_eid.h"
#include "cms_strconv.h"
#include "cms_mem.h"
#include "oal.h"
#include "bcm_ulog.h"
#include "sysutil.h"

/** local definitions **/

/* default settings */

/** external functions **/

/** external data **/

/** internal functions **/

/** public data **/

/** private data **/

/** CMS Log will now use the state variables in BCM uLog */


UINT32 cmsLog_dest_to_mask(CmsLogDestination cmsLogDestination)
{
   if (cmsLogDestination == LOG_DEST_STDERR)
   {
      return BCMULOG_DESTMASK_STDERR;
   }
   else if (cmsLogDestination == LOG_DEST_SYSLOG)
   {
      return BCMULOG_DESTMASK_SYSLOG;
   }
   else if (cmsLogDestination == LOG_DEST_TELNET)
   {
      return BCMULOG_DESTMASK_TELNET;
   }
   else
   {
      // bcmulog doesn't understand telnet destination.  Default to STDERR.
      return BCMULOG_DESTMASK_STDERR;
   }
}

UINT32 cmsLog_mask_to_dest(UINT32 destMask)
{
   // Cannot fully convert a mask to an enum.  Just take the first set bit
   // we understand and return that.
   if (destMask & BCMULOG_DESTMASK_STDERR)
   {
      return LOG_DEST_STDERR;
   }
   else if (destMask & BCMULOG_DESTMASK_SYSLOG)
   {
      return LOG_DEST_SYSLOG;
   }
   else if (destMask & BCMULOG_DESTMASK_TELNET)
   {
      return LOG_DEST_TELNET;
   }
   else
   {
      // Some unknown mask setting
      return LOG_DEST_UNKNOWN;
   }
}

void log_log(CmsLogLevel level, const char *func, UINT32 lineNum, const char *pFmt, ... )
{
   BcmuLogFormatInfo info;
   char *logBuf = NULL;
   size_t bufSize;
   FILE *stream;
   UINT32 destMask;
   SINT32 threadId;
   va_list ap;

   threadId = sysUtl_getThreadId();
   if (level > bcmuLog_getLevelEx(threadId, 0))
   {
      return;
   }

   /* Call external function to format the log line. */
   memset(&info, 0, sizeof(info));
   info.logLevel = level;
   destMask = bcmuLog_getDestMaskEx(threadId, 0);
   info.logDestMask = destMask;
   info.logHeaderMask = bcmuLog_getHdrMaskEx(threadId, 0);
   info.lineNum = lineNum;
   cmsUtl_strncpy(info.funcName, func, sizeof(info.funcName));
   cmsUtl_strncpy(info.appName, bcmuLog_getNameEx(threadId, 0), sizeof(info.appName));

   stream = open_memstream(&logBuf, &bufSize);
   if (stream == NULL)
   {
       printf("Failed to open memory stream!\n");
       return;
   }

   va_start(ap, pFmt);
   bcmuLog_formatLine(stream, &info, pFmt, ap);
   va_end(ap);

   fclose(stream);

   /* Do the output to configured destination */
   if (destMask & BCMULOG_DESTMASK_STDERR)
   {
      fprintf(stderr, "%s\n", logBuf);
      fflush(stderr);
   }

   if (destMask & BCMULOG_DESTMASK_TELNET)
   {
      int logTelnetFd = -1;
#ifdef DESKTOP_LINUX
      /* Fedora Desktop Linux */
      logTelnetFd = open("/dev/pts/1", O_RDWR);
#else
      /* CPE use ptyp0 as the first pesudo terminal */
      logTelnetFd = open("/dev/ttyp0", O_RDWR);
#endif
      if(logTelnetFd != -1)
      {
         if (0 > write(logTelnetFd, logBuf, strlen(logBuf)))
            printf("write to telnet fd failed\n");
         if (0 > write(logTelnetFd, "\n", strlen("\n")))
            printf("write to telnet fd failed(2)\n");
         close(logTelnetFd);
      }
   }

   if (destMask & BCMULOG_DESTMASK_SYSLOG)
   {
      oalLog_syslog(level, logBuf);
   }

   free(logBuf);

}  /* End of log_log() */


void cmsLog_initWithName(CmsEntityId eid __attribute__((unused)),
                         const char *appName)
{
   bcmuLog_setName(appName);

   oalLog_init();

   return;
}


void cmsLog_init(CmsEntityId eid)
{
   const CmsEntityInfo *eInfo;

   if (NULL != (eInfo = cmsEid_getEntityInfo(eid)))
   {
      cmsLog_initWithName(eid, eInfo->name);
   }
   else
   {
      cmsLog_initWithName(eid, NULL);
   }

   return;

}  /* End of cmsLog_init() */


void cmsLog_cleanup(void)
{
   oalLog_cleanup();
   return;

}  /* End of cmsLog_cleanup() */


void cmsLog_setLevel(CmsLogLevel level)
{
   bcmuLog_setLevel(level);
   return;
}


CmsLogLevel cmsLog_getLevel(void)
{
   return (bcmuLog_getLevel());
}


void cmsLog_setDestination(CmsLogDestination dest)
{
   bcmuLog_setDestMask(cmsLog_dest_to_mask(dest));
   return;
}


CmsLogDestination cmsLog_getDestination(void)
{
   // convert from bcmuLog mask to CMS Log Destination enum
   UINT32 mask = bcmuLog_getDestMask();
   return (cmsLog_mask_to_dest(mask));
}


void cmsLog_setHeaderMask(UINT32 headerMask)
{
   bcmuLog_setHdrMask(headerMask);
   return;
}


UINT32 cmsLog_getHeaderMask(void)
{
   return (bcmuLog_getHdrMask());
}


int cmsLog_readPartial(int ptr, char* buffer)
{
   return (oal_readLogPartial(ptr, buffer));
}
