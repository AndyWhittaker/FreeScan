#if !defined(AFX_SUPERVISOR_H__79716CC6_4280_11D3_983E_00E018900F2A__INCLUDED_)
#define AFX_SUPERVISOR_H__79716CC6_4280_11D3_983E_00E018900F2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Supervisor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSupervisor window

#include "SerialPort.h"

class CFreeScanDlg;
class CStatusDlg;

// Protocols installed:
// Add what extra protocols you've included here:
class CELM327Protocol;
class CEspritProtocol;
class CElanProtocol;
class CLotusCarltonProtocol;
class CGM1994CamaroZ28Protocol;
class CGM1993CamaroZ28Protocol;
class CGM1992PontiacProtocol;
class CGM1989CorvetteProtocol;
class CGMA140Protocol;
class CGMA143Protocol;
class CGMA160Protocol;
// You'll need to do a specific include in the .cpp file as well.

// For every protocol, these must be unique IDs
// These are the messages the user sends to the protocol class
#define WM_PROT_CMD_RESETSTATE		WM_USER+110	// Resets the protocol state machine.
#define WM_PROT_CMD_SETINTERACT		WM_USER+111	// Set the interact mode.
#define WM_PROT_CMD_GETINTERACT		WM_USER+112	// Gets the interact mode.
#define WM_PROT_CMD_ECUMODE			WM_USER+113	// Set the next ECU mode.
#define WM_PROT_CMD_GETECUMODE		WM_USER+114	// Get the current ECU mode
#define WM_PROT_CMD_STARTCSV		WM_USER+115	// Starts csv logging
#define WM_PROT_CMD_FORCESHUTUP		WM_USER+116	// Forces the FreeScan to transmit

// These are defines for the ECU message requests.
// For function ECUMode(...)
#define	ECU_GET_DTCS				100
#define ECU_CLEAR_DTCS				101
#define ECU_CLEAR_BLM				102
#define ECU_RESET_IAC				103
#define ECU_SET_DES_IDLE			104
// : public CObject
class CSupervisor
{
// Construction
public:
	CSupervisor(CFreeScanDlg* MainDlg = NULL, CStatusDlg* pStatusDlg = NULL);

// Attributes
public:
	DWORD		m_dwBytesSent;
	DWORD		m_dwBytesReceived;
	CString		m_csProtocolComment; // public copy of the current protocol's comments
	BOOL		m_bCentigrade; // Is FreeScan in Metric or Imperial?
	BOOL		m_bMiles; // Is FreeScan in Metric or Imperial?

	CFreeScanDlg*		m_pMainDlg;
	CStatusDlg*			m_pStatusDlg;

protected:
//	Our protocols go here:
	CELM327Protocol*			m_pELM327Protocol;
	CEspritProtocol*			m_pEspritProtocol;
	CElanProtocol*				m_pElanProtocol;
	CLotusCarltonProtocol*		m_pLotusCarltonProtocol;
	CGM1994CamaroZ28Protocol*	m_pGM1994CamaroZ28Protocol;
	CGM1993CamaroZ28Protocol*	m_pGM1993CamaroZ28Protocol;
	CGM1992PontiacProtocol*		m_pGM1992PontiacProtocol;
	CGM1989CorvetteProtocol*	m_pGM1989CorvetteProtocol;
	CGMA140Protocol*			m_pGMA140Protocol;
	CGMA143Protocol*			m_pGMA143Protocol;
	CGMA160Protocol*			m_pGMA160Protocol;
	CSerialPort*				m_pCom;// Our com port object pointer

	HWND	m_ProtocolWnd;// Handle of the protocol window
	CWnd*	m_pParentWnd; // for SetCurrentPort()
	int		m_iModel; // for SetCurrentPort()

// Data that has been read-in and parsed from the ECU.
public:
	BOOL	m_bEngineClosedLoop; //
	BOOL	m_bEngineStalled; //

	BOOL	m_bACRequest; // bit 0
	BOOL	m_bACClutch; // bit 2

	// Raw Data : just a store so that we can debug
	unsigned char*	m_ucF005;
	unsigned char*	m_ucF00A;
	unsigned char*	m_ucF001; // Mode 1 data buffer
	unsigned char*	m_ucF002; // Mode 2 data buffer
	unsigned char*	m_ucF003; // Mode 3 data buffer
	unsigned char*	m_ucF004; // Mode 4 data buffer

	// Parsed Data Store
	CString			m_csDTC; // Fault Descriptions

	float	m_fBatteryVolts; // in V
	int		m_iRPM;
	int		m_iIACPosition; // Idle Stepper Motor Position 1 - 170
	int		m_iDesiredIdle;
	int		m_iMPH;
	float	m_fStartWaterTemp; // Coolant Temperature when engine was started Deg C
	float	m_fWaterTemp; // Coolant Temperature Deg C
	float	m_fWaterVolts; // Coolant Temperature Sensor Voltage
	int		m_iWaterTempADC; // Coolant Temperature A/D Converter
	float	m_fOilTemp; // Oil Temperature Deg C
	float	m_fMATTemp; // Manifold Air Temperature Deg C
	float	m_fMATVolts; // Manifold Air Temperature Sensor Voltage
	int 	m_iMATADC; // Manifold Air Temperature A/D Converter
	int		m_iEpromID;
	int		m_iRunTime; // in seconds
	int		m_iCrankSensors; // 0, 1 or 2
	int		m_iThrottlePos;
	float	m_fThrottleVolts; // Sensor Voltage
	int		m_iThrottleADC; // A/D Converter
	int		m_iEngineLoad;
	float	m_fBaro; // Air pressure in Bar absolute
	float	m_fBaroVolts; // Air pressure in Volts
	int		m_iBaroADC; // Air pressure in A/D Converter
	float	m_fMAP; // in Bar absolute
	float	m_fMAPVolts; // in Volts
	int		m_iMAPADC; // A/D Converter
	int		m_iBoostPW; // Pulse-width (duty cycle) of the turbo boost controller
	int		m_iCanisterDC; // Duty Cycle of Charcoal Cansister controller
	int		m_iSecondaryInjPW; // Pulse-width of the secondary injectors
	int		m_iInjectorBasePWMsL; // Injector Opening Time in Ms Left
	int		m_iInjectorBasePWMsR; // Injector Opening Time in Ms Right
	float	m_fAFRatio; // Air Fuel Ratio
	float	m_fAirFlow; // Air Flow
	float	m_fO2VoltsLeft; //O2 sensor volts left
	float	m_fO2VoltsRight; //O2 sensor volts right (if fitted)
	int		m_iIntegratorL; // Integrator Value Left
	int		m_iIntegratorR; // Integrator Value Right
	int		m_iBLM;	// Contents of the current BLM Cell
	int		m_iBLMRight;	// Contents of the current BLM Cell
	int		m_iBLMCell; // Current BLM Cell
	int		m_iRichLeanCounterL; // Rich/Lean Counter Left
	int		m_iRichLeanCounterR; // Rich/Lean Counter Right
	float	m_fSparkAdvance;
	float	m_fKnockRetard;
	int		m_iKnockCount;

// Implementation
public:
	virtual ~CSupervisor();
	void WriteStatus(CString csText);
	void Init(CWnd* pParentWnd, int iModel);// Initialises the Supervisor and Protocol
	void UpdateDialog(void);// Updates the dialogs because of a data change
	void ConvertDegrees(void); // converts all temps to degF
	void ConvertMiles(void); // converts speeds to kph
	void SetCurrentPort(UINT nPort);// Sets current com port
	UINT GetCurrentPort(void);// Returns the current com port
	DWORD GetWriteDelay(void);//Gets the Write Delay from the serial port
	void SetWriteDelay(DWORD nDelay);//Sets the Write Delay to the serial port
	BOOL Start(void);// Starts the ECU monitoring
	BOOL Restart(void);
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
	void ECUMode(DWORD dwMode, unsigned char Data = 0);// Changes the ECU mode number
	void ForceShutUp(void); // Forces the protocol to send a shut-up command to the ECU.
	void Test(void); // Sends a packet to the current parser for testing
protected:
	void ResetVariables(void);
	void PumpMessages(void);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUPERVISOR_H__79716CC6_4280_11D3_983E_00E018900F2A__INCLUDED_)
