#if !defined(AFX_GMA160Protocol_H__79716CC6_4280_11D3_983E_00E018900F2A__INCLUDED_)
#define AFX_GMA160Protocol_H__79716CC6_4280_11D3_983E_00E018900F2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GMA160Protocol.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGMA160Protocol window

#include "GMA160Parser.h"

#include "..\SerialPort.h"

#define ECU_HEADER_GMA160				0xf0
#define ECU_CHATTER_HEADER_GMA160		0xf0

class CGMA160Protocol : public CGMA160Parser
{
// Construction
public:
	CGMA160Protocol();
	virtual ~CGMA160Protocol();

// Attributes
public:
	CString			m_csComment; // The developer's comment for this protocol
	
protected:
	CSerialPort*	m_pcom; // Our com port object pointer of the Supervisor
	DWORD			m_dwCurrentMode; // Current ECU mode
	DWORD			m_dwRequestedMode; // Mode we want next
	unsigned char	m_ucData; // data for ECU, e.g. Desired Idle
	BOOL			m_bModeDone; // Have we sent our mode request?
	int				m_iIdleCount;
	int				m_iShutUpCount;
	BOOL			m_bFirstRead; // First time reading ECU. Synchonises F0 byte
	BOOL			m_bReadHeader;
	int				m_iLen; // length of data packet
	int				m_iDataCount; // Current data index
	BOOL			m_bReadLength;
	BOOL			m_bReadData;
	BOOL			m_bReadCRC;
	BOOL			m_bInteract; // for sending mode requests to ECU
	BOOL			m_bSentOnce; // test for send
	unsigned char	m_ucBuffer[400]; // Packet Temporary Buffer

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGMA160Protocol)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL
//	virtual BOOL Create(CWnd* pParentWnd);

// Implementation
public:
	HWND Init(CSupervisor* pSupervisor, CSerialPort* pcom, CWnd* pParentWnd, CStatusDlg* pStatusDlg);// Initialises GMA160Protocol

protected:
	void PumpMessages(void);
	BOOL SendIdle(void); // Sends idle message to get bus
	BOOL SendModeShutUp(void); // Tell ECU to stop chattering packets
	BOOL SendMode1_0(void); // Tell ECU to send Mode 1 message 0 packets
	BOOL ReceiveDTCs(void); // Ask for the trouble codes
	BOOL ClearDTCs(void); // Clear the trouble codes
	BOOL ClearBLM(void); // Clear the BLM
	BOOL ResetIAC(void); // Resets the IAC (Idle Control Valve)
	BOOL SetDesiredIdle(unsigned char DesIdle); // sets the desired idle
	void SendNextCommand(void); // Sends the required command to ECU
	int HandleTX(unsigned char* buffer, int iLength);
	void WriteToECU(unsigned char* string,int stringlength, BOOL bDelay = TRUE);
	BOOL CreateProtocolWnd(CWnd* pParentWnd);

	void OnIdle(void);
	void OnMode1(void);
	void OnModeD1(void);
	void OnMode3(void);
	void OnModeD3(void);
	void OnMode4(void);
	void OnModeD4(void);
	void OnMode10(void);
	// Generated message map functions
protected:
	//{{AFX_MSG(CGMA160Protocol)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg LONG OnResetStateMachine(WPARAM wdummy, LPARAM dummy);
	afx_msg LONG OnInteract(WPARAM bInteract, LPARAM dummy);
	afx_msg LONG OnGetInteract(WPARAM wdummy, LPARAM dummy);
	afx_msg LONG OnGetCurrentMode(WPARAM wdummy, LPARAM dummy);
	afx_msg LONG OnForceShutUp(WPARAM wdummy, LPARAM dummy);
	afx_msg LONG OnECUMode(WPARAM dwMode, LPARAM Data = 0);
	afx_msg LONG OnStartCSV(WPARAM bStart, LPARAM dummy);
	afx_msg LONG OnCharReceived(WPARAM ch, LPARAM port);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GMA160Protocol_H__79716CC6_4280_11D3_983E_00E018900F2A__INCLUDED_)
