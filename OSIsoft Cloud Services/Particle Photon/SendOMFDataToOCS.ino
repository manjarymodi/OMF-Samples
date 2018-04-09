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

// NOTE: this script was designed using the v1.0
// version of the OMF specification, as outlined here:
// http://omf-docs.readthedocs.io/en/v1.0/index.html

// ************************************************************************
// NOTE: PRIOR TO RUNNING THIS PROGRAM, SIGN INTO THE PARTICLE CONSOLE
// (https://console.particle.io/integrations) AND OPEN THE INTEGRATIONS TAB,
// AND CREATE THREE WEBHOOKS USING THE WEBHOOK TEMPLATES FOUND AT THE BOTTOM
// OF THIS FILE!!!  THESE WEBHOOKS RELAY DATA FROM THE PARTICLE CLOUD
// TO OCS, THUS THEY ARE ESSENTIAL, OTHERWISE DATA WILL NOT BE SENT TO OCS.
// ************************************************************************

// This example was designed to run using a Particle Internet Button dev kit (https://store.particle.io/),
// available from Amazon here: https://www.amazon.com/Particle-Internet-Button-Accelerometer-Buttons/dp/B01L46Q9LC

// Particle Photon Specs:
// CPU: STM32 ARM Cortex M3
// Wi-Fi Chip: BCM43362 Wi-Fi (802.11b/g/n)
// ROM and RAM: 1MB flash, 128KB RAM
// (See https://docs.particle.io/datasheets/photon-(wifi)/photon-datasheet/)

// ************************************************************************
// Import necessary packages
// ************************************************************************

#include <InternetButton.h>

// ************************************************************************
// Specify constant values (names, target URLS, etc.) needed by the script
// ************************************************************************

// Store the id of the container that will be used to receive live data values
const String DATA_VALUES_CONTAINER_ID = "Photon " + System.deviceID() + " Data";

// Similarly, specify the name of for the data values type; this should likewise be unique
// for each general class of IoT device--for example, if you were running this
// script on two different devices, each with different numbers and kinds of sensors,
// you'd specify a different data values message type name
// when running the script on each device.  If both devices were the same,
// you could use the same DATA_VALUES_MESSAGE_TYPE_NAME
const String DATA_VALUES_MESSAGE_TYPE_NAME = "Photon Button Dynamic Type";

// Specify the number of seconds to sleep in between value messages
const int DELAY_IN_BETWEENREADS_MILLISECONDS = 2*1000;

// ************************************************************************
// Other vars
// ************************************************************************

// Setup the internet button sensors and LEDs
InternetButton b = InternetButton();

// Main setup function
void setup() {
    
    // Activate the button
    b.begin();
    
    // ************************************************************************
    // Send the DYNAMIC types message, so that these types can be referenced in all later messages
    // ************************************************************************

    Particle.publish("CREATE_OMF_DYNAMIC_TYPE_MESSAGE", 
        ("{\"dataValuesTypeId\":\"" + DATA_VALUES_MESSAGE_TYPE_NAME + "\"}"),
        PRIVATE);
        
    // ************************************************************************
    // Create a JSON packet to define container IDs and the type
    // (using the types listed above) for each new data events container
    // ************************************************************************        
        
    Particle.publish("CREATE_OMF_CONTAINER_MESSAGE", 
        ("{\"contId\":\"" + DATA_VALUES_CONTAINER_ID + "\", \"dataValuesTypeId\":\"" + DATA_VALUES_MESSAGE_TYPE_NAME + "\"}"), 
        PRIVATE);
}

// ************************************************************************
// Finally, loop indefinitely, sending random events
// conforming to the value type that we defined earlier
// ************************************************************************

void loop() {
    
    // Read acceleration, along a range of -127 to +127
    int xValue = b.readX();
    int yValue = b.readY();
    int zValue = b.readZ();

    // This will make the color of the Button change with what direction you shake it
    // The abs() part takes the absolute value, because negatives don't work well
    b.allLedsOn(abs(xValue), abs(yValue), abs(zValue));
    
    // Securely publish the data to a Particle Webhook that will relay it to OCS
    // (Prior to this, these webhooks should be set up with the OCS URL, OMF headers, and producer token)
    Particle.publish("CREATE_OMF_DATA_MESSAGE", 
        ("{\"contId\":\"" + DATA_VALUES_CONTAINER_ID + "\", \"xValue\":" + (String)(xValue) + ", \"yValue\":" + (String)(yValue) + ", \"zValue\":" + (String)(zValue) + "}"),
        PRIVATE);

    // Send the next message after the required interval
    delay(DELAY_IN_BETWEENREADS_MILLISECONDS);
}

// ************************************************************************
// Webhook JSON: create webhooks using these three
// configurations at https://console.particle.io/integrations/webhooks. 
// NOTE:
// REPLACE THE VALUE OF THE "producertoken" HEADER IN EACH WEBHOOK
// WITH A VALID OCS PUBLISHER TOKEN, FOUND IN THE OCS PORTAL AT https://cloud.osisoft.com/ingress
// ************************************************************************
/*

// ************************************************************************
// WEBHOOK #1 TEMPLATE
// ************************************************************************

{
    "event": "CREATE_OMF_DYNAMIC_TYPE_MESSAGE",
    "url": "https://qi-data.osisoft.com/api/omf",
    "requestType": "POST",
    "noDefaults": true,
    "rejectUnauthorized": true,
    "headers": {
        "producertoken": "kjalhdfkasdjfaskjdfalskdjfahskljdf",
        "messagetype": "type",
        "action": "create",
        "omfversion": "1.0",
        "content-type": "application/json"
    },
    "json": [
        {
            "id": "{{dataValuesTypeId}}",
            "type": "object",
            "classification": "dynamic",
            "properties": {
                "Time": {
                    "format": "date-time",
                    "type": "string",
                    "isindex": true
                },
                "X-acceleration": {
                    "type": "number"
                },
                "Y-acceleration": {
                    "type": "number"
                },
                "Z-acceleration": {
                    "type": "number"
                }
            }
        }
    ]
}

// ************************************************************************
// WEBHOOK #2 TEMPLATE
// ************************************************************************

{
    "event": "CREATE_OMF_CONTAINER_MESSAGE",
    "url": "https://qi-data.osisoft.com/api/omf",
    "requestType": "POST",
    "noDefaults": true,
    "rejectUnauthorized": true,
    "headers": {
        "producertoken": "kjalhdfkasdjfaskjdfalskdjfahskljdf",
        "messagetype": "container",
        "action": "create",
        "omfversion": "1.0"
    },
    "json": [
        {
            "id": "{{contId}}",
            "typeid": "{{dataValuesTypeId}}"
        }
    ]
}

// ************************************************************************
// WEBHOOK #3 TEMPLATE
// ************************************************************************

{
    "event": "CREATE_OMF_DATA_MESSAGE",
    "url": "https://qi-data.osisoft.com/api/omf",
    "requestType": "POST",
    "noDefaults": true,
    "rejectUnauthorized": true,
    "headers": {
        "producertoken": "kjalhdfkasdjfaskjdfalskdjfahskljdf",
        "messagetype": "data",
        "action": "create",
        "omfversion": "1.0"
    },
    "json": [
        {
            "containerid": "{{contId}}",
            "values": [
                {
                    "Time": "{{PARTICLE_PUBLISHED_AT}}",
                    "X-acceleration": "{{xValue}}",
                    "Y-acceleration": "{{yValue}}",
                    "Z-acceleration": "{{zValue}}"
                }
            ]
        }
    ]
}

*/