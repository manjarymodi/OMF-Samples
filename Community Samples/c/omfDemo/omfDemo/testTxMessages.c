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

/* testTxMessages.c */

#include <stdio.h>   /* for puts() */
#include <stdlib.h>   /* for free() */

#include "baseBuild.h"
#include "buildTypes.h"
#include "buildStreams.h"
#include "buildObjects.h"
#include "buildData.h"

#include "getValuesAndStreams.h"
#include "main.h"

extern struct Vehicle g_theVehicles[];
extern struct Engine g_theEngines[];
extern struct Transmission g_theTransmissions[];

extern char g_validVehStatus[NUM_VALID_VEHSTATUS][STRING_LENGTH_MED];
extern double g_vehicleFloat[NUM_VEHICLE_FLOAT];
extern int g_vehicleInt[NUM_VEHICLE_INT];

void showJson(JsonNode* theJson, const char* szMsg)
{
	char* sz = json_stringify(theJson, "  ");

	logMsg(szMsg);
	
	puts(sz);
	puts("\n");
	free(sz);

	json_delete(theJson);
}

void test_ShowTypes()
{
	char* sz = defineTypes();
	JsonNode* theJson = json_decode( sz );
	showJson(theJson, "messagetype: Type>");
	free(sz);
}

void test_ShowValueStreams()
{
	char* sz;
	JsonNode* theJson;

	sz = getStreams_RTValues(RTVAL_ENGINE_RPM);
	theJson = json_decode(sz);
	showJson(theJson, "messagetype: Container (real-time values, Engine RPM)>");
	free(sz);

	sz = getStreams_RTValues(RTVAL_ENGINE_COOLANTTEMP);
	theJson = json_decode(sz);
	showJson(theJson, "messagetype: Container (real-time values, Engine Coolant Temp)>");
	free(sz);

	sz = getStreams_RTValues(RTVAL_VEHICLE_MEAS);
	theJson = json_decode(sz);
	showJson(theJson, "messagetype: Container (real-time values, Vehicle Meas)>");
	free(sz);

	sz = getStreams_RTValues(RTVAL_VEHICLE_STATUS);
	theJson = json_decode(sz);
	showJson(theJson, "messagetype: Container (real-time values, Vehicle Status)>");
	free(sz);

	sz = getStreams_RTValues(RTVAL_VEHICLE_FLOAT);
	theJson = json_decode(sz);
	showJson(theJson, "messagetype: Container (real-time values, Vehicle Float)>");
	free(sz);

	sz = getStreams_RTValues(RTVAL_VEHICLE_INT);
	theJson = json_decode(sz);
	showJson(theJson, "messagetype: Container (real-time values, Vehicle Int)>");
	free(sz);
}

void test_ShowLinks()
{
	char* sz;
	JsonNode* theJson;

	sz = getAssets_Value_Links(RTVAL_ENGINE_RPM);
	theJson = json_decode(sz);
	showJson(theJson, "messagetype: Data (real-time values link, Engine RPM)>");
	free(sz);

	sz = getAssets_Value_Links(RTVAL_ENGINE_COOLANTTEMP);
	theJson = json_decode(sz);
	showJson(theJson, "messagetype: Data (real-time values link, Engine Coolant Temp)>");
	free(sz);

	sz = getAssets_Value_Links(RTVAL_VEHICLE_MEAS);
	theJson = json_decode(sz);
	showJson(theJson, "messagetype: Data (real-time values link, Vehicle Meas)>");
	free(sz);

	sz = getAssets_Value_Links(RTVAL_VEHICLE_STATUS);
	theJson = json_decode(sz);
	showJson(theJson, "messagetype: Data (real-time values link, Vehicle Status)>");
	free(sz);

	sz = getAssets_Value_Links(RTVAL_VEHICLE_FLOAT);
	theJson = json_decode(sz);
	showJson(theJson, "messagetype: Data (real-time values link, Vehicle Float)>");
	free(sz);

	sz = getAssets_Value_Links(RTVAL_VEHICLE_INT);
	theJson = json_decode(sz);
	showJson(theJson, "messagetype: Data (real-time values link, Vehicle Int)>");
	free(sz);
}

void test_ShowVehicleAssetObjects(const struct Vehicle* pVehicles, int nVehicles)
{
	char* sz = defineVehicleAssets(pVehicles, nVehicles);
	JsonNode* theJson = json_decode(sz);
	showJson(theJson, "messagetype: Data (Vehicle assets)>");
	free(sz);
}

void test_ShowEngineAssetObjects(const struct Engine* pEngines, int nEngines)
{
	char* sz = defineEngineAssets(pEngines, nEngines);
	JsonNode* theJson = json_decode(sz);
	showJson(theJson, "messagetype: Data (Engine assets)>");
	free(sz);
}

void test_ShowTransmissionAssetValues(const struct Transmission* pTransmissions, int nTransmissions)
{
	char* sz = defineTransmissionAssets(pTransmissions, nTransmissions);
	JsonNode* theJson = json_decode(sz);
	showJson(theJson, "messagetype: Data (Transmission assets)>");
	free(sz);
}

void test_ShowAssetLinks()
{
	char* sz;
	JsonNode* theJson;

	sz = getAssets_Asset_links(ASSET_LINK_VEHICLE_VEHICLE);
	theJson = json_decode(sz);
	showJson(theJson, "messagetype: Data (links between a vehicle and root)>");
	free(sz);

	sz = getAssets_Asset_links(ASSET_LINK_VEHICLE_ENGINE);
	theJson = json_decode(sz);
	showJson(theJson, "messagetype: Data (links between a vehicle and an engine)>");
	free(sz);

	sz = getAssets_Asset_links(ASSET_LINK_VEHICLE_TRANSMISSION);
	theJson = json_decode(sz);
	showJson(theJson, "messagetype: Data (links between a vehicle and a transmission)>");
	free(sz);
}

void test_GenerateEngineData()
{
	char* sz;
	JsonNode* theJson;

	sz = getValues_EngineRPMData();
	theJson = json_decode( sz );
	showJson(theJson, "messagetype: Data (engine RPM data)>");
	free(sz);

	sz = getValues_EngineCoolantTempData();
	theJson = json_decode(sz);
	showJson(theJson, "messagetype: Data (engine CoolantTemp data)>");
	free(sz);
}

void test_GenerateVehicleData()
{
	char* sz = getValues_VehicleData();
	JsonNode* theJson = json_decode( sz );
	showJson(theJson, "messagetype: Data (vehicle data)>");
	free(sz);
}

void test_GenerateSingleValue()
{
	char* sz;
	JsonNode* theJson;
	char szStreamId[STREAM_ID_LEN];

	sz = getOneEngineValue(RTVAL_ENGINE_RPM, g_theEngines[0].szName);
	theJson = json_decode(sz);
	showJson(theJson, "messagetype: Data (one engine RPM value)>");
	free(sz);

	sz = getOneEngineValue(RTVAL_ENGINE_COOLANTTEMP, g_theEngines[1].szName);
	theJson = json_decode(sz);
	showJson(theJson, "messagetype: Data (one engine CoolantTemp value)>");
	free(sz);

	getRTStreamId(szStreamId, STREAM_ID_LEN, STREAM_VStr_vVehicleStatus, g_theVehicles[0].szName);
	sz = sendOneValStr(szStreamId, KEY_IndexedDateTime, get_currtime(), KEY_vVehicleStatus, g_validVehStatus[0]);
	theJson = json_decode(sz);
	showJson(theJson, "messagetype: Data (one VehicleStatus value)>");
	free(sz);

	getRTStreamId(szStreamId, STREAM_ID_LEN, STREAM_VStr_vVehicleStatus, g_theVehicles[1].szName);
	sz = sendOneValStr(szStreamId, KEY_IndexedDateTime, get_currtime(), KEY_vVehicleStatus, g_validVehStatus[1]);
	theJson = json_decode(sz);
	showJson(theJson, "messagetype: Data (one VehicleStatus value)>");
	free(sz);

	getRTStreamId(szStreamId, STREAM_ID_LEN, STREAM_VStr_vVehicleStatus, g_theVehicles[2].szName);
	sz = sendOneValStr(szStreamId, KEY_IndexedDateTime, get_currtime(), KEY_vVehicleStatus, g_validVehStatus[2]);
	theJson = json_decode(sz);
	showJson(theJson, "messagetype: Data (one VehicleStatus value)>");
	free(sz);

	getRTStreamId(szStreamId, STREAM_ID_LEN, STREAM_VStr_vVehicleFloat, g_theVehicles[0].szName);
	sz = sendOneVal(szStreamId, KEY_IndexedDateTime, get_currtime(), KEY_vVehicleFloat, g_vehicleFloat[3]);
	theJson = json_decode(sz);
	showJson(theJson, "messagetype: Data (one VehicleFloat value)>");
	free(sz);

	getRTStreamId(szStreamId, STREAM_ID_LEN, STREAM_VStr_vVehicleFloat, g_theVehicles[1].szName);
	sz = sendOneVal(szStreamId, KEY_IndexedDateTime, get_currtime(), KEY_vVehicleFloat, g_vehicleFloat[2]);
	theJson = json_decode(sz);
	showJson(theJson, "messagetype: Data (one VehicleFloat value)>");
	free(sz);

	getRTStreamId(szStreamId, STREAM_ID_LEN, STREAM_VStr_vVehicleFloat, g_theVehicles[2].szName);
	sz = sendOneVal(szStreamId, KEY_IndexedDateTime, get_currtime(), KEY_vVehicleFloat, g_vehicleFloat[1]);
	theJson = json_decode(sz);
	showJson(theJson, "messagetype: Data (one VehicleFloat value)>");
	free(sz);

	getRTStreamId(szStreamId, STREAM_ID_LEN, STREAM_VStr_vVehicleInt, g_theVehicles[0].szName);
	sz = sendOneVal(szStreamId, KEY_IndexedDateTime, get_currtime(), KEY_vVehicleInt, g_vehicleInt[0]);
	theJson = json_decode(sz);
	showJson(theJson, "messagetype: Data (one VehicleInt value)>");
	free(sz);

	getRTStreamId(szStreamId, STREAM_ID_LEN, STREAM_VStr_vVehicleInt, g_theVehicles[1].szName);
	sz = sendOneVal(szStreamId, KEY_IndexedDateTime, get_currtime(), KEY_vVehicleInt, g_vehicleInt[1]);
	theJson = json_decode(sz);
	showJson(theJson, "messagetype: Data (one VehicleInt value)>");
	free(sz);

	getRTStreamId(szStreamId, STREAM_ID_LEN, STREAM_VStr_vVehicleInt, g_theVehicles[2].szName);
	sz = sendOneVal(szStreamId, KEY_IndexedDateTime, get_currtime(), KEY_vVehicleInt, g_vehicleInt[2]);
	theJson = json_decode(sz);
	showJson(theJson, "messagetype: Data (one VehicleInt value)>");
	free(sz);

}

void runTests()
{
	test_ShowTypes();

	test_ShowValueStreams();

	test_ShowVehicleAssetObjects( g_theVehicles, NUM_VEHICLES );
	test_ShowEngineAssetObjects( g_theEngines, NUM_ENGINES);
	test_ShowTransmissionAssetValues( g_theTransmissions, NUM_TRANSMISSIONS );

	test_ShowAssetLinks();
	test_ShowLinks();	

	test_GenerateSingleValue();
	test_GenerateEngineData();
	test_GenerateVehicleData();
}
