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

#include "bdmf_user_interface.h"
#include "rdpa_user_int.h"

int bdmf_user_new_and_set(bdmf_ioctl_t *ba)
{
    BDMF_TRACE_DBG("ENTER bdmf_user_new_and_set");

    if ((ba->ret = bdmf_new_and_set(ba->drv, ba->owner_or_ds, ba->mattr, &ba->mo_or_us)))
    {
        BDMF_TRACE_ERR("bdmf_new_and_set failed, ret:%d\n", ba->ret);
    }

    return 0;
}

int bdmf_user_destroy(bdmf_ioctl_t *ba)
{
    BDMF_TRACE_DBG("ENTER bdmf_user_destroy");

    if ((ba->ret = bdmf_destroy(ba->mo_or_us)))
    {
        BDMF_TRACE_ERR("bdmf_destroy failed, ret:%d\n", ba->ret);
    }

    return 0;
}

int bdmf_user_mattr_alloc(bdmf_ioctl_t *ba)
{
    BDMF_TRACE_DBG("ENTER bdmf_user_mattr_alloc");
 
    if ((ba->mattr = bdmf_mattr_alloc(ba->drv)) == NULL)
    {
        BDMF_TRACE_ERR("bdmf_user_mattr_alloc failed\n");
    }

    return 0;
}

int bdmf_user_mattr_free(bdmf_ioctl_t *ba)
{
    BDMF_TRACE_DBG("ENTER bdmf_user_mattr_free");
    
    bdmf_mattr_free(ba->mattr);

    return 0;
}

int bdmf_user_get(bdmf_ioctl_t *ba)
{
    BDMF_TRACE_DBG("ENTER bdmf_user_get");
  
    bdmf_get(ba->mo_or_us);

    return 0;
}

int bdmf_user_put(bdmf_ioctl_t *ba)
{
    BDMF_TRACE_DBG("ENTER bdmf_user_put");
    
    bdmf_put(ba->mo_or_us);

    return 0;
}

int bdmf_user_get_next(bdmf_ioctl_t *ba)
{
    BDMF_TRACE_DBG("ENTER bdmf_user_get_next");
    
    ba->owner_or_ds = bdmf_get_next(ba->drv, ba->mo_or_us, ba->str);

    return 0;
}

int bdmf_user_link(bdmf_ioctl_t *ba)
{
    BDMF_TRACE_DBG("ENTER bdmf_user_link");
      
    if ((ba->ret = bdmf_link(ba->owner_or_ds, ba->mo_or_us, ba->str)) != 0)
    {
        BDMF_TRACE_ERR("bdmf_user_link failed, ret=%d\n", ba->ret);
    }

    return 0;
}

int bdmf_user_unlink(bdmf_ioctl_t *ba)
{
    BDMF_TRACE_DBG("ENTER bdmf_user_unlink");
    
    if ((ba->ret = bdmf_unlink(ba->owner_or_ds, ba->mo_or_us)) != 0)
    {
        BDMF_TRACE_ERR("bdmf_user_unlink failed, ret=%d\n", ba->ret);
    }

    return 0;
}

int bdmf_user_get_next_us_link(bdmf_ioctl_t *ba)
{
    BDMF_TRACE_DBG("ENTER bdmf_user_get_next_us_link");
    
    if ((ba->link = bdmf_get_next_us_link(ba->owner_or_ds, ba->link)) == NULL)
    {
        BDMF_TRACE_DBG("bdmf_user_get_next_us_link NULL\n");
    }

    return 0;
}

int bdmf_user_get_next_ds_link(bdmf_ioctl_t *ba)
{
    BDMF_TRACE_DBG("ENTER bdmf_user_get_next_ds_link");
    
    if ((ba->link = bdmf_get_next_ds_link(ba->mo_or_us, ba->link)) == NULL)
    {
        BDMF_TRACE_DBG("bdmf_user_get_next_ds_link NULL\n");
    }

    return 0;
}

int bdmf_user_us_link_to_object(bdmf_ioctl_t *ba)
{
    BDMF_TRACE_DBG("ENTER bdmf_user_us_link_to_object");
    
    if ((ba->mo_or_us = bdmf_us_link_to_object(ba->link)) == NULL)
    {
        BDMF_TRACE_ERR("bdmf_user_lus_link_to_object failed\n");
    }

    return 0;
}

int bdmf_user_ds_link_to_object(bdmf_ioctl_t *ba)
{
    BDMF_TRACE_DBG("ENTER bdmf_user_us_link_to_object");
    
    if ((ba->owner_or_ds = bdmf_us_link_to_object(ba->link)) == NULL)
    {
        BDMF_TRACE_ERR("bdmf_user_lus_link_to_object failed\n");
    }

    return 0;
}

int bdmf_user_get_owner(bdmf_ioctl_t *ba)
{
    BDMF_TRACE_DBG("ENTER bdmf_user_get_owner");
    
    bdmf_get_owner(ba->mo_or_us, &ba->owner_or_ds);

    return 0;
}
