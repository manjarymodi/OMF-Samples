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

// NOTE: THIS WAS DESIGNED TO RUN ON AN ARDUINO ESP 8266, SUCH AS AN ADAFRUIT FEATHER

// ************************************************************************
// Import necessary packages
// ************************************************************************

/* To load the board type,
 Enter 
http://arduino.esp8266.com/stable/package_esp8266com_index.json 
 into Additional Board Manager URLs field in the Arduino v1.6.4+ File > Preferences.
 Then select as the board type
 Adafruit HUZZAH ESP8266
*/

#include <ESP8266WiFi.h>
#include <NTPClient.h> // Download ZIP for this library from https://github.com/arduino-libraries/NTPClient
#include <WiFiUdp.h>
#include <Time.h>
#include <TimeLib.h>

// These are new third-party libraries used if you are connecting to local sensors and a display via I2C
// They can be omitted if you are just using a stock board
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_IS31FL3731.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <ESP8266HTTPClient.h>

// ************************************************************************
// Specify constant values (names, target URLS, etc.) needed by the script
// ************************************************************************

// Specify the name of this device, or simply use the hostname; this is the name
// of the PI AF Element that will be created, and it'll be included in the names
// of PI Points that get created as well
String DEVICE_NAME = "ARDUINO_ESP8266";

// Specify a device location (optional); this will be added as a static
// string attribute to the AF Element that is created
String DEVICE_LOCATION = "IoT Test Lab";

// Specify the name of the Assets type message; this will also end up becoming
// part of the name of the PI AF Element template that is created; for example, this could be
// "AssetsType_RaspberryPI" or "AssetsType_Dragonboard"
// You will want to make this different for each general class of IoT module that you use
String ASSETS_MESSAGE_TYPE_NAME = DEVICE_NAME + (String) "_assets_type";

// Similarly, specify the name of for the data values type; this should likewise be unique
// for each general class of IoT device--for example, if you were running this
// script on two different devices, each with different numbers and kinds of sensors,
// you'd specify a different data values message type name
// when running the script on each device.  If both devices were the same,
// you could use the same DATA_VALUES_MESSAGE_TYPE_NAME
String DATA_VALUES_MESSAGE_TYPE_NAME = DEVICE_NAME + (String) "data_values_type";

// Store the id of the container that will be used to receive live data values
String DATA_VALUES_CONTAINER_ID = (DEVICE_NAME + (String) "_data_values_container");

// Specify the number of seconds to sleep in between value messages
int NUMBER_OF_SECONDS_BETWEEN_VALUE_MESSAGES = 2;

// Specify whether you're sending data to OSIsoft cloud services or not
boolean SEND_DATA_TO_OSISOFT_CLOUD_SERVICES = false;

// Specify the address of the destination endpoint; it should be of the form
// http://<host/ip>:<port>/ingress/messages
// For example, "https://myservername:8118/ingress/messages"
String TARGET_URL = "https://lopezpiserver:777/ingress/messages";
// !!! Note: if sending data to OSIsoft cloud services,
// uncomment the below line in order to set the target URL to the OCS OMF endpoint:
//TARGET_URL = "https://qi-data.osisoft.com/api/omf"

// Specify the producer token; this will be the parent AF element
// beneath which the new AF element will appear,
// and it will be part of the prefix of all PI Points that are created by this script
String PRODUCER_TOKEN = "OMFv1";
//PRODUCER_TOKEN = "778408" // An example
// !!! Note: if sending data to OSIsoft cloud services, the producer token should be the
// security token obtained for a particular Tenant and Publisher; see
// http://qi-docs.readthedocs.io/en/latest/OMF_Ingress_Specification.html//headers
//PRODUCER_TOKEN = ""

// ************************************************************************
// Specify options for sending web requests to the target
// ************************************************************************

// If self-signed certificates are used (true by default),
// do not verify HTTPS SSL certificates; normally, leave this as is
boolean VERIFY_SSL = false;

// Specify the timeout, in seconds, for sending web requests
// (if it takes longer than this to send a message, an error will be thrown)
//int WEB_REQUEST_TIMEOUT_SECONDS = 30;

// ************************************************************************
// Other vars
// ************************************************************************

// Message to scroll on an LED display
String badgeMessage = "Ask me about OMF!";

// Specify the target network
const char* MyNetworkSSID           = "myssid";
const char* MyNetworkP              = "mynetworkpassword";

// Initialize the HTTP client
HTTPClient http;

// Specify the tumbprint of the SSL certificate used by the target endpoint
String SSL_CERT_THUMBPRINT = "70 58 69 93 A4 BA 79 6C 33 18 87 8D 5F G4 FB 1B 9C 06 AB 2F";

// Initialize NTP vars
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// ************************************************************************
// Helper function: run any code needed to initialize local sensors, if necessary for this hardware
// ************************************************************************

// Below is where you can initialize any global variables that are needed by your application;
// certain sensors, for example, will require global interface or sensor variables

// If you're using the full breakout...
//Adafruit_IS31FL3731 MyLEDMatrix = Adafruit_IS31FL3731();
// If you're using the FeatherWing version
Adafruit_IS31FL3731_Wing MyLEDMatrix = Adafruit_IS31FL3731_Wing(); // These are used if you are connecting to local sensors and a display via I2C; they can be omitted if you are just using a stock board

// Create a global var for the environment sensor
Adafruit_BME280 MyBME280EnvironmentSensor; // These are used if you are connecting to local sensors and a display via I2C; they can be omitted if you are just using a stock board

// The following function is where you can insert specific initialization code to set up
// sensors for a particular IoT module or platform
void initialize_sensors() {
  Serial.println("\n--- Sensors initializing...");
  // These are used if you are connecting to local sensors and a display via I2C;
  // they can be omitted if you are just using a stock board
  // Make sure the LED matrix is added
  if (! MyLEDMatrix.begin()) {
    Serial.println("IS31 LED matrix not found");
    while (1);
  }
  Serial.println("IS31 LED matrix found!");

  // Test that the environment sensor is working
  Serial.println(F("BME280 test..."));
  if (!MyBME280EnvironmentSensor.begin()) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  Serial.println("BME sensor found!");
  Serial.println("--- Sensors initialized!");
}

// ************************************************************************
// Helper function: REQUIRED: create a JSON message that contains sensor data values
// ************************************************************************

// The following function you can customize to allow this script to send along any
// number of different data values, so long as the values that you send here match
// up with the values defined in the "DataValuesType" OMF message type (see the next section)
// In this example, this function simply generates two random values for the sensor values,
// but here is where you could change this function to reference a library that actually
// reads from sensors attached to the device that's running the script

String makeTwoDigits(int digits) {
  if (digits < 10) {
    return (String)"0" + (String) digits;
  } else {
    return (String) digits;
  }
}

String create_data_values_message() {
  // Get the current timestamp in ISO format
   time_t t = timeClient.getEpochTime();
   String timestamp = (String)year(t) + (String) "-" + makeTwoDigits(month(t)) + (String)"-" + makeTwoDigits(day(t)) + (String)"T" + (String)timeClient.getFormattedTime() + (String)"Z";
  // Assemble a JSON object containing the streamId and any data values
  return (String)"[{" +
              (String)"\"containerid\": \"" + DATA_VALUES_CONTAINER_ID + (String)"\"," +
              (String)"\"values\": [{" +
                      (String)"\"Time\": \"" + timestamp + (String)"\"," +
                      (String)"\"Heartbeat (Random)\":" + 
                        (String) random(100) + 
                      // If you don't have these sensors, comment out these lines
                      (String)"," +
                      (String)"\"Temperature\":" + (String) (MyBME280EnvironmentSensor.readTemperature() * 9 / 5 + 32) + 
                      (String)"," +
                      (String)"\"Pressure\":" + (String) (MyBME280EnvironmentSensor.readPressure() / 100.0F) + 
                      (String)"," +
                      (String)"\"Humidity\":" + (String) (MyBME280EnvironmentSensor.readHumidity()) + 
                  (String)"}]}]";
}

// ************************************************************************
// Helper function: REQUIRED: wrapper function for sending an HTTPS message
// ************************************************************************

// Define a helper function to allow easily sending web request messages;
// this function can later be customized to allow you to port this script to other languages.
// All it does is take in a data object and a message type, and it sends an HTTPS
// request to the target OMF endpoint
void send_omf_message_to_endpoint(String action, String message_type, String message_json) {
  // !!! Note: if desired, uncomment the below line to Serial.println the outgoing message
  Serial.println("\nOutgoing message: " + message_json);
      
  if(WiFi.status()== WL_CONNECTED) { //Check WiFi connection status
  
    // Create a connection object
    http.begin(TARGET_URL, SSL_CERT_THUMBPRINT);
  
    // Assemble headers that contain the producer token and message type
    // Note: in this example, the only action that is used is \"create\",
    // which will work totally fine;
    // to expand this application, you could modify it to use the \"update\"
    // action to, for example, modify existing AF element template types
    http.addHeader("Content-Type", "application/json");
    http.addHeader("producertoken", PRODUCER_TOKEN);
    http.addHeader("messagetype", message_type);
    http.addHeader("action", action);
    http.addHeader("messageformat", "JSON");
    http.addHeader("omfversion", "1.0");
  
    // Send the request
    Serial.print("Http result:");
    Serial.println(http.POST(message_json));
    
    // Close the connection
    http.end();
  } else {
    Serial.print("Error sending message: Wi-Fi not connected!");
  }
}

// ************************************************************************
// Setup function; runs before the main loop  
// ************************************************************************  
void setup() {
  delay(2000);
  
  // Start the serial monitor
  Serial.begin(9600);
  Serial.println("");
  Serial.println("Smart badge program...");

  // ************************************************************************
  // Initialize sensors prior to sending data (if needed), using the function defined earlier
  // ************************************************************************
  initialize_sensors();

  // Config static IP, if desired, before running WiFi.begin
  /*
  IPAddress ip(172,22,2,200);
  IPAddress gateway(172, 22, 0, 1);
  IPAddress subnet(255, 255, 255, 0); 
  Serial.print(F("Setting static IP to : "));
  Serial.println(ip);
  WiFi.config(ip, gateway, subnet);
  */
  
  // Connect to Wi-Fi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(MyNetworkSSID);
  WiFi.begin(MyNetworkSSID, MyNetworkP);
  delay(500);
  
  // Wait until connection is established...
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected!");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Sync time!
  Serial.println("Syncing time!");
  timeClient.begin();
  timeClient.update();
  Serial.println(timeClient.getFormattedTime());
  
  // Continue with the main program
  Serial.println(
      "\n--- Setup: targeting endpoint \"" + TARGET_URL + "\"..." +
      "\n--- Now sending types, defining containers, and creating assets and links..." +
      "\n--- (Note: a successful message will return a response code \"202 or 200\".)\n"
  );

  // ************************************************************************
  // Create a JSON packet to define the types of streams that will be sent
  // ************************************************************************

  String DYNAMIC_TYPES_MESSAGE_JSON = 

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

           (String)"[{" + 
               (String)"\"id\": \"" + DATA_VALUES_MESSAGE_TYPE_NAME + (String)"\"," + 
               (String)"\"type\": \"object\"," + 
               (String)"\"classification\": \"dynamic\"," + 
               (String)"\"properties\": {" +
                   (String)"\"Time\": {" +
                      (String)"\"format\": \"date-time\"," + 
                       (String)"\"type\": \"string\"," + 
                       (String)"\"isindex\": true" +
                   (String)"}," + 
                   (String)"\"Heartbeat (Random)\": {" +
                       (String)"\"type\": \"number\"" +
                   // If you don't have the below sensors, comment out those lines
                   (String)"}," + 
                   (String)"\"Temperature\": {" +
                       (String)"\"type\": \"number\"" +
                   (String)"}," + 
                   (String)"\"Pressure\": {" +
                       (String)"\"type\": \"number\"" +
                   (String)"}," +                    
                   (String)"\"Humidity\": {" +
                       (String)"\"type\": \"number\"" +
                   (String)"}" +                   
                   // For example, to allow you to send a string-type live data value," + 
                   // such as \"Status\", you would add
                   //\"Status\": {
                   //   \"type\": \"string\"
                   //}
               (String)"}}]";

  // ************************************************************************
  // Send the DYNAMIC types message, so that these types can be referenced in all later messages
  // ************************************************************************

  send_omf_message_to_endpoint("create", "Type", DYNAMIC_TYPES_MESSAGE_JSON);
  
  // !!! Note: if sending data to OCS, types are not included!
  if (!SEND_DATA_TO_OSISOFT_CLOUD_SERVICES) {
      String STATIC_TYPES_MESSAGE_JSON = 
          // This asset type is used to define a PI AF Element that will be created;
          // this type also defines two string attributes that will be created
          // as well; feel free to rename these or add additional
          // attributes for each Element (PI Point attributes will be added later);
          // The name of this type will also end up being part of the name of the PI AF Element template
          // that is automatically created
          (String)"[{" +
              (String)"\"id\": \"" + ASSETS_MESSAGE_TYPE_NAME + (String)"\"," + 
              (String)"\"type\": \"object\"," + 
              (String)"\"classification\": \"static\"," + 
              (String)"\"properties\": {" +
                  (String)"\"Name\": {" +
                      (String)"\"type\": \"string\"," + 
                      (String)"\"isindex\": true" +
                  (String)"}," + 
                  (String)"\"Device Type\": {" +
                      (String)"\"type\": \"string\"" +
                  (String)"}," + 
                  (String)"\"Location\": {" +
                      (String)"\"type\": \"string\"" +
                  (String)"}," + 
                  (String)"\"Data Ingress Method\": {" +
                      "\"type\": \"string\"" +
                  (String)"}" +
                  // For example, to add a number-type static
                  // attribute for the device model, you would add
                  // \"Model\": {
                  //   \"type\": \"number\"
                  //}
              (String)"}" +
          (String)"}]";

      // ************************************************************************
      // Send the types message, so that these types can be referenced in all later messages
      // ************************************************************************

      send_omf_message_to_endpoint("create", "Type", STATIC_TYPES_MESSAGE_JSON);
  }
  
  // ************************************************************************
  // Create a JSON packet to define container IDs and the type
  // (using the types listed above) for each new data events container
  // ************************************************************************

  // The device name that you specified earlier will be used as the AF Element name!
  String NEW_AF_ELEMENT_NAME = DEVICE_NAME;

  String CONTAINERS_MESSAGE_JSON = (String)"[{" +
          (String)"\"id\": \"" + DATA_VALUES_CONTAINER_ID + (String)"\"," + 
          (String)"\"typeid\": \"" + DATA_VALUES_MESSAGE_TYPE_NAME + (String)"\"" +
      (String)"}]";
  
  // ************************************************************************
  // Send the container message, to instantiate this particular container;
  // we can now directly start sending data to it using its Id
  // ************************************************************************

  send_omf_message_to_endpoint("create", "Container", CONTAINERS_MESSAGE_JSON);

  // !!! Note: if sending data to OCS, assets and links are not included!
  if (!SEND_DATA_TO_OSISOFT_CLOUD_SERVICES) {
  
    // ************************************************************************
      // Create a JSON packet to containing the asset and
      // linking data for the PI AF asset that will be made
      // ************************************************************************

      // Here is where you can specify values for the PI AF attributes;
      // in this case, we"re auto-populating the Device Type," + 
      // but you can manually hard-code in values if you wish
      // we also add the LINKS to be made, which will both position the new PI AF
      // Element, so it will show up in AF, and will associate the PI Points
      // that will be created with that Element
      String ASSETS_AND_LINKS_MESSAGE_JSON = (String)"[{" +
              // This will end up creating a new PI AF Element with
              // this specific name and attribute values
              (String)"\"typeid\": \"" + ASSETS_MESSAGE_TYPE_NAME + (String)"\"," + 
              (String)"\"values\": [{" +
                      (String)"\"Name\":\"" + NEW_AF_ELEMENT_NAME + (String)"\"," + 
                      (String)"\"Device Type\": \"" + "Type74656" + (String)"\"," + 
                      (String)"\"Location\": \"" + DEVICE_LOCATION + (String)"\"," + 
                      (String)"\"Data Ingress Method\": \"OMF\"" +
              (String)"}]},{" +
              (String)"\"typeid\": \"__Link\"," + 
              (String)"\"values\": [{" +
                  // This first link will locate such a newly created AF Element under
                  // the root PI element targeted by the PI Connector in your target AF database
                  // This was specified in the Connector Relay Admin page; note that a new
                  // parent element, with the same name as the PRODUCER_TOKEN, will also be made
                      (String)"\"Source\": {" +
                          (String)"\"typeid\": \"" + ASSETS_MESSAGE_TYPE_NAME + (String)"\"," + 
                          (String)"\"index\": \"_ROOT\"" +
                      (String)"}," + 
                      (String)"\"Target\": {" +
                          (String)"\"typeid\": \"" + ASSETS_MESSAGE_TYPE_NAME + (String)"\"," + 
                          (String)"\"index\": \"" + NEW_AF_ELEMENT_NAME + (String)"\"" +
                  (String)"}},{" + 
                  // This second link will map new PI Points (created by messages
                  // sent to the data values container) to a newly create element
                      (String)"\"Source\": {" +
                          (String)"\"typeid\": \"" + ASSETS_MESSAGE_TYPE_NAME + (String)"\"," + 
                          (String)"\"index\":\"" + NEW_AF_ELEMENT_NAME + (String)"\"" +
                      (String)"}," + 
                      (String)"\"Target\": {" +
                          (String)"\"containerid\": \"" + DATA_VALUES_CONTAINER_ID + (String)"\"" +
                      (String)"}" +
                  (String)"}]}]";

      // ************************************************************************
      // Send the message to create the PI AF asset; it won"t appear in PI AF," + 
      // though, because it hasn't yet been positioned...
      // ************************************************************************

      send_omf_message_to_endpoint("create", "Data", ASSETS_AND_LINKS_MESSAGE_JSON);

    }
  
    // ************************************************************************
    // Finally, loop indefinitely, sending random events
    // conforming to the value type that we defined earlier
    // ************************************************************************

    Serial.println(
        (String) "\n--- Now sending live data every " + (NUMBER_OF_SECONDS_BETWEEN_VALUE_MESSAGES) +
        (String) " second(s) for device \"" + NEW_AF_ELEMENT_NAME + (String) "\"... (press CTRL+C to quit at any time)\n"
    );
    if (!SEND_DATA_TO_OSISOFT_CLOUD_SERVICES) {
        Serial.println(
            (String) "--- (Look for a new AF Element named \"" + NEW_AF_ELEMENT_NAME + (String) "\".)\n"
        );
    }
}

// ************************************************************************
// Main loop
// ************************************************************************
void loop() {

  // Comment out these lines if you aren't using an attached LED matrix display
  Serial.println("Now scrolling text...");
  // Initialize the LED matrix to not rotate; clear it, set text size, wrap, and color
  MyLEDMatrix.setRotation(0);
  MyLEDMatrix.clear();
  MyLEDMatrix.setTextSize(1);
  MyLEDMatrix.setTextWrap(false);  
  MyLEDMatrix.setTextColor(10);
  // Start printing out the message, advancing the cursor one pixel at a time to get a scroll effect
  for (int8_t x=0; x>=-120; x--) {
    MyLEDMatrix.clear();
    MyLEDMatrix.setCursor(x,0);
    MyLEDMatrix.print(badgeMessage);
    delay(50);
  }

  // Call the custom function that builds a JSON object that
  // contains new data values; see the beginning of this script
  String VALUES_MESSAGE_JSON = create_data_values_message();

  // Send the JSON message to the target URL
  send_omf_message_to_endpoint("create", "Data", VALUES_MESSAGE_JSON);

  // Send the next message after the required interval
  delay(NUMBER_OF_SECONDS_BETWEEN_VALUE_MESSAGES * 1000);
}

