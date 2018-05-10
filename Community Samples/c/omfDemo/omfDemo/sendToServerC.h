/*

©2009-2018 OSIsoft, LLC. All Rights Reserved.

No Warranty or Liability.  The OSIsoft Samples contained herein are being supplied to Licensee
"AS IS" without any warranty of any kind.  OSIsoft DISCLAIMS ALL EXPRESS AND IMPLIED WARRANTIES,
INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE and NONINFRINGEMENT. In no event will OSIsoft be liable to Licensee or to any third party
for damages of any kind arising from Licensee's use of the OSIsoft Samples OR OTHERWISE, including
but not limited to direct, indirect, special, incidental and consequential damages, and Licensee
expressly assumes the risk of all such damages.  FURTHER, THE OSIsoft SAMPLES ARE NOT ELIGIBLE FOR
SUPPORT UNDER EITHER OSISOFT'S STANDARD OR ENTERPRISE LEVEL SUPPORT AGREEMENTS

*/

/* sendToServerC.h*/

#pragma once

#define COMM_LIB_NAME  "libCurl"
#define ERR_CouldNotConnect   -28
#define INFO_WROTE_MSG_TO_BUFFER   -600

int initCommLib();
void endCommLib();
char* getCommLibErr();
int setUpBuffering();

void setServerParams(
	const char* hostAndPort,
	const char* page,
	const char* producerid,
	const char* version,
	const char* msgformat
	);

int processPostMsg(
	int* goodHttpResponse,
	const char* msgtype,
	const char* action,
	const char* poststr);

int sendMsgToRelay(
	int* goodHttpResponse,
	const char* msgtype,
	const char* action,
	const char* poststr);
