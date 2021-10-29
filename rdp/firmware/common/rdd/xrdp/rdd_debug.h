/*
    <:copyright-BRCM:2014-2016:DUAL/GPL:standard
    
       Copyright (c) 2014-2016 Broadcom 
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

#ifndef _RDD_DEBUG_H
#define _RDD_DEBUG_H

#include "rdpa_types.h"

struct debug_prints_info {
  void* debug_print_buf;
  uint32_t  debug_print_buf_len;       /* allocated print buffer length */
  uint32_t  double_buf_num;            /* incdicates current part of double buffer */
  uint32_t  priority;                  /* messages below priority are not displayed */
  uint32_t  perodicity_ms;             /* print function called every X msec */
  int32_t  num_of_messages_in_period;  /* number of messages to print per period. 0 - disable, -1 show_all */
};

extern struct debug_prints_info rdd_debug_prints_info;

void rdd_debug_init_runners_strings(void);
const char *rdd_debug_get_debug_string(int core_id, int ind, uint32_t params_num );
void rdd_debug_prints_init(void);
void rdd_debug_prints_update_params(int32_t max_prints_per_period, uint32_t period, uint32_t priority);
void rdd_debug_prints_handle(void);
#if !defined(RDP_SIM) 
void rdd_debug_prints_start_thread(void);
void rdd_debug_prints_stop_thread(void);
#endif

#endif