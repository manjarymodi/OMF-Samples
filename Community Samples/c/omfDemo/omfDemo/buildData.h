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

/* buildData.h */

#include "deviceSimulator.h"


#pragma once
JsonNode* objArrNumVal(const char* stream, const char* idxDT, const char* szTag, const char* szTime, int sLength, double* dVal, int nVal);

char* sendOneVal(const char* valStream, const char* dateTimeKey, const char* szTime, char* szTag, double dVal);
char* sendOneValStr(const char* valStream, const char* dateTimeKey, const char* szTime, char* szTag, char* szVal);

char* val_Link_Asset_RT(
	const char* source_type,
	const char* source_typeversion,
	const char* containerid_prefix, /* a value such as "VStr_vRPM") */
	char* source_indexes, /* array of AF element names; e.g., Engine_1, Engine_2, Engine_3 */
	const int nElements,  /* number of elements */
	const int nStringLength  /* string length of the name of AF elements */
);

JsonNode* objArrNumVals(
	const char* stream,  /* streamID such as "VStr_vRPM_Engine_1 */
	const char* idxDT,   /* timestamp index indicator; e.g., IndexedDateTime */
	const char* szTime,   /* array of timestamps */
	int sLength,    /* length of szTime string */
	int nTime,   /* size of timestamp array */
	const char* szTag,   /* real-time value name; e.g., vRMP */
	int sTagLength,   /* length of szTag strings */
	int nTags,
	double* dVal    /* array of values */
); 
