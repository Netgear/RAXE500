/*

*/

#ifndef _SOAPC_DEF_H
#define _SOAPC_DEF_H


#ifndef C_MAX_NUM_ARGS
#define C_MAX_NUM_ARGS  10
#endif
#ifndef C_MAX_ARGS_LEN
#define C_MAX_ARGS_LEN  128//30 Jasmine modiy, 09/09/201930
#endif
#ifndef C_MAX_VAR_SIZE
#define C_MAX_VAR_SIZE 256
#endif

#ifndef ERROR_SOAPC_FWCHECK_UNINIT
#define ERROR_SOAPC_FWCHECK_UNINIT 0xFF
#endif


#define NOT_SOAP -1
#define SOAPC_MSG_Buffer 4096
#define IP_Routed 1
#define IP_Bridged 2

#define NO_Error 0
#define RebootRequired 1

/* Error Code  */  /* @Vista */
#define Bad_Request 400
#define Invalid_Action 401
#define Invail_Args 402
#define Out_of_Sync 403
#define Invalid_Var 404
#define Invalid_MAC 405
#define Precondition 412
#define NosuchDev 430
#define DevNotRegiser 431
#define Unknown_Error 499
#define InternalServerError 500
#define Action_Failed 501
#define OutofMemory 603 
#define StringArgumentTooLong 605
#define ValueSpecifiedIsInvalid 702
#define InactiveConnectionStateRequired      703
#define ConnectionSetupFailed 704
#define ConnectionSetupInProgress 705
#define ConnectionNotConfigured 706
#define DisconnectInProgress 707
#define InvalidLayer2Address 708
#define InternetAccessDisabled 709
#define InvalidConnectionType 710
#define ConnectionAlreadyTerminated 711
#define NullValueAtSpecifiedArrayIndex 712
#define SpecifiedArrayIndexInvalid           713
#define NoSuchEntryInArray                  714
#define WildCardNotPermittedInSrcIP        715
#define WildCardNotPermittedInExtPort      716
#define ConflictInMappingEntry               718
#define ActionDisallowedWhenAutoConfigEnabled 719
#define InvalidDeviceUUID 720
#define InvalidServiceID 721
#define InvalidConnServiceSelection 723
#define SamePortValuesRequired               724
#define OnlyPermanentLeasesSupported         725
#define RemoteHostOnlySupportsWildcard      726
#define ExternalPortOnlySupportsWildcard    727
#define InvalidChannel 728


//Jasmine add, not NTGR spec. 10/14/2016
#define RemoteServerConnectFail 900
#define FailtoGetRemoteFile 901
#define UnknownError 902
#define StillDownloading 903
#define InvalidParameter 1001
#define InvalidMacAddress 1002
#define InvalidAllowOrBlockParameter 1003


#define InvalidOldPassword 1101
#define InvalidNewPassword 1102

#define FirmwareChecksumError 1201
#define Updatenewfirmware2UnknowError 1202


#define ProcessingStillRunning 1300
#define ResultInvalid 1301
#define ActionFailed 1302
#define InvalidMAC 1303
#define ProcessAlreadyRunning 1304
#define ResultAlreadyAvailable 1305
//#define ResultAlreadyAvailable 1306
#define DeviceDisconnect 1307



#define NUM_SOAP_SET_EVENT 18   //Jasmine, 09/08, @Vista
#define argu_length 128 /* must sync with MAX_TOKEN_LEN in upnp.h */ //Jasmine, 9/18, @Vista

#define MAX_SOAP_REQUEST_TIMEOUT 20
#define MAX_SOAP_REQUEST_COUNT 2


#define MAX_CONNECT_TRY_COUNT 6
#define CONNECT_TRY_TIME_PERIOD 5


#define MAX_SOAPC_ARRAR_NUM 64
#define MAX_LEN_SOAP_BUFFER 4096

#define SOAP_RESPONSE_CODE "ResponseCode"

#define ErrorCodeCaptureFail 9900
#define TryThisEventAgain 9901
#define TryThisEventGroupAgain 9902


#endif
