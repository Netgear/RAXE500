// SPDX-License-Identifier: GPL-2.0+
/*
   Copyright (c) 2019 Broadcom Corporation
   All Rights Reserved

    
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

