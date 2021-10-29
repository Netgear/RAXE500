#ifndef _PROJECT_CONFIG_H
#define _PROJECT_CONFIG_H

#define NETGEAR_DEVICE_ID               "MS60"
#define NETGEAR_HARDWARE_VERSION        "V1"

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
#define FXCN_BR_DEVICE_DETECT_CLI_MAJOR_NUM             135

#define FOXSHM_DEVICE_DETECT                            0x11223344
#define FOXSHM_USER_DEFINED_KNOWN_DEVICE                0x11223345
#define FOXSHM_AUTO_DETECTED_KNOWN_DEVICE               0x11223346
#define FOXSHM_KNOWN_SATELLITE                          0x11223347
#define FOXSHM_HISTORICAL_DEVICE_INFO                   0x11223348
#define FOXSHM_LED_CONTROL                              0x11223349
#define FOXSEM_LED_CONTROL                              "/sem_led"

#define BACKUP_CFG_HEADER			"<header>MS60-Foxconn<\header>"
#define BACKUP_CFG_PASSWORD_BYTE1   'M'
#define BACKUP_CFG_PASSWORD_BYTE2   'S'
#define BACKUP_CFG_PASSWORD_BYTE3   '6'
#define BACKUP_CFG_PASSWORD_BYTE4   '0'
#define BACKUP_CFG_PASSWORD_BYTE5   'f'
#define BACKUP_CFG_PASSWORD_BYTE6   'F'
#define BACKUP_CFG_PASSWORD_BYTE7   'g'
#define BACKUP_CFG_PASSWORD_BYTE8   '7'
#define BACKUP_CFG_PASSWORD_BYTE9   'Q'
#define BACKUP_CFG_PASSWORD_BYTE10  '3'
#define BACKUP_CFG_PASSWORD_BYTE11  '#'
#define BACKUP_CFG_PASSWORD_BYTE12  '#'
#define BACKUP_CFG_PASSWORD_BYTE13  '\0'


#define BACKUP_CFG_MAX_SIZE         256*1024
#define FIRMWARE_FILE_MAX__SIZE     128*1024*1024

#define FLASH_STORAGE_PATH                  "/data"
#define OOPS_WRITER_LOG_STORED_PATH         "/data/router-analytics"
#define ROUTER_ANALYTICS_DATA_STORED_PATH   "/data/router-analytics"

#define WIFI_BACKHAUL_INTREFACE            "wl1"
/* Attached devices parameters */
#define DHCPD_LEASE_TABLE                   "/proc/devname_dhcp"
#define FXCN_MAX_ATTACHED_DEVICE_NUMBER     256
#define FXCN_MAX_KNOWN_SATELLITE_NUMBER     16
#define WIFI_2G_FRONTHAUL_INTERFACE         "wl0"
#define WIFI_5G_FRONTHAUL_INTERFACE         "wl1.1"
#define WIFI_2G_GUEST_INTERFACE             "wl0.1"
#define WIFI_5G_GUEST_INTERFACE             "wl1.3"

#define WIFI_2G_UPLINK_BACKHAUL_INTERFACE   ""
#define WIFI_5G_UPLINK_BACKHAUL_INTERFACE   "wl1"
#define ETHERNET_UPLINK_BACKHAUL_INTERFACE  "eth0.0"
#define WIFI_5G_DOWNLINK_BACKHAUL_INTERFACE "wds"

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
