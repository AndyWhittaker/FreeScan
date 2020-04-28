#pragma once

#include "BaseDefines.h"

#include "StatusWriter.h"
#include "SupervisorInterface.h"
#include "SerialPort.h"

class CBaseProtocol : public CObject
{
private:
	CStatusWriter* const		m_pStatusWriter;
	CSupervisorInterface* const	m_pSupervisor;

protected:
	BOOL						m_bInteract; // allow sending mode requests to ECU
	CSerialPort*				m_pcom; // Our com port object pointer of the Supervisor

protected:
	CBaseProtocol(CStatusWriter* const pStatusWriter, CSupervisorInterface* const pSupervisor, BOOL bInteract);

	void WriteLogEntry(const LPCTSTR pstrFormat, ...);
	void WriteStatusLogged(const CString csText);

public:
	virtual ~CBaseProtocol();

	virtual void InitializeSupportedValues(CEcuData* const ecuData) = 0;
	virtual void Reset() = 0;
	virtual void SetECUMode(const DWORD dwMode, const unsigned char data) = 0;
	virtual DWORD GetCurrentMode(void) = 0;
	virtual void ForceDataFromECU(void) = 0;
	virtual BOOL OnCharsReceived(const unsigned char* const buffer, const DWORD bytesRead, CEcuData* const ecuData) = 0;

	virtual BOOL Init(CSerialPort* pcom);
	virtual void WriteToECU(unsigned char* string, int stringlength, BOOL bDelay = TRUE);
	virtual DWORD GetTimeoutForPingDuringInteract(void);
	virtual void OnTimer(void);

	void SetInteract(const BOOL bInteract);

	void WriteStatus(const CString csText);
	void WriteASCII(const unsigned char* const buffer, const int ilength);
};