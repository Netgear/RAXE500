#ifndef _PROJECT_CONFIG_H
#define _PROJECT_CONFIG_H

#define NETGEAR_DEVICE_ID               "RAXE500"
#define NETGEAR_HARDWARE_VERSION        "V1"

#define MAX_INTERFACE_NAME_LENGTH   					16
#define FXCN_MAX_GUEST_NETWORK_NUM                      16
#define FXCN_MAX_GUEST_NETWORK_INTERFACE_NAME_LENGTH    MAX_INTERFACE_NAME_LENGTH
#define FXCN_MAX_BACKHAUL_INTERFACE_NUM             	16
#define FXCN_MAX_BACHHAUL_INTERFACE_NAME_LENGTH     	MAX_INTERFACE_NAME_LENGTH
#define FXCN_ACCESS_CONTROL_MAX_NUM_DEVICES     		256
#define FXCN_MAX_HIJACK_DOMAIN_NUM      				64
#define FXCN_MAX_HIJACK_DOMAIN_LENGTH   				64

#define FXCN_BR_HIJACK_CLI_MAJOR_NUM            		131
#define FXCN_OOPS_WRITER_CLI_MAJOR_NUM                  132
#define FXCN_FXCN_MISC_MODULE_CLI_MAJOR_NUM            	134
#define FXCN_BR_DEVICE_DETECT_CLI_MAJOR_NUM             135
#define FXCN_ACOS_GPIO_CLI_MAJOR_NUM                    137 /* 136 is occupied by pts on this platform. */

#define ACOS_PPP_SKBUF_CB_OFFSET    39  /* Add a tag in control buffer to indicate the packet is from LAN side. Need to make sure this filed in control buffer is not used by other functions. */

#define FOXSHM_DEVICE_DETECT                            0x11223344
#define FOXSHM_USER_DEFINED_KNOWN_DEVICE                0x11223345
#define FOXSHM_AUTO_DETECTED_KNOWN_DEVICE               0x11223346
#define FOXSHM_KNOWN_SATELLITE                          0x11223347
#define FOXSHM_HISTORICAL_DEVICE_INFO                   0x11223348
#define FOXSHM_LED_CONTROL                              0x11223349
#define FOXSHM_ACOS_NVRAM                               0x11223350
#define FOXSEM_LED_CONTROL                              "/sem_led"
#define FOXSHM_USB_DEVICE_CONTROL                       0x11223359
#define FOXSHM_USB_SVC_INFO                             0x11223360
#define FOXSEM_USB_DEVICE_CONTROL                       "/sem_usb"
#define BACKUP_CFG_HEADER			"<header>RAXE500-Foxconn<\header>"
/* Old issue: Beta user use telnet to login and cat /usr/sbin/httpd,
 *   then user can find the string: "unzip -o -P EAX20agv#a8iT restore.zip" 
 *   then user can guess "EAX20agv#a8iT" is the password
 * So don't use the const string for password which can be found in binary file easily
 */
#define BACKUP_CFG_PASSWORD_BYTE1   'R'
#define BACKUP_CFG_PASSWORD_BYTE2   'A'
#define BACKUP_CFG_PASSWORD_BYTE3   'X'
#define BACKUP_CFG_PASSWORD_BYTE4   'E'
#define BACKUP_CFG_PASSWORD_BYTE5   '5'
#define BACKUP_CFG_PASSWORD_BYTE6   '0'
#define BACKUP_CFG_PASSWORD_BYTE7   '0'
#define BACKUP_CFG_PASSWORD_BYTE8   'v'
#define BACKUP_CFG_PASSWORD_BYTE9   'k'
#define BACKUP_CFG_PASSWORD_BYTE10  'f'
#define BACKUP_CFG_PASSWORD_BYTE11  '9'
#define BACKUP_CFG_PASSWORD_BYTE12  'n'
#define BACKUP_CFG_PASSWORD_BYTE13  '\0'

#define BACKUP_CFG_MAX_SIZE         256*1024
#define FIRMWARE_FILE_MAX__SIZE     128*1024*1024

#define FLASH_STORAGE_PATH                  "/data"
#define OOPS_WRITER_LOG_STORED_PATH         "/data/router-analytics"
#define ROUTER_ANALYTICS_DATA_STORED_PATH   "/data/router-analytics"
#define NVRAMS_FILE_STORED_PATH             "/data/acos_nvram"

#define WPS_PUSH_BUTTON_GPIO        53
#define WPS_PUSH_BUTTON_PRESSED     0
#define WPS_AP_INTERFACE            "wl0 wl1 wl2"
#define WPS_CLIENT_INTERFACE        ""

/* Attached devices parameters */
#define DHCPD_LEASE_TABLE                   "/tmp/udhcpd.leases"
#define FXCN_MAX_ATTACHED_DEVICE_NUMBER     256
#define FXCN_MAX_KNOWN_SATELLITE_NUMBER     16
#define WIFI_2G_FRONTHAUL_INTERFACE         "wl0"
#define WIFI_5G_FRONTHAUL_INTERFACE         "wl1"
#define WIFI_5G_2_FRONTHAUL_INTERFACE       "wl2"
#define WIFI_6G_FRONTHAUL_INTERFACE       	"wl2"

#define WIFI_2G_GUEST_INTERFACE             "wl0.1"
#define WIFI_5G_GUEST_INTERFACE             "wl1.1"
#define WIFI_5G_2_GUEST_INTERFACE           "wl2.1"
#define WIFI_6G_GUEST_INTERFACE           	"wl2.1"

#define WIFI_2G_UPLINK_BACKHAUL_INTERFACE   "dummy"
#define WIFI_5G_UPLINK_BACKHAUL_INTERFACE   "dummy"
#define WIFI_5G_2_UPLINK_BACKHAUL_INTERFACE   "dummy"
#define ETHERNET_UPLINK_BACKHAUL_INTERFACE  "dummy"
#define WIFI_5G_DOWNLINK_BACKHAUL_INTERFACE "dummy"
#define WIFI_5G_2_DOWNLINK_BACKHAUL_INTERFACE "dummy"

#define WIRELESS_6G_HT80_DEFAULT_CHANNEL		"39"
#define WIRELESS_6G_HT160_DEFAULT_CHANNEL		"47"

/* MTD Table */
#define MTD_PARTITION1				"/dev/mtd1"		/*ubifs*/

#define MTD_ROOTFS                  "/dev/mtd0"  /* as same as mtd4 */
#define MTD_SECOND_CFE              "/dev/mtd1"  /* as same as mtd5 */
#define MTD_DATA                    "/dev/mtd9"
#define MTD_NVRAM                   "/dev/mtd3"
/* /dev/mtd6 and /dev/mtd7 are dummy */
#define MTD_MISC3                   "/dev/mtd8"
#define MTD_MISC2                   "/dev/mtd2"
#define MTD_NVM                     "/dev/mtd10"

#define CIRCLE_DOWNLOAD_SERVER		"http://http.updates1.netgear.com/sw-apps/parental-control/circle/raxe500/"
#endif
