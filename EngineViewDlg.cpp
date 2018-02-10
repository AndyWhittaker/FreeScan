// EngineViewDlg.cpp : implementation file
//
// (c) 1996-99 Andy Whittaker, Chester, England. 
// mail@andywhittaker.com
//

#include "stdafx.h"
#include "FreeScan.h"
#include "MainDlg.h"
#include "Supervisor.h"

#include "EngineViewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// These are the spacings for the client view
//    Horizontal Spacing
#define HS	26
//    Title spacing from data
#define TS	57
//    Data spacing from title
#define DS	100
//    Column 1 Titles
#define COLT1 0
//    Column 1 Data
#define COLD1 DS
//    Column 2 Titles
#define COLT2 (COLD1 + TS)
//    Column 2 Data
#define COLD2 (COLT2 + DS)
//    Column 2 Titles
#define COLT3 (COLD2 + TS)
//    Column 2 Data
#define COLD3 (COLT3 + DS)

/////////////////////////////////////////////////////////////////////////////
// CEngineViewDlg property page

IMPLEMENT_DYNCREATE(CEngineViewDlg, CTTPropertyPage)

CEngineViewDlg::CEngineViewDlg() : CTTPropertyPage(CEngineViewDlg::IDD)
{
	//{{AFX_DATA_INIT(CEngineViewDlg)
	//}}AFX_DATA_INIT
	m_pMainDlg = NULL;
	m_bOneO2 = TRUE;	// True if only one O2 sensor
}

CEngineViewDlg::~CEngineViewDlg()
{
}

void CEngineViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CTTPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEngineViewDlg)
	DDX_Control(pDX, IDC_VIEW, m_view);
	//}}AFX_DATA_MAP

	//Updates the dialog.
	Refresh();
}

// Returns a pointer to the Supervisor
CSupervisor* CEngineViewDlg::GetSupervisor(void)
{
	return m_pMainDlg->m_pSupervisor;
}

// Returns a pointer to the Supervisor
CSupervisor* CEngineViewDlg::GetData(void)
{
	return m_pMainDlg->m_pSupervisor;
}

// Returns the current ECU Mode
DWORD CEngineViewDlg::GetCurrentMode(void)
{
	return GetSupervisor()->GetCurrentMode();
}

// Updates all of our controls
void CEngineViewDlg::Refresh(void)
{
// This draws the actual data to the screen
	CClientDC	Client(&m_view);
	Client.SetBkColor(RGB(0,0,180)); // sets the background colour
	Client.SetTextColor(RGB(255,255,255));

	CString buf;
	DWORD	dwCurrentMode = GetCurrentMode();

	buf.Format("%04X", GetData()->m_iEpromID);
	Client.TextOut(COLD1, 0  * HS, buf); // EPROM ID

	buf.Format("%d    ", GetData()->m_iRPM);
	Client.TextOut(COLD1, 1  * HS, buf); // RPM

	buf.Format("%d  ", GetData()->m_iIACPosition);
	Client.TextOut(COLD1, 2  * HS, buf); // IAC Position

	buf.Format("%d  ", GetData()->m_iDesiredIdle);
	Client.TextOut(COLD1, 3  * HS, buf); // Desired Idle

	buf.Format("%d   ", GetData()->m_iMPH);
	Client.TextOut(COLD1, 4  * HS, buf); // MPH

	buf.Format("%d   ", GetData()->m_iThrottlePos);
	Client.TextOut(COLD1, 5  * HS, buf); // Throttle Pos

	if (GetData()->m_iEngineLoad != 0)
		buf.Format("%d   ", GetData()->m_iEngineLoad);
	else
		buf.Format("  ");
	Client.TextOut(COLD1, 6  * HS, buf); // Engine Load

	if (GetData()->m_fMAP != 0.0)
		buf.Format("%5.3f ", GetData()->m_fMAP);
	else
		buf.Format(" ");
	Client.TextOut(COLD1, 7  * HS, buf); // MAP

	if (GetData()->m_fBaro != 0.0)
		buf.Format("%5.3f ", GetData()->m_fBaro);
	else
		buf.Format(" ");
	Client.TextOut(COLD1, 8  * HS, buf); // Barometer

	buf.Format("%5.3f ", GetData()->m_fO2VoltsLeft);
	Client.TextOut(COLD1, 9  * HS, buf); // O2 Volts Left

	if (!m_bOneO2)
		buf.Format("%5.3f ", GetData()->m_fO2VoltsRight);
	else
		buf.Format(" ");
	Client.TextOut(COLD1, 10 * HS, buf); // O2 Volts Right

	buf.Format("%d", GetData()->m_iRunTime);
	Client.TextOut(COLD1, 11 * HS, buf); // Engine Run Time

	// Draw second column
	if (GetData()->m_fStartWaterTemp != 0.0)
		buf.Format("%3.0f  ", GetData()->m_fStartWaterTemp);
	else
		buf.Format(" ");
	Client.TextOut(COLD2, 0  * HS, buf); // Start Temp

	buf.Format("%3.0f  ", GetData()->m_fWaterTemp);
	Client.TextOut(COLD2, 1  * HS, buf); // Coolant Temp

	if (GetData()->m_fMATTemp != 0.0)
		buf.Format("%3.0f  ", GetData()->m_fMATTemp);
	else
		buf.Format(" ");
	Client.TextOut(COLD2, 2  * HS, buf); // MAT

	if ( ((GetSupervisor()->m_bCentigrade == TRUE && GetData()->m_fOilTemp != 0.0) || (GetSupervisor()->m_bCentigrade == FALSE && GetData()->m_fOilTemp != 32.0)))
		buf.Format("%3.0f ", GetData()->m_fOilTemp);
	else
		buf.Format(" ");
	Client.TextOut(COLD2, 3  * HS, buf); // Oil Temp

	buf.Format("%3.1f  ", GetData()->m_fSparkAdvance);
	Client.TextOut(COLD2, 4  * HS, buf); // Spark Advance

	if (GetData()->m_fKnockRetard != 0.0)
		buf.Format("%3.0f ", GetData()->m_fKnockRetard);
	else
		buf.Format(" ");
	Client.TextOut(COLD2, 5  * HS, buf); // Knock Retard

	if (GetData()->m_iKnockCount != 0)
		buf.Format("%d  ", GetData()->m_iKnockCount);
	else
		buf.Format(" ");
	Client.TextOut(COLD2, 6  * HS, buf); // Knock Count

	if (GetData()->m_fAirFlow != 0.0)
		buf.Format("%4.0f ", GetData()->m_fAirFlow);
	else
		buf.Format("    ");
	Client.TextOut(COLD2, 7  * HS, buf); // Air Flow

	buf.Format("%3.1f ", GetData()->m_fBatteryVolts);
	Client.TextOut(COLD2, 8  * HS, buf); // Battery Volts

	buf.Format("%d   ", GetData()->m_iIntegratorL);
	Client.TextOut(COLD2, 9  * HS, buf); // Integrator L

	if (!m_bOneO2)
		buf.Format("%d  ", GetData()->m_iIntegratorR);
	else
		buf.Format("    ");
	Client.TextOut(COLD2, 10 * HS, buf); // Integrator R

	// Draw third column.
	buf.Format("%d ", GetData()->m_iCrankSensors);
	Client.TextOut(COLD3, 0  * HS, buf); // Crank Sensors

	if (GetData()->m_fAFRatio != 0.0)
		buf.Format("%3.1f", GetData()->m_fAFRatio);
	else
		buf.Format("   ");
	Client.TextOut(COLD3, 1  * HS, buf); // A/F Ratio

	if (GetData()->m_iBLM != 0)
		buf.Format("%d    ", GetData()->m_iBLM);
	else
		buf.Format("   ");
	Client.TextOut(COLD3, 2  * HS, buf); // BLM Contents

	if (GetData()->m_iBLMCell != 0)
		buf.Format("%d    ", GetData()->m_iBLMCell);
	else
		buf.Format("   ");
	Client.TextOut(COLD3, 3  * HS, buf); // BLM Cell #

	buf.Format("%d    ", GetData()->m_iRichLeanCounterL);
	Client.TextOut(COLD3, 9  * HS, buf); // Rich/Lean L

	if (!m_bOneO2)
		buf.Format("%d    ", GetData()->m_iRichLeanCounterR);
	else
		buf.Format("    ");
	Client.TextOut(COLD3, 10 * HS, buf); // Rich/Lean R
}

BEGIN_MESSAGE_MAP(CEngineViewDlg, CTTPropertyPage)
	//{{AFX_MSG_MAP(CEngineViewDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEngineViewDlg message handlers

BOOL CEngineViewDlg::OnInitDialog() 
{
	CTTPropertyPage::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEngineViewDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CClientDC	Client(&m_view);

	// A simple check to see if we have only one O2 sensor fitted
	if (GetData()->m_fO2VoltsRight == 0.0)
		m_bOneO2 = TRUE;
	else
		m_bOneO2 = FALSE;

	RECT	Rect;
	m_view.GetClientRect(&Rect);
	Client.FillSolidRect(&Rect,RGB(0,0,180)); // also sets the background colour
	Client.SetTextColor(RGB(255,255,255));

	Client.TextOut(COLT1, 0  * HS,"EPROM ID");
	Client.TextOut(COLT1, 1  * HS,"RPM");
	Client.TextOut(COLT1, 2  * HS,"IAC Position");
	Client.TextOut(COLT1, 3  * HS,"Desired Idle");
	if (GetSupervisor()->m_bMiles == TRUE)
		Client.TextOut(COLT1, 4  * HS,"MPH");
	else
		Client.TextOut(COLT1, 4  * HS,"KPH");

	Client.TextOut(COLT1, 5  * HS,"Throttle Pos");
	Client.TextOut(COLT1, 6  * HS,"Engine Load");
	Client.TextOut(COLT1, 7  * HS,"MAP");
	Client.TextOut(COLT1, 8  * HS,"Barometer");
	if (!m_bOneO2)
	{
		Client.TextOut(COLT1, 9  * HS,"O2 Volts L");
		Client.TextOut(COLT1, 10 * HS,"O2 Volts R");
	}
	else
		Client.TextOut(COLT1, 9  * HS,"O2 Volts  ");

	Client.TextOut(COLT1, 11 * HS,"Run Time");

	// draw a vertical partition.
	Client.MoveTo(COLT2 - 6 , 0);
	Client.LineTo(COLT2 - 6, Rect.bottom);
	Client.MoveTo(COLT2 - 5 , 0);
	Client.LineTo(COLT2 - 5, Rect.bottom);

	Client.TextOut(COLT2, 0  * HS,"Start Temp");
	Client.TextOut(COLT2, 1  * HS,"Coolant Temp");
	Client.TextOut(COLT2, 2  * HS,"Mass Air Temp");
	if ( ((GetSupervisor()->m_bCentigrade == TRUE && GetData()->m_fOilTemp != 0.0) || (GetSupervisor()->m_bCentigrade == FALSE && GetData()->m_fOilTemp != 32.0)))
	{
		Client.TextOut(COLT2, 3  * HS,"Oil Temp");
	}
	Client.TextOut(COLT2, 4  * HS,"Spark Advance");
	Client.TextOut(COLT2, 5  * HS,"Knock Retard");
	Client.TextOut(COLT2, 6  * HS,"Knock Count");
	Client.TextOut(COLT2, 7  * HS,"Air Flow");
	Client.TextOut(COLT2, 8  * HS,"Battery Volts");
	if (!m_bOneO2)
	{
		Client.TextOut(COLT2, 9  * HS,"Integrator L");
		Client.TextOut(COLT2, 10 * HS,"Integrator R");
	}
	else
		Client.TextOut(COLT2, 9  * HS,"Integrator  ");

	if (GetSupervisor()->m_bCentigrade == TRUE)
	{
		Client.TextOut(COLT2 + 137, 0  * HS,"°C");
		Client.TextOut(COLT2 + 137, 1  * HS,"°C");
		Client.TextOut(COLT2 + 137, 2  * HS,"°C");
		if (GetData()->m_fOilTemp != 0.0)
			Client.TextOut(COLT2 + 137, 3  * HS,"°C");
	}
	else
	{
		Client.TextOut(COLT2 + 137, 0  * HS,"°F");
		Client.TextOut(COLT2 + 137, 1  * HS,"°F");
		Client.TextOut(COLT2 + 137, 2  * HS,"°F");
		if (GetData()->m_fOilTemp != 32.0)
			Client.TextOut(COLT2 + 137, 3  * HS,"°F");
	}

	// draw a vertical partition.
	Client.MoveTo(COLT3 - 6 , 0);
	Client.LineTo(COLT3 - 6, Rect.bottom);
	Client.MoveTo(COLT3 - 5 , 0);
	Client.LineTo(COLT3 - 5, Rect.bottom);

	Client.TextOut(COLT3, 0  * HS,"Crank Sensors");
	Client.TextOut(COLT3, 1  * HS,"A/F Ratio");
	Client.TextOut(COLT3, 2  * HS,"BLM Contents");
	Client.TextOut(COLT3, 3  * HS,"BLM Cell #");
	if (!m_bOneO2)
	{
		Client.TextOut(COLT3, 9  * HS,"Rich/Lean L");
		Client.TextOut(COLT3, 10 * HS,"Rich/Lean R");
	}
	else
		Client.TextOut(COLT3, 9  * HS,"Rich/Lean  ");

	Refresh(); // update the dynamic data
	
	// Do not call CTTPropertyPage::OnPaint() for painting messages
}
