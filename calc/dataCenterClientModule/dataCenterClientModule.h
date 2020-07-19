#pragma once

#include "dataCenterClientExports.h"
#include "dataCenterClientInterface.h"

class DataCenterServerProxy;

class DCClientAPI dataCenterClientModule
{
public:
	dataCenterClientModule();
	~dataCenterClientModule();

	//��������
	//soap_endpoint "http://127.0.0.1:8090";
	bool	Start(std::string& strEndPoint, IWebServiceServeResult* pResult);

	//�رշ���
	bool	Stop();

	//�洢����
	bool	AddTask_SetData(const SBlockKeyType& key, const SBlockDataType& data);
	
	//��ȡ����
	bool	AddTask_GetData(const SBlockKeyType& key);

	//��ȡ����У���
	bool	AddTask_GetDataCheckSum(const SBlockKeyType& key);

protected:

	IWebServiceServeResult*	_pResult;
	DataCenterServerProxy*	_pSoap;

	static void		threadFunction(void*);
};