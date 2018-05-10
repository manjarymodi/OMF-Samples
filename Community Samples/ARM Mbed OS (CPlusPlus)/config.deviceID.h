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

#ifndef DEVICE_ID_H
#define DEVICE_ID_H

// Specify the name of this device, or simply use the hostname; this is the name
// of the PI AF Element that will be created, and it will be included in the names
// of PI Points that get created as well
const string DEVICE_NAME = "K64F ARM Dev Board 1";

// Specify the device "type"; this will be part of the types message
//const string DEVICE_TYPE = DEVICE_NAME + "Type";
const string DEVICE_TYPE = "K64F ARM Dev Board";

// Optional (if sending assets data) specify the location, which will be a static
// AF attribute on the created AF Element (if not sending to OCS)
const string DEVICE_LOCATION = "IoT Test Lab";

#endif
