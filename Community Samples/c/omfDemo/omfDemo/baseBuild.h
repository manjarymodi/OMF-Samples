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

/* baseBuild.h */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "json.h"

#define KEY_id   "id"
#define KEY_isindex   "isindex"
#define KEY_format   "format"

#define FORMAT_date_time   "date-time"

#define KEY_type   "type" //data type e.g. int, float, string...
#define TYPE_integer   "integer"
#define TYPE_number   "number"
#define TYPE_object   "object"
#define TYPE_string   "string"

#define KEY_classification     "classification"
#define CLASSIFICATION_static  "static"
#define CLASSIFICATION_dynamic "dynamic"

#define LIT_Source   "source"
#define LIT_Target   "target"
#define LIT_values	 "values"
#define LIT_typeid	 "typeid" //omf's type e.g VehicleType, Vehicle_Meas_Link
#define LIT_typeversion "typeversion"
#define LIT_index    "index"

char* combineJObjs(JsonNode** objs, int nObjs);

JsonNode* oCharChar(const char* key, const char* val);
JsonNode* oCharBool(const char* key, bool b);
JsonNode* oCharDouble(const char* key, double val);

char* get_time(time_t theTime);
char* get_currtime();

void getRTStreamId(char* streamId, int nLen, const char* szPrefix, const char* szTag);

