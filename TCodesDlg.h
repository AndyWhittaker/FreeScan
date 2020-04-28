#if !defined(AFX_TCODESDLG_H__E0E7EDF0_4DB7_11D3_8DE5_00E018900F2A__INCLUDED_)
#define AFX_TCODESDLG_H__E0E7EDF0_4DB7_11D3_8DE5_00E018900F2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseDefines.h"

#include <afxwin.h>
#include <afxdlgs.h>

#include "EcuData.h"
#include "resource.h"
#include "SupervisorInterface.h"

/////////////////////////////////////////////////////////////////////////////
// CTCodesDlg dialog

class CTCodesDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CTCodesDlg)

// Construction
public:
	CTCodesDlg();
	~CTCodesDlg();

// Dialog Data
private:
	//{{AFX_DATA(CTCodesDlg)
	enum { IDD = IDD_TCODES };
	CButton	m_ResetDTC;
	CListBox	m_TList;
	//}}AFX_DATA

	CSupervisorInterface* m_pSupervisor;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTCodesDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void Refresh(const CEcuData* const ecuData);
	void RegisterSupervisor(CSupervisorInterface* const pSupervisor);

// Implementation
private:
	void FillListBox(CListBox& lbT, const CString& csT);// Populates a ListBox with a CString
	// Generated message map functions
	//{{AFX_MSG(CTCodesDlg)
	afx_msg void OnResetdtc();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TCODESDLG_H__E0E7EDF0_4DB7_11D3_8DE5_00E018900F2A__INCLUDED_)
