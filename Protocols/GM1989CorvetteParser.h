// Parser.h: interface for the CGM1989CorvetteParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GM1989CorvettePARSER_H__19F33D4B_4031_11D3_9828_0080C83832F8__INCLUDED_)
#define AFX_GM1989CorvettePARSER_H__19F33D4B_4031_11D3_9828_0080C83832F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseDefines.h"

#include "BaseParser.h"
#include "BaseProtocol.h"

#define ECU_HEADER_GM1989Corvette				0x80
#define ECU_CHATTER_HEADER_GM1989Corvette		0x80

class CGM1989CorvetteParser : public CBaseParser
{
public:
	CGM1989CorvetteParser(CBaseProtocol* const pProtocol);
	virtual ~CGM1989CorvetteParser();

	void InitializeSupportedValues(CEcuData* const ecuData);
	BOOL Parse(const unsigned char* const buffer, int const length, CEcuData* const ecuData);

private:
	unsigned char	m_ucDTC[4]; // Fault codes buffer

private:
	//Implementation
	BOOL ParseMode0A(const unsigned char* buffer, int len, CEcuData* const ecuData);
	BOOL ParseMode90(const unsigned char* buffer, int len, CEcuData* const ecuData);
	BOOL ParseMode1_0(const unsigned char* buffer, int len, CEcuData* const ecuData);
	BOOL ParseMode2(const unsigned char* buffer, int len, CEcuData* const ecuData);
	BOOL ParseMode3(const unsigned char* buffer, int len, CEcuData* const ecuData);
	BOOL ParseMode4(const unsigned char* buffer, int len, CEcuData* const ecuData);
	BOOL ParseMode7(const unsigned char* buffer, int len, CEcuData* const ecuData);
	BOOL ParseMode8(const unsigned char* buffer, int len, CEcuData* const ecuData);
	BOOL ParseMode9(const unsigned char* buffer, int len, CEcuData* const ecuData);
	BOOL ParseMode10(const unsigned char* buffer, int len, CEcuData* const ecuData);
	void ParseDTCs(CEcuData *const ecuData);// Parse the DTCs
};

#endif // !defined(AFX_GM1989CorvettePARSER_H__19F33D4B_4031_11D3_9828_0080C83832F8__INCLUDED_)
