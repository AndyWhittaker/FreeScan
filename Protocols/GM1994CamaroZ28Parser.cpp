// GM1994CamaroZ28Parser.cpp: implementation of the CGM1994CamaroZ28Parser class.
//
//////////////////////////////////////////////////////////////////////
// (c) 1996-2000 Andy Whittaker, Chester, England. 
// mail@andywhittaker.com
//
// This contains all low-level parsing functions.

#include "GM1994CamaroZ28Parser.h"

#include "GMBaseFunctions.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGM1994CamaroZ28Parser::CGM1994CamaroZ28Parser(CBaseProtocol* const m_pProtocol) : CBaseParser(m_pProtocol) {
	memset(m_ucDTC, 0, sizeof(m_ucDTC));// Reset DTC buffer
}

CGM1994CamaroZ28Parser::~CGM1994CamaroZ28Parser() {
}

void CGM1994CamaroZ28Parser::InitializeSupportedValues(CEcuData* const ecuData) {
	ecuData->m_csDTC = "No reported faults.";
	ecuData->m_bEngineClosedLoop = CEcuData::c_bSUPPORTED_BY_PROTOCOL;
	ecuData->m_bACRequest = CEcuData::c_bSUPPORTED_BY_PROTOCOL;
	ecuData->m_bACClutch = CEcuData::c_bSUPPORTED_BY_PROTOCOL;
	ecuData->m_iDesiredIdle = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iRPM = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fWaterTemp = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_fWaterVolts = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iWaterTempADC = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fMATVolts = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iMATADC = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fMATTemp = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_fMAP = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_fMAPVolts = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iMAPADC = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fThrottleVolts = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iThrottleADC = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iThrottlePos = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fBatteryVolts = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_fBaro = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_fBaroVolts = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iBaroADC = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fO2VoltsLeft = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_fO2VoltsRight = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iBLMCell = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iBLM = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iBLMRight = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iIntegratorL = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iIntegratorR = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iInjectorBasePWMsL = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iInjectorBasePWMsR = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fAirFlow = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iIACPosition = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_fSparkAdvance = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_fKnockRetard = CEcuData::c_fSUPPORTED_BY_PROTOCOL;
	ecuData->m_iKnockCount = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iMPH = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
	ecuData->m_iRunTime = CEcuData::c_iSUPPORTED_BY_PROTOCOL;
}

// Parses the buffer into real data
BOOL CGM1994CamaroZ28Parser::Parse(const unsigned char* const buffer, int const length, CEcuData* const ecuData) {
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
		case ECU_HEADER_GM1994CAMAROZ28: // Packet Header Start
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
						if(ucLenByte==0x92) // length 61 including mode byte
							dataWasUpdated |= ParseMode1_0(&buffer[iIndex], iPacketLength, ecuData);
						else if(ucLenByte==0x84) // length 47 including mode byte
							dataWasUpdated |= ParseMode1_1(&buffer[iIndex], iPacketLength, ecuData);
						else if(ucLenByte==0x8b) // length 54 including mode byte
							dataWasUpdated |= ParseMode1_2(&buffer[iIndex], iPacketLength, ecuData);
						else if(ucLenByte==0x83) // length 46 including mode byte
							dataWasUpdated |= ParseMode1_4(&buffer[iIndex], iPacketLength, ecuData);
						else if(ucLenByte==0x7c) // length 39 including mode byte
							dataWasUpdated |= ParseMode1_6(&buffer[iIndex], iPacketLength, ecuData);
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
		case ECU_CHATTER_HEADER_GM1994CAMAROZ28: // from chatter
			{
				iIndex++; // now find the length
				iPacketLength = CGMBaseFunctions::GetLength((int)buffer[iIndex]); // Length of data
				iIndex++; // This has the mode number
				// No data so don't parse.
//				ParseMode0A(&buffer[iIndex], iPacketLength, ecuData);
				iIndex += iPacketLength; // should be 3
				// Check CRC
				if (!CGMBaseFunctions::CheckChecksum(pPacketStart, 3 + iPacketLength))
					WriteStatus("Checksum Error on 0xf0");
				break;
			}
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
		case 0x10: // from chatter
			{
				iIndex++; // now find the length
				iPacketLength = CGMBaseFunctions::GetLength((int)buffer[iIndex]); // Length of data
				iIndex++; // This has the mode number
				dataWasUpdated |= ParseCMode10(&buffer[iIndex], iPacketLength, ecuData);
				iIndex += iPacketLength; // should be 3
				// Check CRC
				if (!CGMBaseFunctions::CheckChecksum(pPacketStart, 3 + iPacketLength))
					WriteStatus("Checksum Error on 0x10");
				break;
			}
		case 0x40: // from chatter
			{
				iIndex++; // now find the length
				iPacketLength = CGMBaseFunctions::GetLength((int)buffer[iIndex]); // Length of data
				iIndex++; // This has the mode number
				dataWasUpdated |= ParseMode40(&buffer[iIndex], iPacketLength, ecuData);
				iIndex += iPacketLength; // should be 3
				// Check CRC
				if (!CGMBaseFunctions::CheckChecksum(pPacketStart, 3 + iPacketLength))
					WriteStatus("Checksum Error on 0x40");
				break;
			}
		case 0x41: // from chatter
			{
				iIndex++; // now find the length
				iPacketLength = CGMBaseFunctions::GetLength((int)buffer[iIndex]); // Length of data
				iIndex++; // This has the mode number
				dataWasUpdated |= ParseMode41(&buffer[iIndex], iPacketLength, ecuData);
				iIndex += iPacketLength; // should be 3
				// Check CRC
				if (!CGMBaseFunctions::CheckChecksum(pPacketStart, 3 + iPacketLength))
					WriteStatus("Checksum Error on 0x41");
				break;
			}
		case 0x45: // from chatter
			{
				iIndex++; // now find the length
				iPacketLength = CGMBaseFunctions::GetLength((int)buffer[iIndex]); // Length of data
				iIndex++; // This has the mode number
				dataWasUpdated |= ParseMode45(&buffer[iIndex], iPacketLength, ecuData);
				iIndex += iPacketLength; // should be 3
				// Check CRC
				if (!CGMBaseFunctions::CheckChecksum(pPacketStart, 3 + iPacketLength))
					WriteStatus("Checksum Error on 0x45");
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
		case 0x92: // from chatter
			{
				iIndex++; // now find the length
				iPacketLength = CGMBaseFunctions::GetLength((int)buffer[iIndex]); // Length of data
				iIndex++; // This has the mode number
				dataWasUpdated |= ParseMode92(&buffer[iIndex], iPacketLength, ecuData);
				iIndex += iPacketLength; // should be 5
				// Check CRC
				if (!CGMBaseFunctions::CheckChecksum(pPacketStart, 3 + iPacketLength))
					WriteStatus("Checksum Error on 0x92");
				break;
			}
		case 0x93: // from chatter
			{
				iIndex++; // now find the length
				iPacketLength = CGMBaseFunctions::GetLength((int)buffer[iIndex]); // Length of data
				iIndex++; // This has the mode number
				dataWasUpdated |= ParseMode93(&buffer[iIndex], iPacketLength, ecuData);
				iIndex += iPacketLength; // should be 5
				// Check CRC
				if (!CGMBaseFunctions::CheckChecksum(pPacketStart, 3 + iPacketLength))
					WriteStatus("Checksum Error on 0x93");
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
BOOL CGM1994CamaroZ28Parser::ParseMode0A(const unsigned char* /*buffer*/, int /*len*/, CEcuData* const /*ecuData*/) {
	return FALSE;
}

// Translates the incoming data stream from the chatter
BOOL CGM1994CamaroZ28Parser::ParseCMode10(const unsigned char* /*buffer*/, int /*len*/, CEcuData* const /*ecuData*/) {
	return FALSE;
}

// Translates the incoming data stream from the chatter
BOOL CGM1994CamaroZ28Parser::ParseMode40(const unsigned char* /*buffer*/, int /*len*/, CEcuData* const /*ecuData*/) {
	return FALSE;
}

// Translates the incoming data stream from the chatter
BOOL CGM1994CamaroZ28Parser::ParseMode41(const unsigned char* /*buffer*/, int /*len*/, CEcuData* const /*ecuData*/) {
	return FALSE;
}

// Translates the incoming data stream from the chatter
BOOL CGM1994CamaroZ28Parser::ParseMode45(const unsigned char* /*buffer*/, int /*len*/, CEcuData* const /*ecuData*/) {
	return FALSE;
}

// Translates the incoming data stream from the chatter
BOOL CGM1994CamaroZ28Parser::ParseMode90(const unsigned char* /*buffer*/, int /*len*/, CEcuData* const /*ecuData*/) {
	return FALSE;
}

// Translates the incoming data stream from the chatter
BOOL CGM1994CamaroZ28Parser::ParseMode92(const unsigned char* /*buffer*/, int /*len*/, CEcuData* const /*ecuData*/) {
	return FALSE;
}

// Translates the incoming data stream from the chatter
BOOL CGM1994CamaroZ28Parser::ParseMode93(const unsigned char* /*buffer*/, int /*len*/, CEcuData* const /*ecuData*/) {
	return FALSE;
}

// Translates the incoming data stream as Mode 1 Msg 0
BOOL CGM1994CamaroZ28Parser::ParseMode1_0(const unsigned char* buffer, int len, CEcuData* const ecuData) {
	if (len<10) // remember half duplex. We read our commands as well
	{
		WriteStatus("Received our TX command echo for mode 1 Msg 0.");
		return FALSE;
	}
	else if (len>61)
	{
		WriteStatus("Warning: F001 larger than expected, packet truncated.");
		len = 61;
	}

	ecuData->copyToF001(buffer, len);

	// Work out real-world data from the packet.
	// Mode number is in index 0

	// Status Word 1
	if (buffer[13] & 0x01)
		ecuData->m_bEngineClosedLoop = TRUE;  // bit 7
	else
		ecuData->m_bEngineClosedLoop = FALSE; // bit 7

	if (buffer[9] & 0x02)
		ecuData->m_bACRequest = TRUE;  // mode 1, byte 10, bit 1
	else
		ecuData->m_bACRequest = FALSE; // mode 1, byte 10, bit 1

	if (buffer[9] & 0x01)
		ecuData->m_bACClutch = TRUE;  // mode 1, byte 10, bit 0
	else
		ecuData->m_bACClutch = FALSE; // mode 1, byte 10, bit 0

	// Analogues
	m_ucDTC[0] = buffer[1]; // Fault code byte 1
	m_ucDTC[1] = buffer[2]; // Fault code byte 2
	m_ucDTC[2] = buffer[3]; // Fault code byte 3
	m_ucDTC[3] = buffer[4]; // Fault code byte 4
	m_ucDTC[4] = buffer[5]; // Fault code byte 5
	m_ucDTC[5] = buffer[6]; // Fault code byte 6
	m_ucDTC[6] = buffer[7]; // Fault code byte 7
	m_ucDTC[7] = buffer[8]; // Fault code byte 8 - Bloody-hell!
	ecuData->m_iDesiredIdle = (int)((float)buffer[21] * (float) 12.5);
	ecuData->m_iRPM = buffer[22] * 25;
	ecuData->m_fWaterTemp = ((float)buffer[23] * (float)0.75) - (float)40.0; // in °C
	ecuData->m_fWaterVolts = (float)(((float)buffer[23] / (float)255.0) * (float) 5.0);
	ecuData->m_iWaterTempADC = buffer[23];
	ecuData->m_fMATVolts = ((float)buffer[25] / (float)255.0) * (float)5.0; // in Volts
	ecuData->m_iMATADC = buffer[25];
	ecuData->m_fMATTemp = ((float)buffer[25] * (float)0.75) - (float)40.0; // in °C
	ecuData->m_fMAP = (((float)buffer[28] + (float)28.06) / (float)271); // in Bar Absolute
	ecuData->m_fMAPVolts = ((float)buffer[28] / (float) 255.0) * (float) 5.0; // in Volts
	ecuData->m_iMAPADC = buffer[28];
	ecuData->m_fThrottleVolts = (float)(((float)buffer[29] / (float)255.0) * (float) 5.0);
	ecuData->m_iThrottleADC = buffer[29];
	ecuData->m_iThrottlePos = (int)((float)buffer[30] / (float)2.55);
	ecuData->m_fBatteryVolts = (float)buffer[31] / (float)10.0;
	ecuData->m_fBaro = (((float)buffer[32] + (float)28.06) / (float)271); // in Bar Absolute
	ecuData->m_fBaroVolts = ((float)buffer[32] / (float) 255.0) * (float) 5.0; // in Volts
	ecuData->m_iBaroADC = buffer[32];
	ecuData->m_fO2VoltsLeft = (float) buffer[33] * (float) 0.00444; // 1st Bank
	ecuData->m_fO2VoltsRight = (float) buffer[34] * (float) 0.00444; // 2nd Bank
	ecuData->m_iBLMCell = (int)buffer[37];
	ecuData->m_iBLM = (int)buffer[38];
	ecuData->m_iBLMRight = (int)buffer[39];
	ecuData->m_iIntegratorL = (int)buffer[40];
	ecuData->m_iIntegratorR = (int)buffer[41];
	ecuData->m_iInjectorBasePWMsL = (int) ( (float)((buffer[42] * 256) + buffer[43]) / (float)65.536);
	ecuData->m_iInjectorBasePWMsR = (int) ( (float)((buffer[44] * 256) + buffer[45]) / (float)65.536);
	ecuData->m_fAirFlow = ((float)((buffer[46] * 256) + buffer[47]) * (float)512.0) / (float) 65536.0;
	ecuData->m_iIACPosition = (int)buffer[49];
	ecuData->m_fSparkAdvance = (float)buffer[51]; // in °
	ecuData->m_fKnockRetard = ((float)buffer[52] / (float)16.0); // in °
	ecuData->m_iKnockCount = (int)(buffer[53] * 256) + buffer[54];
	ecuData->m_iMPH = (int)buffer[58]; // Count is in MPH
	ecuData->m_iRunTime = (buffer[59] * 256) + buffer[60]; // Total running time

	ParseDTCs(ecuData); // Process the DTCs into text

	return TRUE;
}

// Translates the incoming data stream as Mode 1 Msg 1
BOOL CGM1994CamaroZ28Parser::ParseMode1_1(const unsigned char* /*buffer*/, int len, CEcuData* const /*ecuData*/) {
	if (len==0) // remember half duplex. We read our commands as well
	{
		WriteStatus("0 Received our TX command echo for mode 1 Msg 1.");
		return FALSE;
	}
	else if (len==1) // remember half duplex. We read our commands as well
	{
		WriteStatus("1 Received our TX command echo for mode 1 Msg 1.");
		return FALSE;
	}
	else if (len>47)
	{
		WriteStatus("Warning: F001 larger than expected, packet truncated.");
		len = 47;
	}

	return TRUE;
}

// Translates the incoming data stream as Mode 1 Msg 2
BOOL CGM1994CamaroZ28Parser::ParseMode1_2(const unsigned char* /*buffer*/, int len, CEcuData* const /*ecuData*/) {
	if (len==0) // remember half duplex. We read our commands as well
	{
		WriteStatus("0 Received our TX command echo for mode 1 Msg 2.");
		return FALSE;
	}
	else if (len==1) // remember half duplex. We read our commands as well
	{
		WriteStatus("1 Received our TX command echo for mode 1 Msg 2.");
		return FALSE;
	}
	else if (len>54)
	{
		WriteStatus("Warning: F001 larger than expected, packet truncated.");
		len = 54;
	}

	return FALSE;
}

// Translates the incoming data stream as Mode 1 Msg 4
BOOL CGM1994CamaroZ28Parser::ParseMode1_4(const unsigned char* /*buffer*/, int len, CEcuData* const /*ecuData*/) {
	if (len==0) // remember half duplex. We read our commands as well
	{
		WriteStatus("0 Received our TX command echo for mode 1 Msg 4.");
		return FALSE;
	}
	else if (len==1) // remember half duplex. We read our commands as well
	{
		WriteStatus("1 Received our TX command echo for mode 1 Msg 4.");
		return FALSE;
	}
	else if (len>46)
	{
		WriteStatus("Warning: F001 larger than expected, packet truncated.");
		len = 46;
	}

	return TRUE;
}

// Translates the incoming data stream as Mode 1 Msg 6
BOOL CGM1994CamaroZ28Parser::ParseMode1_6(const unsigned char* /*buffer*/, int len, CEcuData* const /*ecuData*/) {
	if (len==0) // remember half duplex. We read our commands as well
	{
		WriteStatus("0 Received our TX command echo for mode 1 Msg 6.");
		return FALSE;
	}
	else if (len==1) // remember half duplex. We read our commands as well
	{
		WriteStatus("1 Received our TX command echo for mode 1 Msg 6.");
		return FALSE;
	}
	else if (len>39)
	{
		WriteStatus("Warning: F001 larger than expected, packet truncated.");
		len = 39;
	}

	return FALSE;
}

// Translates the incoming data stream as Mode 2
BOOL CGM1994CamaroZ28Parser::ParseMode2(const unsigned char* buffer, int len, CEcuData* const ecuData) {
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
BOOL CGM1994CamaroZ28Parser::ParseMode3(const unsigned char* buffer, int len, CEcuData* const ecuData) {
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
	m_ucDTC[0] = buffer[1]; // Fault code byte 1
	m_ucDTC[1] = buffer[2]; // Fault code byte 2
	m_ucDTC[2] = buffer[3]; // Fault code byte 3

	ParseDTCs(ecuData); // Process the DTCs into text

	return TRUE;
}

// Translates the incoming data stream as Mode 4
BOOL CGM1994CamaroZ28Parser::ParseMode4(const unsigned char* buffer, int len, CEcuData* const ecuData) {
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
BOOL CGM1994CamaroZ28Parser::ParseMode7(const unsigned char* /*buffer*/, int len, CEcuData* const /*ecuData*/) {
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
BOOL CGM1994CamaroZ28Parser::ParseMode8(const unsigned char* /*buffer*/, int len, CEcuData* const /*ecuData*/) {
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
BOOL CGM1994CamaroZ28Parser::ParseMode9(const unsigned char* /*buffer*/, int len, CEcuData* const /*ecuData*/) {
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
BOOL CGM1994CamaroZ28Parser::ParseMode10(const unsigned char* /*buffer*/, int len, CEcuData* const /*ecuData*/) {
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
void CGM1994CamaroZ28Parser::ParseDTCs(CEcuData *const ecuData) {

	ecuData->m_csDTC.Empty();

	if ((m_ucDTC[0] == 0) && (m_ucDTC[1] == 0) && (m_ucDTC[2] == 0) && (m_ucDTC[3] == 0) && (m_ucDTC[4] == 0) && (m_ucDTC[5] == 0) && (m_ucDTC[6] == 0) && (m_ucDTC[7] == 0)) {
		ecuData->m_csDTC = "No reported faults.";
	}
	else {
		ecuData->m_csDTC = "The following faults are reported:\n";
		
		// Now print the fault-codes
		// MALFFW1     LOGGED MALF FLAG WORD 1
		if (m_ucDTC[0] & 0x80)
		{ //
			ecuData->m_csDTC += "CODE 79  TRANSMISSION OVER TEMPERATURE";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x40)
		{ //
			ecuData->m_csDTC += "CODE 80  TRANSMISSION COMPONENT SLIPPING";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x20)
		{ //
			ecuData->m_csDTC += "CODE 81  SHIFT B SOLENOID FAULT (ODM)";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x10)
		{ //
			ecuData->m_csDTC += "CODE 82  SHIFT A SOLENOID FAULT (ODM)";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x08)
		{ //
			ecuData->m_csDTC += "CODE 83  TCC CONTROL FEEDBACK FAULT (ODM)";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x04)
		{ //
			ecuData->m_csDTC += "CODE 84  3-2 DOWNSHIFT FEEDBACK FAULT (ODM)";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x02)
		{ //
			ecuData->m_csDTC += "CODE 85  TCC STUCK ON";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[0] & 0x01)
		{ //
			ecuData->m_csDTC += "CODE 86  LOW RATIO";
			ecuData->m_csDTC += "\n";
		}

		// MALFFW2     LOGGED MALF FLAG WORD 2
		if (m_ucDTC[1] & 0x80)
		{ //
			ecuData->m_csDTC += "CODE 11  MALF INDICATOR LAMP (MIL) FAULT";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x40)
		{ //
			ecuData->m_csDTC += "CODE 12  NO REFERENCE PULSES";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x20)
		{ //
			ecuData->m_csDTC += "CODE 13  LEFT O2 SENSOR FAILED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x10)
		{ //
			ecuData->m_csDTC += "CODE 16  LO-RES FAILURE";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x08)
		{ //
			ecuData->m_csDTC += "CODE 23  MAT SENSOR LOW";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x04)
		{ //
			ecuData->m_csDTC += "CODE 25  MAT SENSOR HIGH";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x02)
		{ //
			ecuData->m_csDTC += "CODE 26  CCP ELECTRICAL FAULT (ODM)";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[1] & 0x01)
		{ //
			ecuData->m_csDTC += "CODE 27  EGR ELECTRICAL FAULT (ODM)";
			ecuData->m_csDTC += "\n";
		}

		// MALFFW3     LOGGED MALF FLAG WORD 3
		if (m_ucDTC[2] & 0x80)
		{ //
			ecuData->m_csDTC += "CODE 29  EAS ELECTRICAL FAULT (ODM)";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x40)
		{ //
			ecuData->m_csDTC += "CODE 32  EGR SYSTEM FAULT";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x20)
		{ //
			ecuData->m_csDTC += "CODE 47  NOT USED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x10)
		{ //
			ecuData->m_csDTC += "CODE 48  MASS AIR FLOW SYSTEM FAILURE";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x08)
		{ //
			ecuData->m_csDTC += "CODE 36  HI-RES FAILURE";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x04)
		{ //
			ecuData->m_csDTC += "CODE 41  EST OPEN";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x02)
		{ //
			ecuData->m_csDTC += "CODE 42  EST GROUNDED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[2] & 0x01)
		{ //
			ecuData->m_csDTC += "CODE 43  ESC FAILURE";
			ecuData->m_csDTC += "\n";
		}

		// MALFFW4     LOGGED MALF FLAG WORD 4
		if (m_ucDTC[3] & 0x80)
		{ //
			ecuData->m_csDTC += "CODE 44  LEFT O2 SENSOR LEAN";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[3] & 0x40)
		{ //
			ecuData->m_csDTC += "CODE 45  LEFT O2 SENSOR RICH";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[3] & 0x20)
		{ //
			ecuData->m_csDTC += "CODE 46  FUEL ENABLE FAILURE";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[3] & 0x10)
		{ //
			ecuData->m_csDTC += "CODE 50  SYSTEM VOLTAGE LOW";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[3] & 0x08)
		{ //
			ecuData->m_csDTC += "CODE 52  OIL TEMPERATURE LOW";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[3] & 0x04)
		{ //
			ecuData->m_csDTC += "CODE 54  NOT USED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[3] & 0x02)
		{ //
			ecuData->m_csDTC += "CODE 55  FUEL SYSTEM LEAN";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[3] & 0x01)
		{ //
			ecuData->m_csDTC += "CODE 61  A/C LOW CHARGE";
			ecuData->m_csDTC += "\n";
		}

		// MALFFW5     LOGGED MALF FLAG WORD 5
		if (m_ucDTC[4] & 0x80)
		{ //
			ecuData->m_csDTC += "CODE 62  OIL TEMPERATURE HIGH";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[4] & 0x40)
		{ //
			ecuData->m_csDTC += "CODE 63  RIGHT O2 SENSOR FAILED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[4] & 0x20)
		{ //
			ecuData->m_csDTC += "CODE 64  RIGHT O2 SENSOR LEAN";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[4] & 0x10)
		{ //
			ecuData->m_csDTC += "CODE 65  RIGHT O2 SENSOR RICH";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[4] & 0x08)
		{ //
			ecuData->m_csDTC += "CODE 66  A/C PRESSURE TRANSDUCER OPEN/SHORT";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[4] & 0x04)
		{ //
			ecuData->m_csDTC += "CODE 67  A/C PRESSURE TRANSDUCER DEGRADED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[4] & 0x02)
		{ //
			ecuData->m_csDTC += "CODE 68  A/C RELAY SHORTED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[4] & 0x01)
		{ //
			ecuData->m_csDTC += "CODE 69  A/C CLUTCH CIRCUIT OPEN";
			ecuData->m_csDTC += "\n";
		}

		// MALFFW6     LOGGED MALF FLAG WORD 6
 		if (m_ucDTC[5] & 0x80)
		{ //
			ecuData->m_csDTC += "CODE 17  NOT USED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[5] & 0x40)
		{ //
			ecuData->m_csDTC += "CODE 18  NOT USED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[5] & 0x20)
		{ //
			ecuData->m_csDTC += "CODE 19  NOT USED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[5] & 0x10)
		{ //
			ecuData->m_csDTC += "CODE 31  NOT USED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[5] & 0x08)
		{ //
			ecuData->m_csDTC += "CODE 70  A/C CLUTCH RELAY FAULT (ODM)";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[5] & 0x04)
		{ //
			ecuData->m_csDTC += "CODE 71  A/C EVAPORATOR TEMP SENSOR FAILED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[5] & 0x02)
		{ //
			ecuData->m_csDTC += "CODE 77  FAN 1 FAULT (ODM)";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[5] & 0x01)
		{ //
			ecuData->m_csDTC += "CODE 78  FAN 2 FAULT (ODM)";
			ecuData->m_csDTC += "\n";
		}

		// MALFFW7     LOGGED MALF FLAG WORD 7
		if (m_ucDTC[6] & 0x80)
		{ //
			ecuData->m_csDTC += "CODE 24  OUTPUT SPEED LOW";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[6] & 0x40)
		{ //
			ecuData->m_csDTC += "CODE 35  NOT USED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[6] & 0x20)
		{ //
			ecuData->m_csDTC += "CODE 39  NOT USED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[6] & 0x10)
		{ //
			ecuData->m_csDTC += "CODE 33  MAP SENSOR HIGH";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[6] & 0x08)
		{ //
			ecuData->m_csDTC += "CODE 34  MAP SENSOR LOW";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[6] & 0x04)
		{ //
			ecuData->m_csDTC += "CODE 49  NOT USED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[6] & 0x02)
		{ //
			ecuData->m_csDTC += "CODE 56  NOT USED";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[6] & 0x01)
		{ //
			ecuData->m_csDTC += "CODE 57  NOT USED";
			ecuData->m_csDTC += "\n";
		}

		// MALFFW8     LOGGED MALF FLAG WORD 8
		if (m_ucDTC[7] & 0x80)
		{ //
			ecuData->m_csDTC += "CODE 14  COOLANT TEMPERATURE HIGH";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[7] & 0x40)
		{ //
			ecuData->m_csDTC += "CODE 15  COOLANT TEMPERATURE LOW";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[7] & 0x20)
		{ //
			ecuData->m_csDTC += "CODE 21  THROTTLE POSITION HIGH";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[7] & 0x10)
		{ //
			ecuData->m_csDTC += "CODE 22  THROTTLE POSITION LOW";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[7] & 0x08)
		{ //
			ecuData->m_csDTC += "CODE 37  BRAKE SWITCH STUCK ON";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[7] & 0x04)
		{ //
			ecuData->m_csDTC += "CODE 38  BRAKE SWITCH STUCK OFF";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[7] & 0x02)
		{ //
			ecuData->m_csDTC += "CODE 51  PROM/FLASH ERROR";
			ecuData->m_csDTC += "\n";
		}
		if (m_ucDTC[7] & 0x01)
		{ //
			ecuData->m_csDTC += "CODE 53  SYSTEM VOLTAGE HIGH";
			ecuData->m_csDTC += "\n";
		}
	}

}

