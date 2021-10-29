/* //device/system/reference-ril/atchannel.h
**
** Copyright 2006, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/
#ifndef SHARED_CMIF_H
#define SHARED_CMIF_H

#define DEVICE_REV_ID_MAX_LEN 50
#define SIM_STATUS_MAX_LEN 50

enum {
	SHARED_CMIF_API_MSG_OPEN_FAIL = 0,
	SHARED_CMIF_API_MSG_SEND_FAIL,
	SHARED_CMIF_API_RECV_FAIL,
	SHARED_CMIF_API_SUCCESS,
	SHARED_CMIF_API_GET_FAIL,
};

int shared_cmif_set_timezone(int set_num);

// Foxconn add start, Edward, CMIF shared API, 12-17-2019
int shared_RA_get_mobileHWVersion(char *get_val);

int shared_RA_get_mobileFWVersion(char *get_val);

int shared_RA_get_mobileNetworkMCC(char *get_val);

int shared_RA_get_mobileNetworkMNC(char *get_val);

int shared_RA_get_mobileNetworkCellID(char *get_val);

int shared_RA_get_mobileRSSI(char *get_val);

int shared_RA_get_mobileRSRP(char *get_val);

int shared_RA_get_mobileRSRQ(char *get_val);

int shared_RA_get_mobileRSSINR(char *get_val);

int shared_RA_get_mobileRadioBand(char *get_val);

int shared_RA_get_mobileChannelNumber(char *get_val);

int shared_RA_get_mobileACtiveNetwork(char *get_val);

int shared_RA_get_mobileNetworkMode(char *get_val);//RA WEB; auto 3Gonly 4Gonly

int shared_RA_set_mobileNetworkMode(char *set_mode);//RA WEB; auto 3Gonly 4Gonly

int shared_RA_get_mobileNetworkOperatorName(char *get_val);
// Foxconn add end, Edward, CMIF shared API, 12-17-2019

// Foxconn add start, Edward, for IMEI, 12-17-2019
int shared_RA_get_mobileIMEI(char *get_val);
// Foxconn add start, Edward, for IMEI, 12-17-2019

// Foxconn add start, YuCheng, for IMSI, 12-17-2019
int shared_RA_get_mobileIMSI(char *get_val);
// Foxconn add end, YuCheng, for IMSI, 12-17-2019

// Foxconn add start, Edward, for ICCID, 12-18-2019
int shared_RA_get_mobileSimICCID(char *get_val);
// Foxconn add start, Edward, for ICCID, 12-18-2019

// Foxconn add start, Edward, for IPv4Address, 12-19-2019
int shared_RA_get_mobileIPv4Address(char *get_val);
// Foxconn add start, Edward, for IPv4Address, 12-19-2019

// Foxconn add start, Edward, for PDP APN 4gMode, 12-20-2019
int shared_RA_get_mobileNetworkPDPType(char *get_val);

int shared_RA_get_mobileNetworkAPN(char *get_val);

int shared_RA_get_mobile4gMode(char *get_val);

// Foxconn add start, Edward, for get mobileNetworkLAC used, 12-24-2019
int shared_RA_get_mobileNetworkLAC(char *get_val);
// Foxconn add end, Edward, for get mobileNetworkLAC used, 12-24-2019
// Foxconn add start, Edward, for get mobileInternationalRoam used, 12-24-2019
int shared_RA_get_mobileInternationalRoam(char *get_val);//RA SOAP
int shared_RA_set_mobileInternationalRoam(char *set_mode);//RA SOAP
// Foxconn add end, Edward, for get mobileInternationalRoam used, 12-24-2019

//int shared_SOAP_GetNetworkMode(char *get_val);
// Foxconn add start, Edward, for Get/SetNetworkMode and ProvideList, 12-24-2019
int shared_SOAP_GetNetworkMode(int *ret_val);
int shared_SOAP_SetNetworkMode(int set_num);
typedef struct ProvideList{
    char LongName[20];
    char ShortName[20];
    char Plmn[20];
	int Stat;
	int Act;
}ProvideList;
// Foxconn add end, Edward, Get/SetNetworkMode and ProvideList, 12-24-2019

// Foxconn add start, Yu Cheng, for getting number of attempts left to enter PIN or PUK , 12-25-2019
typedef enum {
	SHARED_CMIF_PINPUKC_TYPE_PIN1 = 0,
	SHARED_CMIF_PINPUKC_TYPE_PUK1,
	SHARED_CMIF_PINPUKC_TYPE_PIN2,
	SHARED_CMIF_PINPUKC_TYPE_PUK2
}PINPUKC_type;

int shared_WEBUI_get_mobilePINPUKC(int PINPUKC[]);
// Foxconn add end, Yu Cheng, for getting number of attempts left to enter PIN or PUK , 12-25-2019
// Foxconn add start, Yu Cheng, Enter SIM PIN, 12-26-2019
int shared_WEBUI_set_EntermobilePIN(char PIN[]);
// Foxconn add end, Yu Cheng, Enter SIM PIN, 12-26-2019
// Foxconn add start, Yu Cheng, Enter SIM PUK and set new SIM PIN, 12-26-2019
int shared_WEBUI_set_EntermobilePINPUK(char PUK[], char newPIN[]);
// Foxconn add end, Yu Cheng, Enter SIM PUK and set new SIM PIN, 12-26-2019

typedef struct __shared_cmif_profile {
    const char apn[50];
    const char user[50];
    const char password[50];
    const char pincode[50];
} shared_cmif_profile;

int shared_cmif_get_device_rev_id(char** rev_id);

int shared_cmif_get_profile_settings(shared_cmif_profile* profile);

int shared_cmif_modify_profile_settings(shared_cmif_profile profile);

int shared_cmif_start_network_interface(void);

int shared_cmif_stop_network_interface(void);

int shared_cmif_get_sim_status(char** sim_status);

int shared_cmif_get_connection_status(int* connection_status);

#endif
