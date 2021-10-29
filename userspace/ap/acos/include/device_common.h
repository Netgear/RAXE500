/*******************************************************************************
 * Copyright 2020 Hon Hai Precision Ind. Co. Ltd.
 * All Rights Reserved.
 * No portions of this material shall be reproduced in any form without the
 * written permission of Hon Hai Precision Ind. Co. Ltd.
 *
 * All information contained in this document is Hon Hai Precision Ind.
 * Co. Ltd. company private, proprietary, and trade secret property and
 * are protected by international intellectual property laws and treaties.
 *
 ******************************************************************************/

#ifndef __DEVICE_COMMON_H__
#define __DEVICE_COMMON_H__

#define BOTH_MODE   2
#define NON_ALLOWED 1
#define NON_BLOCKED 0

#define C_128_SIZE        128
#define C_256_SIZE        256
#define C_512_SIZE        512
#define C_1024_SIZE      1024
#define C_2048_SIZE      2048
#define API_FAIL            -1

typedef struct _tKADUnit
{
    int   index;
    char    name[C_128_SIZE];
    char    mac[C_128_SIZE];
    char    type[C_128_SIZE];
    char    model[C_128_SIZE];
    int   customization;
    struct _tKADUnit  *next;
} tKADUnit;

typedef struct _tKnownAttachDevices
{
    int   total;
    tKADUnit  *next;
} tKnownAttachDevices;

#endif /* __DEVICE_COMMON_H__ */



