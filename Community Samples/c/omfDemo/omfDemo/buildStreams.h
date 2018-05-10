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

/* buildStreams.h */

#pragma once

#define LIT_containerid "containerid"

#define STREAM_VStr_vRPM   "VStr_vRPM"
#define STREAM_VStr_vCoolantTemp   "VStr_vCoolantTemp"
#define STREAM_VStr_vVehicleMeas   "VStr_vVehicleMeas"
#define STREAM_VStr_vVehicleStatus   "VStr_vVehicleStatus"
#define STREAM_VStr_vVehicleFloat   "VStr_vVehicleFloat"
#define STREAM_VStr_vVehicleInt   "VStr_vVehicleInt"

#define STREAM_ID_LEN 80

char* defineRTStreams(
	const char* szPrefix, /* a value such as "VStr_vRPM" */
	char* szElement, /* array of AF element names; e.g., Engine_1, Engine_2, Engine_3 */
	int nElements,  /* number of elements */
	int nStringLength,  /* string length of the name of AF elements */
	const char* typeId /* a value such as "vRPMType" */
	);
