#*************************************************************************************
# Copyright 2018 OSIsoft, LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# <http://www.apache.org/licenses/LICENSE-2.0>
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# NOTE: this script was designed using the v1.0
# version of the OMF specification, as outlined here:
# http://omf-docs.osisoft.com/en/v1.0
#*************************************************************************************

# ************************************************************************
# Import necessary packages
# ************************************************************************

import json
import time
import datetime
import platform
import socket
import gzip
import random # Used to generate sample data; comment out this line if real data is used
import requests

# ************************************************************************
# Specify options for sending web requests to the target PI System
# ************************************************************************

# Specify the producer token
# For more information see PI Connector Administration Guide
PRODUCER_TOKEN = "uid=3f4249a9-37e8-4041-89cf-ebda7d7d2797&sig=c5NE3tL93Or18eeSvc9Fwh0EZ+I6HLt9OTaq2t7zrfQ="

# Specify the address of the destination endpoint
# For more information see PI Connector Administration Guide
INGRESS_URL = "http://localhost:8118/ingress/messages"

# Specify whether to compress OMF message before
# sending it to ingress endpoint
USE_COMPRESSION = True

# If self-signed certificates are used (true by default),
# do not verify HTTPS SSL certificates; normally, leave this as is
VERIFY_SSL = False

# Specify the timeout, in seconds, for sending web requests
# (if it takes longer than this to send a message, an error will be thrown)
WEB_REQUEST_TIMEOUT_SECONDS = 30



# ************************************************************************
# Helper function: REQUIRED: wrapper function for sending an HTTPS message
# ************************************************************************

# Define a helper function to allow easily sending web request messages;
# this function can later be customized to allow you to port this script to other languages.
# All it does is take in a data object and a message type, and it sends an HTTPS
# request to the target OMF endpoint
def send_omf_message_to_endpoint(message_type, message_omf_json):
    try:
        # Compress json omf payload, if specified
        compression = 'none'
        if USE_COMPRESSION:
            msg_body = gzip.compress(bytes(json.dumps(message_omf_json), 'utf-8'))
            compression = 'gzip'
        else:
            msg_body = json.dumps(message_omf_json)
        # Assemble headers
        msg_headers = {
            'producertoken': PRODUCER_TOKEN,
            'messagetype': message_type,
            'action': 'create',
            'messageformat': 'JSON',
            'omfversion': '1.0',
            'compression': compression
        }
        # Send the request, and collect the response
        response = requests.post(
            INGRESS_URL,
            headers = msg_headers,
            data = msg_body,
            verify = VERIFY_SSL,
            timeout = WEB_REQUEST_TIMEOUT_SECONDS
        )
        # Print a debug message, if desired; note: you should receive a
        # response code 204 if the request was successful!
        print('Response from relay from the initial "{0}" message: {1} {2}'.format(message_type, response.status_code, response.text))
    
    except Exception as e:
        # Log any error, if it occurs
        print(str(datetime.datetime.now()) + " An error ocurred during web request: " + str(e))		

# ************************************************************************
# Turn off HTTPS warnings, if desired
# (if the default certificate configuration was used by the PI Connector)
# ************************************************************************

# Suppress insecure HTTPS warnings, if an untrusted certificate is used by the target endpoint
# Remove if targetting trusted targets
if not VERIFY_SSL:
    requests.packages.urllib3.disable_warnings()

# ************************************************************************
# Send the types messages to define the types of streams that will be sent.
# These types are referenced in all later messages
# ************************************************************************

# The sample divides types, and sends static and dynamic types
# separatly only for readability; you can send all the type definitions
# in one message, as far as its size is below maximum allowed - 192K
# ************************************************************************

# Send a JSON packet to define static types
send_omf_message_to_endpoint("type", [
    {
        "id": "FirstStaticType",
        "name": "First static type",
        "classification": "static",
        "type": "object",
        "description": "First static asset type",
        "properties": {
            "index": {
                "type": "string",
                "isindex": True,
                "name": "not in use",
                "description": "not in use"
            },
            "name": {
                "type": "string",
                "isname": True,
                "name": "not in use",
                "description": "not in use"
            },
            "StringProperty": {
                "type": "string",
                "name": "First configuration attribute",
                "description": "First static asset type's configuration attribute"
            }
        }
    },
    {
        "id": "SecondStaticType",
        "name": "Second static type",
        "classification": "static",
        "type": "object",
        "description": "Second static asset type",
        "properties": {
            "index": {
                "type": "string",
                "isindex": True,
                "name": "not in use",
                "description": "not in use"
            },
            "name": {
                "type": "string",
                "isname": True,
                "name": "not in use",
                "description": "not in use"
            },
            "StringProperty": {
                "type": "string",
                "name": "Second configuration attribute",
                "description": "Second static asset type's configuration attribute"
            }
        }
    }
])

# Send a JSON packet to define dynamic types
send_omf_message_to_endpoint("type", [
    {
        "id": "FirstDynamicType",
        "name": "First dynamic type",
        "classification": "dynamic",
        "type": "object",
        "description": "not in use",
        "properties": {
            "timestamp": {
                "format": "date-time",
                "type": "string",
                "isindex": True,
                "name": "not in use",
                "description": "not in use"
            },
            "IntegerProperty": {
                "type": "integer",
                "name": "Integer attribute",
                "description": "PI point data referenced integer attribute"
            }
        }
    },
    {
        "id": "SecondDynamicType",
        "name": "Second dynamic type",
        "classification": "dynamic",
        "type": "object",
        "description": "not in use",
        "properties": {
            "timestamp": {
                "format": "date-time",
                "type": "string",
                "isindex": True,
                "name": "not in use",
                "description": "not in use"
            },
            "SecondDynamicType NumberProperty 1": {
                "type": "number",
                "name": "Number attribute 1",
                "description": "PI point data referenced number attribute 1"
            },
            "SecondDynamicType NumberProperty 2": {
                "type": "number",
                "name": "Number attribute 2",
                "description": "PI point data referenced number attribute 2"
            },
            "SecondDynamicType String Enum": {
                 "type": "string",
                 "enum": ["False", "True"],
                 "name": "String enumeration representing Boolean type",
                 "description": "PI point data referenced digital set for Boolean"
            }
        }
    },
    {
        "id": "ThirdDynamicType",
        "name": "Third dynamic type",
        "classification": "dynamic",
        "type": "object",
        "description": "not in use",
        "properties": {
            "timestamp": {
                "format": "date-time",
                "type": "string",
                "isindex": True,
                "name": "not in use",
                "description": "not in use"
            },
            "BooleanEnum": {
                "type": "integer",
                "format": "int16",
                "enum": [0, 1],
                "name": "Integer enumeration",
                "description": "Integer enumeration to replace boolean type"
            }
        }
    }
])


# ************************************************************************
# Send a JSON packet to define containerids and the type
# (using the types listed above) for each new data events container.
# This instantiates these particular containers.
# We can now directly start sending data to it using its Id.
# ************************************************************************
send_omf_message_to_endpoint("container", [
    {
        "id": "Container1",
        "typeid": "FirstDynamicType"
    },
    {
        "id": "Container2",
        "typeid": "FirstDynamicType"
    },
    {
        "id": "Container3",
        "typeid": "SecondDynamicType"
    },
    {
        "id": "Container4",
        "typeid": "ThirdDynamicType"
    }
])


# ************************************************************************
# Send the messages to create the PI AF asset structure
#
# The following packets can be sent in one data message; the example
# splits the data into several messages only for readability;
# you can send all of the following data in one message,
# as far as its size is below maximum allowed - 192K
# ************************************************************************

# Send a JSON packet to define assets
send_omf_message_to_endpoint("data", [
    {
        "typeid": "FirstStaticType",
        "values": [
            {
                "index": "Asset1",
                "name": "Parent element",
                "StringProperty": "Parent element attribute value"
            }
        ]
    },
    {
        "typeid": "SecondStaticType",
        "values": [
            {
                "index": "Asset2",
                "name": "Child element",
                "StringProperty": "Child element attribute value"
            }
        ]
    }
])

# Send a JSON packet to define links between assets
# to create AF Asset structure
send_omf_message_to_endpoint("data", [
    {
        "typeid": "__Link",
        "values": [
            {
                "source": {
                        "typeid": "FirstStaticType",
                        "index": "_ROOT"
                },
                "target": {
                        "typeid": "FirstStaticType",
                        "index": "Asset1"
                }
            },
            {
                "source": {
                        "typeid": "FirstStaticType",
                        "index": "Asset1"
                },
                "target": {
                        "typeid": "SecondStaticType",
                        "index": "Asset2"
                }
            }
        ]
    }
])

# Send a JSON packet to define links between assets and
# containerids to create attributes with PI point references
# from containerid properties
send_omf_message_to_endpoint("data", [
    {
        "typeid": "__Link",
        "values": [
            {
                "source": {
                        "typeid": "FirstStaticType",
                        "index": "Asset1"
                },
                "target": {
                        "containerid": "Container1"
                }
            },
            {
                "source": {
                        "typeid": "SecondStaticType",
                        "index": "Asset2"
                },
                "target": {
                        "containerid": "Container2"
                }
            },
            {
                "source": {
                        "typeid": "SecondStaticType",
                        "index": "Asset2"
                },
                "target": {
                        "containerid": "Container3"
                }
            },
            {
                "source": {
                        "typeid": "SecondStaticType",
                        "index": "Asset2"
                },
                "target": {
                        "containerid": "Container4"
                }
            }
        ]
    }
])



# ************************************************************************
# Helper functions: REQUIRED: create a JSON message that contains data values
# for all defined containerids
#
# Note: if you do not send one of the values for the container, Relay
# will emit the default value for missing property - it is the default
# behavior of JSON serialization; this might lead to undesireable
# results: for example, putting a value of zero into referenced PI
# point
# ************************************************************************

def getCurrentTime():
    return datetime.datetime.utcnow().isoformat() + 'Z'


# Creates a JSON packet containing data values for Container1
# of type FirstDynamicType defined below
def create_data_values_for_container1():
    return [
        {
            "containerid": "Container1",
            "values": [
                {
                    "timestamp": getCurrentTime(),
                    "IntegerProperty": int(100*random.random())
                }
            ]
        }
    ]

# Creates a JSON packet containing data values for Container2
# of type FirstDynamicType defined below
def create_data_values_for_container2():
    return [
        {
            "containerid": "Container2",
            "values": [
                {
                    "timestamp": getCurrentTime(),
                    "IntegerProperty": int(100*random.random())
                }
            ]
        }
    ]


string_boolean_value = "True"

# Creates a JSON packet containing data values for Container3
# of type SecondDynamicType defined below
def create_data_values_for_container3():
    global string_boolean_value
    if string_boolean_value == "True":
        string_boolean_value = "False"
    else:
        string_boolean_value = "True"

    return [
        {
            "containerid": "Container3",
            "values": [
                {
                    "timestamp": getCurrentTime(),
                    "SecondDynamicType NumberProperty 1": 100*random.random(),
                    "SecondDynamicType NumberProperty 2": 100*random.random(),
                    "SecondDynamicType String Enum": string_boolean_value
                }
            ]
        }
    ]


# Creates a JSON packet containing data values for Container4
# of type ThirdDynamicType defined below
boolean_value = 0
def create_data_values_for_container4():
    global boolean_value
    if boolean_value == 0:
        boolean_value = 1
    else:
        boolean_value = 0
    return [
        {
            "containerid": "Container4",
            "values": [
                {
                    "timestamp": getCurrentTime(),
                    "BooleanEnum": boolean_value
                }
            ]
        }
    ]


# ************************************************************************
# Finally, loop indefinitely, sending random events
# conforming to the container types that we defined earlier

# Note: PI points will be created on the first data value message
# arrived for a given container
#
# Note: values for each containerid are sent as a batch; you can update
# different containerids at different times
# ************************************************************************

update = 0
stream1_update = 1
stream2_update = 2
stream3_update = 3

while True:
    update = update + 1
    if update == stream1_update: 
        send_omf_message_to_endpoint("data", create_data_values_for_container1())
    elif update == stream2_update: 
        send_omf_message_to_endpoint("data", create_data_values_for_container2())
    elif update == stream3_update: 
        send_omf_message_to_endpoint("data", create_data_values_for_container3())
        send_omf_message_to_endpoint("data", create_data_values_for_container4())
    if update == 3:
        update = 0
    time.sleep(1)
