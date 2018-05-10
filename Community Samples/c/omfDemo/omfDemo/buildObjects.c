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

/* buildObjects.c */

#include "baseBuild.h"
#include "buildStreams.h"
#include "buildObjects.h"
#include "buildTypes.h"

#include "deviceSimulator.h"

/*
  {
	  "VehicleName": <p->szName>,
	  "VIN" : <p->szVIN>,
	  "Year" : <p->year>,
	  "Make" : <p->szMake>,
	  "Model" : <p->szModel>,
	  "InServiceDate" : <pVehicle->szInServiceDate>
  }

This function builds a JSON object that represents a Vehicle.
*/
JsonNode* objVehicle(const struct Vehicle* p)
{
	JsonNode* ret = json_mkobject();
	json_append_member(ret, KEY_VehicleName, json_mkstring(p->szName));
	json_append_member(ret, KEY_VIN, json_mkstring(p->szVIN));
	json_append_member(ret, KEY_Year, json_mknumber(p->year));
	json_append_member(ret, KEY_Make, json_mkstring(p->szMake));
	json_append_member(ret, KEY_Model, json_mkstring(p->szModel));
	json_append_member(ret, KEY_InServiceDate, json_mkstring(p->szInServiceDate));

	return ret;
}

/*
{
	"TransmissionName": <p->szName>,
	"SerialNumber" : <p->szSerialNumber>,
	"Manufacturer" : <p->szManufacturer>,
	"InServiceDate" : <p->szInServiceDate>
}
This function builds a JSON object that represents a Transmission.
*/
JsonNode* objTransmission(const struct Transmission* p)
{
	JsonNode* ret = json_mkobject();
	json_append_member(ret, KEY_TransmissionName, json_mkstring(p->szName));
	json_append_member(ret, KEY_SerialNumber, json_mkstring(p->szSerialNumber));
	json_append_member(ret, KEY_Manufacturer, json_mkstring(p->szManufacturer));
	json_append_member(ret, KEY_InServiceDate, json_mkstring(p->szInServiceDate));

	return ret;
}

/*
{
	"EngineName": <p->szName>,
	"SerialNumber" : <p->szSerialNumber>,
	"Cylinders" : <p->cylinders>,
	"Manufacturer" : <p->szManufacturer>,
	"InServiceDate" : <p->szInServiceDate>
}
This function builds a JSON object that represents an Engine.
*/
JsonNode* objEngine(const struct Engine* p)
{
	JsonNode* ret = json_mkobject();
	json_append_member(ret, KEY_EngineName, json_mkstring(p->szName));
	json_append_member(ret, KEY_SerialNumber, json_mkstring(p->szSerialNumber));
	json_append_member(ret, KEY_Cylinders, json_mknumber(p->cylinders));
	json_append_member(ret, KEY_Manufacturer, json_mkstring(p->szManufacturer));
	json_append_member(ret, KEY_InServiceDate, json_mkstring(p->szInServiceDate));

	return ret;
}

/*
{
	"type": "VehicleType",
	"values" : [
	{
		"VehicleName": "Vehicle_1",
		"VIN" : "VIN_1",
		"Year" : 2000,
		"Make" : "Make_1",
		"Model" : "Model_1",
		"InServiceDate" : "2016-04-19T20:19:16Z"
	},
	{
		"VehicleName": "Vehicle_2",
		"VIN" : "VIN_2",
		"Year" : 2001,
		"Make" : "Make_2",
		"Model" : "Model_2",
		"InServiceDate" : "2016-04-19T20:19:16Z"
	}
	]
}
This function build a string that contains Vehicle assets as an array of values.
We use this string when we POST an Object messagtype.
*/
char* defineVehicleAssets(
	const struct Vehicle* pVehicles,  /* an array of Vehicles */
	int numVehicles   /* number of Vehicles */
	)
{
	char* szRet;
	int idx;
	JsonNode* arrayRet = json_mkarray();

	JsonNode* obj = json_mkobject();
	JsonNode* theArray = json_mkarray();
	for (idx = 0; idx < numVehicles; idx++) {
		json_append_element(theArray, objVehicle(&(pVehicles[idx])));
	}

	json_append_member(obj, LIT_typeid, json_mkstring(TYPE_VehicleType));
	json_append_member(obj, LIT_values, theArray);

	json_append_element(arrayRet, obj);
	szRet = json_encode(arrayRet);
	json_delete(theArray);
	json_delete(arrayRet);

	return szRet;
}

/*
[
{
	"type": "EngineType",
	"values" : [
	{
		"EngineName": "Engine_1",
		"SerialNumber" : "SerialNumber_1",
		"Cylinders" : 2,
		"Manufacturer" : "Manufacturer_1",
		"InServiceDate" : "2016-04-19T20:19:16Z"
	},
	{
		"EngineName": "Engine_2",
		"SerialNumber" : "SerialNumber_2",
		"Cylinders" : 3,
		"Manufacturer" : "Manufacturer_2",
		"InServiceDate" : "2016-04-19T20:19:16Z"
	}
	]
}
]
This function build a string that contains Engine assets as an array of values.
We use this string when we POST an Object messagtype.
*/
char* defineEngineAssets(
	const struct Engine* pEngines,  /* an array of Engines */
	int numEngines   /* number of Engines */
	)
{
	char* szRet;
	int idx;
	JsonNode* arrayRet = json_mkarray();

	JsonNode* obj = json_mkobject();
	JsonNode* theArray = json_mkarray();
	for (idx = 0; idx < numEngines; idx++) {
		json_append_element(theArray, objEngine(&(pEngines[idx])));
	}

	json_append_member(obj, LIT_typeid, json_mkstring(TYPE_EngineType));
	json_append_member(obj, LIT_values, theArray);

	json_append_element(arrayRet, obj);
	szRet = json_encode(arrayRet);
	json_delete(theArray);
	json_delete(arrayRet);

	return szRet;
}

/*
[
{
	"type": "Transmission",
	"values" : [
	{
		"TransmissionName": "Transmission_1",
		"SerialNumber" : "T_SerialNumber_1",
		"Manufacturer" : "T_Manufacturer_1",
		"InServiceDate" : "2016-04-19T20:19:16Z"
	},
	{
		"TransmissionName": "Transmission_2",
		"SerialNumber" : "T_SerialNumber_2",
		"Manufacturer" : "T_Manufacturer_2",
		"InServiceDate" : "2016-04-19T20:19:16Z"
	}
	]
}
]
This function build a string that contains Transmission assets as an array of values.
We use this string when we POST an Object messagetype.
*/
char* defineTransmissionAssets(
	const struct Transmission* pTransmissions,  /* an array of Transmissions */
	int numTransmissions   /* number of Transmissions */
	)
{
	char* szRet;
	int idx;
	JsonNode* arrayRet = json_mkarray();

	JsonNode* obj = json_mkobject();
	JsonNode* theArray = json_mkarray();
	for (idx = 0; idx < numTransmissions; idx++) {
		json_append_element(theArray, objTransmission(&(pTransmissions[idx])));
	}

	json_append_member(obj, LIT_typeid, json_mkstring(TYPE_TransmissionType));
	json_append_member(obj, LIT_values, theArray);

	json_append_element(arrayRet, obj);
	szRet = json_encode(arrayRet);
	json_delete(theArray);
	json_delete(arrayRet);

	return szRet;
}

/*
  Creates an JSON object which can be used to specify the source or target of a
  link using the given containerid.
  Result is something like the json object:
      {"containerid": container_id}
*/
JsonNode* container_link_node_info(const char * container_id)
{
	JsonNode* node_info = json_mkobject();
	json_append_member(node_info, LIT_containerid, json_mkstring(container_id));
	return node_info;
}

/* 
  Creates a JSON object which encodes information about the node which is
  either source or target of a link, and which is not a container element (e.g.
  a static element).
  type_version is nullable.
*/
JsonNode* noncontainer_link_node_info(const char * typeid, const char * type_version, const char * index)
{
	JsonNode* node_info = json_mkobject();
	json_append_member(node_info, LIT_typeid, json_mkstring(typeid));
	if (NULL != type_version)
	{
			json_append_member(node_info, LIT_typeversion, json_mkstring(type_version));
	}
	json_append_member(node_info, LIT_index, json_mkstring(index));
	return node_info;
}

/*
    Construct a JSON object encoding a link from a static element to another
	static element.
	source_type_version and target_type_version are nullable.
*/
JsonNode* static_static_link(
	const char * source_index,
	const char * source_type,
	const char * source_type_version,
	const char * target_index,
	const char * target_type,
	const char * target_type_version)
{
	JsonNode* link = json_mkobject();
	json_append_member(link,
		LIT_Source,
		noncontainer_link_node_info(source_type, source_type_version, source_index));
	json_append_member(
		link,
		LIT_Target,
		noncontainer_link_node_info(target_type, target_type_version, target_index));
	return link;
}

JsonNode* static_container_link(
	const char * source_index,
	const char * source_type,
	const char * source_type_version,
	const char * target_container_index)
{
	JsonNode* link = json_mkobject();
	json_append_member(link,
		LIT_Source,
		noncontainer_link_node_info(source_type, source_type_version, source_index));
	json_append_member(link,
		LIT_Target,
		container_link_node_info(target_container_index));
	return link;
}

/*
[
{
	"Type": "Vehicle_Engine_Link",
	"values" : [
	{
		"Source": "Vehicle_1",
		"Target" : "Engine_1"
	},
	{
		"Source": "Vehicle_2",
		"Target" : "Engine_2"
	},
	{
		"Source": "Vehicle_3",
		"Target" : "Engine_3"
	}
	]
}
]
This funtion builds a string that contains a JSON array of source/target links.
We use this string when we POST an Object messagetype.
*/
char* defineLink_Sources_Targets(
	const char* szSource,    /* array of Source names */
	const char* sources_type, /* all sources must be the same type */
	const char* sources_type_version, /* safely nullable */
	const char* szTarget,    /* array of Target names */
	const char* targets_type, /* all targets must be the same type */
	const char* targets_type_version, /* safely nullable */
	const int nStrings,    /* size of the array */
	const int sLength   /* string length of Source and Target names */
	)
{
	char* szRet;
	int idx;
	JsonNode* arrayRet = json_mkarray();

	JsonNode* obj = json_mkobject();
	JsonNode* anArray = json_mkarray();

	for (idx = 0; idx < nStrings; idx++)
	{
		const char* source_name;
		const char* target_name = szTarget + idx*sLength;
		if (szSource) {
			source_name = szSource + idx*sLength;
		}
		else {
			source_name = "_ROOT";
		}
		JsonNode* link = static_static_link(source_name, sources_type, sources_type_version, szTarget + idx*sLength, targets_type, targets_type_version);

		json_append_element(anArray, link);
	}

	json_append_member(obj, LIT_typeid, json_mkstring(TYPE_link));
	json_append_member(obj, LIT_values, anArray);

	json_append_element(arrayRet, obj);
	szRet = json_encode(arrayRet);
	json_delete(anArray);
	json_delete(arrayRet);

	return szRet;
}

/*
Build a JSON object such as
{
   "type" : <type>,     such as "Engine_RPM_Link"
   "values" :
   [
      {
         "Source": <source>,    such as "Engine_1"
         "Target": <target>     such as "VStr_vRPM_Engine_1
      }
   ]
}
This object defines a link stream value between a real-time value stream (e.g., RPM) to an asset (e.g., Engine)
*/
JsonNode* objRTVal_Link(const char* type, const char* source, const char* target)
{
	JsonNode* ret = json_mkobject();

	JsonNode* theArray = json_mkarray();
	JsonNode* st = json_mkobject();
	json_append_member(st, LIT_Source, json_mkstring(source));
	json_append_member(st, LIT_Target, json_mkstring(target));
	json_append_element(theArray, st);

	json_append_member(ret, LIT_typeid, json_mkstring(type));
	json_append_member(ret, LIT_values, theArray);

	return ret;
}

/*
[
{
	"type": "Engine_RPM_Link",
	"values" : [
		{
			"Source": "Engine_1",
			"Target" : "VStr_vRPM_Engine_1"
		}
	]
},
{
	"type": "Engine_RPM_Link",
	"values" : [
		{
			"Source": "Engine_2",
			"Target" : "VStr_vRPM_Engine_2"
		}
	]
},
{
	"type": "Engine_RPM_Link",
	"values" : [
		{
			"Source": "Engine_3",
			"Target" : "VStr_vRPM_Engine_3"
		}
	]
}
]
This function generates a string such as the preceding.
*/
char* val_Link_Asset_RT(
	const char* source_type,
	const char* source_typeversion,
	const char* containerid_prefix, /* a value such as "VStr_vRPM") */
	char* source_indexes, /* array of AF element names; e.g., Engine_1, Engine_2, Engine_3 */
	const int nElements,  /* number of elements */
	const int nStringLength  /* string length of the name of AF elements */
	)
{
	const char * const type = "__Link";
	char* szRet;
	int idx;
	JsonNode* arrStreams = json_mkarray();

	for (idx = 0; idx < nElements; idx++) {
		char containerid[STREAM_ID_LEN];
		getRTStreamId(containerid, STREAM_ID_LEN, containerid_prefix, source_indexes + idx*nStringLength);

		json_append_element(arrStreams,
			static_container_link(source_indexes + idx*nStringLength, source_type, source_typeversion, containerid));
	}

	JsonNode* links_object = json_mkobject();
	json_append_member(links_object, LIT_typeid, json_mkstring(TYPE_link));
	json_append_member(links_object, LIT_values, arrStreams);

	JsonNode * message_array = json_mkarray();
	json_append_element(message_array, links_object);

	szRet = json_encode(message_array);
	json_delete(message_array);

	return szRet;
}

