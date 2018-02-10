// Parser.h: interface for the CGMA143Parser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GMA143PARSER_H__19F33D4B_4031_11D3_9828_0080C83832F8__INCLUDED_)
#define AFX_GMA143PARSER_H__19F33D4B_4031_11D3_9828_0080C83832F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GMBaseFunctions.h"

#define ECU_HEADER_GMA143					0xf0

class CSupervisor;
class CGMA143Parser : public CGMBaseFunctions
{
public:
	CGMA143Parser();
	virtual ~CGMA143Parser();
	
	int Parse(unsigned char*, int iLength);

protected:
	// Protected pointers
	CSupervisor*	m_pSupervisor; // pointer to the owner.
	unsigned char	m_ucDTC[6]; // Fault codes buffer

protected:
	void ParseADC(unsigned char* buffer, int len);
	void ParseAnalogues(unsigned char* buffer, int len);
	void ParseMode1(unsigned char* buffer, int len);
	void ParseMode2(unsigned char* buffer, int len);
	void ParseMode3(unsigned char* buffer, int len);
	void ParseMode4(unsigned char* buffer, int len);
	void ParseDTCs(void);// Parse the DTCs

	// CSV Logging
	BOOL StartCSVLog(BOOL bStart);// Starts or stops csv logging to file
	CStdioFile	m_file;// File class for logging to disk
	CString		m_csCSVLogFile;// Filename for CSV logging
	DWORD		m_dwCSVRecord;//CSV record number

	void WriteCSV(BOOL bTitle);//Write CSV data to disk

	void UpdateDialog(void);// forces dialog to be updated
};

#endif // !defined(AFX_GMA143PARSER_H__19F33D4B_4031_11D3_9828_0080C83832F8__INCLUDED_)
