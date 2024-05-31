//
//      Copyright(C) 2001, 2015 Renesas Electronics Corporation
//      Renesas CONFIDENTIAL AND PROPRIETARY   
//      All rights reserved by Renesas Electronics Corporation.
//      Use of copyright notice does not evidence publication.
//

/************************************************************************/
/*	User-Open-Interface System Header				*/
/************************************************************************/


#if !defined(__SUO_H__)
#define __SUO_H__


#include <windows.h>
#define SIMPILS_LV1	// **** SIMPILS function ****
#define SIMPILS_LV2  

/* extended-attribute */
#ifdef UO_SYS
#define SuoUserEntry
#define	SuoSystemEntry	__declspec(dllexport)
#else
#define SuoUserEntry	__declspec(dllexport)
#define	SuoSystemEntry
#endif


/*
 *	MakeUserModel
 */

/* function */
SuoUserEntry void MakeUserModel(const char *option);


/*
 *	Common
 */

/* type */
typedef void* SuoHandle;

/* error */
#define	SUO_NOERROR		0x0000	/* No error */
#define	SUO_CANTALLOC		0x0100	/* Cannot allocate memory */
#define	SUO_ILLIFNAME		0x0101	/* Illegal interface name */
#define	SUO_ILLHANDLE		0x0102	/* Illegal handle */
#define	SUO_ILLPARAM		0x0103	/* Illegal parameter */
#define	SUO_CANTCALL		0x01f0	/* Cannot call function */
#define	SUO_CONFLICTRES		0x01f1	/* Conflict resource */
#define	SUO_ILLFILENAME		0x0200	/* Illegal file name */
#define	SUO_CANTOPENFILE	0x0201	/* Cannot open file */
#define	SUO_ILLFILEFMT		0x0202	/* Illegal file format */
#define	SUO_ILLFILECONT		0x0203	/* Illegal file contents */
#define	SUO_ILLPINNAME		0x0210	/* Illegal pin name */
#define	SUO_ILLADDRRANGE	0x0211	/* Illegal address range */
#define	SUO_UNDERSENDING	0x0221	/* Under sending */
#define	SUO_UNDERRECEIVING	0x0222	/* Under receiving */

/*
 *	Basic interface
 */

/* callback */
typedef void (*SuoInitCallback)(void);
typedef void (*SuoResetCallback)(void);
#if defined(SIMPILS_LV2)
typedef void (*SuoRequestEnforcedBreakCallback)(int flag);
#endif
#ifdef V220
typedef void (*SuoRequestCanbusCallback)(unsigned char ch, unsigned short state, const CANFRAME *frame);
#endif
/* function */
SuoSystemEntry void SuoSetInitCallback(SuoInitCallback func);
SuoSystemEntry void SuoSetResetCallback(SuoResetCallback func);
#if defined(SIMPILS_LV2)
//SuoSystemEntry void SuoSetRequestEnforcedBreakCallback(SuoRequestEnforcedBreakCallback func); //型は多分intで良いと思う
SuoSystemEntry int SuoSetRequestEnforcedBreakCallback(SuoRequestEnforcedBreakCallback func);
SuoSystemEntry DWORD SuoGetParentPathName(LPTSTR lpFilename,DWORD nSize);
#endif
SuoSystemEntry int SuoGetMainClock(unsigned long* time);
#if defined(SIMPILS_LV3)
SuoSystemEntry int SuoReadMemory(unsigned long addr, unsigned short size, unsigned char data[]);
#endif
#if defined(SIMPILS_LV3)
SuoSystemEntry int SuoWriteMemory(unsigned long addr, unsigned short size, const unsigned char data[]);
#endif


/*
 *	Timer interface
 */

/* define */
typedef enum SuoTimeUnit {
	SUO_MAINCLK = 0x00,
#if defined(SIMPILS_LV1)
	SUO_USEC    = 0x12,
	SUO_NSEC    = 0x13
#else
	SUO_USEC    = 0x12
#endif
}	SuoTimeUnit;

/* callback */
typedef void (*SuoNotifyTimerCallback)(SuoHandle handle);

/* function */
SuoSystemEntry int SuoCreateTimer(const char* timerName, SuoHandle* handle);
SuoSystemEntry SuoHandle SuoGetTimerHandle(const char* timerName);
SuoSystemEntry int SuoSetTimer(SuoHandle handle, int timeUnit, unsigned long timeValue);
SuoSystemEntry int SuoKillTimer(SuoHandle handle);
SuoSystemEntry int SuoSetNotifyTimerCallback(SuoHandle handle, SuoNotifyTimerCallback func);


/*
 *	Pin interface
 */

/* define */
enum {
	SUO_LOW		= 0x0,
	SUO_HIGH	= 0x1
};

/* callback */
typedef void (*SuoInputDigitalPinCallback)(SuoHandle handle, int pinValue);
typedef void (*SuoInputAnalogPinCallback)(SuoHandle handle, double pinValue);
typedef void (*SuoInputHighImpedanceCallback)(SuoHandle handle);

/* function */
#if defined(SIMPILS_LV1)
//SuoSystemEntry void SuoEnableDevicePin(void); //多分型はintで良いと思う・・・
SuoSystemEntry int SuoEnableDevicePin(void);
#endif
SuoSystemEntry int SuoCreatePin(const char* pinName, SuoHandle* handle);
#if defined(SIMPILS_LV1)
SuoSystemEntry int SuoOpenPin(const char* pinName, SuoHandle* handle);
#endif
#if defined(SIMPILS_LV1)
SuoSystemEntry int SuoClosePin(SuoHandle handle);
#endif
SuoSystemEntry SuoHandle SuoGetPinHandle(const char* pinName);
SuoSystemEntry int SuoOutputDigitalPin(SuoHandle handle, int pinValue);
SuoSystemEntry int SuoOutputAnalogPin(SuoHandle handle, double pinValue);
SuoSystemEntry int SuoSetInputDigitalPinCallback(SuoHandle handle, SuoInputDigitalPinCallback func);
SuoSystemEntry int SuoSetInputAnalogPinCallback(SuoHandle handle, SuoInputAnalogPinCallback func);
SuoSystemEntry int SuoOutputHighImpedance(SuoHandle handle);
SuoSystemEntry int SuoSetInputHighImpedanceCallback(SuoHandle handle, SuoInputHighImpedanceCallback func);


/*
 *	Extbus interface
 */

/* callback */
typedef void (*SuoReadExtbusCallback)(SuoHandle handle, unsigned long addr, int accessSize, unsigned char data[]);
typedef void (*SuoWriteExtbusCallback)(SuoHandle handle, unsigned long addr, int accessSize, const unsigned char data[]);

/* function */
SuoSystemEntry int SuoCreateExtbus(const char* extbusName, unsigned long addr, unsigned long size, SuoHandle* handle);
SuoSystemEntry SuoHandle SuoGetExtbusHandle(const char* extbusName);
SuoSystemEntry int SuoSetReadExtbusCallback(SuoHandle handle, SuoReadExtbusCallback func);
SuoSystemEntry int SuoSetWriteExtbusCallback(SuoHandle handle, SuoWriteExtbusCallback func);

/*
 *	Serial interface
 */

/* define */
enum {
	SUO_MASTER	= 0x0,
	SUO_SLAVE	= 0x1
};
enum {
	SUO_PRECEDEDATA	= 0x0001,
	SUO_REVERSECLOCK= 0x0002
};
enum {
	SUO_LSBFIRST	= 0x0,
	SUO_MSBFIRST	= 0x1
};
enum {
	SUO_NONEPARITY	= 0x0,
	SUO_ODDPARITY	= 0x1,
	SUO_EVENPARITY	= 0x2,
	SUO_ZEROPARITY	= 0x3
};
enum {
	SUO_PARITYERR	= 0x10,
	SUO_FRAMINGERR	= 0x11
};
// add for uart reverse function [10/26/2009 xuxiuchun]
enum {
	SUO_NOT_REVERSE = 0x0, //transmit and receive are not reverse
	SUO_R_REVERSE   = 0x1, //receive is reverse, but transmit is not reverse
	SUO_T_REVERSE   = 0x2, //transmit is reverse, but receive is not reverse
  SUO_BOTH_REVERSE = 0x3 //transmit and receive all are reverse
}; //reverse


enum {
	SUO_TRANSMIT	= 0x0,
	SUO_RECEIVE	= 0x1
};

enum {
	SUO_WAIT8	= 0x0,
	SUO_WAIT9	= 0x1
};

enum {
	SUO_ENABLE_OK	= 0x0,
	SUO_ENABLE_FAILED	= 0x1
};

enum {
	SUO_ADDRESS_OK	= 0x0,
	SUO_ADDRESS_NO_MATCH	= 0x1
};

/* struct */
typedef struct {
	unsigned long	baudrate;
	int		direction;
	int		dataLength;
	int		stopLength;
	int		parity;
}	SuoSerialParameterUART;
// add for uart reverse function [10/26/2009 xuxiuchun]
typedef struct {
  int   reverse;   //reverse
  int   param2;    //for expanding
  int   param3;    //for expanding
  int   param4;    //for expanding
}	SuoSerialParameterUART2;
typedef struct {
	int		mode;
	unsigned long	frequency;
	int		phase;
	int		direction;
	int		dataLength;
}	SuoSerialParameterCSI;

typedef struct {
	unsigned long		master_slave;
	unsigned long	  iicwl;
	unsigned long		iicwh;
	unsigned long		slave_address;
	unsigned long		direction;
	unsigned long		transfer_clk;
	unsigned long		wait_mode;
	unsigned long		match_address;
}	SuoSerialParameterIIC;

/* callback */
typedef void (*SuoNotifySentSerialCallback)(SuoHandle handle);
typedef void (*SuoReceiveSerialCallback)(SuoHandle handle, unsigned long data, int status);
// for iic [7/31/2015 wang-lei]
typedef void (*SuoEnableSerialCallback)(SuoHandle handle,int status);
typedef void (*SuoStopSerialCallback)(SuoHandle handle);
typedef void (*SuoAddressSerialCallback)(SuoHandle handle, unsigned long address, int direction, int status);

/* function */
SuoSystemEntry int SuoCreateSerialUART(const char* serialName, const char* pinNameTXD, const char* pinNameRXD, SuoHandle* handle);
SuoSystemEntry int SuoCreateSerialCSI(const char* serialName, const char* pinNameSO, const char* pinNameSI, const char* pinNameSCK, SuoHandle* handle);
// for IIC [7/20/2015 wang-lei]
SuoSystemEntry int SuoCreateSerialIIC(const char* serialName, const char* pinNameSCL, const char* pinNameSDA, SuoHandle* handle);

SuoSystemEntry SuoHandle SuoGetSerialHandle(const char* serialName);
SuoSystemEntry int SuoSetSerialParameterUART(SuoHandle handle, const SuoSerialParameterUART* param);
// add for uart reverse function [10/26/2009 xuxiuchun]
SuoSystemEntry int SuoSetSerialParameterUART2(SuoHandle handle, const SuoSerialParameterUART2* param);
SuoSystemEntry int SuoSetSerialParameterCSI(SuoHandle handle, const SuoSerialParameterCSI* param);
SuoSystemEntry int SuoGetSerialParameterUART(SuoHandle handle, SuoSerialParameterUART* param);
// for IIC [7/20/2015 wang-lei]
SuoSystemEntry int SuoSetSerialParameterIIC(SuoHandle handle, const SuoSerialParameterIIC* param);
SuoSystemEntry int SuoGetSerialParameterIIC(SuoHandle handle, SuoSerialParameterIIC* param);
SuoSystemEntry int SuoStartIIC(SuoHandle handle);
SuoSystemEntry int SuoStopIIC(SuoHandle handle);
SuoSystemEntry int SuoAddressingIIC(SuoHandle handle);
SuoSystemEntry int SuoReceiveIICData(SuoHandle handle);

// add for uart reverse function [10/26/2009 xuxiuchun]
SuoSystemEntry int SuoGetSerialParameterUART2(SuoHandle handle, SuoSerialParameterUART2* param);
SuoSystemEntry int SuoGetSerialParameterCSI(SuoHandle handle, SuoSerialParameterCSI* param);
SuoSystemEntry int SuoSendSerialData(SuoHandle handle, unsigned long data);
SuoSystemEntry int SuoSendSerialDataList(SuoHandle handle, long count, unsigned long dataList[]);
SuoSystemEntry int SuoSendSerialFile(SuoHandle handle, const char* serialFile);
SuoSystemEntry int SuoSetNotifySentSerialCallback(SuoHandle handle, SuoNotifySentSerialCallback func);
SuoSystemEntry int SuoSetReceiveSerialCallback(SuoHandle handle, SuoReceiveSerialCallback func);

// for iic callback [9/6/2015 zhang-dan]
SuoSystemEntry int SuoSetEnableSerialCallback(SuoHandle handle, SuoEnableSerialCallback func);
SuoSystemEntry int SuoSetStopSerialCallback(SuoHandle handle, SuoStopSerialCallback func);
SuoSystemEntry int SuoSetAddressSerialCallback(SuoHandle handle, SuoAddressSerialCallback func);

/*
 *	Wave interface
 */

/* callback */
typedef void (*SuoNotifySentWaveCallback)(SuoHandle handle);

/* function */
SuoSystemEntry int SuoCreateWave(const char* waveName, int count, const char* pinNameList[], SuoHandle* handle);
SuoSystemEntry SuoHandle SuoGetWaveHandle(const char* waveName);
SuoSystemEntry int SuoSendWaveFile(SuoHandle handle, const char* waveFile);
SuoSystemEntry int SuoSetNotifySentWaveCallback(SuoHandle handle, SuoNotifySentWaveCallback func);

/*
 *	GUI interface
 */

/* function */
SuoSystemEntry int SuoSetUserOpenGuiIF(SuoHandle handle);


/*
 *	Canbus interface
 */
typedef struct {
	unsigned long	id;
	unsigned char	len;
	unsigned char	rtr;
	unsigned char	r0;
	unsigned char	r1;
	unsigned char	data[8];
}CANFRAME;

/* callback */
typedef void (*SuoRequestCanbusCallback)(unsigned char ch, unsigned short state, const CANFRAME *frame);

SuoSystemEntry int SuoSetRequestCanbusCallback(SuoRequestCanbusCallback func);
SuoSystemEntry int SuoSetCanbus(unsigned char ch, unsigned short state, const CANFRAME *frame);
SuoSystemEntry void SuoOperateCan(int op, unsigned char ch, unsigned long *data); 

/* CANBUS state */
#define SUO_SENDCANDATA		0x0001		/* Send data frame (request/state) */
#define SUO_SENDCANERR		0x0002		/* Send error frame (request/state) */
#define SUO_RECEIVECANDATA	0x0010		/* Receive data frame (statue) */
#define SUO_RECEIVECANERR	0x0020		/* Receive error frame (state) */
#define SUO_ENDCANDATA		0x0040		/* End of previous data frame (state) */
#define SUO_ENDCANERR		0x0080		/* End of previous error frame (state) */
#define SUO_ADDEND			0x0100		/* End of this frame (extended-state) */
#define SUO_NOTICE_ARBITLOST	0x0200		/* Arbitration lost occured */
#define SUO_BUSY			0x1000		/* Bus-Busy (internal) */

/* Operation of FCAN */
#define SUO_OFC_GETCHCNT	0x0000		/* Get channel count */
#define SUO_OFC_GETTIMEPARAM	0x0001		/* Get timinng parameter */
#define SUO_OFC_SETRXERRRATE	0x0002		/* Set receive error rate */
#define SUO_OFC_SETTXERRRATE	0x0003		/* Set transmission error rate */

enum {
	FRAME_NORMAL	= 0x00,
	FRAME_EXTEND	= 0x01
};


/*
 *	LCD interface
 */
typedef struct {
	unsigned char common;		/* コモン信号数 */
	unsigned char segment;		/* セグメント信号数 */
	unsigned char buffer_size;	/* LCDバッファのバイト数 */
}SuoLCDParameter;

typedef struct {
	unsigned char common;		/* 変化したコモン信号数 */
	unsigned char segment;		/* 変化したセグメント信号数 */
	unsigned char value;		/* 変化点の値 */
}SuoLcdChgValue;

/* LCD status */
#define SUO_LCD_ENABLED		0x0001		/* just after LCD Enabled */
#define SUO_LCD_DISABLED	0x0002		/* just after LCD Disabled */
#define SUO_LCD_CHANGED		0x0003		/* LCD Memory changed */

/* callback */
typedef void (*SuoLCDParameterCallback)(SuoHandle handle, SuoLCDParameter param);
typedef void (*SuoInputLCDChgCallback)(SuoHandle handle, int status, SuoLcdChgValue *LCDValue, unsigned int num);

/* callback */
SuoSystemEntry int SuoSetLCDParameterCallback(SuoHandle handle, SuoLCDParameterCallback func);
SuoSystemEntry int SuoGetLCDChannel(char *channel);
SuoSystemEntry int SuoGetLCDParameter(SuoLCDParameter* param);//added by shizy [2007/11/23] for gui_lcd
SuoSystemEntry int SuoCreateLCD(const char* name, char ChannelNo, SuoHandle *handle);
SuoSystemEntry int SuoSetInputLCDCallback(SuoHandle handle, SuoInputLCDChgCallback func);

// send string callback [10/28/2008 caojiancang]
typedef int (*SuoSendStringCallback)(const char* message);
SuoSystemEntry int SuoSetSendStringCallback(unsigned int id, SuoSendStringCallback func);
SuoSystemEntry int SuoSendString(unsigned int id, const char *message);

//  [11/10/2008 caojiancang]
SuoSystemEntry int SuoStartElapsedTimeCounter(unsigned char id);
SuoSystemEntry int SuoCurrentElapsedTimeCounter(unsigned char id, unsigned long *count);
SuoSystemEntry int SuoDeleteElapsedTimeCounter(unsigned char id);

// for TOTO new I/F: SuoStop [4/20/2009 chengpenghui]
SuoSystemEntry int SuoStop();

// for TOTO I/F [7/1/2010 chengpenghui]
typedef void (*SuoRequestEnforcedStartCallback)();
enum{
  PIN_CALLBACK_DEFAULT,
  PIN_CALLBACK_CHANGED,
};

SuoSystemEntry int SuoSetCallbackMode(unsigned short Mode);
SuoSystemEntry int SuoGetDigitalPin(SuoHandle handle, unsigned long *pinValue);
SuoSystemEntry int SuoGetAnalogPin(SuoHandle handle, double *pinValue);
SuoSystemEntry int SuoSetRequestEnforcedStartCallback(SuoRequestEnforcedStartCallback func);

#endif // !defined(__SUO_H__)
