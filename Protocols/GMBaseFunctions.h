// GMBaseFuntions.h: interface for the CGMBaseFunctions class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GMBASEFUNTIONS_H__E717A35F_1C87_4B07_838D_FE2B975D5DA3__INCLUDED_)
#define AFX_GMBASEFUNTIONS_H__E717A35F_1C87_4B07_838D_FE2B975D5DA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseDefines.h"

class CGMBaseFunctions
{
private:
	CGMBaseFunctions() = delete;
	virtual ~CGMBaseFunctions() = delete;

public:
	static BOOL CheckChecksum(const unsigned char* const buffer, int iLength);
	static void SetChecksum(unsigned char * buffer, int iLength);// Calculates and sets the correct CRC
	static int  GetLength(int iLength);
	static float ReturnTemp(int iADC);
};

#endif // !defined(AFX_GMBASEFUNTIONS_H__E717A35F_1C87_4B07_838D_FE2B975D5DA3__INCLUDED_)
