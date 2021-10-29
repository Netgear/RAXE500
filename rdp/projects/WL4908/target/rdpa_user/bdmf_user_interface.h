// <:copyright-BRCM:2013:DUAL/GPL:standard
// 
//    Copyright (c) 2013 Broadcom 
//    All Rights Reserved
// 
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License, version 2, as published by
// the Free Software Foundation (the "GPL").
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// 
// A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
// writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
// Boston, MA 02111-1307, USA.
// 
// :>

#ifndef _BDMF_USER_INTERFACE_H_
#define _BDMF_USER_INTERFACE_H_

#include "rdpa_user.h"

#define BDMF_NEW_AND_SET        _IOWR(DRIVER_IOCTL, 0, ioctl_pa_t)
#define BDMF_DESTROY            _IOWR(DRIVER_IOCTL, 1, ioctl_pa_t)
#define BDMF_MATTR_ALLOC        _IOWR(DRIVER_IOCTL, 2, ioctl_pa_t)
#define BDMF_MATTR_FREE         _IOWR(DRIVER_IOCTL, 3, ioctl_pa_t)
#define BDMF_GET                _IOWR(DRIVER_IOCTL, 4, ioctl_pa_t)
#define BDMF_PUT                _IOWR(DRIVER_IOCTL, 5, ioctl_pa_t)
#define BDMF_GET_NEXT           _IOWR(DRIVER_IOCTL, 6, ioctl_pa_t)
#define BDMF_LINK               _IOWR(DRIVER_IOCTL, 7, ioctl_pa_t)
#define BDMF_UNLINK             _IOWR(DRIVER_IOCTL, 8, ioctl_pa_t)
#define BDMF_GET_NEXT_US_LINK   _IOWR(DRIVER_IOCTL, 9, ioctl_pa_t)
#define BDMF_GET_NEXT_DS_LINK   _IOWR(DRIVER_IOCTL, 10, ioctl_pa_t)
#define BDMF_US_LINK_TO_OBJECT  _IOWR(DRIVER_IOCTL, 11, ioctl_pa_t)
#define BDMF_DS_LINK_TO_OBJECT  _IOWR(DRIVER_IOCTL, 12, ioctl_pa_t)
#define BDMF_GET_OWNER          _IOWR(DRIVER_IOCTL, 13, ioctl_pa_t)

#endif /*_BDMF_USER_INTERFACE_H_*/
