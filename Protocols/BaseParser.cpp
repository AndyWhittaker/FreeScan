
#include "BaseParser.h"

CBaseParser::CBaseParser(CBaseProtocol* const pBaseProtocol): m_pBaseProtocol(pBaseProtocol) {
}

CBaseParser::~CBaseParser() {
}

void CBaseParser::WriteStatus(const CString csText) {
	m_pBaseProtocol->WriteStatus(csText);
}

void CBaseParser::WriteASCII(const unsigned char* const buffer, const int ilength) {
	m_pBaseProtocol->WriteASCII(buffer, ilength);
}

