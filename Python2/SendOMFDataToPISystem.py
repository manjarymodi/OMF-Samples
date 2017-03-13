#Copyright 2016 OSIsoft, LLC
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

# ************************************************************************

# Import packages
import sys, json, random, time, platform, requests, socket, datetime
# Used to generate sample data; comment out this line if real data is used
import random

# Import any special packages needed for a particular hardware platform, for example, for a Raspberry PI,
# import RPi.GPIO as GPIO 

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

# Specify the number of seconds to sleep in between value messages
number_of_seconds_between_value_messages = 1

# Specify the address of the destination endpoint; it should be of the form http://<host/ip>:<port>/ingress/messages
# For example, "https://myservername:777/ingress/messages"
relay_url = "https://localhost:777/ingress/messages"

# Specify the producer token; this will be the parent AF element beneath which the new AF element will appear,
# and it will be part of the prefix of all PI Points that are created by this script
producer_token = "OMF" + ""

# If self-signed certificates are used (true by default), do not verify HTTPS SSL certificates
verify_SSL = False

# ************************************************************************
# Below is where you can initialize any global variables that are needed by your applicatio;
# certain sensors, for example, will require global interface or sensor variables
# myExampleInterfaceKitGlobalVar = None

# The following function is where you can insert specific initialization code to set up
# sensors for a particular IoT module or platform
def initialize_sensors():
	print("--- Sensors initializing...")
	try:
		#For a raspberry pi, for example, to set up pins 4 and 5, you would add 
		#GPIO.setmode(GPIO.BCM)
		#GPIO.setup(4, GPIO.IN)
		#GPIO.setup(5, GPIO.IN)
		print("--- Sensors initialized!")	
	except Exception as e:
		# Log any error, if it occurs
		print(str(datetime.datetime.now()) + " An error has ocurred when initializing sensors: " + str(e))	

# ************************************************************************
# The following function you can customize to allow this script to send along any
# number of different data values, so long as the values that you send here match
# up with the values defined in the "DataValuesType" OMF message type (see the next section)
# In this example, this function simply generates two random values for the sensor values,
# but here is where you could change this function to reference a library that actually
# reads from sensors attached to the device that's running the script

def create_data_values_stream_message(target_container_id):
    data_values_JSON = ''
    # Get the current timestamp
    timestamp = datetime.datetime.utcnow().isoformat() + 'Z'
    # Assemble a JSON object containing the streamId and any data values
    data_values_JSON = [
        {
            "containerid": target_container_id,
            "values": [
                {
                    "Time": timestamp,
                    # Again, in this example, we're just sending along random values for these two "sensors"
                    "Raw Sensor Reading 1": 100*random.random(),
                    "Raw Sensor Reading 2": 100*random.random()
			# If you wanted to read, for example, the digital GPIO pins 4 and 5 on a Raspberry PI, 
			# you would add to the earlier package import section:
			# 		import RPi.GPIO as GPIO
			# then add the below 3 lines to the above initialize_sensors function to set up the GPIO pins:
			# 		GPIO.setmode(GPIO.BCM) 
			# 		GPIO.setup(4, GPIO.IN)
			# 		GPIO.setup(5, GPIO.IN)
			# and then lastly, you would change the two Raw Sensor reading lines above to
			#		"Raw Sensor Reading 1": GPIO.input(4),
                        #       	"Raw Sensor Reading 2": GPIO.input(5)
                }
            ]
        }
    ]
    return data_values_JSON

# ************************************************************************

# Define a helper function to allow easily sending web request messages;
# this function can later be customized to allow you to port this script to other languages.
# All it does is take in a data object and a message type, and it sends an HTTPS
# request to the target OMF endpoint
def sendOMFMessageToEndPoint(message_type, OMF_data):
        try: 
                # Assemble headers that contain the producer token and message type
                msg_header = {'producertoken': producer_token, 'messagetype': message_type, 'action': 'create', 'messageformat': 'JSON', 'omfversion': '1.0' }
                # Send the request, and collect the response
                response = requests.post(relay_url, headers=msg_header, data=json.dumps(OMF_data), verify=verify_SSL, timeout=30)
                # Print a debug message, if desired
                print('Response from relay from the initial "{0}" message: {1} {2}'.format(message_type, response.status_code, response.text))
        except Exception as e:
                # Log any error, if it occurs
                print(str(datetime.datetime.now()) + " An error ocurred during web request: " + str(e))		

# ************************************************************************

# Create a JSON packet to define the types of streams that will be sent
types = [

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
        "id": "my_data_values_type",
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

# Suppress insecure HTTPS warnings, if an untrusted certificate is used by the target endpoint
# Remove if targetting trusted targets
if not verify_SSL:
    requests.packages.urllib3.disable_warnings()

print('--- Setup: targeting endpoint "' + relay_url + '"...\n    Now sending types, defining containers, and creating assets and links...')
    
# Send the types message, so that these types can be referenced in all later messages
sendOMFMessageToEndPoint("Type", types)

# ************************************************************************

# Create a JSON packet to define containerIds and the type (using the types listed above) for each new stream

# Create variables to store the Ids of the containers that will be used
container_id_for_sending_values = (device_name + "_data_values_stream")
containers = [
    {
        "id": container_id_for_sending_values,
        "typeid": "my_data_values_type"
    }
]

# Send the streams message, to instantiate streams; we can now directly start sending data to each stream
sendOMFMessageToEndPoint("Container", containers)

# ************************************************************************

# Create a JSON packet to containing the data for the PI AF asset that will be made;
# Here is where you can specify values for the static PI AF attributes;
# in this case, we're auto-populating the Device Type, but you can manually hard-code in values if you wish
# we also add the LINKS to be made, which will both position the new PI AF
# Element, so it will show up in AF, and will associate the PI Points that will be created with that Element
assets_and_links = [
    {
        "typeid": assets_message_type_name,
        "values": [
            {
                "Name": device_name,
                "Device Type": (platform.machine() + " - " + platform.platform() + " - " + platform.processor()),
                "Location": device_location,
                "Data Ingress Method": "OMF"
            }
        ]
    },
    {
        "typeid": "__Link",
        "values": [
            {
                "Source": {
                    "typeid": assets_message_type_name,
                    "index": "_ROOT"
                    },
                "Target": {
                    "typeid": assets_message_type_name,
                    "index": device_name
                    }
            },
            {
                "Source": {
                    "typeid": assets_message_type_name,
                    "index": device_name
                    },
                "Target": {
                    "containerid": container_id_for_sending_values
                    }
            }
        ]
    }
]

# Send the message to create the PI AF asset; it won't appear in PI AF, though, because it hasn't yet been positioned...
sendOMFMessageToEndPoint("Data", assets_and_links)
# ************************************************************************

# Initialize sensors prior to sending data (if needed), using the function defined earlier
initialize_sensors()

# Loop indefinitely, sending random events conforming to the value type that we defined earlier
print('--- Now sending live data every ' + str(number_of_seconds_between_value_messages) + ' second(s) for device "' + device_name + '"...')
while True:
    # Call the custom function that builds a JSON object that contains new data values; see the beginning of this script
    values = create_data_values_stream_message(container_id_for_sending_values)
    # Send the data to the relay; it will be stored in a point called <producer_token>.<streamId>
    sendOMFMessageToEndPoint("Data", values)
 
    # Send the next message after the required interval
    time.sleep(number_of_seconds_between_value_messages)
