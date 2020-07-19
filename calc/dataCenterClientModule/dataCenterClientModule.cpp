// dataCenterClientModule.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "dataCenterClientModule.h"
#include "soapDataCenterServerProxy.h"	// get proxy
#include "DataCenterServer.nsmap"		// import the generated namespace mapping table

#include "threads.h"
#include <list>

using namespace std;

static MUTEX_TYPE module_cs;
volatile bool s_bRunning = false;
THREAD_TYPE tid;
std::string g_strEndPoint;

enum ETaskType
{
	eSetData,
	eGetData,
	eGetSum
};

struct STask
{
	ETaskType		_eType;
	SBlockKeyType	_key;
	SBlockDataType	_data;
};

typedef std::list<STask*>	TaskListType;
TaskListType	g_Tasks;


dataCenterClientModule::dataCenterClientModule()
: _pResult(0)
, _pSoap(0)
{
	MUTEX_SETUP(module_cs);
}

dataCenterClientModule::~dataCenterClientModule()
{
	MUTEX_CLEANUP(module_cs);
}

bool dataCenterClientModule::Start(std::string& strEndPoint, IWebServiceServeResult* pResult)
{
	g_strEndPoint = strEndPoint;

	_pResult = pResult;

	_pSoap = new DataCenterServerProxy;
	_pSoap->soap_endpoint = g_strEndPoint.c_str();

	tid = (HANDLE)_beginthread(&dataCenterClientModule::threadFunction, 8*4096, this);
	
	s_bRunning = true;

	return true;
}

bool dataCenterClientModule::Stop()
{
	if (s_bRunning)
		return false;

	s_bRunning = false;

	THREAD_JOIN(tid);

	if (_pSoap)
	{
		_pSoap->destroy();
		
		delete _pSoap;
		_pSoap = 0;

		return true;
	}

	return false;
}

//存储数据
bool dataCenterClientModule::AddTask_SetData(const SBlockKeyType& key, const SBlockDataType& data)
{
	if (!s_bRunning)
		return false;

	STask* pTask = new STask;
	pTask->_eType	= eSetData;
	pTask->_key		= key;
	pTask->_data	= data;

	MUTEX_LOCK(module_cs);
	g_Tasks.push_back(pTask);
	MUTEX_UNLOCK(module_cs);

	return true;
}


//获取数据
bool dataCenterClientModule::AddTask_GetData(const SBlockKeyType& key)
{
	if (!s_bRunning)
		return false;

	STask* pTask = new STask;
	pTask->_eType	= eGetData;
	pTask->_key		= key;

	MUTEX_LOCK(module_cs);
	g_Tasks.push_back(pTask);
	MUTEX_UNLOCK(module_cs);

	return true;
}

//获取数据校验和
bool dataCenterClientModule::AddTask_GetDataCheckSum(const SBlockKeyType& key)
{
	if (!s_bRunning)
		return false;

	STask* pTask = new STask;
	pTask->_eType	= eGetSum;
	pTask->_key		= key;

	MUTEX_LOCK(module_cs);
	g_Tasks.push_back(pTask);
	MUTEX_UNLOCK(module_cs);

	return true;
}

void dataCenterClientModule::threadFunction(void* param)
{
	dataCenterClientModule* pModule = (dataCenterClientModule*)param;

	while (s_bRunning)
	{
		MUTEX_LOCK(module_cs);
		while(g_Tasks.size())
		{
			STask* pTask;
			pTask = g_Tasks.front();
			g_Tasks.pop_front();

			BlockKeyType key;
			key.appKey		= pTask->_key.appKey;
			key.dataKey		= pTask->_key.dataKey;
			key.dataType	= pTask->_key.dataType;
			key.regionKey	= pTask->_key.regionKey;

			if (pTask->_eType == eSetData)
			{
				BlockDataType data;
				data.data			= pTask->_data.data;
				data.dataCheckSum	= pTask->_data.dataCheckSum;

				pModule->_pSoap->setData(&key, &data);
			}
			else if (pTask->_eType == eGetData)
			{
				string data;
				if( SOAP_OK == pModule->_pSoap->getData(&key, data) )
				{
					pModule->_pResult->OnGetDataOK(pTask->_key, data);
				}
				else
				{
					pModule->_pSoap->soap_stream_fault(std::cerr);
					pModule->_pResult->OnGetDataFailed(pTask->_key);
				}
			}
			else
			{
				string strSum;
				if( SOAP_OK == pModule->_pSoap->getDataCheckSum(&key, strSum) )
				{
					pModule->_pResult->OnGetDataCheckSumOK(pTask->_key, strSum);
				}
				else
				{
					pModule->_pSoap->soap_stream_fault(std::cerr);
					pModule->_pResult->OnGetDataCheckSumFailed(pTask->_key);
				}
			}

			delete pTask;
		}
		MUTEX_UNLOCK(module_cs);

		Sleep(1);
	}
}
