/***********************************************************************
 *
 * <:copyright-BRCM:2015:DUAL/GPL:standard
 * 
 *    Copyright (c) 2015 Broadcom 
 *    All Rights Reserved
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

#if !defined(CONFIG_BCM963158)

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include "rdpa_types.h"
#include "opticaldet.h"
#include "wanconf.h"
#if !defined(PON_WAN_TYPE_AUTO_DETECT)
#include "board.h"
#include "cms_psp.h"
#endif
#include "pmd.h"


#if defined(PON_WAN_TYPE_AUTO_DETECT)
#if defined(PMD_JSON_LIB)
extern int load_pmd_calibration_data(pmd_calibration_parameters *calibration_binary);
#endif

static int detect_wan_type(rdpa_wan_type *wan_type)
{
    int fd, ret;
    pmd_wan_type_auto_detect_settings pmd_wan_auto_detect = {0};

    *wan_type = rdpa_wan_none;
    fd = open("/dev/wantypedetect", O_RDWR);
    if (fd < 0)
    {
        wc_log_err("%s: %s\n", "/dev/wantypedetect", strerror(errno));
        return DETECTION_ERROR;
    }
 
#if defined(PMD_JSON_LIB)
    ret = load_pmd_calibration_data(&(pmd_wan_auto_detect.calibration_parameters_from_json));
    if (!ret)
    {
        pmd_wan_auto_detect.is_calibration_file_valid = 1;
    }
#endif

    ret = ioctl(fd, OPTICALDET_IOCTL_DETECT, &pmd_wan_auto_detect);
    close(fd);

    if (ret)
    {
        wc_log_err("ioctl failed, Errno[%s] ret=%d\n", strerror(errno), ret);
        return DETECTION_ERROR;
    }

    *wan_type = *(rdpa_wan_type *)&pmd_wan_auto_detect; /* read/write pointer */
    return DETECTION_OK;
}

#endif


static rdpa_wan_type detect_and_set_scratchpad(void)
{
    rdpa_wan_type wan_type = rdpa_wan_none;

#if defined(PON_WAN_TYPE_AUTO_DETECT)
    if (detect_wan_type(&wan_type))
    {
        wan_type = rdpa_wan_none;
    }
#endif

    return wan_type;
}


int try_wan_type_detect_and_set(rdpa_wan_type *wan_type)
{
#if defined(PON_WAN_TYPE_AUTO_DETECT)
    int ret;

    ret = system("insmod /lib/modules/"KERNELVER"/extra/wantypedet.ko");
    if (ret)
    {
        wc_log_err("Failed to load wantypedet.ko (rc=%d)\n", ret);
        return DETECTION_ERROR;
    }

    *wan_type = detect_and_set_scratchpad();

    ret = system("rmmod wantypedet.ko");
    if (ret)
        wc_log_err("Failed to unload wantypedet.ko (rc=%d)\n", ret);
#else
    *wan_type = rdpa_wan_none;
#endif

    return DETECTION_OK;
}

#endif
