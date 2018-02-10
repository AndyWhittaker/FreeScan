#if !defined(AFX_RAWMODE01_H__50F853A3_5572_11D3_9841_0080C83832F8__INCLUDED_)
#define AFX_RAWMODE01_H__50F853A3_5572_11D3_9841_0080C83832F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RawMode01.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRawMode01 dialog
class CFreeScanDlg;
class CSupervisor;

class CRawMode01 : public CPropertyPage
{
	DECLARE_DYNCREATE(CRawMode01)

// Construction
public:
	CRawMode01();
	~CRawMode01();

// Dialog Data
	//{{AFX_DATA(CRawMode01)
	enum { IDD = IDD_RAWHEX_1 };
	CEdit	m_db07;
	CEdit	m_db06;
	CEdit	m_db05;
	CEdit	m_db04;
	CEdit	m_db03;
	CEdit	m_db02;
	CEdit	m_db01;
	CEdit	m_db00;
	//}}AFX_DATA

	CFreeScanDlg*	m_pMainDlg; // Base Dialog Pointer.

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CRawMode01)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
		void Refresh(void);

protected:
	CSupervisor* GetSupervisor(void); // returns a pointer to the Supervisor
	// Generated message map functions
	//{{AFX_MSG(CRawMode01)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RAWMODE01_H__50F853A3_5572_11D3_9841_0080C83832F8__INCLUDED_)
