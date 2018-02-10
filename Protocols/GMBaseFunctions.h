// GMBaseFuntions.h: interface for the CGMBaseFunctions class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GMBASEFUNTIONS_H__E717A35F_1C87_4B07_838D_FE2B975D5DA3__INCLUDED_)
#define AFX_GMBASEFUNTIONS_H__E717A35F_1C87_4B07_838D_FE2B975D5DA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStatusDlg;

class CGMBaseFunctions : public CWnd
{
public:
	CGMBaseFunctions();
	virtual ~CGMBaseFunctions();

	CStatusDlg*		m_pStatusDlg; // for peep window

	//Implementation
	void WriteStatus(CString csText);
	void WriteASCII(unsigned char * buffer, int ilength);
	void WriteLogEntry(LPCTSTR pstrFormat, ...);
	void WriteStatusLogged(CString csText);
	BOOL CheckChecksum(unsigned char * buffer, int iLength);
	void SetChecksum(unsigned char * buffer, int iLength);// Calculates and sets the correct CRC
	int  GetLength(int iLength);
	float ReturnTemp(int iADC);
};

#endif // !defined(AFX_GMBASEFUNTIONS_H__E717A35F_1C87_4B07_838D_FE2B975D5DA3__INCLUDED_)
