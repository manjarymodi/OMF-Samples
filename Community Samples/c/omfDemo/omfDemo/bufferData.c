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

/* bufferData.c */

/*
The functions in this file buffers data to a file on disk. Here's an example 
of the contents of this file:

Offset(d) 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15
00000000  29 76 06 00 0C 00 00 00 97 0E 00 00 06 00 00 00  )v......�.......
00000016  54 79 70 65 73 00 07 00 00 00 63 72 65 61 74 65  Types.....create
00000032  00 1B 0C 00 00 5B 7B 22 69 64 22 3A 22 56 65 68  .....[{"id":"Veh
00000048  69 63 6C 65 54 79 70 65 22 2C 22 64 65 73 63 72  icleType","descr
00000064  69 70 74 69 6F 6E 22 3A 22 56 65 68 69 63 6C 65  iption":"Vehicle
00000080  20 64 65 73 63 72 69 70 74 69 6F 6E 22 2C 22 74   description","t
00000096  79 70 65 22 3A 22 6F 62 6A 65 63 74 22 2C 22 63  ype":"object","c
00000112  6C 61 73 73 69 66 69 63 61 74 69 6F 6E 22 3A 22  lassification":"
00000128  61 73 73 65 74 22 2C 22 70 72 6F 70 65 72 74 69  asset","properti
00000144  65 73 22 3A 7B 22 56 65 68 69 63 6C 65 4E 61 6D  es":{"VehicleNam
00000160  65 22 3A 7B 22 74 79 70 65 22 3A 22 73 74 72 69  e":{"type":"stri
00000176  6E 67 22 2C 22 69 6E 64 65 78 22 3A 74 72 75 65  ng","index":true
00000192  7D 2C 22 56 49 4E 22 3A 7B 22 74 79 70 65 22 3A  }, "VIN":{"type":
00000208  22 73 74 72 69 6E 67 22 7D 2C 22 59 65 61 72 22  "string"}, "Year"
00000224  3A 7B 22 74 79 70 65 22 3A 22 69 6E 74 65 67 65 : {"type":"intege
00000240  72 22 7D 2C 22 4D 61 6B 65 22 3A 7B 22 74 79 70  r"},"Make":{"typ
00000256  65 22 3A 22 73 74 72 69 6E 67 22 7D 2C 22 4D 6F  e":"string"},"Mo

The first 4 bytes (1st long) contain a header value so that we can 
confirm that the file is a buffer file.

The next 4 bytes (2nd long) store the read position. In the example above, 
the value is 0x0000000c, which is 12.

The next 4 bytes (3rd long) store the write position. In the example above, 
the value is 0x00000e97, which is 3735.

When we buffer data, we go to the write position and add data. We then
update the value in the 2nd long.

When we read from the buffer file, we go to the read position and read the
data and send to the Relay. If sending is successful, we update the
value in the 3rd long.

After we have read all the data from the buffer file, we re-initialize
the buffer file.
*/

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS  /* to remove warnings on fopen() */
#include <stdio.h>
#include <windows.h>
#elif linux
#include <stdio.h>
#else
need to implement
#endif

#include <string.h>
#include <stdlib.h>

#include "bufferData.h"
#include "sendToServerC.h"  /* for sendMsgToRelay() */
#include "main.h"

#define BUFFER_FILE  "omfBuffer.bin"

#define HEADER_VALUE  (777 * 545 )
#define INITIAL_DATA_LOCATION   (sizeof(long) + sizeof(long) + sizeof(long))
#define LOCATION_TO_STORE_READ   (sizeof(long))
#define LOCATION_TO_STORE_WRITE   (sizeof(long) + sizeof(long))

#define DESER_ALL_DONE  -200
#define DESER_MEM_ERR  -201

//#define PATH_LENGTH  2048

/* variable that holds the name of the buffer file */
static char g_BufferFile[PATH_LENGTH];

/* Function to retrieve the read position and write position of the buffer file */
static void getReadWritePos(FILE* fp, long* readPosParam, long* writePosParam)
{
	long header;
	long readPos;
	long writePos;

	rewind(fp);
	fread(&header, sizeof(long), 1, fp);
	fread(&readPos, sizeof(long), 1, fp);
	fread(&writePos, sizeof(long), 1, fp);

	*readPosParam = readPos;
	*writePosParam = writePos;
}

/* 
Function to get the position of the last occurrence of a character in a string
For example, get the final '\' in the string
   C:\Program Files\MyCompany\MyConnector.exe
*/
static int GetLastCharOccur(const char* sz, char c)
{
	int retval = -1;
	size_t idx = 0;
	for (idx = 0; idx < strlen(sz); idx++) {
		if (sz[idx] == c) {
			retval = idx;
		}
	}

	return retval;
}

/*
Function to get the full path of the currently running program
For example, get the string
   C:\Program Files\MyCompany\MyConnector.exe
*/
#ifdef _WIN32
char* GetFullPath()
{
	static char szRet[PATH_LENGTH];

	int lastPos;
	char szFileName[PATH_LENGTH];
	GetModuleFileName(NULL, szFileName, sizeof(szFileName));

	lastPos = GetLastCharOccur(szFileName, '\\');
	memset(szRet, 0, PATH_LENGTH);
	strncpy(szRet, szFileName, lastPos + 1); /* use lastPos+1 to include the '\' */

	return szRet;
}
#elif linux
char* GetFullPath()
{
	static char szRet[PATH_LENGTH];

	int lastPos;
	char szFileName[PATH_LENGTH];
	memset(szFileName, 0, PATH_LENGTH);
	readlink("/proc/self/exe", szFileName, sizeof(szFileName));

	lastPos = GetLastCharOccur(szFileName, '/');
	memset(szRet, 0, PATH_LENGTH);
	strncpy(szRet, szFileName, lastPos + 1);

	return szRet;
}
#else
need to implement
#endif

/*
Function to generate the full path of the buffer file. For example,
   C:\Program Files\MyCompany\MyProgram\omfbuffer.bin
*/
void SetBufferFileName()
{
	snprintf(g_BufferFile, PATH_LENGTH, "%s%s", GetFullPath(), BUFFER_FILE);
}

/*
   Returns
      0 -- successful creation of the buffer file
      FILE_CREATE_ERR -- cannot create the buffer file
*/
int CreateBufferFile()
{
	long header = HEADER_VALUE;
	long readPos = INITIAL_DATA_LOCATION;
	long writePos = INITIAL_DATA_LOCATION;

	/* create the file for binary read/write */
	FILE* fp = fopen(g_BufferFile, "w+b");
	if (!fp) {
		return FILE_CREATE_ERR;
	}

	fwrite(&header, sizeof(long), 1, fp);
	fwrite(&readPos, sizeof(long), 1, fp);
	fwrite(&writePos, sizeof(long), 1, fp);

	fclose(fp);

#ifdef _DEBUG
	printf("*** Created a new buffer file ***\n");
#endif
	return 0;
}

/*
returns
     1   if we need to create the buffer file
     0   if we do not need to create the buffer file
*/
int NeedToCreateBufferFile()
{
	int retval = 1;

	long header;
	FILE* fp = fopen(g_BufferFile, "r+b");
	if (!fp) {
		return retval;
	}

	fread(&header, sizeof(long), 1, fp);
	if (HEADER_VALUE == header) {
		retval = 0;
	}

	fclose(fp);

	return retval;
}

/*
returns
   FILE_OPEN_ERR -- if we cannot open the buffer file
   BUFFER_FILE_NO_DATA -- if the file simply has header info and contains no data
   BUFFER_FILE_HAS_DATA -- if the file already has buffered data
*/
int ExamineBufferFile()
{
	int retval = BUFFER_FILE_HAS_DATA;
	
	long readPos;
	long writePos;

	FILE* fp = fopen(g_BufferFile, "r+b");
	if (!fp) {
		return FILE_OPEN_ERR;
	}

	getReadWritePos(fp, &readPos, &writePos);
	fclose(fp);

	if ((INITIAL_DATA_LOCATION == readPos) && (INITIAL_DATA_LOCATION == writePos)) {
		retval = BUFFER_FILE_NO_DATA;
	}

	return retval;
}

/*
This function 
   writes the legth of the null-terminated <sz>
   writes <sz> itself
Returns
   the number of bytes written
*/
static int writeElement(const char* sz, FILE* fp)
{
	int bytesWritten = 0;
	size_t fRet;
	size_t writeLen = strlen(sz) + 1;  /* add 1 for the null terminator */

	fRet = fwrite(&writeLen, sizeof(size_t), 1, fp);
	bytesWritten += (fRet * sizeof(size_t));

	fRet = fwrite(sz, sizeof(char), writeLen, fp);
	bytesWritten += (fRet * sizeof(char));

	return bytesWritten;
}

/*
This function writes <msgtype>, <action>, and <poststr> to the buffer file.

Returns 0    
*/
int SerializeData (
	const char* msgtype,
	const char* action,
	const char* poststr
	)
{
	long readPos;
	long writePos;
	size_t fRet;
	long bytesWritten = 0;

	/* open file for binary read/write */
	FILE* fp = fopen(g_BufferFile, "r+b");
	if (!fp) {
		return FILE_OPEN_ERR;
	}

	/* get the value of the stored read and write positions */
	getReadWritePos(fp, &readPos, &writePos);

	/* go to the indicated write position */
	fseek(fp, writePos, SEEK_SET);

	fRet = writeElement(msgtype, fp);
	bytesWritten += fRet;

	fRet = writeElement(action, fp);
	bytesWritten += fRet;

	fRet = writeElement(poststr, fp);
	bytesWritten += fRet;

	writePos += bytesWritten;
#ifdef _DEBUG
	printf("writePos = %d\n", writePos);
#endif

	/* save the write position */
	fseek(fp, LOCATION_TO_STORE_WRITE, SEEK_SET);
	fwrite(&writePos, sizeof(long), 1, fp);

	fclose(fp);

	return 0;
}

/*
This function
   reads <msgtype>, <action>, and <poststr> from the buffer file.
   calls sendMsgToRelay()
Returns
   FILE_OPEN_ERR if we cannot open the buffer file
   DESER_MEM_ERR if malloc() fails
   DESER_ALL_DONE if we have processed all the data in the buffer file
   return value from sendMsgToRelay() otherwise
*/
int deserializeData()
{
	size_t fRet;
	long writePos;
	long readPos;
	long newReadPos;

	size_t sLen;
	char* msgtype;
	char* action;
	char* poststr;
	int goodHttpResponse;

	int retSend;
	int retval = 0;

	/* open file for binary read/write */
	FILE* fp = fopen(g_BufferFile, "r+b");
	if (!fp) {
		return FILE_OPEN_ERR;
	}

	/* get the value of the stored read/write positions */
	getReadWritePos(fp, &readPos, &writePos);
	if (readPos >= writePos) {
		fclose(fp);
		return DESER_ALL_DONE;
	}
	newReadPos = readPos;

	/* go to the indicated read position */
	fseek(fp, readPos, SEEK_SET);

	/* get "msgtype" */
	fRet = fread(&sLen, sizeof(size_t), 1, fp);
	newReadPos += (fRet * sizeof(size_t));
	msgtype = malloc(sLen);
	if (!msgtype) {
		printf("malloc() error\n");
		fclose(fp);
		return DESER_MEM_ERR;
	}
	fRet = fread(msgtype, sizeof(char), sLen, fp);
	newReadPos += (fRet * sizeof(char));

	/* get "action" */
	fRet = fread(&sLen, sizeof(size_t), 1, fp);
	newReadPos += (fRet * sizeof(size_t));
	action = malloc(sLen);
	if (!action) {
		printf("malloc() error\n");
		fclose(fp);
		return DESER_MEM_ERR;
	}
	fRet = fread(action, sizeof(char), sLen, fp);
	newReadPos += (fRet * sizeof(char));

	/* get "poststr" */
	fRet = fread(&sLen, sizeof(size_t), 1, fp);
	newReadPos += (fRet * sizeof(size_t));
	poststr = malloc(sLen);
	if (!poststr) {
		printf("malloc() error\n");
		fclose(fp);
		return DESER_MEM_ERR;
	}
	fRet = fread(poststr, sizeof(char), sLen, fp);
	newReadPos += (fRet * sizeof(char));
	
	retSend = sendMsgToRelay(&goodHttpResponse, msgtype, action, poststr);
	if (0 == retSend) {
		fseek(fp, LOCATION_TO_STORE_READ, SEEK_SET);
		fwrite(&newReadPos, sizeof(long), 1, fp);
	}

	free(msgtype);
	free(action);
	free(poststr);

	fclose(fp);

	retval = retSend;

	return retval;
}

/*
This function processes the buffer file.
Returns
    0 -- if we have completely processed the buffer data file
	return value of deserializeData() -- if buffer data file still has data
*/
int ProcessBufferFile()
{
	int ret = 0;
	while (0 == ret) {
		ret = deserializeData();
	}

	if (DESER_ALL_DONE == ret) {
		CreateBufferFile();
		ret = 0;
	}

	return ret;
}