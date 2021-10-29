/*************************************************************************
 *
<:copyright-BRCM:2015:DUAL/GPL:standard

   Copyright (c) 2015 Broadcom 
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

#include <stdint.h>
#include <math.h>
#include "cms_math.h"
#include "cms_log.h"


/*****************************************************************************
*  FUNCTION:  pointOneMicroWattsTodB
*  PURPOSE:   Convert power in 0.1uw unit to dBmW or dBuW in 0.001 dB unit.
*  PARAMETERS:
*      puw - power in 0.1uw unit.
*      isdBmW - whether it is represented as dBmW (1) or dBuW (0).
*  RETURNS:
*      dB * 1000.
*  NOTES:
*      1uw-> -30dBm -> OMCI representation -15000.
*      1uw->   0dBu -> OMCI representation 0.
*****************************************************************************/
SINT32 pointOneMicroWattsTodB(UINT32 puw, UBOOL8 isdBmW)
{
    float p = puw;
    float dbm;
    SINT32 rv;

    if (puw == 0)
    {
        return INT32_MIN;
    }

    if (isdBmW == TRUE)
    {
        p = p / 10000; /* Convert 0.1 uW to mW. */
    }
    else
    {
        p = p / 10;    /* Convert 0.1 uW to uW. */
    }

    dbm = 10 * (logf(p) / logf(10));

    dbm *= 1000;

    rv = (SINT32)dbm;

    return rv;
}

/*****************************************************************************
*  FUNCTION:  convertPointOneMicroWattsToOmcidB
*  PURPOSE:   Convert power in 0.1uw unit to the OMCI representation.
*  PARAMETERS:
*      dir - string indicating which power this is (for debugging).
*      puw - Power in 0.1 micro-watts.
*      min - min allowed value by OMCI data model spec.
*      max - max allowed value by OMCI data model spec.
*      inc - reporting increments (steps) required by OMCI data model spec.
*      isdBmW - whether it is represented as dBmW (1) or dBuW (0).
*  RETURNS:
*      SINT32.
*  NOTES:
*      None.
*****************************************************************************/
SINT32 convertPointOneMicroWattsToOmcidB(const char *dir, UINT32 puw,
  SINT32 min, SINT32 max, SINT32 inc, UBOOL8 isdBmW)
{
    SINT32 rv;

    if (inc == 0)
    {
        cmsLog_error("Invalid inc value %d", inc);
        return min;
    }

    rv = pointOneMicroWattsTodB(puw, isdBmW);
    if (rv == INT32_MIN)
    {
        rv = min;
        return rv;
    }

    rv = rv / inc;
    if (rv < min)
    {
        cmsLog_notice("(%s) %d uW => %d below min, limit to %d",
          dir, puw, rv, min);
        rv = min;
    }
    else if (rv > max)
    {
        cmsLog_notice("(%s) %d uW => %d above max, limit to %d",
          dir, puw, rv, max);
        rv = max;
    }
    else
    {
        cmsLog_debug("(%s) %d uW => %d", dir, puw, rv);
    }

    return rv;
}
