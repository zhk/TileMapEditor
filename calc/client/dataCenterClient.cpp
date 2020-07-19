// calcClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "soapDataCenterServerProxy.h"	// get proxy
#include "DataCenterServer.nsmap"		// import the generated namespace mapping table

#include <iostream>

const char server[] = "http://127.0.0.1:8090";


int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 4)
	{
		fprintf(stderr, "Usage: [add|sub|mul|div|pow] num num\n");
		exit(0);
	}

	double a, b, result;
	a = strtod(argv[2], NULL);
	b = strtod(argv[3], NULL);

	DataCenterServerProxy calc;
	calc.soap_endpoint = server;
	switch (*argv[1])
	{
	case 'a':
		calc.add(a, b, result);
		break;
	default:
		fprintf(stderr, "Unknown command\n");
		exit(0);
	}

	if (calc.error)
		calc.soap_stream_fault(std::cerr);
	else
		printf("result = %g\n", result);

	return 0;
}
