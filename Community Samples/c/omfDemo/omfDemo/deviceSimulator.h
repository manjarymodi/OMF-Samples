/*

©2009-2018 OSIsoft, LLC. All Rights Reserved.

No Warranty or Liability.  The OSIsoft Samples contained herein are being supplied to Licensee
"AS IS" without any warranty of any kind.  OSIsoft DISCLAIMS ALL EXPRESS AND IMPLIED WARRANTIES,
INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE and NONINFRINGEMENT. In no event will OSIsoft be liable to Licensee or to any third party
for damages of any kind arising from Licensee's use of the OSIsoft Samples OR OTHERWISE, including
but not limited to direct, indirect, special, incidental and consequential damages, and Licensee
expressly assumes the risk of all such damages.  FURTHER, THE OSIsoft SAMPLES ARE NOT ELIGIBLE FOR
SUPPORT UNDER EITHER OSISOFT'S STANDARD OR ENTERPRISE LEVEL SUPPORT AGREEMENTS

*/

/* deviceSimulator.h*/

#pragma once

#define MAX_ELEMENTS 3

#define NUM_VEHICLES 3
#define NUM_ENGINES 3
#define NUM_TRANSMISSIONS 3

#define NUM_VALID_MAKES 3

#define STRING_LENGTH_MED  100
#define STRING_LENGTH_LARGE 500

struct Vehicle {
	char szName[STRING_LENGTH_MED];
	char szVIN[STRING_LENGTH_MED];
	int year;
	char szMake[STRING_LENGTH_MED];
	char szModel[STRING_LENGTH_MED];
	char szInServiceDate[STRING_LENGTH_MED];
};

struct Engine {
	char szName[STRING_LENGTH_MED];
	char szSerialNumber[STRING_LENGTH_MED];
	int cylinders;
	char szManufacturer[STRING_LENGTH_MED];
	char szInServiceDate[STRING_LENGTH_MED];
};

struct Transmission {
	char szName[STRING_LENGTH_MED];
	char szSerialNumber[STRING_LENGTH_MED];
	char szManufacturer[STRING_LENGTH_MED];
	char szInServiceDate[STRING_LENGTH_MED];
};

void StartSimulator();
void GetVehicleNames(char* , int, int);
void GetEngineNames(char* , int, int);
void GetTransmissionNames(char* , int, int);
void GetSimulatedRTValues(char* szTimestamps, int sLength, double* dVal, int nArraySize, int dValOffset);
void GetSimulatedRTData(char* szTimestamps, int sLength, double* dVal, int nArraySize, int nTags, int dValOffset);