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
// Import necessary packages
// ************************************************************************

#include "mbed.h"
#include "easy-connect.h"
#include "https_request.h"
#include "ntp-client/NTPClient.h"

// Set up USB serial printing out (for debugging)
Serial pc(USBTX, USBRX);

// ************************************************************************
// Specify constant values (names, target URLS, etc.) needed by the script
// ************************************************************************

// Specify the name of this device, or simply use the hostname; this is the name
// of the PI AF Element that will be created, and it'll be included in the names
// of PI Points that get created as well
const string DEVICE_NAME = "MyDevice_Mbed";
//DEVICE_NAME = "MyCustomDeviceName"

// Specify a device location (optional); this will be added as a static
// string attribute to the AF Element that is created
const string DEVICE_LOCATION = "IoT Test Lab";

// Specify the name of the Assets type message; this will also end up becoming
// part of the name of the PI AF Element template that is created; for example, this could be
// "AssetsType_RaspberryPI" or "AssetsType_Dragonboard"
// You will want to make this different for each general class of IoT module that you use
const string ASSETS_MESSAGE_TYPE_NAME = DEVICE_NAME + "_assets_type" + "";
//ASSETS_MESSAGE_TYPE_NAME = "assets_type" + "IoT Device Model 74656" // An example

// Similarly, specify the name of for the data values type; this should likewise be unique
// for each general class of IoT device--for example, if you were running this
// script on two different devices, each with different numbers and kinds of sensors,
// you'd specify a different data values message type name
// when running the script on each device.  If both devices were the same,
// you could use the same DATA_VALUES_MESSAGE_TYPE_NAME
const string DATA_VALUES_MESSAGE_TYPE_NAME = "CPlusPlus_data_values_type";
//DATA_VALUES_MESSAGE_TYPE_NAME = "data_values_type" + "IoT Device Model 74656" // An example

// Store the id of the container that will be used to receive live data values
const string DATA_VALUES_CONTAINER_ID = (DEVICE_NAME + "_data_values_container");

// Specify the number of seconds to sleep in between value messages
const int NUMBER_OF_SECONDS_BETWEEN_VALUE_MESSAGES = 2;

// Specify whether you're sending data to OSIsoft cloud services or not
const bool SEND_DATA_TO_OSISOFT_CLOUD_SERVICES = false;

// Specify the address of the destination endpoint; it should be of the form
// http://<host/ip>:<port>/ingress/messages
// For example, "https://myservername:8118/ingress/messages"
const char* TARGET_URL = "https://lopezpiserver:777/ingress/messages";
// !!! Note: if sending data to OSIsoft cloud services,
// uncomment the below line in order to set the target URL to the OCS OMF endpoint:
//TARGET_URL = "https://dat-a.osisoft.com/api/omf"

// Specify the producer token, a unique token used to identify and authorize a given OMF producer. Consult the OSIsoft Cloud Services or PI Connector Relay documentation for further information.
const string PRODUCER_TOKEN = "OMFv1";
// !!! Note: if sending data to OSIsoft cloud services, the producer token should be the
// security token obtained for a particular Tenant and Publisher; see
// http://qi-docs.readthedocs.io/en/latest/OMF_Ingress_Specification.html//headers

/* List of trusted root CA certificates for the target endpoint
* To add more root certificates, just concatenate them.
*/
const char SSL_CA_PEM[] =
/* %%%%%%%%%%%% Local Dev Server %%%%%%%%%%%% */
"-----BEGIN CERTIFICATE-----\n"
"MIIFaTCCA1GgAwIBAgIQAP7Wl4bpwwxcncFEh2FzMTANBgkqhkiG9w0BAQsFADAv\n"
"MRYwFAYDVQQDDA1MT1BFWlBJU0VSVkVSMRUwEwYDVQQKDAxPU0lzb2Z0LCBMTEMw\n"
"IBcNMTYwOTI5MTk0MTU1WhgPOTk5OTEyMzEyMzU5NTlaMC8xFjAUBgNVBAMMDUxP\n"
"UEVaUElTRVJWRVIxFTATBgNVBAoMDE9TSXNvZnQsIExMQzCCAiIwDQYJKoZIhvcN\n"
"AQEBBQADggIPADCCAgoCggIBALul3jCAk0f0SJXhzTeLGqBmGLcqcr0QSphe3eJJ\n"
"7Ehn9/MRyEcI1DHBHy/XuwSIBHVt0bRs8ZTdXxoPjElQPwDwjZcQyRSyc6sIxILV\n"
"A58jDB6Cd0ZKrKZd2NB1QAlo0uEUc1NEfqoRCo+2+h6S+I2D7julGXoTcl+OvaaD\n"
"dh+XT+iOgXfoPdeGHqhsRsGh1nebsZ62P1/3m10evMTlXABkteVI/o3H46GPfFMH\n"
"5ftovXUpPI7wPG9r4goJ0gha7dL4jAuKAkruHd7ALFQOF/CeYdt01Hg+x2SXjy36\n"
"2DpK5cQ5+g5h3hqpuEO2mkDRc0tGdw0RkKvQSiaxQNYZmE/OqaMIVezTe3NMbw7U\n"
"9UBXGNR4tSH0693PXrNjO4NHSoshbNJnZqc+eW2MsxoVGU5EQ+VZ09O7EpcSP9uL\n"
"Q4EFPgeTNXp3/ax6XhqVTKOM2ZaTOxUQIGAKShkU9ULm4miSifYVUVOpqIehxHW0\n"
"hRPeqR6/BVZ5HYnxvFmj7CuuBTG2eXAivROSB+/QHxvIEnLfMilet6K6rFs39TBc\n"
"Z1VDRXs0QOoo08dMsFqE0i9CS62yD5Hfy2/p95f2RMDXprFuFbZwKgsmoYC/e1GF\n"
"LJCHN1l+0GHf3l04pYiO52Sn1iPqA/2jb+hkUciUFmAstniJq5qM+1lBipp5TAWz\n"
"VU51AgMBAAGjfzB9MGYGA1UdIwRfMF2AFDRBoyppASyZG415I/jIp05s2MHAoTOk\n"
"MTAvMRYwFAYDVQQDDA1MT1BFWlBJU0VSVkVSMRUwEwYDVQQKDAxPU0lzb2Z0LCBM\n"
"TEOCEAD+1peG6cMMXJ3BRIdhczEwEwYDVR0lBAwwCgYIKwYBBQUHAwEwDQYJKoZI\n"
"hvcNAQELBQADggIBAFG9Hit074c6QnvTVSPdIi9eFnSO43U1LHpfVu+gZ11Xt4fW\n"
"nYKKniCsen7s4aUtvB4PjpZgrzs3080Z33mEh3kazA+jsSt4//4/IhHsD9u6mzYe\n"
"rFwMmjIu3tb1CaotfAgvCaLJQL11YFSF7iqaBeUMYj5j8GOGnIKFvV82iERzO0By\n"
"lO8ubkRMWlw3t6CA8b7zFqt4/SEYLpV/D+Os59zZqIiSSNnierOWVktsasuhshJU\n"
"LAK1jyxj/kIDswULB31QNXUjBin/fg91+WmgpbnRfbei6TZYEJ4UKQIHWpO3ykSC\n"
"CfiqOHQqo1FTUCsNr5KRStXEaQ+6hjdrvlmOQ+128Mfhh2Cjpjb6luS/RTTx+PNy\n"
"pQvTF7RhfA/qR6kZgXRi7uZSjLEeyOeKX8jASGBlyXnGqW6jsAp0B2+V/6XMgLHu\n"
"8Q9PGaqM0XA5HekXfqSQWZpMGz1F6FLVKnrb1EEWhyvyc/UXYB4MNpgZKqcDZbdq\n"
"Hx+LggcScLrkg6UD4dTEuVSoTsOeKESWT4ikDfumlt+Qi7zt2kkt9uRExD7sAjEf\n"
"dW+EnITltQFVGjcFruPRqBDNK2qb8375Ejk/SCVAfkU078ouEbIFcf4cDlSHPfV+\n"
"4/oNf5a3H7itl2nesaGbXMNlqs28YGmGXTUP9hJLa4M7HK1uy2u/eUMgd9Q0\n"
"-----END CERTIFICATE-----\n";

// ************************************************************************
// Helper function: run any code needed to initialize local sensors, if necessary for this hardware
// ************************************************************************

// Below is where you can initialize any global variables that are needed by your application;
// certain sensors, for example, will require global interface or sensor variables
// myExampleInterfaceKitGlobalVar = None

// The following function is where you can insert specific initialization code to set up
// sensors for a particular IoT module or platform
void initialize_sensors()
{
    printf ("\n--- Sensors initializing...");
    // Initialize the random generator
    srand(time(NULL));
    //For a raspberry pi, for example, to set up pins 4 and 5, you would add
    //GPIO.setmode(GPIO.BCM)
    //GPIO.setup(4, GPIO.IN)
    //GPIO.setup(5, GPIO.IN)
    printf ("--- Sensors initialized!");
    // In short, in this example, by default,
    // this function is called but doesn't do anything (it's just a placeholder)
}


// ************************************************************************
// Helper function that casts floats into strings
// ************************************************************************

string OMFLib_float_to_string(float f) {
    char buffer[20];
    int n = sprintf(buffer, "%f", f);
    // Return -1 if a parse failure ocurred
    if (string(buffer) == "NaN") {
        return string("-1");
    }
    else {
        return string(buffer);
    }
}

// ************************************************************************
// Helper function: prints out an HTTP response
// ************************************************************************
 
void OMFLib_dump_response(HttpResponse* res)
{
    printf("\n\r----- HTTPS POST response -----\n\r");
    // Print the status code
    mbedtls_printf("Status: %d - %s\n\r", res->get_status_code(), res->get_status_message().c_str());
    // Print the headers
    mbedtls_printf("Response headers:\n\r");
    for (size_t ix = 0; ix < res->get_headers_length(); ix++) {
        mbedtls_printf("\t%s: %s\n\r", res->get_headers_fields()[ix]->c_str(), res->get_headers_values()[ix]->c_str());
    }
    // Print the body
    mbedtls_printf("Response body (%d bytes):\n\n%s", res->get_body_length(), res->get_body_as_string().c_str());
}

// ************************************************************************
// Helper function that sends an actual web request; does not reuse sockets
// ************************************************************************

void send_omf_message_to_endpoint(NetworkInterface* network, const char* action, const char* message_type, const char* body) {

    // !!! Note: if desired, uncomment the below line to printf  the outgoing message
    //printf("\n\rOutgoing message:\n\t%s\n\r", body);
    
    // Create a connection object
    HttpsRequest* post_req = new HttpsRequest(network, SSL_CA_PEM, HTTP_POST, TARGET_URL);

    // Turn on or off debugging - this shows or hides TLS connection information
    //post_req->set_debug(true);

    // Assemble headers that contain the producer token and message type
    // Note: in this example, the only action that is used is \"create\",
    // which will work totally fine;
    // to expand this application, you could modify it to use the \"update\"
    // action to, for example, modify existing AF element template types
    post_req->set_header("producertoken", PRODUCER_TOKEN);
    post_req->set_header("messagetype", message_type);
    post_req->set_header("action", action);
    post_req->set_header("messageformat", "JSON");
    post_req->set_header("omfversion", "1.0");

    // Send the request, and collect the response
    HttpResponse* post_res = post_req->send(body, strlen(body));
    
    // Show the response
    if (!post_res) {
        printf("HttpRequest failed (error code %d)\n\r", post_req->get_error());
    } else {
        printf("Success!\n\r");
    }

    // Free up the request object
    delete post_req;
}

// ************************************************************************
// Sets the clock via NTP via the nwtwork; can point to a local or internet-based server
// ************************************************************************

void OMFLib_syncClockViaNTP(NetworkInterface* network) {
    printf("\n\r----- Setting internal clock -----\n\r");
    // See https://github.com/ARMmbed/ntp-client-example/blob/master/main.cpp
    
    // Connect the ntp object to the network
    NTPClient ntp(network);
    
    // Set the ntp server to either an internet-based server OR to a local server
    ntp.set_server("2.pool.ntp.org", 123);
    
    // Get the timestamp via NTP
    time_t timestamp = ntp.get_timestamp();
    if (timestamp > 0) {
        set_time(timestamp);
        printf("Clock set via NTP to UTC %s", ctime(&timestamp));
    } else {
        printf("NTP time sync failed.");
    }
}

// ************************************************************************
// Create a JSON packet to define the types of streams that will be sent
// ************************************************************************

string DYNAMIC_TYPES_MESSAGE_JSON =

    // ************************************************************************
    // There are several different message types that will be used by this script, but
    // you can customize this script for your own needs by modifying the types:
    // First, you can modify the \"AssetsType\", which will allow you to customize which static
    // attributes are added to the new PI AF Element that will be created, and second," + 
    // you can modify the \"DataValuesType\", which will allow you to customize this script to send
    // additional sensor values, in addition to (or instead of) the two shown here
    
    // This values type is going to be used to send real-time values; feel free to rename the
    // values from \"Raw Sensor Reading 1\" to, say, \"Temperature\", or \"Pressure\"
    // Note:
    // all keywords (\"id\", \"type\", \"classification\", etc. are case sensitive!);
    // For a list of the specific keywords used in these messages," + 
    // see http://omf-docs.readthedocs.io/
    
    "[{" 
    "\"id\": \"" + DATA_VALUES_MESSAGE_TYPE_NAME + "\"," 
    "\"type\": \"object\"," 
    "\"classification\": \"dynamic\"," 
    "\"properties\": {" 
    "\"Time\": {" 
    "\"format\": \"date-time\"," 
    "\"type\": \"string\"," 
    "\"isindex\": true" 
    "}," 
    "\"Raw Sensor Reading 1\": {" 
    "\"type\": \"number\"" 
    "}," 
    "\"Raw Sensor Reading 2\": {" 
    "\"type\": \"number\"" 
    "}" 
    // For example, to allow you to send a string-type live data value," + 
    // such as \"Status\", you would add
    //\"Status\": {
    //   \"type\": \"string\"
    //}
    "}}]";

string STATIC_TYPES_MESSAGE_JSON =
    // This asset type is used to define a PI AF Element that will be created;
    // this type also defines two string attributes that will be created
    // as well; feel free to rename these or add additional
    // attributes for each Element (PI Point attributes will be added later);
    // The name of this type will also end up being part of the name of the PI AF Element template
    // that is automatically created
    "[{" 
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
// Create a JSON packet to define container IDs and the type
// (using the types listed above) for each new data events container
// ************************************************************************

// The device name that you specified earlier will be used as the AF Element name!
string NEW_AF_ELEMENT_NAME = DEVICE_NAME;
string CONTAINERS_MESSAGE_JSON = "[{" 
    "\"id\": \"" + DATA_VALUES_CONTAINER_ID + "\"," 
    "\"typeid\": \"" + DATA_VALUES_MESSAGE_TYPE_NAME + "\"" 
    "}]";

// ************************************************************************
// Create a JSON packet to containing the asset and
// linking data for the PI AF asset that will be made
// ************************************************************************

// Here is where you can specify values for the static PI AF attributes;
// in this case, we"re auto-populating the Device Type," + 
// but you can manually hard-code in values if you wish
// we also add the LINKS to be made, which will both position the new PI AF
// Element, so it will show up in AF, and will associate the PI Points
// that will be created with that Element
string ASSETS_AND_LINKS_MESSAGE_JSON = "[{" 
    // This will end up creating a new PI AF Element with
    // this specific name and attribute values
    "\"typeid\": \"" + ASSETS_MESSAGE_TYPE_NAME + "\"," 
    "\"values\": [{" 
    "\"Name\":\"" + NEW_AF_ELEMENT_NAME + "\"," 
    "\"Device Type\": \"" + "Type74656" + "\"," 
    "\"Location\": \"" + DEVICE_LOCATION + "\"," 
    "\"Data Ingress Method\": \"OMF\"" 
    "}]},{" 
    "\"typeid\": \"__Link\"," 
    "\"values\": [{" 
    // This first link will locate such a newly created AF Element under
    // the root PI element targeted by the PI Connector in your target AF database
    // This was specified in the Connector Relay Admin page; note that a new
    // parent element, with the same name as the PRODUCER_TOKEN, will also be made
    "\"Source\": {" 
    "\"typeid\": \"" + ASSETS_MESSAGE_TYPE_NAME + "\"," 
    "\"index\": \"_ROOT\"" 
    "}," 
    "\"Target\": {" 
    "\"typeid\": \"" + ASSETS_MESSAGE_TYPE_NAME + "\"," 
    "\"index\": \"" + NEW_AF_ELEMENT_NAME + "\"" 
    "}},{" 
    // This second link will map new PI Points (created by messages
    // sent to the data values container) to a newly create element
    "\"Source\": {" 
    "\"typeid\": \"" + ASSETS_MESSAGE_TYPE_NAME + "\"," 
    "\"index\":\"" + NEW_AF_ELEMENT_NAME + "\"" 
    "}," 
    "\"Target\": {" 
    "\"containerid\": \"" + DATA_VALUES_CONTAINER_ID + "\"" 
    "}" 
    "}]}]";

// ************************************************************************
// Gets the current time in the appropriate OMF format
// ************************************************************************

string OMFLib_getCurrentTimeString() {
    // Declar vars
    char timestampBuffer[80];
    time_t now;
    struct tm ts;
    
    // Get the current time
    time(&now);
    
    // Cast the current time into the correct format
    ts = *localtime(&now);
    strftime(timestampBuffer, sizeof(timestampBuffer), "%Y-%m-%dT%H:%M:%SZ", &ts);
    
    // Return the result
    return string(timestampBuffer);
}
                                        
int main()
{
    // Open a serial connection
    pc.baud(9600);
    
    // Connect to the network
    printf("\n\r----- Connecting to network -----\n\r");
    NetworkInterface* network = easy_connect(true);
    if (!network) {
        printf("Network connection failed!");
        return 1;
    }
    
    // Try to set the current time in UTC seconds:
    OMFLib_syncClockViaNTP(network);   
    wait(1);
        
    printf (
        "\n--- Setup: targeting endpoint \"%s\"...\n--- Now sending types, defining containers, and creating assets and links...\n--- (Note: a successful message will return a 20X response code.)\n", TARGET_URL
    );

    // ************************************************************************
    // Send the DYNAMIC types message, so that these types can be referenced in all later messages
    // ************************************************************************

    send_omf_message_to_endpoint(network, "create", "Type", DYNAMIC_TYPES_MESSAGE_JSON.c_str());

    // !!! Note: if sending data to OCS, static types are not included!
    if (!SEND_DATA_TO_OSISOFT_CLOUD_SERVICES) {

        // ************************************************************************
        // Send the STATIC types message, so that these types can be referenced in all later messages
        // ************************************************************************

        send_omf_message_to_endpoint(network, "create", "Type", STATIC_TYPES_MESSAGE_JSON.c_str());
    }

    // ************************************************************************
    // Send the container message, to instantiate this particular container;
    // we can now directly start sending data to it using its Id
    // ************************************************************************

    send_omf_message_to_endpoint(network, "create", "Container", CONTAINERS_MESSAGE_JSON.c_str());

    // !!! Note: if sending data to OCS, assets and links are not included!
    if (!SEND_DATA_TO_OSISOFT_CLOUD_SERVICES) {

        // ************************************************************************
        // Send the message to create the PI AF asset; it won"t appear in PI AF," + 
        // though, because it hasn't yet been positioned...
        // ************************************************************************

        send_omf_message_to_endpoint(network, "create", "Data", ASSETS_AND_LINKS_MESSAGE_JSON.c_str());

    }

    // ************************************************************************
    // Initialize sensors prior to sending data (if needed), using the function defined earlier
    // ************************************************************************

    initialize_sensors();

    // ************************************************************************
    // Finally, loop indefinitely, sending random events
    // conforming to the value type that we defined earlier
    // ************************************************************************

    printf (
        "\n--- Now sending live data every %i second(s) for device \"%s\"... (press CTRL+C to quit at any time)\n", NUMBER_OF_SECONDS_BETWEEN_VALUE_MESSAGES, NEW_AF_ELEMENT_NAME
    );
    if (!SEND_DATA_TO_OSISOFT_CLOUD_SERVICES) {
        printf (
            "--- (Look for a new AF Element named \"%s\".)\n", NEW_AF_ELEMENT_NAME
        );
    }
    while (true) {
        // ************************************************************************
        // Create a JSON message that contains sensor data values
        // ************************************************************************
        
        // The following code you can customize to allow this script to send along any
        // number of different data values, so long as the values that you send here match
        // up with the values defined in the dynamic OMF message type
        // In this example, this function simply generates two random values for the sensor values,
        // but here is where you could change this function to reference a library that actually
        // reads from sensors attached to the device that's running the script
        string VALUES_MESSAGE_JSON = ("[{" 
            "\"containerid\": \"" + DATA_VALUES_CONTAINER_ID + "\"," 
            "\"values\": [" 
            "{" 
            "\"Time\": \"" + OMFLib_getCurrentTimeString() + "\"," 
            // Again, in this example,
            // we're just sending along random values for these two \"sensors\"
            "\"Raw Sensor Reading 1\":" + OMFLib_float_to_string(100 * static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) + "," 
            "\"Raw Sensor Reading 2\":" + OMFLib_float_to_string(100 * static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) + "" 
            "}]}]");

        // Send the JSON message to the target URL
        send_omf_message_to_endpoint(network, "create", "Data", VALUES_MESSAGE_JSON.c_str());

        // Send the next message after the required interval
        wait(NUMBER_OF_SECONDS_BETWEEN_VALUE_MESSAGES);
    }
}

