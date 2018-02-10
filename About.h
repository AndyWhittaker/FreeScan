//{{AFX_INCLUDES()
#include "rbgauge.h"
//}}AFX_INCLUDES
#if !defined(AFX_ABOUT_H__09BF9084_1326_11D2_983E_00E018900F2A__INCLUDED_)
#define AFX_ABOUT_H__09BF9084_1326_11D2_983E_00E018900F2A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// About.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAbout dialog

#include "HyperLink.h"

class CAbout : public CPropertyPage
{
	DECLARE_DYNCREATE(CAbout)

// Construction
public:
	CAbout();
	~CAbout();

	#define     DISPLAY_TIMER_ID		150		// timer id

 	RECT        m_ScrollRect,r;		   // rect of Static Text frame
	int         nArrIndex,nCounter;		   // work ints
	CString     m_szWork;			   // holds display line
	BOOL        m_bFirstTime;
	BOOL        m_bDrawText;
	int         nClip;
	int         nCurrentFontHeight;

	CWnd*       m_pDisplayFrame;

	CBitmap     m_bmpWork;                  // bitmap holder
	CBitmap* 	pBmpOld;                    // other bitmap work members
	CBitmap*    m_bmpCurrent;
	HBITMAP 	m_hBmpOld;

	CSize 		m_size;                     // drawing helpers
	CPoint 		m_pt;
	BITMAP 		m_bmpInfo;
	CDC 		m_dcMem;
	BOOL 		m_bProcessingBitmap;

// Dialog Data
	//{{AFX_DATA(CAbout)
	enum { IDD = IDD_ABOUT };
	CHyperLink	m_HyperLink1;
	CHyperLink	m_HyperLink2;
	CRBGauge	m_gauge;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAbout)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAbout)
	afx_msg void OnRbgabout();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnStnClickedHyperlink2();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABOUT_H__09BF9084_1326_11D2_983E_00E018900F2A__INCLUDED_)
