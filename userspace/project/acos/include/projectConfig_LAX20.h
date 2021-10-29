#ifndef _PROJECT_CONFIG_H
#define _PROJECT_CONFIG_H

#define MAX_INTERFACE_NAME_LENGTH   					16
#define FXCN_MAX_GUEST_NETWORK_NUM                      16
#define FXCN_MAX_GUEST_NETWORK_INTERFACE_NAME_LENGTH    MAX_INTERFACE_NAME_LENGTH
#define FXCN_MAX_BACKHAUL_INTERFACE_NUM             	16
#define FXCN_MAX_BACHHAUL_INTERFACE_NAME_LENGTH     	MAX_INTERFACE_NAME_LENGTH
#define FXCN_ACCESS_CONTROL_MAX_NUM_DEVICES     		256
#define FXCN_MAX_HIJACK_DOMAIN_NUM      				36
#define FXCN_MAX_HIJACK_DOMAIN_LENGTH   				64

#define FXCN_BR_HIJACK_CLI_MAJOR_NUM            		131
#define FXCN_OOPS_WRITER_CLI_MAJOR_NUM                  132
#define FXCN_FXCN_MISC_MODULE_CLI_MAJOR_NUM            	134

#define BACKUP_CFG_HEADER			"<header>RAX20-Foxconn<\header>"
#define BACKUP_CFG_PASSWORD			"RAX20w!a4udk"

#define OOPS_WRITER_LOG_STORED_PATH         "/data/router-analytics"
#define ROUTER_ANALYTICS_DATA_STORED_PATH   "/data/router-analytics"

#define WPS_PUSH_BUTTON_GPIO        4
#define WPS_PUSH_BUTTON_PRESSED     0
#define WPS_AP_INTERFACE            "wl0 wl1"
#define WPS_CLIENT_INTERFACE        ""

/* MTD Table */
#define MTD_ROOTFS                  "/dev/mtd0"  /* as same as mtd4 */
#define MTD_SECOND_CFE              "/dev/mtd1"  /* as same as mtd5 */
#define MTD_DATA                    "/dev/mtd2"
#define MTD_NVRAM                   "/dev/mtd3"
/* /dev/mtd6 and /dev/mtd7 are dummy */
#define MTD_MISC3                   "/dev/mtd8"
#define MTD_MISC2                   "/dev/mtd9"
#define MTD_NVM                     "/dev/mtd10"

#endif
