// DetailDlg.cpp : implementation file
//
// (c) 1996-99 Andy Whittaker, Chester, England. 
// mail@andywhittaker.com
//

#include "stdafx.h"
#include "FreeScan.h"
#include "MainDlg.h"
#include "Supervisor.h"
#include "StatusDlg.h"
#include "EnumSer.h" // for EnumerateSerialPorts(...)

#include "DetailDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDetailDlg property page

IMPLEMENT_DYNCREATE(CDetailDlg, CTTPropertyPage)

CDetailDlg::CDetailDlg() : CTTPropertyPage(CDetailDlg::IDD)
{
	//{{AFX_DATA_INIT(CDetailDlg)
	//}}AFX_DATA_INIT

	Enumerate(); // Build a list of available serial ports
	m_pMainDlg = NULL;
	m_bCSVFirstTime = TRUE;
	m_bLogFirstTime = TRUE;

	// Recall previous settings from the registry.
	CWinApp* pApp = AfxGetApp();
	m_iModel = pApp->GetProfileInt("Supervisor", "Model", 0);
}

CDetailDlg::~CDetailDlg()
{
	// Save our settings to the registry
	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileInt("Supervisor", "Model", m_iModel);
}

void CDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CTTPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDetailDlg)
	DDX_Control(pDX, IDC_DELAY, m_WriteDelay);
	DDX_Control(pDX, IDC_MPH, m_mph);
	DDX_Control(pDX, IDC_KPH, m_kph);
	DDX_Control(pDX, IDC_DEGF, m_DegF);
	DDX_Control(pDX, IDC_DEGC, m_DegC);
	DDX_Control(pDX, IDC_FORCE, m_Force);
	DDX_Control(pDX, IDC_COMMENTS, m_Comments);
	DDX_Control(pDX, IDC_MODEL, m_Model);
	DDX_Control(pDX, IDC_STARTLOG, m_StartLog);
	DDX_Control(pDX, IDC_CSV, m_CSV);
	DDX_Control(pDX, IDC_RECEIVED, m_Received);
	DDX_Control(pDX, IDC_SENT, m_Sent);
	DDX_Control(pDX, IDC_STOP, m_Stop);
	DDX_Control(pDX, IDC_START, m_Start);
	DDX_Control(pDX, IDC_COMSELECT, m_ComSelect);
	DDX_Control(pDX, IDC_HIDE, m_Hide);
	DDX_Control(pDX, IDC_LISTEN, m_Listen);
	DDX_Control(pDX, IDC_INTERACT, m_Interact);
	//}}AFX_DATA_MAP

	Refresh();
}

// Write to the Status Dialog
void CDetailDlg::WriteStatus(CString csText)
{
	m_pMainDlg->WriteStatus(csText);
}

void CDetailDlg::WriteASCII(unsigned char * buffer, int ilength)
{
	m_pMainDlg->WriteASCII(buffer, ilength);
}

BEGIN_MESSAGE_MAP(CDetailDlg, CTTPropertyPage)
	//{{AFX_MSG_MAP(CDetailDlg)
	ON_BN_CLICKED(IDC_STARTLOG, OnStartlog)
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_BN_CLICKED(IDC_HIDE, OnHide)
	ON_CBN_SELENDOK(IDC_COMSELECT, OnSelendokComselect)
	ON_CBN_KILLFOCUS(IDC_COMSELECT, OnKillfocusComselect)
	ON_BN_CLICKED(IDC_LISTEN, OnListen)
	ON_BN_CLICKED(IDC_INTERACT, OnInteract)
	ON_BN_CLICKED(IDC_CSV, OnCsv)
	ON_BN_CLICKED(IDC_CSVOPTIONS, OnCsvoptions)
	ON_CBN_SELENDOK(IDC_MODEL, OnSelendokModel)
	ON_BN_CLICKED(IDC_FORCE, OnForce)
	ON_BN_CLICKED(IDC_DEGC, OnDegC)
	ON_BN_CLICKED(IDC_DEGF, OnDegF)
	ON_BN_CLICKED(IDC_KPH, OnKph)
	ON_BN_CLICKED(IDC_MPH, OnMph)
	ON_CBN_SELENDOK(IDC_DELAY, OnSelendokDelay)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// Returns a pointer to the Supervisor
CSupervisor* CDetailDlg::GetSupervisor(void)
{
	return m_pMainDlg->m_pSupervisor;
}

// Returns a pointer to the Supervisor
CSupervisor* CDetailDlg::GetData(void)
{
	return m_pMainDlg->m_pSupervisor;
}

// Returns if the ECU is interactive
BOOL CDetailDlg::GetInteract(void)
{
	return GetSupervisor()->GetInteract();
}

// Returns if FreeScan is in Metric
BOOL CDetailDlg::GetCentigrade(void)
{
	return GetSupervisor()->GetCentigrade();
}

// Returns if FreeScan is in Metric
BOOL CDetailDlg::GetMiles(void)
{
	return GetSupervisor()->GetMiles();
}

// Updates all of our controls
void CDetailDlg::Refresh(void)
{
	CString buf;
	
	// Update bytes sent and received
	buf.Format("%d", GetSupervisor()->m_dwBytesReceived);
	m_Received.SetWindowText(buf);
	buf.Format("%d", GetSupervisor()->m_dwBytesSent);
	m_Sent.SetWindowText(buf);

	m_Comments.ResetContent();	// Clear protocol comments

	// Display the protocol's comments
	CString csTemp;
	csTemp = GetSupervisor()->m_csProtocolComment;
	int		iIndex=0;

	// Format the text in the ListBox.
	// This routine correctly handles CR/LF.
	while (iIndex != -1)
	{
		iIndex = csTemp.Find('\n');
		m_Comments.AddString(csTemp.SpanExcluding("\n"));
		csTemp.Delete(0, iIndex+1);
	}
}

// Initialises bits and pieces that need the Supervisor
void CDetailDlg::Init(void)
{
	// Restore the model here for our dialog
	CString		csText;
	m_Model.SetCurSel(m_iModel);
	m_Model.GetLBText(m_iModel, csText);
	csText = "Model " + csText + " selected.";
	WriteStatus(csText);

	GetSupervisor()->Init(m_pMainDlg, m_iModel);
	
	// Set the write delay
	CString buf;
	buf.Format("%d", (DWORD)GetSupervisor()->GetWriteDelay());
	m_WriteDelay.SetWindowText(buf);

	// Display the current com port in the ComboBox
	CString csTemp;
	csTemp.Format("COM%d", GetSupervisor()->GetCurrentPort());
	m_ComSelect.SetWindowText(csTemp);

	Refresh();

	// Set up interact/listen buttons
	if (GetInteract())
		OnInteract();
	else
		OnListen();

	// Set up DegC/DegF buttons
	if (GetCentigrade())
		OnDegC();
	else
		OnDegF();

	// Set up mph/kph buttons
	if (GetMiles())
		OnMph();
	else
		OnKph();
}

/////////////////////////////////////////////////////////////////////////////
// CDetailDlg message handlers

void CDetailDlg::OnStartlog()
{
	if (m_bLogFirstTime)
	{
		m_bLogFirstTime = FALSE;
		if (!(m_pMainDlg->StartLog(TRUE)))
			m_bLogFirstTime = TRUE;// Call may have failed
		else
			m_StartLog.SetWindowText(_T("Stop Logging"));
	}
	else
	{
		m_bLogFirstTime = TRUE;
		m_StartLog.SetWindowText(_T("Log ECU Coms to Disk"));
		m_pMainDlg->StartLog(FALSE);
	}
}

void CDetailDlg::OnStart()
{
	// ** for tesing only **
//	m_pMainDlg->m_pSupervisor->Test(); // test the parser
//	return;

	m_pMainDlg->StartComs();
	m_Stop.EnableWindow(TRUE); // enable the stop monitoring button
	m_Start.SetWindowText(_T("Restart")); // Start monitoring button's text
	m_Start.EnableWindow(FALSE); // disable the start monitoring button
	m_ComSelect.EnableWindow(FALSE); // Disable COM Port changes
}

void CDetailDlg::OnStop()
{
	m_Start.EnableWindow(TRUE); // enable the start monitoring button
	m_Stop.EnableWindow(FALSE); // disable the stop monitoring button
	m_ComSelect.EnableWindow(TRUE); // Enable COM Port changes
	m_pMainDlg->StopComs();
}

// Hides or unhides the status messages window when asked to
void CDetailDlg::OnHide() 
{// CButton
	m_pMainDlg->m_pStatusDlg->Hide(m_Hide.GetCheck());
}

BOOL CDetailDlg::OnInitDialog()
{
	CTTPropertyPage::OnInitDialog();

	// Add dialog items that want ToolTip text
	m_toolTip.AddTool( GetDlgItem(IDC_STARTLOG), IDC_STARTLOG);
	m_toolTip.AddTool( GetDlgItem(IDC_CSV),IDC_CSV);
	m_toolTip.AddTool( GetDlgItem(IDC_CSVOPTIONS),IDC_CSVOPTIONS);
	m_toolTip.AddTool( GetDlgItem(IDC_START), IDC_START);
	m_toolTip.AddTool( GetDlgItem(IDC_STOP), IDC_STOP);
	m_toolTip.AddTool( GetDlgItem(IDC_HIDE),IDC_HIDE);
	m_toolTip.AddTool( GetDlgItem(IDC_LISTEN),IDC_LISTEN);
	m_toolTip.AddTool( GetDlgItem(IDC_INTERACT),IDC_INTERACT);
	m_toolTip.AddTool( GetDlgItem(IDC_DELAY),IDC_DELAY);
	m_toolTip.AddTool( GetDlgItem(IDC_SENT),IDC_SENT);
	m_toolTip.AddTool( GetDlgItem(IDC_RECEIVED),IDC_RECEIVED);
	m_toolTip.AddTool( GetDlgItem(IDC_FORCE),IDC_FORCE);

	// Set the button state to the state of the messages window
	m_Hide.SetCheck(m_pMainDlg->m_pStatusDlg->HideStatus());

	m_Stop.EnableWindow(FALSE); // disable the stop monitoring button

	m_ComSelect.ResetContent();
	// Fill up the COMBOBOX with our serial ports
	for (UINT i=0; i<m_cuPorts.size(); i++)
	{
//		CComboBox
		CString csTemp;
		csTemp.Format("COM%d", m_cuPorts[i]);
		m_ComSelect.AddString(csTemp);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// Enumerate the serial ports available on this computer
void CDetailDlg::Enumerate(void)
{
	TRACE(_T("Enumerating serial ports in this Machine. "));
	
	m_EnumSerial.UsingCreateFile(m_cuPorts);

	int i=m_cuPorts.size();

	TRACE(_T("Done, %d serial ports found\n"), i);
}

// When the "Com Port Select" combobox is closed, handle the new com port.
void CDetailDlg::OnSelendokComselect()
{//CComboBox
	CString		csText;
	m_ComSelect.GetLBText(m_ComSelect.GetCurSel(),csText);
	csText = "Selected " + csText;
	WriteStatus(csText);
	
	GetSupervisor()->SetCurrentPort(m_cuPorts[m_ComSelect.GetCurSel()]);
}

void CDetailDlg::OnKillfocusComselect()
{
	// Display the current com port in the ComboBox
	CString csTemp;
	csTemp.Format("COM%d", GetSupervisor()->GetCurrentPort());
	m_ComSelect.SetWindowText(csTemp);
}

// Here is where we set the model of car we've got.
// The model number is the index of the resources in the ComboBox.
// The order of these must match what the supervisor is expecting.
void CDetailDlg::OnSelendokModel()
{
	m_iModel = m_Model.GetCurSel();
	m_Start.EnableWindow(TRUE); // enable the start monitoring button
	m_Stop.EnableWindow(FALSE); // disable the stop monitoring button
	Init();
}

// Listen radio button
void CDetailDlg::OnListen()
{
	m_Listen.SetCheck(TRUE);
	m_Interact.SetCheck(FALSE);
	GetSupervisor()->Interact(FALSE);
	m_Force.EnableWindow(FALSE);
}

// Interact radio button
void CDetailDlg::OnInteract()
{
	m_Listen.SetCheck(FALSE);
	m_Interact.SetCheck(TRUE);
	GetSupervisor()->Interact(TRUE);
	m_Force.EnableWindow(TRUE);
}

void CDetailDlg::OnCsv()
{
	if (m_bCSVFirstTime)
	{
		m_bCSVFirstTime = FALSE;
		if (!(m_pMainDlg->StartCSVLog(TRUE)))
			m_bCSVFirstTime = TRUE;// Call may have failed
		else
			m_CSV.SetWindowText("Stop");
	}
	else
	{
		m_bCSVFirstTime = TRUE;
		m_CSV.SetWindowText("Start");
		m_pMainDlg->StartCSVLog(FALSE);
	}
}

void CDetailDlg::OnCsvoptions() 
{
	AfxMessageBox("Sorry, not yet implemented");
}

void CDetailDlg::OnForce() 
{
	GetSupervisor()->ForceShutUp();
}

void CDetailDlg::OnDegC() 
{
	m_DegC.SetCheck(TRUE);
	m_DegF.SetCheck(FALSE);
	GetSupervisor()->Centigrade(TRUE);
	WriteStatus("Temperatures will be in degrees C");
}

void CDetailDlg::OnDegF() 
{
	m_DegC.SetCheck(FALSE);
	m_DegF.SetCheck(TRUE);
	GetSupervisor()->Centigrade(FALSE);
	WriteStatus("Temperatures will be in degrees F");
	AfxMessageBox("Please Note: this version only displays Centrigrade and Miles correctly in the dashboard");
}

void CDetailDlg::OnKph() 
{
	m_kph.SetCheck(TRUE);
	m_mph.SetCheck(FALSE);
	GetSupervisor()->Miles(FALSE);
	WriteStatus("Speeds will be in kph");
	AfxMessageBox("Please Note: this version only displays Centrigrade and Miles correctly in the dashboard");
}

void CDetailDlg::OnMph() 
{
	m_kph.SetCheck(FALSE);
	m_mph.SetCheck(TRUE);
	GetSupervisor()->Miles(TRUE);
	WriteStatus("Speeds will be in mph");
}

// Handles the delay combobox
void CDetailDlg::OnSelendokDelay() 
{
	CStringEx	csText;
	m_WriteDelay.GetLBText(m_WriteDelay.GetCurSel(),csText);
	GetSupervisor()->SetWriteDelay(csText.GetInt());
	csText = "Write Delay Set to " + csText + "mS";
	WriteStatus(csText);
}
