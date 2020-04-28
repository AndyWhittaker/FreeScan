// SensorDlg.cpp : implementation file
//
// (c) 1996-99 Andy Whittaker, Chester, England. 
// mail@andywhittaker.com

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
	m_pSupervisor = NULL;
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

// Updates all of our controls
void CSensorDlg::Refresh(const CEcuData* const ecuData)
{
	DWORD	dwCurrentMode = m_pSupervisor->GetCurrentMode();

	if (m_pSupervisor->GetCentigrade() == TRUE) {
		updateField(dwCurrentMode, &m_CoolantTemp, "%3.1f", ecuData->m_fWaterTemp);
		updateField(dwCurrentMode, &m_MAT, "%3.1f", ecuData->m_fMATTemp);
	}
	else {
		updateField(dwCurrentMode, &m_CoolantTemp, "%3.1f", ecuData->m_fWaterTemp_inF);
		updateField(dwCurrentMode, &m_MAT, "%3.1f", ecuData->m_fMATTemp_inF);
	}
	updateField(dwCurrentMode, &m_ThrottlePos, "%3d", ecuData->m_iThrottlePos);
	updateField(dwCurrentMode, &m_Baro, "%5.3f", ecuData->m_fBaro);
	updateField(dwCurrentMode, &m_Boost, "%5.3f", ecuData->m_fMAP);

	updateField(dwCurrentMode, &m_CoolantVolts, "%4.2f", ecuData->m_fWaterVolts);
	updateField(dwCurrentMode, &m_MATVolts, "%4.2f", ecuData->m_fMATVolts);
	updateField(dwCurrentMode, &m_ThrottleVolts, "%4.2f", ecuData->m_fThrottleVolts);
	updateField(dwCurrentMode, &m_BaroVolts, "%4.2f", ecuData->m_fBaroVolts);
	updateField(dwCurrentMode, &m_MapVolts, "%4.2f", ecuData->m_fMAPVolts);

	updateField(dwCurrentMode, &m_CoolantADC, "0x%02X", ecuData->m_iWaterTempADC);
	updateField(dwCurrentMode, &m_MATADC, "0x%02X", ecuData->m_iMATADC);
	updateField(dwCurrentMode, &m_ThrottleADC, "0x%02X", ecuData->m_iThrottleADC);
	updateField(dwCurrentMode, &m_BaroADC, "0x%02X", ecuData->m_iBaroADC);
	updateField(dwCurrentMode, &m_MapADC, "0x%02X", ecuData->m_iMAPADC);
}

void CSensorDlg::RegisterSupervisor(CSupervisorInterface* const pSupervisor) {
	m_pSupervisor = pSupervisor;
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
