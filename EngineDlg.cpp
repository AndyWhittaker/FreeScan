// EngineDlg.cpp : implementation file
//
// (c) 1996-99 Andy Whittaker, Chester, England. 
// mail@andywhittaker.com
//

#include "stdafx.h"
#include "FreeScan.h"
#include "MainDlg.h"
#include "Supervisor.h"

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
	m_pMainDlg = NULL;
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
	Refresh();
}

// Returns a pointer to the Supervisor
CSupervisor* CEngineDlg::GetSupervisor(void)
{
	return m_pMainDlg->m_pSupervisor;
}

// Returns a pointer to the Supervisor
CSupervisor* CEngineDlg::GetData(void)
{
	return m_pMainDlg->m_pSupervisor;
}

// Returns the current ECU Mode
DWORD CEngineDlg::GetCurrentMode(void)
{
	return GetSupervisor()->GetCurrentMode();
}

// Updates all of our controls
void CEngineDlg::Refresh(void)
{
	CString buf;
	DWORD	dwCurrentMode = GetCurrentMode();

	if (!( (dwCurrentMode == 1) ||
		(dwCurrentMode == 0) ))
		m_status7.SetWindowText("??");
	else if (GetData()->m_bEngineClosedLoop)
		m_status7.SetWindowText("Closed Loop");
	else
		m_status7.SetWindowText("Open Loop");

	if (!( (dwCurrentMode == 1) ||
		(dwCurrentMode == 0) ))
		m_status6.SetWindowText("N/A");
	else if (GetData()->m_bEngineStalled)
		m_status6.SetWindowText("No Ref Pulses");
	else
		m_status6.SetWindowText("Running");

	if (dwCurrentMode != 1)
		m_ACRequest.SetWindowText("N/A");
	else if (GetData()->m_bACRequest)
		m_ACRequest.SetWindowText("A/C Requested");
	else
		m_ACRequest.SetWindowText("A/C Off");

	if (dwCurrentMode != 1)
		m_ACClutch.SetWindowText("N/A");
	else if (GetData()->m_bACClutch)
		m_ACClutch.SetWindowText("A/C Clutch ON");
	else
		m_ACClutch.SetWindowText("A/C Clutch OFF");

	if (dwCurrentMode != 1)
		buf.Format("N/A");
	else
		buf.Format("%04X", GetData()->m_iEpromID);
	m_EpromID.SetWindowText(buf);

	if (!( (dwCurrentMode == 1) ||
		(dwCurrentMode == 0) ))
		buf.Format("N/A");
	else
		buf.Format("%d", GetData()->m_iRPM);
	m_RPM.SetWindowText(buf);

	if (dwCurrentMode != 1)
		buf.Format("N/A");
	else
		buf.Format("%d", GetData()->m_iIACPosition);
	m_IAC.SetWindowText(buf);

	if (dwCurrentMode != 1)
		buf.Format("N/A");
	else
		buf.Format("%d", GetData()->m_iDesiredIdle);
	m_DesiredIdle.SetWindowText(buf);

	if (!( (dwCurrentMode == 1) ||
		(dwCurrentMode == 0) ))
		buf.Format("N/A");
	else
		buf.Format("%d", GetData()->m_iMPH);
	m_MPH.SetWindowText(buf);

	if (!( (dwCurrentMode == 1) ||
		(dwCurrentMode == 0) ))
		buf.Format("N/A");
	else
		buf.Format("%3.1f", GetData()->m_fBatteryVolts);
	m_BatteryVolts.SetWindowText(buf);

	if (!( (dwCurrentMode == 1) ||
		(dwCurrentMode == 0) ))
		buf.Format("N/A");
	else if (GetData()->m_fStartWaterTemp != 0.0)
		buf.Format("%3.1f", GetData()->m_fStartWaterTemp);
	else
		buf.Format(" ");
	m_StartCoolant.SetWindowText(buf);

	if (!( (dwCurrentMode == 1) ||
		(dwCurrentMode == 0) ))
		buf.Format("N/A");
	else
		buf.Format("%3.1f", GetData()->m_fWaterTemp);
	m_CoolantTemp.SetWindowText(buf);

	if (dwCurrentMode != 1)
		buf.Format("N/A");
	else if (GetData()->m_fMATTemp != 0.0)
		buf.Format("%3.1f", GetData()->m_fMATTemp);
	else
		buf.Format(" ");
	m_MAT.SetWindowText(buf);

	if (dwCurrentMode != 1)
		buf.Format("N/A");
	else
		buf.Format("%d", GetData()->m_iRunTime);
	m_RunTime.SetWindowText(buf);

	if (dwCurrentMode != 1)
		buf.Format("N/A");
	else
		buf.Format("%d", GetData()->m_iCrankSensors);
	m_CrankSensors.SetWindowText(buf);

	if (dwCurrentMode != 1)
		buf.Format("N/A");
	else
		buf.Format("%3d", GetData()->m_iThrottlePos);
	m_ThrottlePos.SetWindowText(buf);

	if (dwCurrentMode != 1)
		buf.Format("N/A");
	else if (GetData()->m_iEngineLoad != 0)
		buf.Format("%3d", GetData()->m_iEngineLoad);
	else
		buf.Format(" ");
	m_EngineLoad.SetWindowText(buf);

	if (dwCurrentMode != 1)
		buf.Format("N/A");
	else if (GetData()->m_fBaro != 0.0)
		buf.Format("%5.3f", GetData()->m_fBaro);
	else
		buf.Format(" ");
	m_Baro.SetWindowText(buf);

	if (dwCurrentMode != 1)
		buf.Format("N/A");
	else if (GetData()->m_fMAP != 0.0)
		buf.Format("%5.3f", GetData()->m_fMAP);
	else
		buf.Format(" ");
	m_Boost.SetWindowText(buf);

	if (dwCurrentMode != 1)
		buf.Format("N/A");
	else
		buf.Format("%3.1f", GetData()->m_fSparkAdvance);
	m_SparkAdv.SetWindowText(buf);

	if (dwCurrentMode != 1)
		buf.Format("N/A");
	else if (GetData()->m_fKnockRetard != 0.0)
		buf.Format("%3.1f", GetData()->m_fKnockRetard);
	else
		buf.Format(" ");
	m_Knock_Retard.SetWindowText(buf);

	if (dwCurrentMode != 1)
		buf.Format("N/A");
	else if (GetData()->m_iKnockCount != 0)
		buf.Format("%3d", GetData()->m_iKnockCount);
	else
		buf.Format(" ");
	m_Knock_Count.SetWindowText(buf);

	if (dwCurrentMode != 1)
		buf.Format("N/A");
	else
		buf.Format("%5.3f", GetData()->m_fO2VoltsLeft);
	m_O2VoltsLeft.SetWindowText(buf);

	if (dwCurrentMode != 1)
		buf.Format("N/A");
	else if (GetData()->m_fO2VoltsRight != 0.0)
		buf.Format("%5.3f", GetData()->m_fO2VoltsRight);
	else
		buf.Format(" ");
	m_O2VoltsRight.SetWindowText(buf);

	if (dwCurrentMode != 1)
		buf.Format("N/A");
	else if (GetData()->m_iIntegratorL != 0)
		buf.Format("%d", GetData()->m_iIntegratorL);
	else
		buf.Format(" ");
	m_IntegratorL.SetWindowText(buf);

	if (dwCurrentMode != 1)
		buf.Format("N/A");
	else if (GetData()->m_iIntegratorR != 0)
		buf.Format("%d", GetData()->m_iIntegratorR);
	else
		buf.Format(" ");
	m_IntegratorR.SetWindowText(buf);

	if (dwCurrentMode != 1)
		buf.Format("N/A");
	else if (GetData()->m_iRichLeanCounterL != 0)
		buf.Format("%d", GetData()->m_iRichLeanCounterL);
	else
		buf.Format(" ");
	m_RichLeanCounterL.SetWindowText(buf);

	if (dwCurrentMode != 1)
		buf.Format("N/A");
	else if (GetData()->m_iRichLeanCounterR != 0)
		buf.Format("%d", GetData()->m_iRichLeanCounterR);
	else
		buf.Format(" ");
	m_RichLeanCounterR.SetWindowText(buf);

	if (dwCurrentMode != 1)
		buf.Format("N/A");
	else if (GetData()->m_fAFRatio != 0.0)
		buf.Format("%3.1f", GetData()->m_fAFRatio);
	else
		buf.Format(" ");
	m_AirFuel.SetWindowText(buf);

	if (dwCurrentMode != 1)
		buf.Format("N/A");
	else if (GetData()->m_iBLM != 0)
		buf.Format("%3d", GetData()->m_iBLM);
	else
		buf.Format(" ");
	m_BLM.SetWindowText(buf);

	if (dwCurrentMode != 1)
		buf.Format("N/A");
	else if (GetData()->m_iBLMCell != 0)
		buf.Format("%3d", GetData()->m_iBLMCell);
	else
		buf.Format(" ");
	m_BLM_Number.SetWindowText(buf);

	if (dwCurrentMode != 1)
		buf.Format("N/A");
	else if (GetData()->m_fAirFlow != 0.0)
		buf.Format("%4.0f", GetData()->m_fAirFlow);
	else
		buf.Format(" ");
	m_AirFlow.SetWindowText(buf);
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
