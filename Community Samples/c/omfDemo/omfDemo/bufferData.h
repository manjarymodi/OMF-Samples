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

/* bufferData.h */

#pragma once

#define FILE_CREATE_ERR  -100
#define FILE_OPEN_ERR   -101

#define BUFFER_FILE_NO_DATA  -300
#define BUFFER_FILE_HAS_DATA  -301

void SetBufferFileName();
int CreateBufferFile();
int NeedToCreateBufferFile();

int ExamineBufferFile();

int SerializeData(
	const char* msgtype,
	const char* action,
	const char* poststr
	);

int ProcessBufferFile();
