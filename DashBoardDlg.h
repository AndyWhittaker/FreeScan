
#if !defined(AFX_DashBoardDlg_H__CB945454_6752_11D3_1234_0080C83832F8__INCLUDED_)
#define AFX_DashBoardDlg_H__CB945664_6752_11D3_1234_0080C83832F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseDefines.h"

#include <afxwin.h>
#include <afxext.h>

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>          // MFC support for Internet Explorer 4 Common Controls
#endif

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                    // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "TTPropertyPage.h" // Our Tooltip Class
#include "EcuData.h"
#include "SupervisorInterface.h"

/////////////////////////////////////////////////////////////////////////////
// CDashBoardDlg dialog
class CFreeScanDlg;

class CDashBoardDlg : public CTTPropertyPage
{
	DECLARE_DYNCREATE(CDashBoardDlg)

// Construction
public:
	CDashBoardDlg();
	~CDashBoardDlg();

// Dialog Data
private:
	//{{AFX_DATA(CDashBoardDlg)
	enum { IDD = IDD_DASHBOARD };
	CProgressCtrl	m_Throttle;
	CProgressCtrl	m_EngineLoad;
	CProgressCtrl	m_Boost;
	CEdit			m_BoostText;
	CProgressCtrl	m_MAT;
	CEdit			m_MATText;
	CProgressCtrl	m_Speedo;
	CEdit			m_SpeedoMphText;
	CEdit			m_SpeedoKphText;
	CProgressCtrl	m_Tacho;
	CEdit			m_TachoText;
	CProgressCtrl	m_AirFuelRatio;
	CEdit			m_AirFuelRatioText;
	CProgressCtrl	m_Water;
	CEdit			m_WaterText;
	CProgressCtrl	m_Volt;
	CEdit			m_VoltText;
	CProgressCtrl	m_Spark;
	CEdit			m_SparkText;
	//}}AFX_DATA

	CBrush m_brush; // For our background colour

	CSupervisorInterface* m_pSupervisor;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDashBoardDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void Refresh(const CEcuData* const ecuData); // Updates all of our controls
	void RegisterSupervisor(CSupervisorInterface* const pSupervisor);

private:
	// Generated message map functions
	//{{AFX_MSG(CDashBoardDlg)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DashBoardDlg_H__CB945454_6752_11D3_9867_0080C83832F8__INCLUDED_)
