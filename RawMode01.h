#if !defined(AFX_RAWMODE01_H__50F853A3_5572_11D3_9841_0080C83832F8__INCLUDED_)
#define AFX_RAWMODE01_H__50F853A3_5572_11D3_9841_0080C83832F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseDefines.h"

#include <afxwin.h>
#include <afxdlgs.h>

#include "EcuData.h"
#include "SupervisorInterface.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CRawMode01 dialog

class CRawMode01 : public CPropertyPage
{
	DECLARE_DYNCREATE(CRawMode01)

// Construction
public:
	CRawMode01();
	~CRawMode01();

// Dialog Data
private:
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

	CSupervisorInterface* m_pSupervisor;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CRawMode01)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void Refresh(const CEcuData* const ecuData);
	void RegisterSupervisor(CSupervisorInterface* const pSupervisor);

private:
	// Generated message map functions
	//{{AFX_MSG(CRawMode01)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RAWMODE01_H__50F853A3_5572_11D3_9841_0080C83832F8__INCLUDED_)
