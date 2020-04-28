#if !defined(AFX_DESIDLEDLG_H__6DE481BD_8382_4D13_9ACF_0E966422D192__INCLUDED_)
#define AFX_DESIDLEDLG_H__6DE481BD_8382_4D13_9ACF_0E966422D192__INCLUDED_

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

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CDesIdleDlg dialog

class CDesIdleDlg : public CDialog
{
// Construction
public:
	CDesIdleDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDesIdleDlg)
	enum { IDD = IDD_DESIDLE };
	CSpinButtonCtrl	m_Spin;
	UINT	m_Value;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDesIdleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDesIdleDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESIDLEDLG_H__6DE481BD_8382_4D13_9ACF_0E966422D192__INCLUDED_)
