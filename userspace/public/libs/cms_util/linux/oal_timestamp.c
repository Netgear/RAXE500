/***********************************************************************
 *
<:copyright-BRCM:2007:DUAL/GPL:standard

   Copyright (c) 2007 Broadcom 
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

#include "../oal.h"
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include "bcm_timestamp.h"


/** OS dependent timestamp functions go in this file.
 */
void oalTms_get(CmsTimestamp *tms)
{
   struct timespec ts;
   SINT32 rc;

   if (tms == NULL)
   {
      return;
   }

   // CLOCK_MONOTONIC is the standard POSIX way to get a monotonically 
   // increasing timestamp, but it does not seem to be sync'd across all
   // our ARM cores.
   // We tried CLOCK_BOOTTIME, but that is not sync'd either.
   // Another possibility is CLOCK_REALTIME, but ntp may cause a large jump
   // when initially sync'd at bootup, and then may go backwards due to 
   // adjustments made by ntp or admin user.
   // TODO: cmsLog calls bcmuLog, which gets its timestamp by directly calling
   // clock_gettime.  Maybe unify the implementations?
   rc = bcm_libc_clock_gettime(CLOCK_MONOTONIC, &ts);
   if (rc == 0)
   {
      tms->sec = ts.tv_sec;
      tms->nsec = ts.tv_nsec;
   }
   else
   {
      cmsLog_error("bcm_libc_clock_gettime failed, set timestamp to 0");
      tms->sec = 0;
      tms->nsec = 0;
   }
}

UINT32 oal_getUpTime_inSeconds()
{
   struct sysinfo info;
   sysinfo( &info );
   return( info.uptime );
}

CmsRet oalTms_getXSIDateTime(UINT32 t, char *buf, UINT32 bufLen)
{
	int          c;
   time_t       now;
	struct tm   *tmp;

   if (t == 0)
   {
      now = time(NULL);
   }
   else
   {
      now = t;
   }

	tmp = localtime(&now);
   memset(buf, 0, bufLen);
	c = strftime(buf, bufLen, "%Y-%m-%dT%H:%M:%S%z", tmp);
   if ((c == 0) || (c+1 > (int) bufLen))
   {
      /* buf was not long enough */
      return CMSRET_RESOURCE_EXCEEDED;
   }

	/* fix missing : in time-zone offset-- change -500 to -5:00 */
   buf[c+1] = '\0';
   buf[c] = buf[c-1];
   buf[c-1] = buf[c-2];
   buf[c-2]=':';

   return CMSRET_SUCCESS;
}

CmsRet oalTms_getXSIDateTimeMicroseconds(UINT32 t, UINT32 t_ms, char *buf, UINT32 bufLen)
{
   int          c;
   struct tm   *tmp;
   struct timeval tv;
   char *format;

   if (t == 0 && t_ms == 0)
   {
      gettimeofday(&tv, NULL);
   }
   else
   {
      tv.tv_sec = t;
      tv.tv_usec = t_ms;
   }

   tmp = localtime(&tv.tv_sec);
   format = (char *) malloc(bufLen);
   if (!format) 
      return CMSRET_RESOURCE_EXCEEDED;

   memset(buf, 0, bufLen);
   memset(format, 0, bufLen);
	c = strftime(format, bufLen, "%Y-%m-%dT%H:%M:%S.%%06u", tmp);
   snprintf(buf, bufLen, format, tv.tv_usec);
   free(format);
   if ((c == 0) || (c+1 > (int) bufLen))
   {
      /* buf was not long enough */
      return CMSRET_RESOURCE_EXCEEDED;
   }

   return CMSRET_SUCCESS;
}
