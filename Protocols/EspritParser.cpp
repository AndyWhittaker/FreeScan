// EspritParser.cpp: implementation of the CEspritParser class.
//
//////////////////////////////////////////////////////////////////////
// (c) 1996-2000 Andy Whittaker, Chester, England. 
// mail@andywhittaker.com
//
// This contains all low-level parsing functions.

#include "EspritParser.h"

#include "GMBaseFunctions.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEspritParser::CEspritParser(CBaseProtocol* const m_pProtocol): CBaseParser(m_pProtocol) {
	memset(m_ucDTC, 0, sizeof(m_ucDTC));// Reset DTC buffer
}

CEspritParser::~CEspritParser() {
}

void CEspritParser::InitializeSupportedValues(CEcuData* const ecuData) {
	ecuData->m_csDTC = "No reported faults.";
	ecuData->m_iEpromID = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_bEngineClosedLoop = CEcuData::c_bSUPPORTED_BY_PROTOCOL;
	ecuData->m_bEngineStalled = CEcuData::c_bSUPPORTED_BY_PROTOCOL;
	ecuData->m_bACRequest = CEcuData::c_bSUPPORTED_BY_PROTOCOL;
	ecuData->m_bACClutch = CEcuData::c_bSUPPORTED_BY_PROTOCOL;
	ecuData->m_fWaterVolts = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iWaterTempADC = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fStartWaterTemp = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_fThrottleVolts = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iThrottleADC = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iDesiredIdle = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iRPM = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iMPH = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iCrankSensors = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fO2VoltsLeft = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iRichLeanCounterL = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iIntegratorL = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iBLM = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iBLMCell = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iIACPosition = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fBaro = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_fBaroVolts = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iBaroADC = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fMAP = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_fMAPVolts = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iMAPADC = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iThrottlePos = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fMATVolts = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iMATADC = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iBoostPW = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fBatteryVolts = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iEngineLoad = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iSecondaryInjPW = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fSparkAdvance = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_fWaterTemp = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_fMATTemp = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iKnockCount = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fKnockRetard = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iInjectorBasePWMsL = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fAFRatio = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iRunTime = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iCanisterDC = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
}

// Parses the buffer into real data
BOOL CEspritParser::Parse(const unsigned char* const buffer, int const length, CEcuData* const ecuData) {
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
		case ECU_HEADER_ESPRIT: // Packet Header Start
			{
				iIndex++; // now find the length
				iPacketLength = CGMBaseFunctions::GetLength((int)buffer[iIndex]); // Length of data
				iIndex++; // This has the mode number
				if (iPacketLength != 0)
				{// Data in Header
					if(buffer[iIndex]==1) // length 65 including mode byte
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
						temp.Format(_T("Unrecognised Mode: %02x"), buffer[iIndex]);
						WriteStatus(temp);
					}
					iIndex += iPacketLength;
				}
				// Check CRC
				if (!CGMBaseFunctions::CheckChecksum(pPacketStart, 3 + iPacketLength))
					WriteStatus(_T("Checksum Error"));
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
					WriteStatus(_T("Checksum Error"));
				break;
			}
		case 0x05: // ADC Values
			{
				iIndex++; // now find the length
				iPacketLength = CGMBaseFunctions::GetLength((int)buffer[iIndex]); // Length of data
				iIndex++; // This has the mode number
				dataWasUpdated |= ParseADC(&buffer[iIndex], iPacketLength, ecuData);
				iIndex += iPacketLength; // should be 10 or 58 from ECU
				// Check CRC
				if (!CGMBaseFunctions::CheckChecksum(pPacketStart, 3 + iPacketLength))
					WriteStatus(_T("Checksum Error"));
				break;
			}
		default:
			{
				CString buf;
				buf.Format(_T("%02x <- Unrecognised Header"), buffer[iIndex]);
				WriteStatus(buf);
			}
		}// Switch
	}// for()

	return dataWasUpdated;
}

// Translates the incoming data stream as ADC Values
BOOL CEspritParser::ParseADC(const unsigned char* buffer, int len, CEcuData* const ecuData) {
	if (len>10)
	{
		WriteStatus(_T("Warning: F005 larger than expected, packet truncated."));
		len = 10;
	}

	ecuData->copyToF005(buffer, len);

	// Work out real world data from the packet.

	if (buffer[63] & 0x80)
		ecuData->m_bEngineClosedLoop = TRUE;  // bit 7
	else
		ecuData->m_bEngineClosedLoop = FALSE; // bit 7

	if (buffer[0] & 0x40)
		ecuData->m_bEngineStalled = TRUE;  // bit 6
	else
		ecuData->m_bEngineStalled = FALSE; // bit 6

	ecuData->m_iMPH = (int)buffer[2]; // Count is in MPH
	ecuData->m_fBatteryVolts = (float)buffer[4] / (float)10.0;
	ecuData->m_fWaterTemp = ((float)buffer[9] * (float)0.75) - (float)40.0; // in °C

	return TRUE;
}

// Translates the incoming data stream as Analogue Values
BOOL CEspritParser::ParseAnalogues(const unsigned char* buffer, int len, CEcuData* const ecuData) {
	if (len>3)
	{
		WriteStatus(_T("Warning: F00A larger than expected, packet truncated."));
		len = 3;
	}

	ecuData->copyToF00A(buffer, len);

	// Work out real world data from the packet.

	ecuData->m_iRPM = ((int)buffer[1] * 256) + (int)buffer[2];

	return TRUE;
}

// Translates the incoming data stream as Mode 1
BOOL CEspritParser::ParseMode1(const unsigned char* buffer, int len, CEcuData* const ecuData) {
	if (len<10) // remember half duplex. We read our commands as well
	{
		WriteStatus(_T("Received our TX command echo for mode 1."));
		return FALSE;
	}
	else if (len>65)
	{
		WriteStatus(_T("Warning: F001 larger than expected, packet truncated."));
		len = 65;
	}

	ecuData->copyToF001(buffer, len);

	// Work out real-world data from the packet.
	// Mode number is in index 0
	if (buffer[63] & 0x80)
		ecuData->m_bEngineClosedLoop = TRUE;  // bit 7
	else
		ecuData->m_bEngineClosedLoop = FALSE; // bit 7

	if (buffer[64] & 0x40)
		ecuData->m_bEngineStalled = TRUE;  // bit 6
	else
		ecuData->m_bEngineStalled = FALSE; // bit 6

	// Status Word 2
	if (buffer[6] & 0x08)
		ecuData->m_bACRequest = TRUE;  // byte 6, bit 3
	else
		ecuData->m_bACRequest = FALSE; // byte 6, bit 3

	if (buffer[7] & 0x01)
		ecuData->m_bACClutch = TRUE;  // byte 7, bit 0
	else
		ecuData->m_bACClutch = FALSE; // byte 7, bit 0

	// Analogues
	ecuData->m_iEpromID = (int)buffer[2] + ((int)buffer[1] * 256);
	m_ucDTC[0] = buffer[3]; // Fault code byte 1
	m_ucDTC[1] = buffer[4]; // Fault code byte 2
	m_ucDTC[2] = buffer[5]; // Fault code byte 3
	ecuData->m_fWaterVolts = (float)(((float)buffer[8] / (float)255.0) * (float) 5.0);
	ecuData->m_iWaterTempADC = buffer[8]; // in Counts
	ecuData->m_fStartWaterTemp = ((float)buffer[9] * (float)0.75) - (float)40.0; // in °C
	ecuData->m_fThrottleVolts = (float)(((float)buffer[10] / (float)255.0) * (float) 5.0);
	ecuData->m_iThrottleADC = buffer[10]; // in Counts
	ecuData->m_iDesiredIdle = (int)((float)buffer[11] * (float) 12.5);
	ecuData->m_iRPM = ((int)buffer[12] * 256) + (int)buffer[13];
	ecuData->m_iMPH = (int)buffer[14]; // Count is in MPH
	ecuData->m_iCrankSensors = buffer[15];
	ecuData->m_fO2VoltsLeft = (float) buffer[17] * (float) 0.00444;
	ecuData->m_iRichLeanCounterL = (int)buffer[18];
	ecuData->m_iIntegratorL = (int)buffer[19];
	ecuData->m_iBLM = (int)buffer[20];
	ecuData->m_iBLMCell = (int)buffer[21];
	ecuData->m_iIACPosition = (int)buffer[23];
	ecuData->m_fBaro = (((float)buffer[24] - (float)130.0)/ (float)100) + (float) 1.0; // in Bar Absolute
	ecuData->m_fBaroVolts = ((float)buffer[24] / (float) 255.0) * (float) 5.0; // in Volts
	ecuData->m_iBaroADC = buffer[24]; // in Counts
	ecuData->m_fMAP = (((float)buffer[25] - (float)130.0)/ (float)100) + (float) 1.0; // in Bar Absolute
	ecuData->m_fMAPVolts = ((float)buffer[25] / (float) 255.0) * (float) 5.0; // in Volts
	ecuData->m_iMAPADC = buffer[25]; // in Counts
	ecuData->m_iThrottlePos = (int)((float)buffer[27] / (float)2.55);
	ecuData->m_fMATVolts = ((float)buffer[29] / (float)255.0) * (float)5.0; // in Volts
	ecuData->m_iMATADC = buffer[29]; // in Counts
	ecuData->m_iBoostPW = (int) ((float)buffer[31] / (float)2.55); // Boost Solenoid
	ecuData->m_fBatteryVolts = (float)buffer[34] / (float)10.0;
	ecuData->m_iEngineLoad = (int)((float)buffer[36] / (float) 2.55);
	ecuData->m_iSecondaryInjPW = (int) ((float)buffer[37] / (float)2.55); // Secondary Injectors
	ecuData->m_fSparkAdvance = ((float)((buffer[39] * 256) + buffer[40]) * (float)90.0) / (float)256.0; // in °
	ecuData->m_fWaterTemp = ((float)buffer[41] * (float)0.75) - (float)40.0; // in °C
	ecuData->m_fMATTemp = ((float)buffer[42] * (float)0.75) - (float)40.0; // in °C
	ecuData->m_iKnockCount = (int)buffer[43];
	ecuData->m_fKnockRetard = ((float)buffer[44] * (float)22.5) / (float)256.0; // in °
	ecuData->m_iInjectorBasePWMsL = (int) ( (float)((buffer[45] * 256) + buffer[46]) / (float)65.536);
	ecuData->m_fAFRatio = (float)buffer[47] / (float)10.0; // Air Fuel Ratio
	ecuData->m_iRunTime = (buffer[52] * 256) + buffer[53]; // Total running time
	ecuData->m_iCanisterDC = (int)(((float)buffer[30]) / 2.56f); // Canister Purge
	
	ParseDTCs(buffer, len, ecuData); // Process the DTCs into text

	return TRUE;
}

// Translates the incoming data stream as Mode 2
BOOL CEspritParser::ParseMode2(const unsigned char* buffer, int len, CEcuData* const ecuData) {
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
BOOL CEspritParser::ParseMode3(const unsigned char* buffer, int len, CEcuData* const ecuData) {
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
	else if (len>20)
	{
		WriteStatus("Warning: F003 larger than expected, packet truncated.");
		len = 20;
	}

	ecuData->copyToF003(buffer, len);

	return TRUE;
}

// Translates the incoming data stream as Mode 4
BOOL CEspritParser::ParseMode4(const unsigned char* buffer, int len, CEcuData* const ecuData) {
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

// Translates the DTC Codes
void CEspritParser::ParseDTCs(const unsigned char* buffer, int len, CEcuData* const ecuData) {

	ecuData->m_csDTC.Empty();

	if ((m_ucDTC[0] == 0) && (m_ucDTC[1] == 0) && (m_ucDTC[2] == 0)) {
		ecuData->m_csDTC = "No reported faults.";
	}
	else {
		ecuData->m_csDTC = "The following faults are reported:\n";
		
		// Now print the fault-codes
		if (m_ucDTC[0] & 0x80) // 
		{ // 41 - Engine Speed Signal Missing
			ecuData->m_csDTC += "41 - Engine Speed Signal Missing";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x40)
		{ // 13 - Oxygen Sensor; Open Circuit
			ecuData->m_csDTC += "13 - Oxygen Sensor; Open Circuit";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x20)
		{ // 14 - Coolant Temperature Sensor Circuit; High Temperature Indicated
			ecuData->m_csDTC += "14 - Coolant Temperature Sensor Circuit; High Temperature Indicated";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x10)
		{ // 15 - Coolant Temperature Sensor Circuit; Low Temperature Indicated
			ecuData->m_csDTC += "15 - Coolant Temperature Sensor Circuit; Low Temperature Indicated";
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
			ecuData->m_csDTC += "22 - TIP: Is the sensor plugged in?";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x02)
		{ // 23 - Mass Air Temperature (MAT) Sensor Circuit; Low Temperature Indicated
			ecuData->m_csDTC += "23 - Mass Air Temperature (MAT) Sensor Circuit; Low Temperature Indicated";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x01)
		{ // 24 - Vehicle Speed Sensor (VSS) Circuit
			ecuData->m_csDTC += "24 - Vehicle Speed Sensor (VSS) Circuit";
			ecuData->m_csDTC += "\n";
			ecuData->m_csDTC += "24 - TIP: A weak idle mixture or bad misfire can cause this.";
			ecuData->m_csDTC += "\n";
		}

		if (m_ucDTC[1] & 0x80)
		{ // 25 - Mass Air Temperature (MAT) Sensor Circuit; High Temperature Indicated
			ecuData->m_csDTC += "25 - Mass Air Temperature (MAT) Sensor Circuit; High Temperature Indicated";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x40)
		{ // 66 - Air conditioner Pressure Transducer Open or Short Circuited
			ecuData->m_csDTC += "66 - Air conditioner Pressure Transducer Open or Short Circuited";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x20)
		{ // 32 - EGR Diagnostic
			ecuData->m_csDTC += "32 - EGR Diagnostic";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x10)
		{ // 33 - Manifold Absolute Pressure (MAP) Sensor Circuit; Signal Voltage High
			ecuData->m_csDTC += "33 - Manifold Absolute Pressure (MAP) Sensor Circuit; Signal Voltage High";
			ecuData->m_csDTC += "\n";
			ecuData->m_csDTC += "33 - TIP: Is the pipe between the manifold and MAP sensor disconnected?";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x08)
		{ // 34 - Manifold Absolute Pressure (MAP) Sensor Circuit; Signal Voltage Low
			ecuData->m_csDTC += "34 - Manifold Absolute Pressure (MAP) Sensor Circuit; Signal Voltage Low";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x04)
		{ // 35 - Idle Speed Error
			ecuData->m_csDTC += "35 - Idle Speed Error";
			ecuData->m_csDTC += "\n";
			ecuData->m_csDTC += "35 - TIP: Maybe IAC Valve or bad misfire.";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x02)
		{ // 26 - Quad-Driver (QDM) Circuit
			ecuData->m_csDTC += "26 - Quad-Driver (QDM) Circuits";
			ecuData->m_csDTC += "\n";
			ecuData->m_csDTC += "26 - TIP - Some relay (probably) or secondary injector is open-circuit";
			ecuData->m_csDTC += "\n";
			if (len >54 && buffer[54] & 0x80)
			{
				ecuData->m_csDTC += "26 -B A/C Clutch, EGR, Chk Light, Fan, Wastegate or Canister Relay";
				ecuData->m_csDTC += "\n";
			}
			if (len > 54 && buffer[54] & 0x01)
			{
				ecuData->m_csDTC += "26 -A  Coolant or RPM Relay, Secondary Injectors";
				ecuData->m_csDTC += "\n";
			}
		}
		if (m_ucDTC[1] & 0x01)
		{ // 42 - Electronic Spark Timing (EST) Circuit
			ecuData->m_csDTC += "42 - Electronic Spark Timing (EST) Circuit";
			ecuData->m_csDTC += "\n";
		}

		if (m_ucDTC[2] & 0x80)
		{ // 43 - Electronic Spark Control (ESC) Circuit
			ecuData->m_csDTC += "43 - Electronic Spark Control (ESC) Circuit - Knock Sensor";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x40)
		{ // 44 - Oxygen Sensor Circuit; Lean Exhaust Indicated
			ecuData->m_csDTC += "44 - Oxygen Sensor Circuit; Lean Exhaust Indicated";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x20)
		{ // 45 - Oxygen Sensor Circuit; Rich Exhaust Indicated
			ecuData->m_csDTC += "45 - Oxygen Sensor Circuit; Rich Exhaust Indicated";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x10)
		{ // 51 - Mem-Cal Error
			ecuData->m_csDTC += "51 - Mem-Cal Error";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x08)
		{ // 31 - Baro Sensor Circuit; Signal Voltage Low or High
			ecuData->m_csDTC += "31 - Baro Sensor Circuit; Signal Voltage Low or High";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x04)
		{ // 53 - Battery Voltage Too High
			ecuData->m_csDTC += "53 - Battery Voltage Too High";
			ecuData->m_csDTC += "\n";
			ecuData->m_csDTC += "53 - TIP: Check alternator and, if faulty, also check battery for damage.";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x02)
		{ // 80 - Oil Temperature Sensor Too High
			ecuData->m_csDTC += "80 - Oil Temperature Sensor Too High";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x01)
		{ // 65 - Fuel Injector Circuit, Low Current
			ecuData->m_csDTC += "65 - Fuel Injector Circuit, Low Current";
			ecuData->m_csDTC += "\n";
			ecuData->m_csDTC += "65 - TIP: Check battery volts, is the alternator OK?";
			ecuData->m_csDTC += "\n";
		}
	}
}
