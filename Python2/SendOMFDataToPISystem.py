# ***********************************************************************
# * DISCLAIMER: 
# * All sample code is provided by OSIsoft for illustrative purposes only.
# * These examples have not been thoroughly tested under all conditions.
# * OSIsoft provides no guarantee nor implies any reliability, 
# * serviceability, or function of these programs.
# * ALL PROGRAMS CONTAINED HEREIN ARE PROVIDED TO YOU "AS IS" 
# * WITHOUT ANY WARRANTIES OF ANY KIND. ALL WARRANTIES INCLUDING 
# * THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY
# * AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY DISCLAIMED.
# ************************************************************************

# Import packages
import sys, json, random, time, platform, requests, socket, datetime
# Used to generate sample data; comment out this line if real data is used
import random

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
# The following function you can customize to allow this script to send along any
# number of different data values, so long as the values that you send here match
# up with the values defined in the "DataValuesType" OMF message type (see the next section)
# In this example, this function simply generates two random values for the sensor values,
# but here is where you could change this function to reference a library that actually
# reads from sensors attached to the device that's running the script

def create_data_values_stream_message(target_stream_id):
    data_values_JSON = ''
    # Get the current timestamp
    timestamp = datetime.datetime.utcnow().isoformat() + 'Z'
    # Assemble a JSON object containing the streamId and any data values
    data_values_JSON = [
        {
            "streamId": target_stream_id,
            "values": [
                {
                    "Time": timestamp,
                    # Again, in this example, we're just sending along random values for these two "sensors"
                    "Raw Sensor Reading 1": 100*random.random(),
                    "Raw Sensor Reading 2": 100*random.random()
					# If you wanted to read, for example, the digital GPIO pins 4 and 5 on a Raspberry PI, 
					# you would add to the earlier package import section:
					# 		import RPi.GPIO as GPIO
					# then add the below 3 lines to set up the GPIO pins:
					# 		GPIO.setmode(GPIO.BCM) 
					# 		GPIO.setup(4, GPIO.IN)
					# 		GPIO.setup(5, GPIO.IN)
					# and then lastly, you would change the two Raw Sensor reading lines above to
					#		"Raw Sensor Reading 1": GPIO.input(4),
                    #		"Raw Sensor Reading 2": GPIO.input(5)
                }
            ]
        }
    ]
    return data_values_JSON

# ************************************************************************

# Create a JSON packet to define the types of streams that will be sent
types = [

    # ************************************************************************
    # There are several different message types that will be used by this script, but
    # you can customize this script for your own needs by only modifying two of those types:
    # First, you can modify the "AssetsType", which will allow you to customize which static
    # attributes are added to the new PI AF Element that will be created, and second,
    # you can modify the "DataValuesType", which will allow you to customize this script to send
    # additional sensor values, in addition to (or instead of) the two shown here

    # This values type is going to be used to send real-time values; feel free to rename the
    # values from "Raw Sensor Reading 1" to, say, "Temperature", or "Pressure"
    {
        "id": "my_data_values_type",
        "type": "object",
        "properties": {
            "Time": {
                "format": "date-time",
                "type": "string",
                "index": True
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
        "classification": "asset",
        "properties": {
            "Name": {
                "type": "string",
                "index": True
            },
            "Device Type": {
                "type": "string"
            },
            "Location": {
                "type": "string"
            }
            # For example, to add a number-type static attribute for the device model, you would add
            #"Model": {
            #   "type": "number"
            #}
        }
    },
    # ************************************************************************
    # The below types are standard ones that will be used regardless of the device
    # you're using or the number of data items; thus, to customize this script for your own
    # needs, you only need to modify the above two types 
	
    # This link type is used to instruct the relay where to locate each new PI AF
    # Element in the target PI AF database; you shouldn't need to modify it
    {
        "id": "my_asset_asset_links_type",
        "type": "object",
        "classification": "link",
        "properties": {
            # The parent PI AF Element
            "Source": {
                "type": "string",
                "LinkSchema": assets_message_type_name
            },
            # The new PI AF Element that will appear beneath it
            "Target": {
                "type": "string",
                "LinkSchema": assets_message_type_name
            }
        }
    },
    # This link type is used to map live PI Points from the values stream to the new
    # PI AF Element that will be created
    {
        "id": "my_asset_values_links_type",
        "type": "object",
        "classification": "link",
        "properties": {
            # The target PI AF Element
            "Source": {
                "type": "string",
                "LinkSchema": assets_message_type_name
            },
            # The stream of values that should be associated with this Element
            "Target": {
                "type": "string",
                "LinkSchema": "my_data_values_type"
            }
        }
    }
]

# Suppress insecure HTTPS warnings, if an untrusted certificate is used by the target endpoint
# Remove if targetting trusted targets
if not verify_SSL:
    requests.packages.urllib3.disable_warnings()

print('--- Setup: targeting endpoint "' + relay_url + '"...\n    Now sending types, defining streams, and creating assets and links...')
    
# Send the types message, so that these types can be referenced in all later messages
msg_header = {'producertoken': producer_token, 'messagetype': 'Types', 'action': 'create', 'messageformat': 'JSON'}
response = requests.post(relay_url, headers=msg_header, data=json.dumps(types), verify=verify_SSL)
print('Response from relay from the initial TYPES message: {0} {1}'.format(response.status_code, response.text))

# ************************************************************************

# Create a JSON packet to define streamIds and the type (using the types listed above) for each new stream

# Create variables to store the Ids of the streams that will be used
stream_id_for_creating_assets = (device_name + "_asset_creation_stream")
stream_id_for_creating_asset_asset_links = (device_name + "_asset_asset_link_creation_stream")
stream_id_for_creating_asset_values_links = (device_name + "_asset_values_linking_stream")
stream_id_for_sending_values = (device_name + "_data_values_stream")
streams = [
    {
        "streamId": stream_id_for_creating_assets,
        "typeId": assets_message_type_name
    },
    {
        "streamId": stream_id_for_creating_asset_asset_links,
        "typeId": "my_asset_asset_links_type"
    },
    {
        "streamId": stream_id_for_creating_asset_values_links,
        "typeId": "my_asset_values_links_type"
    },
    {
        "streamId": stream_id_for_sending_values,
        "typeId": "my_data_values_type"
    }
]

# Send the streams message, to instantiate streams; we can now directly start sending data to each stream
msg_header = {'producertoken': producer_token, 'messagetype': 'Streams', 'action': 'create', 'messageformat': 'JSON'}
response = requests.post(relay_url, headers=msg_header, data=json.dumps(streams), verify=verify_SSL)
print('Response from relay from the initial STREAMS message: {0} {1}'.format(response.status_code, response.text))

# ************************************************************************

# Create a JSON packet to containing the data for the PI AF asset that will be made;
# for the streamId, use the name of the asset creation stream ID from above
# Here is where you can specify values for the static PI AF attributes;
# in this case, we're auto-populating the Device Type, but you can manually hard-code in values if you wish
assets = [
    {
        "streamId": stream_id_for_creating_assets,
        "values": [
            {
                "Name": device_name,
                "Device Type": (platform.machine() + " - " + platform.platform() + " - " + platform.processor()), "Location": device_location
            }
        ]
    }
]

# Send the message to create the PI AF asset; it won't appear in PI AF, though, because it hasn't yet been positioned...
msg_header = {'producertoken': producer_token, 'messagetype': 'Values', 'messageformat': 'JSON'}
response = requests.post(relay_url, headers=msg_header, data=json.dumps(assets), verify=verify_SSL, timeout=60)
print('Response from relay from the initial ASSETS message: {0} {1}'.format(response.status_code, response.text))

# ************************************************************************

# Create a JSON packet to containing the LINKS to be made, which will both position the new PI AF
# Element, so it will show up in AF, and will associate the PI Points that will be created with that Element
# Again, for the stream IDs, use the stream IDs sent earlier
links = [
    {
        "streamId": stream_id_for_creating_asset_asset_links,
        "values": [
            {
                "Source": "_ROOT",
                "Target": device_name
            }
        ]
    },
    {
        "streamId": stream_id_for_creating_asset_values_links,
        "values": [
            {
                "Source": device_name,
                "Target": stream_id_for_sending_values
            }
        ]
    },
]

# Send the message to create the links, which will finish making the AF element and will set up links for PI POints
msg_header = {'producertoken': producer_token, 'messagetype': 'Values', 'messageformat': 'JSON'}
response = requests.post(relay_url, headers=msg_header, data=json.dumps(links), verify=verify_SSL, timeout=60)
print('Response from relay from the initial LINKS message: {0} {1}'.format(response.status_code, response.text))

# ************************************************************************

# Loop indefinitely, sending random events conforming to the value type that we defined earlier
print('--- Now sending live data every ' + str(number_of_seconds_between_value_messages) + ' second(s) for device "' + device_name + '"...')
while True:
    # Call the custom function that builds a JSON object that contains new data values; see the beginning of this script
    values = create_data_values_stream_message(stream_id_for_sending_values)
    # Convert the data into bytes prior to sending it
    json_values = json.dumps(values)
    # Send the data to the relay; it will be stored in a point called <producer_token>.<streamId>
    try:
        response = requests.post(relay_url, headers=msg_header, data=json_values, verify=verify_SSL, timeout=60)
    except Exception as e:
        # Log any error, if it occurs
        print(str(datetime.datetime.now()) + " An error has ocurred: " + str(e))		
    # Comment out this line if you want to disable printing the response with each message request
    print('Response from relay from values message: {0} {1}'.format(response.status_code, response.text))

    # Send the next message after the required interval
    time.sleep(number_of_seconds_between_value_messages)
