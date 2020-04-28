// FreeScan.h : main header file for the FreeScan application
//

#if !defined(AFX_FreeScan_H__DC786428_3F99_11D3_983E_00E018900F2A__INCLUDED_)
#define AFX_FreeScan_H__DC786428_3F99_11D3_983E_00E018900F2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseDefines.h"

#include <afxwin.h>

//class	CStatusDlg;

/////////////////////////////////////////////////////////////////////////////
// CFreeScanApp:
// See FreeScan.cpp for the implementation of this class
//

class CFreeScanApp : public CWinApp
{
public:
	CFreeScanApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFreeScanApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CFreeScanApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FreeScan_H__DC786428_3F99_11D3_983E_00E018900F2A__INCLUDED_)
