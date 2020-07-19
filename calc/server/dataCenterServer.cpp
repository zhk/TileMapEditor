// calcClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "ConnectPoolThreadingServer.h"
#include "DataCenterServer.nsmap"
#include "threads.h"

#include <map>
#include <string>
using namespace std;

static MUTEX_TYPE data_cs;
#include <iostream>


int http_get(struct soap *soap)
{
	FILE *fd = NULL;
	char *s = strchr(soap->path, ' ?');

	if (!s || strcmp(s, "?wsdl"))
		return SOAP_GET_METHOD;

	fd = fopen("DataCenterServer.wsdl", "rb"); // open WSDL file to copy
	if (!fd)
		return 404; // return HTTP not found error

	soap->http_content = "text/xml"; // HTTP header with text/xml content
	soap_response(soap, SOAP_FILE);
	for (;;)
	{
		size_t r = fread(soap->tmpbuf, 1, sizeof(soap->tmpbuf), fd);
		if (!r)
			break;
		
		if (soap_send_raw(soap, soap->tmpbuf, r))
			break; // can't send, but little we can do about that
	}
	
	fclose(fd);
	
	soap_end_send(soap);

	return SOAP_OK;
}


int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 2)
	{
		fprintf(stderr, "Usage: dataCenterServer <port>\n");
		exit(0);
	}
	else
	{
		int port = atoi(argv[1]);

		if (!port)
		{
			fprintf(stderr, "Usage: dataCenterServer++ <port>\n");

			exit(0);
		}

		ConnectPoolThreadingServer dataCenter(SOAP_IO_KEEPALIVE, SOAP_IO_CHUNK);
		dataCenter.send_timeout		= 60;		// 60 seconds
		dataCenter.recv_timeout		= 60;		// 60 seconds
		dataCenter.accept_timeout	= 3600 * 8;	// server stops after 8 hours of inactivity
		dataCenter.max_keep_alive	= 100;		// max keep-alive sequence


		dataCenter.fget = http_get;


		MUTEX_SETUP(data_cs);

		/* run iterative server on port until fatal error */
		if (dataCenter.run(port))
		{
			dataCenter.soap_stream_fault(std::cerr);
			exit(-1);
		}

		MUTEX_CLEANUP(data_cs);
	}

	return 0;
} 


struct SKey_Less
{
	bool operator()(const BlockKeyType& l, const BlockKeyType& r) const
	{
		string strl = l.appKey + l.regionKey + l.dataType + l.dataKey;
		string strr = r.appKey + r.regionKey + r.dataType + r.dataKey;

		return strcmp(strl.c_str(), strr.c_str()) < 0;
	}
};

typedef map<BlockKeyType, BlockDataType, SKey_Less>	KeyDataMapType;
KeyDataMapType g_Data;


int DataCenterServerService::setData(struct BlockKeyType *key, struct BlockDataType *data)
{
	MUTEX_LOCK(data_cs);
	g_Data[*key] = *data;
	MUTEX_UNLOCK(data_cs);

	return SOAP_OK;
}

int DataCenterServerService::getData(struct BlockKeyType *key, std::string &data)
{
	MUTEX_LOCK(data_cs);
	if (g_Data.find(*key) != g_Data.end())
	{
		data = g_Data[*key].data;
	}
	MUTEX_UNLOCK(data_cs);

	return SOAP_OK;
}

int DataCenterServerService::getDataCheckSum(struct BlockKeyType *key, std::string &checkSum)
{
	MUTEX_LOCK(data_cs);
	if (g_Data.find(*key) != g_Data.end())
	{
		checkSum = g_Data[*key].dataCheckSum;
	}
	MUTEX_UNLOCK(data_cs);

	return SOAP_OK;
}

int DataCenterServerService::add(double a, double b, double& result)
{
	result = a + b;
	return SOAP_OK;
} 