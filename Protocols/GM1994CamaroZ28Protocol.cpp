// GM1994CamaroZ28Protocol.cpp : implementation file
//
//////////////////////////////////////////////////////////////////////
// (c) 1996-2000 Andy Whittaker, Chester, England. 
// mail@andywhittaker.com
//

#include "GM1994CamaroZ28Protocol.h"

#include "GMBaseFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGM1994CamaroZ28Protocol

CGM1994CamaroZ28Protocol::CGM1994CamaroZ28Protocol(CStatusWriter* pStatusWriter, CSupervisorInterface* pSupervisor, BOOL bInteract) : CBaseProtocol(pStatusWriter, pSupervisor, bInteract), m_parser(this) {
	Reset();
}

CGM1994CamaroZ28Protocol::~CGM1994CamaroZ28Protocol() {
}

void CGM1994CamaroZ28Protocol::InitializeSupportedValues(CEcuData* const ecuData) {
	// Put your comments and release notes about the protocol here.
	ecuData->m_csProtocolComment.Format("Name: GM724B 1994 Camaro Z28 - Impala SS - Corvette.\nVersion v1.12\nDate: 30th June 2000\nEngine Usage:\n5.7L  SFI  (LT1) (VIN = P)    1994 1,2F - Cars Auto\n5.7L  SFI  (LT1) (VIN = P)    1995 1,2F - Cars Auto\nImplemented by Andy Whittaker.\nProtocol by Andrew Mattei\nTesting by Dale Klein");
	m_parser.InitializeSupportedValues(ecuData);
}

// Resets the protocol state machine
void CGM1994CamaroZ28Protocol::Reset(void) {
	m_dwCurrentMode = 0;
	m_dwRequestedMode = 1; // Mode we want next
	m_bModeDone = TRUE; // Have we sent our mode request?
	m_ucData = 0; // data for ECU, e.g. Desired Idle

	// Reset the receive data, state machine
	m_bReadHeader = TRUE;
	m_iIdleCount = 1;
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
void CGM1994CamaroZ28Protocol::SetECUMode(const DWORD dwMode, const unsigned char data) {
	m_ucData = data; // data for ECU, e.g. Desired Idle
	m_dwRequestedMode = dwMode; // Mode we want next
	m_bModeDone = FALSE; // Have we sent our mode request?
}

// Returns the current ECU Mode
DWORD CGM1994CamaroZ28Protocol::GetCurrentMode(void) {
	return m_dwCurrentMode;
}

// Forces Shut-Up to be sent.
void CGM1994CamaroZ28Protocol::ForceDataFromECU() {
	WriteStatus("Forcing ECU with a shut-up");
	SendModeShutUp();
}

/////////////////////////////////////////////////////////////////////////////
// Commands to ECU

// Sends the Idle Mode data bytes to the ECU
BOOL CGM1994CamaroZ28Protocol::SendIdle(void)
{ //0xf4 0x56, 0x00, 0xB6
	unsigned char	ucRequestIdle[] = { 0xf4, 0x56, 0x00, 0xb6 }; // Idle
	CGMBaseFunctions::SetChecksum(ucRequestIdle, 4);
	WriteStatus("*** Sending Idle to ECU ***");
	WriteToECU(ucRequestIdle, 4, FALSE); //No delay before transmit
	return TRUE;
}

// Sends the shut-up command to the ECU
BOOL CGM1994CamaroZ28Protocol::SendModeShutUp(void)
{ //0xf4 0x56 0x08 0xae
	unsigned char	ucRequestMode0[] = { 0xf4, 0x56, 0x08, 0xae }; //
//	unsigned char	ucRequestMode0[] = { 0xf1, 0x56, 0x08, 0xb1 }; //
	WriteStatus("*** Sending Shut-Up to ECU ***");
	WriteToECU(ucRequestMode0, 4);
	return TRUE;
}

// Sends the Mode 1 Message 0 data bytes to the ECU
BOOL CGM1994CamaroZ28Protocol::SendMode1_0(void)
{ //0xf4 0x57 0x01 0x00 0xB9
	unsigned char	ucRequestMode1_0[] = { 0xf4, 0x57, 0x01, 0x00, 0xb4 }; //
	CGMBaseFunctions::SetChecksum(ucRequestMode1_0, 5);
	WriteStatus("*** Requesting Mode 1 Msg 0 from ECU ***");
	WriteToECU(ucRequestMode1_0, 5);
	return TRUE;
}

// Sends the Mode 1 Message 1 data bytes to the ECU
BOOL CGM1994CamaroZ28Protocol::SendMode1_1(void)
{ //0xf4 0x57 0x01 0x01 0xB3
	unsigned char	ucRequestMode1_0[] = { 0xf4, 0x57, 0x01, 0x01, 0xb3 }; //
	CGMBaseFunctions::SetChecksum(ucRequestMode1_0, 5);
	WriteStatus("*** Requesting Mode 1 Msg 1 from ECU ***");
	WriteToECU(ucRequestMode1_0, 5);
	return TRUE;
}

// Sends the Mode 1 Message 2 data bytes to the ECU
BOOL CGM1994CamaroZ28Protocol::SendMode1_2(void)
{ //0xf4 0x57 0x01 0x02 0xB2
	unsigned char	ucRequestMode1_0[] = { 0xf4, 0x57, 0x01, 0x02, 0xb2 }; //
	CGMBaseFunctions::SetChecksum(ucRequestMode1_0, 5);
	WriteStatus("*** Requesting Mode 1 Msg 2 from ECU ***");
	WriteToECU(ucRequestMode1_0, 5);
	return TRUE;
}

// Sends the Mode 1 Message 4 data bytes to the ECU
BOOL CGM1994CamaroZ28Protocol::SendMode1_4(void)
{ //0xf4 0x57 0x01 0x04 0xB0
	unsigned char	ucRequestMode1_0[] = { 0xf4, 0x57, 0x01, 0x04, 0xb0 }; //
	CGMBaseFunctions::SetChecksum(ucRequestMode1_0, 5);
	WriteStatus("*** Requesting Mode 1 Msg 4 from ECU ***");
	WriteToECU(ucRequestMode1_0, 5);
	return TRUE;
}

// Sends the Mode 1 Message 6 data bytes to the ECU
BOOL CGM1994CamaroZ28Protocol::SendMode1_6(void)
{ //0xf4 0x57 0x01 0x06 0xAE
	unsigned char	ucRequestMode1_0[] = { 0xf4, 0x57, 0x01, 0x06, 0xae }; //
	CGMBaseFunctions::SetChecksum(ucRequestMode1_0, 5);
	WriteStatus("*** Requesting Mode 1 Msg 6 from ECU ***");
	WriteToECU(ucRequestMode1_0, 5);
	return TRUE;
}

// Sends the Mode 3 data bytes to the ECU (Get DTCs)
BOOL CGM1994CamaroZ28Protocol::ReceiveDTCs(void)
{ //0xf4 0x57 0x01 0x00 0xB9
	unsigned char	ucRequestMode1_0[] = { 0xf4, 0x57, 0x01, 0x00, 0xb4 }; //
	CGMBaseFunctions::SetChecksum(ucRequestMode1_0, 5);
	WriteStatus("*** Requesting Mode 1 Msg 0 from ECU ***");
	WriteToECU(ucRequestMode1_0, 5);
	return TRUE;
}

// Sends the Mode 10 data bytes to the ECU (Clears DTCs)
BOOL CGM1994CamaroZ28Protocol::ClearDTCs(void)
{ //0xf4 0x56 0x0a 0xac
	unsigned char	ucRequestMode4[] = { 0xf4, 0x56, 0x0a, 0xac};
	CGMBaseFunctions::SetChecksum(ucRequestMode4, 4);
	// ECU should confirm with 0xf4 0x56 0x0a 0xac
	WriteStatus("*** Clearing DTCs in ECU ***");
	WriteToECU(ucRequestMode4, 4);
	return TRUE;
}

// Sends the Mode 4 data bytes to the ECU (Clears BLM)
BOOL CGM1994CamaroZ28Protocol::ClearBLM(void)
{ //0xf4 0x60 0x04 0x00 0x00 0x10 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x9C
	unsigned char	ucRequestMode4[] = { 0xf4, 0x60, 0x04, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9c };
	CGMBaseFunctions::SetChecksum(ucRequestMode4, 14);
	// ECU should confirm with 0xf4 0x56 0x04 0xB6
	WriteStatus("*** Clearing BLM in ECU ***");
	WriteToECU(ucRequestMode4, 14);
	return TRUE;
}

// Sends the Mode 4 data bytes to the ECU (Resets IAC)
BOOL CGM1994CamaroZ28Protocol::ResetIAC(void)
{ //0xf4 0x60 0x04 0x00 0x00 0x20 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x8C
	unsigned char	ucRequestMode4[] = { 0xf4, 0x60, 0x04, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8c };
	CGMBaseFunctions::SetChecksum(ucRequestMode4, 14);
	// ECU should confirm with 0xf4 0x56 0x04 0xB6
	WriteStatus("*** Resetting IAC in ECU ***");
	WriteToECU(ucRequestMode4, 14);
	return TRUE;
}

// Sends the Mode 4 data bytes to the ECU (Sets Desired Idle)
BOOL CGM1994CamaroZ28Protocol::SetDesiredIdle(unsigned char DesIdle)
{ //0xf4 0x60 0x04 0x01 0x01 0x00 0x00 0x10 0xFF 0x03 0x90 0x00 0x00 0x08 

	// We must set the desired idle ( desired / 12.5 ) and then
	// calculate the correct CRC.

	// Currently 1800 rpm (0x90).
	unsigned char	ucRequestDesIdle[] = { 0xf4, 0x60, 0x04, 0x01, 0x01, 0x00, 0x00, 0x10, 0xff, 0x03, 0x90, 0x00, 0x00, 0x08 };
	ucRequestDesIdle[10] = DesIdle;

	CGMBaseFunctions::SetChecksum(ucRequestDesIdle, 14);
	CString buf;
	buf.Format("*** Setting Desired Idle in ECU to %d RPM ***", (int)((DesIdle * 25) / 2));
	WriteStatus(buf);
	// ECU should confirm with 0xf4 0x56 0x04 0xB6
	WriteToECU(ucRequestDesIdle, 14);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// ECU Command handlers

// We must continuously send commands to the ECU. Here, the correct command
// is sequenced out from the PC into the ECU, as required.
void CGM1994CamaroZ28Protocol::SendNextCommand(void)
{
	if (!m_bModeDone)
	{
		switch (m_dwRequestedMode)
		{
		case 0:
			SendModeShutUp();
			break;
		case 1:
			SendMode1_0();
			break;
		case 2:
			SendMode1_1();
			break;
		case 3:
			SendMode1_2();
			break;
		case 4:
			SendMode1_4();
			break;
		case 5:
			SendMode1_6();
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
			SendMode1_0();
		}
		m_bModeDone = TRUE;
	}
	else // we normally send out Mode 1 Msg0 requests
		SendMode1_0();
}

// Handle the message from the serial port class.
BOOL CGM1994CamaroZ28Protocol::OnCharsReceived(const unsigned char* const buffer, const DWORD bytesRead, CEcuData* const ecuData) {
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
			// Find the header byte in the chatter, i.e. F0 for an GM 1994 Camaro Z28.
			if (m_bFirstRead)
			{
				if ((ucRX != ECU_HEADER_GM1994CAMAROZ28) & (ucRX != ECU_CHATTER_HEADER_GM1994CAMAROZ28))
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
				if((ucRX != ECU_HEADER_GM1994CAMAROZ28) & (ucRX != ECU_CHATTER_HEADER_GM1994CAMAROZ28) & (ucRX != 0x0a) & (ucRX != 0x10) & (ucRX != 0x40) & (ucRX != 0x41) & (ucRX != 0x45) & (ucRX != 0x90) & (ucRX != 0x92) & (ucRX != 0x93))
				{// These headers must coincide with what the Parser(..) understands;
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

// Receives the buffer and decides what mode commands to send
int CGM1994CamaroZ28Protocol::HandleTX(unsigned char* buffer, int iLength)
{
	unsigned char	ucHeader = buffer[0];
	unsigned char	ucMode = buffer[2];
	unsigned char	ucMsg = buffer[3];
	unsigned char	ucCRC = buffer[iLength - 1]; // Index 0

	// There is a minor bug with this code. If we get a checksum error,
	// then the routine will not fire off the next request for
	// information packet. This cause the whole program to freeze for
	// 5 seconds.

	// notify parent that a whole idle header has been received
	// Camaro Z28 and Impala SS
	if ((ucHeader == 0xf0) && (iLength == 4) && (ucCRC == 0xc6)) 
		OnIdle();

	// notify parent that a whole idle header has been received
	// 1994 Corvette
	if ((ucHeader == 0xf0) && (iLength == 4) && (ucCRC == 0xc9)) 
		OnIdle();

//	unsigned char	ucRequestMode0[] = { 0xf4, 0x56, 0x08, 0xae }; //
//	unsigned char	ucRequestMode0[] = { 0xf1, 0x56, 0x08, 0xb1 }; //
	// notify parent that a whole mode shut-up request has been received
	if ((iLength == 4) && (ucCRC == 0xae)) 
//	if ((iLength == 4) && (ucCRC == 0xb1)) 
		OnModeShutUp();

//	// notify parent that a whole mode shut-up packet has been received
//	if ((iLength == 4)) 
//		OnModeDShutUp();

	// notify parent that a whole mode 1 Msg0 request has been received
	if ((iLength == 5) && (ucMode == 0x01) && (ucMsg == 0x00) && (ucCRC == 0xb4)) 
		OnMode1Msg0();

	// notify parent that a whole mode 1 Msg0 packet has been received
	if ((iLength == 64) && (ucMode == 0x01))
		OnModeD1Msg0();

	// notify parent that a whole mode 1 Msg1 request has been received
	if ((iLength == 5) && (ucMode == 0x01) && (ucMsg == 0x01) && (ucCRC == 0xb3)) 
		OnMode1Msg1();

	// notify parent that a whole mode 1 Msg1 packet has been received
	if ((iLength == 50) && (ucMode == 0x01))
		OnModeD1Msg1();

	// notify parent that a whole mode 1 Msg2 request has been received
	if ((iLength == 5) && (ucMode == 0x01) && (ucMsg == 0x02) && (ucCRC == 0xb2)) 
		OnMode1Msg2();

	// notify parent that a whole mode 1 Msg2 packet has been received
	if ((iLength == 57) && (ucMode == 0x01))
		OnModeD1Msg2();

	// notify parent that a whole mode 1 Msg4 request has been received
	if ((iLength == 5) && (ucMode == 0x01) && (ucMsg == 0x04) && (ucCRC == 0xb0)) 
		OnMode1Msg4();

	// notify parent that a whole mode 1 Msg4 packet has been received
	if ((iLength == 49) && (ucMode == 0x01))
		OnModeD1Msg4();

	// notify parent that a whole mode 1 Msg6 request has been received
	if ((iLength == 5) && (ucMode == 0x01) && (ucMsg == 0x06) && (ucCRC == 0xae)) 
		OnMode1Msg6();

	// notify parent that a whole mode 1 Msg6 packet has been received
	if ((iLength == 42) && (ucMode == 0x01))
		OnModeD1Msg6();

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

	// notify parent that a whole mode 10 request has been received
	if ((iLength == 4) && (ucMode == 0x0a))
		OnMode10();

//	// notify parent that a whole mode 10 packet has been received
//	if ((iLength == 4) && (ucMode == 0x0a))
//		OnModeD10();

	return iLength;
}

// Signals that the Idle header has been received
void CGM1994CamaroZ28Protocol::OnIdle(void)
{
	if (!m_bInteract)
	{
		WriteStatus("From OnIdle - Idle Header Detected but not in Interactive");
	}
	else
	{
		WriteStatus("From OnIdle - Idle Header Detected");
		SendModeShutUp(); // Stop ECU communications
	}

	//	TRACE("From OnIdle - Idle Header Detected\n");
	return;
}

// Signals that the Mode shut-up header has been received
void CGM1994CamaroZ28Protocol::OnModeShutUp(void)
{
	WriteStatus("From OnModeShutUp - Shut-Up Header Detected");
	if (!m_bInteract) 
		return; // Don't want to transmit

	if ((m_iShutUpCount==1))
	{
	}
	if ((m_iShutUpCount>=2))
	{
		SendNextCommand();
		m_iShutUpCount=0;
	}

	m_iShutUpCount++;
	return;
}

// Signals that the Mode shut-up data has been received
void CGM1994CamaroZ28Protocol::OnModeDShutUp(void)
{
	WriteStatus("From OnModeD0 - Mode Shut-Up Data Detected");
	m_dwCurrentMode = 0;
	if (!m_bInteract) return; // Don't want to transmit

//	SendNextCommand();
	//	TRACE("From OnModeD0 - Mode 0 Data Detected\n");
	return;
}

// Signals that the Mode 1 Msg0 header has been received
void CGM1994CamaroZ28Protocol::OnMode1Msg0(void)
{
	WriteStatus("From OnMode1 - Mode 1 Msg 0 Header Detected");
	if (!m_bInteract) return; // Don't want to transmit
	//	TRACE("From OnMode1 - Mode 1 Header Detected\n");
	return;
}

// Signals that the Mode 1 Msg0 data has been received
void CGM1994CamaroZ28Protocol::OnModeD1Msg0(void)
{
	WriteStatus("From OnModeD1 - Mode 1 Msg 0 Data Detected");
	m_dwCurrentMode = 1;
	if (m_bInteract)
	{
	// Because I currently only parse Msg0 we do not 
	// need the next line.
//	OnECUMode(2, NULL); // Send the next message in sequence
	SendNextCommand();
	//	TRACE("From OnModeD1 - Mode 1 Data Detected\n");
	}
}

// Signals that the Mode 1 Msg1 header has been received
void CGM1994CamaroZ28Protocol::OnMode1Msg1(void)
{
	WriteStatus("From OnMode1 - Mode 1 Msg 1 Header Detected");
	if (!m_bInteract) return; // Don't want to transmit
	//	TRACE("From OnMode1 - Mode 1 Header Detected\n");
}

// Signals that the Mode 1 Msg1 data has been received
void CGM1994CamaroZ28Protocol::OnModeD1Msg1(void)
{
	WriteStatus("From OnModeD1 - Mode 1 Msg 1 Data Detected");
	m_dwCurrentMode = 1;
	if (!m_bInteract) return; // Don't want to transmit

	SetECUMode(3, NULL); // Send the next message in sequence
	SendNextCommand();
	//	TRACE("From OnModeD1 - Mode 1 Data Detected\n");
}

// Signals that the Mode 1 Msg2 header has been received
void CGM1994CamaroZ28Protocol::OnMode1Msg2(void)
{
	WriteStatus("From OnMode1 - Mode 1 Msg 2 Header Detected");
	if (!m_bInteract) return; // Don't want to transmit
	//	TRACE("From OnMode1 - Mode 1 Header Detected\n");
}

// Signals that the Mode 1 Msg2 data has been received
void CGM1994CamaroZ28Protocol::OnModeD1Msg2(void)
{
	WriteStatus("From OnModeD1 - Mode 1 Msg 2 Data Detected");
	m_dwCurrentMode = 1;
	if (!m_bInteract) return; // Don't want to transmit

	SetECUMode(4, NULL); // Send the next message in sequence
	SendNextCommand();
	//	TRACE("From OnModeD1 - Mode 1 Data Detected\n");
}

// Signals that the Mode 1 Msg4 header has been received
void CGM1994CamaroZ28Protocol::OnMode1Msg4(void)
{
	WriteStatus("From OnMode1 - Mode 1 Msg 4 Header Detected");
	if (!m_bInteract) return; // Don't want to transmit
	//	TRACE("From OnMode1 - Mode 1 Header Detected\n");
}

// Signals that the Mode 1 Msg4 data has been received
void CGM1994CamaroZ28Protocol::OnModeD1Msg4(void)
{
	WriteStatus("From OnModeD1 - Mode 1 Msg 4 Data Detected");
	m_dwCurrentMode = 1;
	if (!m_bInteract) return; // Don't want to transmit

	SetECUMode(5, NULL); // Send the next message in sequence
	SendNextCommand();
	//	TRACE("From OnModeD1 - Mode 1 Data Detected\n");
}

// Signals that the Mode 1 Msg6 header has been received
void CGM1994CamaroZ28Protocol::OnMode1Msg6(void)
{
	WriteStatus("From OnMode1 - Mode 1 Msg 6 Header Detected");
	if (!m_bInteract) return; // Don't want to transmit
	//	TRACE("From OnMode1 - Mode 1 Header Detected\n");
}

// Signals that the Mode 1 Msg6 data has been received
void CGM1994CamaroZ28Protocol::OnModeD1Msg6(void)
{
	WriteStatus("From OnModeD1 - Mode 1 Msg 6 Detected");
	m_dwCurrentMode = 1;
	if (!m_bInteract) return; // Don't want to transmit

	SendNextCommand();
	//	TRACE("From OnModeD1 - Mode 1 Data Detected\n");
}

// Signals that the Mode 3 header has been received
void CGM1994CamaroZ28Protocol::OnMode3(void)
{
	WriteStatus("From OnMode3 - Mode 3 Header Detected");
	if (!m_bInteract) return; // Don't want to transmit

	//	TRACE("From OnMode3 - Mode 3 Header Detected\n");
	return;
}

// Signals that the Mode 3 data has been received
void CGM1994CamaroZ28Protocol::OnModeD3(void)
{
	WriteStatus("From OnModeD3 - Mode 3 Data Detected");
	m_dwCurrentMode = 3;
	if (!m_bInteract) return; // Don't want to transmit

	SendNextCommand();
	//	TRACE("From OnModeD3 - Mode 3 Data Detected\n");
}

// Signals that the Mode 4 header has been received
void CGM1994CamaroZ28Protocol::OnMode4(void)
{
	WriteStatus("From OnMode4 - Mode 4 Header Detected");
	if (!m_bInteract) return; // Don't want to transmit

	//	TRACE("From OnMode4 - Mode 4 Header Detected\n");
}

// Signals that the Mode 4 data has been received
void CGM1994CamaroZ28Protocol::OnModeD4(void)
{
	WriteStatus("From OnModeD4 - Mode 4 Data Detected");
	m_dwCurrentMode = 4;
	if (!m_bInteract) return; // Don't want to transmit

	SendNextCommand();
	//	TRACE("From OnModeD4 - Mode 4 Data Detected\n");
}

// Signals that the Mode 10 header has been received
void CGM1994CamaroZ28Protocol::OnMode10(void)
{
	WriteStatus("From OnMode10 - Mode 10 Header Detected");
	if (!m_bInteract) return; // Don't want to transmit

	//	TRACE("From OnMode10 - Mode 10 Header Detected\n");
}
