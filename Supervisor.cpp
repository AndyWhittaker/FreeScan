// Supervisor.cpp : implementation file
//
//////////////////////////////////////////////////////////////////////
// (c) 1996-2006 Andy Whittaker, Macclesfield, England. 
// mail@andywhittaker.com
//

#include "Supervisor.h"

#include "FreeScan.h"

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

#define		WM_TIMER_EVT1	0

/////////////////////////////////////////////////////////////////////////////
// CSupervisor

CSupervisor::CSupervisor(CFreeScanDlg* pMainDlg, CStatusWriter* pStatusDlg)
{
	m_pMainDlg = pMainDlg;
	m_pStatusDlg = pStatusDlg;
	
	// Recall previous settings from the registry.
	CFreeScanApp* pApp = (CFreeScanApp*) AfxGetApp();
	m_bCentigrade = pApp->GetProfileInt(_T("Supervisor"), _T("Centigrade"), TRUE);
	m_bMiles = pApp->GetProfileInt(_T("Supervisor"), _T("Miles"), TRUE);

	m_pCom = NULL;
	m_dwBytesSent = 0;
	m_dwBytesReceived = 0;
	m_bHasRun = FALSE;
	m_iModel = 0;

	m_csCSVLogFile = pApp->GetProfileString("Supervisor", "CSV Log Filename", "");
	m_dwCSVRecord = 0; // Initialise the CSV record number

	// Recall previous settings from the registry.
	m_bInteract = pApp->GetProfileInt("Supervisor", "Interact", FALSE);

	// Null protocol pointers
	m_pProtocol = NULL;

	m_pEcuData = new CEcuData();
}

CSupervisor::~CSupervisor()
{
	if (m_file.m_hFile != CFile::hFileNull)
	{ // close our log file if it's open
		m_file.Flush();
		m_file.Close(); // close the logging file when we exit.
	}

	// Save our settings to the registry
	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileInt(_T("Supervisor"), _T("Centigrade"), m_bCentigrade);
	pApp->WriteProfileInt(_T("Supervisor"), _T("Miles"), m_bMiles);

	pApp->WriteProfileString("Supervisor", "CSV Log Filename", m_csCSVLogFile);

	pApp->WriteProfileInt("Supervisor", "Interact", m_bInteract);

	// Mop-up any allocated protocol classes
	Deallocate();
}

// Deallocates the models we don't want to monitor.
void CSupervisor::Deallocate(void) {

	if (m_pProtocol != NULL && m_pProtocol->GetTimeoutForPingDuringInteract() > 0 && ::IsWindow(m_hWnd)) {
		KillTimer(WM_TIMER_EVT1);
	}

	// Delete the active protocol
	if (m_pProtocol != NULL)
		delete m_pProtocol;
	m_pProtocol = NULL;

	if (m_pCom != NULL)
		delete m_pCom;
	m_pCom = NULL;

	if (m_pEcuData != NULL) {
		delete m_pEcuData;
	}
	m_pEcuData = NULL;
}

// Initialises the Supervisor
void CSupervisor::Init(int iModel)
{
	m_iModel = iModel;

	WriteStatus("Initialising selected protocol...");

	// Allocate and initialise the chosen ECU protocol.
	Stop();
	Deallocate();
	m_pEcuData = new CEcuData();

	// ****** This pointer should be the protocol window ******
	// The protocol window also needs access to the com port.
	
	// Create a new serial port object
	m_pCom = new CSerialPort;

	// These indexes must match the order of the text in the Vehicle Select ComboBox.
	switch(iModel)
	{
		case 0: // ELM327 OBD-II Interface.
			m_pProtocol = new CELM327Protocol(m_pStatusDlg, this, m_bInteract);

			break;

		case 1: // Lotus Esprit 4 Cylinder.
			m_pProtocol = new CEspritProtocol(m_pStatusDlg, this, m_bInteract);

			break;

		case 2: // Lotus Elan M100.
			m_pProtocol = new CElanProtocol(m_pStatusDlg, this, m_bInteract);
			break;

		case 3: // Lotus Carlton.
			m_pProtocol = new CLotusCarltonProtocol(m_pStatusDlg, this, m_bInteract);
			break;

		case 4:	// GM 1994 Camaro Z28.
			m_pProtocol = new CGM1994CamaroZ28Protocol(m_pStatusDlg, this, m_bInteract);
			break;
	
		case 5:	// GM 1993 Camaro Z28.
			m_pProtocol = new CGM1993CamaroZ28Protocol(m_pStatusDlg, this, m_bInteract);
			break;
	
		case 6:	// GM 1992 Pontiac.
			m_pProtocol = new CGM1992PontiacProtocol(m_pStatusDlg, this, m_bInteract);
			break;
	
		case 7:	// GM 1989 Corvette.
			m_pProtocol = new CGM1989CorvetteProtocol(m_pStatusDlg, this, m_bInteract);
			break;
	
		case 8:	// GM A140
			m_pProtocol = new CGMA140Protocol(m_pStatusDlg, this, m_bInteract);
			break;
	
		case 9:	// GM A143
			m_pProtocol = new CGMA143Protocol(m_pStatusDlg, this, m_bInteract);
			break;
	
		case 10:	// GM A160
			m_pProtocol = new CGMA160Protocol(m_pStatusDlg, this, m_bInteract);
			break;

		default:
			m_pProtocol = NULL;
			break;
	}

	if (m_pProtocol != NULL) {
		m_pProtocol->InitializeSupportedValues(m_pEcuData);
		m_pProtocol->Init(m_pCom);
	}

}

// The supervisor is a hidden window. This is to enable it to receive
// messages from itself and the serial port class.
BOOL CSupervisor::CreateProtocolWnd(CWnd* pParentWnd)
{
	// TODO: Add your specialized code here and/or call the base class
	DWORD	dwStyle = WS_BORDER | WS_CAPTION | WS_CHILD;
	RECT	rect;
	UINT nID = 67; // It's my house number!

	rect.top = 0;
	rect.bottom = 50;
	rect.left = 0;
	rect.right = 50;

	return Create(NULL, "ECUCommunications Supervisor", dwStyle, rect, pParentWnd, nID, NULL);
}

BEGIN_MESSAGE_MAP(CSupervisor, CWnd)
	//{{AFX_MSG_MAP(CGM1992PontiacProtocol)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_MESSAGE(WM_COMM_RXCHAR, OnCharReceived)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

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

// Handle the message from the serial port class.
LONG CSupervisor::OnCharReceived(WPARAM ch, LPARAM BytesRead) {
	if (m_pProtocol != NULL && BytesRead > 0) {
		m_dwBytesReceived += BytesRead;

		CEcuData* const ecuData = new CEcuData(*m_pEcuData);
		const BOOL modifiedEcuData = m_pProtocol->OnCharsReceived((const unsigned char* const) ch, (const DWORD) BytesRead, ecuData);
		
		if (modifiedEcuData == TRUE) {
			m_pEcuData->copyFields(ecuData); // this is and should remain the only place where m_pEcuData is updated
		}
		delete ecuData;

		if (modifiedEcuData == TRUE) {
			ConvertMiles();
			ConvertDegrees();
			WriteCSV(false);
			m_pMainDlg->Update(m_pEcuData);
		}
	}
	return 0;
}

//WriteCSV(..) Writes CSV data to our log file.
//If bTitle is true, the first title line is written, otherwise the data is written.
void CSupervisor::WriteCSV(BOOL bTitle) {
	if (m_file.m_pStream == NULL || (!bTitle && m_dwCSVRecord == 0)) {// i.e. no file open, or writing line before title
		return;
	}

	CString	csBuf;
	DWORD newCSVRecordNumber;
	if (bTitle) {
		csBuf = _T("PC-Timestamp,Sample");
		csBuf += m_pEcuData->generateCsvLine(TRUE);
		newCSVRecordNumber = 1;
	}
	else {
		SYSTEMTIME localTime;

		GetLocalTime(&localTime);

		csBuf.Format(
			_T("%04d-%02d-%02d %02d:%02d:%02d.%03d,%ld"),
			localTime.wYear, localTime.wMonth, localTime.wDay, localTime.wHour, localTime.wMinute, localTime.wSecond, localTime.wMilliseconds,
			m_dwCSVRecord
		);
		csBuf += m_pEcuData->generateCsvLine(FALSE);

		newCSVRecordNumber = m_dwCSVRecord + 1;
	}
	csBuf += _T("\n"); // Line Feed because we're logging to disk
	m_file.WriteString(csBuf);
	m_dwCSVRecord = newCSVRecordNumber;
}

// Converts temps to degF
void CSupervisor::ConvertDegrees(void) {
	if (CEcuData::isValid(m_pEcuData->m_fStartWaterTemp)) {
		m_pEcuData->m_fStartWaterTemp_inF = (float)((m_pEcuData->m_fStartWaterTemp * (float)1.8) + (float)32.0);
	}
	else if (CEcuData::isSupported(m_pEcuData->m_fStartWaterTemp)) {
		m_pEcuData->m_fStartWaterTemp_inF = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	}

	if (CEcuData::isValid(m_pEcuData->m_fWaterTemp)) {
		m_pEcuData->m_fWaterTemp_inF = (float)((m_pEcuData->m_fWaterTemp * (float)1.8) + (float)32.0);
	}
	else if (CEcuData::isSupported(m_pEcuData->m_fWaterTemp)) {
		m_pEcuData->m_fWaterTemp_inF = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	}

	if (CEcuData::isValid(m_pEcuData->m_fOilTemp)) {
		m_pEcuData->m_fOilTemp_inF = (float)((m_pEcuData->m_fOilTemp * (float)1.8) + (float)32.0);
	}
	else if (CEcuData::isSupported(m_pEcuData->m_fOilTemp)) {
		m_pEcuData->m_fOilTemp_inF = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	}

	if (CEcuData::isValid(m_pEcuData->m_fMATTemp)) {
		m_pEcuData->m_fMATTemp_inF = (float)((m_pEcuData->m_fMATTemp * (float)1.8) + (float)32.0);
	}
	else if (CEcuData::isSupported(m_pEcuData->m_fMATTemp)) {
		m_pEcuData->m_fMATTemp_inF = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	}
}

// Converts miles to kilometers
void CSupervisor::ConvertMiles(void) {
	if (CEcuData::isValid(m_pEcuData->m_iMPH)) {
		m_pEcuData->m_iMPH_inKPH = (int)((float)m_pEcuData->m_iMPH * (float)1.609344f);
	}
	else if (CEcuData::isSupported(m_pEcuData->m_iMPH)) {
		m_pEcuData->m_iMPH_inKPH = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	}
}

// Writes a line of ASCII to the spy window
void CSupervisor::WriteStatus(const CString csText) {
	CString text = "Supervisor: " + csText;
	m_pStatusDlg->WriteStatus(text);
}

void CSupervisor::WriteStatusLogged(const CString csText) {
	m_pStatusDlg->WriteStatusTimeLogged(csText);
}

// Returns the current com port
void CSupervisor::SetCurrentPort(UINT nPort)
{
	ASSERT(m_pCom != NULL);
	ASSERT(nPort != NULL);
	Deallocate();
	m_pCom->SetPort(nPort);
	Init(m_iModel);
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
	ASSERT(m_pProtocol != NULL);
	// This sets the com port up
	if (m_bHasRun == FALSE) {
		if (!CreateProtocolWnd(m_pMainDlg)) {
			WriteStatus(_T("Failed to create the protocol window"));
			return FALSE;
		}

		if (!m_pCom->StartMonitoring()) {
			WriteStatus(_T("Failed to start the Com Port"));
			return FALSE;
		}
		else {
			WriteStatus(_T("Com Port started"));
			m_bHasRun = TRUE;
			m_pProtocol->Reset();
			m_pProtocol->InitializeSupportedValues(m_pEcuData);

			this->Interact(m_bInteract); // reset interact and start timer, if required

			return TRUE;
		}
	}
	else {
		if (!m_pCom->RestartMonitoring()) {
			WriteStatus(_T("Failed to restart the Com Port"));
			return FALSE;
		}
		else {
			WriteStatus(_T("Com Port restarted"));
			m_pProtocol->Reset();
			m_pProtocol->InitializeSupportedValues(m_pEcuData);

			this->Interact(m_bInteract); // reset interact and start timer, if required
		}
		return TRUE;
	}
}

// Stops the Comport.
BOOL CSupervisor::Stop(void)
{
	if (m_pCom == NULL)
		return TRUE;

	// This sets the com port up
	if (!m_pCom->StopMonitoring()) {
		WriteStatus(_T("Failed to stop the Com Port"));
		return FALSE;
	}
	else
		WriteStatus(_T("Com Port stopped"));
	return TRUE;
}

// Ensures the Supervisor is exited cleanly
BOOL CSupervisor::ShutDown(void)
{
//	DestroyWindow();
	return TRUE;
}

// Starts or stops csv logging to file
BOOL CSupervisor::StartCSVLog(BOOL bStart) {
	CString csBuf = "";

	if (!bStart) { // we want to close the logging file
		if (m_file.m_hFile != CFile::hFileNull)	{
			WriteStatusLogged("CSV Log file has been stopped");
			m_dwCSVRecord = 0;
			m_file.Close(); // close the logging file when we exit.
		}
		else {
			WriteStatusLogged("CSV Log file is already closed");
		}

		return FALSE;
	}

	// We now must want to log to a file

	// Construct our File Dialog
	CFileDialog		Dialog(FALSE, "csv",
		m_csCSVLogFile,
		OFN_HIDEREADONLY | OFN_LONGNAMES | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT,
		"log Files (*.csv)|*.csv|All Files (*.*)|*.*||", NULL);

	// Change the title
	Dialog.m_ofn.lpstrTitle = "Create/Open CSV Log File";

	// Display the dialog box
	if (Dialog.DoModal() == IDOK) {
		m_csCSVLogFile = Dialog.GetPathName();

		m_dwCSVRecord = 0;
		if (!m_file.Open(m_csCSVLogFile, CFile::modeCreate | CFile::modeReadWrite | CFile::typeText)) {
			csBuf.Format("Cannot open %s", m_csCSVLogFile.GetString());
			WriteStatus(csBuf);
			AfxMessageBox(csBuf, MB_OK | MB_ICONSTOP);
			return FALSE;
		}

		WriteStatusLogged("CSV Log file has been opened");
		WriteCSV(TRUE);
	}
	else { // User pressed cancel
		WriteStatus("User cancelled CSV log file");
	}

	if (m_file.m_hFile != NULL) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// Requests whether FreeScan talks to the ECU or not
void CSupervisor::Interact(BOOL bInteract) {
	if (bInteract) {
		m_bInteract = TRUE;
		if (m_pProtocol != NULL && m_pProtocol->GetTimeoutForPingDuringInteract() > 0 && ::IsWindow(m_hWnd)) {
			SetTimer(WM_TIMER_EVT1, m_pProtocol->GetTimeoutForPingDuringInteract(), NULL);
			// This is new. Provides a way of continually asking
			// data from an ECU that doesn't have idle chatter.
		}
	}
	else {
		m_bInteract = FALSE;
		if (::IsWindow(m_hWnd)) {
			KillTimer(WM_TIMER_EVT1);
		}
	}
	if (m_pProtocol != NULL) {
		m_pProtocol->SetInteract(bInteract);
	}
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
BOOL CSupervisor::GetInteract(void) {
	return m_bInteract;
}

// Returns the current ECU Mode
DWORD CSupervisor::GetCurrentMode(void) {
	if (m_pProtocol != NULL) {
		return m_pProtocol->GetCurrentMode();
	}
	return 0;
}

// This switches the mode number that is sent to the ECU. It changes the
// behaviour of SendNextCommand(..).
void CSupervisor::ECUMode(DWORD dwMode, const unsigned char data) {
	if (m_pProtocol != NULL) {
		return m_pProtocol->SetECUMode(dwMode, data);
	}
}

// Forces the protocol to send a shut-up command to the ECU.
void CSupervisor::ForceDataFromECU(void) {
	if (m_pProtocol != NULL) {
		return m_pProtocol->ForceDataFromECU();
	}
}

const CEcuData *const CSupervisor::GetEcuData(void) {
	return m_pEcuData;
}

DWORD CSupervisor::GetReceivedBytes() {
	return m_dwBytesReceived;
}

DWORD CSupervisor::GetSentBytes() {
	return m_dwBytesSent;
}

void CSupervisor::IncreaseSentBytes(const DWORD additionalBytesSent) {
	m_dwBytesSent += additionalBytesSent;
}

void CSupervisor::OnTimer(UINT nIDEvent) {
	WriteStatus("OnTimer");

	if (m_pProtocol != NULL) {
		m_pProtocol->OnTimer();
	}

	CWnd::OnTimer(nIDEvent);
}

// Sends a packet to the current parser for testing
void CSupervisor::Test() 
{
	if (m_pProtocol != NULL && dynamic_cast<CEspritProtocol*>(m_pProtocol) != nullptr)
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

		m_pProtocol->OnCharsReceived(buffer, 68, m_pEcuData);
	}
	else if (m_pProtocol != NULL && dynamic_cast<CGM1989CorvetteProtocol*>(m_pProtocol) != nullptr)
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

		m_pProtocol->OnCharsReceived(buffer, 67, m_pEcuData);
	}
	else if (m_pProtocol != NULL && dynamic_cast<CGM1994CamaroZ28Protocol*>(m_pProtocol) != nullptr)
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

		m_pProtocol->OnCharsReceived(buffer, 64, m_pEcuData);
	}
	else if (m_pProtocol != NULL && dynamic_cast<CElanProtocol*>(m_pProtocol) != nullptr)
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

		m_pProtocol->OnCharsReceived(buffer, 67, m_pEcuData);
	}
	else if (m_pProtocol != NULL && dynamic_cast<CGMA143Protocol*>(m_pProtocol) != nullptr)
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

		m_pProtocol->OnCharsReceived(buffer, 71, m_pEcuData);
	}
	else if (m_pProtocol != NULL && dynamic_cast<CGMA160Protocol*>(m_pProtocol) != nullptr)
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
		m_pProtocol->OnCharsReceived(buffer, 67, m_pEcuData);
	}
	else
	{
		AfxMessageBox("No Test Bytes for this protocol");
	}
}
