// EngineViewDlg.cpp : implementation file
//
// (c) 1996-99 Andy Whittaker, Chester, England. 
// mail@andywhittaker.com
//

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
	m_pSupervisor = NULL;
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
	if (m_pSupervisor != NULL) {
		Refresh(m_pSupervisor->GetEcuData());
	}
}

static inline void renderField(CClientDC * const client, const int column, const int row, const char *const textFormat, const float fValue) {
	if (!CEcuData::isValid(fValue)) {
		client->TextOut(column, row  * HS, "N/A");
	}
	else {
		CString buf;
		buf.Format(textFormat, fValue);
		client->TextOut(column, row  * HS, buf);
	}
}

static inline void renderField(CClientDC * const client, const int column, const int row, const char *const textFormat, const int iValue) {
	if (!CEcuData::isValid(iValue)) {
		client->TextOut(column, row  * HS, "N/A");
	}
	else {
		CString buf;
		buf.Format(textFormat, iValue);
		client->TextOut(column, row  * HS, buf);
	}
}

// Updates all of our controls
void CEngineViewDlg::Refresh(const CEcuData* const ecuData)
{
// This draws the actual data to the screen
	CClientDC	Client(&m_view);
	Client.SetBkColor(RGB(0,0,180)); // sets the background colour
	Client.SetTextColor(RGB(255,255,255));

	LOGFONT lf;
	Client.GetCurrentFont()->GetLogFont(&lf);

	lf.lfWeight = FW_NORMAL;
	lf.lfPitchAndFamily = FIXED_PITCH | FF_MODERN;
	CFont font;
	font.CreateFontIndirect(&lf);
	Client.SelectObject(font);
	
	renderField(&Client, COLD1, 0, " %04X", ecuData->m_iEpromID);

	renderField(&Client, COLD1, 1, "%5d", ecuData->m_iRPM);

	renderField(&Client, COLD1, 2, "%5d", ecuData->m_iIACPosition);

	renderField(&Client, COLD1, 3, "%5d", ecuData->m_iDesiredIdle);

	if (m_pSupervisor->GetMiles() == TRUE) {
		renderField(&Client, COLD1, 4, "%5d", ecuData->m_iMPH);
	}
	else {
		renderField(&Client, COLD1, 4, "%5d", ecuData->m_iMPH_inKPH);
	}

	renderField(&Client, COLD1, 5, "%5d", ecuData->m_iThrottlePos);

	renderField(&Client, COLD1, 6, "%5d", ecuData->m_iEngineLoad);

	renderField(&Client, COLD1, 7, "%5.3f", ecuData->m_fMAP);

	renderField(&Client, COLD1, 8, "%5.3f", ecuData->m_fBaro);

	renderField(&Client, COLD1, 9, "%5.3f", ecuData->m_fO2VoltsLeft);

	if (m_bOneO2 == FALSE) {
		renderField(&Client, COLD1, 10, "%5.3f", ecuData->m_fO2VoltsRight);
	}
	else {
		renderField(&Client, COLD1, 10, "     ", 0);
	}

	renderField(&Client, COLD1, 11, "%5d", ecuData->m_iRunTime);

	// Draw second column
	if (m_pSupervisor->GetCentigrade() == TRUE) {
		renderField(&Client, COLD2, 0, "%3.0f", ecuData->m_fStartWaterTemp);
		renderField(&Client, COLD2, 1, "%3.0f", ecuData->m_fWaterTemp);
		renderField(&Client, COLD2, 2, "%3.0f", ecuData->m_fMATTemp);
		if (CEcuData::isSupported(ecuData->m_fOilTemp)) {
			renderField(&Client, COLD2, 3, "%3.0f", ecuData->m_fOilTemp);
		}
	}
	else {
		renderField(&Client, COLD2, 0, "%3.0f", ecuData->m_fStartWaterTemp_inF);
		renderField(&Client, COLD2, 1, "%3.0f", ecuData->m_fWaterTemp_inF);
		renderField(&Client, COLD2, 2, "%3.0f", ecuData->m_fMATTemp_inF);
		if (CEcuData::isSupported(ecuData->m_fOilTemp)) {
			renderField(&Client, COLD2, 3, "%3.0f", ecuData->m_fOilTemp_inF);
		}
	}
	
	renderField(&Client, COLD2, 4, "%5.1f", ecuData->m_fSparkAdvance);
	
	renderField(&Client, COLD2, 5, "%5.0f", ecuData->m_fKnockRetard);
	
	renderField(&Client, COLD2, 6, "%5d", ecuData->m_iKnockCount);
	
	if (CEcuData::isSupported(ecuData->m_fAirFlow)) {
		renderField(&Client, COLD2, 7, "%5.0f", ecuData->m_fAirFlow);
	}
	
	renderField(&Client, COLD2, 8, "%5.1f", ecuData->m_fBatteryVolts);
	
	renderField(&Client, COLD2, 9, "%5d", ecuData->m_iIntegratorL);

	if (m_bOneO2 == FALSE) {
		renderField(&Client, COLD2, 10, "%5d", ecuData->m_iIntegratorR);
	}
	else {
		renderField(&Client, COLD2, 10, "     ", 0);
	}

	// Draw third column.
	renderField(&Client, COLD3, 0, "%5d", ecuData->m_iCrankSensors);

	renderField(&Client, COLD3, 1, "%5.1f", ecuData->m_fAFRatio);

	renderField(&Client, COLD3, 2, "%5d", ecuData->m_iBLM);

	renderField(&Client, COLD3, 3, "%5d", ecuData->m_iBLMCell);

	renderField(&Client, COLD3, 9, "%5d", ecuData->m_iRichLeanCounterL);

	if (m_bOneO2 == FALSE) {
		renderField(&Client, COLD2, 10, "%5d", ecuData->m_iRichLeanCounterR);
	}
	else {
		renderField(&Client, COLD2, 10, "     ", 0);
	}
}

void CEngineViewDlg::RegisterSupervisor(CSupervisorInterface* const pSupervisor) {
	m_pSupervisor = pSupervisor;
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

	const CEcuData *const ecuData = m_pSupervisor->GetEcuData();

	// A simple check to see if we have only one O2 sensor fitted
	if (!CEcuData::isSupported(ecuData->m_fO2VoltsRight))
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
	if (m_pSupervisor->GetMiles() == TRUE)
		Client.TextOut(COLT1, 4  * HS,"MPH");
	else
		Client.TextOut(COLT1, 4  * HS,"KPH");

	Client.TextOut(COLT1, 5  * HS,"Throttle Pos");
	Client.TextOut(COLT1, 6  * HS,"Engine Load");
	Client.TextOut(COLT1, 7  * HS,"MAP");
	Client.TextOut(COLT1, 8  * HS,"Barometer");
	if (m_bOneO2 == FALSE) {
		Client.TextOut(COLT1, 9  * HS,"O2 Volts L");
		Client.TextOut(COLT1, 10 * HS,"O2 Volts R");
	}
	else {
		Client.TextOut(COLT1, 9  * HS,"O2 Volts  ");
	}

	Client.TextOut(COLT1, 11 * HS,"Run Time");

	// draw a vertical partition.
	Client.MoveTo(COLT2 - 6 , 0);
	Client.LineTo(COLT2 - 6, Rect.bottom);
	Client.MoveTo(COLT2 - 5 , 0);
	Client.LineTo(COLT2 - 5, Rect.bottom);

	Client.TextOut(COLT2, 0  * HS,"Start Temp");
	Client.TextOut(COLT2, 1  * HS,"Coolant Temp");
	Client.TextOut(COLT2, 2  * HS,"Mass Air Temp");
	if (CEcuData::isSupported(ecuData->m_fOilTemp)) {
		Client.TextOut(COLT2, 3  * HS,"Oil Temp");
	}
	Client.TextOut(COLT2, 4  * HS,"Spark Advance");
	Client.TextOut(COLT2, 5  * HS,"Knock Retard");
	Client.TextOut(COLT2, 6  * HS,"Knock Count");
	
	if (CEcuData::isSupported(ecuData->m_fAirFlow)) {
		Client.TextOut(COLT2, 7 * HS, "Air Flow");
	}
	Client.TextOut(COLT2, 8  * HS,"Battery Volts");
	if (m_bOneO2 == FALSE) {
		Client.TextOut(COLT2, 9  * HS,"Integrator L");
		Client.TextOut(COLT2, 10 * HS,"Integrator R");
	}
	else {
		Client.TextOut(COLT2, 9  * HS,"Integrator  ");
	}

	if (m_pSupervisor->GetCentigrade() == TRUE)	{
		Client.TextOut(COLT2 + 130, 0  * HS,"°C");
		Client.TextOut(COLT2 + 130, 1  * HS,"°C");
		Client.TextOut(COLT2 + 130, 2  * HS,"°C");
		if (CEcuData::isSupported(ecuData->m_fOilTemp)) {
			Client.TextOut(COLT2 + 130, 3 * HS, "°C");
		}
	}
	else {
		Client.TextOut(COLT2 + 130, 0  * HS,"°F");
		Client.TextOut(COLT2 + 130, 1  * HS,"°F");
		Client.TextOut(COLT2 + 130, 2  * HS,"°F");
		if (CEcuData::isSupported(ecuData->m_fOilTemp)) {
			Client.TextOut(COLT2 + 130, 3 * HS, "°F");
		}
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
	if (m_bOneO2 == FALSE) {
		Client.TextOut(COLT3, 9  * HS,"Rich/Lean L");
		Client.TextOut(COLT3, 10 * HS,"Rich/Lean R");
	}
	else {
		Client.TextOut(COLT3, 9 * HS, "Rich/Lean  ");
	}

	Refresh(ecuData); // update the dynamic data
	
	// Do not call CTTPropertyPage::OnPaint() for painting messages
}
