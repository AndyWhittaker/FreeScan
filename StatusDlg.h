#if !defined(AFX_STATUSDLG_H__06F52203_27C3_11D2_983E_00E018900F2A__INCLUDED_)
#define AFX_STATUSDLG_H__06F52203_27C3_11D2_983E_00E018900F2A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// StatusDlg.h : header file
//

#include "resource.h"

#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatusDlg dialog

class CStatusDlg : public CDialog
{
// Construction
public:
	CStatusDlg(CWnd* pParent = NULL);   // standard constructor
	~CStatusDlg();   // standard destructor

	void WriteStatus(CString csText);
	void WriteStatusLogged(CString csText);
	void WriteASCII(unsigned char * buffer, int ilength);
	void WriteLogEntry(LPCTSTR pstrFormat, ...);
	BOOL StartLog(BOOL bStart);
	void Hide(BOOL yes);
	BOOL HideStatus(void);
	void PumpMessages(void);
	void PrintRect(LPRECT lpR);

// Dialog Data
	//{{AFX_DATA(CStatusDlg)
	enum { IDD = IDD_STATUS };
	CEdit	m_Time;
	CListBox	m_Status;
	//}}AFX_DATA

protected:
	CTime		m_now; // Put the current date and time into the log file
	CStdioFile	m_file; // File class for logging to disk
	CString		m_csLogFile; // Filename for logging
	BOOL		m_hidden; // Is status windows visible?
	RECT		m_WindowPos; // Stores the postion of the Window

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatusDlg)
	public:
	virtual BOOL OnInitDialog();
	virtual BOOL Create(CWnd* pParentWnd);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStatusDlg)
	afx_msg void OnMove(int x, int y);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATUSDLG_H__06F52203_27C3_11D2_983E_00E018900F2A__INCLUDED_)
