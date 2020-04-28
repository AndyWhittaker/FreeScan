
#include "BaseProtocol.h"

CBaseProtocol::CBaseProtocol(CStatusWriter* const pStatusWriter, CSupervisorInterface* const pSupervisor, BOOL bInteract): m_pStatusWriter(pStatusWriter), m_pSupervisor(pSupervisor), m_bInteract(bInteract) {
	m_pcom = NULL;
}

CBaseProtocol::~CBaseProtocol() {
}

// Writes a line of ASCII to the spy window
void CBaseProtocol::WriteStatus(const CString csText) {
	CString statusText = "P: " + csText;
	m_pStatusWriter->WriteStatus(statusText);
}

// Writes a line of binary as ASCII to the spy window
void CBaseProtocol::WriteASCII(const unsigned char* const buffer, const int ilength) {
	m_pStatusWriter->WriteASCII(buffer, ilength);
}

void CBaseProtocol::WriteLogEntry(LPCTSTR pstrFormat, ...) {
	va_list args;
	va_start(args, pstrFormat);
	m_pStatusWriter->WriteLogEntry(pstrFormat, args);
}

void CBaseProtocol::WriteStatusLogged(CString csText) {
	m_pStatusWriter->WriteStatusTimeLogged(csText);
}

BOOL CBaseProtocol::Init(CSerialPort* pcom) {

	m_pcom = pcom; // assign our serial port pointer.

	// This sets up the com port CSerialPort Object
	// Note: Look in SerialPort.h for the defaults:
	// Most protocols need 8192baud, 1 start, 1 stop and no parity.
	// We pass the CSerialPort a m_pSupervisor pointer because it
	// needs to send messages to this window via the CWnd Object
	if (!m_pcom->InitPort(m_pSupervisor, NULL)) {
		WriteStatus("Failed to initialise the Com Port");
		return FALSE;
	}
	else {
		WriteStatus("Com Port initialised");
		return TRUE;
	}
}

// Write a string to the port - This can even write NULL characters
void CBaseProtocol::WriteToECU(unsigned char* string, int stringlength, BOOL bDelay) {
	m_pcom->WriteToPort(string, stringlength, bDelay);
	if (stringlength > 0) {
		m_pSupervisor->IncreaseSentBytes(stringlength);
	}
}

DWORD CBaseProtocol::GetTimeoutForPingDuringInteract(void) {
	return 0; // disable timer by default
}

void CBaseProtocol::OnTimer(void) {
	WriteStatus("Unused timer event -> Software Bug...");
}

void CBaseProtocol::SetInteract(const BOOL bInteract) {
	if (bInteract) {
		WriteStatus("Interaction with the ECU enabled.");
		m_bInteract = TRUE;
	}
	else {
		WriteStatus("In monitor mode, no interaction with ECU will be done.");
		m_bInteract = FALSE;
	}
}
