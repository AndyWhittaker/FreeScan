// MainDlg.h : header file
//

#if !defined(AFX_MAINDLG_H__DC78642A_3F99_11D3_983E_00E018900F2A__INCLUDED_)
#define AFX_MAINDLG_H__DC78642A_3F99_11D3_983E_00E018900F2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Property Pages
#include "DetailDlg.h"
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

/////////////////////////////////////////////////////////////////////////////
// CFreeScanDlg dialog

class CSupervisor;
class CStatusDlg;
class CFreeScanDlg : public CPropertySheet
{
	DECLARE_DYNAMIC(CFreeScanDlg);

// Construction
public:
	CFreeScanDlg(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CFreeScanDlg(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CFreeScanDlg();
protected:
	void AllocateAndAddPages(void);
	void SetLogoFont(CString Name, int nHeight = 24, int nWeight = FW_BOLD,
		BYTE bItalic = true, BYTE bUnderline = false);
	void SetLogoText(CString Text);

public:
	// pointer to the app for our global data
	CFreeScanApp* m_pApp;

	//Attributes
	CStatusDlg*		m_pStatusDlg; // This is the "Spy" window
	CSupervisor*	m_pSupervisor; // This phathoms out the data-stream
	BOOL			m_bHasRun; // Has the supervisor ran at least once?

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

	// Implementation
	void UpdateDialog(void); //Updates the dialog
	BOOL StartComs(void); //Starts the ECU Comms
	BOOL StopComs(void); //Stops the ECU Comms
	void WriteStatus(CString csText);
	void WriteASCII(unsigned char * buffer, int ilength);
	void WriteLogEntry(LPCTSTR pstrFormat, ...);
	void WriteStatusLogged(CString csText);
	BOOL StartLog(BOOL bStart);// Starts or stops logging to file
	BOOL StartCSVLog(BOOL bStart);// Starts or stops csv logging to file

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFreeScanDlg)
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON		m_hIcon;
	CFont 		m_fontLogo; // Font for our logo
	CString		m_LogoText; // Text for our logo

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
