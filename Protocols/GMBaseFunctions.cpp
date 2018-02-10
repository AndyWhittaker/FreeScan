// GMBaseFuntions.cpp: implementation of the CGMBaseFunctions class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\freescan.h"
#include "..\StatusDlg.h"

#include "GMBaseFunctions.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

	static const double	m_fTemps[256] =   {
					151.3,149.9,148.6,147.3,146.0,144.7,143.4,142.1,
					140.8,139.4,138.1,136.8,135.5,134.2,132.9,131.6,
					130.3,128.9,127.6,126.3,125.0,123.7,122.4,121.1,
					119.8,118.4,117.1,115.8,114.5,113.2,111.9,110.6,
					109.3,108.2,107.1,106.0,104.9,103.9,102.8,101.7,
					100.6, 99.5, 98.5, 97.4, 96.3, 95.2, 94.2, 93.1,
					 92.0, 91.3, 90.6, 89.9, 89.2, 88.5, 87.8, 87.1,
					 86.4, 85.7, 85.0, 84.3, 83.6, 82.9, 82.2, 81.5,
					 80.8, 80.1, 79.5, 78.9, 78.3, 77.7, 77.1, 76.5,
					 75.9, 75.3, 74.7, 74.0, 73.4, 72.8, 72.2, 71.6,
					 71.0, 70.5, 70.0, 69.5, 68.9, 68.4, 67.9, 67.4,
					 66.9, 66.4, 65.8, 65.3, 64.8, 64.3, 63.8, 63.3,
					 62.8, 62.3, 61.9, 61.5, 61.1, 60.6, 60.2, 59.8,
					 59.4, 59.0, 58.5, 58.1, 57.7, 57.3, 56.8, 56.4,
					 56.0, 55.6, 55.3, 54.9, 54.5, 54.1, 53.8, 53.4,
					 53.0, 52.6, 52.3, 51.9, 51.5, 51.1, 50.8, 50.4,
					 50.0, 49.6, 49.2, 48.7, 48.3, 47.9, 47.5, 47.0,
					 46.6, 46.2, 45.8, 45.4, 44.9, 44.5, 44.1, 43.7,
					 43.3, 42.9, 42.5, 42.1, 41.8, 41.4, 41.0, 40.6,
					 40.3, 39.9, 39.5, 39.1, 38.8, 38.4, 38.0, 37.6,
					 37.3, 36.9, 36.5, 36.1, 35.8, 35.4, 35.0, 34.6,
					 34.3, 33.9, 33.5, 33.1, 32.8, 32.4, 32.0, 31.6,
					 31.3, 30.8, 30.3, 29.8, 29.4, 28.9, 28.4, 28.0,
					 27.5, 27.0, 26.6, 26.1, 25.6, 25.2, 24.7, 24.2,
					 23.8, 23.3, 22.8, 22.3, 21.9, 21.4, 20.9, 20.5,
					 20.0, 19.5, 19.1, 18.6, 18.1, 17.7, 17.2, 16.7,
					 16.3, 15.6, 14.9, 14.3, 13.6, 13.0, 12.3, 11.7,
					 10.0, 10.3,  9.7,  9.0,  8.3,  7.7,  7.1,  6.4,
					  5.8,  4.9,  4.0,  3.1,  2.2,  1.3,  0.4, -0.5,
					 -1.4, -2.3, -3.2, -4.0, -4.9, -5.8, -6.7, -7.6,
					 -8.5,-10.5,-12.4,-14.4,-16.4,-18.3,-20.3,-22.3,
					 -24.2,-26.2,-28.2,-30.2,-32.1,-34.1,-36.1,-38.0};

CGMBaseFunctions::CGMBaseFunctions()
{
	m_pStatusDlg = NULL;
}

CGMBaseFunctions::~CGMBaseFunctions()
{
}

// Writes a line of ASCII to the spy window
void CGMBaseFunctions::WriteStatus(CString csText)
{
	csText = "P: " + csText;
	m_pStatusDlg->WriteStatus(csText);
}

// Writes a line of binary as ASCII to the spy window
void CGMBaseFunctions::WriteASCII(unsigned char * buffer, int ilength)
{
	m_pStatusDlg->WriteASCII(buffer, ilength);
}

void CGMBaseFunctions::WriteLogEntry(LPCTSTR pstrFormat, ...)
{
	va_list args;
	va_start(args, pstrFormat);
	m_pStatusDlg->WriteLogEntry(pstrFormat, args);
}

void CGMBaseFunctions::WriteStatusLogged(CString csText) 
{
	m_pStatusDlg->WriteStatusLogged(csText);
}

// Calculates and checks the CRC
BOOL CGMBaseFunctions::CheckChecksum(unsigned char * buffer, int iLength)
{
	unsigned int sum = 0;
	int i;
	for (i = 0; i < iLength; i++) 
		sum += buffer[i];

	// convert to 8 bits
	sum = sum & 0xff;
	if (sum == 0)
		return TRUE;
	else
		return FALSE;
}

// Calculates and sets the correct CRC
void CGMBaseFunctions::SetChecksum(unsigned char * buffer, int iLength)
{
	unsigned char sum = 0;
	int i;
	// add up all data except the CRC
	for (i = 0; i < iLength - 1; i++) 
		sum += buffer[i];

	// CRC must be this...
	buffer[iLength - 1] = (unsigned char) 0x00 - sum;
}

// returns length of Packet
int CGMBaseFunctions::GetLength(int iLength)
{
		if (iLength < 0xA0)
			iLength = iLength - 0x55;
		else
			iLength = iLength - 0xc6;
		return iLength; // Length of data
}

// Returns the correct Analogue Temperature from an 8 bit sensor
float CGMBaseFunctions::ReturnTemp(int iADC)
{
	return (float)m_fTemps[iADC];
}
