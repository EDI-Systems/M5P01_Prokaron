//
//      Copyright(C) 2001, 2019 Renesas Electronics Corporation
//      Renesas CONFIDENTIAL AND PROPRIETARY   
//      All rights reserved by Renesas Electronics Corporation.
//      Use of copyright notice does not evidence publication.
//

//////////////////////////////////////////////////////////////////////////
//	User-Open-Interface System Link Utility. 			//
//////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include <string>
extern "C" {
#include "suo.h"
}
#define MYNAME		"suolink.cpp"
#define	USEROPEN	"useropen.dll"


using namespace std;
//
//	SuoLink Class
//
class SuoLink
{
public:
	SuoLink();
	~SuoLink();
private:
	HINSTANCE hLibrary;
	struct FuncTable {
		char* name;
		FARPROC* func;
	};
	static FuncTable funcTable[];
	void link();
	void unlink();
};


//
//	Auto link on load
//
SuoLink SuoLinkObject;

//SIMPILS_LV2 : SM+ からの強制ブレーク機能　段階的に対応する為、LV3を追加した
//SIMPILS_LV3 : ユーザオープンを使ったメモリR/W
//
//	Pointer of Entry
//
// Common
//#define	SuoEnt1		__declspec(dllimport)
#define	SuoEnt1
//#define	SuoEnt2		WINAPI
#define	SuoEnt2
// Basic interface
static SuoEnt1 void (SuoEnt2 *pSuoSetInitCallback)(SuoInitCallback func);
static SuoEnt1 void (SuoEnt2 *pSuoSetResetCallback)(SuoResetCallback func);
#if defined(SIMPILS_LV2)
static SuoEnt1 int (SuoEnt2 *pSuoSetRequestEnforcedBreakCallback)(SuoRequestEnforcedBreakCallback func);
static SuoEnt1 DWORD (SuoEnt2 *pSuoGetParentPathName)(LPTSTR lpFilename,DWORD nSize);
#endif
static SuoEnt1 int (SuoEnt2 *pSuoGetMainClock)(unsigned long* time);
#if defined(SIMPILS_LV3)
static SuoEnt1 int (SuoEnt2 *pSuoReadMemory)(unsigned long addr, unsigned short size, unsigned char data[]);
static SuoEnt1 int (SuoEnt2 *pSuoWriteMemory)(unsigned long addr, unsigned short size, const unsigned char data[]);
#endif
// Timer interface
static SuoEnt1 int (SuoEnt2 *pSuoCreateTimer)(const char* timerName, SuoHandle* handle);
static SuoEnt1 SuoHandle (SuoEnt2 *pSuoGetTimerHandle)(const char* timerName);
static SuoEnt1 int (SuoEnt2 *pSuoSetTimer)(SuoHandle handle, int timeUnit, unsigned long timeValue);
static SuoEnt1 int (SuoEnt2 *pSuoKillTimer)(SuoHandle handle);
static SuoEnt1 int (SuoEnt2 *pSuoSetNotifyTimerCallback)(SuoHandle handle, SuoNotifyTimerCallback func);
// Pin interface
#if defined(SIMPILS_LV1)
static SuoEnt1 int (SuoEnt2 *pSuoEnableDevicePin)(void);
#endif
static SuoEnt1 int (SuoEnt2 *pSuoCreatePin)(const char* pinName, SuoHandle* handle);
#if defined(SIMPILS_LV1)
static SuoEnt1 int (SuoEnt2 *pSuoOpenPin)(const char* pinName, SuoHandle* handle);
#endif
#if defined(SIMPILS_LV1)
static SuoEnt1 int (SuoEnt2 *pSuoClosePin)(SuoHandle handle);
#endif
static SuoEnt1 SuoHandle (SuoEnt2 *pSuoGetPinHandle)(const char* pinName);
static SuoEnt1 int (SuoEnt2 *pSuoOutputDigitalPin)(SuoHandle handle, int pinValue);
static SuoEnt1 int (SuoEnt2 *pSuoOutputAnalogPin)(SuoHandle handle, double pinValue);
static SuoEnt1 int (SuoEnt2 *pSuoOutputHighImpedance)(SuoHandle handle);
static SuoEnt1 int (SuoEnt2 *pSuoSetInputDigitalPinCallback)(SuoHandle handle, SuoInputDigitalPinCallback func);
static SuoEnt1 int (SuoEnt2 *pSuoSetInputAnalogPinCallback)(SuoHandle handle, SuoInputAnalogPinCallback func);
static SuoEnt1 int (SuoEnt2 *pSuoSetInputHighImpedanceCallback)(SuoHandle handle, SuoInputHighImpedanceCallback func);
// Extbus interface
static SuoEnt1 int (SuoEnt2 *pSuoCreateExtbus)(const char* extbusName, unsigned long addr, unsigned long size, SuoHandle* handle);
static SuoEnt1 SuoHandle (SuoEnt2 *pSuoGetExtbusHandle)(const char* extbusName);
static SuoEnt1 int (SuoEnt2 *pSuoSetReadExtbusCallback)(SuoHandle handle, SuoReadExtbusCallback func);
static SuoEnt1 int (SuoEnt2 *pSuoSetWriteExtbusCallback)(SuoHandle handle, SuoWriteExtbusCallback func);
// Serial interface
static SuoEnt1 int (SuoEnt2 *pSuoCreateSerialUART)(const char* serialName, const char* pinNameTXD, const char* pinNameRXD, SuoHandle* handle);
static SuoEnt1 int (SuoEnt2 *pSuoCreateSerialCSI)(const char* serialName, const char* pinNameSO, const char* pinNameSI, const char* pinNameSCK, SuoHandle* handle);
// for iic [7/24/2015 wang-lei]
static SuoEnt1 int (SuoEnt2 *pSuoCreateSerialIIC)(const char* serialName, const char* pinNameSCL, const char* pinNameSDA, SuoHandle* handle);
static SuoEnt1 SuoHandle (SuoEnt2 *pSuoGetSerialHandle)(const char* serialName);
static SuoEnt1 int (SuoEnt2 *pSuoSetSerialParameterUART)(SuoHandle handle, const SuoSerialParameterUART* param);
static SuoEnt1 int (SuoEnt2 *pSuoSetSerialParameterCSI)(SuoHandle handle, const SuoSerialParameterCSI* param);
// for iic [7/24/2015 wang-lei]
static SuoEnt1 int (SuoEnt2 *pSuoSetSerialParameterIIC)(SuoHandle handle, const SuoSerialParameterIIC* param);
static SuoEnt1 int (SuoEnt2 *pSuoGetSerialParameterIIC)(SuoHandle handle, SuoSerialParameterIIC* param);
static SuoEnt1 int (SuoEnt2 *pSuoGetSerialParameterUART)(SuoHandle handle, SuoSerialParameterUART* param);
static SuoEnt1 int (SuoEnt2 *pSuoGetSerialParameterCSI)(SuoHandle handle, SuoSerialParameterCSI* param);
static SuoEnt1 int (SuoEnt2 *pSuoSendSerialData)(SuoHandle handle, unsigned long data);
static SuoEnt1 int (SuoEnt2 *pSuoSendSerialDataList)(SuoHandle handle, long count, unsigned long dataList[]);
static SuoEnt1 int (SuoEnt2 *pSuoSendSerialFile)(SuoHandle handle, const char* serialFile);
static SuoEnt1 int (SuoEnt2 *pSuoSetNotifySentSerialCallback)(SuoHandle handle, SuoNotifySentSerialCallback func);
static SuoEnt1 int (SuoEnt2 *pSuoSetReceiveSerialCallback)(SuoHandle handle, SuoReceiveSerialCallback func);
// for iic [7/24/2015 wang-lei]
static SuoEnt1 int (SuoEnt2 *pSuoStartIIC)(SuoHandle handle);
static SuoEnt1 int (SuoEnt2 *pSuoStopIIC)(SuoHandle handle);
static SuoEnt1 int (SuoEnt2 *pSuoAddressingIIC)(SuoHandle handle);
static SuoEnt1 int (SuoEnt2 *pSuoReceiveIICData)(SuoHandle handle);
// for iic callback [9/6/2015 zhang-dan]
static SuoEnt1 int (SuoEnt2 *pSuoSetEnableSerialCallback)(SuoHandle handle, SuoEnableSerialCallback func);
static SuoEnt1 int (SuoEnt2 *pSuoSetStopSerialCallback)(SuoHandle handle, SuoStopSerialCallback func);
static SuoEnt1 int (SuoEnt2 *pSuoSetAddressSerialCallback)(SuoHandle handle, SuoAddressSerialCallback func);
// Wave interface
static SuoEnt1 int (SuoEnt2 *pSuoCreateWave)(const char* waveName, int count, const char* pinNameList[], SuoHandle* handle);
static SuoEnt1 SuoHandle (SuoEnt2 *pSuoGetWaveHandle)(const char* waveName);
static SuoEnt1 int (SuoEnt2 *pSuoSendWaveFile)(SuoHandle handle, const char* waveFile);
static SuoEnt1 int (SuoEnt2 *pSuoSetNotifySentWaveCallback)(SuoHandle handle, SuoNotifySentWaveCallback func);
// GUI interface
static SuoEnt1 int (SuoEnt2 *pSuoSetUserOpenGuiIF)(SuoHandle handle);
// [1/24/2008 chengpenghui]
static SuoEnt1 void (SuoEnt2 *pSuoSetUseropenIFPtr)(struct UserOpenIF* uo_if);
static SuoEnt1 UserOpenIF* (SuoEnt2 *pSuoGetUseropenIFPtr)();

// send string [10/27/2008 caojiancang]
static SuoEnt1 int (SuoEnt2 *pSuoSendString)(unsigned int id, const char *message);
static SuoEnt1 int (SuoEnt2 *pSuoSetSendStringCallback)(unsigned int id, SuoSendStringCallback func);

//  [11/10/2008 caojiancang]
static SuoEnt1 int (SuoEnt2 *pSuoStartElapsedTimeCounter)(unsigned char id);
static SuoEnt1 int (SuoEnt2 *pSuoCurrentElapsedTimeCounter)(unsigned char id, unsigned long *count);
static SuoEnt1 int (SuoEnt2 *pSuoDeleteElapsedTimeCounter)(unsigned char id);

// stop CPU [4/20/2009 chengpenghui]
static SuoEnt1 int (SuoEnt2 *pSuoStop)();

static SuoEnt1 int (SuoEnt2 *pSuoSetCallbackMode)(unsigned short Mode);
static SuoEnt1 int (SuoEnt2 *pSuoGetDigitalPin)(SuoHandle handle, unsigned long *pinValue);
static SuoEnt1 int (SuoEnt2 *pSuoGetAnalogPin)(SuoHandle handle, double *pinValue);
static SuoEnt1 int (SuoEnt2 *pSuoSetRequestEnforcedStartCallback)(SuoRequestEnforcedStartCallback func);

//
//	Table of Entry
//
SuoLink::FuncTable SuoLink::funcTable[] = {
	// Basic interface
	"SuoSetInitCallback",			(FARPROC*)&pSuoSetInitCallback,
	"SuoSetResetCallback",			(FARPROC*)&pSuoSetResetCallback,
#if defined(SIMPILS_LV2)
	"SuoSetRequestEnforcedBreakCallback",	(FARPROC*)&pSuoSetRequestEnforcedBreakCallback,
        "SuoGetParentPathName",                 (FARPROC*)&pSuoGetParentPathName,
#endif
	"SuoGetMainClock",			(FARPROC*)&pSuoGetMainClock,
#if defined(SIMPILS_LV3)
	"SuoReadMemory",			(FARPROC*)&pSuoReadMemory,
#endif
#if defined(SIMPILS_LV3)
	"SuoWriteMemory",			(FARPROC*)&pSuoWriteMemory,
#endif
	// Timer interface
	"SuoCreateTimer",			(FARPROC*)&pSuoCreateTimer,
	"SuoGetTimerHandle",			(FARPROC*)&pSuoGetTimerHandle,
	"SuoSetTimer",				(FARPROC*)&pSuoSetTimer,
	"SuoKillTimer",				(FARPROC*)&pSuoKillTimer,
	"SuoSetNotifyTimerCallback",		(FARPROC*)&pSuoSetNotifyTimerCallback,
	// Pin interface
#if defined(SIMPILS_LV1)
	"SuoEnableDevicePin",			(FARPROC*)&pSuoEnableDevicePin,
#endif
	"SuoCreatePin",				(FARPROC*)&pSuoCreatePin,
#if defined(SIMPILS_LV1)
	"SuoOpenPin",				(FARPROC*)&pSuoOpenPin,
#endif
#if defined(SIMPILS_LV1)
	"SuoClosePin",				(FARPROC*)&pSuoClosePin,
#endif
	"SuoGetPinHandle",			(FARPROC*)&pSuoGetPinHandle,
	"SuoOutputDigitalPin",			(FARPROC*)&pSuoOutputDigitalPin,
	"SuoOutputAnalogPin",			(FARPROC*)&pSuoOutputAnalogPin,
	"SuoOutputHighImpedance",		(FARPROC*)&pSuoOutputHighImpedance,
	"SuoSetInputDigitalPinCallback",	(FARPROC*)&pSuoSetInputDigitalPinCallback,
	"SuoSetInputAnalogPinCallback",		(FARPROC*)&pSuoSetInputAnalogPinCallback,
	"SuoSetInputHighImpedanceCallback",	(FARPROC*)&pSuoSetInputHighImpedanceCallback,
	// Extbus interface
	"SuoCreateExtbus",			(FARPROC*)&pSuoCreateExtbus,
	"SuoGetExtbusHandle",			(FARPROC*)&pSuoGetExtbusHandle,
	"SuoSetReadExtbusCallback",		(FARPROC*)&pSuoSetReadExtbusCallback,
	"SuoSetWriteExtbusCallback",		(FARPROC*)&pSuoSetWriteExtbusCallback,
	// Serial interface
	"SuoCreateSerialUART",			(FARPROC*)&pSuoCreateSerialUART,
	"SuoCreateSerialCSI",			(FARPROC*)&pSuoCreateSerialCSI,
	// for iic [7/24/2015 wang-lei]
	"SuoCreateSerialIIC",			(FARPROC*)&pSuoCreateSerialIIC,
	"SuoGetSerialHandle",			(FARPROC*)&pSuoGetSerialHandle,
	"SuoSetSerialParameterUART",		(FARPROC*)&pSuoSetSerialParameterUART,
	"SuoSetSerialParameterCSI",		(FARPROC*)&pSuoSetSerialParameterCSI,
	"SuoGetSerialParameterUART",		(FARPROC*)&pSuoGetSerialParameterUART,
	"SuoGetSerialParameterCSI",		(FARPROC*)&pSuoGetSerialParameterCSI,
	// for iic [7/24/2015 wang-lei]
	"SuoSetSerialParameterIIC",		(FARPROC*)&pSuoSetSerialParameterIIC,
	"SuoGetSerialParameterIIC",		(FARPROC*)&pSuoGetSerialParameterIIC,
	"SuoSendSerialData",			(FARPROC*)&pSuoSendSerialData,
	"SuoSendSerialDataList",		(FARPROC*)&pSuoSendSerialDataList,
	"SuoSendSerialFile",			(FARPROC*)&pSuoSendSerialFile,
	"SuoSetNotifySentSerialCallback",	(FARPROC*)&pSuoSetNotifySentSerialCallback,
	"SuoSetReceiveSerialCallback",		(FARPROC*)&pSuoSetReceiveSerialCallback,
	// for iic [7/24/2015 wang-lei]
	"SuoStartIIC",		(FARPROC*)&pSuoStartIIC,
	"SuoStopIIC",		(FARPROC*)&pSuoStopIIC,
	"SuoAddressingIIC",		(FARPROC*)&pSuoAddressingIIC,
	"SuoReceiveIICData",		(FARPROC*)&pSuoReceiveIICData,
  // for iic callback [9/6/2015 zhang-dan]
  "SuoSetEnableSerialCallback",		(FARPROC*)&pSuoSetEnableSerialCallback,
  "SuoSetStopSerialCallback",		(FARPROC*)&pSuoSetStopSerialCallback,
  "SuoSetAddressSerialCallback",		(FARPROC*)&pSuoSetAddressSerialCallback,
	// Wave interface
	"SuoCreateWave",			(FARPROC*)&pSuoCreateWave,
	"SuoGetWaveHandle",			(FARPROC*)&pSuoGetWaveHandle,
	"SuoSendWaveFile",			(FARPROC*)&pSuoSendWaveFile,
	"SuoSetNotifySentWaveCallback",		(FARPROC*)&pSuoSetNotifySentWaveCallback,
	"SuoSetUserOpenGuiIF",			(FARPROC*)&pSuoSetUserOpenGuiIF,
  "SuoSetUseropenIFPtr",			(FARPROC*)&pSuoSetUseropenIFPtr,
  "SuoGetUseropenIFPtr",			(FARPROC*)&pSuoGetUseropenIFPtr,
  //  [10/27/2008 caojiancang]
  "SuoSendString",            (FARPROC*)&pSuoSendString,
  "SuoSetSendStringCallback",            (FARPROC*)&pSuoSetSendStringCallback,
  "SuoStartElapsedTimeCounter",          (FARPROC*)&pSuoStartElapsedTimeCounter,
  "SuoCurrentElapsedTimeCounter",        (FARPROC*)&pSuoCurrentElapsedTimeCounter,
  "SuoDeleteElapsedTimeCounter",         (FARPROC*)&pSuoDeleteElapsedTimeCounter,
  // Stop Cpu [4/20/2009 chengpenghui]
  "SuoStop",                  (FARPROC*)&pSuoStop,
  // TOTO new APIs [7/2/2010 chengpenghui]
  "SuoSetCallbackMode",     (FARPROC*)&pSuoSetCallbackMode,
  "SuoGetDigitalPin",       (FARPROC*)&pSuoGetDigitalPin,
  "SuoGetAnalogPin",        (FARPROC*)&pSuoGetAnalogPin,
  "SuoSetRequestEnforcedStartCallback",   (FARPROC*)&pSuoSetRequestEnforcedStartCallback,
	NULL,					NULL
};

//
//	Link and Unlink
//
SuoLink::SuoLink()
{
	link();
}

SuoLink::~SuoLink()
{
	unlink();
}
	
void SuoLink::link()
{
	char filepath[MAX_PATH+1] = {0};
  ::GetModuleFileName(GetModuleHandle("init.dll"), filepath, MAX_PATH);
    string path(filepath);
    int pos = path.rfind('\\');
    path = path.substr(0,pos+1);
    path = path.append("common\\");
	if(!(hLibrary = ::LoadLibraryEx((path.append(USEROPEN).c_str()), NULL, LOAD_WITH_ALTERED_SEARCH_PATH))){
	// if(!(hLibrary = ::LoadLibrary(USEROPEN))){
		char message[128];
		::wsprintf(message, "%s: Cannot load library (%s)", MYNAME, USEROPEN);
		::MessageBox(NULL, message, "ERROR", MB_OK|MB_ICONERROR);
		return;
	}

	FARPROC func;
	for(FuncTable *tp = funcTable; tp->name; tp++){
		if(!(func = ::GetProcAddress(hLibrary, tp->name))){
			char message[128];
			::wsprintf(message, "%s: Cannot link function (%s)", MYNAME, tp->name);
			::MessageBox(NULL, message, "ERROR", MB_OK|MB_ICONERROR);
			return;
		}
		*(tp->func) = func;
	}
}

void SuoLink::unlink()
{
	if(hLibrary){
		::FreeLibrary(hLibrary);
	}
}


//
//	Interface function of Entry
//
extern "C" {

// Basic interface
void SuoSetInitCallback(SuoInitCallback func)
{
	(*pSuoSetInitCallback)(func);
}
void SuoSetResetCallback(SuoResetCallback func)
{
	(*pSuoSetResetCallback)(func);
}
#if defined(SIMPILS_LV2)
int SuoSetRequestEnforcedBreakCallback(SuoRequestEnforcedBreakCallback func)
{
	return (*pSuoSetRequestEnforcedBreakCallback)(func);
}

DWORD SuoGetParentPathName(LPTSTR lpFilename,DWORD nSize)
{
        return (*pSuoGetParentPathName)(lpFilename, nSize);
}
#endif
int SuoGetMainClock(unsigned long* time)
{
	return (*pSuoGetMainClock)(time);
}
#if defined(SIMPILS_LV3)
int SuoReadMemory(unsigned long addr, unsigned short size, unsigned char data[])
{
	return (*pSuoReadMemory)(addr, size, data);
}
#endif
#if defined(SIMPILS_LV3)
int SuoWriteMemory(unsigned long addr, unsigned short size, const unsigned char data[])
{
	return (*pSuoWriteMemory)(addr, size, data);
}
#endif
// Timer interface
int SuoCreateTimer(const char* timerName, SuoHandle* handle)
{
	return (*pSuoCreateTimer)(timerName, handle);
}
SuoHandle SuoGetTimerHandle(const char* timerName)
{
	return (*pSuoGetTimerHandle)(timerName);
}
int SuoSetTimer(SuoHandle handle, int timeUnit, unsigned long timeValue)
{
	return (*pSuoSetTimer)(handle, timeUnit, timeValue);
}
int SuoKillTimer(SuoHandle handle)
{
	return (*pSuoKillTimer)(handle);
}
int SuoSetNotifyTimerCallback(SuoHandle handle, SuoNotifyTimerCallback func)
{
	return (*pSuoSetNotifyTimerCallback)(handle, func);
}
// Pin interface
#if defined(SIMPILS_LV1)
int SuoEnableDevicePin(void)
{
	return (*pSuoEnableDevicePin)();
}
#endif
int SuoCreatePin(const char* pinName, SuoHandle* handle)
{
	return (*pSuoCreatePin)(pinName, handle);
}
#if defined(SIMPILS_LV1)
int SuoOpenPin(const char* pinName, SuoHandle* handle)
{
	return (*pSuoOpenPin)(pinName, handle);
}
#endif
#if defined(SIMPILS_LV1)
int SuoClosePin(SuoHandle handle)
{
	return (*pSuoClosePin)(handle);
}
#endif
SuoHandle SuoGetPinHandle(const char* pinName)
{
	return (*pSuoGetPinHandle)(pinName);
}
int SuoOutputDigitalPin(SuoHandle handle, int pinValue)
{
	return (*pSuoOutputDigitalPin)(handle, pinValue);
}
int SuoOutputAnalogPin(SuoHandle handle, double pinValue)
{
	return (*pSuoOutputAnalogPin)(handle, pinValue);
}
int SuoOutputHighImpedance(SuoHandle handle)
{
	return (*pSuoOutputHighImpedance)(handle);
}
int SuoSetInputDigitalPinCallback(SuoHandle handle, SuoInputDigitalPinCallback func)
{
	return (*pSuoSetInputDigitalPinCallback)(handle, func);
}
int SuoSetInputAnalogPinCallback(SuoHandle handle, SuoInputAnalogPinCallback func)
{
	return (*pSuoSetInputAnalogPinCallback)(handle, func);
}
int SuoSetInputHighImpedanceCallback(SuoHandle handle, SuoInputHighImpedanceCallback func)
{
	return (*pSuoSetInputHighImpedanceCallback)(handle, func);
}
// Extbus interface
int SuoCreateExtbus(const char* extbusName, unsigned long addr, unsigned long size, SuoHandle* handle)
{
	return (*pSuoCreateExtbus)(extbusName, addr, size, handle);
}
SuoHandle SuoGetExtbusHandle(const char* extbusName)
{
	return (*pSuoGetExtbusHandle)(extbusName);
}
int SuoSetReadExtbusCallback(SuoHandle handle, SuoReadExtbusCallback func)
{
	return (*pSuoSetReadExtbusCallback)(handle, func);
}
int SuoSetWriteExtbusCallback(SuoHandle handle, SuoWriteExtbusCallback func)
{
	return (*pSuoSetWriteExtbusCallback)(handle, func);
}
// Serial interface
int SuoCreateSerialUART(const char* serialName, const char* pinNameTXD, const char* pinNameRXD, SuoHandle* handle)
{
	return (*pSuoCreateSerialUART)(serialName, pinNameTXD, pinNameRXD, handle);
}
int SuoCreateSerialCSI(const char* serialName, const char* pinNameSO, const char* pinNameSI, const char* pinNameSCK, SuoHandle* handle)
{
	return (*pSuoCreateSerialCSI)(serialName, pinNameSO, pinNameSI, pinNameSCK, handle);
}
// for iic [7/24/2015 wang-lei]
int SuoCreateSerialIIC(const char* serialName, const char* pinNameSCL, const char* pinNameSDA, SuoHandle* handle)
{
	return (*pSuoCreateSerialIIC)(serialName, pinNameSCL, pinNameSDA, handle);
}
SuoHandle SuoGetSerialHandle(const char* serialName)
{
	return (*pSuoGetSerialHandle)(serialName);
}
int SuoSetSerialParameterUART(SuoHandle handle, const SuoSerialParameterUART* param)
{
	return (*pSuoSetSerialParameterUART)(handle, param);
}
// for iic [7/24/2015 wang-lei]
int SuoSetSerialParameterIIC(SuoHandle handle, const SuoSerialParameterIIC* param)
{
	return (*pSuoSetSerialParameterIIC)(handle, param);
}
int SuoSetSerialParameterCSI(SuoHandle handle, const SuoSerialParameterCSI* param)
{
	return (*pSuoSetSerialParameterCSI)(handle, param);
}
int SuoGetSerialParameterUART(SuoHandle handle, SuoSerialParameterUART* param)
{
	return (*pSuoGetSerialParameterUART)(handle, param);
}
// for iic [7/24/2015 wang-lei]
int SuoGetSerialParameterIIC(SuoHandle handle, SuoSerialParameterIIC* param)
{
	return (*pSuoGetSerialParameterIIC)(handle, param);
}
int SuoGetSerialParameterCSI(SuoHandle handle, SuoSerialParameterCSI* param)
{
	return (*pSuoGetSerialParameterCSI)(handle, param);
}
int SuoSendSerialData(SuoHandle handle, unsigned long data)
{
	return (*pSuoSendSerialData)(handle, data);
}
int SuoSendSerialDataList(SuoHandle handle, long count, unsigned long dataList[])
{
	return (*pSuoSendSerialDataList)(handle, count, dataList);
}
int SuoSendSerialFile(SuoHandle handle, const char* serialFile)
{
	return (*pSuoSendSerialFile)(handle, serialFile);
}
int SuoSetNotifySentSerialCallback(SuoHandle handle, SuoNotifySentSerialCallback func)
{
	return (*pSuoSetNotifySentSerialCallback)(handle, func);
}
int SuoSetReceiveSerialCallback(SuoHandle handle, SuoReceiveSerialCallback func)
{
	return (*pSuoSetReceiveSerialCallback)(handle, func);
}
// for iic [7/24/2015 wang-lei]
int SuoStartIIC(SuoHandle handle)
{
	return (*pSuoStartIIC)(handle);
}
int SuoStopIIC(SuoHandle handle)
{
	return (*pSuoStopIIC)(handle);
}
int SuoAddressingIIC(SuoHandle handle)
{
	return (*pSuoAddressingIIC)(handle);
}
int SuoReceiveIICData(SuoHandle handle)
{
	return (*pSuoReceiveIICData)(handle);
}
// for iic callback [9/6/2015 zhang-dan]
int SuoSetEnableSerialCallback(SuoHandle handle, SuoEnableSerialCallback func){
  return (*pSuoSetEnableSerialCallback)(handle, func);
}
int SuoSetStopSerialCallback(SuoHandle handle, SuoStopSerialCallback func){
  return (*pSuoSetStopSerialCallback)(handle, func);
}
int SuoSetAddressSerialCallback(SuoHandle handle, SuoAddressSerialCallback func){
  return (*pSuoSetAddressSerialCallback)(handle, func);
}
// Wave interface
int SuoCreateWave(const char* waveName, int count, const char* pinNameList[], SuoHandle* handle)
{
	return (*pSuoCreateWave)(waveName, count, pinNameList, handle);
}
SuoHandle SuoGetWaveHandle(const char* waveName)
{
	return (*pSuoGetWaveHandle)(waveName);
}
int SuoSendWaveFile(SuoHandle handle, const char* waveFile)
{
	return (*pSuoSendWaveFile)(handle, waveFile);
}
int SuoSetNotifySentWaveCallback(SuoHandle handle, SuoNotifySentWaveCallback func)
{
	return (*pSuoSetNotifySentWaveCallback)(handle, func);
}
int SuoSetUserOpenGuiIF(SuoHandle handle)
{
	return (*pSuoSetUserOpenGuiIF)(handle);
}
void SuoSetUseropenIFPtr(struct UserOpenIF* uoif)
{
	(*pSuoSetUseropenIFPtr)(uoif);
}
struct UserOpenIF* SuoGetUseropenIFPtr()
{
	return (*pSuoGetUseropenIFPtr)();
}

// for send string [10/27/2008 caojiancang]
int SuoSendString(unsigned int id, const char *message){
  return (*pSuoSendString)(id, message);
}

int SuoSetSendStringCallback(unsigned int id, SuoSendStringCallback func){
  return (*pSuoSetSendStringCallback)(id , func);
}

//  [11/10/2008 caojiancang]
int SuoStartElapsedTimeCounter(unsigned char id){
  return (*pSuoStartElapsedTimeCounter)(id);
}

int SuoCurrentElapsedTimeCounter(unsigned char id, unsigned long *count){
  return (*pSuoCurrentElapsedTimeCounter)(id , count);
}

int SuoDeleteElapsedTimeCounter(unsigned char id){
  return (*pSuoDeleteElapsedTimeCounter)(id);
}

// Stop Cpu [4/20/2009 chengpenghui]
int SuoStop(){
  return (*pSuoStop)();
}

int SuoSetCallbackMode(unsigned short Mode){
  return (*pSuoSetCallbackMode)(Mode);
}

int SuoGetDigitalPin(SuoHandle handle, unsigned long *pinValue){
  return (*pSuoGetDigitalPin)(handle , pinValue);
}

int SuoGetAnalogPin(SuoHandle handle, double *pinValue){
  return (*pSuoGetAnalogPin)(handle , pinValue);
}

int SuoSetRequestEnforcedStartCallback(SuoRequestEnforcedStartCallback func){
  return (*pSuoSetRequestEnforcedStartCallback)(func);
}

} // extern "C"

