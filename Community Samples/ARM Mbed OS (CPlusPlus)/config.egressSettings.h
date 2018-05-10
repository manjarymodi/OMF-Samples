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

#ifndef EGRESS_SETTINGS_H
#define EGRESS_SETTINGS_H

// Specify the address of the destination endpoint
//const char* const TARGET_URL = "https://qi-data.osisoft.com/api/omf";
const char* const TARGET_URL = "https://targetserver:777/ingress/messages";

// ******************************************************************
// IMPORTANT NOTE
// ******************************************************************
// To turn off certificate validation, in mbed-http > source > tls_socket.h, 
// change line 117 to mbedtls_ssl_conf_authmode(&_ssl_conf, MBEDTLS_SSL_VERIFY_OPTIONAL);

// Specify whether to send static assets and links
// This should be false when writing to OCS
//const bool SEND_ASSETS_AND_LINKS = false;
const bool SEND_ASSETS_AND_LINKS = true;

// Specify the target URL host name and port number, for cases when re-using sockets
//const char* const ENDPOINT_HOST = "qi-data.osisoft.com";
//const int ENDPOINT_PORT = 443;

// Specify the number of seconds to sleep in between value messages
const int SECONDS_BETWEEN_MESSAGES = 1;

#endif
