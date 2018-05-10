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

/* deviceSimulator.c */

#include <stdio.h>
#include <time.h>
#include "baseBuild.h"
#include "deviceSimulator.h"

struct Vehicle g_theVehicles[NUM_VEHICLES];
struct Engine g_theEngines[NUM_ENGINES];
struct Transmission g_theTransmissions[NUM_TRANSMISSIONS];

char g_validMakes[NUM_VALID_MAKES][STRING_LENGTH_MED] = { "Audi", "BMW", "Mercedes-Benz" };
enum g_validCylinders {
	cylThree = 3,
	cylFour = 4,
	cylFive = 5,
	cylSix = 6,
	cylSeven = 7,
	cylEight = 8,
	cylTen = 10,
	cylTwelve = 12,
	cylFourteen = 14,
	cylSixteen = 16
};

int g_nCylinders[] = { cylThree, cylFour, cylFive, cylSix, cylSeven, cylEight, cylTen, cylTwelve, cylFourteen, cylSixteen };
int g_nEnumCylinders;

void Simul_Vehicles(struct Vehicle* theVehicles, int n)
{
	int idx;

	for (idx = 0; idx < n; idx++) {
		snprintf(theVehicles[idx].szName, STRING_LENGTH_MED, "Vehicle_%d", idx + 1);
		snprintf(theVehicles[idx].szVIN, STRING_LENGTH_MED, "VIN_%d", idx + 5000 + 1);
		theVehicles[idx].year = 2000 + idx;
		snprintf(theVehicles[idx].szInServiceDate, STRING_LENGTH_MED, "%s", get_time( time(NULL) ));
	}

	snprintf(theVehicles[0].szMake, STRING_LENGTH_MED, g_validMakes[0]);
	snprintf(theVehicles[0].szModel, STRING_LENGTH_MED, "740il");

	snprintf(theVehicles[1].szMake, STRING_LENGTH_MED, g_validMakes[1]);
	snprintf(theVehicles[1].szModel, STRING_LENGTH_MED, "E350");

	snprintf(theVehicles[2].szMake, STRING_LENGTH_MED, g_validMakes[2]);
	snprintf(theVehicles[2].szModel, STRING_LENGTH_MED, "S4");
}

void Simul_Engines(struct Engine* theEngines, int n)
{
	int idx;
	for (idx = 0; idx < n; idx++) {
		snprintf(theEngines[idx].szName, STRING_LENGTH_MED, "Engine_%d", idx + 1);
		snprintf(theEngines[idx].szSerialNumber, STRING_LENGTH_MED, "E_SerialNumber_%d", idx + 1);
		snprintf(theEngines[idx].szInServiceDate, STRING_LENGTH_MED, "%s", get_time( time(NULL) ));
	}

	g_nEnumCylinders = sizeof(g_nCylinders) / sizeof(g_nCylinders[0]);

	theEngines[0].cylinders = cylEight; // 8 cylinders
	snprintf(theEngines[0].szManufacturer, STRING_LENGTH_MED, "BMW Engines");

	theEngines[1].cylinders = cylSix; // 6 cylinders
	snprintf(theEngines[1].szManufacturer, STRING_LENGTH_MED, "Mercedes-Benz Engines");

	theEngines[2].cylinders = cylFour; // 4 cylinders
	snprintf(theEngines[2].szManufacturer, STRING_LENGTH_MED, "Audi Engines");
}

void Simul_Transmissions(struct Transmission* theElements, int n)
{
	int idx;
	for (idx = 0; idx < n; idx++) {
		snprintf(theElements[idx].szName, STRING_LENGTH_MED, "Transmission_%d", idx + 1);
		snprintf(theElements[idx].szSerialNumber, STRING_LENGTH_MED, "T_SerialNumber_%d", idx + 100 + 1);
		snprintf(theElements[idx].szInServiceDate, STRING_LENGTH_MED, "%s", get_time(time(NULL)));
	}

	snprintf(theElements[0].szManufacturer, STRING_LENGTH_MED, "BMW Transmissions");
	snprintf(theElements[1].szManufacturer, STRING_LENGTH_MED, "Mercedes-Benz Transmissions");
	snprintf(theElements[2].szManufacturer, STRING_LENGTH_MED, "Audi Transmissions");
}

void StartSimulator()
{
	Simul_Vehicles(g_theVehicles, NUM_VEHICLES);
	Simul_Engines(g_theEngines, NUM_ENGINES);
	Simul_Transmissions(g_theTransmissions, NUM_TRANSMISSIONS);
}

void GetVehicleNames(char* szVehicle, int nVehicles, int nameLength)
{
	int idx;
	for (idx = 0; idx < nVehicles; idx++) {
		snprintf(szVehicle + idx*nameLength, nameLength, "%s", g_theVehicles[idx].szName);
	}
}

void GetEngineNames(char* szEngine, int nEngines, int nameLength)
{
	int idx;
	for (idx = 0; idx < nEngines; idx++) {
		snprintf(szEngine + idx*nameLength, nameLength, "%s", g_theEngines[idx].szName);
	}
}

void GetTransmissionNames(char* szTransmission, int nTransmissions, int nameLength)
{
	int idx;
	for (idx = 0; idx < nTransmissions; idx++) {
		snprintf(szTransmission + idx*nameLength, nameLength, "%s", g_theTransmissions[idx].szName);
	}
}

void GetSimulatedRTValues(char* szTimestamps, int sLength, double* dVal, int nArraySize, int dValOffset)
{
	int idx;
	time_t tNow = time(NULL);
#ifdef _WIN32
	struct tm local;
	localtime_s(&local, &tNow);
#elif linux
	struct tm* local = localtime(&tNow);
#else
	need to implement
#endif

	for (idx = 0; idx < nArraySize; idx++) {
		snprintf(szTimestamps + idx*sLength, sLength, "%s", get_time( tNow - nArraySize + idx));
#ifdef _WIN32
		dVal[idx] = local.tm_mday + local.tm_sec + dValOffset + 1.12 * (idx+1);
#elif linux
		dVal[idx] = local->tm_mday + local->tm_sec + dValOffset + 1.12 * (idx + 1);
#else
		need to implement
#endif
	}

}

void GetSimulatedRTData(char* szTimestamps, int sLength, double* dVal, int nArraySize, int nTags, int dValOffset)
{
	int idx;
	int jdx;
	time_t tNow = time(NULL);
#ifdef _WIN32
	struct tm local;
	localtime_s(&local, &tNow);
#elif linux
	struct tm* local = localtime(&tNow);
#else
	need to implement
#endif

	for (idx = 0; idx < nArraySize; idx++) {
		snprintf(szTimestamps + idx*sLength, sLength, "%s", get_time(tNow - nArraySize + idx));
		for (jdx = 0; jdx < nTags; jdx++) {
#ifdef _WIN32
			*(dVal + idx + jdx*nArraySize) = local.tm_mday + local.tm_sec + dValOffset + 1.5 * (idx + 1) + ((jdx+1)*1000);
#elif linux
			*(dVal + idx + jdx*nArraySize) = local->tm_mday + local->tm_sec + dValOffset + 1.5 * (idx + 1) + ((jdx + 1) * 1000);
#else
			need to implement
#endif
		}
	}

}