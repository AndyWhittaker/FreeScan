#if !defined(AFX_ADVANCEDDLG_H__CB945664_6752_11D3_1234_0080C83832F8__INCLUDED_)
#define AFX_ADVANCEDDLG_H__CB945664_6752_11D3_1234_0080C83832F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdvancedDlg.h : header file
//

#include "TTPropertyPage.h" // Our Tooltip Class

/////////////////////////////////////////////////////////////////////////////
// CAdvancedDlg dialog
class CFreeScanDlg;
class CSupervisor;

class CAdvancedDlg : public CTTPropertyPage
{
	DECLARE_DYNCREATE(CAdvancedDlg)

// Construction
public:
	CAdvancedDlg();
	~CAdvancedDlg();

// Dialog Data
	//{{AFX_DATA(CAdvancedDlg)
	enum { IDD = IDD_ADVANCED };
	CButton	m_SetDesIdle;
	CButton	m_ResetIAC;
	CButton	m_ResetBLM;
	CEdit	m_DesiredIdle;
	//}}AFX_DATA

	CFreeScanDlg*	m_pMainDlg; // Base Dialog Pointer.

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAdvancedDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void Refresh(void); // Updates all of our controls
protected:
	CSupervisor* GetSupervisor(void); // returns a pointer to the Supervisor
	CSupervisor* GetData(void); // return a pointer to the Data
	BOOL GetInteract(void);
	DWORD GetCurrentMode(void); // Returns the current ECU Mode

	// Generated message map functions
	//{{AFX_MSG(CAdvancedDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSetdesidle();
	afx_msg void OnResetBLM();
	afx_msg void OnResetIAC();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADVANCEDDLG_H__CB945664_6752_11D3_9867_0080C83832F8__INCLUDED_)
