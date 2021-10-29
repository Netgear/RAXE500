/*******************************************************************************
 * Copyright 2005-2018  Hon Hai Precision Ind. Co. Ltd.
 * All Rights Reserved.
 * No portions of this material shall be reproduced in any form without the
 * written permission of Hon Hai Precision Ind. Co. Ltd.
 *
 * All information contained in this document is Hon Hai Precision Ind.
 * Co. Ltd. company private, proprietary, and trade secret property and
 * are protected by international intellectual property laws and treaties.
 *
 ******************************************************************************/

#ifndef __ACOS_COMMON_H__
#define __ACOS_COMMON_H__

#define ACOS_STRNCPY(dst, src, count) \
	do { \
		strncpy(dst, src, count - 1); \
		dst[count - 1] = '\0'; \
	} while (0)

#define ACOS_AGENT_PID_FILE     "/var/run/acos_agent.pid"
#define CHECK_FW_PID_FILE     "/var/run/checkfw.pid"

#endif /* __ACOS_COMMON_H__ */



