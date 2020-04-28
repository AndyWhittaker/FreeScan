// Parser.h: interface for the CGMA140Parser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GMA140Parser_H__19F33D4B_4031_11D3_9828_0080C83832F8__INCLUDED_)
#define AFX_GMA140Parser_H__19F33D4B_4031_11D3_9828_0080C83832F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseDefines.h"

#include "BaseParser.h"
#include "BaseProtocol.h"

#define ECU_HEADER_GMA140				0xf4
#define ECU_CHATTER_HEADER_GMA140		0xf4

class CGMA140Parser : public CBaseParser
{
public:
	CGMA140Parser(CBaseProtocol* const pProtocol);
	virtual ~CGMA140Parser();

	void InitializeSupportedValues(CEcuData* const ecuData);
	BOOL Parse(const unsigned char* const buffer, int const length, CEcuData* const ecuData);

private:
	unsigned char	m_ucDTC[4]; // Fault codes buffer

private:
	BOOL ParseADC(const unsigned char* buffer, int len, CEcuData* const ecuData);
	BOOL ParseAnalogues(const unsigned char* buffer, int len, CEcuData* const ecuData);
	BOOL ParseMode1(const unsigned char* buffer, int len, CEcuData* const ecuData);
	BOOL ParseMode2(const unsigned char* buffer, int len, CEcuData* const ecuData);
	BOOL ParseMode3(const unsigned char* buffer, int len, CEcuData* const ecuData);
	BOOL ParseMode4(const unsigned char* buffer, int len, CEcuData* const ecuData);
	BOOL ParseMode7(const unsigned char* buffer, int len, CEcuData* const ecuData);
	BOOL ParseMode8(const unsigned char* buffer, int len, CEcuData* const ecuData);
	BOOL ParseMode9(const unsigned char* buffer, int len, CEcuData* const ecuData);
	BOOL ParseMode10(const unsigned char* buffer, int len, CEcuData* const ecuData);
	void ParseDTCs(CEcuData *const ecuData);// Parse the DTCs
};

#endif // !defined(AFX_GMA140Parser_H__19F33D4B_4031_11D3_9828_0080C83832F8__INCLUDED_)
