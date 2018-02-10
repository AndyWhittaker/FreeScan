// SensorDlg.cpp : implementation file
//
// (c) 1996-99 Andy Whittaker, Chester, England. 
// mail@andywhittaker.com

#include "stdafx.h"
#include "FreeScan.h"
#include "MainDlg.h"
#include "Supervisor.h"

#include "SensorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSensorDlg property page

IMPLEMENT_DYNCREATE(CSensorDlg, CTTPropertyPage)

CSensorDlg::CSensorDlg() : CTTPropertyPage(CSensorDlg::IDD)
{
	//{{AFX_DATA_INIT(CSensorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pMainDlg = NULL;
}

CSensorDlg::~CSensorDlg()
{
}

void CSensorDlg::DoDataExchange(CDataExchange* pDX)
{
	CTTPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSensorDlg)
	DDX_Control(pDX, IDC_THROTTLEADC, m_ThrottleADC);
	DDX_Control(pDX, IDC_MATADC, m_MATADC);
	DDX_Control(pDX, IDC_BAROADC, m_BaroADC);
	DDX_Control(pDX, IDC_MAPADC, m_MapADC);
	DDX_Control(pDX, IDC_COOLANTADC, m_CoolantADC);
	DDX_Control(pDX, IDC_BAROVOLTS, m_BaroVolts);
	DDX_Control(pDX, IDC_BAROMETER, m_Baro);
	DDX_Control(pDX, IDC_MAPVOLTS, m_MapVolts);
	DDX_Control(pDX, IDC_MAP, m_Boost);
	DDX_Control(pDX, IDC_COOLANTVOTS, m_CoolantVolts);
	DDX_Control(pDX, IDC_THROTTLEVOLTS, m_ThrottleVolts);
	DDX_Control(pDX, IDC_THROTTLEPOS, m_ThrottlePos);
	DDX_Control(pDX, IDC_MATVOLTS, m_MATVolts);
	DDX_Control(pDX, IDC_MAT, m_MAT);
	DDX_Control(pDX, IDC_COOLANT, m_CoolantTemp);
	//}}AFX_DATA_MAP

	//Updates the dialog.
	Refresh();
}

// Returns a pointer to the Supervisor
CSupervisor* CSensorDlg::GetSupervisor(void)
{
	return m_pMainDlg->m_pSupervisor;
}

// Returns a pointer to the Supervisor
CSupervisor* CSensorDlg::GetData(void)
{
	return m_pMainDlg->m_pSupervisor;
}

// Returns if the ECU is interactive
BOOL CSensorDlg::GetInteract(void)
{
	return GetSupervisor()->GetInteract();
}

// Returns the current ECU Mode
DWORD CSensorDlg::GetCurrentMode(void)
{
	return GetSupervisor()->GetCurrentMode();
}

// Updates all of our controls
void CSensorDlg::Refresh(void)
{
	CString buf;
	DWORD	dwCurrentMode = GetCurrentMode();

	if (!( (dwCurrentMode == 1) ||
		(dwCurrentMode == 0) ))
		buf.Format("N/A");
	else
		buf.Format("%3.1f", GetData()->m_fWaterTemp);
	m_CoolantTemp.SetWindowText(buf);

	if (dwCurrentMode != 1)
		buf.Format("N/A");
	else
		buf.Format("%4.2f", GetData()->m_fWaterVolts);
	m_CoolantVolts.SetWindowText(buf);

	if (dwCurrentMode != 1)
		buf.Format("N/A");
	else
		buf.Format("0x%02X", GetData()->m_iWaterTempADC);
	m_CoolantADC.SetWindowText(buf);

	if (dwCurrentMode != 1)
		buf.Format("N/A");
	else
		buf.Format("%3.1f", GetData()->m_fMATTemp);
	m_MAT.SetWindowText(buf);

	if (dwCurrentMode != 1)
		buf.Format("N/A");
	else
		buf.Format("%4.2f", GetData()->m_fMATVolts);
	m_MATVolts.SetWindowText(buf);

	if (dwCurrentMode != 1)
		buf.Format("N/A");
	else
		buf.Format("0x%02X", GetData()->m_iMATADC);
	m_MATADC.SetWindowText(buf);

	if (dwCurrentMode != 1)
		buf.Format("N/A");
	else
		buf.Format("%3d", GetData()->m_iThrottlePos);
	m_ThrottlePos.SetWindowText(buf);

	if (dwCurrentMode != 1)
		buf.Format("N/A");
	else
		buf.Format("%4.2f", GetData()->m_fThrottleVolts);
	m_ThrottleVolts.SetWindowText(buf);

	if (dwCurrentMode != 1)
		buf.Format("N/A");
	else
		buf.Format("0x%02X", GetData()->m_iThrottleADC);
	m_ThrottleADC.SetWindowText(buf);

	if (dwCurrentMode != 1)
		buf.Format("N/A");
	else
		buf.Format("%5.3f", GetData()->m_fBaro);
	m_Baro.SetWindowText(buf);

	if (dwCurrentMode != 1)
		buf.Format("N/A");
	else
		buf.Format("%4.2f", GetData()->m_fBaroVolts);
	m_BaroVolts.SetWindowText(buf);

	if (dwCurrentMode != 1)
		buf.Format("N/A");
	else
		buf.Format("0x%02X", GetData()->m_iBaroADC);
	m_BaroADC.SetWindowText(buf);

	if (dwCurrentMode != 1)
		buf.Format("N/A");
	else
		buf.Format("%5.3f", GetData()->m_fMAP);
	m_Boost.SetWindowText(buf);

	if (dwCurrentMode != 1)
		buf.Format("N/A");
	else
		buf.Format("%4.2f", GetData()->m_fMAPVolts);
	m_MapVolts.SetWindowText(buf);

	if (dwCurrentMode != 1)
		buf.Format("N/A");
	else
		buf.Format("0x%02X", GetData()->m_iMAPADC);
	m_MapADC.SetWindowText(buf);
}

BEGIN_MESSAGE_MAP(CSensorDlg, CTTPropertyPage)
	//{{AFX_MSG_MAP(CSensorDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSensorDlg message handlers

BOOL CSensorDlg::OnInitDialog() 
{
	CTTPropertyPage::OnInitDialog();
	
	// Add dialog items that want ToolTip text
//	m_toolTip.AddTool( GetDlgItem(IDC_EPROMID), IDC_EPROMID);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
