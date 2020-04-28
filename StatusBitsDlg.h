#if !defined(AFX_StatusBitsDLG_H__E0E7EDF0_4DB7_11D3_8DE5_00E018900F2A__INCLUDED_)
#define AFX_StatusBitsDLG_H__E0E7EDF0_4DB7_11D3_8DE5_00E018900F2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseDefines.h"

#include <afxwin.h>
#include <afxdlgs.h>

#include "EcuData.h"
#include "SupervisorInterface.h"
#include "Led.h"
#include "resource.h"

class CStatusBitsDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CStatusBitsDlg)

// Construction
public:
	CStatusBitsDlg();
	~CStatusBitsDlg();

// Dialog Data
private:
	//{{AFX_DATA(CStatusBitsDlg)
	enum { IDD = IDD_STATUSBITS };
	CLed	m_led3;
	CLed	m_led2;
	CLed	m_led1;
	//}}AFX_DATA

	CSupervisorInterface* m_pSupervisor;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CStatusBitsDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void Refresh(const CEcuData* const ecuData);
	void RegisterSupervisor(CSupervisorInterface* const pSupervisor);

// Implementation
private:
	// Generated message map functions
	//{{AFX_MSG(CStatusBitsDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_StatusBitsDLG_H__E0E7EDF0_4DB7_11D3_8DE5_00E018900F2A__INCLUDED_)
