// FreeScan.cpp : Defines the class behaviors for the application.
//
// (c) 1996-99 Andy Whittaker, Chester, England. 
// mail@andywhittaker.com
//

#include "stdafx.h"
#include "FreeScan.h"
#include "MainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFreeScanApp

BEGIN_MESSAGE_MAP(CFreeScanApp, CWinApp)
	//{{AFX_MSG_MAP(CFreeScanApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFreeScanApp construction

CFreeScanApp::CFreeScanApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CFreeScanApp object

CFreeScanApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CFreeScanApp initialization

BOOL CFreeScanApp::InitInstance()
{
// Find out what version of Windows we are running on.
    OSVERSIONINFO version;

	memset(&version, 0, sizeof(version));

        // must set size of structure
    version.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        // get version data
    if (!GetVersionEx(&version))
    {
		CString s;
        s.Format("Error getting Version Info: %d", GetLastError());
		AfxMessageBox(s, MB_ICONSTOP | MB_OK);
        return FALSE;
    }

    switch(version.dwPlatformId)
    {
        case VER_PLATFORM_WIN32_NT:
//            AfxMessageBox("----->Windows NT\n");
            break;
        case VER_PLATFORM_WIN32_WINDOWS :
//            AfxMessageBox("----->Windows95\n");
            break;
        case VER_PLATFORM_WIN32s:
                // Not supported
            AfxMessageBox("Sorry, Windows 3.1 Win32s is not supported by FreeScan", MB_ICONSTOP | MB_OK);
			return FALSE;
            break;
        default:
			{
			char szBuffer[100];
            sprintf_s(szBuffer, "Untested Win32 Version: %d\nFreeScan may not run correctly\nPlease Email fs@andywhittaker.com with this error.", version.dwPlatformId);
			AfxMessageBox(szBuffer, MB_ICONEXCLAMATION | MB_OK);
			//return FALSE;
			}
    }            

	AfxEnableControlContainer(); // For ActiveX Controls

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	// Setup the registry for our application
	SetRegistryKey("arw Software");
	LoadStdProfileSettings();  // Load standard INI file options (including MRU)		

	CFreeScanDlg dlg(IDS_TITLE, NULL);
	m_pMainWnd = &dlg;

	// Find out of the screen saver is active.
	BOOL	bActive=FALSE;
	SystemParametersInfo(SPI_GETSCREENSAVEACTIVE,0,&bActive,0);
	if (bActive)// Disable the screensaver
		SystemParametersInfo(SPI_SETSCREENSAVEACTIVE,FALSE,0,SPIF_SENDWININICHANGE); 

	// call our dialog
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	if (bActive)// Reenable the screensaver
		SystemParametersInfo(SPI_SETSCREENSAVEACTIVE,TRUE,0,SPIF_SENDWININICHANGE); 

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
