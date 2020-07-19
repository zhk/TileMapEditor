#pragma once

#include "dataCenterClientExports.h"
#include "dataCenterClientInterface.h"

class DataCenterServerProxy;

class DCClientAPI dataCenterClientModule
{
public:
	dataCenterClientModule();
	~dataCenterClientModule();

	//启动服务
	//soap_endpoint "http://127.0.0.1:8090";
	bool	Start(std::string& strEndPoint, IWebServiceServeResult* pResult);

	//关闭服务
	bool	Stop();

	//存储数据
	bool	AddTask_SetData(const SBlockKeyType& key, const SBlockDataType& data);
	
	//获取数据
	bool	AddTask_GetData(const SBlockKeyType& key);

	//获取数据校验和
	bool	AddTask_GetDataCheckSum(const SBlockKeyType& key);

protected:

	IWebServiceServeResult*	_pResult;
	DataCenterServerProxy*	_pSoap;

	static void		threadFunction(void*);
};