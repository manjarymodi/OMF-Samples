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

/* buildTypes.c */

/*
The functions in this file define JSON messages that are messagetype Type.
*/

#include <stdlib.h>
#include <stdio.h>

#include "baseBuild.h"
#include "buildTypes.h"

#include "deviceSimulator.h"

char g_validVehStatus[NUM_VALID_VEHSTATUS][STRING_LENGTH_MED] = { "Parked", "Stopped", "Running" };
double g_vehicleFloat[NUM_VEHICLE_FLOAT] = { 5.45, 7.77, 19.96, 201.6 };
int g_vehicleInt[NUM_VEHICLE_INT] = { 545, 777, 1980, 1996, 2016 };

extern char g_validMakes[NUM_VALID_MAKES][STRING_LENGTH_MED];
extern int g_nCylinders[];
extern int g_nEnumCylinders;

/*
This function generates the following JSON object:
   { 
      "type": "string",
	  "format": "date-time"
   }
*/
JsonNode* oStringDateTime()
{
	JsonNode* ret = json_mkobject();

	json_append_member(ret, KEY_type, json_mkstring(TYPE_string));
	json_append_member(ret, KEY_format, json_mkstring(FORMAT_date_time));

	return ret;
}

/*
This function generates the following JSON object:
   {
      "type": "string",
      "index": <true> or <false>
   }
*/
JsonNode* oStringBool(bool b)
{
	JsonNode* ret = json_mkobject();

	json_append_member(ret, KEY_type, json_mkstring(TYPE_string));
	json_append_member(ret, KEY_isindex, json_mkbool(b));

	return ret;
}

JsonNode* idxDT()
{
	JsonNode* idxDTP = json_mkobject();

	json_append_member(idxDTP, KEY_type, json_mkstring(TYPE_string));
	json_append_member(idxDTP, KEY_format, json_mkstring(FORMAT_date_time));
	json_append_member(idxDTP, KEY_isindex, json_mkbool(true));

	return idxDTP;
}

JsonNode* typeRTVal_Common(char* id, JsonNode* objProperties)
{
	JsonNode* ret = json_mkobject();

	json_append_member(ret, KEY_id, json_mkstring(id));
	json_append_member(ret, KEY_description, json_mkstring("Timestamp and real-time value(s)"));
	json_append_member(ret, KEY_type, json_mkstring(TYPE_object));
	json_append_member(ret, KEY_classification, json_mkstring(CLASSIFICATION_dynamic));
	json_append_member(ret, KEY_properties, objProperties);

	return ret;
}

/*
{
   "id": <id>,
   "description": "Number and  timestamp",
   "type": "object",
   "classification: "stream",
   "properties":
   {
      "IndexedDateTime": {"type":"string","format":"date-time","index":true},
      <szRtTagNames[0]>: {"type":"number"}
	  <szRtTagNames[1]>: {"type":"number"}
	  ...
	  <szRtTagNames[ntags-1]>: {"type":"number"}
   }
}

This function creates a JSON object as indicated above. We use this function to 
define a real-time stream.
*/
JsonNode* typeRTVals(char* id, char* szRtTagNames, int sLength, int nTags)
{
	int idx;
	JsonNode* objProperties = json_mkobject();

	json_append_member(objProperties, KEY_IndexedDateTime, idxDT());
	for (idx = 0; idx < nTags; idx++) {
		json_append_member(objProperties, szRtTagNames + idx*sLength, oCharChar(KEY_type, TYPE_number));
	}

	return typeRTVal_Common(id, objProperties);
}

/*
This function defines a real-time stream that is a string enumeration. For example,
{
	"id": "vVehicleStatusType",
	"description": "Timestamp and real-time value(s)",
	"type": "object",
	"classification": "stream",
	"properties": {
		"IndexedDateTime": {
			"type": "string",
			"format": "date-time",
			"index": true
		},
		"vVehicleStatus": {
			"type": "string",
			"enum": [
				"Parked",
				"Stopped",
				"Running"
			]
		}
	}
}
*/
JsonNode* typeRTVal_EnumString(char* id, char* szTag, char* szEnums, int sLength, int enumLength)
{
	JsonNode* objProperties = json_mkobject();

	json_append_member(objProperties, KEY_IndexedDateTime, idxDT());
	json_append_member(objProperties, szTag, oCharChar(KEY_type, TYPE_string));

	return typeRTVal_Common(id, objProperties);
}

JsonNode* typeRTVal_EnumFloat(char* id, char* szTag, double* fEnums, int enumLength)
{
	JsonNode* objProperties = json_mkobject();

	json_append_member(objProperties, KEY_IndexedDateTime, idxDT());
	json_append_member(objProperties, szTag, oCharChar(KEY_type, TYPE_number));

	return typeRTVal_Common(id, objProperties);
}

JsonNode* typeRTVal_EnumInt(char* id, char* szTag, int* iEnums, int enumLength)
{
	JsonNode* objProperties = json_mkobject();

	json_append_member(objProperties, KEY_IndexedDateTime, idxDT());
	json_append_member(objProperties, szTag, oCharChar(KEY_type, TYPE_integer));

	return typeRTVal_Common(id, objProperties);
}

/*
{
   "id": "VehicleType",
   "description": "This is a vehicle such as a truck or boat",
   "type": "object",
   "classification": "asset",
   "properties":
   {
      "VehicleName": { "type": "string", "index": true },
      "VIN": { "type": "string" },
      "Year": { "type": "integer" },
      "Make": { 
	     "type": "string" 
		 "enum": ["Audi", "BMW", "Mercedes-Benz"]
	  },
      "Model": { "type": "string" },
      "InServiceDate": { "type": "string", "format": "date-time" }
   }
}

This function creates a JSON object for the "properties" as indicated above.
We use this function to define a Vehicle asset type.
*/
JsonNode* genVehicle_Properties()
{
	JsonNode* ret = json_mkobject();

	json_append_member(ret, KEY_VehicleName, oStringBool(true));
	json_append_member(ret, KEY_VIN, oCharChar(KEY_type, TYPE_string));
	json_append_member(ret, KEY_Year, oCharChar(KEY_type, TYPE_integer));
	json_append_member(ret, KEY_Make, oCharChar(KEY_type, TYPE_string));
	json_append_member(ret, KEY_Model, oCharChar(KEY_type, TYPE_string));
	json_append_member(ret, KEY_InServiceDate, oStringDateTime());

	return ret;
}

/*
{
   "id": "EngineType",
   "description": "This is an engine",
   "type": "object",
   "classification": "asset",
   "properties":
   {
      "EngineName": { "type": "string", "index": true },
      "SerialNumber": { "type": "string" },
      "Manufacturer": { "type": "string" },
      "Cylinders": { 
	     "type": "integer",
		 "enum": [4, 5, 6, 8, 10, 12] },
      "InServiceDate": { "type": "string", "format": "date-time" }
   }
}

This function creates a JSON object for the "properties" as indicated above.
We use this function to define an Engine asset type.
*/
JsonNode* genEngine_Properties()
{
	JsonNode* ret = json_mkobject();

	json_append_member(ret, KEY_EngineName, oStringBool(true));
	json_append_member(ret, KEY_SerialNumber, oCharChar(KEY_type, TYPE_string));
	json_append_member(ret, KEY_Manufacturer, oCharChar(KEY_type, TYPE_string));
	json_append_member(ret, KEY_Cylinders, oCharChar(KEY_type, TYPE_integer));
	json_append_member(ret, KEY_InServiceDate, oStringDateTime());

	return ret;
}

/*
{
   "id": "TransmissionType",
   "description": "This is a transmission",
   "type": "object",
   "classification": "asset",
   "properties":
   {
      "TransmissionName": { "type": "string", "index": true },
      "SerialNumber": { "type": "string" },
      "Manufacturer": { "type": "string" },
      "InServiceDate": { "type": "string", "format": "date-time" }
   }
}

This function creates a JSON object for the "properties" as indicated above.
We use this function to define a Transmision asset type.
*/
JsonNode* genTransmission_Properties()
{
	JsonNode* ret = json_mkobject();

	json_append_member(ret, KEY_TransmissionName, oStringBool(true));
	json_append_member(ret, KEY_SerialNumber, oCharChar(KEY_type, TYPE_string));
	json_append_member(ret, KEY_Manufacturer, oCharChar(KEY_type, TYPE_string));
	json_append_member(ret, KEY_InServiceDate, oStringDateTime());

	return ret;
}

/*
{
   "id": <szId>,
   "description": <szDescr>,
   "type": "object",
   "classification": <szClassification>,
   "properties": <properties>
}

This function creates a JSON object for static types.
*/
JsonNode* genType(char* szId, char* szDescr, char* szClassification, JsonNode* properties)
{
	JsonNode* ret = json_mkobject();
	json_append_member(ret, KEY_id, json_mkstring(szId));
	json_append_member(ret, KEY_description, json_mkstring(szDescr));
	json_append_member(ret, KEY_type, json_mkstring(TYPE_object));
	if (szClassification) {
		json_append_member(ret, KEY_classification, json_mkstring(szClassification));
	}
	json_append_member(ret, KEY_properties, properties);

	return ret;
}

/*
This function defines the Types that we will use. These types are classified as dynamic or static.
   dynamic -- a real-time measurement; e.g., PI tag
   static -- an AF element
*/
char* defineTypes()
{
#define NUM_TYPES 9
	JsonNode* jn[NUM_TYPES];

	/* define Types that describe the assets */
	jn[0] =	genType(TYPE_VehicleType, "Vehicle description", CLASSIFICATION_static, genVehicle_Properties());
	jn[1] =	genType(TYPE_EngineType, "Engine description", CLASSIFICATION_static, genEngine_Properties());
	jn[2] =	genType(TYPE_TransmissionType, "Transmission description", CLASSIFICATION_static, genTransmission_Properties());

	/* define Types that describe the real-time values */
	{
#define NUM_VEHICLE_MEAS 2
#define MEAS_LENGTH 80
		char szMeas[NUM_VEHICLE_MEAS][MEAS_LENGTH];

		snprintf(szMeas[0], MEAS_LENGTH, "%s", KEY_vRPM);
		jn[3] = typeRTVals(TYPE_Value_vRPM, &(szMeas[0][0]), MEAS_LENGTH, 1);

		snprintf(szMeas[0], MEAS_LENGTH, "%s", KEY_vCoolantTemp);
		jn[4] = typeRTVals(TYPE_Value_vCoolantTemp, &(szMeas[0][0]), MEAS_LENGTH, 1);

		snprintf(szMeas[0], MEAS_LENGTH, "%s", KEY_vSpeed);
		snprintf(szMeas[1], MEAS_LENGTH, "%s", KEY_vPressure);
		jn[5] = typeRTVals(TYPE_Value_vVehicleMeas, &(szMeas[0][0]), MEAS_LENGTH, NUM_VEHICLE_MEAS);

		jn[6] = typeRTVal_EnumString(TYPE_Value_vVehicleStatus, KEY_vVehicleStatus, &(g_validVehStatus[0][0]), STRING_LENGTH_MED, NUM_VALID_VEHSTATUS);
		jn[7] = typeRTVal_EnumFloat(TYPE_Value_vVehicleFloat, KEY_vVehicleFloat, &(g_vehicleFloat[0]), NUM_VEHICLE_FLOAT);
		jn[8] = typeRTVal_EnumInt(TYPE_Value_vVehicleInt, KEY_vVehicleInt, &(g_vehicleInt[0]), NUM_VEHICLE_INT);
	}

	return (combineJObjs(jn, NUM_TYPES));
}
