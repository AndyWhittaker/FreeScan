// MainDlg.cpp : implementation file
//
// (c) 1996-99 Andy Whittaker, Chester, England. 
// mail@andywhittaker.com
//

#include "stdafx.h"
#include "FreeScan.h"
#include "MainDlg.h"
#include "StatusDlg.h"
#include "Supervisor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFreeScanDlg dialog

IMPLEMENT_DYNAMIC(CFreeScanDlg, CPropertySheet);

CFreeScanDlg::CFreeScanDlg(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AllocateAndAddPages();

	m_bHasRun = FALSE;

	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CFreeScanDlg::CFreeScanDlg(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AllocateAndAddPages();

	m_bHasRun = FALSE;

	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CFreeScanDlg::~CFreeScanDlg()
{
	delete m_pDetailDlg;
	delete m_pEngineViewDlg;
	delete m_pStatusBitsDlg;
	delete m_pDashBoardDlg;
	delete m_pTCodesDlg;
	delete m_pSensorDlg;
	delete m_pAdvancedDlg;
	delete m_pAbout;
	delete m_pStatusDlg;

	#ifdef _DEBUG
		delete m_pEngineDlg; // Old Engine Data View
		delete m_pRawMode00Dlg;
		delete m_pRawMode01Dlg;
	#endif // _DEBUG

	if (m_pSupervisor!=NULL)
		delete m_pSupervisor;
}

// Creates storage space for the PropertyPages
// then adds them to the dialog
void CFreeScanDlg::AllocateAndAddPages(void)
{
	// Allocate all of the classes for the PropertyPages
	m_pDetailDlg =  new CDetailDlg;
	m_pEngineViewDlg = new	CEngineViewDlg; // new view
	m_pStatusBitsDlg = new	CStatusBitsDlg; // Status Bits
	m_pDashBoardDlg = new	CDashBoardDlg;
	m_pSensorDlg = new	CSensorDlg;
	m_pAdvancedDlg = new CAdvancedDlg;
	m_pTCodesDlg = new CTCodesDlg;
	m_pAbout = new CAbout;

	// Status Dialog
	m_pStatusDlg = new CStatusDlg;

	// Pass pointers to the called dialogs
	m_pDetailDlg->m_pMainDlg = this;
	m_pEngineViewDlg->m_pMainDlg = this; // new view
	m_pStatusBitsDlg->m_pMainDlg = this; // Status Bits
	m_pDashBoardDlg->m_pMainDlg = this;
	m_pTCodesDlg->m_pMainDlg = this;
	m_pSensorDlg->m_pMainDlg = this;
	m_pAdvancedDlg->m_pMainDlg = this;

	// Now Add the pages to the Dialog
	AddPage(m_pDetailDlg);
	AddPage(m_pEngineViewDlg); // new view
	AddPage(m_pStatusBitsDlg); // Status Bits
	AddPage(m_pDashBoardDlg);
	AddPage(m_pTCodesDlg);
	AddPage(m_pSensorDlg);
	AddPage(m_pAdvancedDlg);
	#ifdef _DEBUG // Add these only in Debug builds
		m_pEngineDlg = new	CEngineDlg; // Old Engine Data View
		m_pRawMode00Dlg = new CRawMode00;
		m_pRawMode01Dlg = new CRawMode01;
		m_pEngineDlg->m_pMainDlg = this; // Old Engine Data View
		m_pRawMode00Dlg->m_pMainDlg = this;
		m_pRawMode01Dlg->m_pMainDlg = this;
		AddPage(m_pEngineDlg); // Old Engine Data View
		AddPage(m_pRawMode00Dlg);
		AddPage(m_pRawMode01Dlg);
	#endif // _DEBUG
	AddPage(m_pAbout);
}

BEGIN_MESSAGE_MAP(CFreeScanDlg, CPropertySheet)
	//{{AFX_MSG_MAP(CFreeScanDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CFreeScanDlg::SetLogoText(CString Text)
{
	m_LogoText = Text;
}

void CFreeScanDlg::SetLogoFont(CString Name, int nHeight/* = 24*/,
	int nWeight/* = FW_BOLD*/, BYTE bItalic/* = true*/, BYTE bUnderline/* = false*/)
{
	if(m_fontLogo.m_hObject)
		m_fontLogo.Detach();

	m_fontLogo.CreateFont(nHeight, 0, 0, 0, nWeight, bItalic, bUnderline,0,0,0,0,0,0, Name);
}

// Updates all property pages in this dialog
void CFreeScanDlg::UpdateDialog(void)
{
	if (IsWindow(*m_pDetailDlg))
	{
		m_pDetailDlg->Refresh();
	}
	if (IsWindow(*m_pEngineViewDlg))
	{
		m_pEngineViewDlg->Refresh();
	}
	if (IsWindow(*m_pStatusBitsDlg))
	{
		m_pStatusBitsDlg->Refresh();
	}
	if (IsWindow(*m_pDashBoardDlg))
	{
		m_pDashBoardDlg->Refresh();
	}
	if (IsWindow(*m_pTCodesDlg))
	{
		m_pTCodesDlg->Refresh();
	}
	if (IsWindow(*m_pSensorDlg))
	{
		m_pSensorDlg->Refresh();
	}
	if (IsWindow(*m_pAdvancedDlg))
	{
		m_pAdvancedDlg->Refresh();
	}
#ifdef _DEBUG // Handle these only in Debug builds
	if (IsWindow(*m_pEngineDlg))
	{ // Old Engine Data View
		m_pEngineDlg->Refresh();
	}
	if (IsWindow(*m_pRawMode00Dlg))
	{
		m_pRawMode00Dlg->Refresh();
	}
	if (IsWindow(*m_pRawMode01Dlg))
	{
		m_pRawMode01Dlg->Refresh();
	}
#endif // _DEBUG
//	UpdateData(TRUE);
}

// Starts the Supervisor
BOOL CFreeScanDlg::StartComs(void)
{
	if (!m_bHasRun)
	{ // First time started
		if (m_pSupervisor->Start())
		{
			m_bHasRun = TRUE;
			return TRUE;
		}
		else
			return FALSE;
	}
	else
		return m_pSupervisor->Restart();
}

// Stops the Supervisor
BOOL CFreeScanDlg::StopComs(void)
{
	return m_pSupervisor->Stop();
}

// Writes a line of ASCII to the spy window
void CFreeScanDlg::WriteStatus(CString csText)
{
	csText = "Main: " + csText;
	m_pStatusDlg->WriteStatus(csText);
}

// Writes a line of binary as ASCII to the spy window
void CFreeScanDlg::WriteASCII(unsigned char * buffer, int ilength)
{
	m_pStatusDlg->WriteASCII(buffer, ilength);
}

void CFreeScanDlg::WriteLogEntry(LPCTSTR pstrFormat, ...)
{
	va_list args;
	va_start(args, pstrFormat);
	m_pStatusDlg->WriteLogEntry(pstrFormat, args);
}

void CFreeScanDlg::WriteStatusLogged(CString csText) 
{
	m_pStatusDlg->WriteStatusLogged(csText);
}

// Starts or stops csv logging to file
BOOL CFreeScanDlg::StartLog(BOOL bStart)
{
	return m_pStatusDlg->StartLog(bStart);
}

// Starts or stops csv logging to file
BOOL CFreeScanDlg::StartCSVLog(BOOL bStart)
{
	return m_pSupervisor->StartCSVLog(bStart);
}

/////////////////////////////////////////////////////////////////////////////
// CFreeScanDlg message handlers

BOOL CFreeScanDlg::OnInitDialog()
{
	// hide the buttons we do not want to show.
	CWnd *pWnd = GetDlgItem( IDCANCEL );
	ASSERT_VALID(pWnd);
	pWnd->DestroyWindow();
	pWnd = GetDlgItem( ID_APPLY_NOW );
	ASSERT_VALID(pWnd);
	pWnd->DestroyWindow();
	pWnd = GetDlgItem( IDHELP );
	ASSERT_VALID(pWnd);
	pWnd->EnableWindow(FALSE);
	
	//move the OK button
	pWnd = GetDlgItem( IDOK );
	ASSERT_VALID(pWnd);
	pWnd->SetWindowText("Exit"); // rename OK to Exit
	CRect rcTemp(0, 0, 0, 0);
	int nLeft = 0;
	pWnd->GetWindowRect(rcTemp);
	ScreenToClient(rcTemp);
	nLeft -= -235 + rcTemp.Width();
	pWnd->MoveWindow(nLeft, rcTemp.top, rcTemp.Width(), rcTemp.Height());

	CString buf;
	// Load the version string from our resource
	buf.LoadString(IDS_VERSION);

	// This is for the Logo Text on the property sheet
	SetLogoFont("Arial");
	SetLogoText(buf);

	// Create the "spy" window
	m_pStatusDlg->Create(NULL);
	buf = buf + " by Andy Whittaker";
	WriteStatus(buf);
	WriteStatus("Starting Up..");

	// Initialise the communications supervisor
	m_pSupervisor = new CSupervisor(this, m_pStatusDlg);

	// We now start creating our User Interface
	CPropertySheet::OnInitDialog();

	// Initialise the first dialog sheet. It is done this way because
	// it's only now that the Protocol window has been created and able
	// to receive and answer messages.
	m_pDetailDlg->Init();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	WriteStatus("Ready..");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CFreeScanDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{ // Paint our logo in list box
		CPaintDC dc(this); // device context for painting

		if (m_LogoText.IsEmpty())
		{
		}
		else
		{ // Draw a logo on our property sheet
			// Thanks to Frumento Enrico for modifications
			// and Nikolay Sokratov for the original.
			BOOL bWizMode; 
        
			// Get the current style from PROPSHEETHEADER structure 
			if( m_psh.dwFlags & PSH_WIZARD ) 
				bWizMode = TRUE;                // It's in wizard mode
			else 
				bWizMode = FALSE;               // It's in property sheet mode


			// If this is a Wizard, cannot retrieve the tab control dimension. 
			// So Get the Dialog's Window Rect  

			CRect rectTabCtrl;
			if( bWizMode ) 
			{ 
				GetWindowRect(rectTabCtrl); 
				rectTabCtrl.OffsetRect(14,0);	// A little correction
			} 
			else 
			{ 
				GetTabControl()->GetWindowRect(rectTabCtrl);
			} 
			ScreenToClient(rectTabCtrl);


			CRect rectOk;
			GetDlgItem(IDOK)->GetWindowRect(rectOk);
			ScreenToClient(rectOk);

			dc.SetBkMode(TRANSPARENT);

			CRect rectText;
			rectText.left = rectTabCtrl.left;
			rectText.top = rectOk.top;
			rectText.bottom = rectOk.bottom;
			rectText.right = rectOk.left;

			CFont * OldFont = dc.SelectObject(&m_fontLogo);

			// draw text in DC
			COLORREF OldColor = dc.SetTextColor( ::GetSysColor( COLOR_3DHILIGHT));

			// before drawing calculate Rect size and if greater reduce font size
			CRect RealrectText;
			BOOL bOK=FALSE;
			BOOL bShouldDrawText=TRUE;
			LOGFONT LogFont;
			TEXTMETRIC tmText;
			CFont *resizedfontLogo=&m_fontLogo;
			RealrectText=rectText;
			while (!bOK) {
			   CFont newfontLogo;
			   dc.DrawText( m_LogoText, &RealrectText, 
					  DT_CALCRECT | DT_SINGLELINE | DT_LEFT | DT_VCENTER);
			   dc.GetTextMetrics(&tmText);
			   //leave space for a char on the right
			   if (RealrectText.right < (rectText.right- tmText.tmMaxCharWidth) )
				  bOK=TRUE;
			   resizedfontLogo->GetLogFont(&LogFont);
			   //set lower bound if we can't draw even with the smallest font
			   if (LogFont.lfHeight<8 && bOK==FALSE) {
				  bShouldDrawText=FALSE;
				  bOK=TRUE;
			   }
			   LogFont.lfHeight--;
			   newfontLogo.CreateFontIndirect(&LogFont);
			   resizedfontLogo= dc.SelectObject(&newfontLogo);
			}
					
			//doesn't draw if the font results too much little
			if (bShouldDrawText) {
			   dc.DrawText( m_LogoText, rectText + CPoint(1,1), 
				DT_SINGLELINE | DT_LEFT | DT_VCENTER);
			   dc.SetTextColor( ::GetSysColor( COLOR_3DSHADOW));
			   dc.DrawText( m_LogoText, rectText, 
				DT_SINGLELINE | DT_LEFT | DT_VCENTER);
			}

			// restore old text color
			dc.SetTextColor( OldColor);

			// restore old font
			dc.SelectObject(OldFont);

			// Do not call CPropertySheet::OnPaint() for painting messages
			// CPropertySheet::OnPaint();
		}
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFreeScanDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CFreeScanDlg::OnClose() 
{
	// Close the communications Supervisor
	m_pSupervisor->ShutDown();	

	// Close the spy window
	m_pStatusDlg->DestroyWindow();
}
