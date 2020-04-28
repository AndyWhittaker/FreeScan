// MainDlg.h : header file
//

#if !defined(AFX_MAINDLG_H__DC78642A_3F99_11D3_983E_00E018900F2A__INCLUDED_)
#define AFX_MAINDLG_H__DC78642A_3F99_11D3_983E_00E018900F2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include "BaseDefines.h"

#include <afxwin.h>

// Property Pages
#include "DetailDlg.h"
#include "StatusDlg.h"
#include "EngineViewDlg.h" // New client view
#include "StatusBitsDlg.h" // Binary Status Bits
#include "DashBoardDlg.h" // Pretty graphics
#include "TCodesDlg.h" // Trouble Codes
#include "SensorDlg.h" // Raw ADC Sensor Values
#include "AdvancedDlg.h" // Advanced ECU things like Desired Idle
#include "About.h"
#ifdef _DEBUG
	#include "EngineDlg.h" // Real world values of the Engine
	#include "RawMode00.h" // Hex Dump of Mode 0 Dlg
	#include "RawMode01.h" // Hex Dump of Mode 1 Dlg
#endif // _DEBUG
#include "EcuData.h"
#include "SupervisorInterface.h"

/////////////////////////////////////////////////////////////////////////////
// CFreeScanDlg dialog

class CFreeScanDlg : public CPropertySheet
{
	DECLARE_DYNAMIC(CFreeScanDlg);

// Construction
public:
	CFreeScanDlg(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CFreeScanDlg(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CFreeScanDlg();

private:
	void AllocateAndAddPages(void);
	void SetLogoFont(CString Name, int nHeight = 24, int nWeight = FW_BOLD,
		BYTE bItalic = true, BYTE bUnderline = false);
	void SetLogoText(CString Text);

	//Attributes
	CStatusDlg*				m_pStatusDlg; // This is the "Spy" window
	CSupervisorInterface*	m_pSupervisor; // This fathoms out the data-stream

	//Classes for our property pages
	CDetailDlg*		m_pDetailDlg;
	CEngineViewDlg*	m_pEngineViewDlg;
	CStatusBitsDlg*	m_pStatusBitsDlg;
	CDashBoardDlg*	m_pDashBoardDlg;
	CTCodesDlg*		m_pTCodesDlg;
	CSensorDlg*		m_pSensorDlg;
	CAdvancedDlg*	m_pAdvancedDlg;
	CAbout*			m_pAbout;
#ifdef _DEBUG
	CEngineDlg*		m_pEngineDlg;
	CRawMode00*		m_pRawMode00Dlg;
	CRawMode01*		m_pRawMode01Dlg;
#endif // _DEBUG

public:
	// Implementation
	void Update(const CEcuData* const ecuData); //Updates the dialog

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFreeScanDlg)
	//}}AFX_VIRTUAL

// Implementation
private:
	HICON		m_hIcon;
	CFont 		m_fontLogo; // Font for our logo
	CString		m_LogoText; // Text for our logo

	void WriteStatus(CString csText);
	void WriteASCII(unsigned char * buffer, int ilength);
	void WriteLogEntry(LPCTSTR pstrFormat, ...);
	void WriteStatusLogged(CString csText);

	// Generated message map functions
	//{{AFX_MSG(CFreeScanDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__DC78642A_3F99_11D3_983E_00E018900F2A__INCLUDED_)
