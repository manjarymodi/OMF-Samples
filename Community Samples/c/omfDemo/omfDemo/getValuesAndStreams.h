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

/* getValuesAndStreams.h */

#pragma once

#define RTVAL_ENGINE_RPM  1
#define RTVAL_ENGINE_COOLANTTEMP  2
#define RTVAL_VEHICLE_MEAS  3
#define RTVAL_VEHICLE_STATUS 4
#define RTVAL_VEHICLE_FLOAT 5
#define RTVAL_VEHICLE_INT 6
char* getStreams_RTValues(int code);
char* getAssets_Value_Links(int code);
char* getOneEngineValue(int code, const char* szEngineName);

#define ASSET_LINK_VEHICLE_VEHICLE  1
#define ASSET_LINK_VEHICLE_ENGINE  2
#define ASSET_LINK_VEHICLE_TRANSMISSION  3
char* getAssets_Asset_links(int code);


char* getValues_EngineRPMData();
char* getValues_EngineCoolantTempData();
char* getValues_VehicleData();
