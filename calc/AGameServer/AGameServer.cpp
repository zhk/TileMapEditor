// AGameServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../dataCenterClientModule/dataCenterClientInterface.h"
#include "../dataCenterClientModule/dataCenterClientModule.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <iostream>

using namespace std;

class MyHandler : public IWebServiceServeResult
{
public:
	MyHandler()
	{
	}

	//返回获取的数据
	virtual void	OnGetDataOK(const SBlockKeyType& key, const std::string& data)
	{
		cout << "OnGetDataOK: " << endl;
		cout << key.appKey << " : " << key.dataKey << " : " << key.dataType << " : " << key.regionKey << endl;
		cout << data << endl << endl;
	}

	virtual void	OnGetDataFailed(const SBlockKeyType& key)
	{
		cout << "OnGetDataFailed: " << endl;
		cout << key.appKey << " : " << key.dataKey << " : " << key.dataType << " : " << key.regionKey << endl << endl;
	}

	//返回获取的校验和
	virtual void	OnGetDataCheckSumOK(const SBlockKeyType& key, const std::string& checkSum)
	{
		cout << "OnGetDataCheckSumOK: " << endl;
		cout << key.appKey << " : " << key.dataKey << " : " << key.dataType << " : " << key.regionKey << endl;
		cout << checkSum << endl << endl;
	}

	virtual void	OnGetDataCheckSumFailed(const SBlockKeyType& key)
	{
		cout << "OnGetDataCheckSumFailed: " << endl;
		cout << key.appKey << " : " << key.dataKey << " : " << key.dataType << " : " << key.regionKey << endl << endl;
	}

}myHandler;

int _tmain(int argc, _TCHAR* argv[])
{
	dataCenterClientModule wsModule;

	wsModule.Start(string("http://127.0.0.1:8090"), &myHandler);

	int iStep = 0;

	while(iStep < 9)
	{
		SBlockKeyType key;
		key.appKey		= "FSWD";
		key.dataKey		= "10000_01";
		key.dataType	= "Social";
		key.regionKey	= "Region01";

		switch (iStep%3 + 1)
		{
		case 1:
			{
				SBlockDataType data;
				data.data			= "FSWD_Region01_Social_10000_01_";

				for (int i = 0; i < rand()% 52; ++i)
				{
					char ch = i + 'A';
					data.data += ch;
				}
				for (int i = 0; i < 4; ++i)
				{
					char buffer[4];
					sprintf(buffer, "%02X", rand()% 16);
					data.dataCheckSum += buffer;
				}

				wsModule.AddTask_SetData(key, data);

				cout << "AddTask_SetData: " << endl;
				cout << data.data << ", " << data.dataCheckSum << endl << endl;
			}
			break;
		case 2:
			{
				wsModule.AddTask_GetData(key);
			}
			break;
		case 3:
			{
				wsModule.AddTask_GetDataCheckSum(key);
			}
			break;
		}

		iStep++;
		Sleep(3000);
	}

	wsModule.Stop();

	return 0;
}

