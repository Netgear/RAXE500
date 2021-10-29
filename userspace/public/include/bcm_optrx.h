/***********************************************************************
 *
 * Copyright (c) 2019 Broadcom
 * All Rights Reserved
 *
 * <:label-BRCM:2019:DUAL/GPL:standard
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
 
#ifndef __BCM_OPTRX_H__
#define __BCM_OPTRX_H__
 
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include "number_defs.h"
#include "bcm_retcodes.h"
#include "bcm_ulog.h"
#include "laser.h"
 
typedef struct OpticalTrxData
{
    UINT32 temperature;
    UINT32 voltage;
    UINT32 biasCurrent;
    UINT32 txPower;
    UINT32 rxPower;
} OpticalTrxData;
 
 
/** Read optical transceiver raw data. It is caller's responsiblity to convert
 *  and represent the data according to the data model specification.
 *
 * @param trxDataP (IN) optical transceiver raw data.
 *
 * @return BcmRet enum.
 */
static inline BcmRet bcmOpTrx_getOpticalTrxInfo(OpticalTrxData *trxDataP)
{
#ifndef DESKTOP_LINUX
    int laserFd;
    UINT32 word;
    int ret;
 
    memset(trxDataP, 0x0, sizeof(OpticalTrxData));
 
    laserFd = open("/dev/laser_dev", O_RDWR);
    if (laserFd < 0)
    {
       bcmuLog_error("Laser driver open error");
       return BCMRET_INTERNAL_ERROR;
    }
 
    ret = ioctl(laserFd, LASER_IOCTL_GET_TEMPTURE, &word);
    if (ret < 0)
    {
        bcmuLog_error("ioctl error on LASER_IOCTL_GET_TEMPTURE");
    }
    else
    {
        trxDataP->temperature = word & 0xFFFF;
    }
 
    ret = ioctl(laserFd, LASER_IOCTL_GET_VOLTAGE, &word);
    if ((ret < 0) && (errno != EOPNOTSUPP)) /* VOLTAGE is optional in XFP */
    {
        bcmuLog_error("ioctl error on LASER_IOCTL_GET_VOTAGE");
        trxDataP->voltage = 0xFFFF;
    }
    else
    {
        trxDataP->voltage = word & 0xFFFF;
    }
 
    ret = ioctl(laserFd, LASER_IOCTL_GET_BIAS_CURRENT, &word);
    if (ret < 0)
    {
        bcmuLog_error("ioctl error on LASER_IOCTL_GET_BIAS_CURRENT");
    }
    else
    {
        trxDataP->biasCurrent = word & 0xFFFF;
    }
 
    ret = ioctl(laserFd, LASER_IOCTL_GET_TX_PWR, &word);
    if (ret < 0)
    {
        bcmuLog_error("ioctl error on LASER_IOCTL_GET_TX_PWR");
    }
    else
    {
        trxDataP->txPower = word & 0xFFFF;
    }
 
    ret = ioctl(laserFd, LASER_IOCTL_GET_RX_PWR, &word);
    if (ret < 0)
    {
        bcmuLog_error("ioctl error on LASER_IOCTL_GET_RX_PWR");
    }
    else
    {
        trxDataP->rxPower = word & 0xFFFF;
    }
 
    close(laserFd);
#else
    memset(trxDataP, 0x0, sizeof(OpticalTrxData));
#endif /* DESKTOP_LINUX */
 
    return BCMRET_SUCCESS;
}
 
#endif /* __BCM_OPTRX_H__ */
