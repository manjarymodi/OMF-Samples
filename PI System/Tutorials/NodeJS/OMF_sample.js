/*************************************************************************************
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
# For more info, see OMF Developer Companion Guide:
# http://omf-companion-docs.osisoft.com
****************************************************************************************/

var monitor = require('os-monitor'),
    request = require('request'),
    nconf = require('nconf'),
    gzip = require('zlib'),
    seq = require('seq');

// get producertoken, endpoint, data rate, etc from configuration file
nconf.file({ file: "./config.json" });

// gzip compression 
var compressionOn = nconf.get("compressionGzip");

// setup common headers
var msgHeader = {
    producertoken: nconf.get("producertoken"),
    action: "create",
    messageformat: "JSON",
    omfversion: "1.0"

};

// setup common request options
var options = {
    uri: nconf.get("endpoint"),
    rejectUnauthorized: nconf.get("verifyCert"),
    method: "POST",
    headers: msgHeader
};


/*************************************************************************
# Helper functions: REQUIRED: create a JSON message that contains data values
# for all defined containerids
#
# Note: if you do not send one of the values for the container, Relay
# will emit the default value for missing property - it is the default
# behavior of JSON serialization; this might lead to undesireable
# results: for example, putting a value of zero into referenced PI
# point
# ************************************************************************/

// Creates a JSON packet containing data values for Container1
// of type FirstDynamicType defined below
function create_data_values_for_container1() {
    var timestring = new Date(Date.now()).toISOString(); 

    var event = [
        {
            "containerid": "Container1",
            "values": [
                {
                    "timestamp": timestring,
                    "IntegerProperty": Math.floor(Math.random() * 100)
                }
            ]
        }
    ];

    return event;
}

// Creates a JSON packet containing data values for Container2
// of type FirstDynamicType defined below
function create_data_values_for_container2() {
    var timestring = new Date(Date.now()).toISOString(); 

    var event = [
        {
            "containerid": "Container2",
            "values": [
                {
                    "timestamp": timestring,
                    "IntegerProperty": Math.floor(Math.random() * 100)
                }
            ]
        }
    ];
    return event;
}

// Creates a JSON packet containing data values for Container3
// of type SecondDynamicType defined below
var string_boolean_value = "True";

function create_data_values_for_container3() {
    var timestring = new Date(Date.now()).toISOString(); 
    if (string_boolean_value == "True")
        string_boolean_value = "False";
    else
        string_boolean_value = "True";

    var event = [
        {
            "containerid": 'Container3',
            "values": [
                {
                    "timestamp": timestring,
                    "SecondDynamicType NumberProperty 1": Math.random() * 100,
                    "SecondDynamicType NumberProperty 2": Math.random() * 100,
                    "SecondDynamicType String Enum": string_boolean_value

                }
            ]
        }
    ];

    return event;
}

// Creates a JSON packet containing data values for Container4
// of type ThirdDynamicType defined below
var boolean_value = 0;
function create_data_values_for_container4() {
    if (boolean_value == 0)
        boolean_value = 1;
    else {
        boolean_value = 0;
    }
    
    var timestamp = new Date(Date.now()).toISOString();
    var event = [
        {
            "containerid": "Container4",
            "values": [
                {
                    "timestamp": timestamp,
                    "BooleanEnum": boolean_value
                }
            ]
        }
    ];
    return event;
}


/**************************************************************************
# Helper function: REQUIRED: wrapper function for sending an HTTPS message
# ************************************************************************/

// Define a helper function to allow easily sending web request messages;
// this function can later be customized to allow you to port this script to other languages.
// All it does is take in a data object and a message type, and it sends an HTTPS
// request to the target OMF endpoint
function send_omf_message_to_endpoint(message_type, message_omf_json, cb) {
    msgHeader.messagetype = message_type;
    var jsonValue = JSON.stringify(message_omf_json);
    if (compressionOn) {
        msgHeader.compression = 'gzip';
        
        gzip.gzip(jsonValue, function (err, compressed) {
                if (err) throw err;
                options.body = compressed;
                request(options,
                    function (err, res, body) {
                        if (err) {
                            console.log(new Date(Date.now()).toISOString(), err, 'Error sending Gzip ', message_type, ' message.');
                            throw Error(err);
                        } else {
                            console.log(new Date(Date.now()).toISOString(), res.statusCode, message_type, ' Gzip message sent.');
                            if (cb != null) {
                                cb();
                            }
                        }
                    });
            }
        );
    }
    else {
        options.body = jsonValue;
        request(options,
            function (err, res, body) {
                if (err) {
                    console.log(new Date(Date.now()).toISOString(), err, 'Error sending', message_type, ' message.');
                    throw Error(err);
                } else {
                    console.log(new Date(Date.now()).toISOString(), res.statusCode, message_type, ' message sent.');
                    if (cb != null) {
                        cb();
                    }
                }
            });
    }
       
}


//Create a JSON packet to define static types
typesStatic = [
    {
        "id": "FirstStaticType",
        "name": "First static type",
        "classification": "static",
        "type": "object",
        "description": "First static asset type",
        "properties": {
            "index": {
                "type": "string",
                "isindex": true,
                "name": "not in use",
                "description": "not in use"
            },
            "name": {
                "type": "string",
                "isname": true,
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
                "isindex": true,
                "name": "not in use",
                "description": "not in use"
            },
            "name": {
                "type": "string",
                "isname": true,
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
]

//Create a JSON packet to define dynamic types
typesDynamic = [
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
                "isindex": true,
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
                "isindex": true,
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
                "isindex": true,
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
]

/*************************************************************************
# Create a JSON packet to define containerids and the type
# (using the types listed above) for each new data events container
# ************************************************************************/
containers = [
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
]

/**************************************************************************
# Create a JSON packets to containing the assets and
# linking data for the PI AF assets that will be made
#
# The following packates can be sent in one data message; the example
# splits the data into several messages only for readability;
# you can send all of the following data in one message,
# as far as its size is below maximum allowed - 192K
# ************************************************************************/

// Create a JSON packet to define assets
staticData = [
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
]


// Create a JSON packet to define links between assets
// to create AF Asset structure
linkDataElements = [
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
]

// Create a JSON packet to define links between assets and
// containerids to create attributes with PI point references
// from containerid properties
linkDataDynamicAttributes = [
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
]


// use seq to ensure that the monitor is not started until
// the type, container and data messages are successfully sent in order
seq()
    /**************************************************************************
    # Send the types messages, so that these types can be referenced in all later messages
    # ************************************************************************/
    .seq(function() {
        send_omf_message_to_endpoint("type", typesStatic, this);
    })
    .seq(function() {
        send_omf_message_to_endpoint("type", typesDynamic, this);
    })

    /**************************************************************************
    # Send the container message, to instantiate these particular containers;
    # we can now directly start sending data to it using its Id
    # ************************************************************************/
    .seq(function() {
        send_omf_message_to_endpoint("container", containers, this);
    })

    /************************************************************************
     Send the messages to create the PI AF asset structure
    ************************************************************************/
    .seq(function() {
        send_omf_message_to_endpoint("data", staticData, this);

    })
    .seq(function() {
        send_omf_message_to_endpoint("data", linkDataElements, this);
    })
    .seq(function () {
        send_omf_message_to_endpoint("data", linkDataDynamicAttributes, this);
    })

   
    .seq(function () {
        update = 0;
        stream1_update = 1;
        stream2_update = 2;
        stream3_update = 3;
        monitor.start({delay: nconf.get('interval')});
    });

/**************************************************************************
# Finally, loop indefinitely, sending random events
# conforming to the container types that we defined earlier

# Note: PI points will be created on the first data value message
# arrived for a given container
#
# Note: values for each containerid are sent as a batch; you can update
# different containerids at different times
# ************************************************************************/
monitor.on('monitor',
    function() {
        switch (update) {
        case stream1_update:
            send_omf_message_to_endpoint("data", create_data_values_for_container1(), null);
            break;
        case stream2_update:
            send_omf_message_to_endpoint("data", create_data_values_for_container2(), null);
            break;
        case stream3_update:
                send_omf_message_to_endpoint("data", create_data_values_for_container3(), null);
                send_omf_message_to_endpoint("data", create_data_values_for_container4(), null);
            break;            
        }
        update = (update + 1) % (stream3_update + 1);

    });

//shutdown gracefully on SIGTERM
process.on('SIGTERM',
    function() {
        monitor.stop();
    });