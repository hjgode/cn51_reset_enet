//ndis_ctl.cpp

#include "stdafx.h"
#include "snapi.h"
#include "ntddndis.h"
#include "nuiouser.h"
#include <ndis.h>
//#include "rndis.h"
#include <ndispwr.h>
#include <regext.h>
//USB80231

#define ADAPTERNAME8023 L"USB80231\0\0"

//
//	NIO_CONTEXT
//
//	Structure containing state information necessary to
//	manage configuring/sending/receiving information
//	through the NDISUIO interface.
//
#define EAPOL_MAX_ADAPTER_NAME_LENGTH 64

enum NotifType
{
      ActiveSync = 0,
      NetworkCount,
      NetworkAdapter
};

	HREGNOTIFY hNotify = NULL;
	HREGNOTIFY hNotify1 = NULL;
	HREGNOTIFY hNotify2 = NULL;
	void RegistryNotifyCallbackFunc(HREGNOTIFY hNotify, DWORD dwUserData, const PBYTE pData, const UINT cbData);

void getErrorMsg(DWORD Err){

	return;

	LPWSTR lpMsgBuf; 

	DWORD dwRes = FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM 
		| FORMAT_MESSAGE_FROM_HMODULE
//		| FORMAT_MESSAGE_IGNORE_INSERTS 
		,
		NULL, 
		Err, 
		0, // Default language 
		lpMsgBuf, 
		0, 
		NULL 
		); 

	if(dwRes!=0){
		// Display the string. 
		MessageBox( NULL, lpMsgBuf, L"Error", MB_OK | MB_ICONINFORMATION ); 
	}
	else{
		DEBUGMSG(1, (L"FormatMessage error: %i\r\n", GetLastError()));
	}
	// Free the buffer. 
	LocalFree( lpMsgBuf );
}

TCHAR* addText(TCHAR* txt){
	static TCHAR newTxt[8192];
	wsprintf(newTxt,L"%s", txt);
	return newTxt;
}

void deregisterEnet(TCHAR* szMsg){
	HANDLE hLoaderDev;
	DWORD bytesReturned;
	TCHAR devName[MAX_PATH]={0};;
	_tcscpy(devName, ADAPTERNAME8023);
	DWORD dwErr=0;

	hLoaderDev = CreateFile(_T("NDS0:"), GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hLoaderDev != INVALID_HANDLE_VALUE) 
	{
		if(DeviceIoControl( hLoaderDev, IOCTL_NDIS_DEREGISTER_ADAPTER,  devName, 50, NULL, 0, &bytesReturned, NULL)){
			wsprintf(szMsg, L"80231 deregister OK\r\n");
			DEBUGMSG(1, (L"80231 deregister OK\r\n"));
			szMsg=addText(szMsg);
		}
		else{
			dwErr=GetLastError();
			wsprintf(szMsg, L"80231 deregister failed: %i\r\n", dwErr);
			DEBUGMSG(1, (L"80231 deregister failed: %i\r\n", dwErr));
			szMsg=addText(szMsg);
			getErrorMsg(dwErr);
		}
	}
	else{
		wsprintf(szMsg, L"NDS0: deregisterEnet:InvalidHandle\r\n");
		DEBUGMSG(1, (L"NDS0: deregisterEnet:InvalidHandle\r\n"));
	}
}

void unBindEnet(TCHAR* szMsg){
	HANDLE hLoaderDev;
	DWORD bytesReturned;
	TCHAR devName[MAX_PATH]={0};;
	_tcscpy(devName, ADAPTERNAME8023);
	DWORD dwErr=0;

	hLoaderDev = CreateFile(_T("NDS0:"), GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hLoaderDev != INVALID_HANDLE_VALUE) 
	{
		if(DeviceIoControl( hLoaderDev, IOCTL_NDIS_UNBIND_ADAPTER,  devName, 50, NULL, 0, &bytesReturned, NULL)){
			wsprintf(szMsg, L"80231 unbind OK\r\n");
			szMsg=addText(szMsg);			
			DEBUGMSG(1, (L"80231 unbind OK\r\n"));

		}
		else{
			dwErr=GetLastError();
			wsprintf(szMsg, L"80231 unbind failed: %i\r\n", dwErr);
			DEBUGMSG(1, (L"80231 unbind failed: %i\r\n", dwErr));
				szMsg=addText(szMsg);
			getErrorMsg(dwErr);
		}
		CloseHandle(hLoaderDev);
	}
	else{
		wsprintf(szMsg, L"NDS0: unBindEnet:InvalidHandle\r\n");
		DEBUGMSG(1, (L"NDS0: unBindEnet:InvalidHandle\r\n"));
	}
}

void registerEnet(TCHAR* szMsg){
	HANDLE hLoaderDev;
	DWORD bytesReturned;
	TCHAR devName[MAX_PATH]={0};;
	_tcscpy(devName, ADAPTERNAME8023);
	DWORD dwErr=0;

	hLoaderDev = CreateFile(_T("NDS0:"), GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hLoaderDev != INVALID_HANDLE_VALUE) 
	{
		//IOCTL_NDIS_DEREGISTER_ADAPTER
		if(DeviceIoControl( hLoaderDev, IOCTL_NDIS_REGISTER_ADAPTER,  devName, 50, NULL, 0, &bytesReturned, NULL)){
			wsprintf(szMsg, L"80231 register OK\r\n");
			DEBUGMSG(1, (L"80231 register OK\r\n"));
		}
		else{
			dwErr=GetLastError();
			wsprintf(szMsg, L"80231 register failed: %i\r\n", dwErr);
			DEBUGMSG(1, (L"80231 register failed: %i\r\n", dwErr));
			getErrorMsg(dwErr);
		}
		CloseHandle(hLoaderDev);
	}
	else{
		wsprintf(szMsg, L"NDS0: registerEnet:InvalidHandle\r\n");
		DEBUGMSG(1, (L"NDS0: registerEnetInvalidHandle\r\n"));
	}

}

void bindEnet(TCHAR* szMsg){
	HANDLE hLoaderDev;
	DWORD bytesReturned;
	TCHAR devName[MAX_PATH]={0};;
	_tcscpy(devName, ADAPTERNAME8023);
	DWORD dwErr=0;

	hLoaderDev = CreateFile(_T("NDS0:"), GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hLoaderDev != INVALID_HANDLE_VALUE) 
	{
		//IOCTL_NDIS_DEREGISTER_ADAPTER
		if(DeviceIoControl( hLoaderDev, IOCTL_NDIS_BIND_ADAPTER,  devName, 50, NULL, 0, &bytesReturned, NULL)){
			wsprintf(szMsg, L"80231 bind OK\r\n");
			DEBUGMSG(1, (L"80231 bind OK\r\n"));
		}
		else{
			dwErr=GetLastError();
			wsprintf(szMsg, L"80231 bind failed: %i\r\n", dwErr);
			DEBUGMSG(1, (L"80231 bind failed: %i\r\n", dwErr));
			getErrorMsg(dwErr);
		}

		CloseHandle(hLoaderDev);
	}
	else{
		wsprintf(szMsg, L"NDS0: bindEnet:InvalidHandle\r\n");
		DEBUGMSG(1, (L"NDS0: bindEnet:InvalidHandle\r\n"));
	}
}

void resetUSB80231(TCHAR* szMsg){
	HANDLE hLoaderDev;
	DWORD bytesReturned;
	TCHAR devName[MAX_PATH]={0};;
	_tcscpy(devName, ADAPTERNAME8023);

	hLoaderDev = CreateFile(_T("NDS0:"), GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hLoaderDev != INVALID_HANDLE_VALUE) 
	{
		if(DeviceIoControl( hLoaderDev, IOCTL_NDIS_UNBIND_ADAPTER,  devName, 50, NULL, 0, &bytesReturned, NULL)){
			wsprintf(szMsg, L"80231 unbind OK\r\n");
			DEBUGMSG(1, (L"80231 unbind failed\r\n"));
		}
		else{
			wsprintf(szMsg, L"80231 unbind failed: %i\r\n", GetLastError());
			DEBUGMSG(1, (L"80231 unbind failed: %i\r\n", GetLastError()));
		}
		Sleep(3000);

		if(DeviceIoControl( hLoaderDev, IOCTL_NDIS_BIND_ADAPTER,  devName, 50, NULL, 0, &bytesReturned, NULL)){
			wsprintf(szMsg, L"80231 bind OK\r\n");
			DEBUGMSG(1, (L"80231 bind OK\r\n"));
		}
		else{
			wsprintf(szMsg, L"80231 bind failed: %i\r\n", GetLastError());
			DEBUGMSG(1, (L"80231 bind failed: %i\r\n", GetLastError()));
		}
		CloseHandle(hLoaderDev);
	}
	else{
		wsprintf(szMsg, L"NDS0: InvalidHandle\r\n");
		DEBUGMSG(1, (L"NDS0: InvalidHandle\r\n"));
	}
}

BOOL IsEthernetConnected()
{
	HKEY  hKey1;
	TCHAR RegValue [1024];
	TCHAR Eth_adapter[] = _T("USB80231");
	DWORD dwRegValueSize=sizeof(RegValue);
	DWORD dwRegType;
	BOOL  bEthernet = FALSE;
	Sleep(500);
	DEBUGMSG(1,(_T("=======IsEthernetConnected\r\n")));
	if(ERROR_SUCCESS==RegOpenKeyEx(SN_CONNECTIONSNETWORKADAPTERS_ROOT,SN_CONNECTIONSNETWORKADAPTERS_PATH,0,KEY_ALL_ACCESS,&hKey1))
	{
		if(ERROR_SUCCESS==RegQueryValueEx(hKey1,SN_CONNECTIONSNETWORKADAPTERS_VALUE, NULL, &dwRegType, (LPBYTE)RegValue, &dwRegValueSize))
		{
			if(_tcsstr(RegValue,Eth_adapter)!=NULL)
			{
				DEBUGMSG(1,(_T("=======Found Ethernet is connected, %s\r\n"),RegValue));
				bEthernet = TRUE;
			}
			else
			{
				DEBUGMSG(1,(_T("=======Couldn't Find Ethernet connected, %s\r\n"),RegValue));
				bEthernet = FALSE;
			}
		}
		RegCloseKey(hKey1);
	}
	return bEthernet;
}

void HoldEthernet()
{
	HANDLE hLoaderDev;
	DWORD bytesReturned;
	HRESULT hRes;
	HKEY    hRegKey;
	int   DhcpEnabled=0;
	TCHAR keyName[MAX_PATH]={0};
	TCHAR devName[MAX_PATH]={0};;
	_tcscpy(keyName, _T("Comm\\"));
	_tcscat(keyName, ADAPTERNAME8023);
	_tcscat(keyName, _T("\\Parms\\TcpIp"));
	DEBUGMSG(1,(_T("=======HoldEthernet \r\n")));

	hRes = RegOpenKeyEx(HKEY_LOCAL_MACHINE, keyName, 0, 0, &hRegKey);
	hRes = RegSetValueEx(hRegKey, _T("IpAddress"), 0, REG_SZ, (LPBYTE)_T("127.0.0.1"), 20);
	hRes = RegSetValueEx(hRegKey, _T("Subnetmask"), 0, REG_SZ, (LPBYTE)_T("255.0.0.0"), 20);
	hRes = RegSetValueEx(hRegKey, _T("EnableDHCP"), 0, REG_DWORD, (LPBYTE)&DhcpEnabled, sizeof(int));
	RegCloseKey(hRegKey);

	_tcscpy(devName, ADAPTERNAME8023);

	hLoaderDev = CreateFile(_T("NDS0:"), GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hLoaderDev != INVALID_HANDLE_VALUE) 
	{
		DeviceIoControl( hLoaderDev, IOCTL_NDIS_REBIND_ADAPTER,  devName, 50, NULL, 0, &bytesReturned, NULL);
		CloseHandle(hLoaderDev);
	}
}

void UnholdEthernet()
{
	HANDLE hLoaderDev;
	DWORD bytesReturned;
	HRESULT hRes;
	HKEY    hRegKey;
	int   DhcpEnabled=1;
	TCHAR keyName[MAX_PATH]={0};
	TCHAR devName[MAX_PATH]={0};;
	_tcscpy(keyName, _T("Comm\\"));
	_tcscat(keyName, ADAPTERNAME8023);
	_tcscat(keyName, _T("\\Parms\\TcpIp"));
	DEBUGMSG(1,(_T("=======UnholdEthernet \r\n")));

	hRes = RegOpenKeyEx(HKEY_LOCAL_MACHINE, keyName, 0, 0, &hRegKey);
	hRes = RegSetValueEx(hRegKey, _T("EnableDHCP"), 0, REG_DWORD, (LPBYTE)&DhcpEnabled, sizeof(int));
	RegCloseKey(hRegKey);

	_tcscpy(devName, ADAPTERNAME8023);

	hLoaderDev = CreateFile(_T("NDS0:"), GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hLoaderDev != INVALID_HANDLE_VALUE) 
	{
		// IOCTL_NDIS_UNBIND_ADAPTER
		DeviceIoControl( hLoaderDev, IOCTL_NDIS_REBIND_ADAPTER,  devName, 50, NULL, 0, &bytesReturned, NULL);
		CloseHandle(hLoaderDev);
	}
}

HWND g_hEdit=NULL;
void startRegWatch(HWND hEdit){
	g_hEdit=hEdit;

	HRESULT hr;
	NOTIFICATIONCONDITION Condition;

	Condition.ctComparisonType = REG_CT_ANYCHANGE;
	Condition.dwMask = ~(0); // look at all bits
	Condition.TargetValue.dw = 0; // ignored when REG_CT_ANYCHANGE is selected

	// Register for adapters changes
	//hr = RegistryNotifyCallback(
	//	HKEY_LOCAL_MACHINE,// SN_CONNECTIONSDESKTOPCOUNT_ROOT, 
 //       L"System\\State\\Connections\\Network", // SN_CONNECTIONSDESKTOPCOUNT_PATH, 
 //       L"Adapters", //SN_CONNECTIONSDESKTOPCOUNT_VALUE, 
 //       RegistryNotifyCallbackFunc, 
 //       NetworkAdapter,// ActiveSync, 
	//	NULL, //any change //&Condition, 
 //       &hNotify);

	hr = RegistryNotifyCallback(
		SN_CONNECTIONSDESKTOPCOUNT_ROOT, 
        SN_CONNECTIONSDESKTOPCOUNT_PATH, 
        SN_CONNECTIONSDESKTOPCOUNT_VALUE, 
        RegistryNotifyCallbackFunc, 
        ActiveSync, 
		&Condition, 
        &hNotify1);

	// Register for Netowrk changes
	hr = RegistryNotifyCallback(SN_CONNECTIONSNETWORKCOUNT_ROOT, 
                                SN_CONNECTIONSNETWORKCOUNT_PATH, 
                                SN_CONNECTIONSNETWORKCOUNT_VALUE, 
                                RegistryNotifyCallbackFunc, 
                                NetworkCount, 
			     &Condition, 
                                &hNotify2);

}

// #######################################################
/*
[HKEY_LOCAL_MACHINE\System\State\Connections\Network]"Adapters"="USB80231""Descriptions"="Network Card""Count"=dword:00000001
*/
// The call-back function for Registry Notifications.
void RegistryNotifyCallbackFunc(HREGNOTIFY hNotify, DWORD dwUserData, const PBYTE pData, const UINT cbData)
{
	TCHAR szOutput[MAX_PATH];
	DEBUGMSG(1,(_T("=======Entering RegistryNotifyCallbackFunc\r\n")));
	// Identify the Notification received, based upon the User Data passed in, while registering for the notification.
	switch( dwUserData )
	{
	case ActiveSync:
		// StringCchPrintf(szOutput, MAX_PATH, _T("=======ActiveSync Connection is %d\r\n"), (DWORD) *pData);
		wsprintf(szOutput, L"=>ActiveSync Connection is %d\r\n", (DWORD) *pData);
		//if Activesync is connected then disconnect WLAN and Ethernet
		DEBUGMSG(1,(_T("=======ActiveSync\r\n")));
		PostMessage(g_hEdit, WM_REG_CHANGED, (WPARAM) szOutput, 0);
		break;
	case NetworkCount:
		//StringCchPrintf(szOutput, MAX_PATH, _T("=======Network Connection is %d\r\n"), (DWORD) *pData);
		wsprintf(szOutput, L"=>Network Connection is %d\r\n", (DWORD) *pData);
		PostMessage(g_hEdit, WM_REG_CHANGED, (WPARAM) szOutput, 0);
		DEBUGMSG(1,(_T("=======NetworkCount changed\r\n")));
		break;
	case NetworkAdapter:
		//StringCchPrintf(szOutput, MAX_PATH, _T("=======Network Connection is %d\r\n"), (DWORD) *pData);
		wsprintf(szOutput, L"=>NetworkAdapter changed: %s\r\n", (TCHAR) *pData);
		PostMessage(g_hEdit, WM_REG_CHANGED, (WPARAM) szOutput, 0);
		DEBUGMSG(1,(_T("=======NetworkAdapter changed\r\n")));
		break;
	default :
		break;
	}
	OutputDebugString( szOutput );
	return;
}
