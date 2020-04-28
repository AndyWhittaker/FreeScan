#if !defined(AFX_GM1994CamaroZ28Protocol_H__79716CC6_4280_11D3_983E_00E018900F2A__INCLUDED_)
#define AFX_GM1994CamaroZ28Protocol_H__79716CC6_4280_11D3_983E_00E018900F2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseDefines.h"

#include "BaseProtocol.h"
#include "GM1994CamaroZ28Parser.h"
#include "StatusWriter.h"

class CGM1994CamaroZ28Protocol : public CBaseProtocol
{
// Construction
public:
	CGM1994CamaroZ28Protocol(CStatusWriter* pStatusWriter, CSupervisorInterface* pSupervisor, BOOL bInteract);
	virtual ~CGM1994CamaroZ28Protocol();
	
private:
	CGM1994CamaroZ28Parser	m_parser;
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

public:
	void Reset(void);
	void SetECUMode(const DWORD dwMode, const unsigned char data);
	DWORD GetCurrentMode(void);
	void ForceDataFromECU(void);
	BOOL OnCharsReceived(const unsigned char* const buffer, const DWORD bytesRead, CEcuData* const ecuData);

private:
	BOOL SendIdle(void); // Sends idle message to get bus
	BOOL SendModeShutUp(void); // Tell ECU to stop chattering packets
	BOOL SendMode1_0(void); // Tell ECU to send Mode 1 message 0 packets
	BOOL SendMode1_1(void); // Tell ECU to send Mode 1 message 1 packets
	BOOL SendMode1_2(void); // Tell ECU to send Mode 1 message 2 packets
	BOOL SendMode1_4(void); // Tell ECU to send Mode 1 message 4 packets
	BOOL SendMode1_6(void); // Tell ECU to send Mode 1 message 6 packets
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
	void OnMode1Msg1(void);
	void OnModeD1Msg1(void);
	void OnMode1Msg2(void);
	void OnModeD1Msg2(void);
	void OnMode1Msg4(void);
	void OnModeD1Msg4(void);
	void OnMode1Msg6(void);
	void OnModeD1Msg6(void);
	void OnMode3(void);
	void OnModeD3(void);
	void OnMode4(void);
	void OnModeD4(void);
	void OnMode10(void);

protected:
	void InitializeSupportedValues(CEcuData* const ecuData);
};

#endif // !defined(AFX_GM1994CamaroZ28Protocol_H__79716CC6_4280_11D3_983E_00E018900F2A__INCLUDED_)
