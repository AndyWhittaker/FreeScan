#if !defined(AFX_TCODESDLG_H__E0E7EDF0_4DB7_11D3_8DE5_00E018900F2A__INCLUDED_)
#define AFX_TCODESDLG_H__E0E7EDF0_4DB7_11D3_8DE5_00E018900F2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TCodesDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTCodesDlg dialog
class CFreeScanDlg;
class CSupervisor;

class CTCodesDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CTCodesDlg)

// Construction
public:
	CTCodesDlg();
	~CTCodesDlg();

// Dialog Data
	//{{AFX_DATA(CTCodesDlg)
	enum { IDD = IDD_TCODES };
	CButton	m_ResetDTC;
	CListBox	m_TList;
	//}}AFX_DATA

	CFreeScanDlg*	m_pMainDlg; // Base Dialog Pointer.

	void Refresh(void);
protected:
	BOOL GetInteract(void);// Returns if the ECU is interactive

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTCodesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CSupervisor* GetSupervisor(void); // returns a pointer to the Supervisor
	CSupervisor* GetData(void); // return a pointer to the Data
	void FillListBox(CListBox& lbT, CString& csT);// Populates a ListBox with a CString
	// Generated message map functions
	//{{AFX_MSG(CTCodesDlg)
	afx_msg void OnResetdtc();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TCODESDLG_H__E0E7EDF0_4DB7_11D3_8DE5_00E018900F2A__INCLUDED_)
