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

/* buildData.c */

#include "baseBuild.h"
#include "buildStreams.h"
#include "buildData.h"

/*
Build a JSON object that represents a single instance of a real-time value
{
"stream" : <stream>,
"values" : [
{
<dateTimeKey>: <szTime>,
<szRTVal>: <dVal>
}
]
}
*/
JsonNode* objRTVal(const char* stream, const char* dateTimeKey, const char* szTime, const char* szRTVal, double dVal)
{
	JsonNode* ret = json_mkobject();

	JsonNode* theArray = json_mkarray();
	JsonNode* timeAndVal = json_mkobject();
	json_append_member(timeAndVal, dateTimeKey, json_mkstring(szTime));
	json_append_member(timeAndVal, szRTVal, json_mknumber(dVal));
	json_append_element(theArray, timeAndVal);

	json_append_member(ret, LIT_containerid, json_mkstring(stream));
	json_append_member(ret, LIT_values, theArray);

	return ret;
}

/*
Build a JSON object that represents a single instance of a real-time value
{
"stream" : <stream>,
"values" : [
{
<dateTimeKey>: <szTime>,
<szRTVal>: <szVal>
}
]
}
*/
JsonNode* objRTValStr(const char* stream, const char* dateTimeKey, const char* szTime, const char* szRTVal, char* szVal)
{
	JsonNode* ret = json_mkobject();

	JsonNode* theArray = json_mkarray();
	JsonNode* timeAndVal = json_mkobject();
	json_append_member(timeAndVal, dateTimeKey, json_mkstring(szTime));
	json_append_member(timeAndVal, szRTVal, json_mkstring(szVal));
	json_append_element(theArray, timeAndVal);

	json_append_member(ret, LIT_containerid, json_mkstring(stream));
	json_append_member(ret, LIT_values, theArray);

	return ret;
}


/*
[
{
"stream": <valStream>,
"values" : [
{
<dateTimeKey>: <szTime>,
<vRTVal> : <dVal>
}
]
}
]
getRTStreamId(szStreamId, STREAM_ID_LEN, STREAM_VStr_vRPM, g_theEngines[0].szName);
sTX = sendOneVal(szStreamId, KEY_IndexedDateTime, get_currtime(), KEY_vRPM, 8.88);

This function builds a string that contains a JSON array of a single instance of
a real-time value
*/
char* sendOneVal(const char* valStream, const char* dateTimeKey, const char* szTime, char* vRTVal, double dVal)
{
	JsonNode* jn[1];

	jn[0] = objRTVal(valStream, dateTimeKey, szTime, vRTVal, dVal);

	return (combineJObjs(jn, 1));
}

/*
This function builds a string that contains a JSON array of a single instance of
a real - time string value
*/
char* sendOneValStr(const char* valStream, const char* dateTimeKey, const char* szTime, char* vRTVal, char* szVal)
{
	JsonNode* jn[1];

	jn[0] = objRTValStr(valStream, dateTimeKey, szTime, vRTVal, szVal);

	return (combineJObjs(jn, 1));
}


/*
{
"stream": "VStr_vRPM_Engine_1",
"values" : [
{
"IndexedDateTime": "2016-04-19T20:19:17.236Z",
"vRPM" : 1.23
},
{
"IndexedDateTime": "2016-04-19T20:19:18.236Z",
"vRPM" : 2.23
},
{
"IndexedDateTime": "2016-04-19T20:19:19.236Z",
"vRPM" : 3.23
},
{
"IndexedDateTime": "2016-04-19T20:19:20.236Z",
"vRPM" : 4.23
},
{
"IndexedDateTime": "2016-04-19T20:19:21.236Z",
"vRPM" : 5.23
}
]
}
This function returns a JSON object such as the above
*/
JsonNode* objArrNumVal(
	const char* stream,  /* streamID such as "VStr_vRPM_Engine_1 */
	const char* idxDT,   /* timestamp index indicator; e.g., IndexedDateTime */
	const char* szTag,   /* real-time value name; e.g., vRMP */
	const char* szTime,   /* array of timestamps */
	int sLength,    /* length of szTime string */
	double* dVal,    /* array of values */
	int nVal   /* size of timestamp/value arrays */
)
{
	int idx;
	JsonNode* objRet = json_mkobject();

	JsonNode* valuesArray = json_mkarray();
	for (idx = 0; idx < nVal; idx++) {
		JsonNode* timeAndVal = json_mkobject();
		json_append_member(timeAndVal, idxDT, json_mkstring(szTime + idx*sLength));
		json_append_member(timeAndVal, szTag, json_mknumber(dVal[idx]));

		json_append_element(valuesArray, timeAndVal);
	}

	json_append_member(objRet, LIT_containerid, json_mkstring(stream));
	json_append_member(objRet, LIT_values, valuesArray);

	return objRet;
}

/*
{
"streamId": "VStr_vRPM_Engine_1",
"values" : [
{
"IndexedDateTime": "2016-04-19T20:19:17.236Z",
<szTag[0]> : 1.23
<szTag[1]> : 11.23
},
{
"IndexedDateTime": "2016-04-19T20:19:18.236Z",
<szTag[0]> : 2.23
<szTag[1]> : 12.23
},
{
"IndexedDateTime": "2016-04-19T20:19:19.236Z",
<szTag[0]> : 1.23
<szTag[1]> : 11.23
},
{
"IndexedDateTime": "2016-04-19T20:19:20.236Z",
<szTag[0]> : 4.23
<szTag[1]> : 14.23
},
{
"IndexedDateTime": "2016-04-19T20:19:21.236Z",
<szTag[0]> : 5.23
<szTag[1]> : 6.23
}
]
}
This function returns a JSON object such as the above
*/
JsonNode* objArrNumVals(
	const char* stream,  /* streamID such as "VStr_vRPM_Engine_1 */
	const char* idxDT,   /* timestamp index indicator; e.g., IndexedDateTime */
	const char* szTime,   /* array of timestamps */
	int sLength,    /* length of szTime string */
	int nTime,   /* size of timestamp array */
	const char* szTag,   /* real-time value name; e.g., vRMP */
	int sTagLength,   /* length of szTag strings */
	int nTags,   /* number of real-time values */
	double* dVal    /* array of values */
)
{
	int idx;
	int jdx;
	JsonNode* objRet = json_mkobject();

	JsonNode* valuesArray = json_mkarray();
	for (idx = 0; idx < nTime; idx++) {
		JsonNode* timeAndVal = json_mkobject();
		json_append_member(timeAndVal, idxDT, json_mkstring(szTime + idx*sLength));

		for (jdx = 0; jdx < nTags; jdx++) {
			json_append_member(timeAndVal, szTag + jdx*sTagLength, json_mknumber(*(dVal + idx + (jdx*nTime))));
		}

		json_append_element(valuesArray, timeAndVal);
	}

	json_append_member(objRet, LIT_containerid, json_mkstring(stream));
	json_append_member(objRet, LIT_values, valuesArray);

	return objRet;
}
