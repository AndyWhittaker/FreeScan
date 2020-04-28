#pragma once

#include "BaseDefines.h"

class CStatusWriter
{
public:
	virtual BOOL StartLog(BOOL bStart) = 0;

	virtual void Hide(BOOL yes) = 0;
	virtual BOOL IsHidden(void) = 0;

	virtual void WriteStatus(CString csText) = 0;
	virtual void WriteStatusTimeLogged(CString csText) = 0;
	virtual void WriteASCII(const unsigned char* const buffer, int ilength) = 0;
	virtual void WriteLogEntry(LPCTSTR pstrFormat, ...) = 0;
};

