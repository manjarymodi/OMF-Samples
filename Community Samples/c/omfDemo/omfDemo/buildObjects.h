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

/* buildValues.h */

#pragma once

#include "deviceSimulator.h"

char* defineVehicleAssets(const struct Vehicle* pAsset, int nAsset);
char* defineEngineAssets(const struct Engine* pAsset, int nAsset);
char* defineTransmissionAssets(const struct Transmission* pAsset, int nAsset);
char* defineLink_Sources_Targets(
	const char* szSource,    /* array of Source names */
	const char* sources_type, /* all sources must be the same type */
	const char* sources_type_version, /* safely nullable */
	const char* szTarget,    /* array of Target names */
	const char* targets_type, /* all targets must be the same type */
	const char* targets_type_version, /* safely nullable */
	const int nStrings,    /* size of the array */
	const int sLength   /* string length of Source and Target names */
);

