#pragma once

#include <string>

struct SBlockKeyType
{
	std::string appKey;			//Ӧ�ó���ؼ���
	std::string regionKey;		//�����ؼ���
	std::string dataType;		//�������ͣ���Һ���������һ���������ͣ���������Ҳ��һ���������ͣ�
	std::string dataKey;		//���ݼ�¼�ؼ��֣��������û�ID��Ҳ�������û�ID + ��ɫID��Ҳ���������˺š�ֻ�������������²���Ч��
};

struct SBlockDataType
{
	std::string dataCheckSum;	//����У��� 
	std::string data;			//����  
};

//�ýӿڻ��ڶ��߳�������ã�ע�⻥�Ᵽ��
class IWebServiceServeResult
{
public:
	//���ػ�ȡ������
	virtual void	OnGetDataOK(const SBlockKeyType& key, const std::string& data) = 0;
	virtual void	OnGetDataFailed(const SBlockKeyType& key) = 0;

	//���ػ�ȡ��У���
	virtual void	OnGetDataCheckSumOK(const SBlockKeyType& key, const std::string& checkSum) = 0;
	virtual void	OnGetDataCheckSumFailed(const SBlockKeyType& key) = 0;
};
