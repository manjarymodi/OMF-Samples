#Copyright 2017 OSIsoft, LLC
#
#Licensed under the Apache License, Version 2.0 (the "License");
#you may not use this file except in compliance with the License.
#You may obtain a copy of the License at
#
#<http://www.apache.org/licenses/LICENSE-2.0>
#
#Unless required by applicable law or agreed to in writing, software
#distributed under the License is distributed on an "AS IS" BASIS,
#WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#See the License for the specific language governing permissions and
#limitations under the License.

# NOTE: this script was designed using the v1.0 version of the OMF specification, as outlined here:
# http://omf-docs.readthedocs.io/en/v1.0/index.html

# ************************************************************************
# Import necessary packages 
# ************************************************************************

# Import packages
import sys, json, random, time, platform, requests, socket, datetime
# Used to generate sample data; comment out this line if real data is used
import random

# Import any special packages needed for a particular hardware platform, for example, for a Raspberry PI,
# import RPi.GPIO as GPIO 

# ************************************************************************
# Specify constant values (names, target URLS, et centera) needed by the script
# ************************************************************************

# Specify the name of this device, or simply use the hostname; this is the name
# of the PI AF Element that will be created, and it'll be included in the names
# of PI Points that get created as well
device_name = (socket.gethostname())+ ""
#device_name = "MyCustomDeviceName"

# Specify a device location (optional); this will be added as a static string attribute to the AF Element that is created
device_location = "IoT Test Lab"

# Specify the name of the Assets type message; this will also end up becoming
# part of the name of the PI AF Element template that is created; for example, this could be
# "AssetsType_RaspberryPI" or "AssetsType_Dragonboard"
# You will want to make this different for each general class of IoT module that you use
assets_message_type_name = "assets_type" + ""
#assets_message_type_name = "assets_type" + "IoT Device Model 74656" # An example

# Similarly, specify the name of for the data values type; this should likewise be unique
# for each general class of IoT device--for example, if you were running this script on two different devices,
# each with different numbers and kinds of sensors, you'd specify a different data values message type name
# when running the script on each device.  If both devices were the same, you could use the same data_values_message_type_name
data_values_message_type_name = "data_values_type" + ""
#data_values_message_type_name = "data_values_type" + "IoT Device Model 74656" # An example


# Specify the number of seconds to sleep in between value messages
number_of_seconds_between_value_messages = 2

# Specify the address of the destination endpoint; it should be of the form http://<host/ip>:<port>/ingress/messages
# For example, "https://myservername:8118/ingress/messages"
relay_url = "http://localhost:8118/ingress/messages"

# Specify the producer token; this will be the parent AF element beneath which the new AF element will appear,
# and it will be part of the prefix of all PI Points that are created by this script
producer_token = "OMFv1"
#producer_token = "778408" # An example

# ************************************************************************
# Specify options for sending web requests to the target PI System
# ************************************************************************

# If self-signed certificates are used (true by default), do not verify HTTPS SSL certificates; normally, leave this as is
verify_SSL = False

# Specify the timeout, in seconds, for sending web requests (if it takes longer than this to send a message, an error will be thrown)
web_request_timeout_seconds = 30

# ************************************************************************
# Helper function: run any code needed to initialize local sensors, if necessary for this hardware
# ************************************************************************

# Below is where you can initialize any global variables that are needed by your applicatio;
# certain sensors, for example, will require global interface or sensor variables
# myExampleInterfaceKitGlobalVar = None

# The following function is where you can insert specific initialization code to set up
# sensors for a particular IoT module or platform
def initialize_sensors():
	print("\n--- Sensors initializing...")
	try:
		#For a raspberry pi, for example, to set up pins 4 and 5, you would add 
		#GPIO.setmode(GPIO.BCM)
		#GPIO.setup(4, GPIO.IN)
		#GPIO.setup(5, GPIO.IN)
		print("--- Sensors initialized!")
		# In short, in this example, by default, this function is called but doesn't do anything (it's just a placeholder)
	except Exception as e:
		# Log any error, if it occurs
		print(str(datetime.datetime.now()) + " An error has ocurred when initializing sensors: " + str(e))	

# ************************************************************************
# Helper function: REQUIRED: create a JSON message that contains sensor data values
# ************************************************************************

# The following function you can customize to allow this script to send along any
# number of different data values, so long as the values that you send here match
# up with the values defined in the "DataValuesType" OMF message type (see the next section)
# In this example, this function simply generates two random values for the sensor values,
# but here is where you could change this function to reference a library that actually
# reads from sensors attached to the device that's running the script

def create_data_values_stream_message(data_values_container_id):
    data_values_message_JSON = ''
    # Get the current timestamp in ISO format
    timestamp = datetime.datetime.utcnow().isoformat() + 'Z'
    # Assemble a JSON object containing the streamId and any data values
    data_values_message_JSON = [
        {
            "containerid": data_values_container_id,
            "values": [
                {
                    "Time": timestamp,
                    # Again, in this example, we're just sending along random values for these two "sensors"
                    "Raw Sensor Reading 1": 100*random.random(),
                    "Raw Sensor Reading 2": 100*random.random()
					# If you wanted to read, for example, the digital GPIO pins 4 and 5 on a Raspberry PI, 
					# you would add to the earlier package import section:
					# import RPi.GPIO as GPIO
					# then add the below 3 lines to the above initialize_sensors function to set up the GPIO pins:
					# GPIO.setmode(GPIO.BCM) 
					# GPIO.setup(4, GPIO.IN)
					# GPIO.setup(5, GPIO.IN)
					# and then lastly, you would change the two Raw Sensor reading lines above to
					# "Raw Sensor Reading 1": GPIO.input(4),
                    # "Raw Sensor Reading 2": GPIO.input(5)
                }
            ]
        }
    ]
    return data_values_message_JSON

# ************************************************************************
# Helper function: REQUIRED: wrapper function for sending an HTTPS message
# ************************************************************************

# Define a helper function to allow easily sending web request messages;
# this function can later be customized to allow you to port this script to other languages.
# All it does is take in a data object and a message type, and it sends an HTTPS
# request to the target OMF endpoint
def sendOMFMessageToEndPoint(action, message_type, message_JSON):
        try: 
                # Assemble headers that contain the producer token and message type
                # Note: in this example, the only action that is used is "create", which will work totally fine;
                # to expand this application, you could modify it to use the "update" action to, for example, modify existing AF element template types
                web_request_header = {'producertoken': producer_token, 'messagetype': message_type, 'action': action, 'messageformat': 'JSON', 'omfversion': '1.0' }
                # Send the request, and collect the response; json.dumps is used to properly format the message JSON so that it can be sent as a web request
                response = requests.post(relay_url, headers=web_request_header, data=json.dumps(message_JSON), verify=verify_SSL, timeout=web_request_timeout_seconds)
                # Print a debug message, if desired; note: you should receive a response code 202 if the request was successful!
                print('Response from relay from sending a message of type "{0}" with action "{1}": {2} {3}'.format(message_type, action, response.status_code, response.text))
        except Exception as e:
                # Log any error, if it occurs
                print(str(datetime.datetime.now()) + " An error ocurred during web request: " + str(e))		

# ************************************************************************
# Turn off HTTPS warnings, if desired (if the default certificate configuration was used by the PI Connector)
# ************************************************************************

# Suppress insecure HTTPS warnings, if an untrusted certificate is used by the target endpoint
# Remove if targetting trusted targets
if not verify_SSL:
    requests.packages.urllib3.disable_warnings()

print('\n--- Setup: targeting endpoint "' + relay_url + '"...\n--- Now sending types, defining containers, and creating assets and links...\n--- (Note: a successful message will return a response code "202".)\n')

# ************************************************************************
# Create a JSON packet to define the types of streams that will be sent
# ************************************************************************

types_message_JSON = [

    # ************************************************************************
    # There are several different message types that will be used by this script, but
    # you can customize this script for your own needs by modifying the types:
    # First, you can modify the "AssetsType", which will allow you to customize which static
    # attributes are added to the new PI AF Element that will be created, and second,
    # you can modify the "DataValuesType", which will allow you to customize this script to send
    # additional sensor values, in addition to (or instead of) the two shown here

    # This values type is going to be used to send real-time values; feel free to rename the
    # values from "Raw Sensor Reading 1" to, say, "Temperature", or "Pressure"
    {
        "id": data_values_message_type_name,
        "type": "object",
        "classification": "dynamic",
        "properties": {
            "Time": {
                "format": "date-time",
                "type": "string",
                "isindex": True
            },
            "Raw Sensor Reading 1": {
                "type": "number"
            },
            "Raw Sensor Reading 2": {
                "type": "number"
            }
            # For example, to allow you to send a string-type live data value, such as "Status", you would add
            #"Status": {
            #   "type": "string"
            #}
        }
    },
    # This asset type is used to define a PI AF Element that will be created;
    # this type also defines two static string attributes that will be created
    # as well; feel free to rename these or add additional
    # static attributes for each Element (PI Point attributes will be added later)
    # The name of this type will also end up being part of the name of the PI AF Element template
    # that is automatically created
    {
        "id": assets_message_type_name,
        "type": "object",
        "classification": "static",
        "properties": {
            "Name": {
                "type": "string",
                "isindex": True
            },
            "Device Type": {
                "type": "string"
            },
            "Location": {
                "type": "string"
            },
            "Data Ingress Method": {
                "type": "string"
            }
            # For example, to add a number-type static attribute for the device model, you would add
            #"Model": {
            #   "type": "number"
            #}
        }
    }
]

# ************************************************************************  
# Send the types message, so that these types can be referenced in all later messages
# ************************************************************************

sendOMFMessageToEndPoint("create", "Type", types_message_JSON)

# ************************************************************************
# Create a JSON packet to define containerIds and the type (using the types listed above) for each new data events container 
# ************************************************************************

# The device name that you specified earlier will be used as the AF Element name!
new_AF_element_name = device_name

# Create variables to store the Ids of the containers that will be used
data_values_container_id = (new_AF_element_name + "_data_values_container")
containers_message_JSON = [
    {
        "id": data_values_container_id,
        "typeid": data_values_message_type_name
    }
]

# ************************************************************************
# Send the container message, to instantiate this particular container; we can now directly start sending data to it using its Id
# ************************************************************************

sendOMFMessageToEndPoint("create", "Container", containers_message_JSON)

# ************************************************************************
# Create a JSON packet to containing the asset and linking data for the PI AF asset that will be made
# ************************************************************************

# Here is where you can specify values for the static PI AF attributes;
# in this case, we're auto-populating the Device Type, but you can manually hard-code in values if you wish
# we also add the LINKS to be made, which will both position the new PI AF
# Element, so it will show up in AF, and will associate the PI Points that will be created with that Element
assets_and_links_message_JSON = [
    {
        # This will end up creating a new PI AF Element with this specific name and static attribute values
        "typeid": assets_message_type_name,
        "values": [
            {
                "Name": new_AF_element_name,
                "Device Type": (platform.machine() + " - " + platform.platform() + " - " + platform.processor()),
                "Location": device_location,
                "Data Ingress Method": "OMF"
            }
        ]
    },
    {
        "typeid": "__Link",
        "values": [
            # This first link will locate such a newly created AF Element under the root PI element targeted by the PI Connector in your target AF database
            # This was specfied in the Connector Relay Admin page; note that a new parent element, with the same name as the producer_token, will also be made
                
            {
                "Source": {
                    "typeid": assets_message_type_name,
                    "index": "_ROOT"
                },
                "Target": {
                    "typeid": assets_message_type_name,
                    "index": new_AF_element_name
                }
            },
            # This second link will map new PI Points (created by messages sent to the data values container) to a newly create element
            {
                "Source": {
                    "typeid": assets_message_type_name,
                    "index": new_AF_element_name
                },
                "Target": {
                    "containerid": data_values_container_id
                }
            }
        ]
    }
]

# ************************************************************************
# Send the message to create the PI AF asset; it won't appear in PI AF, though, because it hasn't yet been positioned...
# ************************************************************************

sendOMFMessageToEndPoint("create", "Data", assets_and_links_message_JSON)

# ************************************************************************
# Initialize sensors prior to sending data (if needed), using the function defined earlier
# ************************************************************************

initialize_sensors()

# ************************************************************************
# Finally, loop indefinitely, sending random events conforming to the value type that we defined earlier
# ************************************************************************

print('\n--- Now sending live data every ' + str(number_of_seconds_between_value_messages) + ' second(s) for device "' + new_AF_element_name + '"... (press CTRL+C to quit at any time)')
print('--- (Look for a new AF Element named "' + new_AF_element_name + '" beneath the element "' + producer_token + '".)\n')
while True:
    # Call the custom function that builds a JSON object that contains new data values; see the beginning of this script
    values_message_JSON = create_data_values_stream_message(data_values_container_id)

    # Send the JSON message to the relay
    sendOMFMessageToEndPoint("create", "Data", values_message_JSON)

    # Send the next message after the required interval
    time.sleep(number_of_seconds_between_value_messages)
