#if !defined(AFX_LotusCarltonProtocol_H__79716CC6_4280_11D3_983E_00E018900F2A__INCLUDED_)
#define AFX_LotusCarltonProtocol_H__79716CC6_4280_11D3_983E_00E018900F2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseDefines.h"

#include "BaseProtocol.h"
#include "LotusCarltonParser.h"
#include "StatusWriter.h"

class CLotusCarltonProtocol : public CBaseProtocol
{
// Construction
public:
	CLotusCarltonProtocol(CStatusWriter* pStatusWriter, CSupervisorInterface* pSupervisor, BOOL bInteract);
	virtual ~CLotusCarltonProtocol();
	
private:
	CLotusCarltonParser	m_parser;
	DWORD			m_dwCurrentMode; // Current ECU mode
	DWORD			m_dwRequestedMode; // Mode we want next
	unsigned char	m_ucData; // data for ECU, e.g. Desired Idle
	BOOL			m_bModeDone; // Have we sent our mode request?
	int				m_iShutUpCount;
	BOOL			m_bFirstRead; // First time reading ECU. Synchonises F0 byte
	BOOL			m_bReadHeader;
	int				m_iLen; // length of data packet
	int				m_iDataCount; // Current data index
	BOOL			m_bReadLength;
	BOOL			m_bReadData;
	BOOL			m_bReadCRC;
	BOOL			m_bSentOnce; // test for send
	BOOL			m_bTimeOut; // Timeout
	unsigned char	m_ucBuffer[400]; // Packet Temporary Buffer

// Implementation
public:
	void Reset(void);
	void SetECUMode(const DWORD dwMode, const unsigned char data);
	DWORD GetCurrentMode(void);
	void ForceDataFromECU(void);
	BOOL OnCharsReceived(const unsigned char* const buffer, const DWORD bytesRead, CEcuData* const ecuData);
	DWORD GetTimeoutForPingDuringInteract(void);

protected:
	BOOL SendIdle(void); // Sends idle message to get bus
	BOOL SendModeShutUp(void); // Tell ECU to shut-up
	BOOL SendMode0(void); // Tell ECU to send Mode 0 packets
	BOOL SendMode1(void); // Tell ECU to send Mode 1 packets
	BOOL ReceiveDTCs(void); // Ask for the trouble codes
	BOOL ClearDTCs(void); // Clear the trouble codes
	BOOL ClearBLM(void); // Clear the BLM
	BOOL ResetIAC(void); // Resets the IAC (Idle Control Valve)
	BOOL SetDesiredIdle(unsigned char DesIdle); // sets the desired idle
	void SendNextCommand(void); // Sends the required command to ECU
	int HandleTX(unsigned char* buffer, int iLength);

	void OnIdle(void);
	void OnModeShutUp(void);
	void OnModeDShutUp(void);
	void OnMode0(void);
	void OnModeD0(void);
	void OnMode1(void);
	void OnModeD1(void);
	void OnMode3(void);
	void OnModeD3(void);
	void OnMode4(void);
	void OnModeD4(void);

protected:
	void InitializeSupportedValues(CEcuData* const ecuData);
	void OnTimer(void);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LotusCarltonProtocol_H__79716CC6_4280_11D3_983E_00E018900F2A__INCLUDED_)
