#pragma once

#include "BaseDefines.h"

#include <afxwin.h>

#include "EcuData.h"

// These are defines for the ECU message requests.
// For function ECUMode(...)
#define	ECU_GET_DTCS				100
#define ECU_CLEAR_DTCS				101
#define ECU_CLEAR_BLM				102
#define ECU_RESET_IAC				103
#define ECU_SET_DES_IDLE			104

class CSupervisorInterface: public CWnd
{
public:
	virtual void Init(int iModel) = 0;

	virtual BOOL GetInteract(void) = 0;
	virtual void Interact(BOOL bInteract) = 0;
	virtual UINT GetCurrentPort(void) = 0;
	virtual void SetCurrentPort(UINT nPort) = 0;
	virtual DWORD GetWriteDelay(void) = 0;
	virtual void SetWriteDelay(DWORD nDelay) = 0;
	virtual BOOL GetCentigrade(void) = 0;
	virtual void Centigrade(BOOL bUnit) = 0;
	virtual BOOL GetMiles(void) = 0;
	virtual void Miles(BOOL bUnit) = 0;
	virtual DWORD GetCurrentMode(void) = 0;
	virtual void ECUMode(DWORD dwMode, const unsigned char Data = 0) = 0;
	virtual DWORD GetReceivedBytes() = 0;
	virtual DWORD GetSentBytes() = 0;
	virtual void IncreaseSentBytes(const DWORD additionalBytesSent) = 0;

	virtual BOOL Start(void) = 0;// Starts the ECU monitoring
	virtual BOOL Stop(void) = 0;
	virtual BOOL ShutDown(void) = 0;

	virtual BOOL StartCSVLog(BOOL bStart) = 0;

	virtual void ForceDataFromECU(void) = 0;

	virtual const CEcuData* const GetEcuData(void) = 0;
};