#if !defined(AFX_TTPROPERTYPAGE_H__7BF1BF57_1757_11D2_81A9_444553540000__INCLUDED_)
#define AFX_TTPROPERTYPAGE_H__7BF1BF57_1757_11D2_81A9_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TTPropertyPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTTPropertyPage dialog
#include "Resource.h"
#include "OXToolTipCtrl.h"

class CTTPropertyPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CTTPropertyPage)

// Construction
public:
	CTTPropertyPage();
	CTTPropertyPage(UINT uID);   // non-standard constructor
	~CTTPropertyPage();

// Dialog Data
	//{{AFX_DATA(CTTPropertyPage)
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	BOOL	m_bBoldTips;
	BOOL	m_bSetMaxWidth;
	UINT	m_nMaxWidth;
	//}}AFX_DATA
	COLORREF m_clrBackground;
	COLORREF m_clrText;
	CFont m_font;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTTPropertyPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTTPropertyPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
    BOOL OnToolTipNotify(UINT id, NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL PreTranslateMessage( MSG* pMsg );
	DECLARE_MESSAGE_MAP()

	COXToolTipCtrl m_toolTip;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TTPROPERTYPAGE_H__7BF1BF57_1757_11D2_81A9_444553540000__INCLUDED_)
