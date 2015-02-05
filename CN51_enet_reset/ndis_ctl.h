//ndis_ctl.h

#include "stdafx.h"

#ifndef _NDIS_CTL_H_
#define _NDIS_CTL_H_

#define WM_REG_CHANGED WM_USER +1
void startRegWatch(HWND hEdit);

BOOL IsEthernetConnected();
void HoldEthernet();
void UnholdEthernet();
//void resetUSB80231(TCHAR* szMsg);
void bindEnet(TCHAR* szMsg);
void unBindEnet(TCHAR* szMsg);
void deregisterEnet(TCHAR* szMsg);
void registerEnet(TCHAR* szMsg);

#endif