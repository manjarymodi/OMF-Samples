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

/* baseBuild.c */

#ifdef _WIN32
#include <Windows.h>
#endif
#include <time.h>
#include <string.h>

#include "baseBuild.h"

char* combineJObjs(JsonNode** objs, int nObjs)
{
	char* szRet;

	int idx;
	JsonNode* arrayRet = json_mkarray();
	for (idx = 0; idx < nObjs; idx++) {
		JsonNode* temp = *(objs + idx);

		json_append_element(arrayRet, *(objs + idx));
	}
	szRet = json_encode(arrayRet);

	json_delete(arrayRet);

	return szRet;
}

JsonNode* oCharChar(const char* key, const char* val)
{
	JsonNode* oRet = json_mkobject();
	json_append_member(oRet, key, json_mkstring(val));

	return oRet;
}

JsonNode* oCharBool(const char* key, bool b)
{
	JsonNode* oRet = json_mkobject();
	json_append_member(oRet, key, json_mkbool(b));

	return oRet;
}

JsonNode* oCharDouble(const char* key, double val)
{
	JsonNode* oRet = json_mkobject();
	json_append_member(oRet, key, json_mknumber(val));

	return oRet;
}

#define TIME_BUF 80
char* get_time(time_t theTime)
{
	static char sRet[TIME_BUF];

	struct tm bufTm;

#ifdef _WIN32
	gmtime_s(&bufTm, &theTime);
#elif linux
	gmtime_r(&theTime, &bufTm);
#else
	need to implement
#endif

	strftime(sRet, TIME_BUF, "%FT%TZ", &bufTm);

	return sRet;
}

#ifdef _WIN32
/*from
https://blogs.msdn.microsoft.com/joshpoley/2007/12/19/datetime-formats-and-conversions/
*/
void SystemTimeToTime_t(SYSTEMTIME *systemTime, time_t* dosTime)
{
	unsigned __int64 utcDosTime;
	LARGE_INTEGER utcFT = { 0 };
	LARGE_INTEGER jan1970FT = { 0 };

	jan1970FT.QuadPart = 116444736000000000I64; // january 1st 1970		
	SystemTimeToFileTime(systemTime, (FILETIME*)&utcFT);

	utcDosTime = (utcFT.QuadPart - jan1970FT.QuadPart) / 10000000;
	*dosTime = (time_t)utcDosTime;
}
#endif

char* get_currtime()
#ifdef _WIN32
{
	static char sRet[TIME_BUF];

	SYSTEMTIME st;
	int ms;
	time_t s;

	struct tm bufTm;
	char buf[sizeof("yyyy-mm-ddThh:mm:ss")];
	char msBuf[sizeof("000")];

	GetSystemTime(&st);

	SystemTimeToTime_t(&st, &s);
	ms = st.wMilliseconds;
	snprintf(msBuf, sizeof(msBuf), "%03d", ms);

	gmtime_s(&bufTm, &s);
	strftime(buf, sizeof buf, "%FT%T", &bufTm);

	snprintf(sRet, TIME_BUF, "%s.%sZ", buf, msBuf);
	return sRet;
}
#elif linux
{
	static char sRet[TIME_BUF];
	struct timespec spec;
	int ms;
	time_t s;

	struct tm bufTm;
	char buf[sizeof("yyyy-mm-ddThh:mm:ss")];
	char msBuf[sizeof("000")];

	clock_gettime(CLOCK_REALTIME, &spec);

	s = spec.tv_sec;
	ms = spec.tv_nsec / 1.0e6; /* change nano to milliseconds */
	snprintf(msBuf, sizeof(msBuf), "%03d", ms);

	gmtime_r(&s, &bufTm);
	strftime(buf, sizeof buf, "%FT%T", &bufTm);

	snprintf(sRet, TIME_BUF, "%s.%sZ", buf, msBuf);

	return sRet;
}
#else
need to implement
#endif

void getRTStreamId(char* streamId, int nLen, const char* szPrefix, const char* szTag)
{
	snprintf(streamId, nLen, "%s_%s", szPrefix, szTag);
}
