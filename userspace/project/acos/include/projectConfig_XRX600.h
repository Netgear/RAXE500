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

#define BACKUP_CFG_HEADER			"<header>RAX50-Foxconn<\header>"
#define BACKUP_CFG_PASSWORD_BYTE1   'R'
#define BACKUP_CFG_PASSWORD_BYTE2   'A'
#define BACKUP_CFG_PASSWORD_BYTE3   'X'
#define BACKUP_CFG_PASSWORD_BYTE4   '5'
#define BACKUP_CFG_PASSWORD_BYTE5   '0'
#define BACKUP_CFG_PASSWORD_BYTE6   'w'
#define BACKUP_CFG_PASSWORD_BYTE7   '!'
#define BACKUP_CFG_PASSWORD_BYTE8   'a'
#define BACKUP_CFG_PASSWORD_BYTE9   '4'
#define BACKUP_CFG_PASSWORD_BYTE10  'u'
#define BACKUP_CFG_PASSWORD_BYTE11  'd'
#define BACKUP_CFG_PASSWORD_BYTE12  'k'
#define BACKUP_CFG_PASSWORD_BYTE13  '\0'


#define OOPS_WRITER_LOG_STORED_PATH         "/data/router-analytics"
#define ROUTER_ANALYTICS_DATA_STORED_PATH   "/data/router-analytics"

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
