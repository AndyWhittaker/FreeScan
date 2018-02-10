// DesIdleDlg.cpp : implementation file
//
// (c) 1996-99 Andy Whittaker, Chester, England. 
// mail@andywhittaker.com

#include "stdafx.h"
#include "FreeScan.h"
#include "DesIdleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDesIdleDlg dialog


CDesIdleDlg::CDesIdleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDesIdleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDesIdleDlg)
	m_Value = 0;
	//}}AFX_DATA_INIT

	//CSpinButtonCtrl
}


void CDesIdleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDesIdleDlg)
	DDX_Control(pDX, IDC_SPIN, m_Spin);
	DDX_Text(pDX, IDC_VALUE, m_Value);
	DDV_MinMaxUInt(pDX, m_Value, 800, 1800);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDesIdleDlg, CDialog)
	//{{AFX_MSG_MAP(CDesIdleDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDesIdleDlg message handlers

BOOL CDesIdleDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_Spin.SetRange(800, 1800);
	m_Value= 800;

	UDACCEL uda;

	uda.nSec = 0;
	uda.nInc = 25;
	m_Spin.SetAccel(1, &uda);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
