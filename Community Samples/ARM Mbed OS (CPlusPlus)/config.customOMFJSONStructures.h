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

#ifndef OMF_JSON_H
#define OMF_JSON_H

#include "osisoft-omf.h"

// ************************************************************************
// Create JSON to define the type that will be used to send live values
// ************************************************************************

string DYNAMIC_TYPES_MESSAGE_JSON = "[{" 
    "\"id\": \"" + DATA_VALUES_MESSAGE_TYPE_NAME + "\"," 
    "\"type\": \"object\"," 
    "\"classification\": \"dynamic\"," 
    "\"properties\": {" 
        "\"Time\": {" 
            "\"format\": \"date-time\"," 
            "\"type\": \"string\"," 
            "\"isindex\": true" 
        "}," 
        // If you are NOT using a K64f, comment out the below lines
        "\"X-acceleration\": {\"type\": \"number\",\"description\": \"In Gs\"}," 
        "\"Y-acceleration\": {\"type\": \"number\",\"description\": \"In Gs\"}," 
        "\"Z-acceleration\": {\"type\": \"number\",\"description\": \"In Gs\"}," 
        "\"X-orientation\": {\"type\": \"number\",\"description\": \"In degrees\"}," 
        "\"Y-orientation\": {\"type\": \"number\",\"description\": \"In degrees\"}," 
        "\"Z-orientation\": {\"type\": \"number\",\"description\": \"In degrees\"}" 
        // Send two random heartbeats
        //"\"Raw Sensor Reading 1\": {\"type\": \"number\"}," 
        //"\"Raw Sensor Reading 2\": {\"type\": \"number\"}" 
        // For example, to allow you to send a string-type live data value," + 
        // such as \"Status\", you would add
        //\"Status\": {
        //   \"type\": \"string\"
        //}
"}}]";

// ************************************************************************
// Create JSON to define the type that will be used to send AF assets
// ************************************************************************

string STATIC_TYPES_MESSAGE_JSON = "[{" 
    "\"id\": \"" + ASSETS_MESSAGE_TYPE_NAME + "\"," 
    "\"type\": \"object\"," 
    "\"classification\": \"static\"," 
    "\"properties\": {" 
        "\"Name\": {" 
            "\"type\": \"string\"," 
            "\"isindex\": true" 
        "}," 
        "\"Device Type\": {" 
            "\"type\": \"string\"" 
        "}," 
        "\"Location\": {" 
            "\"type\": \"string\"" 
        "}," 
        "\"Data Ingress Method\": {" 
            "\"type\": \"string\"" 
        "}" 
        // For example, to add a number-type static
        // attribute for the device model, you would add
        // \"Model\": {
        //   \"type\": \"number\"
        //}
    "}" 
"}]";

// ************************************************************************
// Create a JSON packet to containing the asset data for the PI AF asset that will be made
// ************************************************************************

// Here is where you can specify values for the static PI AF attributes;
// in this case, we are auto-populating the Device Type, but you can manually hard-code in values if you wish
// we also add the LINKS to be made, which will both position the new PI AF
// Element, so it will show up in AF, and will associate the PI Points that will be created with that Element        
string ASSETS_MESSAGE_JSON = "["
    "{"
        // This will end up creating a new PI AF Element with this specific name and static attribute values
        "\"typeid\":\"" + ASSETS_MESSAGE_TYPE_NAME + "\","
        "\"values\": ["
            "{" ""
                "\"Name\": \"" + DEVICE_NAME + "\","
                "\"Device Type\": \"" + "ARM dev board" + "\","
                "\"Location\": \"" + DEVICE_LOCATION + "\","
                "\"Data Ingress Method\": \"OMF\""
            "}"
        "]"
    "}"
"]";  

// The containers message JSON requires both of the above constants; it is also a constant
const string CONTAINERS_MESSAGE_JSON = "["
    "{"
        "\"id\": \"" + DATA_VALUES_CONTAINER_ID + "\","
        "\"typeid\": \"" + DATA_VALUES_MESSAGE_TYPE_NAME + "\""
    "}"
"]";

// A JSON packet to containing linking data for the PI AF asset that will be made
const string LINKS_MESSAGE_JSON = "["
    "{"
        "\"typeid\": \"__Link\"" ","
        "\"values\": ["
            // This first link will locate such a newly created AF Element under the root PI element targeted by the PI Connector in your target AF database
            // This was specfied in the Connector Relay Admin page; note that a new parent element, with the same name as the producer_token, will also be made
            "{"
                "\"Source\": {"
                    "\"typeid\": \"" + ASSETS_MESSAGE_TYPE_NAME + "\","
                    "\"index\": \"_ROOT\""
                "},"
                "\"Target\": {"
                    "\"typeid\": \"" + ASSETS_MESSAGE_TYPE_NAME + "\","
                    "\"index\": \"" +  DEVICE_NAME + "\""
                "}"
            "},"
            // This second link will map new PI Points (created by messages sent to the data values container) to a newly create element
            "{"
            "\"Source\": {"
                "\"typeid\": \"" + ASSETS_MESSAGE_TYPE_NAME + "\","
                    "\"index\": \"" + DEVICE_NAME + + "\""
                "},"
                "\"Target\": {"
                    "\"containerid\": \"" + DATA_VALUES_CONTAINER_ID + "\""
                "}"
            "}"
        "]"
    "}" 
"]";
      
#endif