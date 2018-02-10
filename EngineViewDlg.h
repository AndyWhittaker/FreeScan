#if !defined(AFX_EngineViewDLG_H__3323A342_40DD_11D3_983E_00E018900F2A__INCLUDED_)
#define AFX_EngineViewDLG_H__3323A342_40DD_11D3_983E_00E018900F2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EngineViewDlg.h : header file
//
#include "TTPropertyPage.h" // Our Tooltip Class

/////////////////////////////////////////////////////////////////////////////
// CEngineViewDlg dialog
class CFreeScanDlg;
class CSupervisor;

class CEngineViewDlg : public CTTPropertyPage
{
	DECLARE_DYNCREATE(CEngineViewDlg)

// Construction
public:
	CEngineViewDlg();
	~CEngineViewDlg();

// Dialog Data
	//{{AFX_DATA(CEngineViewDlg)
	enum { IDD = IDD_ENGINEVIEW };
	CStatic	m_view;
	//}}AFX_DATA

	CFreeScanDlg*	m_pMainDlg; // Base Dialog Pointer.
	BOOL			m_bOneO2;	// True if only one O2 sensor

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CEngineViewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void Refresh(void);

protected:
	CSupervisor* GetSupervisor(void); // returns a pointer to the Supervisor
	CSupervisor* GetData(void); // return a pointer to the Data
	DWORD GetCurrentMode(void);

	// Generated message map functions
	//{{AFX_MSG(CEngineViewDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EngineViewDLG_H__3323A342_40DD_11D3_983E_00E018900F2A__INCLUDED_)
