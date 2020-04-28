// DashBoardDlg.cpp : implementation file
//
// (c) 1996-99 Andy Whittaker, Chester, England. 
// mail@andywhittaker.com

#include "DashBoardDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const int THROTTLE_MIN = 0;
static const int THROTTLE_MAX = 100;

static const int LOAD_MIN = 0;
static const int LOAD_MAX = 100;

static const int BOOST_MIN = 0;
static const int BOOST_MAX = 220; // 2.20 Bar

static const int MAT_MIN = 0;
static const int MAT_MAX = 800; // 80°C

static const int SPEEDO_MIN = 0;
static const int SPEEDO_MAX = 170; // 170 MPH

static const int TACHO_MIN = 0;
static const int TACHO_MAX = 8000; // 8000 RPM

static const int AIR_FUEL_RATIO_MIN = 100; // 10.0
static const int AIR_FUEL_RATIO_MAX = 170; // 17.0

static const int WATER_MIN = 500; // 50°C
static const int WATER_MAX = 1100; // 110°C

static const int BAT_VOLT_MIN = 100; // 10.0V
static const int BAT_VOLT_MAX = 170; // 17.0V

static const int SPARK_MIN = 0;
static const int SPARK_MAX = 600; // 60.0°

/////////////////////////////////////////////////////////////////////////////
// CDashBoardDlg property page

IMPLEMENT_DYNCREATE(CDashBoardDlg, CTTPropertyPage)

CDashBoardDlg::CDashBoardDlg() : CTTPropertyPage(CDashBoardDlg::IDD)
{
	//{{AFX_DATA_INIT(CDashBoardDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pSupervisor = NULL;
}

CDashBoardDlg::~CDashBoardDlg()
{
}

void CDashBoardDlg::DoDataExchange(CDataExchange* pDX)
{
	CTTPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDashBoardDlg)
	DDX_Control(pDX, IDC_THROT, m_Throttle);
	DDX_Control(pDX, IDC_ENGINELOAD, m_EngineLoad);	
	DDX_Control(pDX, IDC_BOOST, m_Boost);
	DDX_Control(pDX, IDC_BOOST_TEXT, m_BoostText);
	DDX_Control(pDX, IDC_MAT, m_MAT);
	DDX_Control(pDX, IDC_MAT_TEXT, m_MATText);
	DDX_Control(pDX, IDC_SPEEDO, m_Speedo);
	DDX_Control(pDX, IDC_SPEEDO_MPH_TEXT, m_SpeedoMphText);
	DDX_Control(pDX, IDC_SPEEDO_KPH_TEXT, m_SpeedoKphText);
	DDX_Control(pDX, IDC_TACHO, m_Tacho);
	DDX_Control(pDX, IDC_TACHO_TEXT, m_TachoText);	
	DDX_Control(pDX, IDC_AIRFUEL, m_AirFuelRatio);
	DDX_Control(pDX, IDC_AIRFUEL_TEXT, m_AirFuelRatioText);
	DDX_Control(pDX, IDC_WATER, m_Water);
	DDX_Control(pDX, IDC_WATER_TEXT, m_WaterText);	
	DDX_Control(pDX, IDC_VOLT, m_Volt);
	DDX_Control(pDX, IDC_VOLT_TEXT, m_VoltText);
	DDX_Control(pDX, IDC_SPARK, m_Spark);
	DDX_Control(pDX, IDC_SPARK_TEXT, m_SparkText);
	//}}AFX_DATA_MAP

	//Updates the dialog.
	if (m_pSupervisor != NULL) {
		Refresh(m_pSupervisor->GetEcuData());
	}
}

static inline void updateField(CProgressCtrl *const progressMeter, CEdit * const textBox, const char *const textFormat, const float fValue, const float progressMeterScaleFactor, const int progressMeterMin, const int progressMeterMax) {
	if (CEcuData::isValid(fValue)) {
		if (textBox != NULL && textFormat != NULL) {
			CString buf;
			buf.Format(textFormat, fValue);
			textBox->SetWindowText(buf);
		}

		if (progressMeter != NULL) {
			int progressMeterValue = (int)(fValue * progressMeterScaleFactor);
			if (progressMeterValue < progressMeterMin) {
				progressMeterValue = progressMeterMin;
			}
			else if (progressMeterValue > progressMeterMax) {
				progressMeterValue = progressMeterMax;
			}
			progressMeter->SetPos(progressMeterValue);
		}
	}
	else if (textBox != NULL) {
		textBox->SetWindowText("N/A ");
	}
}

static inline void updateField(CProgressCtrl *const progressMeter, CEdit * const textBox, const char *const textFormat, const int iValue, const int progressMeterScaleFactor, const int progressMeterMin, const int progressMeterMax) {
	if (CEcuData::isValid(iValue)) {
		if (textBox != NULL) {
			CString buf;
			buf.Format(textFormat, iValue);
			textBox->SetWindowText(buf);
		}

		if (progressMeter != NULL) {
			int progressMeterValue = iValue * progressMeterScaleFactor;
			if (progressMeterValue < progressMeterMin) {
				progressMeterValue = progressMeterMin;
			}
			else if (progressMeterValue > progressMeterMax) {
				progressMeterValue = progressMeterMax;
			}
			progressMeter->SetPos(progressMeterValue);
		}
	}
	else if (textBox != NULL) {
		textBox->SetWindowText("N/A ");
	}
}

// Updates all of our controls
void CDashBoardDlg::Refresh(const CEcuData* const ecuData)
{
	updateField(&m_AirFuelRatio, &m_AirFuelRatioText, "%3.1f ", ecuData->m_fAFRatio, 10.0f, AIR_FUEL_RATIO_MIN, AIR_FUEL_RATIO_MAX);

	updateField(&m_Water, &m_WaterText, "%3.1f ", ecuData->m_fWaterTemp, 10.0f, WATER_MIN, WATER_MAX);
	
	updateField(&m_MAT, &m_MATText, "%3.1f ", ecuData->m_fMATTemp, 10.0f, MAT_MIN, MAT_MAX);
	
	updateField(&m_Volt, &m_VoltText, "%3.1f ", ecuData->m_fBatteryVolts, 10.0f, BAT_VOLT_MIN, BAT_VOLT_MAX);
	
	updateField(&m_Boost, &m_BoostText, "%3.2f ", ecuData->m_fMAP, 100.0f, BOOST_MIN, BOOST_MAX);
	
	updateField(&m_Spark, &m_SparkText, "%3.1f ", ecuData->m_fSparkAdvance, 10.0f, SPARK_MIN, SPARK_MAX);

	updateField(&m_Tacho, &m_TachoText, "%4d ", ecuData->m_iRPM, 1, TACHO_MIN, TACHO_MAX);

	updateField(&m_Speedo, &m_SpeedoMphText, "%3d ", ecuData->m_iMPH,       1, SPEEDO_MIN, SPEEDO_MAX);
	updateField(NULL,      &m_SpeedoKphText, "%3d ", ecuData->m_iMPH_inKPH, 1,          0,          0);

	updateField(&m_Throttle, NULL, NULL, ecuData->m_iThrottlePos, 1, THROTTLE_MIN, THROTTLE_MAX);

	updateField(&m_EngineLoad, NULL, NULL, ecuData->m_iEngineLoad, 1, LOAD_MIN, LOAD_MAX);
}

void CDashBoardDlg::RegisterSupervisor(CSupervisorInterface* const pSupervisor) {
	m_pSupervisor = pSupervisor;
}

BEGIN_MESSAGE_MAP(CDashBoardDlg, CTTPropertyPage)
	//{{AFX_MSG_MAP(CDashBoardDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static inline void setProgressMeterBounds(CProgressCtrl *const progressMeter, const int progressMeterMin, const int progressMeterMax, const int pixesWidth) {
	progressMeter->SetRange32(progressMeterMin, progressMeterMax);
	progressMeter->SetStep(min(1, (progressMeterMax - progressMeterMin) / pixesWidth));
}

/////////////////////////////////////////////////////////////////////////////
// CDashBoardDlg message handlers

BOOL CDashBoardDlg::OnInitDialog() 
{
	// Create the different background colour for this page
	m_brush.CreateSolidBrush(RGB(0, 0, 0)); // Black

	CTTPropertyPage::OnInitDialog();

	setProgressMeterBounds(&m_Throttle, THROTTLE_MIN, THROTTLE_MAX, 80);

	setProgressMeterBounds(&m_EngineLoad, LOAD_MIN, LOAD_MAX, 80);

	setProgressMeterBounds(&m_Boost, BOOST_MIN, BOOST_MAX, 80);

	setProgressMeterBounds(&m_MAT, MAT_MIN, MAT_MAX, 80);

	setProgressMeterBounds(&m_Speedo, SPEEDO_MIN, SPEEDO_MAX, 240);

	setProgressMeterBounds(&m_Tacho, TACHO_MIN, TACHO_MAX, 240);

	setProgressMeterBounds(&m_AirFuelRatio, AIR_FUEL_RATIO_MIN, AIR_FUEL_RATIO_MAX, 80);

	setProgressMeterBounds(&m_Water, WATER_MIN, WATER_MAX, 80);

	setProgressMeterBounds(&m_Volt, BAT_VOLT_MIN, BAT_VOLT_MAX, 80);

	setProgressMeterBounds(&m_Spark, SPARK_MIN, SPARK_MAX, 80);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CDashBoardDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	CTTPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
		switch (nCtlColor)
	{
	//Edit controls need white background and black text
	//Note the 'return hbr' which is needed to draw the Edit
	//control's internal background (as opposed to text background)
/*	case CTLCOLOR_EDIT:
		pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkColor(RGB(255,255,255));
		return hbr; */
	//Static controls need white text and same background as m_brush
	case CTLCOLOR_STATIC:
		LOGBRUSH logbrush;
		m_brush.GetLogBrush( &logbrush );
		pDC->SetTextColor(RGB(255,255,255));
		pDC->SetBkColor(logbrush.lbColor);
		return m_brush;
/*	//For listboxes, scrollbars, buttons, messageboxes and dialogs,
	//use the new brush (m_brush)
	case CTLCOLOR_LISTBOX:
	case CTLCOLOR_SCROLLBAR:
	case CTLCOLOR_BTN:
	case CTLCOLOR_MSGBOX: */
	case CTLCOLOR_DLG:
		return m_brush;
	//This shouldn't occurr since we took all the cases, but
	//JUST IN CASE, return the new brush
	default:
		return m_brush;
	}
}
