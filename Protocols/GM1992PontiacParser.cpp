// GM1992PontiacParser.cpp: implementation of the CGM1992PontiacParser class.
//
//////////////////////////////////////////////////////////////////////
// (c) 1996-2000 Andy Whittaker, Chester, England. 
// mail@andywhittaker.com
//
// This contains all low-level parsing functions.

#include "stdafx.h"
#include "..\FreeScan.h"
#include "..\MainDlg.h"
#include "GM1992PontiacParser.h"

#include "..\Supervisor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGM1992PontiacParser::CGM1992PontiacParser()
{
	m_pSupervisor = NULL;

	CFreeScanApp* pApp = (CFreeScanApp*) AfxGetApp();
	m_csCSVLogFile = pApp->GetProfileString("GM1992PontiacParser", "CSV Log Filename", "");

	m_dwCSVRecord = 0; // Initialise the CSV record number
	memset(m_ucDTC, 0, 3);// Reset DTC buffer
}

CGM1992PontiacParser::~CGM1992PontiacParser()
{
	if (m_file.m_hFile != CFile::hFileNull)
	{ // close our log file if it's open
		m_file.Flush();
		m_file.Close(); // close the logging file when we exit.
	}

	CFreeScanApp* pApp = (CFreeScanApp*) AfxGetApp();
	pApp->WriteProfileString("GM1992PontiacParser", "CSV Log Filename", m_csCSVLogFile);
}

// Starts or stops csv logging to file
BOOL CGM1992PontiacParser::StartCSVLog(BOOL bStart)
{
	CString csBuf = "";

	if (!bStart)
	{ // we want to close the logging file
		if (m_file.m_hFile != CFile::hFileNull)
		{
			WriteStatusLogged("CSV Log file has been stopped");
			m_file.Close(); // close the logging file when we exit.
		}
		else
			WriteStatusLogged("CSV Log file is already closed");

		return FALSE;
	}

	// We now must want to log to a file

	// Construct our File Dialog
	CFileDialog		Dialog(FALSE, "csv", 
						m_csCSVLogFile, 
						OFN_HIDEREADONLY | OFN_LONGNAMES | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT,
						"log Files (*.csv)|*.csv|All Files (*.*)|*.*||", NULL);

	// Change the title
	Dialog.m_ofn.lpstrTitle = "Create/Open CSV Log File";

	// Display the dialog box
	if (Dialog.DoModal() == IDOK)
	{
		m_csCSVLogFile = Dialog.GetPathName();

		if (!m_file.Open(m_csCSVLogFile, CFile::modeCreate | CFile::modeReadWrite | CFile::typeText))
		{
			csBuf.Format("Cannot open %s", m_csCSVLogFile);
			WriteStatus(csBuf);
			AfxMessageBox(csBuf, MB_OK | MB_ICONSTOP );
			return FALSE;
		}

		WriteStatusLogged("CSV Log file has been opened");
		WriteCSV(TRUE);
	}
	else // User pressed cancel
		WriteStatus("User cancelled CSV log file");

	if (m_file.m_hFile != NULL)
		return TRUE;
	else
		return FALSE;
}

//WriteCSV(..) Writes CSV data to our log file.
//If bTitle is true, the first title line is written, otherwise the data is written.
void CGM1992PontiacParser::WriteCSV(BOOL bTitle) 
{
	if (m_file.m_pStream == NULL) return;// i.e. no file open

	CString	csBuf;
	if (bTitle)
	{
		m_dwCSVRecord = 0;
		csBuf.Format("1992 Pontiac Sample,Coolant Sensor Volts,Start Water Temp,TPS Volts,Desired Idle,RPM,Road Speed,O2 Left,O2 Right,Injector Base PW,IAC,Barometric Air Pressure,MAP,Air:Fuel Ratio,TPS,MAT Volts,Knock Retard,Knock Count,Battery Volts,Engine Load,Spark Timing,Coolant Temp,MAT,Engine Running Time");
	}
	else
	{
		csBuf.Format("%ld,%4.2f,%3.1f,%4.2f,%d,%d,%d,%5.3f,%5.3f,%d,%d,%4.2f,%4.2f,%3.1f,%d,%4.2f,%3.1f,%d,%3.1f,%d,%3.1f,%3.1f,%3.1f,%d",
			m_dwCSVRecord,m_pSupervisor->m_fWaterVolts,m_pSupervisor->m_fStartWaterTemp,m_pSupervisor->m_fThrottleVolts,
			m_pSupervisor->m_iDesiredIdle,m_pSupervisor->m_iRPM,m_pSupervisor->m_iMPH,m_pSupervisor->m_fO2VoltsLeft,m_pSupervisor->m_fO2VoltsRight,
			m_pSupervisor->m_iInjectorBasePWMsL,m_pSupervisor->m_iIACPosition,
			m_pSupervisor->m_fBaro,m_pSupervisor->m_fMAP,m_pSupervisor->m_fAFRatio,m_pSupervisor->m_iThrottlePos,
			m_pSupervisor->m_fMATVolts,m_pSupervisor->m_fKnockRetard,m_pSupervisor->m_iKnockCount,m_pSupervisor->m_fBatteryVolts,m_pSupervisor->m_iEngineLoad,m_pSupervisor->m_fSparkAdvance,
			m_pSupervisor->m_fWaterTemp,m_pSupervisor->m_fMATTemp,m_pSupervisor->m_iRunTime);
		m_dwCSVRecord++;
	}
	csBuf = csBuf + "\n"; // Line Feed because we're logging to disk
	m_file.WriteString(csBuf);
}

// Tells the Supervisor that there's been a data change
void CGM1992PontiacParser::UpdateDialog(void)
{
	m_pSupervisor->UpdateDialog();
	WriteCSV(FALSE); // log our data to a csv file
}

// Parses the buffer into real data
int CGM1992PontiacParser::Parse(unsigned char* buffer, int iLength)
{
	WriteASCII(buffer, iLength); // Log the activity

	int	iIndex=0, iPacketLength;
	unsigned char* pPacketStart=NULL;

	// Scan the whole packet for its header.
	for(iIndex=0;iIndex<iLength;iIndex++)
	{
		pPacketStart = buffer + iIndex; // Marks the start of Packet for the CRC.
		switch(buffer[iIndex])
		{ // Find valid headers. Length parameter is always the 2nd byte.
		case ECU_HEADER_GM1992Pontiac: // Packet Header Start
			{
				iIndex++; // now find the length
				unsigned char ucLenByte = 0;
				ucLenByte = buffer[iIndex];
				iPacketLength = GetLength((int)ucLenByte); // Length of data
				iIndex++; // This has the mode number
				if (iPacketLength != 0)
				{// Data in Header
					if(buffer[iIndex]==1) // Main data-stream
					{
						if(ucLenByte==0x95) // length 64 including mode byte
							ParseMode1_0(&buffer[iIndex], iPacketLength);
					}
					else if(buffer[iIndex]==2) // length x including mode byte
						ParseMode2(&buffer[iIndex], iPacketLength);
					else if(buffer[iIndex]==3) // length max x including mode byte
						ParseMode3(&buffer[iIndex], iPacketLength);
					else if(buffer[iIndex]==4) // length max x including mode byte
						ParseMode4(&buffer[iIndex], iPacketLength);
					else if(buffer[iIndex]==7) // length max 2 including mode byte
						ParseMode7(&buffer[iIndex], iPacketLength);
					else if(buffer[iIndex]==8) // length max 1 including mode byte
						ParseMode8(&buffer[iIndex], iPacketLength);
					else if(buffer[iIndex]==9) // length max 1 including mode byte
						ParseMode9(&buffer[iIndex], iPacketLength);
					else if(buffer[iIndex]==10) // length max 1 including mode byte
						ParseMode10(&buffer[iIndex], iPacketLength);
					else
					{
						CString	temp; // write to the spy window
						temp.Format("Unrecognised Mode: %02x", buffer[iIndex]);
						WriteStatus(temp);
					}
					iIndex += iPacketLength;
				}
				// Check CRC (should always be correct by this point)
				if (!CheckChecksum(pPacketStart, 3 + iPacketLength))
					WriteStatus("Checksum Error on 0xf4");
				break;
			}
		case ECU_CHATTER_HEADER_GM1992Pontiac: // from chatter
			{
				iIndex++; // now find the length
				iPacketLength = GetLength((int)buffer[iIndex]); // Length of data
				iIndex++; // This has the mode number
				// No data so don't parse.
				ParseModeF0(&buffer[iIndex], iPacketLength);
				iIndex += iPacketLength; // should be 3
				// Check CRC
				if (!CheckChecksum(pPacketStart, 3 + iPacketLength))
					WriteStatus("Checksum Error on 0xf0");
				break;
			}
		case 0x05: // from chatter
			{
				iIndex++; // now find the length
				iPacketLength = GetLength((int)buffer[iIndex]); // Length of data
				iIndex++; // This has the mode number
				ParseMode05(&buffer[iIndex], iPacketLength);
				iIndex += iPacketLength; // should be 3
				// Check CRC
				if (!CheckChecksum(pPacketStart, 3 + iPacketLength))
					WriteStatus("Checksum Error on 0x05");
				break;
			}
		case 0x0a: // from chatter
			{
				iIndex++; // now find the length
				iPacketLength = GetLength((int)buffer[iIndex]); // Length of data
				iIndex++; // This has the mode number
				ParseMode0A(&buffer[iIndex], iPacketLength);
				iIndex += iPacketLength; // should be 3
				// Check CRC
				if (!CheckChecksum(pPacketStart, 3 + iPacketLength))
					WriteStatus("Checksum Error on 0x0a");
				break;
			}
		default:
			{
			CString buf;
			buf.Format("%02x <- Unrecognised Header", buffer[iIndex]);
			WriteStatus(buf);
			}
		}// Switch
	}// for()

	// Force the main application to update itself
	UpdateDialog();

	return iLength; // Successfully parsed.
}

// Translates the incoming data stream from the chatter
void CGM1992PontiacParser::ParseModeF0(unsigned char* buffer, int len)
{
}

// Translates the incoming data stream from the chatter
void CGM1992PontiacParser::ParseMode05(unsigned char* buffer, int len)
{
}

// Translates the incoming data stream from the chatter
void CGM1992PontiacParser::ParseMode0A(unsigned char* buffer, int len)
{
}

// Translates the incoming data stream as Mode 1 Msg 0
void CGM1992PontiacParser::ParseMode1_0(unsigned char* buffer, int len)
{
	int iIndex;

	if (len<10) // remember half duplex. We read our commands as well
	{
		WriteStatus("Received our TX command echo for mode 1 Msg 0.");
		return;
	}
	else if (len>64)
	{
		WriteStatus("Warning: F001 larger than expected, packet truncated.");
		len = 64;
	}
	// copy buffer into raw data array
	for(iIndex=0; iIndex<len; iIndex++)
		m_pSupervisor->m_ucF001[iIndex]=buffer[iIndex];

	// Work out real-world data from the packet.
	// Mode number is in index 0

	// Status Word 1
	if (buffer[63] & 0x80)
		m_pSupervisor->m_bEngineClosedLoop = TRUE;  // bit 7
	else
		m_pSupervisor->m_bEngineClosedLoop = FALSE; // bit 7

	if (buffer[59] & 0x80)
		m_pSupervisor->m_bACRequest = TRUE;  // mode 1, byte 59, bit 7
	else
		m_pSupervisor->m_bACRequest = FALSE; // mode 1, byte 59, bit 7

	if (buffer[54] & 0x20)
		m_pSupervisor->m_bACClutch = TRUE;  // mode 1, byte 54, bit 5
	else
		m_pSupervisor->m_bACClutch = FALSE; // mode 1, byte 54, bit 5

	m_pSupervisor->m_iEpromID = (int)buffer[2] + ((int)buffer[1] * 256);
	// Analogues
	m_ucDTC[0] = buffer[3]; // Fault code byte 1
	m_ucDTC[1] = buffer[4]; // Fault code byte 2
	m_ucDTC[2] = buffer[5]; // Fault code byte 3
	m_ucDTC[3] = buffer[6]; // Fault code byte 4
	m_pSupervisor->m_fWaterTemp = ((float)buffer[7] * (float)0.75) - (float)40.0; // in �C
	m_pSupervisor->m_fWaterVolts = (float)(((float)buffer[7] / (float)255.0) * (float) 5.0);
	m_pSupervisor->m_iWaterTempADC = buffer[7];
	m_pSupervisor->m_fStartWaterTemp = ((float)buffer[8] * (float)0.75) - (float)40.0; // in �C
	m_pSupervisor->m_fThrottleVolts = (float)(((float)buffer[9] / (float)255.0) * (float) 5.0);
	m_pSupervisor->m_iThrottleADC = buffer[9];
	m_pSupervisor->m_iThrottlePos = (int)((float)buffer[10] / (float)2.55);
	m_pSupervisor->m_iRPM = buffer[11] * 25;
	m_pSupervisor->m_iMPH = (int)buffer[17]; // Count is in MPH
	m_pSupervisor->m_fO2VoltsLeft = (float) buffer[19] * (float) 4.42; // 1st Bank
	m_pSupervisor->m_iRichLeanCounterL = (int)buffer[20];
	m_pSupervisor->m_iBLM = (int)buffer[22];
	m_pSupervisor->m_iBLMCell = (int)buffer[23];
	m_pSupervisor->m_iIntegratorL = (int)buffer[24];
	m_pSupervisor->m_iIACPosition = (int)buffer[25];
	m_pSupervisor->m_iDesiredIdle = (int)((float)buffer[27] * (float) 12.5);
	m_pSupervisor->m_fBaro = (((float)buffer[28] + (float)28.06) / (float)271); // in Bar Absolute
	m_pSupervisor->m_fBaroVolts = ((float)buffer[28] / (float) 255.0) * (float) 5.0; // in Volts
	m_pSupervisor->m_iBaroADC = buffer[28];
	m_pSupervisor->m_fMAP = (((float)buffer[29] + (float)28.06) / (float)271); // in Bar Absolute
	m_pSupervisor->m_fMAPVolts = ((float)buffer[29] / (float) 255.0) * (float) 5.0; // in Volts
	m_pSupervisor->m_iMAPADC = buffer[29];
	m_pSupervisor->m_fMATVolts = ((float)buffer[30] / (float)255.0) * (float)5.0; // in Volts
	m_pSupervisor->m_iMATADC = buffer[30];
	m_pSupervisor->m_fMATTemp = ReturnTemp(buffer[30]); // in �C
	m_pSupervisor->m_fBatteryVolts = (float)buffer[34] / (float)10.0;
	m_pSupervisor->m_fAirFlow = (float)buffer[37];
	m_pSupervisor->m_fSparkAdvance = (((float)buffer[40] * (float)90.0) / (float)256.0); // in �
	m_pSupervisor->m_fAFRatio = ((float)(buffer[41]) / (float)10.0); // Air Fuel Ratio
	m_pSupervisor->m_iInjectorBasePWMsL = (int) ( (float)((buffer[42] * 256) + buffer[43]) / (float)65.536);
	m_pSupervisor->m_fKnockRetard = (((float)buffer[46] * (float)45.0) / (float)256.0); // in �
	m_pSupervisor->m_iKnockCount = (int)buffer[51];
	m_pSupervisor->m_iRunTime = (buffer[49] * 256) + buffer[50]; // Total running time

	ParseDTCs(); // Process the DTCs into text
}

// Translates the incoming data stream as Mode 2
void CGM1992PontiacParser::ParseMode2(unsigned char* buffer, int len)
{
	int iIndex;

	if (len==0) // remember half duplex. We read our commands as well
	{
		WriteStatus("0 Received our TX command echo for mode 2.");
		return;
	}
	else if (len==1) // remember half duplex. We read our commands as well
	{
		WriteStatus("1 Received our TX command echo for mode 2.");
		return;
	}
	else if (len>65)
	{
		WriteStatus("Warning: F002 larger than expected, packet truncated.");
		len = 65;
	}
	// copy buffer into raw data array
	for(iIndex=0; iIndex<len; iIndex++)
		m_pSupervisor->m_ucF002[iIndex]=buffer[iIndex];

	// Mode number is in index 0
	// Work out real-world data from the packet.
}

// Translates the incoming data stream as Mode 3
void CGM1992PontiacParser::ParseMode3(unsigned char* buffer, int len)
{
	int iIndex;

	if (len==0) // remember half duplex. We read our commands as well
	{
		WriteStatus("0 Received our TX command echo for mode 3.");
		return;
	}
	else if (len==1) // remember half duplex. We read our commands as well
	{
		WriteStatus("1 Received our TX command echo for mode 3.");
		return;
	}
	else if (len>11)
	{
		WriteStatus("Warning: F003 larger than expected, packet truncated.");
		len = 11;
	}
	// copy buffer into raw data array
	for(iIndex=0; iIndex<len; iIndex++)
		m_pSupervisor->m_ucF003[iIndex]=buffer[iIndex];

	// Mode number is in index 0
	// Work out real-world data from the packet.
	m_ucDTC[0] = buffer[3]; // Fault code byte 1
	m_ucDTC[1] = buffer[4]; // Fault code byte 2
	m_ucDTC[2] = buffer[5]; // Fault code byte 3
	m_ucDTC[3] = buffer[6]; // Fault code byte 4

	ParseDTCs(); // Process the DTCs into text
}

// Translates the incoming data stream as Mode 4
void CGM1992PontiacParser::ParseMode4(unsigned char* buffer, int len)
{
	int iIndex;

	if (len==0) // remember half duplex. We read our commands as well
	{
		WriteStatus("0 Received our TX command echo for mode 4.");
		return;
	}
	else if (len==1) // remember half duplex. We read our commands as well
	{
		WriteStatus("1 Received our TX command echo for mode 4.");
		return;
	}
	else if (len>11)
	{
		WriteStatus("Warning: F004 larger than expected, packet truncated.");
		len = 11;
	}
	// copy buffer into raw data array
	for(iIndex=0; iIndex<len; iIndex++)
		m_pSupervisor->m_ucF004[iIndex]=buffer[iIndex];

	// Mode number is in index 0
	// Work out real-world data from the packet.
}

// Translates the incoming data stream as Mode 7
void CGM1992PontiacParser::ParseMode7(unsigned char* buffer, int len)
{
	if (len==0) // remember half duplex. We read our commands as well
	{
		WriteStatus("0 Received our TX command echo for mode 7.");
		return;
	}
	else if (len==1) // remember half duplex. We read our commands as well
	{
		WriteStatus("1 Received our TX command echo for mode 7.");
		return;
	}
	else if (len>2)
	{
		WriteStatus("Warning: F007 larger than expected, packet truncated.");
		len = 2;
	}

	// Mode number is in index 0
	// Work out real-world data from the packet.
}

// Translates the incoming data stream as Mode 8
void CGM1992PontiacParser::ParseMode8(unsigned char* buffer, int len)
{
	if (len==0) // remember half duplex. We read our commands as well
	{
		WriteStatus("0 Received our TX command echo for mode 8.");
		return;
	}
	else if (len==1) // remember half duplex. We read our commands as well
	{
		WriteStatus("1 Received our TX command echo for mode 8.");
		return;
	}
	else if (len>1)
	{
		WriteStatus("Warning: F008 larger than expected, packet truncated.");
		len = 1;
	}

	// Mode number is in index 0
	// Work out real-world data from the packet.
}

// Translates the incoming data stream as Mode 9
void CGM1992PontiacParser::ParseMode9(unsigned char* buffer, int len)
{
	if (len==0) // remember half duplex. We read our commands as well
	{
		WriteStatus("0 Received our TX command echo for mode 9.");
		return;
	}
	else if (len==1) // remember half duplex. We read our commands as well
	{
		WriteStatus("1 Received our TX command echo for mode 9.");
		return;
	}
	else if (len>1)
	{
		WriteStatus("Warning: F009 larger than expected, packet truncated.");
		len = 1;
	}

	// Mode number is in index 0
	// Work out real-world data from the packet.
}

// Translates the incoming data stream as Mode 10
void CGM1992PontiacParser::ParseMode10(unsigned char* buffer, int len)
{
	if (len==0) // remember half duplex. We read our commands as well
	{
		WriteStatus("0 Received our TX command echo for mode 10.");
		return;
	}
	else if (len==1) // remember half duplex. We read our commands as well
	{
		WriteStatus("1 Received our TX command echo for mode 10.");
		return;
	}
	else if (len>1)
	{
		WriteStatus("Warning: F010 larger than expected, packet truncated.");
		len = 1;
	}

	// Mode number is in index 0
	// Work out real-world data from the packet.
}

// Translates the DTC Codes
void CGM1992PontiacParser::ParseDTCs(void)
{
	CString buf; // Temporary Buffer

	m_pSupervisor->m_csDTC.Empty();

	if ((m_ucDTC[0] == 0) && (m_ucDTC[1] == 0) && (m_ucDTC[2] == 0) && (m_ucDTC[3] == 0))
		m_pSupervisor->m_csDTC = "No reported faults.";
	else
	{
		m_pSupervisor->m_csDTC = "The following faults are reported:\n";
		
		// Now print the fault-codes
		// MALFFW1     LOGGED MALF FLAG WORD 1
		if (m_ucDTC[0] & 0x80)
		{ //
			buf = "CODE 12  NO REFERENCE PULSE (ENGINE NOT RUNNING)";
			m_pSupervisor->m_csDTC += buf;
			m_pSupervisor->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x40)
		{ //
			buf = "CODE 13  OXYGEN SENSOR";
			m_pSupervisor->m_csDTC += buf;
			m_pSupervisor->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x20)
		{ //
			buf = "CODE 14  COOLANT SENSOR HIGH TEMPERATURE";
			m_pSupervisor->m_csDTC += buf;
			m_pSupervisor->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x10)
		{ //
			buf = "CODE 15  COOLANT SENSOR LOW TEMPERATURE";
			m_pSupervisor->m_csDTC += buf;
			m_pSupervisor->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x08)
		{ //
			buf = "CODE 21  THROTTLE POSITION SENSOR HI";
			m_pSupervisor->m_csDTC += buf;
			m_pSupervisor->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x04)
		{ //
			buf = "CODE 22  THROTTLE POSITION SENSOR LOW";
			m_pSupervisor->m_csDTC += buf;
			m_pSupervisor->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x02)
		{ //
			buf = "CODE 23  LOW MANIFOLD AIR TEMP";
			m_pSupervisor->m_csDTC += buf;
			m_pSupervisor->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x01)
		{ //
			buf = "CODE 24  VEHICLE SPEED SENSOR LOW";
			m_pSupervisor->m_csDTC += buf;
			m_pSupervisor->m_csDTC += "\n";
		}

		// MALFFW2     LOGGED MALF FLAG WORD 2
		if (m_ucDTC[1] & 0x80)
		{ //
			buf = "CODE 25  HI MANIFOLD AIR TEMP";
			m_pSupervisor->m_csDTC += buf;
			m_pSupervisor->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x40)
		{ //
			buf = "CODE 31  FUEL INJECTOR MALF";
			m_pSupervisor->m_csDTC += buf;
			m_pSupervisor->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x20)
		{ //
			buf = "CODE 32  EVRV/EGR";
			m_pSupervisor->m_csDTC += buf;
			m_pSupervisor->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x10)
		{ //
			buf = "CODE 33  MAP SENSOR HIGH";
			m_pSupervisor->m_csDTC += buf;
			m_pSupervisor->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x08)
		{ //
			buf = "CODE 34  MAP SENSOR LOW";
			m_pSupervisor->m_csDTC += buf;
			m_pSupervisor->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x04)
		{ //
			buf = "CODE 35  IAC ERROR";
			m_pSupervisor->m_csDTC += buf;
			m_pSupervisor->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x02)
		{ //
			buf = "CODE 41  CYLINDER SELECT ERROR";
			m_pSupervisor->m_csDTC += buf;
			m_pSupervisor->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x01)
		{ //
			buf = "CODE 42  EST MONITOR ERROR";
			m_pSupervisor->m_csDTC += buf;
			m_pSupervisor->m_csDTC += "\n";
		}

		// MALFFW3     LOGGED MALF FLAG WORD 3
		if (m_ucDTC[2] & 0x80)
		{ //
			buf = "CODE 43  ESC FAILURE";
			m_pSupervisor->m_csDTC += buf;
			m_pSupervisor->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x40)
		{ //
			buf = "CODE 44  OXYGEN SENSOR LEAN";
			m_pSupervisor->m_csDTC += buf;
			m_pSupervisor->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x20)
		{ //
			buf = "CODE 45  OXYGEN SENSOR RICH";
			m_pSupervisor->m_csDTC += buf;
			m_pSupervisor->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x10)
		{ //
			buf = "CODE 51  PROM ERROR";
			m_pSupervisor->m_csDTC += buf;
			m_pSupervisor->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x08)
		{ //
			buf = "CODE 52  NOT USED";
			m_pSupervisor->m_csDTC += buf;
			m_pSupervisor->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x04)
		{ //
			buf = "CODE 53  HIGH BATTERY VOLTAGE";
			m_pSupervisor->m_csDTC += buf;
			m_pSupervisor->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x02)
		{ //
			buf = "CODE 54  FUEL PUMP VOLTAGE LOW";
			m_pSupervisor->m_csDTC += buf;
			m_pSupervisor->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x01)
		{ //
			buf = "CODE 55  ADU ERROR";
			m_pSupervisor->m_csDTC += buf;
			m_pSupervisor->m_csDTC += "\n";
		}

		// MALFFW4     LOGGED MALF FLAG WORD 4
		if (m_ucDTC[3] & 0x80)
		{ //
			buf = "39  MANUAL CLUTCH SWITCH FAIL";
			m_pSupervisor->m_csDTC += buf;
			m_pSupervisor->m_csDTC += "\n";
		}
		if (m_ucDTC[3] & 0x40)
		{ //
			buf = "61  DEGRADED O2 SENSOR";
			m_pSupervisor->m_csDTC += buf;
			m_pSupervisor->m_csDTC += "\n";
		}
		if (m_ucDTC[3] & 0x20)
		{ //
			buf = "46  VATS FAILED";
			m_pSupervisor->m_csDTC += buf;
			m_pSupervisor->m_csDTC += "\n";
		}
		if (m_ucDTC[3] & 0x10)
		{ //
			buf = "62  TRANSMISSION GEAR SWITCH FAIL";
			m_pSupervisor->m_csDTC += buf;
			m_pSupervisor->m_csDTC += "\n";
		}
		if (m_ucDTC[3] & 0x08)
		{ //
			buf = "63  F31 SOLENOID 'B' FAILED OFF";
			m_pSupervisor->m_csDTC += buf;
			m_pSupervisor->m_csDTC += "\n";
		}
		if (m_ucDTC[3] & 0x04)
		{ //
			buf = "65  NOT USED";
			m_pSupervisor->m_csDTC += buf;
			m_pSupervisor->m_csDTC += "\n";
		}
		if (m_ucDTC[3] & 0x02)
		{ //
			buf = "66  A/C PRESSURE SENSOR FAIL";
			m_pSupervisor->m_csDTC += buf;
			m_pSupervisor->m_csDTC += "\n";
		}
		if (m_ucDTC[3] & 0x01)
		{ //
			buf = "56  NOT USED";
			m_pSupervisor->m_csDTC += buf;
			m_pSupervisor->m_csDTC += "\n";
		}

	}

}
