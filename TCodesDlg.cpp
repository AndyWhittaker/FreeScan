// TCodesDlg.cpp : implementation file
//
// (c) 1996-99 Andy Whittaker, Chester, England. 
// mail@andywhittaker.com

#include "stdafx.h"
#include "FreeScan.h"
#include "MainDlg.h"
#include "Supervisor.h"

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
	m_pMainDlg = NULL;
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
	Refresh();
}

// Returns a pointer to the Supervisor
CSupervisor* CTCodesDlg::GetSupervisor(void)
{
	return m_pMainDlg->m_pSupervisor;
}

// Returns a pointer to the Supervisor
CSupervisor* CTCodesDlg::GetData(void)
{
	return m_pMainDlg->m_pSupervisor;
}

// Returns if the ECU is interactive
BOOL CTCodesDlg::GetInteract(void)
{
	return GetSupervisor()->GetInteract();
}

// Populates a ListBox with a CString
void CTCodesDlg::FillListBox(CListBox& lbT, CString& csT)
{
	lbT.ResetContent();
	// parse the CString Buffer
	CString csTemp;
	csTemp = csT;
	int		iIndex=0;

	while (iIndex != -1)
	{
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
void CTCodesDlg::Refresh(void)
{
	//CListBox
	m_TList.ResetContent();

	FillListBox(m_TList, GetData()->m_csDTC);

	// Hide the buttons that don't work when not interactive
	if (GetInteract())
	{
		m_ResetDTC.EnableWindow(TRUE);
	}
	else
	{
		m_ResetDTC.EnableWindow(FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTCodesDlg message handlers

void CTCodesDlg::OnResetdtc() 
{
	// Sends the ECU command to reset the fault codes
	GetSupervisor()->ECUMode(ECU_CLEAR_DTCS);
}
