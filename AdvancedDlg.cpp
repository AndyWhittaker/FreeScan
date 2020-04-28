// AdvancedDlg.cpp : implementation file
//
// (c) 1996-99 Andy Whittaker, Chester, England. 
// mail@andywhittaker.com


#include "AdvancedDlg.h"

#include "DesIdleDlg.h" // Desired Idle Set Dialog Box

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdvancedDlg property page

IMPLEMENT_DYNCREATE(CAdvancedDlg, CTTPropertyPage)

CAdvancedDlg::CAdvancedDlg() : CTTPropertyPage(CAdvancedDlg::IDD) {
	//{{AFX_DATA_INIT(CAdvancedDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pSupervisor = NULL;
}

CAdvancedDlg::~CAdvancedDlg() {
}

void CAdvancedDlg::DoDataExchange(CDataExchange* pDX) {
	CTTPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdvancedDlg)
	DDX_Control(pDX, IDC_SETDESIDLE, m_SetDesIdle);
	DDX_Control(pDX, IDC_RESETIAC, m_ResetIAC);
	DDX_Control(pDX, IDC_RESETBLM, m_ResetBLM);
	DDX_Control(pDX, IDC_DESIDLE, m_DesiredIdle);
	//}}AFX_DATA_MAP

	//Updates the dialog.
	if (m_pSupervisor != NULL) {
		Refresh(m_pSupervisor->GetEcuData());
	}
}


// Updates all of our controls
void CAdvancedDlg::Refresh(const CEcuData* const ecuData) {
	CString buf;
	DWORD	dwCurrentMode = (m_pSupervisor != NULL)? m_pSupervisor->GetCurrentMode() : 0;

	if (dwCurrentMode != 1 || !CEcuData::isValid(ecuData->m_iDesiredIdle)) {
		buf.Format("N/A");
	}
	else {
		buf.Format("%d", ecuData->m_iDesiredIdle);
	}
	m_DesiredIdle.SetWindowText(buf);

	// Hide the buttons that don't work when not interactive
	if (m_pSupervisor != NULL && m_pSupervisor->GetInteract()) {
		m_SetDesIdle.EnableWindow(TRUE);
		m_ResetIAC.EnableWindow(TRUE);
		m_ResetBLM.EnableWindow(TRUE);
	}
	else {
		m_SetDesIdle.EnableWindow(FALSE);
		m_ResetIAC.EnableWindow(FALSE);
		m_ResetBLM.EnableWindow(FALSE);
	}

}

void CAdvancedDlg::RegisterSupervisor(CSupervisorInterface* const pSupervisor) {
	m_pSupervisor = pSupervisor;
}

BEGIN_MESSAGE_MAP(CAdvancedDlg, CTTPropertyPage)
	//{{AFX_MSG_MAP(CAdvancedDlg)
	ON_BN_CLICKED(IDC_SETDESIDLE, OnSetdesidle)
	ON_BN_CLICKED(IDC_RESETBLM, OnResetBLM)
	ON_BN_CLICKED(IDC_RESETIAC, OnResetIAC)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdvancedDlg message handlers

BOOL CAdvancedDlg::OnInitDialog() {
	CTTPropertyPage::OnInitDialog();
	
	// Add dialog items that want ToolTip text
//	m_toolTip.AddTool( GetDlgItem(IDC_EPROMID), IDC_EPROMID);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAdvancedDlg::OnSetdesidle() {
	CDesIdleDlg		dlg;

	dlg.m_Value = 850;;
	if (CEcuData::isValid(m_pSupervisor->GetEcuData()->m_iDesiredIdle)) {
		dlg.m_Value = m_pSupervisor->GetEcuData()->m_iDesiredIdle;
	}
	
	if (dlg.DoModal() == IDOK) {
		// Sends the ECU command to set the desired idle
		unsigned char uc;
		uc = (unsigned char) ((dlg.m_Value / 25) * 2);
		m_pSupervisor->ECUMode(ECU_SET_DES_IDLE, uc);
	}
}

void CAdvancedDlg::OnResetBLM() {
	m_pSupervisor->ECUMode(ECU_CLEAR_BLM);
}

void CAdvancedDlg::OnResetIAC() {
	m_pSupervisor->ECUMode(ECU_RESET_IAC);
}
