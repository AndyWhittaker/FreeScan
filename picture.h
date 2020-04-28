#if !defined(AFX_PICTURE_H__BB02C9B3_6A81_11D3_9870_0080C83832F8__INCLUDED_)
#define AFX_PICTURE_H__BB02C9B3_6A81_11D3_9870_0080C83832F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseDefines.h"

#include <afxdisp.h>

class CPicture : public COleDispatchDriver
{
public:
	CPicture() {}		// Calls COleDispatchDriver default constructor
	CPicture(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CPicture(const CPicture& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	long GetHandle();
	long GetHPal();
	void SetHPal(long);
	short GetType();
	long GetWidth();
	long GetHeight();

// Operations
public:
	// method 'Render' not emitted because of invalid return type or parameter type
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PICTURE_H__BB02C9B3_6A81_11D3_9870_0080C83832F8__INCLUDED_)
