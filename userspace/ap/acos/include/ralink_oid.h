/*******************************************************************************
 * Copyright 2005  Hon Hai Precision Ind. Co. Ltd.
 * All Rights Reserved.
 * No portions of this material shall be reproduced in any form without the
 * written permission of Hon Hai Precision Ind. Co. Ltd.
 *
 * All information contained in this document is Hon Hai Precision Ind.
 * Co. Ltd. company private, proprietary, and trade secret property and
 * are protected by international intellectual property laws and treaties.
 *
 ******************************************************************************/

/*******************************************************************************
 * Wireless OIDs
 ******************************************************************************/

#ifndef _RALINK_OID_H_
#define _RALINK_OID_H_

#define RT_PRIV_IOCTL                               (SIOCIWFIRSTPRIV + 0x01)
#define RTPRIV_IOCTL_SET                            (SIOCIWFIRSTPRIV + 0x02)
#define RTPRIV_IOCTL_BBP                            (SIOCIWFIRSTPRIV + 0x03)
#define RTPRIV_IOCTL_MAC                            (SIOCIWFIRSTPRIV + 0x05)
#define RTPRIV_IOCTL_E2P                            (SIOCIWFIRSTPRIV + 0x07)
#define RTPRIV_IOCTL_STATISTICS                     (SIOCIWFIRSTPRIV + 0x09)
#define RTPRIV_IOCTL_ADD_PMKID_CACHE                (SIOCIWFIRSTPRIV + 0x0A)
#define RTPRIV_IOCTL_RADIUS_DATA                    (SIOCIWFIRSTPRIV + 0x0C)
#define RTPRIV_IOCTL_GSITESURVEY                    (SIOCIWFIRSTPRIV + 0x0D)
#define RTPRIV_IOCTL_ADD_WPA_KEY                    (SIOCIWFIRSTPRIV + 0x0E)
#define RTPRIV_IOCTL_GET_MAC_TABLE                  (SIOCIWFIRSTPRIV + 0x0F)
#define RTPRIV_IOCTL_GET_MAC_TABLE_STRUCT           (SIOCIWFIRSTPRIV + 0x1F)
#define RTPRIV_IOCTL_STATIC_WEP_COPY                (SIOCIWFIRSTPRIV + 0x10)
#define RTPRIV_IOCTL_WSC_PROFILE                    (SIOCIWFIRSTPRIV + 0x12)
#define RT_QUERY_ATE_TXDONE_COUNT                   0x0401
#define OID_GET_SET_TOGGLE                          0x8000

#define OID_802_11_NETWORK_TYPES_SUPPORTED          0x0103
#define OID_802_11_NETWORK_TYPE_IN_USE              0x0104
#define OID_802_11_RSSI_TRIGGER                     0x0107
#define RT_OID_802_11_RSSI                          0x0108 //rt2860 only , kathy
#define RT_OID_802_11_RSSI_1                        0x0109 //rt2860 only , kathy
#define RT_OID_802_11_RSSI_2                        0x010A //rt2860 only , kathy
#define OID_802_11_NUMBER_OF_ANTENNAS               0x010B
#define OID_802_11_RX_ANTENNA_SELECTED              0x010C
#define OID_802_11_TX_ANTENNA_SELECTED              0x010D
#define OID_802_11_SUPPORTED_RATES                  0x010E
#define OID_802_11_ADD_WEP                          0x0112
#define OID_802_11_REMOVE_WEP                       0x0113
#define OID_802_11_DISASSOCIATE                     0x0114
#define OID_802_11_PRIVACY_FILTER                   0x0118
#define OID_802_11_ASSOCIATION_INFORMATION          0x011E
#define OID_802_11_TEST                             0x011F

#define RT_OID_802_11_COUNTRY_REGION                0x0507
#define OID_802_11_BSSID_LIST_SCAN                  0x0508
#define OID_802_11_SSID                             0x0509
#define OID_802_11_BSSID                            0x050A
#define RT_OID_802_11_RADIO                         0x050B
#define RT_OID_802_11_PHY_MODE                      0x050C
#define RT_OID_802_11_STA_CONFIG                    0x050D
#define OID_802_11_DESIRED_RATES                    0x050E
#define RT_OID_802_11_PREAMBLE                      0x050F
#define OID_802_11_WEP_STATUS                       0x0510
#define OID_802_11_AUTHENTICATION_MODE              0x0511
#define OID_802_11_INFRASTRUCTURE_MODE              0x0512
#define RT_OID_802_11_RESET_COUNTERS                0x0513
#define OID_802_11_RTS_THRESHOLD                    0x0514
#define OID_802_11_FRAGMENTATION_THRESHOLD          0x0515
#define OID_802_11_POWER_MODE                       0x0516
#define OID_802_11_TX_POWER_LEVEL                   0x0517
#define RT_OID_802_11_ADD_WPA                       0x0518
#define OID_802_11_REMOVE_KEY                       0x0519
#define OID_802_11_ADD_KEY                          0x0520
#define OID_802_11_CONFIGURATION                    0x0521
#define OID_802_11_TX_PACKET_BURST                  0x0522
#define RT_OID_802_11_QUERY_NOISE_LEVEL             0x0523
#define RT_OID_802_11_EXTRA_INFO                    0x0524
#define RT_OID_802_11_HARDWARE_REGISTER             0x0525
#define OID_802_11_ENCRYPTION_STATUS                OID_802_11_WEP_STATUS
#define OID_802_11_ACL_LIST                         0x052A

#define OID_802_DOT1X_CONFIGURATION                 0x0540
#define OID_802_DOT1X_PMKID_CACHE                   0x0541
#define OID_802_DOT1X_RADIUS_DATA                   0x0542
#define OID_802_DOT1X_WPA_KEY                       0x0543
#define OID_802_DOT1X_STATIC_WEP_COPY               0x0544
#define OID_802_DOT1X_IDLE_TIMEOUT                  0x0545

#define RT_OID_DEVICE_NAME                          0x0607
#define RT_OID_VERSION_INFO                         0x0608
#define OID_802_11_BSSID_LIST                       0x0609
#define OID_802_3_CURRENT_ADDRESS                   0x060A
#define OID_GEN_MEDIA_CONNECT_STATUS                0x060B
#define RT_OID_802_11_QUERY_LINK_STATUS             0x060C
#define OID_802_11_RSSI                             0x060D
#define OID_802_11_STATISTICS                       0x060E
#define OID_GEN_RCV_OK                              0x060F
#define OID_GEN_RCV_NO_BUFFER                       0x0610
#define RT_OID_802_11_QUERY_EEPROM_VERSION          0x0611
#define RT_OID_802_11_QUERY_FIRMWARE_VERSION        0x0612
#define RT_OID_802_11_QUERY_LAST_RX_RATE            0x0613
#define RT_OID_802_11_TX_POWER_LEVEL_1              0x0614
#define RT_OID_802_11_QUERY_PIDVID                  0x0615

#define OID_SET_COUNTERMEASURES                     0x0616
#define OID_802_11_SET_IEEE8021X                    0x0617
#define OID_802_11_SET_IEEE8021X_REQUIRE_KEY        0x0618
#define OID_802_11_PMKID                            0x0620
#define RT_OID_WPA_SUPPLICANT_SUPPORT               0x0621
#define RT_OID_WE_VERSION_COMPILED                  0x0622

#define RT_OID_NEW_DRIVER                           0x0623
#define OID_AUTO_PROVISION_BSSID_LIST               0x0624
#define RT_OID_WPS_PROBE_REQ_IE                     0x0625

#define RT_OID_802_11_SNR_0                         0x0630
#define RT_OID_802_11_SNR_1                         0x0631
#define RT_OID_802_11_QUERY_LAST_TX_RATE            0x0632
#define RT_OID_802_11_QUERY_HT_PHYMODE              0x0633
#define RT_OID_802_11_SET_HT_PHYMODE                0x0634
#define OID_802_11_RELOAD_DEFAULTS                  0x0635
#define RT_OID_802_11_QUERY_APSD_SETTING            0x0636
#define RT_OID_802_11_SET_APSD_SETTING              0x0637
#define RT_OID_802_11_QUERY_APSD_PSM                0x0638
#define RT_OID_802_11_SET_APSD_PSM                  0x0639
#define RT_OID_802_11_QUERY_DLS                     0x063A
#define RT_OID_802_11_SET_DLS                       0x063B
#define RT_OID_802_11_QUERY_DLS_PARAM               0x063C
#define RT_OID_802_11_SET_DLS_PARAM                 0x063D
#define RT_OID_802_11_QUERY_WMM                     0x063E
#define RT_OID_802_11_SET_WMM                       0x063F
#define RT_OID_802_11_QUERY_IMME_BA_CAP             0x0640
#define RT_OID_802_11_SET_IMME_BA_CAP               0x0641
#define RT_OID_802_11_QUERY_BATABLE                 0x0642
#define RT_OID_802_11_ADD_IMME_BA                   0x0643
#define RT_OID_802_11_TEAR_IMME_BA                  0x0644
#define RT_OID_DRIVER_DEVICE_NAME                   0x0645
#define RT_OID_802_11_QUERY_DAT_HT_PHYMODE          0x0646
#define RT_OID_QUERY_MULTIPLE_CARD_SUPPORT          0x0647
#define OID_802_11_SET_PSPXLINK_MODE                0x0648
#define OID_802_11_SET_PASSPHRASE                   0x0649


#define RT_OID_802_11_QUERY_TX_PHYMODE              0x0650

#define OID_802_11_MESH_LINK_STATUS                 0x0654
#define OID_802_11_MESH_LIST                        0x0655

#define OID_HOSTAPD_SUPPORT                         0x0661

#define HOSTAPD_OID_STATIC_WEP_COPY                 0x0662
#define HOSTAPD_OID_GET_1X_GROUP_KEY                0x0663

#define HOSTAPD_OID_SET_STA_AUTHORIZED              0x0664
#define HOSTAPD_OID_SET_STA_DISASSOC                0x0665
#define HOSTAPD_OID_SET_STA_DEAUTH                  0x0666
#define HOSTAPD_OID_DEL_KEY                         0x0667
#define HOSTAPD_OID_SET_KEY                         0x0668
#define HOSTAPD_OID_SET_802_1X                      0x0669
#define HOSTAPD_OID_GET_SEQ                         0x0670
#define HOSTAPD_OID_GETWPAIE                        0x0671
#define HOSTAPD_OID_COUNTERMEASURES                 0x0672
#define HOSTAPD_OID_SET_WPAPSK                      0x0673
#define HOSTAPD_OID_SET_WPS_BEACON_IE               0x0674
#define HOSTAPD_OID_SET_WPS_PROBE_RESP_IE           0x0675

#define RT_OID_802_11_QUERY_MAP_REAL_TX_RATE        0x0678
#define RT_OID_802_11_QUERY_MAP_REAL_RX_RATE        0x0679

#define RT_OID_802_11_SNR_2                         0x067A
#define RT_OID_802_11_STREAM_SNR                    0x067B
#define RT_OID_802_11_QUERY_TXBF_TABLE              0x067C
#define RT_OID_802_11_PER_BSS_STATISTICS            0x067D

#define RT_HOSTAPD_OID_HOSTAPD_SUPPORT              (OID_GET_SET_TOGGLE | OID_HOSTAPD_SUPPORT)
#define RT_HOSTAPD_OID_STATIC_WEP_COPY              (OID_GET_SET_TOGGLE | HOSTAPD_OID_STATIC_WEP_COPY)
#define RT_HOSTAPD_OID_GET_1X_GROUP_KEY             (OID_GET_SET_TOGGLE | HOSTAPD_OID_GET_1X_GROUP_KEY)
#define RT_HOSTAPD_OID_SET_STA_AUTHORIZED           (OID_GET_SET_TOGGLE | HOSTAPD_OID_SET_STA_AUTHORIZED)
#define RT_HOSTAPD_OID_SET_STA_DISASSOC             (OID_GET_SET_TOGGLE | HOSTAPD_OID_SET_STA_DISASSOC)
#define RT_HOSTAPD_OID_SET_STA_DEAUTH               (OID_GET_SET_TOGGLE | HOSTAPD_OID_SET_STA_DEAUTH)
#define RT_HOSTAPD_OID_DEL_KEY                      (OID_GET_SET_TOGGLE | HOSTAPD_OID_DEL_KEY)
#define RT_HOSTAPD_OID_SET_KEY                      (OID_GET_SET_TOGGLE | HOSTAPD_OID_SET_KEY)
#define RT_HOSTAPD_OID_SET_802_1X                   (OID_GET_SET_TOGGLE | HOSTAPD_OID_SET_802_1X)
#define RT_HOSTAPD_OID_COUNTERMEASURES              (OID_GET_SET_TOGGLE | HOSTAPD_OID_COUNTERMEASURES)
#define RT_HOSTAPD_OID_SET_WPAPSK                   (OID_GET_SET_TOGGLE | HOSTAPD_OID_SET_WPAPSK)
#define RT_HOSTAPD_OID_SET_WPS_BEACON_IE            (OID_GET_SET_TOGGLE | HOSTAPD_OID_SET_WPS_BEACON_IE)
#define RT_HOSTAPD_OID_SET_WPS_PROBE_RESP_IE        (OID_GET_SET_TOGGLE | HOSTAPD_OID_SET_WPS_PROBE_RESP_IE)


#define RT_OID_802_11_BSSID                         (OID_GET_SET_TOGGLE | OID_802_11_BSSID)
#define RT_OID_802_11_SSID                          (OID_GET_SET_TOGGLE | OID_802_11_SSID)
#define RT_OID_802_11_INFRASTRUCTURE_MODE           (OID_GET_SET_TOGGLE | OID_802_11_INFRASTRUCTURE_MODE)
#define RT_OID_802_11_ADD_WEP                       (OID_GET_SET_TOGGLE | OID_802_11_ADD_WEP)
#define RT_OID_802_11_ADD_KEY                       (OID_GET_SET_TOGGLE | OID_802_11_ADD_KEY)
#define RT_OID_802_11_REMOVE_WEP                    (OID_GET_SET_TOGGLE | OID_802_11_REMOVE_WEP)
#define RT_OID_802_11_REMOVE_KEY                    (OID_GET_SET_TOGGLE | OID_802_11_REMOVE_KEY)
#define RT_OID_802_11_DISASSOCIATE                  (OID_GET_SET_TOGGLE | OID_802_11_DISASSOCIATE)
#define RT_OID_802_11_AUTHENTICATION_MODE           (OID_GET_SET_TOGGLE | OID_802_11_AUTHENTICATION_MODE)
#define RT_OID_802_11_PRIVACY_FILTER                (OID_GET_SET_TOGGLE | OID_802_11_PRIVACY_FILTER)
#define RT_OID_802_11_BSSID_LIST_SCAN               (OID_GET_SET_TOGGLE | OID_802_11_BSSID_LIST_SCAN)
#define RT_OID_802_11_WEP_STATUS                    (OID_GET_SET_TOGGLE | OID_802_11_WEP_STATUS)
#define RT_OID_802_11_RELOAD_DEFAULTS               (OID_GET_SET_TOGGLE | OID_802_11_RELOAD_DEFAULTS)
#define RT_OID_802_11_NETWORK_TYPE_IN_USE           (OID_GET_SET_TOGGLE | OID_802_11_NETWORK_TYPE_IN_USE)
#define RT_OID_802_11_TX_POWER_LEVEL                (OID_GET_SET_TOGGLE | OID_802_11_TX_POWER_LEVEL)
#define RT_OID_802_11_RSSI_TRIGGER                  (OID_GET_SET_TOGGLE | OID_802_11_RSSI_TRIGGER)
#define RT_OID_802_11_FRAGMENTATION_THRESHOLD       (OID_GET_SET_TOGGLE | OID_802_11_FRAGMENTATION_THRESHOLD)
#define RT_OID_802_11_RTS_THRESHOLD                 (OID_GET_SET_TOGGLE | OID_802_11_RTS_THRESHOLD)
#define RT_OID_802_11_RX_ANTENNA_SELECTED           (OID_GET_SET_TOGGLE | OID_802_11_RX_ANTENNA_SELECTED)
#define RT_OID_802_11_TX_ANTENNA_SELECTED           (OID_GET_SET_TOGGLE | OID_802_11_TX_ANTENNA_SELECTED)
#define RT_OID_802_11_SUPPORTED_RATES               (OID_GET_SET_TOGGLE | OID_802_11_SUPPORTED_RATES)
#define RT_OID_802_11_DESIRED_RATES                 (OID_GET_SET_TOGGLE | OID_802_11_DESIRED_RATES)
#define RT_OID_802_11_CONFIGURATION                 (OID_GET_SET_TOGGLE | OID_802_11_CONFIGURATION)
#define RT_OID_802_11_POWER_MODE                    (OID_GET_SET_TOGGLE | OID_802_11_POWER_MODE)
#define RT_OID_802_11_SET_PSPXLINK_MODE             (OID_GET_SET_TOGGLE | OID_802_11_SET_PSPXLINK_MODE)
#define RT_OID_802_11_EAP_METHOD                    (OID_GET_SET_TOGGLE | OID_802_11_EAP_METHOD)
#define RT_OID_802_11_SET_PASSPHRASE                (OID_GET_SET_TOGGLE | OID_802_11_SET_PASSPHRASE)


#define RT_OID_802_DOT1X_PMKID_CACHE                (OID_GET_SET_TOGGLE | OID_802_DOT1X_PMKID_CACHE)
#define RT_OID_802_DOT1X_RADIUS_DATA                (OID_GET_SET_TOGGLE | OID_802_DOT1X_RADIUS_DATA)
#define RT_OID_802_DOT1X_WPA_KEY                    (OID_GET_SET_TOGGLE | OID_802_DOT1X_WPA_KEY)
#define RT_OID_802_DOT1X_STATIC_WEP_COPY            (OID_GET_SET_TOGGLE | OID_802_DOT1X_STATIC_WEP_COPY)
#define RT_OID_802_DOT1X_IDLE_TIMEOUT               (OID_GET_SET_TOGGLE | OID_802_DOT1X_IDLE_TIMEOUT)

#define RT_OID_802_11_SET_TDLS_PARAM                (OID_GET_SET_TOGGLE | RT_OID_802_11_QUERY_TDLS_PARAM)
#define RT_OID_802_11_SET_TDLS                      (OID_GET_SET_TOGGLE | RT_OID_802_11_QUERY_TDLS)

#define OID_802_11_WAPI_PID                         0x06A0
#define OID_802_11_PORT_SECURE_STATE                0x06A1
#define OID_802_11_UCAST_KEY_INFO                   0x06A2
#define OID_802_11_MCAST_TXIV                       0x06A3
#define OID_802_11_MCAST_KEY_INFO                   0x06A4
#define OID_802_11_WAPI_CONFIGURATION               0x06A5
#define OID_802_11_WAPI_IE                          0x06A6

#define RT_OID_802_11_WAPI_PID                      (OID_GET_SET_TOGGLE | OID_802_11_WAPI_PID)
#define RT_OID_802_11_PORT_SECURE_STATE             (OID_GET_SET_TOGGLE | OID_802_11_PORT_SECURE_STATE)
#define RT_OID_802_11_UCAST_KEY_INFO                (OID_GET_SET_TOGGLE | OID_802_11_UCAST_KEY_INFO)
#define RT_OID_802_11_MCAST_TXIV                    (OID_GET_SET_TOGGLE | OID_802_11_MCAST_TXIV)
#define RT_OID_802_11_MCAST_KEY_INFO                (OID_GET_SET_TOGGLE | OID_802_11_MCAST_KEY_INFO)
#define RT_OID_802_11_WAPI_CONFIGURATION            (OID_GET_SET_TOGGLE | OID_802_11_WAPI_CONFIGURATION)
#define RT_OID_802_11_WAPI_IE                       (OID_GET_SET_TOGGLE | OID_802_11_WAPI_IE)


#define RT_OID_802_11_MANUFACTUREROUI               0x0700
#define RT_OID_802_11_MANUFACTURERNAME              0x0701
#define RT_OID_802_11_RESOURCETYPEIDNAME            0x0702

#define RT_OID_802_11_PRIVACYOPTIONIMPLEMENTED      0x0703
#define RT_OID_802_11_POWERMANAGEMENTMODE           0x0704
#define OID_802_11_WEPDEFAULTKEYVALUE               0x0705  /* read , write */
#define OID_802_11_WEPDEFAULTKEYID                  0x0706
#define RT_OID_802_11_WEPKEYMAPPINGLENGTH           0x0707
#define OID_802_11_SHORTRETRYLIMIT                  0x0708
#define OID_802_11_LONGRETRYLIMIT                   0x0709
#define RT_OID_802_11_PRODUCTID                     0x0710
#define RT_OID_802_11_MANUFACTUREID                 0x0711

#define OID_802_11_CURRENTCHANNEL                   0x0712

#define RT_OID_802_11_MAC_ADDRESS                   0x0713
#define OID_802_11_BUILD_CHANNEL_EX                 0x0714
#define OID_802_11_GET_CH_LIST                      0x0715
#define OID_802_11_GET_COUNTRY_CODE                 0x0716
#define OID_802_11_GET_CHANNEL_GEOGRAPHY            0x0717

//#define RT_OID_802_11_STATISTICS                  (OID_GET_SET_TOGGLE | OID_802_11_STATISTICS)

#define RT_OID_WAC_REQ                              0x0736
#define RT_OID_WSC_AUTO_PROVISION_WITH_BSSID        0x0737
#define RT_OID_WSC_AUTO_PROVISION                   0x0738
#define RT_OID_LED_WPS_MODE10                       0x0739
#define RT_OID_APCLI_WSC_PIN_CODE                   0x074A
#define RT_OID_WSC_FRAGMENT_SIZE                    0x074D
#define RT_OID_WSC_V2_SUPPORT                       0x074E
#define RT_OID_WSC_CONFIG_STATUS                    0x074F
#define RT_OID_802_11_WSC_QUERY_PROFILE             0x0750

#define RT_OID_WSC_QUERY_STATUS                     0x0751
#define RT_OID_WSC_PIN_CODE                         0x0752
#define RT_OID_WSC_UUID                             0x0753
#define RT_OID_WSC_SET_SELECTED_REGISTRAR           0x0754
#define RT_OID_WSC_EAPMSG                           0x0755
#define RT_OID_WSC_MANUFACTURER                     0x0756
#define RT_OID_WSC_MODEL_NAME                       0x0757
#define RT_OID_WSC_MODEL_NO                         0x0758
#define RT_OID_WSC_SERIAL_NO                        0x0759
#define RT_OID_WSC_READ_UFD_FILE                    0x075A
#define RT_OID_WSC_WRITE_UFD_FILE                   0x075B
#define RT_OID_WSC_QUERY_PEER_INFO_ON_RUNNING       0x075C
#define RT_OID_WSC_MAC_ADDRESS                      0x0760

#define RT_OID_GET_PHY_MODE                         0x761
#define RT_OID_GET_LLTD_ASSO_TABLE                  0x762
#define RT_OID_GET_REPEATER_AP_LINEAGE              0x763

/* New for MeetingHouse Api support */
#define OID_MH_802_1X_SUPPORTED                     0xFFEDC100

#define RT_OID_802_11_BSSID                         (OID_GET_SET_TOGGLE | OID_802_11_BSSID)
#define RT_OID_802_11_SSID                          (OID_GET_SET_TOGGLE | OID_802_11_SSID)
#define RT_OID_802_11_INFRASTRUCTURE_MODE           (OID_GET_SET_TOGGLE | OID_802_11_INFRASTRUCTURE_MODE)
#define RT_OID_802_11_ADD_WEP                       (OID_GET_SET_TOGGLE | OID_802_11_ADD_WEP)
#define RT_OID_802_11_ADD_KEY                       (OID_GET_SET_TOGGLE | OID_802_11_ADD_KEY)
#define RT_OID_802_11_REMOVE_WEP                    (OID_GET_SET_TOGGLE | OID_802_11_REMOVE_WEP)
#define RT_OID_802_11_REMOVE_KEY                    (OID_GET_SET_TOGGLE | OID_802_11_REMOVE_KEY)
#define RT_OID_802_11_DISASSOCIATE                  (OID_GET_SET_TOGGLE | OID_802_11_DISASSOCIATE)
#define RT_OID_802_11_AUTHENTICATION_MODE           (OID_GET_SET_TOGGLE | OID_802_11_AUTHENTICATION_MODE)
#define RT_OID_802_11_PRIVACY_FILTER                (OID_GET_SET_TOGGLE | OID_802_11_PRIVACY_FILTER)
#define RT_OID_802_11_BSSID_LIST_SCAN               (OID_GET_SET_TOGGLE | OID_802_11_BSSID_LIST_SCAN)
#define RT_OID_802_11_WEP_STATUS                    (OID_GET_SET_TOGGLE | OID_802_11_WEP_STATUS)
#define RT_OID_802_11_RELOAD_DEFAULTS               (OID_GET_SET_TOGGLE | OID_802_11_RELOAD_DEFAULTS)
#define RT_OID_802_11_NETWORK_TYPE_IN_USE           (OID_GET_SET_TOGGLE | OID_802_11_NETWORK_TYPE_IN_USE)
#define RT_OID_802_11_TX_POWER_LEVEL                (OID_GET_SET_TOGGLE | OID_802_11_TX_POWER_LEVEL)
#define RT_OID_802_11_RSSI_TRIGGER                  (OID_GET_SET_TOGGLE | OID_802_11_RSSI_TRIGGER)
#define RT_OID_802_11_FRAGMENTATION_THRESHOLD       (OID_GET_SET_TOGGLE | OID_802_11_FRAGMENTATION_THRESHOLD)
#define RT_OID_802_11_RTS_THRESHOLD                 (OID_GET_SET_TOGGLE | OID_802_11_RTS_THRESHOLD)
#define RT_OID_802_11_RX_ANTENNA_SELECTED           (OID_GET_SET_TOGGLE | OID_802_11_RX_ANTENNA_SELECTED)
#define RT_OID_802_11_TX_ANTENNA_SELECTED           (OID_GET_SET_TOGGLE | OID_802_11_TX_ANTENNA_SELECTED)
#define RT_OID_802_11_SUPPORTED_RATES               (OID_GET_SET_TOGGLE | OID_802_11_SUPPORTED_RATES)
#define RT_OID_802_11_DESIRED_RATES                 (OID_GET_SET_TOGGLE | OID_802_11_DESIRED_RATES)
#define RT_OID_802_11_CONFIGURATION                 (OID_GET_SET_TOGGLE | OID_802_11_CONFIGURATION)
#define RT_OID_802_11_POWER_MODE                    (OID_GET_SET_TOGGLE | OID_802_11_POWER_MODE)

/*******************************************************************************
 * Wireless related definitions from wireless driver
 ******************************************************************************/

#define FIXED_TXMODE_HT         0
#define FIXED_TXMODE_CCK        1
#define FIXED_TXMODE_OFDM       2
#define FIXED_TXMODE_VHT        3

#define MAX_WDS_ENTRY           4

#define WDS_DISABLE_MODE        0
#define WDS_RESTRICT_MODE       1
#define WDS_BRIDGE_MODE         2
#define WDS_REPEATER_MODE       3
#define WDS_LAZY_MODE           4

#define MODE_CCK                0
#define MODE_OFDM               1
#define MODE_HTMIX              2
#define MODE_HTGREENFIELD       3
#define MODE_VHT                4

#define HT_BW_20                0
#define HT_BW_40                1

#define VHT_BW_2040             0
#define VHT_BW_80               1
#define VHT_BW_160              2
#define VHT_BW_8080             3

/*******************************************************************************
 * Wireless related structures from wireless driver
 ******************************************************************************/

typedef enum _RT_802_11_PHY_MODE {
    PHY_11BG_MIXED = 0,
    PHY_11B = 1,
    PHY_11A = 2,
    PHY_11ABG_MIXED = 3,
    PHY_11G = 4,
    PHY_11ABGN_MIXED = 5,   /* both band   5 */
    PHY_11N_2_4G = 6,       /* 11n-only with 2.4G band      6 */
    PHY_11GN_MIXED = 7,     /* 2.4G band      7 */
    PHY_11AN_MIXED = 8,     /* 5G  band       8 */
    PHY_11BGN_MIXED = 9,    /* if check 802.11b.      9 */
    PHY_11AGN_MIXED = 10,   /* if check 802.11b.      10 */
    PHY_11N_5G = 11,        /* 11n-only with 5G band                11 */
    PHY_11VHT_N_ABG_MIXED = 12, /* 12 -> AC/A/AN/B/G/GN mixed */
    PHY_11VHT_N_AG_MIXED = 13, /* 13 -> AC/A/AN/G/GN mixed  */
    PHY_11VHT_N_A_MIXED = 14, /* 14 -> AC/AN/A mixed in 5G band */
    PHY_11VHT_N_MIXED = 15, /* 15 -> AC/AN mixed in 5G band */
    PHY_MODE_MAX,
} RT_802_11_PHY_MODE;

#define RALINK_WIFI_INTF    "ra0"
#define RALINK_WIFI_INTF2   "rai0"

typedef union _MACHTTRANSMIT_SETTING {
    struct  {
        unsigned short  MCS:7;  // MCS
        unsigned short  BW:1;   //channel bandwidth 20MHz or 40 MHz
        unsigned short  ShortGI:1;
        unsigned short  STBC:2; //SPACE
        unsigned short  eTxBF:1;
        unsigned short  rsv:1;
        unsigned short  iTxBF:1;
        unsigned short  MODE:2; // Use definition MODE_xxx.
    } field;
    unsigned short      word;
} MACHTTRANSMIT_SETTING;

typedef struct _RT_802_11_MAC_ENTRY {
    unsigned char           ApIdx;
    unsigned char           Addr[6];
    unsigned char           Aid;
    unsigned char           Psm;     // 0:PWR_ACTIVE, 1:PWR_SAVE
    unsigned char           MimoPs;  // 0:MMPS_STATIC, 1:MMPS_DYNAMIC, 3:MMPS_Enabled
    char                    AvgRssi0;
    char                    AvgRssi1;
    char                    AvgRssi2;
    unsigned int            ConnectedTime;
    MACHTTRANSMIT_SETTING   TxRate;
    unsigned int            LastRxRate;
    int                     StreamSnr[3];
    int                     SoundingRespSnr[3];
#if 0
    short                   TxPER;
    short                   reserved;
#endif
} RT_802_11_MAC_ENTRY;

#define MAX_NUMBER_OF_MAC               32 // if MAX_MBSSID_NUM is 8, this value can't be larger than 211
typedef struct _RT_802_11_MAC_TABLE {
    unsigned long            Num;
    RT_802_11_MAC_ENTRY      Entry[MAX_NUMBER_OF_MAC]; //MAX_LEN_OF_MAC_TABLE = 32
} RT_802_11_MAC_TABLE;

/*******************************************************************************
 * WPS related definitions from wireless driver
 ******************************************************************************/

/* WSC connection mode */
#define WSC_PIN_MODE                1
#define WSC_PBC_MODE                2
#define WSC_SMPBC_MODE              3

/* Wsc status code */
#define STATUS_WSC_NOTUSED                      0
#define STATUS_WSC_IDLE                         1
#define STATUS_WSC_FAIL                         2       /* WSC Process Fail */
#define STATUS_WSC_LINK_UP                      3       /* Start WSC Process */
#define STATUS_WSC_EAPOL_START_RECEIVED         4       /* Received EAPOL-Start */
#define STATUS_WSC_EAP_REQ_ID_SENT              5       /* Sending EAP-Req(ID) */
#define STATUS_WSC_EAP_RSP_ID_RECEIVED          6       /* Receive EAP-Rsp(ID) */
#define STATUS_WSC_EAP_RSP_WRONG_SMI            7       /* Receive EAP-Req with wrong WSC SMI Vendor Id */
#define STATUS_WSC_EAP_RSP_WRONG_VENDOR_TYPE    8       /* Receive EAPReq with wrong WSC Vendor Type */
#define STATUS_WSC_EAP_REQ_WSC_START            9       /* Sending EAP-Req(WSC_START) */
#define STATUS_WSC_EAP_M1_SENT                  10      /* Send M1 */
#define STATUS_WSC_EAP_M1_RECEIVED              11      /* Received M1 */
#define STATUS_WSC_EAP_M2_SENT                  12      /* Send M2 */
#define STATUS_WSC_EAP_M2_RECEIVED              13      /* Received M2 */
#define STATUS_WSC_EAP_M2D_RECEIVED             14      /* Received M2D */
#define STATUS_WSC_EAP_M3_SENT                  15      /* Send M3 */
#define STATUS_WSC_EAP_M3_RECEIVED              16      /* Received M3 */
#define STATUS_WSC_EAP_M4_SENT                  17      /* Send M4 */
#define STATUS_WSC_EAP_M4_RECEIVED              18      /* Received M4 */
#define STATUS_WSC_EAP_M5_SENT                  19      /* Send M5 */
#define STATUS_WSC_EAP_M5_RECEIVED              20      /* Received M5 */
#define STATUS_WSC_EAP_M6_SENT                  21      /* Send M6 */
#define STATUS_WSC_EAP_M6_RECEIVED              22      /* Received M6 */
#define STATUS_WSC_EAP_M7_SENT                  23      /* Send M7 */
#define STATUS_WSC_EAP_M7_RECEIVED              24      /* Received M7 */
#define STATUS_WSC_EAP_M8_SENT                  25      /* Send M8 */
#define STATUS_WSC_EAP_M8_RECEIVED              26      /* Received M8 */
#define STATUS_WSC_EAP_RAP_RSP_ACK              27      /* Processing EAP Response (ACK) */
#define STATUS_WSC_EAP_RAP_REQ_DONE_SENT        28      /* Processing EAP Request (Done) */
#define STATUS_WSC_EAP_RAP_RSP_DONE_SENT        29      /* Processing EAP Response (Done) */
#define STATUS_WSC_EAP_FAIL_SENT                30      /* Sending EAP-Fail */
#define STATUS_WSC_ERROR_HASH_FAIL              31      /* WSC_ERROR_HASH_FAIL */
#define STATUS_WSC_ERROR_HMAC_FAIL              32      /* WSC_ERROR_HMAC_FAIL */
#define STATUS_WSC_ERROR_DEV_PWD_AUTH_FAIL      33      /* WSC_ERROR_DEV_PWD_AUTH_FAIL */
#define STATUS_WSC_CONFIGURED                   34
#define STATUS_WSC_SCAN_AP                      35      /* Scanning AP */
#define STATUS_WSC_EAPOL_START_SENT             36
#define STATUS_WSC_EAP_RSP_DONE_SENT            37
#define STATUS_WSC_WAIT_PIN_CODE                38
#define STATUS_WSC_START_ASSOC                  39
#define STATUS_WSC_IBSS_WAIT_NEXT_SMPBC_ENROLLEE    40
#define STATUS_WSC_IBSS_NEW_RANDOM_PIN          41
#define STATUS_WSC_IBSS_FIXED_PIN               42

/* All error message starting from 0x0100 */
#define STATUS_WSC_PBC_TOO_MANY_AP              0x0101      /* Too many PBC AP avaliable */
#define STATUS_WSC_PBC_NO_AP                    0x0102      /* No PBC AP avaliable */
#define STATUS_WSC_EAP_FAIL_RECEIVED            0x0103      /* Received EAP-FAIL */
#define STATUS_WSC_EAP_NONCE_MISMATCH           0x0104      /* Receive EAP with wrong NONCE */
#define STATUS_WSC_EAP_INVALID_DATA             0x0105      /* Receive EAP without integrity (Hmac mismatch) */
#define STATUS_WSC_PASSWORD_MISMATCH            0x0106      /* Error PIN Code (R-Hash mismatch) */
#define STATUS_WSC_EAP_REQ_WRONG_SMI            0x0107      /* Receive EAP-Req with wrong WPS SMI Vendor Id */
#define STATUS_WSC_EAP_REQ_WRONG_VENDOR_TYPE    0x0108      /* Receive EAPReq with wrong WPS Vendor Type */
#define STATUS_WSC_PBC_SESSION_OVERLAP          0x0109      /* AP PBC session overlap */
#define STATUS_WSC_SMPBC_TOO_MANY_REGISTRAR     0x010a      /* Too many SMPBC Registrars avaliable */
#define STATUS_WSC_EMPTY_IPV4_SUBMASK_LIST      0x010b      /* Empty available IPv4 Submask list */
#define STATUS_WSC_SMPBC_NO_AP                  0x010c      /* No SMPBC AP avaliable */

#define WSC_DISABLE                             0x0
#define WSC_ENROLLEE                            0x1
#define WSC_PROXY                               0x2
#define WSC_REGISTRAR                           0x4
#define WSC_ENROLLEE_PROXY                      (WSC_ENROLLEE | WSC_PROXY)
#define WSC_ENROLLEE_REGISTRAR                  (WSC_ENROLLEE | WSC_REGISTRAR)
#define WSC_PROXY_REGISTRAR                     (WSC_PROXY | WSC_REGISTRAR)
#define WSC_ENROLLEE_PROXY_REGISTRAR            (WSC_ENROLLEE | WSC_PROXY | WSC_REGISTRAR)

/* Simple Config state */
#define WSC_SCSTATE_UNCONFIGURED                0x01
#define WSC_SCSTATE_CONFIGURED                  0x02

/*******************************************************************************
 * WPS related structures from wireless driver
 ******************************************************************************/

/*
 * ripped from driver wsc.h,....ugly
  */
#ifndef PACKED
#define PACKED  __attribute__ ((packed))
#endif
#ifndef USHORT
#define USHORT  unsigned short
#endif
#ifndef UCHAR
#define UCHAR   unsigned char
#endif
typedef struct PACKED _WSC_CONFIGURED_VALUE {
    USHORT WscConfigured; // 1 un-configured; 2 configured
    UCHAR   WscSsid[32 + 1];
    USHORT WscAuthMode; // mandatory, 0x01: open, 0x02: wpa-psk, 0x04: shared, 0x08:wpa, 0x10: wpa2, 0x
    USHORT  WscEncrypType;  // 0x01: none, 0x02: wep, 0x04: tkip, 0x08: aes
    UCHAR   DefaultKeyIdx;
    UCHAR   WscWPAKey[64 + 1];
} WSC_CONFIGURED_VALUE;

typedef struct PACKED _NDIS80211SSID
{
    unsigned int    SsidLength;   // length of SSID field below, in bytes;
                                  // this can be zero.
    unsigned char   Ssid[32]; // SSID information field
} NDIS80211SSID;

// WSC configured credential
typedef struct  _WSC_CREDENTIAL
{
    NDIS80211SSID    SSID;               // mandatory
    USHORT              AuthType;           // mandatory, 1: open, 2: wpa-psk, 4: shared, 8:wpa, 0x10: wpa2, 0x20: wpa-psk2
    USHORT              EncrType;           // mandatory, 1: none, 2: wep, 4: tkip, 8: aes
    UCHAR               Key[64];            // mandatory, Maximum 64 byte
    USHORT              KeyLength;
    UCHAR               MacAddr[6];         // mandatory, AP MAC address
    UCHAR               KeyIndex;           // optional, default is 1
    UCHAR               Rsvd[3];            // Make alignment
}   WSC_CREDENTIAL, *PWSC_CREDENTIAL;


// WSC configured profiles
typedef struct  _WSC_PROFILE
{
#ifndef UINT
#define UINT    unsigned long
#endif
    UINT            ProfileCnt;
    UINT        ApplyProfileIdx;  // add by johnli, fix WPS test plan 5.1.1
    WSC_CREDENTIAL      Profile[8];             // Support up to 8 profiles
}   WSC_PROFILE, *PWSC_PROFILE;

typedef struct PACKED _WSC_PEER_DEV_INFO {
    UCHAR   WscPeerDeviceName[32];
    UCHAR   WscPeerManufacturer[64];
    UCHAR   WscPeerModelName[32];
    UCHAR   WscPeerModelNumber[32];
    UCHAR   WscPeerSerialNumber[32];
    UCHAR   WscPeerMAC[6];
} WSC_PEER_DEV_INFO, *PWSC_PEER_DEV_INFO;

typedef enum _WscSecurityMode{
    WPA2PSKAES,
    WPA2PSKTKIP,
    WPAPSKAES,
    WPAPSKTKIP,
} WSC_SECURITY_MODE;


#endif

