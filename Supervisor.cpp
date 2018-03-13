// Supervisor.cpp : implementation file
//
//////////////////////////////////////////////////////////////////////
// (c) 1996-2006 Andy Whittaker, Macclesfield, England. 
// mail@andywhittaker.com
//

#include "stdafx.h"
#include "FreeScan.h"

#include "Supervisor.h"
#include "StatusDlg.h"

// Protocols installed:
// Add what extra protocols you've included here:
#include "Protocols\ELM327Protocol.h"
#include "Protocols\EspritProtocol.h"
#include "Protocols\ElanProtocol.h"
#include "Protocols\LotusCarltonProtocol.h"
#include "Protocols\GM1994CamaroZ28Protocol.h"
#include "Protocols\GM1993CamaroZ28Protocol.h"
#include "Protocols\GM1992PontiacProtocol.h"
#include "Protocols\GM1989CorvetteProtocol.h"
#include "Protocols\GMA140Protocol.h"
#include "Protocols\GMA143Protocol.h"
#include "Protocols\GMA160Protocol.h"
// You'll need to do a specific class declaration in the .h file as well.

#include "MainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSupervisor

CSupervisor::CSupervisor(CFreeScanDlg* pMainDlg, CStatusDlg* pStatusDlg)
{
	m_pMainDlg = pMainDlg;
	m_pStatusDlg = pStatusDlg;
	
	// Recall previous settings from the registry.
	CFreeScanApp* pApp = (CFreeScanApp*) AfxGetApp();
	m_bCentigrade = pApp->GetProfileInt("Supervisor", "Centigrade", TRUE);
	m_bMiles = pApp->GetProfileInt("Supervisor", "Miles", TRUE);

	m_dwBytesSent = 0;
	m_dwBytesReceived = 0;

	m_ucF005 = new unsigned char[100]; // never should get this big
	m_ucF00A = new unsigned char[100]; // never should get this big
	m_ucF001 = new unsigned char[100]; // Mode 1 data buffer
	m_ucF002 = new unsigned char[100]; // Mode 2 data buffer
	m_ucF003 = new unsigned char[100]; // Mode 3 data buffer
	m_ucF004 = new unsigned char[100]; // Mode 4 data buffer

	ResetVariables();

	m_pCom = NULL;
	m_ProtocolWnd = NULL;
	m_pParentWnd = NULL;
	m_iModel = 0;

	// Null protocol pointers
	m_pELM327Protocol = NULL;
	m_pEspritProtocol = NULL;
	m_pElanProtocol = NULL;
	m_pLotusCarltonProtocol = NULL;
	m_pGM1994CamaroZ28Protocol = NULL;
	m_pGM1993CamaroZ28Protocol = NULL;
	m_pGM1992PontiacProtocol = NULL;
	m_pGM1989CorvetteProtocol = NULL;
	m_pGMA140Protocol = NULL;
	m_pGMA143Protocol = NULL;
	m_pGMA160Protocol = NULL;
}

CSupervisor::~CSupervisor()
{
	// Save our settings to the registry
	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileInt("Supervisor", "Centigrade", m_bCentigrade);
	pApp->WriteProfileInt("Supervisor", "Miles", m_bMiles);

	delete m_ucF005;
	delete m_ucF00A;
	delete m_ucF001; // Delete Mode 1 data buffer
	delete m_ucF002; // Delete Mode 2 data buffer
	delete m_ucF003; // Delete Mode 3 data buffer
	delete m_ucF004; // Delete Mode 4 data buffer

	// Mop-up any allocated protocol classes
	Deallocate();
}

// Reset variables.
void CSupervisor::ResetVariables(void)
{
	m_csProtocolComment = "";

	memset(m_ucF005, 0, 100);
	memset(m_ucF00A, 0, 100);
	memset(m_ucF001, 0, 90);
	memset(m_ucF002, 0, 90);
	memset(m_ucF003, 0, 25);
	memset(m_ucF004, 0, 25);

	m_csDTC = "No reported faults."; // Reset Fault Codes

	// Reset normal engine parameters
	m_bEngineClosedLoop = FALSE; //
	m_bEngineStalled = TRUE;  // bit 6
	m_bACRequest = FALSE; // mode 1,  bit 0
	m_bACClutch = FALSE; // mode 1, bit 2
	m_fBatteryVolts = 0.0;
	m_iRPM = 0;
	m_iIACPosition = 0;
	m_iDesiredIdle = 0;
	m_iMPH = 0;
	m_fStartWaterTemp = 0.0;
	m_fWaterTemp = 0.0;
	m_iWaterTempADC = 0;
	m_fOilTemp = 0.0;
	m_fWaterVolts = 0.0;
	m_fMATTemp = 0.0;
	m_fMATVolts = 0.0;
	m_iMATADC = 0;
	m_iEpromID = 0;
	m_iRunTime = 0;
	m_iCrankSensors = 0;
	m_iThrottlePos = 0;
	m_fThrottleVolts = 0.0;
	m_iThrottleADC = 0;
	m_iEngineLoad = 0;
	m_fBaro = 0.0;
	m_fBaroVolts = 0.0;
	m_iBaroADC = 0;
	m_fMAP = 0.0;
	m_fMAPVolts = 0.0;
	m_iMAPADC = 0;
	m_iBoostPW = 0; // Pulse-width of the turbo boost controller
	m_iCanisterDC = 0; // Duty Cycle of Charcoal Cansister controller
	m_iSecondaryInjPW = 0; // Pulse-width of secondary injectors
	m_iInjectorBasePWMsL = 0; // Injector Opening Time in Ms Left
	m_iInjectorBasePWMsR = 0; // Injector Opening Time in Ms Right
	m_fAFRatio = 0.0; // Air Fuel Ratio
	m_fAirFlow = 0.0; // Air Flow
	m_fSparkAdvance = 0.0;
	m_fKnockRetard = 0.0;
	m_iKnockCount = 0;
	m_fO2VoltsLeft = 0.0;
	m_fO2VoltsRight = 0.0;
	m_iIntegratorL = 0; // Integrator Value Left
	m_iIntegratorR = 0; // Integrator Value Right
	m_iRichLeanCounterL = 0; // Rich/Lean Counter Left
	m_iRichLeanCounterR = 0; // Rich/Lean Counter Right
	m_iBLM = 0;	// Contents of the current BLM Cell
	m_iBLMRight = 0;	// Contents of the current BLM Cell
	m_iBLMCell = 0; // Current BLM Cell

}

// Deallocates the models we don't want to monitor.
void CSupervisor::Deallocate(void)
{
	// ELM327 OBD-II Interface Chip.
	if (m_pELM327Protocol != NULL)
		m_pELM327Protocol->DestroyWindow();
		delete m_pELM327Protocol; // Delete the active protocol
	m_pELM327Protocol = NULL;

	// Lotus Esprit 4 Cylinder.
	if (m_pEspritProtocol != NULL)
		m_pEspritProtocol->DestroyWindow();
		delete m_pEspritProtocol; // Delete the active protocol
	m_pEspritProtocol = NULL;

	// Lotus Elan M100.
	if (m_pElanProtocol != NULL)
		m_pElanProtocol->DestroyWindow();
		delete m_pElanProtocol; // Delete the active protocol
	m_pElanProtocol = NULL;

	// Lotus Carlton
	if (m_pLotusCarltonProtocol != NULL)
		m_pLotusCarltonProtocol->DestroyWindow();
		delete m_pLotusCarltonProtocol; // Delete the active protocol
	m_pLotusCarltonProtocol = NULL;

	// GM 1994 Camaro Z28.
	if (m_pGM1994CamaroZ28Protocol != NULL)
		m_pGM1994CamaroZ28Protocol->DestroyWindow();
		delete m_pGM1994CamaroZ28Protocol; // Delete the active protocol
	m_pGM1994CamaroZ28Protocol = NULL;

	// GM 1993 Camaro Z28.
	if (m_pGM1993CamaroZ28Protocol != NULL)
		m_pGM1993CamaroZ28Protocol->DestroyWindow();
		delete m_pGM1993CamaroZ28Protocol; // Delete the active protocol
	m_pGM1993CamaroZ28Protocol = NULL;

	// GM 1992 Pontiac.
	if (m_pGM1992PontiacProtocol != NULL)
		m_pGM1992PontiacProtocol->DestroyWindow();
		delete m_pGM1992PontiacProtocol; // Delete the active protocol
	m_pGM1992PontiacProtocol = NULL;

	// GM 1989 Corvette.
	if (m_pGM1989CorvetteProtocol != NULL)
		m_pGM1989CorvetteProtocol->DestroyWindow();
		delete m_pGM1989CorvetteProtocol; // Delete the active protocol
	m_pGM1989CorvetteProtocol = NULL;

	// GM A140
	if (m_pGMA140Protocol != NULL)
		m_pGMA140Protocol->DestroyWindow();
		delete m_pGMA140Protocol; // Delete the active protocol
	m_pGMA140Protocol = NULL;

	// GM A143
	if (m_pGMA143Protocol != NULL)
		m_pGMA143Protocol->DestroyWindow();
		delete m_pGMA143Protocol; // Delete the active protocol
	m_pGMA143Protocol = NULL;

	// GM A160
	if (m_pGMA160Protocol != NULL)
		m_pGMA160Protocol->DestroyWindow();
		delete m_pGMA160Protocol; // Delete the active protocol
	m_pGMA160Protocol = NULL;

	if (m_pCom != NULL)
		delete m_pCom;
	m_pCom = NULL;
}

// Initialises the Supervisor
void CSupervisor::Init(CWnd* pParentWnd, int iModel)
{
	// Assign member variables for SetCurrentPort()
	m_pParentWnd = pParentWnd;
	m_iModel = iModel;

	WriteStatus(" ");
	WriteStatus("Initialising selected protocol...");

	// Allocate and initialise the chosen ECU protocol.
	Stop();
	Deallocate();
	ResetVariables();

	// ****** This pointer should be the protocol window ******
	// The protocol window also needs access to the com port.
	
	// Create a new serial port object
	m_pCom = new CSerialPort;

	// These indexes must match the order of the text in the Vehicle Select ComboBox.
	switch(iModel)
	{
	//case 0: // ELM327 OBD-II Interface.
	//		m_pELM327Protocol = new CELM327Protocol;
	//		m_ProtocolWnd = m_pELM327Protocol->Init(this, m_pCom, pParentWnd, m_pStatusDlg);

	//		// Copy the protocol's comments across to the Supervisor.
	//		m_csProtocolComment = m_pELM327Protocol->m_csComment;
	//		break;

	case 1: // Lotus Esprit 4 Cylinder.
			m_pEspritProtocol = new CEspritProtocol;
			m_ProtocolWnd = m_pEspritProtocol->Init(this, m_pCom, pParentWnd, m_pStatusDlg);

			// Copy the protocol's comments across to the Supervisor.
			m_csProtocolComment = m_pEspritProtocol->m_csComment;
			break;

	case 2: // Lotus Elan M100.
			m_pElanProtocol = new CElanProtocol;
			m_ProtocolWnd = m_pElanProtocol->Init(this, m_pCom, pParentWnd, m_pStatusDlg);

			// Copy the protocol's comments across to the Supervisor.
			m_csProtocolComment = m_pElanProtocol->m_csComment;
			break;

	case 3: // Lotus Carlton.
			m_pLotusCarltonProtocol = new CLotusCarltonProtocol;
			m_ProtocolWnd = m_pLotusCarltonProtocol->Init(this, m_pCom, pParentWnd, m_pStatusDlg);

			// Copy the protocol's comments across to the Supervisor.
			m_csProtocolComment = m_pLotusCarltonProtocol->m_csComment;
			break;

	case 4:	// GM 1994 Camaro Z28.
			m_pGM1994CamaroZ28Protocol = new CGM1994CamaroZ28Protocol;
			m_ProtocolWnd = m_pGM1994CamaroZ28Protocol->Init(this, m_pCom, pParentWnd, m_pStatusDlg);

			// Copy the protocol's comments across to the Supervisor.
			m_csProtocolComment = m_pGM1994CamaroZ28Protocol->m_csComment;
			break;
	
	case 5:	// GM 1993 Camaro Z28.
			m_pGM1993CamaroZ28Protocol = new CGM1993CamaroZ28Protocol;
			m_ProtocolWnd = m_pGM1993CamaroZ28Protocol->Init(this, m_pCom, pParentWnd, m_pStatusDlg);

			// Copy the protocol's comments across to the Supervisor.
			m_csProtocolComment = m_pGM1993CamaroZ28Protocol->m_csComment;
			break;
	
	case 6:	// GM 1992 Pontiac.
			m_pGM1992PontiacProtocol = new CGM1992PontiacProtocol;
			m_ProtocolWnd = m_pGM1992PontiacProtocol->Init(this, m_pCom, pParentWnd, m_pStatusDlg);

			// Copy the protocol's comments across to the Supervisor.
			m_csProtocolComment = m_pGM1992PontiacProtocol->m_csComment;
			break;
	
	case 7:	// GM 1989 Corvette.
			m_pGM1989CorvetteProtocol = new CGM1989CorvetteProtocol;
			m_ProtocolWnd = m_pGM1989CorvetteProtocol->Init(this, m_pCom, pParentWnd, m_pStatusDlg);

			// Copy the protocol's comments across to the Supervisor.
			m_csProtocolComment = m_pGM1989CorvetteProtocol->m_csComment;
			break;
	
	case 8:	// GM A140
			m_pGMA140Protocol = new CGMA140Protocol;
			m_ProtocolWnd = m_pGMA140Protocol->Init(this, m_pCom, pParentWnd, m_pStatusDlg);

			// Copy the protocol's comments across to the Supervisor.
			m_csProtocolComment = m_pGMA140Protocol->m_csComment;
			break;
	
	case 9:	// GM A143
			m_pGMA143Protocol = new CGMA143Protocol;
			m_ProtocolWnd = m_pGMA143Protocol->Init(this, m_pCom, pParentWnd, m_pStatusDlg);

			// Copy the protocol's comments across to the Supervisor.
			m_csProtocolComment = m_pGMA143Protocol->m_csComment;
			break;
	
	case 10:	// GM A160
			m_pGMA160Protocol = new CGMA160Protocol;
			m_ProtocolWnd = m_pGMA160Protocol->Init(this, m_pCom, pParentWnd, m_pStatusDlg);

			// Copy the protocol's comments across to the Supervisor.
			m_csProtocolComment = m_pGMA160Protocol->m_csComment;
			break;
	
	default:
			break;
	}

}

void CSupervisor::PumpMessages()
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

// Updates the dialogs because of a data change
void CSupervisor::UpdateDialog(void)
{
	// If we want kph, work it out
	if (!m_bMiles)
		ConvertMiles();

	// If we want imperial temperatures, work them out
	if (!m_bCentigrade)
		ConvertDegrees();

	m_pMainDlg->UpdateDialog();
}

// Converts temps to degF
void CSupervisor::ConvertDegrees(void)
{
	m_fStartWaterTemp	= (float)((m_fStartWaterTemp * (float)1.8) + (float)32.0);
	m_fWaterTemp		= (float)((m_fWaterTemp * (float)1.8) + (float)32.0);
	m_fOilTemp			= (float)((m_fOilTemp * (float)1.8) + (float)32.0);
	m_fMATTemp			= (float)((m_fMATTemp * (float)1.8) + (float)32.0);
}

// Converts miles to kilometers
void CSupervisor::ConvertMiles(void)
{
	m_iMPH = (int)((float)m_iMPH * (float)1.6); 
}

// Writes a line of ASCII to the spy window
void CSupervisor::WriteStatus(CString csText)
{
	csText = "Supervisor: " + csText;
	m_pStatusDlg->WriteStatus(csText);
}

// Returns the current com port
void CSupervisor::SetCurrentPort(UINT nPort)
{
	ASSERT(m_pCom != NULL);
	ASSERT(nPort != NULL);
	Deallocate();
	m_pCom->SetPort(nPort);
	Init(m_pParentWnd, m_iModel);
}

// Returns the current com port
UINT CSupervisor::GetCurrentPort(void)
{
	ASSERT(m_pCom != NULL);
	return m_pCom->GetPort();
}

// Sets the Write Delay
void CSupervisor::SetWriteDelay(DWORD nDelay)
{
	m_pCom->SetWriteDelay(nDelay);
}

// Returns the WriteDelay value in milliseconds.
DWORD CSupervisor::GetWriteDelay(void)
{
	ASSERT(m_pCom != NULL);
	return m_pCom->GetWriteDelay();
}

// Starts the Comport.
BOOL CSupervisor::Start(void)
{
	ASSERT(m_pCom != NULL);
	// This sets the com port up
	if (!m_pCom->StartMonitoring())
		WriteStatus("CSupervisor- Failed to start the Com Port");
	else
		WriteStatus("CSupervisor- Com Port started");
	return TRUE;
}

// Restarts the Comport.
BOOL CSupervisor::Restart(void)
{
	ASSERT(m_pCom != NULL);
	// This sets the com port up
	if (!m_pCom->RestartMonitoring())
	{
		WriteStatus("CSupervisor- Failed to restart the Com Port");
		return FALSE;
	}
	else
		WriteStatus("CSupervisor- Com Port restarted");
		::SendMessage(m_ProtocolWnd, WM_PROT_CMD_RESETSTATE, (WPARAM) NULL, (LPARAM) NULL);
	return TRUE;
}

// Stops the Comport.
BOOL CSupervisor::Stop(void)
{
	if (m_pCom == NULL)
		return TRUE;

	// This sets the com port up
	if (!m_pCom->StopMonitoring())
	{
		WriteStatus("CSupervisor- Failed to stop the Com Port");
		return FALSE;
	}
	else
		WriteStatus("CSupervisor- Com Port stopped");
	return TRUE;
}

// Ensures the Supervisor is exited cleanly
BOOL CSupervisor::ShutDown(void)
{
//	DestroyWindow();
	return TRUE;
}

// Starts or stops csv logging to file
BOOL CSupervisor::StartCSVLog(BOOL bStart)
{
	return (BOOL) ::SendMessage(m_ProtocolWnd, WM_PROT_CMD_STARTCSV, (WPARAM) bStart, (LPARAM) NULL);
}

// Requests whether FreeScan talks to the ECU or not
void CSupervisor::Interact(BOOL bInteract)
{
	::SendMessage(m_ProtocolWnd, WM_PROT_CMD_SETINTERACT, (WPARAM) bInteract, (LPARAM) NULL);
}

// Sets FreeScan to Metric or Imperial
void CSupervisor::Centigrade(BOOL bUnit)
{
	m_bCentigrade = bUnit;
}

// Sets FreeScan to Metric or Imperial
void CSupervisor::Miles(BOOL bUnit)
{
	m_bMiles = bUnit;
}

// Returns what FreeScan's Units are set to
BOOL CSupervisor::GetCentigrade(void)
{
	return m_bCentigrade;
}

// Returns what FreeScan's Units are set to
BOOL CSupervisor::GetMiles(void)
{
	return m_bMiles;
}

// Gets the interact status
BOOL CSupervisor::GetInteract(void)
{
	BOOL	bResult;
	bResult = (BOOL) ::SendMessage(m_ProtocolWnd, WM_PROT_CMD_GETINTERACT, (WPARAM) TRUE, (LPARAM) FALSE);
	return bResult;
}

// Returns the current ECU Mode
DWORD CSupervisor::GetCurrentMode(void)
{
#ifdef _DEBUG
	return 1;// **** Test
#else
	return (DWORD) ::SendMessage(m_ProtocolWnd, WM_PROT_CMD_GETECUMODE, (WPARAM) NULL, (LPARAM) NULL);
#endif // _DEBUG
}

// This switches the mode number that is sent to the ECU. It changes the
// behaviour of SendNextCommand(..).
void CSupervisor::ECUMode(DWORD dwMode, unsigned char Data)
{
	::SendMessage(m_ProtocolWnd, WM_PROT_CMD_ECUMODE, (WPARAM) dwMode, (LPARAM) Data);
}

// Forces the protocol to send a shut-up command to the ECU.
void CSupervisor::ForceShutUp(void)
{
	::SendMessage(m_ProtocolWnd, WM_PROT_CMD_FORCESHUTUP, (WPARAM) NULL, (LPARAM) NULL);
}

// Sends a packet to the current parser for testing
void CSupervisor::Test() 
{
	if (m_pEspritProtocol != NULL)
	{
		unsigned char buffer[] = {(unsigned char)0xF0,(unsigned char)0x96,(unsigned char)0x01,
			(unsigned char)0x27,(unsigned char)0x89,(unsigned char)0x00,(unsigned char)0x08,(unsigned char)0x00,(unsigned char)0x60,(unsigned char)0x80,(unsigned char)0x3c,
			(unsigned char)0x67,(unsigned char)0x1c,(unsigned char)0x6a,(unsigned char)0x05,(unsigned char)0x1a,(unsigned char)0x00,(unsigned char)0x01,(unsigned char)0x20,
			(unsigned char)0x2a,(unsigned char)0x38,(unsigned char)0x68,(unsigned char)0x7c,(unsigned char)0x13,(unsigned char)0x00,(unsigned char)0x68,(unsigned char)0x73,
			(unsigned char)0x51,(unsigned char)0xa8,(unsigned char)0x20,(unsigned char)0x21,(unsigned char)0x9f,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x30,
			(unsigned char)0x28,(unsigned char)0x8a,(unsigned char)0x33,(unsigned char)0x12,(unsigned char)0x00,(unsigned char)0x01,(unsigned char)0x00,(unsigned char)0x4e,
			(unsigned char)0x71,(unsigned char)0x67,(unsigned char)0x01,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0xbe,(unsigned char)0x8b,(unsigned char)0x1a,
			(unsigned char)0x1a,(unsigned char)0xf0,(unsigned char)0x09,(unsigned char)0x00,(unsigned char)0x41,(unsigned char)0x1c,(unsigned char)0x7f,(unsigned char)0xbd,
			(unsigned char)0x49,(unsigned char)0x2d,(unsigned char)0x00,(unsigned char)0x01,(unsigned char)0xc6,(unsigned char)0x01,(unsigned char)0x90,(unsigned char)0x20,
			(unsigned char)0x1f};

		m_pEspritProtocol->Parse(buffer, 68);
	}
	else if (m_pGM1989CorvetteProtocol !=NULL)
	{
	//0x80 0x95 0x01 

	//0x08 0xCB 0x00 0x00 0x00 0x00 0x00 0xC3
	//0xA9 0x1D 0x18 0x06 0x56 0x00 0x00 0xFF
	//0x3B 0xE6 0x78 0x6C 0x00 0x76 0x1B 0x00
	//0x30 0x2F 0x80 0x2F 0xC5 0xC5 0x00 0x00
	//0xFF 0x87 0x83 0x05 0xED 0x05 0x00 0x39
	//0x00 0x2A 0x03 0x00 0x00 0x55 0x01 0xBD
	//0xB9 0x37 0x00 0x03 0x5B 0x05 0xD1 0x2E
	//0xE8 0x7B 0x07 0x20 0x02 0x0C 0x82
	//0x08
		unsigned char buffer[] = {(unsigned char)0x80,(unsigned char)0x95,(unsigned char)0x01,

			(unsigned char)0x08,(unsigned char)0xcb,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0xc3,
			(unsigned char)0xa9,(unsigned char)0x1d,(unsigned char)0x18,(unsigned char)0x06,(unsigned char)0x56,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0xff,
			(unsigned char)0x3b,(unsigned char)0xe6,(unsigned char)0x78,(unsigned char)0x6c,(unsigned char)0x00,(unsigned char)0x76,(unsigned char)0x1b,(unsigned char)0x00,
			(unsigned char)0x30,(unsigned char)0x2f,(unsigned char)0x80,(unsigned char)0x2f,(unsigned char)0xc5,(unsigned char)0xc5,(unsigned char)0x00,(unsigned char)0x00,
			(unsigned char)0xff,(unsigned char)0x87,(unsigned char)0x83,(unsigned char)0x05,(unsigned char)0xed,(unsigned char)0x05,(unsigned char)0x00,(unsigned char)0x39,
			(unsigned char)0x00,(unsigned char)0x2a,(unsigned char)0x03,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x55,(unsigned char)0x01,(unsigned char)0xbd,
			(unsigned char)0xb9,(unsigned char)0x37,(unsigned char)0x00,(unsigned char)0x03,(unsigned char)0x5b,(unsigned char)0x05,(unsigned char)0xd1,(unsigned char)0x2e,
			(unsigned char)0xe8,(unsigned char)0x7b,(unsigned char)0x07,(unsigned char)0x20,(unsigned char)0x02,(unsigned char)0x0c,(unsigned char)0x82,
			(unsigned char)0x08};

		m_pGM1989CorvetteProtocol->Parse(buffer, 67);
	}
	else if (m_pGM1994CamaroZ28Protocol !=NULL)
	{
		unsigned char buffer[] = {(unsigned char)0xF4,(unsigned char)0x92,(unsigned char)0x01,
			(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x40,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,
			(unsigned char)0x1c,(unsigned char)0x2f,(unsigned char)0x0c,(unsigned char)0x04,(unsigned char)0x42,(unsigned char)0x40,(unsigned char)0x00,(unsigned char)0x40,
			(unsigned char)0x56,(unsigned char)0x0e,(unsigned char)0x9c,(unsigned char)0x08,(unsigned char)0x3c,(unsigned char)0x20,(unsigned char)0x6e,(unsigned char)0x04,
			(unsigned char)0x50,(unsigned char)0x28,(unsigned char)0x27,(unsigned char)0x3d,(unsigned char)0x1d,(unsigned char)0x00,(unsigned char)0x8a,(unsigned char)0xf2,
			(unsigned char)0xa4,(unsigned char)0x65,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x10,(unsigned char)0x80,(unsigned char)0x80,(unsigned char)0x80,
			(unsigned char)0x80,(unsigned char)0x00,(unsigned char)0xfb,(unsigned char)0x00,(unsigned char)0xfa,(unsigned char)0x04,(unsigned char)0x83,(unsigned char)0x00,
			(unsigned char)0x3c,(unsigned char)0x22,(unsigned char)0x0e,(unsigned char)0x00,(unsigned char)0x02,(unsigned char)0xb3,(unsigned char)0x01,(unsigned char)0x00,
			(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x6f,
			(unsigned char)0xab};

		m_pGM1994CamaroZ28Protocol->Parse(buffer, 64);
	}
	else if (m_pElanProtocol != NULL)
	{
		unsigned char buffer[] = {
				(unsigned char)0xF4,(unsigned char)0x95,(unsigned char)0x01,
				(unsigned char)0x26,(unsigned char)0xca,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0xc6,(unsigned char)0x48,(unsigned char)0x00,
				(unsigned char)0x18,(unsigned char)0x00,(unsigned char)0x22,(unsigned char)0x01,(unsigned char)0x09,(unsigned char)0x23,(unsigned char)0x00,(unsigned char)0x58,
				(unsigned char)0x00,(unsigned char)0x65,(unsigned char)0x80,(unsigned char)0x80,(unsigned char)0x14,(unsigned char)0x4c,(unsigned char)0x48,(unsigned char)0x22,
				(unsigned char)0x00,(unsigned char)0x8c,(unsigned char)0x00,(unsigned char)0xb5,(unsigned char)0x80,(unsigned char)0x30,(unsigned char)0x01,(unsigned char)0x7e,
				(unsigned char)0x8c,(unsigned char)0x07,(unsigned char)0x24,(unsigned char)0x23,(unsigned char)0x2f,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x08,
				(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x24,(unsigned char)0x5e,(unsigned char)0x00,(unsigned char)0x35,(unsigned char)0x6f,(unsigned char)0x16,
				(unsigned char)0x00,(unsigned char)0x1d,(unsigned char)0x02,(unsigned char)0x80,(unsigned char)0x00,(unsigned char)0x04,(unsigned char)0x40,(unsigned char)0x05,
				(unsigned char)0x02,(unsigned char)0x41,(unsigned char)0x01,(unsigned char)0xc9,(unsigned char)0x00,(unsigned char)0x17,(unsigned char)0x65,
				(unsigned char)0x5c};

		m_pElanProtocol->Parse(buffer, 67);
	}
	else if (m_pGMA143Protocol != NULL)
	{
		unsigned char buffer[] = {
				(unsigned char)0xF0,(unsigned char)0x99,(unsigned char)0x01,
				(unsigned char)0xF8,(unsigned char)0x84,(unsigned char)0x9E,(unsigned char)0xC1,(unsigned char)0x86,(unsigned char)0x27,(unsigned char)0x90,(unsigned char)0xA2,
				(unsigned char)0x05,(unsigned char)0x00,(unsigned char)0xAC,(unsigned char)0x62,(unsigned char)0x12,(unsigned char)0x1A,(unsigned char)0x07,(unsigned char)0xD8,
				(unsigned char)0x00,(unsigned char)0x3F,(unsigned char)0x49,(unsigned char)0x10,(unsigned char)0x09,(unsigned char)0x88,(unsigned char)0x7E,(unsigned char)0x10,
				(unsigned char)0x28,(unsigned char)0x34,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x21,(unsigned char)0x8E,(unsigned char)0x04,(unsigned char)0xF0,
				(unsigned char)0x00,(unsigned char)0x47,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0xC8,(unsigned char)0x00,(unsigned char)0x6F,
				(unsigned char)0x87,(unsigned char)0x4C,(unsigned char)0x11,(unsigned char)0x01,(unsigned char)0x71,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,
				(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,
				(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,
				(unsigned char)0x00,(unsigned char)0x1F,(unsigned char)0xEE,(unsigned char)0x07};

		m_pGMA143Protocol->Parse(buffer, 71);
	}
	else if (m_pGMA160Protocol != NULL)
	{
		unsigned char buffer[] = {
				(unsigned char)0xF0,(unsigned char)0x95,(unsigned char)0x01,
				(unsigned char)0x08,(unsigned char)0x35,(unsigned char)0x10,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x7a,(unsigned char)0x35,(unsigned char)0x32,
				(unsigned char)0x18,(unsigned char)0x4f,(unsigned char)0x03,(unsigned char)0xe2,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x1f,(unsigned char)0x54,
				(unsigned char)0x1b,(unsigned char)0x7d,(unsigned char)0x00,(unsigned char)0x93,(unsigned char)0x20,(unsigned char)0x64,(unsigned char)0xe0,(unsigned char)0x91,
				(unsigned char)0x00,(unsigned char)0x70,(unsigned char)0x5f,(unsigned char)0xee,(unsigned char)0xd0,(unsigned char)0x1f,(unsigned char)0x01,(unsigned char)0x00,
				(unsigned char)0x54,(unsigned char)0x64,(unsigned char)0x20,(unsigned char)0x00,(unsigned char)0xae,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0xea,
				(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,
				(unsigned char)0x2a,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x85,(unsigned char)0x00,(unsigned char)0xa3,(unsigned char)0x87,(unsigned char)0xff,
				(unsigned char)0xe0,(unsigned char)0x20,(unsigned char)0x03,(unsigned char)0x7d,(unsigned char)0x6f,(unsigned char)0x00,(unsigned char)0xa0,(unsigned char)0xe9};
/*		unsigned char buffer[] = {
				(unsigned char)0xF0,(unsigned char)0x95,(unsigned char)0x01,
				(unsigned char)0x08,(unsigned char)0x35,(unsigned char)0x10,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x32,(unsigned char)0x32,(unsigned char)0x23,
				(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x65,(unsigned char)0x00,
				(unsigned char)0x00,(unsigned char)0x80,(unsigned char)0x00,(unsigned char)0x80,(unsigned char)0x90,(unsigned char)0x64,(unsigned char)0xeb,(unsigned char)0x74,
				(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0xee,(unsigned char)0x1d,(unsigned char)0x74,(unsigned char)0x00,(unsigned char)0x05,
				(unsigned char)0x08,(unsigned char)0x04,(unsigned char)0x90,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,
				(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,
				(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x00,(unsigned char)0x7e,(unsigned char)0x00,(unsigned char)0xc5,(unsigned char)0x1f,(unsigned char)0xff,
				(unsigned char)0x20,(unsigned char)0x20,(unsigned char)0x02,(unsigned char)0x54,(unsigned char)0x6f,(unsigned char)0x02,(unsigned char)0x00,(unsigned char)0x6a};
*/
		m_pGMA160Protocol->Parse(buffer, 67);
	}
	else
	{
		AfxMessageBox("No Test Bytes for this protocol");
	}
}
