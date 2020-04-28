#if !defined(AFX_SUPERVISOR_H__79716CC6_4280_11D3_983E_00E018900F2A__INCLUDED_)
#define AFX_SUPERVISOR_H__79716CC6_4280_11D3_983E_00E018900F2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseDefines.h"

#include <afxwin.h>

#include "SerialPort.h"
#include "EcuData.h"
#include "StatusWriter.h"
#include "BaseProtocol.h"
#include "MainDlg.h"

// : public CObject
class CSupervisor : public CSupervisorInterface
{
// Construction
public:
	CSupervisor(CFreeScanDlg* MainDlg, CStatusWriter* pStatusDlg);
	virtual ~CSupervisor();

// Attributes
public:
	BOOL			m_bCentigrade; // Is FreeScan in Metric or Imperial?
	BOOL			m_bMiles; // Is FreeScan in Metric or Imperial?
	BOOL			m_bInteract; // allow sending mode requests to ECU

	CFreeScanDlg*	m_pMainDlg;
	CStatusWriter*	m_pStatusDlg;

private:
	CBaseProtocol*	m_pProtocol;
	CSerialPort*	m_pCom;// Our com port object pointer
	DWORD			m_dwBytesSent;
	DWORD			m_dwBytesReceived;
	BOOL			m_bHasRun; // Has the com port been started?

	CStdioFile		m_file;// File class for logging to disk
	CString			m_csCSVLogFile;// Filename for CSV logging
	DWORD			m_dwCSVRecord;//CSV record number

	CWnd*			m_pParentWnd; // for SetCurrentPort()
	int				m_iModel; // for SetCurrentPort()

	CEcuData*					m_pEcuData;

// Implementation
public:
	void Init(int iModel);// Initialises the Supervisor and Protocol

	void ConvertDegrees(void); // converts all temps to degF
	void ConvertMiles(void); // converts speeds to kph
	void SetCurrentPort(UINT nPort);// Sets current com port
	UINT GetCurrentPort(void);// Returns the current com port
	DWORD GetWriteDelay(void);//Gets the Write Delay from the serial port
	void SetWriteDelay(DWORD nDelay);//Sets the Write Delay to the serial port
	BOOL Start(void);// Starts the ECU monitoring
	BOOL Stop(void);
	BOOL ShutDown(void);
	BOOL StartCSVLog(BOOL bStart);// Starts or stops csv logging to file
	void Deallocate(void); // Deallocates the models we don't want monitor.
	void Interact(BOOL bInteract);// Requests whether FreeScan talks to the ECU or not
	BOOL GetInteract(void);// Gets the interact status
	void Centigrade(BOOL bUnit);
	void Miles(BOOL bUnit);
	BOOL GetCentigrade(void);
	BOOL GetMiles(void);
	DWORD GetCurrentMode(void);// Returns the current ECU mode
	void ECUMode(DWORD dwMode, const unsigned char data);// Changes the ECU mode number
	DWORD GetReceivedBytes();
	DWORD GetSentBytes();
	void IncreaseSentBytes(const DWORD additionalBytesSent);

	void ForceDataFromECU(void);
	const CEcuData *const GetEcuData(void);

	void Test(void); // Sends a packet to the current parser for testing

private:
	void WriteCSV(BOOL bTitle);//Write CSV data to disk
	BOOL CreateProtocolWnd(CWnd* pParentWnd);

	void WriteStatus(const CString csText);
	void WriteStatusLogged(const CString csText);

	void PumpMessages(void);

	// Generated message map functions
protected:
	//{{AFX_MSG(CGM1992PontiacProtocol)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg LONG OnCharReceived(WPARAM ch, LPARAM port);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUPERVISOR_H__79716CC6_4280_11D3_983E_00E018900F2A__INCLUDED_)
