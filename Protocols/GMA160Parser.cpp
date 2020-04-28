// GMA160Parser.cpp: implementation of the CGMA160Parser class.
//
//////////////////////////////////////////////////////////////////////
// (c) 1996-2000 Andy Whittaker, Chester, England. 
// mail@andywhittaker.com
//
// This contains all low-level parsing functions.

#include "GMA160Parser.h"

#include "GMBaseFunctions.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGMA160Parser::CGMA160Parser(CBaseProtocol* const m_pProtocol) : CBaseParser(m_pProtocol) {
	memset(m_ucDTC, 0, sizeof(m_ucDTC));// Reset DTC buffer
}

CGMA160Parser::~CGMA160Parser() {
}

void CGMA160Parser::InitializeSupportedValues(CEcuData* const ecuData) {
	ecuData->m_csDTC = "No reported faults.";
	ecuData->m_bEngineClosedLoop = CEcuData::c_bSUPPORTED_BY_PROTOCOL;
	ecuData->m_bACRequest = CEcuData::c_bSUPPORTED_BY_PROTOCOL;
	ecuData->m_bACClutch = CEcuData::c_bSUPPORTED_BY_PROTOCOL;
	ecuData->m_iEpromID = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fWaterTemp = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_fStartWaterTemp = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_fThrottleVolts = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iThrottleADC = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iThrottlePos = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iRPM = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iMPH = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fO2VoltsLeft = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iRichLeanCounterL = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iBLM = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iIntegratorL = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iIACPosition = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fMATVolts = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iMATADC = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fMATTemp = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_fBatteryVolts = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_fSparkAdvance = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_fBaro = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_fBaroVolts = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iBaroADC = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fMAPVolts = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iMAPADC = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fMAP = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iInjectorBasePWMsL = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iDesiredIdle = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iKnockCount = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fKnockRetard = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iRunTime = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fAirFlow = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_fAFRatio = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iBoostPW = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
}

// Parses the buffer into real data
BOOL CGMA160Parser::Parse(const unsigned char* const buffer, int const length, CEcuData* const ecuData) {
	WriteASCII(buffer, length); // Log the activity

	int	iIndex=0, iPacketLength;
	const unsigned char* pPacketStart=NULL;

	BOOL dataWasUpdated = FALSE;

	// Scan the whole packet for its header.
	for(iIndex=0;iIndex< length;iIndex++)
	{
		pPacketStart = buffer + iIndex; // Marks the start of Packet for the CRC.
		switch(buffer[iIndex])
		{ // Find valid headers. Length parameter is always the 2nd byte.
		case ECU_HEADER_GMA160: // Packet Header Start
			{
				iIndex++; // now find the length
				unsigned char ucLenByte = 0;
				ucLenByte = buffer[iIndex];
				iPacketLength = CGMBaseFunctions::GetLength((int)ucLenByte); // Length of data
				iIndex++; // This has the mode number
				if (iPacketLength != 0)
				{// Data in Header
					if(buffer[iIndex]==1) // Main data-stream
					{
						if(ucLenByte==0x95) // length 67 including mode byte
							dataWasUpdated |= ParseMode1_0(&buffer[iIndex], iPacketLength, ecuData);
					}
					else if(buffer[iIndex]==2) // length x including mode byte
						dataWasUpdated |= ParseMode2(&buffer[iIndex], iPacketLength, ecuData);
					else if(buffer[iIndex]==3) // length max x including mode byte
						dataWasUpdated |= ParseMode3(&buffer[iIndex], iPacketLength, ecuData);
					else if(buffer[iIndex]==4) // length max x including mode byte
						dataWasUpdated |= ParseMode4(&buffer[iIndex], iPacketLength, ecuData);
					else if(buffer[iIndex]==7) // length max 2 including mode byte
						dataWasUpdated |= ParseMode7(&buffer[iIndex], iPacketLength, ecuData);
					else if(buffer[iIndex]==8) // length max 1 including mode byte
						dataWasUpdated |= ParseMode8(&buffer[iIndex], iPacketLength, ecuData);
					else if(buffer[iIndex]==9) // length max 1 including mode byte
						dataWasUpdated |= ParseMode9(&buffer[iIndex], iPacketLength, ecuData);
					else if(buffer[iIndex]==10) // length max 1 including mode byte
						dataWasUpdated |= ParseMode10(&buffer[iIndex], iPacketLength, ecuData);
					else
					{
						CString	temp; // write to the spy window
						temp.Format("Unrecognised Mode: %02x", buffer[iIndex]);
						WriteStatus(temp);
					}
					iIndex += iPacketLength;
				}
				// Check CRC (should always be correct by this point)
				if (!CGMBaseFunctions::CheckChecksum(pPacketStart, 3 + iPacketLength))
					WriteStatus("Checksum Error on 0xf4");
				break;
			}
//		case ECU_CHATTER_HEADER_GMA160: // from chatter
//			{
//				iIndex++; // now find the length
//				iPacketLength = GetLength((int)buffer[iIndex]); // Length of data
//				iIndex++; // This has the mode number
//				// No data so don't parse.
//				dataWasUpdated |= ParseMode0A(&buffer[iIndex], iPacketLength);
//				iIndex += iPacketLength; // should be 3
//				// Check CRC
//				if (!CheckChecksum(pPacketStart, 3 + iPacketLength))
//					WriteStatus("Checksum Error on 0xf0");
//				break;
//			}
		case 0x0a: // from chatter
			{
				iIndex++; // now find the length
				iPacketLength = CGMBaseFunctions::GetLength((int)buffer[iIndex]); // Length of data
				iIndex++; // This has the mode number
				dataWasUpdated |= ParseMode0A(&buffer[iIndex], iPacketLength, ecuData);
				iIndex += iPacketLength; // should be 3
				// Check CRC
				if (!CGMBaseFunctions::CheckChecksum(pPacketStart, 3 + iPacketLength))
					WriteStatus("Checksum Error on 0x0a");
				break;
			}
		case 0x90: // from chatter
			{
				iIndex++; // now find the length
				iPacketLength = CGMBaseFunctions::GetLength((int)buffer[iIndex]); // Length of data
				iIndex++; // This has the mode number
				dataWasUpdated |= ParseMode90(&buffer[iIndex], iPacketLength, ecuData);
				iIndex += iPacketLength; // should be 5
				// Check CRC
				if (!CGMBaseFunctions::CheckChecksum(pPacketStart, 3 + iPacketLength))
					WriteStatus("Checksum Error on 0x90");
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

	return dataWasUpdated;
}

// Translates the incoming data stream from the chatter
BOOL CGMA160Parser::ParseMode0A(const unsigned char* /*buffer*/, int /*len*/, CEcuData* const /*ecuData*/) {
	return FALSE;
}

// Translates the incoming data stream from the chatter
BOOL CGMA160Parser::ParseMode90(const unsigned char* /*buffer*/, int /*len*/, CEcuData* const /*ecuData*/) {
	return FALSE;
}

// Translates the incoming data stream as Mode 1 Msg 0
BOOL CGMA160Parser::ParseMode1_0(const unsigned char* buffer, int len, CEcuData* const ecuData) {
	if (len<10) // remember half duplex. We read our commands as well
	{
		WriteStatus("Received our TX command echo for mode 1 Msg 0.");
		return FALSE;
	}
	else if (len>67)
	{
		WriteStatus("Warning: F001 larger than expected, packet truncated.");
		len = 67;
	}
	
	ecuData->copyToF001(buffer, len);

	// Work out real-world data from the packet.
	// Mode number is in index 0

	// Status Word 1
	if (buffer[63] & 0x80)
		ecuData->m_bEngineClosedLoop = TRUE;  // bit 7
	else
		ecuData->m_bEngineClosedLoop = FALSE; // bit 7

	if (buffer[61] & 0x80)
		ecuData->m_bACRequest = TRUE;  // mode 1, byte 59, bit 7
	else
		ecuData->m_bACRequest = FALSE; // mode 1, byte 59, bit 7

	if (buffer[57] & 0x20)
		ecuData->m_bACClutch = FALSE;  // mode 1, byte 54, bit 5
	else
		ecuData->m_bACClutch = TRUE; // mode 1, byte 54, bit 5

	ecuData->m_iEpromID = (int)buffer[2] + ((int)buffer[1] * 256);

	m_ucDTC[0] = buffer[3]; // Fault code byte 1
	m_ucDTC[1] = buffer[4]; // Fault code byte 2
	m_ucDTC[2] = buffer[5]; // Fault code byte 3

	ecuData->m_fWaterTemp = ((float)buffer[6] * (float)0.75) - (float)40.0; // in °C
	ecuData->m_fStartWaterTemp = ((float)buffer[7] * (float)0.75) - (float)40.0; // in °C
	ecuData->m_fThrottleVolts = (float)(((float)buffer[8] / (float)255.0) * (float) 5.0);
	ecuData->m_iThrottleADC = buffer[8];
	ecuData->m_iThrottlePos = (int)((float)buffer[9] / (float)2.55);
	ecuData->m_iRPM = buffer[10] * 25;
	ecuData->m_iMPH = (int)buffer[13]; // Count is in MPH
	ecuData->m_fO2VoltsLeft = (float) buffer[15] * (float) 0.00444; // 1st Bank
	ecuData->m_iRichLeanCounterL = (int)buffer[16];
	ecuData->m_iBLM = (int)buffer[18];
	ecuData->m_iIntegratorL = (int)buffer[20];
	ecuData->m_iIACPosition = (int)buffer[21];
	ecuData->m_fMATVolts = ((float)buffer[23] / (float)255.0) * (float)5.0; // in Volts
	ecuData->m_iMATADC = buffer[23];
	ecuData->m_fMATTemp = CGMBaseFunctions::ReturnTemp(buffer[23]); // in °C
	ecuData->m_fBatteryVolts = (float)buffer[24] / (float)10.0;
	ecuData->m_fSparkAdvance = ((((float)buffer[26]  + ((float) buffer[25] * (float)256.0)) * (float)90.0) / (float)256.0); // in °
	ecuData->m_fBaro =  (((float)buffer[28] - (float)255.0) / (float)100) + (float)1.0; // in Bar Absolute
	ecuData->m_fBaroVolts = ((float)buffer[28] / (float) 255.0) * (float) 5.0; // in Volts
	ecuData->m_iBaroADC = buffer[28]; // in Counts
	ecuData->m_fMAPVolts = ((float)buffer[30] / (float) 255.0) * (float) 5.0; // in Volts
	ecuData->m_iMAPADC = buffer[30];
	ecuData->m_fMAP = (((float)buffer[30] - (float)128.0)/ (float)100) + (float)1.0; // in Bar Absolute
	ecuData->m_iInjectorBasePWMsL = (int) ( (float)((buffer[32] * 256) + buffer[33]) / (float)65.536);
	ecuData->m_iDesiredIdle = (int)((float)buffer[34] * (float) 12.5);
	ecuData->m_iKnockCount = (buffer[36] * 256) + buffer[37]; // Total running time
	ecuData->m_fKnockRetard = ((float)buffer[38] * (float)45.0) / (float)256.0; // in °
	ecuData->m_iRunTime = (buffer[39] * 256) + buffer[40]; // Total running time
	ecuData->m_fAirFlow = ((float)buffer[44] * (float)4.0);
	ecuData->m_fAFRatio = (float)buffer[55] / (float)10.0; // Air Fuel Ratio
	ecuData->m_iBoostPW = (int) ((float)buffer[56] / (float)2.55); // Boost Solenoid

	ParseDTCs(ecuData); // Process the DTCs into text

	return TRUE;
}

// Translates the incoming data stream as Mode 2
BOOL CGMA160Parser::ParseMode2(const unsigned char* buffer, int len, CEcuData* const ecuData) {
	if (len<10) // remember half duplex. We read our commands as well
	{
		WriteStatus("Received our TX command echo for mode 2.");
		return FALSE;
	}
	else if (len>65)
	{
		WriteStatus("Warning: F002 larger than expected, packet truncated.");
		len = 65;
	}
	
	ecuData->copyToF002(buffer, len);

	// Mode number is in index 0
	// Work out real-world data from the packet.

	return TRUE;
}

// Translates the incoming data stream as Mode 3
BOOL CGMA160Parser::ParseMode3(const unsigned char* buffer, int len, CEcuData* const ecuData) {
	if (len==0) // remember half duplex. We read our commands as well
	{
		WriteStatus("0 Received our TX command echo for mode 3.");
		return FALSE;
	}
	else if (len==1) // remember half duplex. We read our commands as well
	{
		WriteStatus("1 Received our TX command echo for mode 3.");
		return FALSE;
	}
	else if (len>11)
	{
		WriteStatus("Warning: F003 larger than expected, packet truncated.");
		len = 11;
	}
	
	ecuData->copyToF003(buffer, len);

	// Mode number is in index 0
	// Work out real-world data from the packet.
//	m_ucDTC[0] = buffer[3]; // Fault code byte 1
//	m_ucDTC[1] = buffer[4]; // Fault code byte 2
//	m_ucDTC[2] = buffer[5]; // Fault code byte 3

	ParseDTCs(ecuData); // Process the DTCs into text

	return TRUE;
}

// Translates the incoming data stream as Mode 4
BOOL CGMA160Parser::ParseMode4(const unsigned char* buffer, int len, CEcuData* const ecuData) {
	if (len==0) // remember half duplex. We read our commands as well
	{
		WriteStatus("0 Received our TX command echo for mode 4.");
		return FALSE;
	}
	else if (len==1) // remember half duplex. We read our commands as well
	{
		WriteStatus("1 Received our TX command echo for mode 4.");
		return FALSE;
	}
	else if (len>11)
	{
		WriteStatus("Warning: F004 larger than expected, packet truncated.");
		len = 11;
	}
	
	ecuData->copyToF004(buffer, len);

	// Mode number is in index 0
	// Work out real-world data from the packet.

	return TRUE;
}

// Translates the incoming data stream as Mode 7
BOOL CGMA160Parser::ParseMode7(const unsigned char* /*buffer*/, int len, CEcuData* const /*ecuData*/) {
	if (len==0) // remember half duplex. We read our commands as well
	{
		WriteStatus("0 Received our TX command echo for mode 7.");
		return FALSE;
	}
	else if (len==1) // remember half duplex. We read our commands as well
	{
		WriteStatus("1 Received our TX command echo for mode 7.");
		return FALSE;
	}
	else if (len>2)
	{
		WriteStatus("Warning: F007 larger than expected, packet truncated.");
		len = 2;
	}

	// Mode number is in index 0
	// Work out real-world data from the packet.

	return FALSE;
}

// Translates the incoming data stream as Mode 8
BOOL CGMA160Parser::ParseMode8(const unsigned char* /*buffer*/, int len, CEcuData* const /*ecuData*/) {
	if (len==0) // remember half duplex. We read our commands as well
	{
		WriteStatus("0 Received our TX command echo for mode 8.");
		return FALSE;
	}
	else if (len==1) // remember half duplex. We read our commands as well
	{
		WriteStatus("1 Received our TX command echo for mode 8.");
		return FALSE;
	}
	else if (len>1)
	{
		WriteStatus("Warning: F008 larger than expected, packet truncated.");
		len = 1;
	}

	// Mode number is in index 0
	// Work out real-world data from the packet.

	return FALSE;
}

// Translates the incoming data stream as Mode 9
BOOL CGMA160Parser::ParseMode9(const unsigned char* /*buffer*/, int len, CEcuData* const /*ecuData*/) {
	if (len==0) // remember half duplex. We read our commands as well
	{
		WriteStatus("0 Received our TX command echo for mode 9.");
		return FALSE;
	}
	else if (len==1) // remember half duplex. We read our commands as well
	{
		WriteStatus("1 Received our TX command echo for mode 9.");
		return FALSE;
	}
	else if (len>1)
	{
		WriteStatus("Warning: F009 larger than expected, packet truncated.");
		len = 1;
	}

	// Mode number is in index 0
	// Work out real-world data from the packet.

	return FALSE;
}

// Translates the incoming data stream as Mode 10
BOOL CGMA160Parser::ParseMode10(const unsigned char* /*buffer*/, int len, CEcuData* const /*ecuData*/) {
	if (len==0) // remember half duplex. We read our commands as well
	{
		WriteStatus("0 Received our TX command echo for mode 10.");
		return FALSE;
	}
	else if (len==1) // remember half duplex. We read our commands as well
	{
		WriteStatus("1 Received our TX command echo for mode 10.");
		return FALSE;
	}
	else if (len>1)
	{
		WriteStatus("Warning: F010 larger than expected, packet truncated.");
		len = 1;
	}

	// Mode number is in index 0
	// Work out real-world data from the packet.

	return FALSE;
}

// Translates the DTC Codes
void CGMA160Parser::ParseDTCs(CEcuData *const ecuData) {

	ecuData->m_csDTC.Empty();

	if ((m_ucDTC[0] == 0) && (m_ucDTC[1] == 0) && (m_ucDTC[2] == 0) && (m_ucDTC[3] == 0)) {
		ecuData->m_csDTC = "No reported faults.";
	}
	else {
		ecuData->m_csDTC = "The following faults are reported:\n";
		
		// Now print the fault-codes
		// MALFFW1     LOGGED MALF FLAG WORD 1
		if (m_ucDTC[0] & 0x80)
		{ //
			ecuData->m_csDTC +=  "CODE 12  NO REFERENCE PULSE (ENGINE NOT RUNNING)";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x40)
		{ //
			ecuData->m_csDTC += "CODE 13  OXYGEN SENSOR";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x20)
		{ //
			ecuData->m_csDTC += "CODE 14  COOLANT SENSOR HIGH TEMPERATURE";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x10)
		{ //
			ecuData->m_csDTC += "CODE 15  COOLANT SENSOR LOW TEMPERATURE";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x08)
		{ //
			ecuData->m_csDTC += "CODE 21  THROTTLE POSITION SENSOR HIGH";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x04)
		{ //
			ecuData->m_csDTC += "CODE 22  THROTTLE POSITION SENSOR LOW";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x02)
		{ //
			ecuData->m_csDTC += "CODE 23  LOW MANIFOLD AIR TEMP (MAT)";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x01)
		{ //
			ecuData->m_csDTC += "CODE 24  VEHICLE SPEED SENSOR LOW";
			ecuData->m_csDTC += "\n";
		}

		// MALFFW2     LOGGED MALF FLAG WORD 2
		if (m_ucDTC[1] & 0x80)
		{ //
			ecuData->m_csDTC += "CODE 25  HIGH MANIFOLD AIR TEMP (MAT)";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x40)
		{ //
			ecuData->m_csDTC += "CODE 31 WASTEGATE OVERBOOST";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x20)
		{ //
			ecuData->m_csDTC += "CODE 32 EGR DIAGNOSTIC";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x10)
		{ //
			ecuData->m_csDTC += "CODE 33  MAP SENSOR HIGH";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x08)
		{ //
			ecuData->m_csDTC += "CODE 34  MAP SENSOR LOW";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x04)
		{ //
			ecuData->m_csDTC += "CODE 35 IAC ERROR";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x02)
		{ //
			ecuData->m_csDTC += "CODE 41 CYLINDER SELECT ERROR";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x01)
		{ //
			ecuData->m_csDTC += "CODE 42  ESC MONITOR ERROR";
			ecuData->m_csDTC += "\n";
		}

		// MALFFW3     LOGGED MALF FLAG WORD 3
		if (m_ucDTC[2] & 0x80)
		{ //
			ecuData->m_csDTC += "CODE 43  ESC FAILURE";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x40)
		{ //
			ecuData->m_csDTC += "CODE 44  OXYGEN SENSOR LEAN";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x20)
		{ //
			ecuData->m_csDTC += "CODE 45  OXYGEN SENSOR RICH";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x10)
		{ //
			ecuData->m_csDTC += "CODE 51  PROM ERROR";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x08)
		{ //
			ecuData->m_csDTC += "CODE 52";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x04)
		{ //
			ecuData->m_csDTC += "CODE 53 HIGH BATTERY VOLTAGE";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x02)
		{ //
			ecuData->m_csDTC += "CODE 54";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x01)
		{ //
			ecuData->m_csDTC += "CODE 55 ADU ERROR";
			ecuData->m_csDTC += "\n";
		}
	}

}

