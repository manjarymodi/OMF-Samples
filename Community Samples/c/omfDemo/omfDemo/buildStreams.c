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

/* buildStreams.c */

/*
The functions in this file define JSON messages that are messagetype Stream.
*/

#include "baseBuild.h"
#include "buildStreams.h"
#include "buildTypes.h"

/*
This function generates the following JSON object:
   {
      "id": <stream>,
      "type": <type>
   }
*/
JsonNode* oStream(const char* stream, const char* type)
{
	JsonNode* ret = json_mkobject();
	json_append_member(ret, KEY_id, json_mkstring(stream));
	json_append_member(ret, LIT_typeid, json_mkstring(type));
	
	return ret;
}

/*
{
	"id": "VStr_vRPM_Engine_1",
	"type" : "vRPMType"
},
{
	"id": "VStr_vRPM_Engine_2",
	"type" : "vRPMType"
},
{
	"id": "VStr_vRPM_Engine_3",
	"type" : "vRPMType"
}

This function defines streams related to a real-time value; these real-time values
will be linked to an asset.
*/
char* defineRTStreams(
	const char* szPrefix, /* a value such as "VStr_vRPM" */
	char* szElement, /* array of AF element names; e.g., Engine_1, Engine_2, Engine_3 */
	int nElements,  /* number of elements */
	int nStringLength,  /* string length of the name of AF elements */
	const char* type /* a value such as "vRPMType" */
	)
{
	char* szRet;

	int idx;
	JsonNode* arrStreams = json_mkarray();

	for (idx = 0; idx < nElements; idx++) {
		char szStreamId[STREAM_ID_LEN];

		getRTStreamId(szStreamId, STREAM_ID_LEN, szPrefix, szElement + idx*nStringLength);
		json_append_element(arrStreams, oStream(szStreamId, type));
	}

	szRet = json_encode(arrStreams);
	json_delete(arrStreams);

	return szRet;
}
