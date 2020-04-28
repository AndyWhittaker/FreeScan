// EngineDlg.cpp : implementation file
//
// (c) 1996-99 Andy Whittaker, Chester, England. 
// mail@andywhittaker.com
//

#include "EngineDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEngineDlg property page

IMPLEMENT_DYNCREATE(CEngineDlg, CTTPropertyPage)

CEngineDlg::CEngineDlg() : CTTPropertyPage(CEngineDlg::IDD)
{
	//{{AFX_DATA_INIT(CEngineDlg)
	//}}AFX_DATA_INIT
	m_pSupervisor = NULL;
}

CEngineDlg::~CEngineDlg()
{
}

void CEngineDlg::DoDataExchange(CDataExchange* pDX)
{
	CTTPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEngineDlg)
	DDX_Control(pDX, IDC_KNOCKCOUNT, m_Knock_Count);
	DDX_Control(pDX, IDC_BLM_NUMBER, m_BLM_Number);
	DDX_Control(pDX, IDC_BLM, m_BLM);
	DDX_Control(pDX, IDC_OILTEMP, m_OilTemp);
	DDX_Control(pDX, IDC_INTR, m_IntegratorR);
	DDX_Control(pDX, IDC_INTL, m_IntegratorL);
	DDX_Control(pDX, IDC_RLCOUNTR, m_RichLeanCounterR);
	DDX_Control(pDX, IDC_RLCOUNTL, m_RichLeanCounterL);
	DDX_Control(pDX, IDC_AIRFLOW, m_AirFlow);
	DDX_Control(pDX, IDC_O2VOLTSR, m_O2VoltsRight);
	DDX_Control(pDX, IDC_O2VOLTSL, m_O2VoltsLeft);
	DDX_Control(pDX, IDC_AIRFUEL, m_AirFuel);
	DDX_Control(pDX, IDC_STARTCOOLANT, m_StartCoolant);
	DDX_Control(pDX, IDC_BARO, m_Baro);
	DDX_Control(pDX, IDC_MAT, m_MAT);
	DDX_Control(pDX, IDC_IAC, m_IAC);
	DDX_Control(pDX, IDC_ENGINELOAD, m_EngineLoad);
	DDX_Control(pDX, IDC_STATUS2, m_ACClutch);
	DDX_Control(pDX, IDC_STATUS0, m_ACRequest);
	DDX_Control(pDX, IDC_CRANK, m_CrankSensors);
	DDX_Control(pDX, IDC_DESIDLE, m_DesiredIdle);
	DDX_Control(pDX, IDC_SPARKADV, m_SparkAdv);
	DDX_Control(pDX, IDC_KNOCK, m_Knock_Retard);
	DDX_Control(pDX, IDC_STATUS7, m_status7);
	DDX_Control(pDX, IDC_STATUS6, m_status6);
	DDX_Control(pDX, IDC_THROTTLEPOS, m_ThrottlePos);
	DDX_Control(pDX, IDC_RUNTIME, m_RunTime);
	DDX_Control(pDX, IDC_EPROMID, m_EpromID);
	DDX_Control(pDX, IDC_BATVOLTS, m_BatteryVolts);
	DDX_Control(pDX, IDC_RPM, m_RPM);
	DDX_Control(pDX, IDC_MPH, m_MPH);
	DDX_Control(pDX, IDC_BOOST, m_Boost);
	DDX_Control(pDX, IDC_COOLANT, m_CoolantTemp);
	//}}AFX_DATA_MAP

	//Updates the dialog.
	if (m_pSupervisor != NULL) {
		Refresh(m_pSupervisor->GetEcuData());
	}
}

static inline void updateField(DWORD dwCurrentMode, CEdit * const textBox, const char *const textFormat, const int iValue) {
	if (dwCurrentMode != 1 || !CEcuData::isValid(iValue)) {
		textBox->SetWindowText("N/A");
	}
	else {
		CString buf;
		buf.Format(textFormat, iValue);
		textBox->SetWindowText(buf);
	}
}

static inline void updateField(DWORD dwCurrentMode, CEdit * const textBox, const char *const textFormat, const float fValue) {
	if (dwCurrentMode != 1 || !CEcuData::isValid(fValue)) {
		textBox->SetWindowText("N/A");
	}
	else {
		CString buf;
		buf.Format(textFormat, fValue);
		textBox->SetWindowText(buf);
	}
}

static inline void updateBoolField(DWORD dwCurrentMode, CEdit* const textBox, const char* const textTrue, const char* const textFalse, const BOOL bValue) {
	if (dwCurrentMode != 1 || !CEcuData::isValid(bValue)) {
		textBox->SetWindowText("N/A");
	}
	else {
		CString buf;
		if (bValue == TRUE) {
			buf = textTrue;
		}
		else {
			buf = textFalse;
		}
		textBox->SetWindowText(buf);
	}
}

// Updates all of our controls
void CEngineDlg::Refresh(const CEcuData* const ecuData)
{
	DWORD	dwCurrentMode = m_pSupervisor->GetCurrentMode();

	updateBoolField(dwCurrentMode, &m_status7, "Closed Loop", "Fixed Map", ecuData->m_bEngineClosedLoop);

	updateBoolField(dwCurrentMode, &m_status6, "No Ref Pulses", "Running", ecuData->m_bEngineStalled);

	updateBoolField(dwCurrentMode, &m_ACRequest, "A/C Requested", "A/C Off", ecuData->m_bACRequest);

	updateBoolField(dwCurrentMode, &m_ACClutch, "A/C Clutch ON", "A/C Clutch OFF", ecuData->m_bACClutch);

	updateField(dwCurrentMode, &m_EpromID, "%04X", ecuData->m_iEpromID);

	updateField(dwCurrentMode, &m_RPM, "%d", ecuData->m_iRPM);

	updateField(dwCurrentMode, &m_IAC, "%d", ecuData->m_iIACPosition);

	updateField(dwCurrentMode, &m_DesiredIdle, "%d", ecuData->m_iDesiredIdle);

	if (m_pSupervisor->GetMiles() == TRUE) {
		updateField(dwCurrentMode, &m_MPH, "%d", ecuData->m_iMPH);
	}
	else {
		updateField(dwCurrentMode, &m_MPH, "%d", ecuData->m_iMPH_inKPH);
	}

	updateField(dwCurrentMode, &m_BatteryVolts, "%3.1f", ecuData->m_fBatteryVolts);

	if (m_pSupervisor->GetCentigrade() == TRUE) {
		updateField(dwCurrentMode, &m_StartCoolant, "%3.1f", ecuData->m_fStartWaterTemp);

		updateField(dwCurrentMode, &m_CoolantTemp, "%3.1f", ecuData->m_fWaterTemp);

		updateField(dwCurrentMode, &m_OilTemp, "%3.1f", ecuData->m_fOilTemp);

		updateField(dwCurrentMode, &m_MAT, "%3.1f", ecuData->m_fMATTemp);
	}
	else {
		updateField(dwCurrentMode, &m_StartCoolant, "%3.1f", ecuData->m_fStartWaterTemp_inF);

		updateField(dwCurrentMode, &m_CoolantTemp, "%3.1f", ecuData->m_fWaterTemp_inF);

		updateField(dwCurrentMode, &m_OilTemp, "%3.1f", ecuData->m_fOilTemp_inF);

		updateField(dwCurrentMode, &m_MAT, "%3.1f", ecuData->m_fMATTemp_inF);
	}

	updateField(dwCurrentMode, &m_RunTime, "%d", ecuData->m_iRunTime);

	updateField(dwCurrentMode, &m_CrankSensors, "%d", ecuData->m_iCrankSensors);

	updateField(dwCurrentMode, &m_ThrottlePos, "%3d", ecuData->m_iThrottlePos);

	updateField(dwCurrentMode, &m_EngineLoad, "%3d", ecuData->m_iEngineLoad);

	updateField(dwCurrentMode, &m_Baro, "%5.3f", ecuData->m_fBaro);

	updateField(dwCurrentMode, &m_Boost, "%5.3f", ecuData->m_fMAP);

	updateField(dwCurrentMode, &m_SparkAdv, "%3.1f", ecuData->m_fSparkAdvance);

	updateField(dwCurrentMode, &m_Knock_Retard, "%3.1f", ecuData->m_fKnockRetard);

	updateField(dwCurrentMode, &m_Knock_Count, "%3d", ecuData->m_iKnockCount);

	updateField(dwCurrentMode, &m_O2VoltsLeft, "%5.3f", ecuData->m_fO2VoltsLeft);
	updateField(dwCurrentMode, &m_O2VoltsRight, "%5.3f", ecuData->m_fO2VoltsRight);

	updateField(dwCurrentMode, &m_IntegratorL, "%d", ecuData->m_iIntegratorL);
	updateField(dwCurrentMode, &m_IntegratorR, "%d", ecuData->m_iIntegratorR);

	updateField(dwCurrentMode, &m_RichLeanCounterL, "%d", ecuData->m_iRichLeanCounterL);
	updateField(dwCurrentMode, &m_RichLeanCounterR, "%d", ecuData->m_iRichLeanCounterR);

	updateField(dwCurrentMode, &m_AirFuel, "%3.1f", ecuData->m_fAFRatio);

	updateField(dwCurrentMode, &m_BLM, "%3d", ecuData->m_iBLM);

	updateField(dwCurrentMode, &m_BLM_Number, "%3d", ecuData->m_iBLMCell);

	updateField(dwCurrentMode, &m_AirFlow, "%4.0f", ecuData->m_fAirFlow);
}

void CEngineDlg::RegisterSupervisor(CSupervisorInterface* const pSupervisor) {
	m_pSupervisor = pSupervisor;
}

BEGIN_MESSAGE_MAP(CEngineDlg, CTTPropertyPage)
	//{{AFX_MSG_MAP(CEngineDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEngineDlg message handlers

BOOL CEngineDlg::OnInitDialog() 
{
	CTTPropertyPage::OnInitDialog();
	
	// Add dialog items that want ToolTip text
	m_toolTip.AddTool( GetDlgItem(IDC_EPROMID), IDC_EPROMID);
	m_toolTip.AddTool( GetDlgItem(IDC_IAC), IDC_IAC);
	m_toolTip.AddTool( GetDlgItem(IDC_STARTCOOLANT), IDC_STARTCOOLANT);
	m_toolTip.AddTool( GetDlgItem(IDC_COOLANT), IDC_COOLANT);
	m_toolTip.AddTool( GetDlgItem(IDC_BOOST), IDC_BOOST);
	m_toolTip.AddTool( GetDlgItem(IDC_BATVOLTS), IDC_BATVOLTS);
	m_toolTip.AddTool( GetDlgItem(IDC_RUNTIME), IDC_RUNTIME);
	m_toolTip.AddTool( GetDlgItem(IDC_THROTTLEPOS), IDC_THROTTLEPOS);
	m_toolTip.AddTool( GetDlgItem(IDC_BARO), IDC_BARO);
	m_toolTip.AddTool( GetDlgItem(IDC_MPH), IDC_MPH);
	m_toolTip.AddTool( GetDlgItem(IDC_CRANK), IDC_CRANK);
	m_toolTip.AddTool( GetDlgItem(IDC_MAT), IDC_MAT);
	m_toolTip.AddTool( GetDlgItem(IDC_O2VOLTSL), IDC_O2VOLTS);
	m_toolTip.AddTool( GetDlgItem(IDC_O2VOLTSR), IDC_O2VOLTS);
	m_toolTip.AddTool( GetDlgItem(IDC_OILTEMP), IDC_OILTEMP);
	m_toolTip.AddTool( GetDlgItem(IDC_RLCOUNTL), IDC_RLCOUNT);
	m_toolTip.AddTool( GetDlgItem(IDC_RLCOUNTR), IDC_RLCOUNT);
	m_toolTip.AddTool( GetDlgItem(IDC_INTL), IDC_INT);
	m_toolTip.AddTool( GetDlgItem(IDC_INTR), IDC_INT);
	m_toolTip.AddTool( GetDlgItem(IDC_DESIDLE), IDC_DESIDLE);
	m_toolTip.AddTool( GetDlgItem(IDC_KNOCK), IDC_KNOCK);
	m_toolTip.AddTool( GetDlgItem(IDC_KNOCKCOUNT), IDS_KNOCKCOUNT);
	m_toolTip.AddTool( GetDlgItem(IDC_RPM), IDC_RPM);
	m_toolTip.AddTool( GetDlgItem(IDC_SPARKADV), IDC_SPARKADV);
	m_toolTip.AddTool( GetDlgItem(IDC_ENGINELOAD), IDC_ENGINELOAD);
	m_toolTip.AddTool( GetDlgItem(IDC_AIRFUEL), IDC_AIRFUEL);
	m_toolTip.AddTool( GetDlgItem(IDC_AIRFLOW), IDC_AIRFLOW);
	m_toolTip.AddTool( GetDlgItem(IDC_BLM_NUMBER), IDC_BLM_NUMBER);
	m_toolTip.AddTool( GetDlgItem(IDC_BLM), IDC_BLM);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
