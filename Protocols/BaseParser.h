#pragma once

#include "BaseDefines.h"

#include "BaseProtocol.h"

class CBaseParser : public CObject
{
private:
	CBaseProtocol* const m_pBaseProtocol;

protected:
	CBaseParser(CBaseProtocol* const m_pBaseProtocol);
	virtual ~CBaseParser();

	void WriteStatus(const CString csText);
	void WriteASCII(const unsigned char* const buffer, const int ilength);

public:
	virtual void InitializeSupportedValues(CEcuData* const ecuData) = 0;
	virtual BOOL Parse(const unsigned char* const buffer, int const length, CEcuData* const ecuData) = 0;
};