// GMA140Parser.cpp: implementation of the CGMA140Parser class.
//
//////////////////////////////////////////////////////////////////////
// (c) 1996-2000 Andy Whittaker, Chester, England. 
// mail@andywhittaker.com
//
// This contains all low-level parsing functions.

#include "GMA140Parser.h"

#include "GMBaseFunctions.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGMA140Parser::CGMA140Parser(CBaseProtocol* const m_pProtocol) : CBaseParser(m_pProtocol) {
	memset(m_ucDTC, 0, sizeof(m_ucDTC));// Reset DTC buffer
}

CGMA140Parser::~CGMA140Parser() {
}

void CGMA140Parser::InitializeSupportedValues(CEcuData* const ecuData) {
	ecuData->m_csDTC = "No reported faults.";
	ecuData->m_iMPH = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fBatteryVolts = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_fWaterTemp = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iRPM = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_bEngineClosedLoop = CEcuData::c_bSUPPORTED_BY_PROTOCOL;
	ecuData->m_bEngineStalled = CEcuData::c_bSUPPORTED_BY_PROTOCOL;
	ecuData->m_bACRequest = CEcuData::c_bSUPPORTED_BY_PROTOCOL;
	ecuData->m_iEpromID = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fStartWaterTemp = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_fThrottleVolts = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iThrottleADC = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iThrottlePos = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fO2VoltsLeft = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iRichLeanCounterL = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iBLM = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iBLMCell = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iIntegratorL = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iIACPosition = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iDesiredIdle = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fBaro = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_fBaroVolts = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iBaroADC = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fMAP = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_fMAPVolts = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iMAPADC = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fMATVolts = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iMATADC = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fMATTemp = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_fAirFlow = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_fSparkAdvance = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_fAFRatio = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iInjectorBasePWMsL = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fKnockRetard = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iRunTime = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iKnockCount = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
}

// Parses the buffer into real data
BOOL CGMA140Parser::Parse(const unsigned char* const buffer, int const length, CEcuData* const ecuData) {
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
		case ECU_HEADER_GMA140: // Packet Header Start
			{
				iIndex++; // now find the length
				iPacketLength = CGMBaseFunctions::GetLength((int)buffer[iIndex]); // Length of data
				iIndex++; // This has the mode number
				if (iPacketLength != 0)
				{// Data in Header
					if(buffer[iIndex]==1) // length 64 including mode byte
						dataWasUpdated |= ParseMode1(&buffer[iIndex], iPacketLength, ecuData);
					else if(buffer[iIndex]==2) // length 65 including mode byte
						dataWasUpdated |= ParseMode2(&buffer[iIndex], iPacketLength, ecuData);
					else if(buffer[iIndex]==3) // length max 7 including mode byte
						dataWasUpdated |= ParseMode3(&buffer[iIndex], iPacketLength, ecuData);
					else if(buffer[iIndex]==4) // length max 11 including mode byte
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
BOOL CGMA140Parser::ParseADC(const unsigned char* buffer, int len, CEcuData* const ecuData) {
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
BOOL CGMA140Parser::ParseAnalogues(const unsigned char* buffer, int len, CEcuData* const ecuData) {
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
BOOL CGMA140Parser::ParseMode1(const unsigned char* buffer, int len, CEcuData* const ecuData) {
	if (len<10) // remember half duplex. We read our commands as well
	{
		WriteStatus("Received our TX command echo for mode 1.");
		return FALSE;
	}
	else if (len>64)
	{
		WriteStatus("Warning: F001 larger than expected, packet truncated.");
		len = 64;
	}

	ecuData->copyToF001(buffer, len);

	// Work out real-world data from the packet.
	// Mode number is in index 0
	if (buffer[63] & 0x80)
		ecuData->m_bEngineClosedLoop = TRUE;  // bit 7
	else
		ecuData->m_bEngineClosedLoop = FALSE; // bit 7

	if (buffer[01] & 0x80)
		ecuData->m_bEngineStalled = TRUE;  // bit 6
	else
		ecuData->m_bEngineStalled = FALSE; // bit 6

	// Status Word 2
	if (buffer[03] & 0x80)
		ecuData->m_bACRequest = TRUE;  // byte 6, bit 3
	else
		ecuData->m_bACRequest = FALSE; // byte 6, bit 3

	ecuData->m_iEpromID = (int)buffer[2] + ((int)buffer[1] * 256);
	m_ucDTC[0] = buffer[3]; // Fault code byte 1
	m_ucDTC[1] = buffer[4]; // Fault code byte 2
	m_ucDTC[2] = buffer[5]; // Fault code byte 3
	m_ucDTC[3] = buffer[6]; // Fault code byte 4
	// Analogues
	ecuData->m_fWaterTemp = ((float)buffer[7] * (float)0.75) - (float)40.0; // in °C
	ecuData->m_fStartWaterTemp = ((float)buffer[8] * (float)0.75) - (float)40.0; // in °C
	ecuData->m_fThrottleVolts = (float)(((float)buffer[9] / (float)255.0) * (float) 5.0);
	ecuData->m_iThrottleADC = buffer[9]; // in Counts
	ecuData->m_iThrottlePos = (int)((float)buffer[10] / (float)2.55);
	ecuData->m_iRPM = ((int)buffer[11] * 25);
	ecuData->m_iMPH = (int)buffer[17]; // Count is in MPH
	ecuData->m_fO2VoltsLeft = (float) buffer[19] * (float) 0.00442;
	ecuData->m_iRichLeanCounterL = (int)buffer[20];
	ecuData->m_iBLM = (int)buffer[22];
	ecuData->m_iBLMCell = (int)buffer[23];
	ecuData->m_iIntegratorL = (int)buffer[24];
	ecuData->m_iIACPosition = (int)buffer[25];
	ecuData->m_iDesiredIdle = (int)((float)buffer[27] * (float) 12.5);
	ecuData->m_fBaro = (((float)buffer[28] + (float)28.06)/ (float)271); // in Bar Absolute
	ecuData->m_fBaroVolts = ((float)buffer[28] / (float) 255.0) * (float) 5.0; // in Volts
	ecuData->m_iBaroADC = buffer[28]; // in Counts
	ecuData->m_fMAP = (((float)buffer[29] + (float)28.06)/ (float)271); // in Bar Absolute
	ecuData->m_fMAPVolts = ((float)buffer[29] / (float) 255.0) * (float) 5.0; // in Volts
	ecuData->m_iMAPADC = buffer[29]; // in Counts
	ecuData->m_fMATVolts = ((float)buffer[30] / (float)255.0) * (float)5.0; // in Volts
	ecuData->m_iMATADC = buffer[30]; // in Counts
	ecuData->m_fMATTemp = CGMBaseFunctions::ReturnTemp(buffer[30]); // in °C
	ecuData->m_fBatteryVolts = (float)buffer[34] / (float)10.0;
	ecuData->m_fAirFlow = (float) buffer[37];
	ecuData->m_fSparkAdvance = ((float)((buffer[38] * 256) + buffer[39]) * (float)90.0) / (float)256.0; // in °
	ecuData->m_fAFRatio = (float) (buffer[41] / (float)10); // Air Fuel Ratio
	ecuData->m_iInjectorBasePWMsL = (int) ( (float)((buffer[42] * 256) + buffer[43]) / (float)65.536);
	ecuData->m_fKnockRetard = ((float)buffer[46] * (float)45) / (float) 256.0; // in °
	ecuData->m_iRunTime = (buffer[48] * 256) + buffer[49]; // Total running time
	ecuData->m_iKnockCount = (int)buffer[51];
	
	ParseDTCs(ecuData); // Process the DTCs into text

	return TRUE;
}

// Translates the incoming data stream as Mode 2
BOOL CGMA140Parser::ParseMode2(const unsigned char* buffer, int len, CEcuData* const ecuData) {
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
BOOL CGMA140Parser::ParseMode3(const unsigned char* buffer, int len, CEcuData* const ecuData) {
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
BOOL CGMA140Parser::ParseMode4(const unsigned char* buffer, int len, CEcuData* const ecuData) {
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
BOOL CGMA140Parser::ParseMode7(const unsigned char* /*buffer*/, int len, CEcuData* const /*ecuData*/) {
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
BOOL CGMA140Parser::ParseMode8(const unsigned char* /*buffer*/, int len, CEcuData* const /*ecuData*/) {
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
BOOL CGMA140Parser::ParseMode9(const unsigned char* /*buffer*/, int len, CEcuData* const /*ecuData*/) {
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
BOOL CGMA140Parser::ParseMode10(const unsigned char* /*buffer*/, int len, CEcuData* const /*ecuData*/) {
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

	return FALSE;
}

// Translates the DTC Codes
void CGMA140Parser::ParseDTCs(CEcuData *const ecuData) {
	ecuData->m_csDTC.Empty();

	if ((m_ucDTC[0] == 0) && (m_ucDTC[1] == 0) && (m_ucDTC[2] == 0) && (m_ucDTC[3] == 0)) {
		ecuData->m_csDTC = "No reported faults.";
	}
	else {
		ecuData->m_csDTC = "The following historical faults are reported:\n";
		
		// Now print the fault-codes
		if (m_ucDTC[0] & 0x80)  // Malfunction Word 1
		{ // 12 - 
			ecuData->m_csDTC += "12 - No reference pulses (Engine not running)";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x40)
		{ // 13 - O2 SENSOR
			ecuData->m_csDTC += "13 - O2 SENSOR";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x20)
		{ // 14 - COOLANT SENSOR HIGH
			ecuData->m_csDTC += "14 - COOLANT SENSOR HIGH";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x10)
		{ // 15 - COOLANT SENSOR LOW
			ecuData->m_csDTC += "15 - COOLANT SENSOR LOW";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x08)
		{ // 21 - Throttle Position Sensor (TPS) Circuit; Signal Voltage High
			ecuData->m_csDTC += "21 - Throttle Position Sensor (TPS) Circuit; Signal Voltage High";
			ecuData->m_csDTC += "\n";
		}

		if (m_ucDTC[0] & 0x04)
		{ // 22 - Throttle Position Sensor (TPS) Circuit; Signal Voltage Low
			ecuData->m_csDTC += "22 - Throttle Position Sensor (TPS) Circuit; Signal Voltage Low";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x02)
		{ // 
			ecuData->m_csDTC += "23 - Low Manifold Air Temperature";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x01)
		{ // 24 - VEHICLE SPEED SENSOR LOW
			ecuData->m_csDTC += "24 - VEHICLE SPEED SENSOR LOW";
			ecuData->m_csDTC += "\n";
		}

		// Malfunction Word #2
		if (m_ucDTC[1] & 0x80)
		{ // 
			ecuData->m_csDTC += "25 - High Manifold Air Temperature";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x40) // 
		{ // 31 - P/N SWITCH FAILURE
			ecuData->m_csDTC += "31 - Fuel Injection Malfunction";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x20)
		{ // NOT USED
			ecuData->m_csDTC += "32 - EVRV/EGR Fault";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x10)
		{ // 33 - MAP SENSOR HIGH
			ecuData->m_csDTC += "33 - MAP SENSOR HIGH";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x08)
		{ // 34 - MAP SENSOR LOW
			ecuData->m_csDTC += "34 - MAP SENSOR LOW";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x04)
		{ // NOT USED
			ecuData->m_csDTC += "35 - IAC - Idle Air Control Value Error";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x02)
		{ // 41 - NOT USED
			ecuData->m_csDTC += "41 - Cylinder Select Error";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x01)
		{ // 42 - EST FAILURE
			ecuData->m_csDTC += "42 - EST FAILURE";
			ecuData->m_csDTC += "\n";
		}

		// Malfunction Word #3
		if (m_ucDTC[2] & 0x80)
		{ // 
			ecuData->m_csDTC += "43 - ESC FAILURE";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x40)
		{ // 
			ecuData->m_csDTC += "44 - O2 SENSOR LEAN";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x20)
		{ //
			ecuData->m_csDTC += "45 - O2 SENSOR RICH";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x10)
		{ //
			ecuData->m_csDTC += "51 - PROM ERROR";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x08)
		{ // 
			ecuData->m_csDTC += "26 - NOT USED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x04)
		{ // 
			ecuData->m_csDTC += "53 - High Battery Voltage";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x02)
		{ //
			ecuData->m_csDTC += "54 - Fuel Pump Voltage Low";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x01)
		{ //
			ecuData->m_csDTC += "55 - ADU Error";
			ecuData->m_csDTC += "\n";
		}

		// Malfunction Word #4
		if (m_ucDTC[3] & 0x80) // Malfunction Word 4
		{ // 
			ecuData->m_csDTC += "56 - NOT USED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[3] & 0x40)
		{ // 
			ecuData->m_csDTC += "61 - Degraded O2 Sensor";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[3] & 0x20)
		{ // 
			ecuData->m_csDTC += "46 - VATS Failed";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[3] & 0x10)
		{ // 
			ecuData->m_csDTC += "62 - Transmission Gear Switch Fail";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[3] & 0x08)
		{ // 
			ecuData->m_csDTC += "64 - NOT USED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[3] & 0x04)
		{ //
			ecuData->m_csDTC += "65 - NOT USED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[3] & 0x02)
		{ //
			ecuData->m_csDTC += "66 - A/C Pressure Transducer Failure";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[3] & 0x01)
		{ // 
			ecuData->m_csDTC += "67 - NOT USED";
			ecuData->m_csDTC += "\n";
		}
	}
}
