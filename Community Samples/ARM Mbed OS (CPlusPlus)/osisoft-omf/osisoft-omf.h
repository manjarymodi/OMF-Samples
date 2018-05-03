//Copyright 2018 OSIsoft, LLC
//
//Licensed under the Apache License, Version 2.0 (the "License");
//you may not use this file except in compliance with the License.
//You may obtain a copy of the License at
//
//<http://www.apache.org/licenses/LICENSE-2.0>
//
//Unless required by applicable law or agreed to in writing, software
//distributed under the License is distributed on an "AS IS" BASIS,
//WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//See the License for the specific language governing permissions and
//limitations under the License.

#ifndef OSISOFT_OMF_H
#define OSISOFT_OMF_H
 
#include "mbed.h"
#include "https_request.h"
#include "config.deviceID.h"

// Automatically generated OMF-SPECIFIC CONSTANTS
// Similarly, specify the name of for the data values type; this should likewise be unique
// for each general class of IoT device--for example, if you were running this script on two different devices,
// each with different numbers and kinds of sensors, you would specify a different data values message type name
// when running the script on each device.  If both devices were the same, you could use the same data_values_message_type_name
const string DATA_VALUES_MESSAGE_TYPE_NAME = DEVICE_TYPE + "_data_values_type";

// The unique device name will also be used as part of the container id used to send live data values
const string DATA_VALUES_CONTAINER_ID = DEVICE_NAME + "_data_values_container";

// The device type will also be used for the asset type constant as well
const string ASSETS_MESSAGE_TYPE_NAME = DEVICE_TYPE + "_asset_type";

// Constants used for setting headers when sending OMF messages
const string OMF_VERSION = "1.0";
const string OMF_MESSAGE_FORMAT = "JSON";
const string CONTENT_TYPE = "application/json";   

// ************************************************************************
// Helper function that casts floats into strings
// ************************************************************************

string OMFLib_float_to_string(float f);

// ************************************************************************
// Helper function that sends an actual web request
// ************************************************************************

void OMFLib_sendMessageToEndpoint(TLSSocket* socket, const char* action, const char* message_type, const char* body);

// ************************************************************************
// Helper function that sends an actual web request; does not reuse sockets
// ************************************************************************

void OMFLib_sendMessageToEndpoint_NoSocketReuse(NetworkInterface* network, const char* action, const char* message_type, const char* body);

// ************************************************************************
// Helper function: prints out an HTTP response
// ************************************************************************
 
void OMFLib_dump_response(HttpResponse* res);

// ************************************************************************
// Gets the current time in the appropriate OMF format
// ************************************************************************

string OMFLib_getCurrentTimeString();

// ************************************************************************
// Sets the clock via NTP via the nwtwork
// ************************************************************************

void OMFLib_syncClockViaNTP(NetworkInterface* network);
 
#endif
