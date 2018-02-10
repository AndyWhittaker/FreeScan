// Parser.h: interface for the CGM1989CorvetteParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GM1989CorvettePARSER_H__19F33D4B_4031_11D3_9828_0080C83832F8__INCLUDED_)
#define AFX_GM1989CorvettePARSER_H__19F33D4B_4031_11D3_9828_0080C83832F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GMBaseFunctions.h"

#define ECU_HEADER_GM1989Corvette				0x80
#define ECU_CHATTER_HEADER_GM1989Corvette		0x80

class CSupervisor;
class CGM1989CorvetteParser : public CGMBaseFunctions
{
public:
	CGM1989CorvetteParser();
	virtual ~CGM1989CorvetteParser();

	
	int Parse(unsigned char*, int iLength);

protected:
	// Protected pointers
	CSupervisor*	m_pSupervisor; // pointer to the owner.
	unsigned char	m_ucDTC[4]; // Fault codes buffer

protected:
	//Implementation
	void ParseMode0A(unsigned char* buffer, int len);
	void ParseMode90(unsigned char* buffer, int len);
	void ParseMode1_0(unsigned char* buffer, int len);
	void ParseMode2(unsigned char* buffer, int len);
	void ParseMode3(unsigned char* buffer, int len);
	void ParseMode4(unsigned char* buffer, int len);
	void ParseMode7(unsigned char* buffer, int len);
	void ParseMode8(unsigned char* buffer, int len);
	void ParseMode9(unsigned char* buffer, int len);
	void ParseMode10(unsigned char* buffer, int len);
	void ParseDTCs(void);// Parse the DTCs

	// CSV Logging
	BOOL StartCSVLog(BOOL bStart);// Starts or stops csv logging to file
	CStdioFile	m_file;// File class for logging to disk
	CString		m_csCSVLogFile;// Filename for CSV logging
	DWORD		m_dwCSVRecord;//CSV record number

	void WriteCSV(BOOL bTitle);//Write CSV data to disk

	void UpdateDialog(void);// forces dialog to be updated
};

#endif // !defined(AFX_GM1989CorvettePARSER_H__19F33D4B_4031_11D3_9828_0080C83832F8__INCLUDED_)
