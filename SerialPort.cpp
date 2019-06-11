/*
**	FILENAME			CSerialPort.cpp
**
**	PURPOSE 			This class can read, write and watch one serial port.
**						It sends messages to its owner when something happends on the port
**						The class creates a thread for reading and writing so the main
**						program is not blocked.
**						NOTE: Writing is now not on thread *** ARW ***
**
**	CREATION DATE		15-09-1997
**	LAST MODIFICATION	24-11-1998
**
**	AUTHOR				Remon Spekreijse
**	MODIFIED BY			Brian Koh Sze Hsian - Andy Whittaker
**
*/

// (c) 1996-99 Andy Whittaker, Chester, England. 
// mail@andywhittaker.com

#include "stdafx.h"
#include "SerialPort.h"
#include <assert.h>
 
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//Staic members must be globally defined
CRITICAL_SECTION	CSerialPort::m_csCommunicationSync;
OVERLAPPED			CSerialPort::m_ov;
int					CSerialPort::m_nActualWriteBufferSize = 0;

////////// Exception handling code

void AfxThrowSerialException(DWORD dwError /* = 0 */)
{
	if (dwError == 0)
		dwError = ::GetLastError();

	CSerialException* pException = new CSerialException(dwError);

	TRACE(_T("Warning: throwing CSerialException for error %d\n"), dwError);
	THROW(pException);
}

BOOL CSerialException::GetErrorMessage(LPTSTR pstrError, UINT nMaxError, PUINT pnHelpContext)
{
	ASSERT(pstrError != NULL && AfxIsValidString(pstrError, nMaxError));

	if (pnHelpContext != NULL)
		*pnHelpContext = 0;

	LPTSTR lpBuffer;
	BOOL bRet = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			                      NULL,  m_dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT),
			                      (LPTSTR) &lpBuffer, 0, NULL);

	if (bRet == FALSE)
		*pstrError = '\0';
	else
	{
		lstrcpyn(pstrError, lpBuffer, nMaxError);
		bRet = TRUE;

		LocalFree(lpBuffer);
	}

	return bRet;
}

CString CSerialException::GetErrorMessage()
{
  CString rVal;
  LPTSTR pstrError = rVal.GetBuffer(4096);
  GetErrorMessage(pstrError, 4096, NULL);
  rVal.ReleaseBuffer();
  return rVal;
}

CSerialException::CSerialException(DWORD dwError)
{
	m_dwError = dwError;
}

CSerialException::~CSerialException()
{
}

IMPLEMENT_DYNAMIC(CSerialException, CException)

#ifdef _DEBUG
void CSerialException::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);

	dc << "m_dwError = " << m_dwError;
}
#endif

//
// Constructor
CSerialPort::CSerialPort()
{
	m_hComm = NULL;
	m_Thread = NULL; // arw: fix for assertion without creating thread.

	// initialize overlapped structure members to zero
	m_ov.Offset = 0;
	m_ov.OffsetHigh = 0;

	// create events
	m_ov.hEvent = NULL;
	m_hShutdownEvent = NULL;

	m_bThreadAlive = FALSE;

	m_szWriteBuffer = NULL;
	m_nWriteBufferSize = 0;

	// Recall previous settings from the registry, default Com 1 in case 1st time run.
	CWinApp* pApp = AfxGetApp();
	m_nPortNr = pApp->GetProfileInt(_T("Communications"), _T("Port"), 1);
	m_nWriteDelay = pApp->GetProfileInt(_T("Communications"), _T("WriteDelay"), 100);
}

// Delete dynamic memory
CSerialPort::~CSerialPort()
{
	// Save our settings to the registry
	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileInt(_T("Communications"), _T("Port"), m_nPortNr);
	pApp->WriteProfileInt(_T("Communications"), _T("WriteDelay"), m_nWriteDelay);

	// resume the thread if it was suspended. Needed to process m_hShutdownEvent!
	if(m_Thread != NULL)
	{
	    DWORD dwSuspendCount;
		do
	    {
	       dwSuspendCount = m_Thread->ResumeThread();
	    }
		while((dwSuspendCount != 0) && (dwSuspendCount != 0xffffffff) );

		do
	    {
	       SetEvent(m_hShutdownEvent);
	    } 
		while (m_bThreadAlive);
	    TRACE(_T("Thread ended\n"));
	}

	// close handles to avoid memory leaks
	CloseHandle(m_ov.hEvent);
	CloseHandle(m_hComm);
	CloseHandle(m_hShutdownEvent);

	m_hComm = NULL;
	m_ov.hEvent = NULL;
	m_hShutdownEvent = NULL;

	if (m_szWriteBuffer != NULL)
		delete [] m_szWriteBuffer;

	DeleteCriticalSection(&m_csCommunicationSync);
}

IMPLEMENT_DYNAMIC(CSerialPort, CObject)

#ifdef _DEBUG
void CSerialPort::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);

	dc << _T("m_hComm = ") << m_hComm << _T("\n");
}
#endif

//
// Initialize the port. This can be any Com Port.
BOOL CSerialPort::InitPort(CWnd* pPortOwner,	// the owner (CWnd) of the port (receives message)
						   UINT  portnr,		// portnumber (e.g. 1..4)
						   UINT  baud,			// baudrate
						   char  parity,		// parity 
						   UINT  databits,		// databits 
						   UINT  stopbits,		// stopbits 
						   DWORD dwCommEvents,	// EV_RXCHAR, EV_CTS etc
						   UINT  writebuffersize)	// size to the writebuffer
{
	ASSERT(portnr >= 0 && portnr < 15);

	if (portnr == NULL)
	{// Use the last port setting.
		TRACE(_T("CSerialPort::InitPort - Setting COM Port to last value\n"));
		CWinApp* pApp = AfxGetApp();
		m_nPortNr = pApp->GetProfileInt(_T("Communications"), _T("Port"), 1);
	}
	else
		m_nPortNr = portnr;

	// Call the 2nd override function
	return InitPort(pPortOwner,baud,parity,databits,stopbits,dwCommEvents,writebuffersize);
}

// 2nd over-ride - uses the previous Com port number
BOOL CSerialPort::InitPort(CWnd* pPortOwner,	// the owner (CWnd) of the port (receives message)
						   UINT  baud,			// baudrate
						   char  parity,		// parity 
						   UINT  databits,		// databits 
						   UINT  stopbits,		// stopbits 
						   DWORD dwCommEvents,	// EV_RXCHAR, EV_CTS etc
						   UINT  writebuffersize)	// size to the writebuffer
{
	BOOL bResult = FALSE;

	ASSERT(pPortOwner != NULL);

	CWinApp* pApp = AfxGetApp();
	m_nPortNr = pApp->GetProfileInt(_T("Communications"), _T("Port"), 1);
	TRACE(_T("CSerialPort::InitPort - Com Port %d\n"), m_nPortNr);

	if (m_nPortNr < 1)
	{// This should never happen, however a mistake could have been made in the registry.
		TRACE(_T("CSerialPort::InitPort - Com Port cannot be 0"));
		return FALSE;
	}

	if (m_nPortNr > 255)
	{// This should never happen, however a mistake could have been made in the registry.
		TRACE(_T("CSerialPort::InitPort - Com Port cannot be greater than 255"));
		return FALSE;
	}

	// if the thread is alive: Kill
	if (m_bThreadAlive)
	{
		do
		{
			SetEvent(m_hShutdownEvent);
		} while (m_bThreadAlive);
		TRACE(_T("Thread ended\n"));
	}

	// create events
	if (m_ov.hEvent != NULL)
		ResetEvent(m_ov.hEvent);

	m_ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (!m_ov.hEvent)
	    AfxThrowSerialException();

	if (m_hShutdownEvent != NULL)
		ResetEvent(m_hShutdownEvent);
	m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (!m_hShutdownEvent)
	    AfxThrowSerialException();

	// initialize the event objects
	m_hEventArray[0] = m_hShutdownEvent;	// highest priority
	m_hEventArray[1] = m_ov.hEvent;

	// initialize critical section
	InitializeCriticalSection(&m_csCommunicationSync);
	
	// set buffersize for writing and save the owner
	m_pOwner = pPortOwner;

	if (m_szWriteBuffer != NULL) // delete the buffer if it exists
		delete [] m_szWriteBuffer;
	m_szWriteBuffer = new unsigned char[writebuffersize];

	m_nWriteBufferSize = writebuffersize;
	m_dwCommEvents = dwCommEvents;

	char *szPort = new char[50];
	char *szBaud = new char[50];

	// now it's critical!
	EnterCriticalSection(&m_csCommunicationSync);

	// if the port is already opened: close it
	if (m_hComm != NULL)
	{
		CloseHandle(m_hComm);
		m_hComm = NULL;
	}

	// prepare port strings
	sprintf_s(szPort, 50, _T("COM%d"), m_nPortNr);
	sprintf_s(szBaud, 50, _T("baud=%d parity=%c data=%d stop=%d"), baud, parity, databits, stopbits);

	// get a handle to the port
	m_hComm = CreateFile(szPort,						// communication port string (COMX)
					     GENERIC_READ | GENERIC_WRITE,	// read/write types
					     0, 							// comm devices must be opened with exclusive access
					     NULL,							// no security attributes
					     OPEN_EXISTING,					// comm devices must use OPEN_EXISTING
					     FILE_FLAG_OVERLAPPED,			// Async I/O
					     0);							// template must be 0 for comm devices

	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		// port not found
		delete [] szPort;
		delete [] szBaud;

	    TRACE(_T("Failed to open up the comms port\n"));
	    AfxThrowSerialException();
		return FALSE;
	}

	// set the timeout values
	m_CommTimeouts.ReadIntervalTimeout = 0;
	m_CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	m_CommTimeouts.ReadTotalTimeoutConstant = 0;
	m_CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	m_CommTimeouts.WriteTotalTimeoutConstant = 0;

	// configure
	if (SetCommTimeouts(m_hComm, &m_CommTimeouts))
	{						   
		if (SetCommMask(m_hComm, dwCommEvents))
		{
			if (GetCommState(m_hComm, &m_dcb))
			{ // DCB Structure
				m_dcb.fRtsControl = RTS_CONTROL_DISABLE; // disable RTS
				m_dcb.fOutxCtsFlow = FALSE; 	// disable CTS
				m_dcb.fOutxDsrFlow  = FALSE; 	// disable DSR
				m_dcb.fDtrControl  = DTR_CONTROL_DISABLE; 	// disable DTR
				m_dcb.fDsrSensitivity   = FALSE; 	// Ignores DSR
				m_dcb.fNull = FALSE; // Disable Null Stripping
				m_dcb.fTXContinueOnXoff = FALSE; // disable XON/XOFF
				m_dcb.fBinary = TRUE; // This is always TRUE in WIN32
				m_dcb.fOutX = FALSE; // Disable XOFF checking
				m_dcb.fInX = FALSE; // Disable XOFF generation
				m_dcb.DCBlength = sizeof(DCB);

				if (BuildCommDCB(szBaud, &m_dcb))
				{
					if (SetCommState(m_hComm, &m_dcb))
						; // normal operation... continue
					else
						ProcessErrorMessage(_T("SetCommState()"));
				}
				else
					ProcessErrorMessage(_T("BuildCommDCB()"));
			}
			else
				ProcessErrorMessage(_T("GetCommState()"));
		}
		else
			ProcessErrorMessage(_T("SetCommMask()"));
	}
	else
		ProcessErrorMessage(_T("SetCommTimeouts()"));

	delete [] szPort;
	delete [] szBaud;

	// flush the port
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	// release critical section
	LeaveCriticalSection(&m_csCommunicationSync);

	EscapeCommFunction(m_hComm, SETDTR); // This is needed for self-powered interfaces
	EscapeCommFunction(m_hComm, SETRTS); // This is needed for self-powered interfaces

	TRACE(_T("Initialisation for Com Port %d completed.\n"),
		m_nPortNr);

	return TRUE;
}

//	The CommThread Function.
UINT CSerialPort::CommThread(LPVOID pParam)
{
	// Cast the void pointer passed to the thread, back to
	// a pointer of CSerialPort class
	CSerialPort *port = (CSerialPort*)pParam;
	
	TRACE(_T("Com Port %d thread is starting.\n"), port->m_nPortNr);

	// Set the status variable in the dialog class to
	// TRUE to indicate the thread is running.
	port->m_bThreadAlive = TRUE; 
		
	// Misc. variables
	DWORD BytesTransfered = 0; 
	DWORD Event = 0;
	DWORD CommEvent = 0;
	DWORD dwError = 0;
	COMSTAT comstat;
	BOOL  bResult = TRUE;
		
	// Clear comm buffers at startup
	if (port->m_hComm)		// check if the port is opened
		PurgeComm(port->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	// begin forever loop.	This loop will run as long as the thread is alive.
	for (;;) 
	{ 
		// Make a call to WaitCommEvent().  This call will return immediatly
		// because our port was created as an async port (FILE_FLAG_OVERLAPPED
		// and an m_OverlappedStructerlapped structure specified).  This call will cause the 
		// m_OverlappedStructerlapped element m_OverlappedStruct.hEvent, which is part of 
		// the m_hEventArray, to be placed in a non-signaled state if there are no bytes
		// available to be read, or to a signaled state if there are bytes available.  
		// If this event handle is set to the non-signaled state, it will be set to
		// signaled when a character arrives at the port.

		// we do this for each port!

		bResult = WaitCommEvent(port->m_hComm, &Event, &port->m_ov);

		if (!bResult)  
		{ 
			// If WaitCommEvent() returns FALSE, process the last error to determin
			// the reason..
			switch (dwError = GetLastError()) 
			{ 
			case ERROR_IO_PENDING:	
				{ 
					// This is a normal return value if there are no bytes
					// to read at the port.
					// Do nothing and continue
					break;
				}
			case 87:
				{
					// Under Windows NT, this value is returned for some reason.
					// I have not investigated why, but it is also a valid reply
					// Also do nothing and continue.
					break;
				}
			default:
				{
					// All other error codes indicate a serious error has
					// occured.  Process this error.
					port->ProcessErrorMessage("WaitCommEvent()");
				    AfxThrowSerialException();
					break;
				}
			}
		}
		else
		{
			// If WaitCommEvent() returns TRUE, check to be sure there are
			// actually bytes in the buffer to read.  
			//
			// If you are reading more than one byte at a time from the buffer 
			// (which this program does not do) you will have the situation occur 
			// where the first byte to arrive will cause the WaitForMultipleObjects() 
			// function to stop waiting.  The WaitForMultipleObjects() function 
			// resets the event handle in m_OverlappedStruct.hEvent to the non-signelead state
			// as it returns.  
			//
			// If in the time between the reset of this event and the call to 
			// ReadFile() more bytes arrive, the m_OverlappedStruct.hEvent handle will be set again
			// to the signeled state. When the call to ReadFile() occurs, it will 
			// read all of the bytes from the buffer, and the program will
			// loop back around to WaitCommEvent().
			// 
			// At this point you will be in the situation where m_OverlappedStruct.hEvent is set,
			// but there are no bytes available to read.  If you proceed and call
			// ReadFile(), it will return immediatly due to the async port setup, but
			// GetOverlappedResults() will not return until the next character arrives.
			//
			// It is not desirable for the GetOverlappedResults() function to be in 
			// this state.	The thread shutdown event (event 0) will not work if the 
			// thread is blocked by GetOverlappedResults().
			//
			// The solution to this is to check the buffer with a call to ClearCommError().
			// This call will reset the event handle, and if there are no bytes to read
			// we can loop back through WaitCommEvent() again, then proceed.
			// If there are really bytes to read, do nothing and proceed.
		
			bResult = ClearCommError(port->m_hComm, &dwError, &comstat);
			if (!bResult)
				AfxThrowSerialException();

			if (comstat.cbInQue == 0)
				continue;
		}	// end if bResult

		// Main wait function.	This function will normally block the thread
		// until one of nine events occur that require action.
		Event = WaitForMultipleObjects(2, port->m_hEventArray, FALSE, INFINITE);

		Event = Event - WAIT_OBJECT_0; // WaitForMultipleObjects help says to do this.
		
		switch (Event)
		{
		case 0:
			{
				// Shutdown event.  This is event zero so it will be
				// the higest priority and be serviced first.

				// Tell the rest of the code that the thread is dying.
				port->m_bThreadAlive = FALSE;
				break;
			}  // case 0
		case 1: // read event
			{
				bResult = GetCommMask(port->m_hComm, &CommEvent);
				if (!bResult)
				    AfxThrowSerialException();

//				if (CommEvent & EV_CTS)
//					::SendMessage(port->m_pOwner->m_hWnd, WM_COMM_CTS_DETECTED, 
//					(WPARAM) 0, (LPARAM) port->m_nPortNr);
//				if (CommEvent & EV_RXFLAG)
//					::SendMessage(port->m_pOwner->m_hWnd, WM_COMM_RXFLAG_DETECTED, 
//					(WPARAM) 0, (LPARAM) port->m_nPortNr);
//				if (CommEvent & EV_BREAK)
//					::SendMessage(port->m_pOwner->m_hWnd, WM_COMM_BREAK_DETECTED, 
//					(WPARAM) 0, (LPARAM) port->m_nPortNr);
//				if (CommEvent & EV_ERR)
//					::SendMessage(port->m_pOwner->m_hWnd, WM_COMM_ERR_DETECTED, 
//					(WPARAM) 0, (LPARAM) port->m_nPortNr);
//				if (CommEvent & EV_RING)
//					::SendMessage(port->m_pOwner->m_hWnd, WM_COMM_RING_DETECTED, 
//					(WPARAM) 0, (LPARAM) port->m_nPortNr);
				if (CommEvent & EV_RXCHAR)
					// Receive character event from port.
					ReceiveChar(port, comstat);
					
				break;
			}  // case 1
		} // end switch

		// we're exiting, break out of loop and end thread
		if (port->m_bThreadAlive == FALSE)
			break;
	} // close forever loop

	// when we get here, the thread is about to shutdown and exit.
	TRACE(_T("Com Port %d thread about to die.\n"), port->m_nPortNr);
	return 0;
}

// Start comm watching
BOOL CSerialPort::StartMonitoring()
{
	TRACE(_T("Com Port %d starting monitoring.\n"), m_nPortNr);
	if (!(m_Thread = AfxBeginThread(CommThread, this, THREAD_PRIORITY_NORMAL)))
		return FALSE;
	// Clear buffer
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	TRACE(_T("Thread %ld started\n"), (DWORD)m_Thread->m_nThreadID);

	return TRUE;
}

// Restart the comm thread
BOOL CSerialPort::RestartMonitoring()
{
	TRACE(_T("Com Port %d re-starting monitoring.\n"), m_nPortNr);
	// Clear buffer
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	if(m_Thread != NULL)
		m_Thread->ResumeThread();
	else
		StartMonitoring();

	return TRUE;
}

// Suspend the comm thread
BOOL CSerialPort::StopMonitoring()
{
	if(m_Thread != NULL)
	{
		TRACE(_T("Com Port %d stopped monitoring.\n"), m_nPortNr);
		m_Thread->SuspendThread(); 
	}
	// Clear buffer
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	return TRUE;
}

//
// If there is a error, give the right message
void CSerialPort::ProcessErrorMessage(char* ErrorText)
{
	char *Temp = new char[200];
	
	LPVOID lpMsgBuf;

	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);

	sprintf_s(Temp, 200, _T("WARNING:  %s Failed with the following error: \n%s\nPort: %d\n"),
		(char*)ErrorText, (char*)lpMsgBuf, m_nPortNr); 
	MessageBox(NULL, Temp, _T("Application Error"), MB_ICONSTOP);

	LocalFree(lpMsgBuf);
	delete[] Temp;
}

// Write a character.
void CSerialPort::WriteChar(CSerialPort* port)
{
	BOOL bWrite = TRUE;
	BOOL bResult = TRUE;

	DWORD BytesSent = 0;

	// Gain ownership of the critical section
	EnterCriticalSection(&port->m_csCommunicationSync);
	
	if (bWrite)
	{
		// Initialise variables
		port->m_ov.Offset = 0;
		port->m_ov.OffsetHigh = 0;

		// Clear buffer
		PurgeComm(port->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

		bResult = WriteFile(port->m_hComm,						// Handle to COM Port
							port->m_szWriteBuffer,			    // Pointer to message buffer in calling function
							port->m_nActualWriteBufferSize,
							&BytesSent, 						// Where to store the number of bytes sent
							&port->m_ov);						// Overlapped structure

		// deal with any error codes
		if (!bResult)  
		{
			DWORD dwError = GetLastError();
			switch (dwError)
			{
				case ERROR_IO_PENDING:
					{
						// continue to GetOverlappedResults()
						BytesSent = 0;
						bWrite = FALSE;
						break;
					}
				default:
					{
						// all other error codes
						port->ProcessErrorMessage(_T("WriteFile()"));
						AfxThrowSerialException();
					}
			}
		} 
		else
		{
			LeaveCriticalSection(&port->m_csCommunicationSync);
		}
	} // end if(bWrite)

	// If we have not written all the bytes out, we call GetOverlappedResult(..) to
	// wait for the pending writes to happen.
	if (!bWrite)
	{
		bWrite = TRUE;
	
		bResult = GetOverlappedResult(port->m_hComm, // Handle to COMM port 
									  &port->m_ov,		//Overlapped structure
									  &BytesSent,		//Stores number of bytes sent
									  TRUE);			//Wait flag

		LeaveCriticalSection(&port->m_csCommunicationSync);

		// deal with the error code 
		if (!bResult)  
		{
			port->ProcessErrorMessage(_T("GetOverlappedResults() in WriteFile()"));
		    AfxThrowSerialException();

			if (GetLastError() != ERROR_IO_PENDING)
			{
				TRACE(_T("Failed in call to GetOverlappedResult\n"));
				AfxThrowSerialException();
			}
		}	
	} // end if (!bWrite)

	// Verify that the data size sent equals what we tried to send
	if (BytesSent != (DWORD)port->m_nActualWriteBufferSize)
	{
		TRACE(_T("WARNING: WriteFile() error.. Bytes Sent: %d; Message Length: %d\n"),
			BytesSent, port->m_nActualWriteBufferSize);
	}
}

// Character received. Inform the owner
void CSerialPort::ReceiveChar(CSerialPort* port, COMSTAT comstat)
{
	BOOL  bRead = TRUE; 
	BOOL  bResult = TRUE;
	DWORD dwError = 0;
	DWORD BytesRead = 0;
	DWORD BytesToRead = 1; //modified
	unsigned char RXBuff[MAXBYTESTOREAD];//modified

	for (;;) 
	{ 
		// Gain ownership of the comm port critical section.
		// This process guarantees no other part of this program 
		// is using the port object. 
		
		EnterCriticalSection(&port->m_csCommunicationSync);

		// ClearCommError() will update the COMSTAT structure and
		// clear any other errors.
		
		bResult = ClearCommError(port->m_hComm, &dwError, &comstat);

		BytesToRead = min(MAXBYTESTOREAD, comstat.cbInQue);

		LeaveCriticalSection(&port->m_csCommunicationSync);

		if (!bResult)
		    AfxThrowSerialException();

		// start forever loop.	I use this type of loop because I
		// do not know at runtime how many loops this will have to
		// run. My solution is to start a forever loop and to
		// break out of it when I have processed all of the
		// data available.  Be careful with this approach and
		// be sure your loop will exit.
		// My reasons for this are not as clear in this sample 
		// as it is in my production code, but I have found this 
		// solution to be the most efficient way to do this.
		
		if (comstat.cbInQue == 0)
		{
			// break out when all bytes have been read
			break;
		}
						
		EnterCriticalSection(&port->m_csCommunicationSync);

		if (bRead)
		{
			bResult = ReadFile(port->m_hComm,		// Handle to COMM port 
							   &RXBuff, 			// RX Buffer Pointer
							   BytesToRead,			//  byte to read depends on que
							   &BytesRead,			// Stores number of bytes read
							   &port->m_ov); 	// pointer to the m_ov structure
			// deal with the error code 
			if (!bResult)  
			{ 
				switch (dwError = GetLastError()) 
				{ 
					case ERROR_IO_PENDING:	
						{ 
							// asynchronous i/o is still in progress 
							// Proceed on to GetOverlappedResults();
							bRead = FALSE;
							break;
						}
					default:
						{
							// Another error has occured.  Process this error.
							port->ProcessErrorMessage("ReadFile()");
						    AfxThrowSerialException();
							break;
						} 
				}
			}
			else
			{
				// ReadFile() returned complete. It is not necessary to call GetOverlappedResults()
				bRead = TRUE;
			}
		}  // close if (bRead)

		if (!bRead)
		{
			bRead = TRUE;
			bResult = GetOverlappedResult(port->m_hComm, // Handle to COMM port 
										  &port->m_ov,	 // Overlapped structure
										  &BytesRead,	 // Stores number of bytes read
										  TRUE);		 // Wait flag

			// deal with the error code 
			if (!bResult)  
			{
				port->ProcessErrorMessage(_T("GetOverlappedResults() in ReadFile()"));
			    AfxThrowSerialException();
			}	
		}  // close if (!bRead)
				
		LeaveCriticalSection(&port->m_csCommunicationSync);

		// notify parent that a byte was received
		::SendMessage((port->m_pOwner)->m_hWnd, WM_COMM_RXCHAR, (WPARAM) RXBuff, (LPARAM) BytesRead);

	} // end forever loop
}

// *************************************************************************************************
// Public Functions
//

// Write a string to the port - This can write even NULL characters
void CSerialPort::WriteToPort(unsigned char* string, int stringlength, BOOL bDelay)
{	
	assert(m_hComm != 0);

	::EnterCriticalSection(&m_csCommunicationSync);

//	memset(m_szWriteBuffer, 0, sizeof(m_szWriteBuffer));
	if (stringlength > MAX_WRITE_BUFFER)
		stringlength = MAX_WRITE_BUFFER;
	for(int i=0;i<stringlength;i++)
		m_szWriteBuffer[i]=string[i];
	m_nActualWriteBufferSize=stringlength;

	::LeaveCriticalSection(&m_csCommunicationSync);

	if (bDelay)
		Sleep(10); // reduces stress on ECU serial port.
	WriteChar(this); // Write to port immediately
	if (bDelay)
		Sleep(m_nWriteDelay); // reduces stress on ECU serial port.
}

// Write a string to the port ** Caution - May be broken
void CSerialPort::WriteToPort(unsigned char* string)
{		
	assert(m_hComm != 0);

	memset(m_szWriteBuffer, 0, sizeof(m_szWriteBuffer));
	strcpy_s((char *) m_szWriteBuffer, sizeof(m_szWriteBuffer), (char *)string);

	m_nActualWriteBufferSize=strlen((char*)m_szWriteBuffer) ? strlen((char*)m_szWriteBuffer) : 1;
		
	WriteChar(this); // Write to port immediately
}

// Returns the device control block
DCB CSerialPort::GetDCB()
{
	return m_dcb;
}

// Returns the communication event masks
DWORD CSerialPort::GetCommEvents()
{
	return m_dwCommEvents;
}

// Returns the output buffer size
DWORD CSerialPort::GetWriteBufferSize()
{
	return m_nWriteBufferSize;
}

// Returns the current port number
UINT CSerialPort::GetPort(void)
{
	return m_nPortNr;
}

// Sets the internal com port number
void CSerialPort::SetPort(UINT nPort)
{
	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileInt(_T("Communications"), _T("Port"), nPort);
    TRACE(_T(_T("Comms port number changed\n")));
}

// Returns the minimum number of milliseconds the TX routine will wait before returning
DWORD CSerialPort::GetWriteDelay(void)
{
	return m_nWriteDelay;
}

// Sets the minimum number of milliseconds the TX routine will wait before returning
void CSerialPort::SetWriteDelay(DWORD nDelay)
{
	m_nWriteDelay = nDelay;
	// Write this to the registry
	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileInt(_T("Communications"), _T("WriteDelay"), m_nWriteDelay);
    TRACE(_T("Comms port write delay changed\n"));
}
