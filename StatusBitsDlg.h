#if !defined(AFX_StatusBitsDLG_H__E0E7EDF0_4DB7_11D3_8DE5_00E018900F2A__INCLUDED_)
#define AFX_StatusBitsDLG_H__E0E7EDF0_4DB7_11D3_8DE5_00E018900F2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatusBitsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatusBitsDlg dialog
class CFreeScanDlg;
class CSupervisor;

#include "Led.h"

class CStatusBitsDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CStatusBitsDlg)

// Construction
public:
	CStatusBitsDlg();
	~CStatusBitsDlg();

// Dialog Data
	//{{AFX_DATA(CStatusBitsDlg)
	enum { IDD = IDD_STATUSBITS };
	CLed	m_led3;
	CLed	m_led2;
	CLed	m_led1;
	//}}AFX_DATA

	CFreeScanDlg*	m_pMainDlg; // Base Dialog Pointer.

	void Refresh(void);
protected:
	BOOL GetInteract(void);// Returns if the ECU is interactive

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CStatusBitsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CSupervisor* GetSupervisor(void); // returns a pointer to the Supervisor
	CSupervisor* GetData(void); // return a pointer to the Data
	// Generated message map functions
	//{{AFX_MSG(CStatusBitsDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_StatusBitsDLG_H__E0E7EDF0_4DB7_11D3_8DE5_00E018900F2A__INCLUDED_)
