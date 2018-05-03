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

#include "osisoft-omf.h"
#include "mbed.h"
#include "https_request.h"
#include "ntp-client/NTPClient.h"
#include "config.egressSettings.h"
#include "config.SSLCertificates.h"
#include "config.producerToken.h"

// ---------------------------------------------------------------------------------------------------

// The clock is set usign NTP; if that fails, the clock defaults to the below time
const int DEFAULT_HARD_CODED_UTC_TIME = 1513175347;

// ---------------------------------------------------------------------------------------------------

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
// Helper function that casts floats into strings
// ************************************************************************

string OMFLib_float_to_string(float f) {
    char buffer[20];
    int n = sprintf (buffer, "%f", f);
    // Return -1 if a parse failure ocurred
    if (string(buffer) == "NaN") {
        return string("-1");
    } else {
        return string(buffer);
    }
}

// ************************************************************************
// Helper function that sends an actual web request
// ************************************************************************

void OMFLib_sendMessageToEndpoint(TLSSocket* socket, const char* action, const char* message_type, const char* body) {
    printf("\n\r----- HTTPS POST request -----\n\r");
    
    // Create the new request
    //HttpsRequest* post_req = new HttpsRequest(network, SSL_CA_PEM, HTTP_POST, TARGET_URL); // Old: doesn't re-use sockets
    HttpsRequest* post_req = new HttpsRequest(socket, HTTP_POST, TARGET_URL);

    // Turn on debugging - this hides TLS connection information
    post_req->set_debug(true);

    // Add headers: content type and authentication
    post_req->set_header("Content-Type", "application/json");

    // Set OMF headers
    post_req->set_header("producertoken", PRODUCER_TOKEN);
    post_req->set_header("messagetype", message_type);
    post_req->set_header("action", action);
    post_req->set_header("messageformat", "JSON");
    post_req->set_header("omfversion", "1.0");
        
    // Send the body
    printf("Now sending request...");
    printf("\n\rOutgoing message headers:\n\tmessagetype: %s\n\taction: %s", message_type, action);
    printf("\n\rOutgoing message body:\n\t%s\n\r", body);
    HttpResponse* post_res = post_req->send(body, strlen(body));

    // Check the response for an error
    if (!post_res) {
        printf("HttpRequest failed (error code %d)\n\r", post_req->get_error());
        printf("Socket connection status after error: %d\n\r", socket->connected());
        //return 1;
    } else {
        // Print the response
        OMFLib_dump_response(post_res);
    }

    // Free up the request object
    delete post_req;
}

// ************************************************************************
// Helper function that sends an actual web request; does not reuse sockets
// ************************************************************************

void OMFLib_sendMessageToEndpoint_NoSocketReuse(NetworkInterface* network, const char* action, const char* message_type, const char* body) {
    
    printf("\n\r----- HTTPS POST request -----\n\r");
    
    // Create the new request
    HttpsRequest* post_req = new HttpsRequest(network, SSL_CA_PEM, HTTP_POST, TARGET_URL);

    // Turn on debugging - this hides TLS connection information
    //post_req->set_debug(true);

    // Add headers: content type 
    post_req->set_header("Content-Type", CONTENT_TYPE);

    // Set OMF headers
    post_req->set_header("producertoken", PRODUCER_TOKEN);
    post_req->set_header("messagetype", message_type);
    post_req->set_header("action", action);
    post_req->set_header("messageformat", OMF_MESSAGE_FORMAT);
    post_req->set_header("omfversion", OMF_VERSION);
        
    // Send the body
    printf("Now sending request... ");
    //printf("\n\rOutgoing message headers:\n\tmessagetype: %s\n\taction: %s", message_type, action);
    //printf("\n\rOutgoing message body:\n\t%s\n\r", body);
    HttpResponse* post_res = post_req->send(body, strlen(body));
    
    // Check the response for an error
    if (!post_res) {
        printf("HttpRequest failed (error code %d)\n\r", post_req->get_error());
        //return 1;
    } else {
        // Print the response
        printf("Success!\n\r");
        //OMFLib_dump_response(post_res);
    }

    // Free up the request object
    delete post_req;
}

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

// ************************************************************************
// Sets the clock via NTP via the nwtwork; can point to a local or internet-based server
// ************************************************************************

void OMFLib_syncClockViaNTP(NetworkInterface* network) {
    // Hard-code a start time... see https://www.epochconverter.com/   
    set_time(DEFAULT_HARD_CODED_UTC_TIME);
    
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
        printf("NTP time sync failed; clock set to %i UTC seconds.", DEFAULT_HARD_CODED_UTC_TIME);
    }
}