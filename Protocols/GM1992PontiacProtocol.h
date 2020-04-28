#if !defined(AFX_GM1992PontiacProtocol_H__79716CC6_4280_11D3_983E_00E018900F2A__INCLUDED_)
#define AFX_GM1992PontiacProtocol_H__79716CC6_4280_11D3_983E_00E018900F2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseDefines.h"

#include "BaseProtocol.h"
#include "GM1992PontiacParser.h"
#include "StatusWriter.h"

class CGM1992PontiacProtocol : public CBaseProtocol
{
// Construction
public:
	CGM1992PontiacProtocol(CStatusWriter* pStatusWriter, CSupervisorInterface* pSupervisor, BOOL bInteract);
	virtual ~CGM1992PontiacProtocol();

private:
	CGM1992PontiacParser	m_parser;
	DWORD			m_dwCurrentMode; // Current ECU mode
	DWORD			m_dwRequestedMode; // Mode we want next
	unsigned char	m_ucData; // data for ECU, e.g. Desired Idle
	BOOL			m_bModeDone; // Have we sent our mode request?
	int				m_iIdleCount;
	int				m_iShutUpCount;
	BOOL			m_bFirstRead; // First time reading ECU. Synchonises F0 byte
	BOOL			m_bReadHeader;
	int				m_iLen; // length of data packet
	int				m_iDataCount; // Current data index
	BOOL			m_bReadLength;
	BOOL			m_bReadData;
	BOOL			m_bReadCRC;
	BOOL			m_bSentOnce; // test for send
	unsigned char	m_ucBuffer[400]; // Packet Temporary Buffer

// Implementation
public:
	void Reset(void);
	void SetECUMode(const DWORD dwMode, const unsigned char data);
	DWORD GetCurrentMode(void);
	void ForceDataFromECU(void);
	BOOL OnCharsReceived(const unsigned char* const buffer, const DWORD bytesRead, CEcuData* const ecuData);

private:
	BOOL SendIdle(void); // Sends idle message to get bus
	BOOL SendMode8_ShutUp(void); // Tell ECU to stop chattering packets
	BOOL SendMode1_0(void); // Tell ECU to send Mode 1 message 0 packets
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
	void OnMode1Msg0(void);
	void OnModeD1Msg0(void);
	void OnMode3(void);
	void OnModeD3(void);
	void OnMode4(void);
	void OnModeD4(void);
	void OnMode10(void);

protected:
	void InitializeSupportedValues(CEcuData* const ecuData);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GM1992PontiacProtocol_H__79716CC6_4280_11D3_983E_00E018900F2A__INCLUDED_)
