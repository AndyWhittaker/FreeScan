#if !defined(AFX_ENGINEDLG_H__3323A342_40DD_11D3_983E_00E018900F2A__INCLUDED_)
#define AFX_ENGINEDLG_H__3323A342_40DD_11D3_983E_00E018900F2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseDefines.h"

#include "TTPropertyPage.h" // Our Tooltip Class
#include "EcuData.h"
#include "SupervisorInterface.h"

/////////////////////////////////////////////////////////////////////////////
// CEngineDlg dialog

class CEngineDlg : public CTTPropertyPage
{
	DECLARE_DYNCREATE(CEngineDlg)

// Construction
public:
	CEngineDlg();
	~CEngineDlg();

// Dialog Data
private:
	//{{AFX_DATA(CEngineDlg)
	enum { IDD = IDD_ENGINEDATA };
	CEdit	m_Knock_Count;
	CEdit	m_BLM_Number;
	CEdit	m_BLM;
	CEdit	m_OilTemp;
	CEdit	m_IntegratorR;
	CEdit	m_IntegratorL;
	CEdit	m_RichLeanCounterR;
	CEdit	m_RichLeanCounterL;
	CEdit	m_AirFlow;
	CEdit	m_O2VoltsRight;
	CEdit	m_O2VoltsLeft;
	CEdit	m_AirFuel;
	CEdit	m_StartCoolant;
	CEdit	m_Baro;
	CEdit	m_MAT;
	CEdit	m_IAC;
	CEdit	m_EngineLoad;
	CEdit	m_ACClutch;
	CEdit	m_ACRequest;
	CEdit	m_CrankSensors;
	CEdit	m_DesiredIdle;
	CEdit	m_SparkAdv;
	CEdit	m_Knock_Retard;
	CEdit	m_status7;
	CEdit	m_status6;
	CEdit	m_ThrottlePos;
	CEdit	m_RunTime;
	CEdit	m_EpromID;
	CEdit	m_BatteryVolts;
	CEdit	m_RPM;
	CEdit	m_MPH;
	CEdit	m_Boost;
	CEdit	m_CoolantTemp;
	//}}AFX_DATA

	CSupervisorInterface* m_pSupervisor;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CEngineDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void Refresh(const CEcuData* const ecuData);
	void RegisterSupervisor(CSupervisorInterface* const pSupervisor);

private:
	// Generated message map functions
	//{{AFX_MSG(CEngineDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENGINEDLG_H__3323A342_40DD_11D3_983E_00E018900F2A__INCLUDED_)
