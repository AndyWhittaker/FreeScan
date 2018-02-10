// TTPropertyPage.cpp : implementation file
//

#include "stdafx.h"
#include "FreeScan.h"
#include "TTPropertyPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTTPropertyPage property page

IMPLEMENT_DYNCREATE(CTTPropertyPage, CPropertyPage)

CTTPropertyPage::CTTPropertyPage() : CPropertyPage()
{
}

CTTPropertyPage::CTTPropertyPage(UINT uID) : CPropertyPage(uID)
{
	//{{AFX_DATA_INIT(CTTPropertyPage)
		// NOTE: the ClassWizard will add member initialization here
	m_bBoldTips = FALSE;
	m_bSetMaxWidth = FALSE;
	m_nMaxWidth = 0;
	//}}AFX_DATA_INIT
}


CTTPropertyPage::~CTTPropertyPage()
{
}

void CTTPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTTPropertyPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTTPropertyPage, CPropertyPage)
	//{{AFX_MSG_MAP(CTTPropertyPage)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTTPropertyPage message handlers

BOOL CTTPropertyPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// initialize the tooltip
	m_toolTip.Create(this);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CTTPropertyPage::OnToolTipNotify(UINT /*id*/, NMHDR* pNMHDR, LRESULT* /*pResult*/)
{
//  TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;    
//	UINT uID = pNMHDR->idFrom;
	return FALSE;
}

BOOL CTTPropertyPage::PreTranslateMessage( MSG *pMsg )
{
   	m_toolTip.RelayEvent(pMsg);

	return CPropertyPage::PreTranslateMessage(pMsg);
}

