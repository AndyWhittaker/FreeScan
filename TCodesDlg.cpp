// TCodesDlg.cpp : implementation file
//
// (c) 1996-99 Andy Whittaker, Chester, England. 
// mail@andywhittaker.com

#include "TCodesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTCodesDlg property page

IMPLEMENT_DYNCREATE(CTCodesDlg, CPropertyPage)

CTCodesDlg::CTCodesDlg() : CPropertyPage(CTCodesDlg::IDD)
{
	//{{AFX_DATA_INIT(CTCodesDlg)
	//}}AFX_DATA_INIT
	m_pSupervisor = NULL;
}

CTCodesDlg::~CTCodesDlg()
{
}

void CTCodesDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTCodesDlg)
	DDX_Control(pDX, IDC_RESETDTC, m_ResetDTC);
	DDX_Control(pDX, IDC_TLIST, m_TList);
	//}}AFX_DATA_MAP

	//Updates the dialog.
	if (m_pSupervisor != NULL) {
		Refresh(m_pSupervisor->GetEcuData());
	}
}

// Populates a ListBox with a CString
void CTCodesDlg::FillListBox(CListBox& lbT, const CString& csT)
{
	lbT.ResetContent();
	// parse the CString Buffer
	CString csTemp;
	csTemp = csT;
	int		iIndex=0;

	while (iIndex != -1) {
		iIndex = csTemp.Find('\n');
		lbT.AddString(csTemp.SpanExcluding("\n"));
		csTemp.Delete(0, iIndex+1);
	}
}

BEGIN_MESSAGE_MAP(CTCodesDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CTCodesDlg)
	ON_BN_CLICKED(IDC_RESETDTC, OnResetdtc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// Updates all of our controls
void CTCodesDlg::Refresh(const CEcuData* const ecuData) {
	//CListBox
	m_TList.ResetContent();

	if (ecuData->m_csDTC.GetLength() > 0) {
		FillListBox(m_TList, ecuData->m_csDTC);
	}
	else {
		FillListBox(m_TList, "DTCs are not supported by this protocol.");
	}

	// Hide the buttons that don't work when not interactive
	if (m_pSupervisor->GetInteract())
	{
		m_ResetDTC.EnableWindow(TRUE);
	}
	else
	{
		m_ResetDTC.EnableWindow(FALSE);
	}
}

void CTCodesDlg::RegisterSupervisor(CSupervisorInterface* const pSupervisor) {
	m_pSupervisor = pSupervisor;
}

/////////////////////////////////////////////////////////////////////////////
// CTCodesDlg message handlers

void CTCodesDlg::OnResetdtc() 
{
	// Sends the ECU command to reset the fault codes
	m_pSupervisor->ECUMode(ECU_CLEAR_DTCS);
}
