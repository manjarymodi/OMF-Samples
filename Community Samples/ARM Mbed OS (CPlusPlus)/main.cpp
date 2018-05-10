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

#include "mbed.h"
#include "easy-connect.h"

// Custom library imports that contain SSL certificates and OMF functions
#include "config.SSLCertificates.h"
#include "osisoft-omf.h"
#include "config.egressSettings.h"
#include "config.customOMFJSONStructures.h"

// Include libraries for different sensors
#include "FXOS8700Q.h"

// Specify serial port baud rate
const int SERIAL_PORT_BAUD_RATE = 115200;

// Initialize the sensors
I2C i2c(I2C_SDA, I2C_SCL);
FXOS8700QAccelerometer acc(i2c, FXOS8700CQ_SLAVE_ADDR1);
FXOS8700QMagnetometer mag(i2c, FXOS8700CQ_SLAVE_ADDR1);

// Set up a status LED that will toggle on while messages are being sent
DigitalOut led1(LED1);
    
// Set up USB serial printing out (for debugging)
Serial pc(USBTX, USBRX);

// NAME-SENSITIVE: Dusl-stacked endpoint will share this global - its name-sensitive
extern NetworkInterface* __network_interface;

//  mbed Cloud: Endpoint inclusion (optional, see future versions)
//#include "mbedCloudEndpoint.h"

// ************************************************************************
// OSISoft Main loop
// ************************************************************************
void runOSISoftMainLoop(const void *argv) {
    // Declare variables that will hold sensor data
    motion_data_units_t acc_data, mag_data;

    // Note: remove serial connection; it is handled in another function
    
    // Note: network connection is handled by mbed cloud!  Use the 
    // __network_interface network interface variable
    
    // Connect to the network
    printf("\n\r----- Connecting to network -----\n\r");
    //NetworkInterface* network = easy_connect(true);
    //if (!network) {
    if (!__network_interface) {
        printf("Network connection failed!");
        return;
    }
    
    // Try to set the current time in UTC seconds:
    //OMFLib_syncClockViaNTP(network);   
    OMFLib_syncClockViaNTP(__network_interface);  
    wait(1);
        // Enable the sensors!
    printf("\n\r----- Enabling sensors -----\n\r");
    acc.enable();
    mag.enable();
    wait(1);

    // Print sensor status (optional)
    printf("\n\r----- Testing sensors -----\n\r");
    pc.printf("FXOS8700QAccelerometer Who Am I= %X\r\n", acc.whoAmI());
    pc.printf("FXOS8700QMagnetometer Who Am I= %X\r\n", mag.whoAmI());   
    wait(1);
            
    // Send the DYNAMIC types message, so that these types can be referenced in all later messages
    printf("\n\r!!!!!!!! Sending DYNAMIC Types message... !!!!!!!!\n\r");
    //OMFLib_sendMessageToEndpoint(socket, "create", "Type", dynamic_types_message_JSON.c_str());
    //OMFLib_sendMessageToEndpoint_NoSocketReuse(network, "create", "Type", DYNAMIC_TYPES_MESSAGE_JSON.c_str());
    OMFLib_sendMessageToEndpoint_NoSocketReuse(__network_interface, "create", "Type", DYNAMIC_TYPES_MESSAGE_JSON.c_str());

    // Send the container message, to instantiate this particular container; we can now directly start sending data to it using its Id
    printf("\n\r!!!!!!!! Sending Containers message... !!!!!!!!\n\r");
    //OMFLib_sendMessageToEndpoint(socket, "create", "Container", CONTAINERS_MESSAGE_JSON.c_str());  
    //OMFLib_sendMessageToEndpoint_NoSocketReuse(network, "create", "Container", CONTAINERS_MESSAGE_JSON.c_str());  
    OMFLib_sendMessageToEndpoint_NoSocketReuse(__network_interface, "create", "Container", CONTAINERS_MESSAGE_JSON.c_str()); 
    
    // Check whether assets and links should be sent!
    if (SEND_ASSETS_AND_LINKS == true)
    {
    
        // ************************************************************************  
        // Send the STATIC types message, so that these types can be referenced in all later messages
        // ************************************************************************
    
        printf("\n\r!!!!!!!! Sending STATIC Types message... !!!!!!!!\n\r");
        //OMFLib_sendMessageToEndpoint(socket, "create", "Type", static_types_message_JSON.c_str()); 
        //OMFLib_sendMessageToEndpoint_NoSocketReuse(network, "create", "Type", STATIC_TYPES_MESSAGE_JSON.c_str());         
        OMFLib_sendMessageToEndpoint_NoSocketReuse(__network_interface, "create", "Type", STATIC_TYPES_MESSAGE_JSON.c_str());
    
        // ************************************************************************
        // Send the message to create the PI AF asset; it will not appear in PI AF, though, because it has not yet been positioned...
        // ************************************************************************
    
        printf("\n\r!!!!!!!! Sending Assets message... !!!!!!!!\n\r");
        //OMFLib_sendMessageToEndpoint(socket, "create", "Data", assets_message_JSON.c_str());
        //OMFLib_sendMessageToEndpoint_NoSocketReuse(network, "create", "Data", ASSETS_MESSAGE_JSON.c_str());  
        OMFLib_sendMessageToEndpoint_NoSocketReuse(__network_interface, "create", "Data", ASSETS_MESSAGE_JSON.c_str());
        
        // ************************************************************************
        // Send the message to link the PI AF asset
        // ************************************************************************
        
        printf("\n\r!!!!!!!! Sending Links message... !!!!!!!!\n\r");
        //OMFLib_sendMessageToEndpoint(socket, "create", "Data", LINKS_MESSAGE_JSON.c_str());
        //OMFLib_sendMessageToEndpoint_NoSocketReuse(network, "create", "Data", LINKS_MESSAGE_JSON.c_str());  
        OMFLib_sendMessageToEndpoint_NoSocketReuse(__network_interface, "create", "Data", LINKS_MESSAGE_JSON.c_str());
    }
    
    // Main loop
    wait(1);
    printf("\n\r!!!!!!!! Now sending live data !!!!!!!!\n\r");
    wait(1);
    while (true)
    {
        // Turn on the LED
        led1 = 1;
        
        // Read the sensor values; see https://os.mbed.com/components/FXOS8700Q/
        acc.getAxis(acc_data);
        mag.getAxis(mag_data);
        
        // Assemble the message: USE THE SPECIFIC OMF DATA MESSAGE TYPE
        string body = ("[{" 
            "\"containerid\": \"" + DATA_VALUES_CONTAINER_ID + "\"," 
            "\"values\": [{" 
                "\"Time\": \"" + OMFLib_getCurrentTimeString() + "\"," 
                // If you are NOT using a K64f, comment out the below lines
                "\"X-acceleration\":" + OMFLib_float_to_string(acc_data.x) + "," 
                "\"Y-acceleration\":" + OMFLib_float_to_string(acc_data.y) + "," 
                "\"Z-acceleration\":" + OMFLib_float_to_string(acc_data.z) + "," 
                "\"X-orientation\":" + OMFLib_float_to_string(mag_data.x) + "," 
                "\"Y-orientation\":" + OMFLib_float_to_string(mag_data.y) + "," 
                "\"Z-orientation\":" + OMFLib_float_to_string(mag_data.z) + "" 
                // Otherwise, in this example,
                // we're just sending along random values for these two \"sensors\"
                //"\"Raw Sensor Reading 1\":" + OMFLib_float_to_string(100 * static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) + "," 
                //"\"Raw Sensor Reading 2\":" + OMFLib_float_to_string(100 * static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) + "" 
            "}]}]"); 
        
        // Print the message (optional)
        //printf(body.c_str());

        // Send message
        //OMFLib_sendMessageToEndpoint(socket, "create", "Data", body.c_str());
        //OMFLib_sendMessageToEndpoint_NoSocketReuse(network, "create", "Data", body.c_str());
        OMFLib_sendMessageToEndpoint_NoSocketReuse(__network_interface, "create", "Data", body.c_str());
    
        // Wait; turn off the LED
        led1.write(0);
        // Note: you can comment this out to remove the wait!
        wait(SECONDS_BETWEEN_MESSAGES);
    }
}

// ************************************************************************
// Main function
// ************************************************************************
int main() {
    // Open a serial connection
    pc.baud(SERIAL_PORT_BAUD_RATE);
    
	/*
    // mbed Cloud: initialize mbed cloud endpoint
    printf("\n\r----- Initializing mbed cloud endpoint -----\n\r");
    init_mbed_cloud();
    printf("\n\r----- Done initializing mbed cloud endpoint.\n\r");
    
    //  mbed Cloud: start a thread to execute the OSISoft event loop
    Thread osi_main_loop;
    printf("\n\r----- Starting OSIsoft thread... -----\n\r");
    osi_main_loop.start(callback(runOSISoftMainLoop, (void *)NULL));
    printf("\n\r----- Done starting OSIsoft thread.\n\r");
    
    //  mbed Cloud: start the mbed cloud endpoint main event loop
    printf("\n\r----- Starting mbed cloud endpoint event loop -----\n\r");
    start_mbed_cloud_event_loop();
    printf("\n\r----- Done starting mbed cloud endpoint event loop.\n\r");
    */
    // Alternatively, simply directly run the OSIsoft main loop
    runOSISoftMainLoop((void *)NULL);
}
