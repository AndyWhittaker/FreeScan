// StatusBitsDlg.cpp : implementation file
//
// (c) 1996-99 Andy Whittaker, Chester, England. 
// mail@andywhittaker.com

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
	m_pSupervisor = NULL;
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
	if (m_pSupervisor != NULL) {
		Refresh(m_pSupervisor->GetEcuData());
	}
}

BEGIN_MESSAGE_MAP(CStatusBitsDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CStatusBitsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Updates all of our controls
void CStatusBitsDlg::Refresh(const CEcuData* const ecuData)
{
	if (ecuData->m_bACRequest == TRUE)
		m_led1.SetLed(CLed::LED_COLOR_RED,CLed::LED_ON,CLed::LED_ROUND);
	else
		m_led1.SetLed(CLed::LED_COLOR_RED,CLed::LED_OFF,CLed::LED_ROUND);

	if (ecuData->m_bACClutch == TRUE)
		m_led2.SetLed(CLed::LED_COLOR_RED,CLed::LED_ON,CLed::LED_ROUND);
	else
		m_led2.SetLed(CLed::LED_COLOR_RED,CLed::LED_OFF,CLed::LED_ROUND);

	if (ecuData->m_bEngineClosedLoop == TRUE)
		m_led3.SetLed(CLed::LED_COLOR_RED,CLed::LED_ON,CLed::LED_ROUND);
	else
		m_led3.SetLed(CLed::LED_COLOR_RED,CLed::LED_OFF,CLed::LED_ROUND);
//	m_led1.SetLed(CLed::LED_COLOR_RED,CLed::LED_DISABLED,CLed::LED_ROUND);
}

void CStatusBitsDlg::RegisterSupervisor(CSupervisorInterface* const pSupervisor) {
	m_pSupervisor = pSupervisor;
}

/////////////////////////////////////////////////////////////////////////////
// CStatusBitsDlg message handlers
