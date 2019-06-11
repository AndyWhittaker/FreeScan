// StatusDlg.cpp : implementation file
//
// Allows the user to peep inside the program by allowing
// the programmer to end data to its screen.
// It also will log the data to a file.
//
// (c) 1991-99 Andy Whittaker, Chester, England. 
// mail@andywhittaker.com
//
#include "stdafx.h"
#include "cderr.h" // for CommDlgExtendedError()
#include "FreeScan.h"
#include "StatusDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatusDlg dialog


CStatusDlg::CStatusDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStatusDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStatusDlg)
	//}}AFX_DATA_INIT
	//Load hide status
	CWinApp* pApp = AfxGetApp();
	m_hidden = pApp->GetProfileInt(_T("StatusDlg"), _T("Hide Status"), FALSE);
	m_csLogFile = pApp->GetProfileString(_T("StatusDlg"), _T("Log Filename"), _T(""));

	m_WindowPos.left = 0;
	m_WindowPos.right = 0;
	m_WindowPos.top = 0;
	m_WindowPos.bottom = 0;
}

CStatusDlg::~CStatusDlg()
{
	if (m_file.m_hFile != CFile::hFileNull)
	{ // close our log file if it's open
		m_file.Flush();
		m_file.Close(); // close the logging file when we exit.
	}

	//Save hide status
	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileInt(_T("StatusDlg"), _T("Hide Status"), m_hidden);
	pApp->WriteProfileString(_T("StatusDlg"), _T("Log Filename"), m_csLogFile);

	StartLog(FALSE);// close the logging file when we exit.
}

void CStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatusDlg)
	DDX_Control(pDX, IDC_TIME, m_Time);
	DDX_Control(pDX, IDC_STATUS, m_Status);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CStatusDlg, CDialog)
	//{{AFX_MSG_MAP(CStatusDlg)
	ON_WM_MOVE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatusDlg access functions

//WriteStatus enters text into the list box at the end of the list
void CStatusDlg::WriteStatus(CString csText) 
{ //CListBox

	if (m_hidden) // don't write to window when hidden
		return;

//	TRACE_T(("%s\n"), csText);
	if (IsWindow(m_hWnd))
	{
		PumpMessages();
		if(m_Status.GetCount() == 100) // Maximum number of strings we want
		{
//			TRACE("Listbox > 100 items, deleting first string\n");
			m_Status.DeleteString(0);
		}
		m_Status.AddString(csText); // Add the new string in again.
		SendDlgItemMessage(IDC_STATUS, WM_VSCROLL , MAKEWPARAM(SB_LINEDOWN, NULL), NULL );
	}

	if (m_file.m_pStream != NULL) // i.e. we have a file open
	{
		csText = csText + _T("\n"); // Line Feed while we log to disk
		m_file.WriteString(csText);
	}
	if (IsWindow(m_hWnd))
		UpdateWindow();
}

//WriteStatus enters text into the list box at the end of the list with timestamp
void CStatusDlg::WriteStatusLogged(CString csText) 
{
	if (m_hidden) // don't write to window when hidden
		return;

	CString csBuf;
	m_now = CTime::GetCurrentTime();
	csBuf = m_now.Format("%d %b %y %H:%M:%S - ");
	if (IsWindow(m_Time.m_hWnd))
		m_Time.SetWindowText(csBuf); // Updates the time string
	csBuf = csBuf + csText;

	WriteStatus(csBuf);
}

//Writes the contents of the given buffer as a Hex-dump
void CStatusDlg::WriteASCII(unsigned char * buffer, int ilength)
{
	if (m_hidden) // don't write to window when hidden
		return;

	CString	cs;
	int		iIndex;

	// Hmmm, I'm sure there's a better way of doing this.
	for(iIndex=0; iIndex < ilength; iIndex++)
	{
		CString csTemp;
		csTemp.Format("0x%02X ", buffer[iIndex]);
		cs = cs + csTemp; // concatenate string
	}

	// Now write the string.
	WriteStatusLogged(cs);
}

//Writes a formatted log entry to the log file
// e.g.	WriteLogEntry("Program started");
//		WriteLogEntry("Processed %d bytes", 91341);
//		WriteLogEntry("%d error(s) found in %d line(s)", 10, 1351);
//		WriteLogEntry("Program completed");
void CStatusDlg::WriteLogEntry(LPCTSTR pstrFormat, ...)
{
	if (m_hidden) // don't write to window when hidden
		return;

	if (m_file.m_hFile == CFile::hFileNull)
	{ // No log-file open
		return;
	}

	CTime timeWrite;
	timeWrite = CTime::GetCurrentTime();

	// write the time out
	CString str = timeWrite.Format("%d %b %y %H:%M:%S - ");
	m_file.Write(str, str.GetLength());

	// format and write the data we were given
	va_list args;
	va_start(args, pstrFormat);
	str.FormatV(pstrFormat, args);
	m_file.Write(str, str.GetLength());

	// put a newline
	m_file.Write("\n", 1);
	return;
}

// Starts or stops logging to file
BOOL CStatusDlg::StartLog(BOOL bStart)
{
	CString csBuf = "";

	if (!bStart)
	{ // we want to close the logging file
		if (m_file.m_hFile != CFile::hFileNull)
		{
			WriteStatusLogged(_T("Log file stopped"));
			m_file.Close(); // close the logging file when we exit.
		}
		else
			WriteStatusLogged(_T("Log file is already closed"));
		
		return FALSE;
	}

	// We now must want to log to a file

	OPENFILENAME ofn;
	memset(&ofn, 0, sizeof(ofn)); // initialize structure to 0/NULL
	TCHAR szFileName[_MAX_PATH]; // contains full path name after return
	TCHAR szFileTitle[_MAX_PATH];
	szFileName[0] = '\0'; // initialises file title buffer
	szFileTitle[0] = '\0';
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = _countof(szFileName);
	ofn.lpstrFileTitle = (LPTSTR)szFileTitle;
	ofn.nMaxFileTitle = _countof(szFileTitle);
	ofn.lpstrTitle = _T("Create/Open Logging File");
	ofn.lpstrFilter = _T("log Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0\0\0");
	ofn.lpstrInitialDir = m_csLogFile.GetString();

	// setup initial file name from the registry
	//lstrcpyn(szFileName, m_csLogFile, _countof(szFileName));//unsafe
	StringCchCopy(szFileName, _countof(szFileName), m_csLogFile.GetString());

	ofn.Flags = OFN_HIDEREADONLY | OFN_LONGNAMES | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

	int nResult = ::GetSaveFileName(&ofn);

	if (nResult == 0) // Handle the error
	{
		DWORD dwerror = CommDlgExtendedError();
		switch (dwerror)
		{
		case 0 : // no error
			csBuf.Format(_T("User cancelled/closed file Dialog"));
			break; // cancel or closed was pressed
		case FNERR_BUFFERTOOSMALL :
			csBuf.Format(_T("Buffer too small"));
			AfxMessageBox(csBuf, MB_OK | MB_ICONSTOP );
			break;
		case FNERR_INVALIDFILENAME :
			csBuf.Format(_T("Invalid file name"));
			AfxMessageBox(csBuf, MB_OK | MB_ICONSTOP );
			break;
		case CDERR_MEMALLOCFAILURE :
			csBuf.Format(_T("Memory allocation failure"));
			AfxMessageBox(csBuf, MB_OK | MB_ICONSTOP );
			break;
		case CDERR_MEMLOCKFAILURE :
			csBuf.Format(_T("Memory lock failure"));
			AfxMessageBox(csBuf, MB_OK | MB_ICONSTOP );
			break;
		default:
			csBuf.Format(_T("An unknown error has occured - %ld"), dwerror);
			AfxMessageBox(csBuf, MB_OK | MB_ICONSTOP );
		}
		WriteStatus(csBuf);
		return FALSE;
	}

	// Copy filename from our dialog to the CString member variable
	m_csLogFile.Format(_T("%s"),ofn.lpstrFile);

	if (!m_file.Open(m_csLogFile, CFile::modeCreate | CFile::modeReadWrite | CFile::typeText))
	{
		csBuf.Format(_T("Cannot open %s"), m_csLogFile.GetString());
		WriteStatus(csBuf);
		AfxMessageBox(csBuf, MB_OK | MB_ICONSTOP );
		return FALSE;
	}

	WriteStatusLogged(_T("Log file has started"));

/*
	// Construct our File Dialog
	CFileDialog		Dialog(FALSE, "txt", 
						m_csLogFile, 
						OFN_HIDEREADONLY | OFN_LONGNAMES | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT,
						"log Files (*.txt)|*.txt|All Files (*.*)|*.*||", NULL);

	// Change the title
	Dialog.m_ofn.lpstrTitle = "Create/Open Logging File";

	// Display the dialog box
	if (Dialog.DoModal() == IDOK)
	{
		m_csLogFile = Dialog.GetPathName();

		if (!m_file.Open(m_csLogFile, CFile::modeCreate | CFile::modeReadWrite | CFile::typeText))
		{
			csBuf.Format("Cannot open %s", m_csLogFile);
			WriteStatus(csBuf);
			AfxMessageBox(csBuf, MB_OK | MB_ICONSTOP );
			return FALSE;
		}

		WriteStatusLogged("Log file has started");
	}
	else // User pressed cancel
		WriteStatus("User cancelled log file");
*/
	if (m_file.m_hFile != CFile::hFileNull)
		return TRUE;
	else
		return FALSE;
}

// Hides the status window from view
void CStatusDlg::Hide(BOOL yes)
{
	if (yes)
	{
		m_hidden = TRUE;
		if (::IsWindow(m_hWnd))
			ShowWindow(SW_HIDE);
	}
	else
	{
		m_hidden = FALSE;
		if (::IsWindow(m_hWnd))
			ShowWindow(SW_SHOW);
	}
}

// Hide status of window
BOOL CStatusDlg::HideStatus(void)
{
	return m_hidden;
}

// Ensures window receives messages and updates correctly
void CStatusDlg::PumpMessages(void)
{
    // Must call Create() before using the dialog
    ASSERT(m_hWnd!=NULL);

    MSG msg;
    // Handle dialog messages
    while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if(!IsDialogMessage(&msg))
      {
        TranslateMessage(&msg);
        DispatchMessage(&msg);  
      }
    }
}

// This is for debugging. Prints the current screen coordinates.
void CStatusDlg::PrintRect(LPRECT lpR)
{
	CString		csData;
	csData.Format(_T("Rect Data:\nleft = %li\ntop = %li\nright = %li\nbottom = %li"),
		lpR->left, lpR->top, lpR->right, lpR->bottom);
	AfxMessageBox(csData, MB_OK | MB_ICONINFORMATION);
}

/////////////////////////////////////////////////////////////////////////////
// CStatusDlg message handlers

BOOL CStatusDlg::OnInitDialog() 
{
	//Retrieve the Window Position from the registry
	CWinApp* pApp = AfxGetApp();
	m_WindowPos.left   = pApp->GetProfileInt(_T("StatusDlg"), _T("left pos"), 0);
	m_WindowPos.top    = pApp->GetProfileInt(_T("StatusDlg"), _T("top pos"), 0);

	// Set the window position
	SetWindowPos( &wndBottom, m_WindowPos.left, m_WindowPos.top, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);

	BOOL bResult = CDialog::OnInitDialog();

	// Sets the virtual size of the list box
	m_Status.SetHorizontalExtent(2000);
	
	// Allocates system memory to listbox to speed up memory allocations
	m_Status.InitStorage(32000, 1048576);

	if (m_hidden)
	{
		if (::IsWindow(m_hWnd))
			ShowWindow(SW_HIDE);
	}
	else
	{
		if (::IsWindow(m_hWnd))
			ShowWindow(SW_SHOW);
	}

	return TRUE;
}

// This is called by the BaseDialog to create this Status Dialog
BOOL CStatusDlg::Create(CWnd* pParentWnd) 
{
	return CDialog::Create(IDD, pParentWnd);
}

// This tracks the window's postition
void CStatusDlg::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);

	GetWindowRect(&m_WindowPos);
}

void CStatusDlg::OnDestroy() 
{
	//Save the current window position
	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileInt(_T("StatusDlg"), _T("left pos"), m_WindowPos.left);
	pApp->WriteProfileInt(_T("StatusDlg"), _T("top pos"), m_WindowPos.top);

	CDialog::OnDestroy();
}
