/*
   Copyright (c) 2019 Broadcom Corporation
   All Rights Reserved

    <:label-BRCM:2019:DUAL/GPL:standard
    
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

#ifndef _BRCM_RGMII_H_
#define _BRCM_RGMII_H_

#ifndef BRCM_RGMII
static inline
#endif
int rgmii_attach(uint32_t port, int delay_rx, int delay_tx, int is_1p8v, int rgmii_pins[], int num_pins)
#ifndef BRCM_RGMII
{ return 0; }
#endif
;

#endif

