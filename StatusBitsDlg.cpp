// StatusBitsDlg.cpp : implementation file
//
// (c) 1996-99 Andy Whittaker, Chester, England. 
// mail@andywhittaker.com

#include "stdafx.h"
#include "FreeScan.h"
#include "MainDlg.h"
#include "Supervisor.h"

#include "StatusBitsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatusBitsDlg property page

IMPLEMENT_DYNCREATE(CStatusBitsDlg, CPropertyPage)

CStatusBitsDlg::CStatusBitsDlg() : CPropertyPage(CStatusBitsDlg::IDD)
{
	//{{AFX_DATA_INIT(CStatusBitsDlg)
	//}}AFX_DATA_INIT
	m_pMainDlg = NULL;
}

CStatusBitsDlg::~CStatusBitsDlg()
{
}

void CStatusBitsDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatusBitsDlg)
	DDX_Control(pDX, IDC_LED3, m_led3);
	DDX_Control(pDX, IDC_LED2, m_led2);
	DDX_Control(pDX, IDC_LED1, m_led1);
	//}}AFX_DATA_MAP

	//Updates the dialog.
	Refresh();
}

// Returns a pointer to the Supervisor
CSupervisor* CStatusBitsDlg::GetSupervisor(void)
{
	return m_pMainDlg->m_pSupervisor;
}

// Returns a pointer to the Supervisor
CSupervisor* CStatusBitsDlg::GetData(void)
{
	return m_pMainDlg->m_pSupervisor;
}

// Returns if the ECU is interactive
BOOL CStatusBitsDlg::GetInteract(void)
{
	return GetSupervisor()->GetInteract();
}

BEGIN_MESSAGE_MAP(CStatusBitsDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CStatusBitsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Updates all of our controls
void CStatusBitsDlg::Refresh(void)
{
	if (GetData()->m_bACRequest)
		m_led1.SetLed(CLed::LED_COLOR_RED,CLed::LED_ON,CLed::LED_ROUND);
	else
		m_led1.SetLed(CLed::LED_COLOR_RED,CLed::LED_OFF,CLed::LED_ROUND);

	if (GetData()->m_bACClutch)
		m_led2.SetLed(CLed::LED_COLOR_RED,CLed::LED_ON,CLed::LED_ROUND);
	else
		m_led2.SetLed(CLed::LED_COLOR_RED,CLed::LED_OFF,CLed::LED_ROUND);

	if (GetData()->m_bEngineClosedLoop)
		m_led3.SetLed(CLed::LED_COLOR_RED,CLed::LED_ON,CLed::LED_ROUND);
	else
		m_led3.SetLed(CLed::LED_COLOR_RED,CLed::LED_OFF,CLed::LED_ROUND);
//	m_led1.SetLed(CLed::LED_COLOR_RED,CLed::LED_DISABLED,CLed::LED_ROUND);
}

/////////////////////////////////////////////////////////////////////////////
// CStatusBitsDlg message handlers
