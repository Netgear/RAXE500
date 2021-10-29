/*
<:copyright-BRCM:2019:DUAL/GPL:standard 

   Copyright (c) 2019 Broadcom 
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
*/

/*****************************************************************************
 *  Description:
 *      This contains special header for different flavors of PMC drivers.
 *****************************************************************************/

#ifndef PMC_DRV_CFG_H
#define PMC_DRV_CFG_H

#define PMC_IMPL_1_X

#define PMC_CPU_BIG_ENDIAN   1
#define PMC_GETRCAL_SUPPORT  1 
#define PMC_CPUTEMP_SUPPORT 1
#define PMC_CLOCK_SET_SUPPORT 1

#endif // #ifndef PMC_DRV_CFG_H
