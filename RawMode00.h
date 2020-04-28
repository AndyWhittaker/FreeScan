#if !defined(AFX_RAWMODE00_H__3323A341_40DD_11D3_983E_00E018900F2A__INCLUDED_)
#define AFX_RAWMODE00_H__3323A341_40DD_11D3_983E_00E018900F2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseDefines.h"

#include <afxwin.h>
#include <afxdlgs.h>

#include "EcuData.h"
#include "Resource.h"
#include "SupervisorInterface.h"

/////////////////////////////////////////////////////////////////////////////
// CRawMode00 dialog

class CRawMode00 : public CPropertyPage
{
	DECLARE_DYNCREATE(CRawMode00)

// Construction
public:
	CRawMode00();
	~CRawMode00();

// Dialog Data
private:
	//{{AFX_DATA(CRawMode00)
	enum { IDD = IDD_RAWHEX_0 };
	CEdit	m_0a_db02;
	CEdit	m_0a_db01;
	CEdit	m_0a_db00;
	CEdit	m_05_db09;
	CEdit	m_05_db08;
	CEdit	m_05_db07;
	CEdit	m_05_db06;
	CEdit	m_05_db05;
	CEdit	m_05_db04;
	CEdit	m_05_db03;
	CEdit	m_05_db02;
	CEdit	m_05_db01;
	CEdit	m_05_db00;
	//}}AFX_DATA

	CSupervisorInterface* m_pSupervisor;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CRawMode00)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void Refresh(const CEcuData* const ecuData);
	void RegisterSupervisor(CSupervisorInterface* const pSupervisor);

private:
	// Generated message map functions
	//{{AFX_MSG(CRawMode00)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RAWMODE00_H__3323A341_40DD_11D3_983E_00E018900F2A__INCLUDED_)
