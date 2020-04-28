// LotusCarltonProtocol.cpp : implementation file
//
//////////////////////////////////////////////////////////////////////
// (c) 1996-2000 Andy Whittaker, Chester, England. 
// mail@andywhittaker.com
//

#include "LotusCarltonProtocol.h"

#include "GMBaseFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLotusCarltonProtocol

CLotusCarltonProtocol::CLotusCarltonProtocol(CStatusWriter* pStatusWriter, CSupervisorInterface* pSupervisor, BOOL bInteract) : CBaseProtocol(pStatusWriter, pSupervisor, bInteract), m_parser(this) {
	m_bTimeOut = FALSE;

	Reset();
}

CLotusCarltonProtocol::~CLotusCarltonProtocol() {
}

void CLotusCarltonProtocol::InitializeSupportedValues(CEcuData* const ecuData) {
	// Put your comments and release notes about the protocol here.
	ecuData->m_csProtocolComment.Format("Name: Lotus Carlton 6 Cylinder\nVersion v1.1\nDate: 1st November 2000\nImplemented by Andy Whittaker\nProtocol by Malcolm Robb\n*** Alpha Version ***.\n\nUse Force Data to start communications");
	m_parser.InitializeSupportedValues(ecuData);
}

// Resets the protocol state machine
void CLotusCarltonProtocol::Reset() {
	m_dwCurrentMode = 0;
	m_dwRequestedMode = 1; // Mode we want next
	m_bModeDone = TRUE; // Have we sent our mode request?
	m_ucData = 0; // data for ECU, e.g. Desired Idle

	// Reset the receive data, state machine
	m_bReadHeader = TRUE;
	m_iShutUpCount = 1;
	m_iLen = 0; // reset length of data packet
	m_iDataCount = 0; // Current data index
	m_bFirstRead = TRUE;
	m_bReadLength = FALSE;
	m_bReadData = FALSE;
	m_bReadCRC = FALSE;

	m_bSentOnce = FALSE;
}

// This switches the mode number that is sent to the ECU. It changes the
// behaviour of SendNextCommand(..).
void CLotusCarltonProtocol::SetECUMode(const DWORD dwMode, const unsigned char data) {
	m_ucData = data; // data for ECU, e.g. Desired Idle
	m_dwRequestedMode = dwMode; // Mode we want next
	m_bModeDone = FALSE; // Have we sent our mode request?
}

// Returns the current ECU Mode
DWORD CLotusCarltonProtocol::GetCurrentMode(void) {
	return m_dwCurrentMode;
}

// Forces Shut-Up to be sent.
void CLotusCarltonProtocol::ForceDataFromECU() {
	Reset();
	SendMode0();
}

/////////////////////////////////////////////////////////////////////////////
// Commands to ECU

// Sends the Idle Mode data bytes to the ECU
BOOL CLotusCarltonProtocol::SendIdle(void)
{ //0xf4 0x55, 0xB7
	unsigned char	ucRequestIdle[] = { 0xf4, 0x55, 0xb7 }; // Idle
	CGMBaseFunctions::SetChecksum(ucRequestIdle, 3);
	WriteStatus("*** Sending Idle to ECU ***");
	WriteToECU(ucRequestIdle, 3, FALSE); //No delay before transmit
	return TRUE;
}

// Sends the shut-up command to the ECU
BOOL CLotusCarltonProtocol::SendModeShutUp(void)
{ //0xf4 0x56 0x08 0xae
	unsigned char	ucRequestMode0[] = { 0xf4, 0x56, 0x08, 0xae }; //
	WriteStatus("*** Sending Shut-Up to ECU ***");
	WriteToECU(ucRequestMode0, 4);
	return TRUE;
}

// Sends the Mode 0 data bytes to the ECU
BOOL CLotusCarltonProtocol::SendMode0(void)
{ //0xF4 0x56 0x00 0xB6
	unsigned char	ucRequestMode0[] = { 0xf4, 0x56, 0x00, 0xb6 }; //
	WriteStatus("*** Requesting Mode0 from ECU ***");
	WriteToECU(ucRequestMode0, 4);
	return TRUE;
}

// Sends the Mode 1 data bytes to the ECU
BOOL CLotusCarltonProtocol::SendMode1(void)
{ //0xf4 0x57 0x01 0x00 0xB9
	unsigned char	ucRequestMode1[] = { 0xf4, 0x57, 0x01, 0x00, 0xb4 }; //
	WriteStatus("*** Requesting Mode1 from ECU ***");
	WriteToECU(ucRequestMode1, 5);
	return TRUE;
}

// Sends the Mode 3 data bytes to the ECU (Get DTCs)
BOOL CLotusCarltonProtocol::ReceiveDTCs(void)
{ //0xF4 0x5C 0x03 0x00 0x05 0x00 0x06 0x00 0x07 0x9b 
	unsigned char	ucRequestMode3[] = { 0xf4, 0x5c, 0x03, 0x00, 0x05, 0x00, 0x06, 0x00, 0x07, 0x9b };
	// ECU should send 0xF0 0x59 0x03 0x00 0x00 0x00 0xB4;
	WriteStatus("*** Requesting DTCs from ECU ***");
	WriteToECU(ucRequestMode3, 10);
	return TRUE;
}

// Sends the Mode 4 data bytes to the ECU (Clears DTCs)
BOOL CLotusCarltonProtocol::ClearDTCs(void)
{ //0xF4 0x60 0x04 0x00 0x00 0x40 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x6C
	unsigned char	ucRequestMode4[] = { 0xf4, 0x60, 0x04, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68 };
	// ECU should confirm with 0xF0 0x56 0x04 0xB6
	WriteStatus("*** Clearing DTCs in ECU ***");
	WriteToECU(ucRequestMode4, 14);
	return TRUE;
}

// Sends the Mode 4 data bytes to the ECU (Clears BLM)
BOOL CLotusCarltonProtocol::ClearBLM(void)
{ //0xF4 0x60 0x04 0x00 0x00 0x10 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x9C
	unsigned char	ucRequestMode4[] = { 0xf4, 0x60, 0x04, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x98 };
	// ECU should confirm with 0xF0 0x56 0x04 0xB6
	WriteStatus("*** Clearing BLM in ECU ***");
	WriteToECU(ucRequestMode4, 14);
	return TRUE;
}

// Sends the Mode 4 data bytes to the ECU (Resets IAC)
BOOL CLotusCarltonProtocol::ResetIAC(void)
{ //0xF4 0x60 0x04 0x00 0x00 0x20 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x8C
	unsigned char	ucRequestMode4[] = { 0xf4, 0x60, 0x04, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88 };
	// ECU should confirm with 0xF0 0x56 0x04 0xB6
	WriteStatus("*** Resetting IAC in ECU ***");
	WriteToECU(ucRequestMode4, 14);
	return TRUE;
}

// Sends the Mode 4 data bytes to the ECU (Sets Desired Idle)
BOOL CLotusCarltonProtocol::SetDesiredIdle(unsigned char DesIdle)
{ //0xF4 0x60 0x04 0x01 0x01 0x00 0x00 0x10 0xFF 0x03 0x90 0x00 0x00 0x08 

	// We must set the desired idle ( desired / 12.5 ) and then
	// calculate the correct CRC.

	// Currently 1800 rpm (0x90).
	unsigned char	ucRequestDesIdle[] = { 0xf4, 0x60, 0x04, 0x01, 0x01, 0x00, 0x00, 0x10, 0xff, 0x03, 0x90, 0x00, 0x00, 0x04 };
	ucRequestDesIdle[10] = DesIdle;

	CGMBaseFunctions::SetChecksum(ucRequestDesIdle, 14);
	CString buf;
	buf.Format("*** Setting Desired Idle in ECU to %d RPM ***", (int)((DesIdle * 25) / 2));
	WriteStatus(buf);
	// ECU should confirm with 0xF4 0x56 0x04 0xB6
	WriteToECU(ucRequestDesIdle, 14);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// ECU Command handlers

// We must continuously send commands to the ECU. Here, the correct command
// is sequenced out from the PC into the ECU, as required.
void CLotusCarltonProtocol::SendNextCommand(void)
{
	m_bTimeOut = FALSE; // reset our timeout event
	
	if (!m_bModeDone)
	{
		switch (m_dwRequestedMode)
		{
		case 0:
			SendMode0();
			break;
		case 1:
			SendMode1();
			break;
		case 2:
			WriteStatus("*** SendNextCommand unsupported request (2) ***");
			break;
		case ECU_GET_DTCS:
			ReceiveDTCs();
			break;
		case ECU_CLEAR_DTCS:
			ClearDTCs();
			break;
		case ECU_CLEAR_BLM:
			ClearBLM();
			break;
		case ECU_RESET_IAC:
			ResetIAC();
			break;
		case ECU_SET_DES_IDLE:
			SetDesiredIdle(m_ucData);
			break;
		default:
			WriteStatus("*** SendNextCommand unsupported request (??) ***");
		}
		m_bModeDone = TRUE;
	}
	else // we normally send out Mode 1 requests
		SendMode1();
}

// Handle the message from the serial port class.
BOOL CLotusCarltonProtocol::OnCharsReceived(const unsigned char* const buffer, const DWORD bytesRead, CEcuData* const ecuData) {
	BOOL			updatedEcuData = FALSE;

	unsigned char	ucRX; // current byte we are reading
	CString			buf; // for status messages
	UINT			uByteIndex;
	
	// we need a loop here to process all read bytes from serial port
	for(uByteIndex = 0; uByteIndex < bytesRead; uByteIndex++)
	{
		ucRX = buffer[uByteIndex]; // index the read-in byte

		// Character received is returned in "ch", then copied as ucRX.

		// OK, we will receive our ECU bytes, one byte at a time. Therefore, we create
		// what is, in effect, a state machine to build up the data buffer to pass to
		// the parsing routine.

		// We need, then, four global flags to correspond with the three parts of
		// the message we are reading: Header, Length, Data, CRC.

		if (m_bReadHeader)
		{
			// Find the header byte, i.e. f4 for an LotusCarlton.
			if (m_bFirstRead)
			{
				if (ucRX != ECU_HEADER_LotusCarlton)
				{
					buf.Format("%02x - Finding start header", ucRX);
					WriteStatus(buf);
					return updatedEcuData;
				}

				buf.Format("%02x - Found main start header", ucRX);
				WriteStatus(buf);
				m_bFirstRead = FALSE;
			}
			else
			{
				if((ucRX != ECU_HEADER_LotusCarlton) & (ucRX != 0x05) & (ucRX != 0x0a))
				{// These headers must coincide with the Parser(..);
					buf.Format("%02x - Unrecognised header", ucRX);
					WriteStatus(buf);
					return updatedEcuData;
				}

				buf.Format("%02x - Header sent by ECU", ucRX);
				WriteStatus(buf);
			}

			m_ucBuffer[0] = ucRX; // Header copied to buffer

			// Received header, go to next stage
			m_bReadHeader = FALSE;
			m_bReadLength = TRUE; // drop through next time
			m_bReadData = FALSE;
			m_bReadCRC = FALSE;
		}
		else if (m_bReadLength)
		{
			// Received length
			m_ucBuffer[1] = ucRX; // Length copied to buffer
			
			m_iLen = CGMBaseFunctions::GetLength(ucRX);

			if (m_iLen == 0)
			{ // No Data so just read the CRC next time around
				m_bReadHeader = FALSE;
				m_bReadLength = FALSE;
				m_bReadData = FALSE;
				m_bReadCRC = TRUE;  // drop through next time
			}
			else
			{
				m_bReadHeader = FALSE;
				m_bReadLength = FALSE;
				m_bReadData = TRUE; // drop through next time
				m_bReadCRC = FALSE;
			}
		}
		else if (m_bReadData)
		{
			m_ucBuffer[m_iDataCount+2] = ucRX;
			m_iDataCount++; // increment our data

			if (m_iDataCount >= m_iLen)
			{ // We've read enough, go to CRC
				m_bReadHeader = FALSE;
				m_bReadLength = FALSE;
				m_bReadData = FALSE;
				m_bReadCRC = TRUE;  // drop through next time
			}
		}
		else if (m_bReadCRC)
		{
			// Received CRC
			m_ucBuffer[2 + m_iLen] = ucRX; // CRC copied to buffer

			// We have now got a complete packet in our temporary buffer.
			// All that's left to do is to parse it.

			// Send the buffer off so we can handle the TX to ECU
			HandleTX(m_ucBuffer, m_iLen + 3);

			// Now Parse it if checksum OK
			if (CGMBaseFunctions::CheckChecksum(m_ucBuffer, m_iLen + 3)) {
				updatedEcuData |= m_parser.Parse(m_ucBuffer, m_iLen + 3, ecuData);
			}
			else { // may have lost our way, so reset to find header
				m_bFirstRead = TRUE;
				WriteStatus("Checksum Error - Not Parsing !!! **** !!! **** !!!");
			}

			m_bReadHeader = TRUE;  // Start all over again... ahhhh
			m_iLen = 0;
			m_iDataCount = 0; // reset current data index
			m_bReadLength = FALSE;
			m_bReadData = FALSE;
			m_bReadCRC = FALSE;

		} // if (m_bReadHeader)
	} // for (..)

	return updatedEcuData;
}

DWORD CLotusCarltonProtocol::GetTimeoutForPingDuringInteract(void) {
	return 1000; // we want 1000ms timeout
}

// Receives the buffer and decides what mode commands to send
int CLotusCarltonProtocol::HandleTX(unsigned char* buffer, int iLength)
{
	unsigned char	ucHeader = buffer[0];
	unsigned char	ucMode = buffer[2];
//	unsigned char	ucMsg = buffer[3];
	unsigned char	ucCRC = buffer[iLength - 1]; // Index 0

	// There is a minor bug with this code. If we get a checksum error,
	// then the routine will not fire off the next request for
	// information packet. This cause the whole program to freeze for
	// 5 seconds.

	// notify parent that a whole idle header has been received
	if ((ucHeader == 0xf4) && (iLength == 3) && (ucCRC == 0xb7)) 
		OnIdle();

	// notify parent that a whole mode shut-up request has been received
	if ((ucHeader == 0xf4) && (iLength == 4) && (ucCRC == 0xae)) 
		OnModeShutUp();

	// notify parent that a whole mode 0 packet has been received
	if ((iLength == 6)) 
		OnModeD0();

	// notify parent that a whole mode 1 Msg0 request has been received
	if ((iLength == 5) && (ucMode == 0x01) && (ucCRC == 0xb5)) 
		OnMode1();

	// notify parent that a whole mode 1 Msg0 packet has been received
	if ((iLength == 67) && (ucMode == 0x01))
		OnModeD1();

	// notify parent that a whole mode 3 request has been received
	if ((iLength == 10) && (ucMode == 0x03)) 
		OnMode3();

	// notify parent that a whole mode 3 packet has been received
	if ((iLength == 7) && (ucMode == 0x03))
		OnModeD3();

	// notify parent that a whole mode 4 request has been received
	if ((iLength == 14) && (ucMode == 0x04))
		OnMode4();

	// notify parent that a whole mode 4 packet has been received
	if ((iLength == 4) && (ucMode == 0x04))
		OnModeD4();

	return iLength;
}

// Signals that the Idle header has been received
void CLotusCarltonProtocol::OnIdle(void)
{
	if (!m_bInteract)
	{
		WriteStatus("From OnIdle - Idle Header Detected but not in Interactive");
	}
	else
	{
		WriteStatus("From OnIdle - Idle Header Detected");
		SendModeShutUp(); // Stop communications
	}

	//	TRACE("From OnIdle - Idle Header Detected\n");
}

// Signals that the Mode shut-up header has been received
void CLotusCarltonProtocol::OnModeShutUp(void)
{
	WriteStatus("From OnModeShutUp - ShutUp Header Detected");
	if (!m_bInteract) 
		return; // Don't want to transmit

	if ((m_iShutUpCount==1))
	{
		WriteStatus("From OnModeShutUp - ShutUp Header Detected Command (1)");
	}
	if ((m_iShutUpCount>=2))
	{
		WriteStatus("From OnModeShutUp - ShutUp Header Detected Echo (>2)");
		SendNextCommand();
		m_iShutUpCount=0;
	}

	m_iShutUpCount++;
}

// Signals that the Mode shut-up data has been received
void CLotusCarltonProtocol::OnModeDShutUp(void)
{
	WriteStatus("From OnModeD0 - Mode Shut-Up Data Detected");
	m_dwCurrentMode = 0;
	if (!m_bInteract) return; // Don't want to transmit

//	SendNextCommand();
	//	TRACE("From OnModeD0 - Mode 0 Data Detected\n");
}

// Signals that the Mode 0 header has been received
void CLotusCarltonProtocol::OnMode0(void)
{
	WriteStatus("From OnMode0 - Mode 0 Header Detected");
	if (!m_bInteract) return; // Don't want to transmit
	m_iShutUpCount = 1; // reset the ECU OnShutUp processing
	//	TRACE("From OnMode0 - Mode 0 Header Detected\n");
}

// Signals that the Mode 0 data has been received
void CLotusCarltonProtocol::OnModeD0(void)
{
	WriteStatus("From OnModeD0 - Mode 0 Data Detected");
	m_dwCurrentMode = 0;
	if (!m_bInteract) return; // Don't want to transmit

//	SendNextCommand();
	//	TRACE("From OnModeD0 - Mode 0 Data Detected\n");
}

// Signals that the Mode 1 header has been received
void CLotusCarltonProtocol::OnMode1(void)
{
	WriteStatus("From OnMode1 - Mode 1 Header Detected");
	if (!m_bInteract) return; // Don't want to transmit
	//	TRACE("From OnMode1 - Mode 1 Header Detected\n");
}

// Signals that the Mode 1 data has been received
void CLotusCarltonProtocol::OnModeD1(void)
{
	WriteStatus("From OnModeD1 - Mode 1 Data Detected");
	m_dwCurrentMode = 1;
	if (!m_bInteract) return; // Don't want to transmit

	SendNextCommand();
	//	TRACE("From OnModeD1 - Mode 1 Data Detected\n");
}

// Signals that the Mode 3 header has been received
void CLotusCarltonProtocol::OnMode3(void)
{
	WriteStatus("From OnMode3 - Mode 3 Header Detected");
	if (!m_bInteract) return; // Don't want to transmit

	//	TRACE("From OnMode3 - Mode 3 Header Detected\n");
}

// Signals that the Mode 3 data has been received
void CLotusCarltonProtocol::OnModeD3(void)
{
	WriteStatus("From OnModeD3 - Mode 3 Data Detected");
	m_dwCurrentMode = 3;
	if (!m_bInteract) return; // Don't want to transmit

	SendNextCommand();
	//	TRACE("From OnModeD3 - Mode 3 Data Detected\n");
}

// Signals that the Mode 4 header has been received
void CLotusCarltonProtocol::OnMode4(void)
{
	WriteStatus("From OnMode4 - Mode 4 Header Detected");
	if (!m_bInteract) return; // Don't want to transmit

	//	TRACE("From OnMode4 - Mode 4 Header Detected\n");
}

// Signals that the Mode 4 data has been received
void CLotusCarltonProtocol::OnModeD4(void)
{
	WriteStatus("From OnModeD4 - Mode 4 Data Detected");
	m_dwCurrentMode = 4;
	if (!m_bInteract) return; // Don't want to transmit

	SendNextCommand();
	//	TRACE("From OnModeD4 - Mode 4 Data Detected\n");
}

// This is used to request data from the ECU.
void CLotusCarltonProtocol::OnTimer() {
	WriteStatus("Timer");
	if (!m_bInteract) return; // Don't want to transmit

	m_dwCurrentMode = 1;

	if (m_bTimeOut == TRUE)	{
		WriteStatus("From OnTimer - *** Time Out ***, sending next mode request");
		SendNextCommand();
	}

	// Always set the timeout as true. Our data stream should reset it
	m_bTimeOut = TRUE;
}
