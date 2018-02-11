// Parser.h: interface for the CGM1994CamaroZ28Parser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GM1994CamaroZ28PARSER_H__19F33D4B_4031_11D3_9828_0080C83832F8__INCLUDED_)
#define AFX_GM1994CamaroZ28PARSER_H__19F33D4B_4031_11D3_9828_0080C83832F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GMBaseFunctions.h"

#define ECU_HEADER_GM1994CAMAROZ28				0xf4
#define ECU_CHATTER_HEADER_GM1994CAMAROZ28		0xf0

class CSupervisor;
class CGM1994CamaroZ28Parser : public CGMBaseFunctions
{
public:
	CGM1994CamaroZ28Parser();
	virtual ~CGM1994CamaroZ28Parser();

	
	int Parse(unsigned char*, int iLength);

protected:
	// Protected pointers
	CSupervisor*	m_pSupervisor; // pointer to the owner.
	unsigned char	m_ucDTC[8]; // Fault codes buffer

protected:
	//Implementation
	void ParseMode0A(unsigned char* buffer, int len);
	void ParseCMode10(unsigned char* buffer, int len);
	void ParseMode40(unsigned char* buffer, int len);
	void ParseMode41(unsigned char* buffer, int len);
	void ParseMode45(unsigned char* buffer, int len);
	void ParseMode90(unsigned char* buffer, int len);
	void ParseMode92(unsigned char* buffer, int len);
	void ParseMode93(unsigned char* buffer, int len);
	void ParseMode1_0(unsigned char* buffer, int len);
	void ParseMode1_1(unsigned char* buffer, int len);
	void ParseMode1_2(unsigned char* buffer, int len);
	void ParseMode1_4(unsigned char* buffer, int len);
	void ParseMode1_6(unsigned char* buffer, int len);
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

#endif // !defined(AFX_GM1994CamaroZ28PARSER_H__19F33D4B_4031_11D3_9828_0080C83832F8__INCLUDED_)