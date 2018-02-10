// RawMode01.cpp : implementation file
//
// (c) 1996-99 Andy Whittaker, Chester, England. 
// mail@andywhittaker.com
//

#include "stdafx.h"
#include "FreeScan.h"
#include "MainDlg.h"
#include "Supervisor.h"

#include "RawMode01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRawMode01 property page

IMPLEMENT_DYNCREATE(CRawMode01, CPropertyPage)

CRawMode01::CRawMode01() : CPropertyPage(CRawMode01::IDD)
{
	//{{AFX_DATA_INIT(CRawMode01)
	//}}AFX_DATA_INIT

	m_pMainDlg = NULL;
}

CRawMode01::~CRawMode01()
{
}

void CRawMode01::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRawMode01)
	DDX_Control(pDX, IDC_DB07, m_db07);
	DDX_Control(pDX, IDC_DB06, m_db06);
	DDX_Control(pDX, IDC_DB05, m_db05);
	DDX_Control(pDX, IDC_DB04, m_db04);
	DDX_Control(pDX, IDC_DB03, m_db03);
	DDX_Control(pDX, IDC_DB02, m_db02);
	DDX_Control(pDX, IDC_DB01, m_db01);
	DDX_Control(pDX, IDC_DB00, m_db00);
	//}}AFX_DATA_MAP

	//Updates the dialog.
	Refresh();
}


BEGIN_MESSAGE_MAP(CRawMode01, CPropertyPage)
	//{{AFX_MSG_MAP(CRawMode01)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// Returns a pointer to the Supervisor
CSupervisor* CRawMode01::GetSupervisor(void)
{
	return m_pMainDlg->m_pSupervisor;
}

/////////////////////////////////////////////////////////////////////////////
// CRawMode01 message handlers
// Updates all of our controls
void CRawMode01::Refresh(void)
{
	CString buf;

	buf.Format("%02X       %02X       %02X       %02X       %02X       %02X       %02X       %02X",
		GetSupervisor()->m_ucF001[1],
		GetSupervisor()->m_ucF001[2],
		GetSupervisor()->m_ucF001[3],
		GetSupervisor()->m_ucF001[4],
		GetSupervisor()->m_ucF001[5],
		GetSupervisor()->m_ucF001[6],
		GetSupervisor()->m_ucF001[7],
		GetSupervisor()->m_ucF001[8]);
	m_db00.SetWindowText(buf);

	buf.Format("%02X       %02X       %02X       %02X       %02X       %02X       %02X       %02X",
		GetSupervisor()->m_ucF001[9],
		GetSupervisor()->m_ucF001[10],
		GetSupervisor()->m_ucF001[11],
		GetSupervisor()->m_ucF001[12],
		GetSupervisor()->m_ucF001[13],
		GetSupervisor()->m_ucF001[14],
		GetSupervisor()->m_ucF001[15],
		GetSupervisor()->m_ucF001[16]);
	m_db01.SetWindowText(buf);

	buf.Format("%02X       %02X       %02X       %02X       %02X       %02X       %02X       %02X",
		GetSupervisor()->m_ucF001[17],
		GetSupervisor()->m_ucF001[18],
		GetSupervisor()->m_ucF001[19],
		GetSupervisor()->m_ucF001[20],
		GetSupervisor()->m_ucF001[21],
		GetSupervisor()->m_ucF001[22],
		GetSupervisor()->m_ucF001[23],
		GetSupervisor()->m_ucF001[24]);
	m_db02.SetWindowText(buf);

	buf.Format("%02X       %02X       %02X       %02X       %02X       %02X       %02X       %02X",
		GetSupervisor()->m_ucF001[25],
		GetSupervisor()->m_ucF001[26],
		GetSupervisor()->m_ucF001[27],
		GetSupervisor()->m_ucF001[28],
		GetSupervisor()->m_ucF001[29],
		GetSupervisor()->m_ucF001[30],
		GetSupervisor()->m_ucF001[31],
		GetSupervisor()->m_ucF001[32]);
	m_db03.SetWindowText(buf);

	buf.Format("%02X       %02X       %02X       %02X       %02X       %02X       %02X       %02X",
		GetSupervisor()->m_ucF001[33],
		GetSupervisor()->m_ucF001[34],
		GetSupervisor()->m_ucF001[35],
		GetSupervisor()->m_ucF001[36],
		GetSupervisor()->m_ucF001[37],
		GetSupervisor()->m_ucF001[38],
		GetSupervisor()->m_ucF001[39],
		GetSupervisor()->m_ucF001[40]);
	m_db04.SetWindowText(buf);

	buf.Format("%02X       %02X       %02X       %02X       %02X       %02X       %02X       %02X",
		GetSupervisor()->m_ucF001[41],
		GetSupervisor()->m_ucF001[42],
		GetSupervisor()->m_ucF001[43],
		GetSupervisor()->m_ucF001[44],
		GetSupervisor()->m_ucF001[45],
		GetSupervisor()->m_ucF001[46],
		GetSupervisor()->m_ucF001[47],
		GetSupervisor()->m_ucF001[48]);
	m_db05.SetWindowText(buf);

	buf.Format("%02X       %02X       %02X       %02X       %02X       %02X       %02X       %02X",
		GetSupervisor()->m_ucF001[49],
		GetSupervisor()->m_ucF001[50],
		GetSupervisor()->m_ucF001[51],
		GetSupervisor()->m_ucF001[52],
		GetSupervisor()->m_ucF001[53],
		GetSupervisor()->m_ucF001[54],
		GetSupervisor()->m_ucF001[55],
		GetSupervisor()->m_ucF001[56]);
	m_db06.SetWindowText(buf);

	buf.Format("%02X       %02X       %02X       %02X       %02X       %02X       %02X       %02X",
		GetSupervisor()->m_ucF001[57],
		GetSupervisor()->m_ucF001[58],
		GetSupervisor()->m_ucF001[59],
		GetSupervisor()->m_ucF001[60],
		GetSupervisor()->m_ucF001[61],
		GetSupervisor()->m_ucF001[62],
		GetSupervisor()->m_ucF001[63],
		GetSupervisor()->m_ucF001[64]);
	m_db07.SetWindowText(buf);
}
