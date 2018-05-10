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

/* getValuesAndStreams.c */

#include "baseBuild.h"
#include "buildTypes.h"
#include "buildStreams.h"
#include "buildObjects.h"
#include "buildData.h"

#include "getValuesAndStreams.h"

#include "deviceSimulator.h"

char* getStreams_RTValues(int code)
{
	char* szRet = NULL;
	char szEngine[NUM_ENGINES][STRING_LENGTH_MED];
	char szVehicle[NUM_VEHICLES][STRING_LENGTH_MED];

	switch (code) {
		case RTVAL_ENGINE_RPM:
			GetEngineNames(&(szEngine[0][0]), NUM_ENGINES, STRING_LENGTH_MED);
			szRet = defineRTStreams(STREAM_VStr_vRPM, &(szEngine[0][0]), 
				NUM_ENGINES, STRING_LENGTH_MED, TYPE_Value_vRPM);
		break;

		case RTVAL_ENGINE_COOLANTTEMP:
			GetEngineNames(&(szEngine[0][0]), NUM_ENGINES, STRING_LENGTH_MED);
			szRet = defineRTStreams(STREAM_VStr_vCoolantTemp, &(szEngine[0][0]), 
				NUM_ENGINES, STRING_LENGTH_MED, TYPE_Value_vCoolantTemp);
		break;

		case RTVAL_VEHICLE_MEAS:
			GetVehicleNames(&(szVehicle[0][0]), NUM_VEHICLES, STRING_LENGTH_MED);
			szRet = defineRTStreams(STREAM_VStr_vVehicleMeas, &(szVehicle[0][0]), 
				NUM_VEHICLES, STRING_LENGTH_MED, TYPE_Value_vVehicleMeas);
		break;

		case RTVAL_VEHICLE_STATUS:
			GetVehicleNames(&(szVehicle[0][0]), NUM_VEHICLES, STRING_LENGTH_MED);
			szRet = defineRTStreams(STREAM_VStr_vVehicleStatus, &(szVehicle[0][0]),
				NUM_VEHICLES, STRING_LENGTH_MED, TYPE_Value_vVehicleStatus);
		break;

		case RTVAL_VEHICLE_FLOAT:
			GetVehicleNames(&(szVehicle[0][0]), NUM_VEHICLES, STRING_LENGTH_MED);
			szRet = defineRTStreams(STREAM_VStr_vVehicleFloat, &(szVehicle[0][0]),
				NUM_VEHICLES, STRING_LENGTH_MED, TYPE_Value_vVehicleFloat);
		break;

		case RTVAL_VEHICLE_INT:
			GetVehicleNames(&(szVehicle[0][0]), NUM_VEHICLES, STRING_LENGTH_MED);
			szRet = defineRTStreams(STREAM_VStr_vVehicleInt, &(szVehicle[0][0]),
				NUM_VEHICLES, STRING_LENGTH_MED, TYPE_Value_vVehicleInt);
		break;
	}

	return ( szRet );
}

char* getAssets_Value_Links(int code)
{
	char* szRet = NULL;
	char szEngine[NUM_ENGINES][STRING_LENGTH_MED];
	char szVehicle[NUM_VEHICLES][STRING_LENGTH_MED];

	switch (code) {
		case RTVAL_ENGINE_RPM:
			GetEngineNames(&(szEngine[0][0]), NUM_ENGINES, STRING_LENGTH_MED);
			szRet = val_Link_Asset_RT(TYPE_EngineType, NULL, STREAM_VStr_vRPM, &(szEngine[0][0]), NUM_ENGINES, STRING_LENGTH_MED);
		break;
		case RTVAL_ENGINE_COOLANTTEMP:
			GetEngineNames(&(szEngine[0][0]), NUM_ENGINES, STRING_LENGTH_MED);
			szRet = val_Link_Asset_RT(TYPE_EngineType, NULL, STREAM_VStr_vCoolantTemp, &(szEngine[0][0]), NUM_ENGINES, STRING_LENGTH_MED);
		break;
		case RTVAL_VEHICLE_MEAS:
			GetVehicleNames(&(szVehicle[0][0]), NUM_VEHICLES, STRING_LENGTH_MED);
			szRet = val_Link_Asset_RT(TYPE_VehicleType, NULL, STREAM_VStr_vVehicleMeas, &(szVehicle[0][0]), NUM_VEHICLES, STRING_LENGTH_MED);
		break;
		case RTVAL_VEHICLE_STATUS:
			GetVehicleNames(&(szVehicle[0][0]), NUM_VEHICLES, STRING_LENGTH_MED);
			szRet = val_Link_Asset_RT(TYPE_VehicleType, NULL, STREAM_VStr_vVehicleStatus, &(szVehicle[0][0]), NUM_VEHICLES, STRING_LENGTH_MED);
		break;
		case RTVAL_VEHICLE_FLOAT:
			GetVehicleNames(&(szVehicle[0][0]), NUM_VEHICLES, STRING_LENGTH_MED);
			szRet = val_Link_Asset_RT(TYPE_VehicleType, NULL, STREAM_VStr_vVehicleFloat, &(szVehicle[0][0]), NUM_VEHICLES, STRING_LENGTH_MED);
		break;
		case RTVAL_VEHICLE_INT:
			GetVehicleNames(&(szVehicle[0][0]), NUM_VEHICLES, STRING_LENGTH_MED);
			szRet = val_Link_Asset_RT(TYPE_VehicleType, NULL, STREAM_VStr_vVehicleInt, &(szVehicle[0][0]), NUM_VEHICLES, STRING_LENGTH_MED);
		break;
	}

	return szRet;
}

char* getAssets_Asset_links(int code)
{
	char* szRet = NULL;

	char szVehicles[NUM_VEHICLES][STRING_LENGTH_MED];
	char szEngines[NUM_ENGINES][STRING_LENGTH_MED];
	char szTransmissions[NUM_TRANSMISSIONS][STRING_LENGTH_MED];

	GetVehicleNames(&(szVehicles[0][0]), NUM_VEHICLES, STRING_LENGTH_MED);

	switch (code) {
		case ASSET_LINK_VEHICLE_VEHICLE:
		szRet = defineLink_Sources_Targets(NULL, TYPE_VehicleType, NULL, &(szVehicles[0][0]), TYPE_VehicleType, NULL, NUM_VEHICLES, STRING_LENGTH_MED);
		break;

		case ASSET_LINK_VEHICLE_ENGINE:
		GetEngineNames(&(szEngines[0][0]), NUM_ENGINES, STRING_LENGTH_MED);
		szRet = defineLink_Sources_Targets(&(szVehicles[0][0]), TYPE_VehicleType, NULL, &(szEngines[0][0]), TYPE_EngineType, NULL, NUM_VEHICLES, STRING_LENGTH_MED);
		break;

		case ASSET_LINK_VEHICLE_TRANSMISSION:
		GetTransmissionNames(&(szTransmissions[0][0]), NUM_TRANSMISSIONS, STRING_LENGTH_MED);
		szRet = defineLink_Sources_Targets(&(szVehicles[0][0]), TYPE_VehicleType, NULL, &(szTransmissions[0][0]), TYPE_TransmissionType, NULL, NUM_VEHICLES, STRING_LENGTH_MED);
		break;
	}

	return szRet;
}

char* getValues_EngineCoolantTempData()
{
#define NUM_COOLANT_VALUES  4
	int idx;
	JsonNode* jn[NUM_ENGINES];

	char szStreamId[NUM_ENGINES][STRING_LENGTH_MED];

	char szTimeCoolant[NUM_COOLANT_VALUES][STRING_LENGTH_MED];
	double dValCoolant[NUM_COOLANT_VALUES];

	char szEngines[NUM_ENGINES][STRING_LENGTH_MED];
	GetEngineNames(&(szEngines[0][0]), NUM_ENGINES, STRING_LENGTH_MED);

	for (idx = 0; idx < NUM_ENGINES; idx++) {
		getRTStreamId(szStreamId[idx], STRING_LENGTH_MED, STREAM_VStr_vCoolantTemp, szEngines[idx]);
		GetSimulatedRTValues(&(szTimeCoolant[0][0]), STRING_LENGTH_MED, dValCoolant, NUM_COOLANT_VALUES, (rand() % 50) + 100);
		jn[idx] = objArrNumVal(szStreamId[idx], KEY_IndexedDateTime, KEY_vCoolantTemp,
			&(szTimeCoolant[0][0]), STRING_LENGTH_MED, dValCoolant, NUM_COOLANT_VALUES);
	}

	return combineJObjs(jn, NUM_ENGINES);
}

char* getValues_EngineRPMData()
{
#define NUM_RPM_VALUES  5
	int idx;
	JsonNode* jn[NUM_ENGINES];

	char szStreamId[NUM_ENGINES][STRING_LENGTH_MED];

	char szTimeRPM[NUM_RPM_VALUES][STRING_LENGTH_MED];
	double dValRPM[NUM_RPM_VALUES];

	char szEngines[NUM_ENGINES][STRING_LENGTH_MED];
	GetEngineNames(&(szEngines[0][0]), NUM_ENGINES, STRING_LENGTH_MED);

	for (idx = 0; idx < NUM_ENGINES; idx++) {
		getRTStreamId(szStreamId[idx], STRING_LENGTH_MED, STREAM_VStr_vRPM, szEngines[idx]);
		GetSimulatedRTValues(&(szTimeRPM[0][0]), STRING_LENGTH_MED, dValRPM, NUM_RPM_VALUES, (rand() % 50) + 2000);
		jn[idx] = objArrNumVal(szStreamId[idx], KEY_IndexedDateTime, KEY_vRPM, &(szTimeRPM[0][0]), STRING_LENGTH_MED,
			dValRPM, NUM_RPM_VALUES);
	}

	return combineJObjs(jn, NUM_ENGINES);
}

char* getValues_VehicleData()
{
#define NUM_VEHICLE_VALUES  5
#define NUM_VEHICLE_MEAS  2

	int idx;
	JsonNode* jn[NUM_VEHICLES];

	char szStreamId[NUM_VEHICLES][STRING_LENGTH_MED];

	char szTime[NUM_VEHICLE_VALUES][STRING_LENGTH_MED];
	double dVehicleMeas[NUM_VEHICLE_MEAS][NUM_VEHICLE_VALUES];
	char szTag[NUM_VEHICLE_MEAS][STRING_LENGTH_MED];

	char szVehicles[NUM_VEHICLES][STRING_LENGTH_MED];
	GetVehicleNames(&(szVehicles[0][0]), NUM_VEHICLES, STRING_LENGTH_MED);

	snprintf(szTag[0], STRING_LENGTH_MED, "%s", KEY_vSpeed);
	snprintf(szTag[1], STRING_LENGTH_MED, "%s", KEY_vPressure);

	for (idx = 0; idx < NUM_VEHICLES; idx++) {
		getRTStreamId(szStreamId[idx], STRING_LENGTH_MED, STREAM_VStr_vVehicleMeas, szVehicles[idx]);
		GetSimulatedRTData(&(szTime[0][0]), STRING_LENGTH_MED, &(dVehicleMeas[0][0]), NUM_VEHICLE_VALUES, NUM_VEHICLE_MEAS, 
			(rand() % 80) + 100);
		jn[idx] = objArrNumVals(szStreamId[idx], KEY_IndexedDateTime, &(szTime[0][0]), STRING_LENGTH_MED, NUM_VEHICLE_VALUES, &(szTag[0][0]),
			STRING_LENGTH_MED, NUM_VEHICLE_MEAS, &(dVehicleMeas[0][0]));
	}
	return (combineJObjs(jn, NUM_VEHICLES));
}

char* getOneEngineValue(int code, const char* szEngineName)
{
	char* szRet = NULL;

	char szStreamId[STREAM_ID_LEN];

	/* get a value for the RPM */
	if (RTVAL_ENGINE_RPM == code) {		
		getRTStreamId(szStreamId, STREAM_ID_LEN, STREAM_VStr_vRPM, szEngineName);
		szRet = sendOneVal(szStreamId, KEY_IndexedDateTime, get_currtime(), KEY_vRPM, 5000 + rand() % 100);
	}

	/* get a value for the CoolantTemp */
	if (RTVAL_ENGINE_COOLANTTEMP == code) {		
		getRTStreamId(szStreamId, STREAM_ID_LEN, STREAM_VStr_vCoolantTemp, szEngineName);
		szRet = sendOneVal(szStreamId, KEY_IndexedDateTime, get_currtime(), KEY_vCoolantTemp, 200 + rand() % 80);
	}

	return szRet;
}
