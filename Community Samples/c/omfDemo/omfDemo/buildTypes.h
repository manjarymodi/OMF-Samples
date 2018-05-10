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

/* buildTypes.h*/

#pragma once

#include "json.h"

#define KEY_description   "description"
#define KEY_properties   "properties"

#define KEY_enum   "enum"

#define TYPE_Value_vRPM   "vRPMType"
#define KEY_vRPM   "vRPM"

#define TYPE_Value_vCoolantTemp   "vCoolantTempType"
#define KEY_vCoolantTemp   "vCoolantTemp"

#define TYPE_Value_vVehicleMeas   "vVehicleMeasType"
#define KEY_vSpeed   "vSpeed"
#define KEY_vPressure   "vPressure"

#define TYPE_Value_vVehicleStatus  "vVehicleStatusType"
#define KEY_vVehicleStatus   "vVehicleStatus"

#define TYPE_Value_vVehicleFloat  "vVehicleFloatType"
#define KEY_vVehicleFloat   "vVehicleFloat"

#define TYPE_Value_vVehicleInt  "vVehicleIntType"
#define KEY_vVehicleInt   "vVehicleInt"

#define KEY_IndexedDateTime   "IndexedDateTime"

#define TYPE_VehicleType   "VehicleType"
#define TYPE_EngineType   "EngineType"
#define TYPE_TransmissionType   "TransmissionType"

#define TYPE_link     "__Link"

#define LINK_Vehicle_Vehicle_Link   "Vehicle_Vehicle_Link"
#define LINK_Vehicle_Engine_Link   "Vehicle_Engine_Link"
#define LINK_Vehicle_Transmission_Link   "Vehicle_Transmission_Link"

#define KEY_VehicleName   "VehicleName"
#define KEY_VIN   "VIN"
#define KEY_Year   "Year"
#define KEY_Make   "Make"
#define KEY_Model   "Model"
#define KEY_InServiceDate   "InServiceDate"

#define KEY_EngineName   "EngineName"
#define KEY_SerialNumber   "SerialNumber"
#define KEY_Manufacturer   "Manufacturer"
#define KEY_Cylinders   "Cylinders"

#define KEY_TransmissionName   "TransmissionName"

#define NUM_VALID_VEHSTATUS  3
#define NUM_VEHICLE_FLOAT 4
#define NUM_VEHICLE_INT 5

char* defineTypes();