// RawMode00.cpp : implementation file
//
// (c) 1996-99 Andy Whittaker, Chester, England. 
// mail@andywhittaker.com
//

#include "stdafx.h"
#include "FreeScan.h"
#include "MainDlg.h"
#include "Supervisor.h"

#include "RawMode00.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRawMode00 property page

IMPLEMENT_DYNCREATE(CRawMode00, CPropertyPage)

CRawMode00::CRawMode00() : CPropertyPage(CRawMode00::IDD)
{
	//{{AFX_DATA_INIT(CRawMode00)
	//}}AFX_DATA_INIT

	m_pMainDlg = NULL;
}

CRawMode00::~CRawMode00()
{
}

void CRawMode00::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRawMode00)
	DDX_Control(pDX, IDC_0A_DB02, m_0a_db02);
	DDX_Control(pDX, IDC_0A_DB01, m_0a_db01);
	DDX_Control(pDX, IDC_0A_DB00, m_0a_db00);
	DDX_Control(pDX, IDC_05_DB09, m_05_db09);
	DDX_Control(pDX, IDC_05_DB08, m_05_db08);
	DDX_Control(pDX, IDC_05_DB07, m_05_db07);
	DDX_Control(pDX, IDC_05_DB06, m_05_db06);
	DDX_Control(pDX, IDC_05_DB05, m_05_db05);
	DDX_Control(pDX, IDC_05_DB04, m_05_db04);
	DDX_Control(pDX, IDC_05_DB03, m_05_db03);
	DDX_Control(pDX, IDC_05_DB02, m_05_db02);
	DDX_Control(pDX, IDC_05_DB01, m_05_db01);
	DDX_Control(pDX, IDC_05_DB00, m_05_db00);
	//}}AFX_DATA_MAP
	
	//Updates the dialog.
	Refresh();
}


BEGIN_MESSAGE_MAP(CRawMode00, CPropertyPage)
	//{{AFX_MSG_MAP(CRawMode00) 
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// Returns a pointer to the Supervisor
CSupervisor* CRawMode00::GetSupervisor(void)
{
	return m_pMainDlg->m_pSupervisor;
}

// Updates all of our controls
void CRawMode00::Refresh(void)
{
	CString buf;

	buf.Format("%02X", GetSupervisor()->m_ucF005[0]);
	m_05_db00.SetWindowText(buf);
	buf.Format("%02X", GetSupervisor()->m_ucF005[1]);
	m_05_db01.SetWindowText(buf);
	buf.Format("%02X", GetSupervisor()->m_ucF005[2]);
	m_05_db02.SetWindowText(buf);
	buf.Format("%02X", GetSupervisor()->m_ucF005[3]);
	m_05_db03.SetWindowText(buf);
	buf.Format("%02X", GetSupervisor()->m_ucF005[4]);
	m_05_db04.SetWindowText(buf);
	buf.Format("%02X", GetSupervisor()->m_ucF005[5]);
	m_05_db05.SetWindowText(buf);
	buf.Format("%02X", GetSupervisor()->m_ucF005[6]);
	m_05_db06.SetWindowText(buf);
	buf.Format("%02X", GetSupervisor()->m_ucF005[7]);
	m_05_db07.SetWindowText(buf);
	buf.Format("%02X", GetSupervisor()->m_ucF005[8]);
	m_05_db08.SetWindowText(buf);
	buf.Format("%02X", GetSupervisor()->m_ucF005[9]);
	m_05_db09.SetWindowText(buf);
	
	buf.Format("%02X", GetSupervisor()->m_ucF00A[0]);
	m_0a_db00.SetWindowText(buf);
	buf.Format("%02X", GetSupervisor()->m_ucF00A[1]);
	m_0a_db01.SetWindowText(buf);
	buf.Format("%02X", GetSupervisor()->m_ucF00A[2]);
	m_0a_db02.SetWindowText(buf);

}

	/////////////////////////////////////////////////////////////////////////////
// CRawMode00 message handlers
