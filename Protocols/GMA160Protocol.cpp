// GMA160Protocol.cpp : implementation file
//
//////////////////////////////////////////////////////////////////////
// (c) 1996-2000 Andy Whittaker, Chester, England. 
// mail@andywhittaker.com
//

#include "stdafx.h"
#include "..\FreeScan.h"
#include "GMA160Protocol.h"

#include "..\Supervisor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGMA160Protocol

CGMA160Protocol::CGMA160Protocol()
{
	// Put your comments and release notes about the protocol here.
	m_csComment.Format("Name: GM610B\nVersion v1.0\nDate: 4th November 2000\nENGINE USAGE:\n2.0L, L4, SOHC Turbo Engine (VIN code M)\n4.3L  TURBO   LB4  (VIN = Z) 1991.5      SYCLONE\n4.3L  TURBO   LB4  (VIN = Z) 1992         T-truck\n4.3L  TURBO   LB4  (VIN = Z) 1993         T-truck\nImplemented by Andy Whittaker.\nTested by Moe Whittemore.");

	// Recall previous settings from the registry.
	CWinApp* pApp = AfxGetApp();
	m_bInteract = pApp->GetProfileInt("GMA160Protocol", "Interact", FALSE);

	m_pcom = NULL;

	OnResetStateMachine(NULL,NULL);
}

CGMA160Protocol::~CGMA160Protocol()
{
	// Save our settings to the registry
	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileInt("GMA160Protocol", "Interact", m_bInteract);
}


BEGIN_MESSAGE_MAP(CGMA160Protocol, CWnd)
	//{{AFX_MSG_MAP(CGMA160Protocol)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_MESSAGE(WM_PROT_CMD_RESETSTATE, OnResetStateMachine)
	ON_MESSAGE(WM_PROT_CMD_SETINTERACT, OnInteract)
	ON_MESSAGE(WM_PROT_CMD_GETINTERACT, OnGetInteract)
	ON_MESSAGE(WM_PROT_CMD_ECUMODE, OnECUMode)
	ON_MESSAGE(WM_PROT_CMD_GETECUMODE, OnGetCurrentMode)
	ON_MESSAGE(WM_PROT_CMD_FORCESHUTUP, OnForceShutUp)
	ON_MESSAGE(WM_PROT_CMD_STARTCSV, OnStartCSV)
	ON_MESSAGE(WM_COMM_RXCHAR, OnCharReceived)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CGMA160Protocol::PumpMessages()
{
	MSG msg;
	// if there is a message on the queue, then dispatch it
	if(::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE )) 
 	{ 
		::GetMessage(&msg, NULL, NULL, NULL);
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	} 
}

/////////////////////////////////////////////////////////////////////////////
// Interfaces to this class

// Initialises the Supervisor
HWND CGMA160Protocol::Init(CSupervisor* pSupervisor, CSerialPort* pcom, CWnd* pParentWnd, CStatusDlg* pStatusDlg)
{
	m_pSupervisor = pSupervisor; // our owner
	m_pStatusDlg = pStatusDlg; // Debug Window
	m_pcom = pcom; // assign our serial port pointer.

	WriteStatus("Creating GMA160 Protocol Window");
	CreateProtocolWnd(pParentWnd); // creates this window for communication messages

	// This sets up the com port CSerialPort Object
	// Note: Look in SerialPort.h for the defaults:
	// We need 8192baud, 1 start, 1 stop and no parity.
	// We pass the CSerialPort a this pointer because it
	// needs to send messages to this window via the CWnd Object
	if (!m_pcom->InitPort(this, NULL))
		WriteStatus("Failed to initialise the Com Port");
	else
		WriteStatus("Com Port initialised");

	return m_hWnd;
}

// Resets the protocol state machine
LONG CGMA160Protocol::OnResetStateMachine(WPARAM wdummy, LPARAM dummy)
{
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

	return 0;
}

// Requests whether FreeScan talks to the ECU or not
LONG CGMA160Protocol::OnInteract(WPARAM bInteract, LPARAM dummy)
{
	if (bInteract)
	{
		WriteStatus("Interaction with the ECU enabled.");
		m_bInteract=TRUE;
	}
	else
	{
		WriteStatus("In monitor mode, no interaction with ECU will be done.");
		m_bInteract=FALSE;
	}
	return 0;
}

// This switches the mode number that is sent to the ECU. It changes the
// behaviour of SendNextCommand(..).
LONG CGMA160Protocol::OnECUMode(WPARAM dwMode, LPARAM Data)
{
	m_ucData = (unsigned char) Data; // data for ECU, e.g. Desired Idle
	m_dwRequestedMode = (DWORD) dwMode; // Mode we want next
	m_bModeDone = FALSE; // Have we sent our mode request?
	return 0;
}

LONG CGMA160Protocol::OnStartCSV(WPARAM bStart, LPARAM dummy)
{
	// call the base class function
	return (LONG) StartCSVLog((BOOL) bStart);
}

// Gets the interact status
LONG CGMA160Protocol::OnGetInteract(WPARAM wdummy, LPARAM dummy)
{
	return (LONG) m_bInteract;
}

// Returns the current ECU Mode
LONG CGMA160Protocol::OnGetCurrentMode(WPARAM wdummy, LPARAM dummy)
{
	return (LONG) m_dwCurrentMode;
}

// Forces Shut-Up to be sent.
LONG CGMA160Protocol::OnForceShutUp(WPARAM wdummy, LPARAM dummy)
{
	WriteStatus("Forcing ECU with a shut-up");
	m_bModeDone = TRUE; // This is for the SendNextCommand() function
	SendMode1_0();
	return (LONG) 0;
}

/////////////////////////////////////////////////////////////////////////////
// Commands to ECU

// Sends the Idle Mode data bytes to the ECU
BOOL CGMA160Protocol::SendIdle(void)
{ //0x80 0x56, 0x00, 0x2a
	unsigned char	ucRequestIdle[] = { 0xF0, 0x55, 0xbb }; // Idle
	SetChecksum(ucRequestIdle, 3);
	WriteStatus("*** Sending Idle to ECU ***");
	WriteToECU(ucRequestIdle, 3, FALSE); //No delay before transmit
	return TRUE;
}

// Sends the shut-up command to the ECU
BOOL CGMA160Protocol::SendModeShutUp(void)
{ //0x80 0x56 0x08 0x22
	unsigned char	ucRequestMode0[] = { 0xF0, 0x56, 0x08, 0xB2 }; //
	WriteStatus("*** Sending Shut-Up to ECU ***");
	WriteToECU(ucRequestMode0, 4);
	return TRUE;
}

// Sends the Mode 1 Message 0 data bytes to the ECU
BOOL CGMA160Protocol::SendMode1_0(void)
{ //
	unsigned char	ucRequestMode1_0[] = { 0xF0, 0x56, 0x01, 0xB9 }; //
	SetChecksum(ucRequestMode1_0, 4);
	WriteStatus("*** Requesting Mode 1 from ECU ***");
	WriteToECU(ucRequestMode1_0, 4);
	return TRUE;
}

// Sends the Mode 3 data bytes to the ECU (Get DTCs)
BOOL CGMA160Protocol::ReceiveDTCs(void)
{ //0x80 0x57 0x01 0x00 0x28
	unsigned char	ucRequestMode3[] = { 0xF0, 0x5c, 0x03, 0x00, 0x05, 0x00, 0x06, 0x00, 0x07, 0x0f };
	// ECU should send 0x80 0x59 0x03 0x00 0x00 0x00 0x0f;
	WriteStatus("*** Requesting DTCs from ECU ***");
	WriteToECU(ucRequestMode3, 10);
	return TRUE;
}

// Sends the Mode 10 data bytes to the ECU (Clears DTCs)
BOOL CGMA160Protocol::ClearDTCs(void)
{ //0x80 0x56 0x0a 0x2c
	unsigned char	ucRequestMode4[] = { 0xF0, 0x56, 0x0a, 0xB0};
	SetChecksum(ucRequestMode4, 4);
	// ECU should confirm with 0x80 0x56 0x0a 0x2c
	WriteStatus("*** Clearing DTCs in ECU ***");
	WriteToECU(ucRequestMode4, 4);
	return TRUE;
}

// Sends the Mode 4 data bytes to the ECU (Clears BLM)
BOOL CGMA160Protocol::ClearBLM(void)
{ //0x80 0x60 0x04 0x00 0x00 0x10 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x0C
	unsigned char	ucRequestMode4[] = { 0xF0, 0x60, 0x04, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c };
	SetChecksum(ucRequestMode4, 14);
	// ECU should confirm with 0x80 0x56 0x04 0x26
	WriteStatus("*** Clearing BLM in ECU ***");
	WriteToECU(ucRequestMode4, 14);
	return TRUE;
}

// Sends the Mode 4 data bytes to the ECU (Resets IAC)
BOOL CGMA160Protocol::ResetIAC(void)
{ //0x80 0x60 0x04 0x00 0x00 0x20 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0xfC
	unsigned char	ucRequestMode4[] = { 0xF0, 0x60, 0x04, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc };
	SetChecksum(ucRequestMode4, 14);
	// ECU should confirm with 0x80 0x56 0x04 0x26
	WriteStatus("*** Resetting IAC in ECU ***");
	WriteToECU(ucRequestMode4, 14);
	return TRUE;
}

// Sends the Mode 4 data bytes to the ECU (Sets Desired Idle)
BOOL CGMA160Protocol::SetDesiredIdle(unsigned char DesIdle)
{ //0x80 0x60 0x04 0x01 0x01 0x00 0x00 0x10 0xFF 0x03 0x90 0x00 0x00 0x78 

	// We must set the desired idle ( desired / 12.5 ) and then
	// calculate the correct CRC.

	// Currently 1800 rpm (0x90).
	unsigned char	ucRequestDesIdle[] = { 0xF0, 0x60, 0x04, 0x01, 0x01, 0x00, 0x00, 0x10, 0xff, 0x03, 0x90, 0x00, 0x00, 0x78 };
	ucRequestDesIdle[10] = DesIdle;

	SetChecksum(ucRequestDesIdle, 14);
	CString buf;
	buf.Format("*** Setting Desired Idle in ECU to %d RPM ***", (int)((DesIdle * 25) / 2));
	WriteStatus(buf);
	// ECU should confirm with 0x80 0x56 0x04 0x26
	WriteToECU(ucRequestDesIdle, 14);
	return TRUE;
}

// Write a string to the port - This can even write NULL characters
void CGMA160Protocol::WriteToECU(unsigned char* string, int stringlength, BOOL bDelay)
{	
	m_pSupervisor->m_dwBytesSent += stringlength;
	m_pcom->WriteToPort(string, stringlength, bDelay);
}

/////////////////////////////////////////////////////////////////////////////
// ECU Command handlers

// We must continuously send commands to the ECU. Here, the correct command
// is sequenced out from the PC into the ECU, as required.
void CGMA160Protocol::SendNextCommand(void)
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
	else // we normally send out Mode 1 requests
		SendMode1_0();
}

/////////////////////////////////////////////////////////////////////////////
// CGMA160Protocol message handlers

// The supervisor is a hidden window. This is to enable it to receive
// messages from itself and the serial port class.
BOOL CGMA160Protocol::CreateProtocolWnd(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	DWORD	dwStyle = WS_BORDER | WS_CAPTION | WS_CHILD;
	RECT	rect;
	UINT nID = 67; // It's my house number!

	rect.top = 0;
	rect.bottom = 50;
	rect.left = 0;
	rect.right = 50;
	
	return Create(NULL, "ECU GMA160 Communications Supervisor", dwStyle, rect, pParentWnd, nID, NULL);
}

// Handle the message from the serial port class.
LONG CGMA160Protocol::OnCharReceived(WPARAM ch, LPARAM BytesRead)
{
	 // convert passed variables
	unsigned char*	pucRX = (unsigned char*) ch;
	DWORD			uBytesRead = (DWORD) BytesRead;
	
	unsigned char	ucRX; // current byte we are reading
	CString			buf; // for status messages
	UINT			uByteIndex;
	
	// we need a loop here to process all read bytes from serial port
	for(uByteIndex = 0; uByteIndex < uBytesRead; uByteIndex++)
	{
		ucRX = pucRX[uByteIndex]; // index the read-in byte
		m_pSupervisor->m_dwBytesReceived ++;

		// Character received is returned in "ch", then copied as ucRX.

		// OK, we will receive our ECU bytes, one byte at a time. Therefore, we create
		// what is, in effect, a state machine to build up the data buffer to pass to
		// the parsing routine.

		// We need, then, four global flags to correspond with the three parts of
		// the message we are reading: Header, Length, Data, CRC.

		if (m_bReadHeader)
		{
			// Find the header byte in the chatter, i.e. 80 for an GM 1989 Corvette.
			if (m_bFirstRead)
			{
				if ((ucRX != ECU_HEADER_GMA160) & (ucRX != ECU_CHATTER_HEADER_GMA160))
				{
					buf.Format("%02x - Finding GMA160 start header", ucRX);
					WriteStatus(buf);
					return 0;
				}

				buf.Format("%02x - Found GMA160 main start header", ucRX);
				WriteStatus(buf);
				m_bFirstRead = FALSE;
			}
			else
			{
				if((ucRX != ECU_HEADER_GMA160) & (ucRX != ECU_CHATTER_HEADER_GMA160) & (ucRX != 0x90) & (ucRX != 0x0a))
				{// These headers must coincide with what the Parser(..) understands;
					buf.Format("%02x - Unrecognised header", ucRX);
					WriteStatus(buf);
					return 0;
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
			
			m_iLen = GetLength(ucRX);

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
			if (CheckChecksum(m_ucBuffer, m_iLen + 3))
				Parse(m_ucBuffer, m_iLen + 3);
			else
			{// may have lost our way, so reset to find header
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
	} // for(..)
	return 0;
}

// Receives the buffer and decides what mode commands to send
int CGMA160Protocol::HandleTX(unsigned char* buffer, int iLength)
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
	if ((ucHeader == 0xF0) && (iLength == 3) && (ucCRC == 0xbb)) 
		OnIdle();

	// notify parent that a whole mode 1 request has been received
	if ((iLength == 4) && (ucMode == 0x01) && (ucCRC == 0xB9)) 
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

	// notify parent that a whole mode 10 request has been received
	if ((iLength == 4) && (ucMode == 0x0a))
		OnMode10();

//	// notify parent that a whole mode 10 packet has been received
//	if ((iLength == 4) && (ucMode == 0x0a))
//		OnModeD10();

	return iLength;
}

// Signals that the Idle header has been received
void CGMA160Protocol::OnIdle(void)
{
	if (!m_bInteract)
	{
		WriteStatus("From OnIdle - Idle Header Detected but not in Interactive");
	}
	else
	{
		WriteStatus("From OnIdle - Idle Header Detected");
		SendMode1_0(); // Try Mode 1
	}
	//	TRACE("From OnIdle - Idle Header Detected\n");
}

// Signals that the Mode 1 header has been received
void CGMA160Protocol::OnMode1(void)
{
	WriteStatus("From OnMode1 - Mode 1 Header Detected");
	if (!m_bInteract) return; // Don't want to transmit
	//	TRACE("From OnMode1 - Mode 1 Header Detected\n");
}

// Signals that the Mode 1 data has been received
void CGMA160Protocol::OnModeD1(void)
{
	WriteStatus("From OnModeD1 - Mode 1 Data Detected");
	m_dwCurrentMode = 1;
	if (!m_bInteract) return; // Don't want to transmit

	SendNextCommand();
	//	TRACE("From OnModeD1 - Mode 1 Data Detected\n");
}

// Signals that the Mode 3 header has been received
void CGMA160Protocol::OnMode3(void)
{
	WriteStatus("From OnMode3 - Mode 3 Header Detected");
	if (!m_bInteract) return; // Don't want to transmit

	//	TRACE("From OnMode3 - Mode 3 Header Detected\n");
}

// Signals that the Mode 3 data has been received
void CGMA160Protocol::OnModeD3(void)
{
	WriteStatus("From OnModeD3 - Mode 3 Data Detected");
	m_dwCurrentMode = 3;
	if (!m_bInteract) return; // Don't want to transmit

	SendNextCommand();
	//	TRACE("From OnModeD3 - Mode 3 Data Detected\n");
}

// Signals that the Mode 4 header has been received
void CGMA160Protocol::OnMode4(void)
{
	WriteStatus("From OnMode4 - Mode 4 Header Detected");
	if (!m_bInteract) return; // Don't want to transmit

	//	TRACE("From OnMode4 - Mode 4 Header Detected\n");
}

// Signals that the Mode 4 data has been received
void CGMA160Protocol::OnModeD4(void)
{
	WriteStatus("From OnModeD4 - Mode 4 Data Detected");
	m_dwCurrentMode = 4;
	if (!m_bInteract) return; // Don't want to transmit

	SendNextCommand();
	//	TRACE("From OnModeD4 - Mode 4 Data Detected\n");
}

// Signals that the Mode 10 header has been received
void CGMA160Protocol::OnMode10(void)
{
	WriteStatus("From OnMode10 - Mode 10 Header Detected");
	if (!m_bInteract) return; // Don't want to transmit

	//	TRACE("From OnMode10 - Mode 10 Header Detected\n");
}

BOOL CGMA160Protocol::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
