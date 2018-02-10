/*
**	FILENAME			CSerialPort.h
**
**	PURPOSE 			This class can read, write and watch one serial port.
**						It sends messages to its owner when something happends on the port
**						The class creates a thread for reading and writing so the main
**						program is not blocked.
**
**	CREATION DATE		15-09-1997
**	LAST MODIFICATION	24-11-1998
**
**	AUTHOR				Remon Spekreijse
**	MODIFIED BY			Brian Koh Sze Hsian
**
*/

#ifndef __SERIALPORT_H__
#define __SERIALPORT_H__

#define WM_COMM_BREAK_DETECTED		WM_USER+1	// A break was detected on input.
#define WM_COMM_CTS_DETECTED		WM_USER+2	// The CTS (clear-to-send) signal changed state. 
#define WM_COMM_DSR_DETECTED		WM_USER+3	// The DSR (data-set-ready) signal changed state. 
#define WM_COMM_ERR_DETECTED		WM_USER+4	// A line-status error occurred. Line-status errors are CE_FRAME, CE_OVERRUN, and CE_RXPARITY. 
#define WM_COMM_RING_DETECTED		WM_USER+5	// A ring indicator was detected. 
#define WM_COMM_RLSD_DETECTED		WM_USER+6	// The RLSD (receive-line-signal-detect) signal changed state. 
#define WM_COMM_RXCHAR				WM_USER+7	// A character was received and placed in the input buffer. 
#define WM_COMM_RXFLAG_DETECTED		WM_USER+8	// The event character was received and placed in the input buffer.  
#define WM_COMM_TXEMPTY_DETECTED	WM_USER+9	// The last character in the output buffer was sent. 

#define MAX_WRITE_BUFFER			512 // Maximum size of the write buffer
#define MAXBYTESTOREAD				512 // Maximum size of read buffer

////// Serial port exception class ////////////////////////////////////////////

void AfxThrowSerialException(DWORD dwError = 0);

class CSerialException : public CException
{
public:
//Constructors / Destructors
	CSerialException(DWORD dwError);
	~CSerialException();

//Methods
#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual BOOL GetErrorMessage(LPTSTR lpstrError, UINT nMaxError,	PUINT pnHelpContext = NULL);
	CString GetErrorMessage();

//Data members
	DWORD m_dwError;

protected:
	DECLARE_DYNAMIC(CSerialException)
};

class CSerialPort : public CObject
{													     
   
public:
	// contruction and destruction
	CSerialPort();
	virtual 	~CSerialPort();

	// port initialisation										     
  
	BOOL InitPort(CWnd* pPortOwner, UINT portnr = 1, UINT baud = 8192, char parity = 'N',
		UINT databits = 8, UINT stopsbits = 1, DWORD dwCommEvents = EV_RXCHAR, UINT nBufferSize = MAX_WRITE_BUFFER);

	BOOL InitPort(CWnd* pPortOwner, UINT baud, char parity,UINT databits, 
		UINT stopsbits, DWORD dwCommEvents, UINT nBufferSize);

	// start/stop comm watching
	BOOL		StartMonitoring(); //Starts the comm port monitor
	BOOL		RestartMonitoring(); //Restarts the comm port monitor
	BOOL		StopMonitoring(); //Stops the comm port monitor

	DWORD		GetWriteBufferSize();// Returns the output buffer size
	DWORD		GetCommEvents();// Returns the communication event masks
	DCB			GetDCB();// Returns the device control block
	UINT		GetPort(void);//Returns the current com port number
	void		SetPort(UINT nPort);//Sets the current com port number
	DWORD		GetWriteDelay(void);//Returns write delay in milliseconds
	void		SetWriteDelay(DWORD nDelay);//Sets the write delay

	void		WriteToPort(unsigned char* string,int stringlength, BOOL bDelay = TRUE);
	void		WriteToPort(unsigned char* string);
#ifdef _DEBUG
  void CSerialPort::Dump(CDumpContext& dc) const;
#endif

protected:
	// protected memberfunctions
	void		ProcessErrorMessage(char* ErrorText);
	static UINT	CommThread(LPVOID pParam);
	static void	ReceiveChar(CSerialPort* port, COMSTAT comstat);
	static void	WriteChar(CSerialPort* port);

	// thread
	CWinThread*			m_Thread;

	// synchronisation objects
static CRITICAL_SECTION	m_csCommunicationSync;
	BOOL				m_bThreadAlive;

	// handles
	HANDLE				m_hShutdownEvent;
	HANDLE				m_hComm;

	// Event array. 
	// One element is used for each event. There are two event handles for each port.
	// A Write event and a receive character event 
	// which is located in the overlapped structure (m_ov.hEvent).
	// There is a general shutdown when the port is closed. 
	// Mod: arw - we now send immediately. WriteChar is not handled by the thread.
	HANDLE				m_hEventArray[2];

	// structures
static	OVERLAPPED		m_ov;
	COMMTIMEOUTS		m_CommTimeouts;
	DCB					m_dcb;

	// owner window
	CWnd*				m_pOwner;

	// misc
	UINT				m_nPortNr;
	unsigned char*		m_szWriteBuffer; // Write buffer
	DWORD				m_dwCommEvents;
	DWORD				m_nWriteBufferSize;
static int				m_nActualWriteBufferSize;
	DWORD				m_nWriteDelay; // milliseconds that write waits before write

	DECLARE_DYNAMIC(CSerialPort)
};

#endif __SERIALPORT_H__
