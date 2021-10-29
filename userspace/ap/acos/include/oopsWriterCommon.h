/*******************************************************************************
 * Copyright 2005-2019  Hon Hai Precision Ind. Co. Ltd.
 * All Rights Reserved.
 * No portions of this material shall be reproduced in any form without the
 * written permission of Hon Hai Precision Ind. Co. Ltd.
 *
 * All information contained in this document is Hon Hai Precision Ind.
 * Co. Ltd. company private, proprietary, and trade secret property and
 * are protected by international intellectual property laws and treaties.
 *
 ******************************************************************************/

#ifndef __OOPS_WRITER_COMMON_H__
#define __OOPS_WRITER_COMMON_H__

#include "ambitCfg.h"
#include "projectConfig.h"

#define OOPS_WRITER_CLI_MAJOR_NUM                   FXCN_OOPS_WRITER_CLI_MAJOR_NUM      /* this is defined in projectConfig.h per project */
#define OOPS_WRITER_CLI_MINOR_NUM                   0
#define OOPS_WRITER_CLI_DEVICE_FILE_NAME            "oops_writer_cli"
#define OOPS_WRITER_CLI_DEVICE_FILE_NAME_W_PATH     "/dev/"OOPS_WRITER_CLI_DEVICE_FILE_NAME
                                                
#define IOCTL_SET_RA_TIMEZONE                       _IOW(OOPS_WRITER_CLI_MAJOR_NUM, 205, int)

#endif /* __OOPS_WRITER_COMMON_H__ */



