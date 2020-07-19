#pragma once

#include <string>

struct SBlockKeyType
{
	std::string appKey;			//应用程序关键字
	std::string regionKey;		//分区关键字
	std::string dataType;		//数据类型，玩家好友数据是一种数据类型；联盟数据也是一种数据类型；
	std::string dataKey;		//数据记录关键字，可以是用户ID，也可以是用户ID + 角色ID，也可以是联盟号。只有在数据类型下才有效；
};

struct SBlockDataType
{
	std::string dataCheckSum;	//数据校验和 
	std::string data;			//数据  
};

//该接口会在多线程里面调用，注意互斥保护
class IWebServiceServeResult
{
public:
	//返回获取的数据
	virtual void	OnGetDataOK(const SBlockKeyType& key, const std::string& data) = 0;
	virtual void	OnGetDataFailed(const SBlockKeyType& key) = 0;

	//返回获取的校验和
	virtual void	OnGetDataCheckSumOK(const SBlockKeyType& key, const std::string& checkSum) = 0;
	virtual void	OnGetDataCheckSumFailed(const SBlockKeyType& key) = 0;
};
