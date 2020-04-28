// GMA143Parser.cpp: implementation of the CGMA143Parser class.
//
//////////////////////////////////////////////////////////////////////
// (c) 1996-2000 Andy Whittaker, Chester, England. 
// mail@andywhittaker.com
//
// This contains all low-level parsing functions.

#include "GMA143Parser.h"

#include "GMBaseFunctions.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGMA143Parser::CGMA143Parser(CBaseProtocol* const m_pProtocol) : CBaseParser(m_pProtocol) {
	memset(m_ucDTC, 0, sizeof(m_ucDTC));// Reset DTC buffer
}

CGMA143Parser::~CGMA143Parser() {
}

void CGMA143Parser::InitializeSupportedValues(CEcuData* const ecuData) {
	ecuData->m_csDTC = "No reported faults.";
	ecuData->m_iMPH = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fBatteryVolts = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_fWaterTemp = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iRPM = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_bEngineClosedLoop = CEcuData::c_bSUPPORTED_BY_PROTOCOL;
	ecuData->m_bEngineStalled = CEcuData::c_bSUPPORTED_BY_PROTOCOL;
	ecuData->m_bACRequest = CEcuData::c_bSUPPORTED_BY_PROTOCOL;
	ecuData->m_bACClutch = CEcuData::c_bSUPPORTED_BY_PROTOCOL;
	ecuData->m_iThrottlePos = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fWaterTemp = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_fStartWaterTemp = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_fThrottleVolts = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iThrottleADC = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iEngineLoad = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fO2VoltsLeft = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iRichLeanCounterL = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iIntegratorL = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iBLM = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iBLMCell = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iIACPosition = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iDesiredIdle = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iCanisterDC = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fAirFlow = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_fSparkAdvance = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iKnockCount = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fKnockRetard = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iInjectorBasePWMsL = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fAFRatio = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iRunTime = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iEpromID = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
}

// Parses the buffer into real data
BOOL CGMA143Parser::Parse(const unsigned char* buffer, int length, CEcuData* const ecuData) {
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
		case ECU_HEADER_GMA143: // Packet Header Start
			{
				iIndex++; // now find the length
				iPacketLength = CGMBaseFunctions::GetLength((int)buffer[iIndex]); // Length of data
				iIndex++; // This has the mode number
				if (iPacketLength != 0)
				{// Data in Header
					if(buffer[iIndex]==1) // length 68 including mode byte
						dataWasUpdated |= ParseMode1(&buffer[iIndex], iPacketLength, ecuData);
					else if(buffer[iIndex]==2) // length 65 including mode byte
						dataWasUpdated |= ParseMode2(&buffer[iIndex], iPacketLength, ecuData);
					else if(buffer[iIndex]==3) // length max 7 including mode byte
						dataWasUpdated |= ParseMode3(&buffer[iIndex], iPacketLength, ecuData);
					else if(buffer[iIndex]==4) // length max 11 including mode byte
						dataWasUpdated |= ParseMode4(&buffer[iIndex], iPacketLength, ecuData);
					else
					{
						CString	temp; // write to the spy window
						temp.Format("Unrecognised Mode: %02x", buffer[iIndex]);
						WriteStatus(temp);
					}
					iIndex += iPacketLength;
				}
				// Check CRC
				if (!CGMBaseFunctions::CheckChecksum(pPacketStart, 3 + iPacketLength))
					WriteStatus("Checksum Error");
				break;
			}
		case 0x0a: // Computed Values
			{
				iIndex++; // now find the length
				iPacketLength = CGMBaseFunctions::GetLength((int)buffer[iIndex]); // Length of data
				iIndex++; // This has the mode number
				dataWasUpdated |= ParseAnalogues(&buffer[iIndex], iPacketLength, ecuData);
				iIndex += iPacketLength; // should be 3
				// Check CRC
				if (!CGMBaseFunctions::CheckChecksum(pPacketStart, 3 + iPacketLength))
					WriteStatus("Checksum Error");
				break;
			}
		case 0x05: // ADC Values
			{
				iIndex++; // now find the length
				iPacketLength = CGMBaseFunctions::GetLength((int)buffer[iIndex]); // Length of data
				iIndex++; // This has the mode number
				dataWasUpdated |= ParseADC(&buffer[iIndex], iPacketLength, ecuData);
				iIndex += iPacketLength; // should be 6 from ECU
				// Check CRC
				if (!CGMBaseFunctions::CheckChecksum(pPacketStart, 3 + iPacketLength))
					WriteStatus("Checksum Error");
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

// Translates the incomming data stream as ADC Values
BOOL CGMA143Parser::ParseADC(const unsigned char* buffer, int len, CEcuData* const ecuData) {
	if (len>10)
	{
		WriteStatus("Warning: F005 larger than expected, packet truncated.");
		len = 10;
	}

	ecuData->copyToF005(buffer, len);

	// Work out real world data from the packet.
	ecuData->m_iMPH = (int)buffer[2]; // Count is in MPH
	ecuData->m_fBatteryVolts = (float)buffer[4] / (float)10.0;
	ecuData->m_fWaterTemp = ((float)buffer[9] * (float)0.75) - (float)40.0; // in °C

	return TRUE;
}

// Translates the incoming data stream as Analogue Values
BOOL CGMA143Parser::ParseAnalogues(const unsigned char* buffer, int len, CEcuData* const ecuData) {
	if (len>3)
	{
		WriteStatus("Warning: F00A larger than expected, packet truncated.");
		len = 3;
	}

	ecuData->copyToF00A(buffer, len);

	// Work out real world data from the packet.
	ecuData->m_iRPM = ((int)buffer[1] * 256) + (int)buffer[2];

	return TRUE;
}

// Translates the incoming data stream as Mode 1
BOOL CGMA143Parser::ParseMode1(const unsigned char* buffer, int len, CEcuData* const ecuData) {
	if (len<10) // remember half duplex. We read our commands as well
	{
		WriteStatus("Received our TX command echo for mode 1.");
		return FALSE;
	}
	else if (len>68)
	{
		WriteStatus("Warning: F001 larger than expected, packet truncated.");
		len = 68;
	}

	ecuData->copyToF001(buffer, len);

	// Work out real-world data from the packet.
	// Mode number is in index 0
	if (buffer[8] & 0x80)
		ecuData->m_bEngineClosedLoop = TRUE;  // bit 7
	else
		ecuData->m_bEngineClosedLoop = FALSE; // bit 7

	if (buffer[1] & 0x80)
		ecuData->m_bEngineStalled = TRUE;  // bit 6
	else
		ecuData->m_bEngineStalled = FALSE; // bit 6

	// Status Word 2
	if (buffer[3] & 0x80)
		ecuData->m_bACRequest = FALSE;  // byte 6, bit 3
	else
		ecuData->m_bACRequest = TRUE; // byte 6, bit 3

	if (buffer[1] & 0x20)
		ecuData->m_bACClutch = FALSE;  // mode 1, byte 10, bit 0
	else
		ecuData->m_bACClutch = TRUE; // mode 1, byte 10, bit 0

	// Analogues
	ecuData->m_iThrottlePos = (int)((float)buffer[10] / (float)2.55);
	ecuData->m_fWaterTemp = ((float)buffer[11] * (float)0.75) - (float)40.0; // in °C
	ecuData->m_fStartWaterTemp = ((float)buffer[12] * (float)0.75) - (float)40.0; // in °C
	ecuData->m_fThrottleVolts = (float)(((float)buffer[13] / (float)255.0) * (float) 5.0);
	ecuData->m_iThrottleADC = buffer[13]; // in Counts
	ecuData->m_iRPM = ((int)buffer[14] * 25);
	ecuData->m_iMPH = (int)buffer[17]; // Count is in MPH
	ecuData->m_iEngineLoad = (int)((float)buffer[19] / (float) 2.55);
	ecuData->m_fO2VoltsLeft = (float) buffer[20] * (float) 0.00444;
	ecuData->m_iRichLeanCounterL = (int)buffer[21];
	ecuData->m_iIntegratorL = (int)buffer[22];
	ecuData->m_iBLM = (int)buffer[23];
	ecuData->m_iBLMCell = (int)buffer[24];
	ecuData->m_iIACPosition = (int)buffer[25];
	ecuData->m_iDesiredIdle = (int)((float)buffer[26] * (float) 12.5);
	ecuData->m_iCanisterDC = (int) ((float)buffer[29] / (float)2.55); // Canister Purge
	ecuData->m_fBatteryVolts = (float)buffer[30] / (float)10.0;
	ecuData->m_fAirFlow = (float) ((buffer[31] + ((float)buffer[32] * (float)0.00390625)) );
	ecuData->m_fSparkAdvance = ((float)((buffer[33] * 256) + buffer[34]) * (float)90.0) / (float)256.0; // in °
	ecuData->m_iKnockCount = (int)buffer[35];
	ecuData->m_fKnockRetard = ((float)buffer[36] * (float).176); // in °
	ecuData->m_iInjectorBasePWMsL = (int) ( (float)((buffer[37] * 256) + buffer[38]) / (float)65.536);
	ecuData->m_fAFRatio = (float)65536.0 / (float)(((buffer[39] * 256) + buffer[40]) * 40); // Air Fuel Ratio
	ecuData->m_iRunTime = (buffer[44] * 256) + buffer[45]; // Total running time

	m_ucDTC[0] = buffer[46]; // Fault code byte 1
	m_ucDTC[1] = buffer[47]; // Fault code byte 2
	m_ucDTC[2] = buffer[48]; // Fault code byte 3
	m_ucDTC[3] = buffer[49]; // Fault code byte 4
	m_ucDTC[4] = buffer[50]; // Fault code byte 5
	m_ucDTC[5] = buffer[52]; // Fault code byte 6

	ecuData->m_iEpromID = (int)buffer[67] + ((int)buffer[66] * 256);

	ParseDTCs(ecuData); // Process the DTCs into text

	return TRUE;
}

// Translates the incoming data stream as Mode 2
BOOL CGMA143Parser::ParseMode2(const unsigned char* buffer, int len, CEcuData* const ecuData) {
	if (len==0) // remember half duplex. We read our commands as well
	{
		WriteStatus("0 Received our TX command echo for mode 2.");
		return FALSE;
	}
	else if (len==1) // remember half duplex. We read our commands as well
	{
		WriteStatus("1 Received our TX command echo for mode 2.");
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
BOOL CGMA143Parser::ParseMode3(const unsigned char* buffer, int len, CEcuData* const ecuData) {
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

	return TRUE;
}

// Translates the incoming data stream as Mode 4
BOOL CGMA143Parser::ParseMode4(const unsigned char* buffer, int len, CEcuData* const ecuData) {
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

	return true;
}

// Translates the DTC Codes
void CGMA143Parser::ParseDTCs(CEcuData *const ecuData) {

	ecuData->m_csDTC.Empty();

	if ((m_ucDTC[0] == 0) && (m_ucDTC[1] == 0) && (m_ucDTC[2] == 0) && (m_ucDTC[3] == 0) && (m_ucDTC[4] == 0) && (m_ucDTC[5] == 0)) {
		ecuData->m_csDTC = "No reported faults.";
	}
	else {
		ecuData->m_csDTC = "The following historical faults are reported:\n";
		
		// Now print the fault-codes
		if (m_ucDTC[0] & 0x80)  // Malfunction Word 1
		{ // 13 - O2 SENSOR
			ecuData->m_csDTC += "13 - O2 SENSOR";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x40)
		{ // 14 - COOLANT SENSOR HIGH
			ecuData->m_csDTC += "14 - COOLANT SENSOR HIGH";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x20)
		{ // 15 - COOLANT SENSOR LOW
			ecuData->m_csDTC += "15 - COOLANT SENSOR LOW";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x10)
		{ // 
			ecuData->m_csDTC += "16 - BATTERY VOLTAGE HIGH";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x08)
		{ // 
			ecuData->m_csDTC += "17 - NOT USED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x04)
		{ // 
			ecuData->m_csDTC += "18 - NOT USED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x02)
		{ // 
			ecuData->m_csDTC += "19 - NOT USED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x01)
		{ // 21 - Throttle Position Sensor (TPS) Circuit; Signal Voltage High
			ecuData->m_csDTC += "21 - Throttle Position Sensor (TPS) Circuit; Signal Voltage High";
			ecuData->m_csDTC += "\n";
		}

		if (m_ucDTC[1] & 0x80) // Malfunction Word 2
		{ // 22 - Throttle Position Sensor (TPS) Circuit; Signal Voltage Low
			ecuData->m_csDTC += "22 - Throttle Position Sensor (TPS) Circuit; Signal Voltage Low";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x40)
		{ // 
			ecuData->m_csDTC += "23 - NOT USED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x20)
		{ // 24 - VEHICLE SPEED SENSOR LOW
			ecuData->m_csDTC += "24 - VEHICLE SPEED SENSOR LOW";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x10)
		{ // 
			ecuData->m_csDTC += "25 - NOT USED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x08)
		{ // 26 - Quad-Driver (QDM) Circuit, Secondary Injectors
			ecuData->m_csDTC += "26 - Quad-Driver (QDM) Circuit, Secondary Injectors Failure";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x04)
		{ // 27 - 2nd Gear Switch Failure
			ecuData->m_csDTC += "27 - 2nd Gear Switch Failure";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x02)
		{ // 28 - 3rd Gear Switch Failure
			ecuData->m_csDTC += "28 - 3rd Gear Switch Failure";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x01)
		{ // 29 - 4th Gear Switch Failure
			ecuData->m_csDTC += "29 - 4th Gear Switch Failure";
			ecuData->m_csDTC += "\n";
		}

		if (m_ucDTC[2] & 0x80) // Malfunction Flag 3
		{ // 31 - P/N SWITCH FAILURE
			ecuData->m_csDTC += "31 - P/N SWITCH FAILURE";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x40)
		{ // NOT USED
			ecuData->m_csDTC += "32 - NOT USED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x20)
		{ // 33 - MASS AIR FLOW SENSOR HIGH
			ecuData->m_csDTC += "33 - MASS AIR FLOW SENSOR HIGH";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x10)
		{ // 34 - MASS AIR FLOW SENSOR LOW
			ecuData->m_csDTC += "34 - MASS AIR FLOW SENSOR LOW";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x08)
		{ // NOT USED
			ecuData->m_csDTC += "35 - NOT USED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x04)
		{ // NOT USED
			ecuData->m_csDTC += "36 - NOT USED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x02)
		{ // NOT USED
			ecuData->m_csDTC += "37 - NOT USED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x01)
		{ // 38 - BRAKE SWITCH FAILURE
			ecuData->m_csDTC += "38 - BRAKE SWITCH FAILURE";
			ecuData->m_csDTC += "\n";
		}


		if (m_ucDTC[3] & 0x80) // Malfunction Word 4
		{ // 39 - TCC FAILURE
			ecuData->m_csDTC += "39 - TCC FAILURE";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[3] & 0x40)
		{ // 41 - NOT USED
			ecuData->m_csDTC += "41 - NOT USED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[3] & 0x20)
		{ // 42 - EST FAILURE
			ecuData->m_csDTC += "42 - EST FAILURE";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[3] & 0x10)
		{ // 
			ecuData->m_csDTC += "43 - ESC FAILURE";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[3] & 0x08)
		{ // 
			ecuData->m_csDTC += "44 - O2 SENSOR LEAN";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[3] & 0x04)
		{ //
			ecuData->m_csDTC += "45 - O2 SENSOR RICH";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[3] & 0x02)
		{ //
			ecuData->m_csDTC += "46 - POWER STEERING PRESSURE SWITCH";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[3] & 0x01)
		{ // 
			ecuData->m_csDTC += "47 - NOT USED";
			ecuData->m_csDTC += "\n";
		}

		if (m_ucDTC[4] & 0x80) // Malfunction Word 5
		{ // 
			ecuData->m_csDTC += "48 - MISFIRE";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[4] & 0x40)
		{ // 
			ecuData->m_csDTC += "49 - VACUUM LEAK";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[4] & 0x20)
		{ // 
			ecuData->m_csDTC += "51 - PROM ERROR";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[4] & 0x10)
		{ // 
			ecuData->m_csDTC += "52 - NOT USED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[4] & 0x08)
		{ // 
			ecuData->m_csDTC += "53 - NOT USED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[4] & 0x04)
		{ //
			ecuData->m_csDTC += "54 - NOT USED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[4] & 0x02)
		{ //
			ecuData->m_csDTC += "55 - NOT USED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[4] & 0x01)
		{ // 
			ecuData->m_csDTC += "56 - NOT USED";
			ecuData->m_csDTC += "\n";
		}

		if (m_ucDTC[5] & 0x80) // Malfunction Word 7
		{ // 
			ecuData->m_csDTC += "66 - A/C PRESSURE TRANSDUCER FAILURE";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[5] & 0x40)
		{ // 
			ecuData->m_csDTC += "67 - NOT USED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[5] & 0x20)
		{ // 
			ecuData->m_csDTC += "68 - NOT USED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[5] & 0x10)
		{ // 
			ecuData->m_csDTC += "69 - NOT USED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[5] & 0x08)
		{ // 
			ecuData->m_csDTC += "88 - RESET";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[5] & 0x04)
		{ //
			ecuData->m_csDTC += "72 - NOT USED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[5] & 0x02)
		{ //
			ecuData->m_csDTC += "73 - NOT USED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[5] & 0x01)
		{ // 
			ecuData->m_csDTC += "74 - NOT USED";
			ecuData->m_csDTC += "\n";
		}
	}
}
