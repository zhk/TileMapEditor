//gsoap ns2 service name:	DataCenterServer
//gsoap ns2 service style:	rpc
//gsoap ns2 service encoding:	encoded
//gsoap ns2 service namespace:	http://localhost/DataCenterServer.wsdl
//gsoap ns2 service location:	http://localhost:8090

//gsoap ns2 schema namespace: urn:DataCenter

#import "stlvector.h"
#define SOAP_NAMESPACE_OF_ns2	"urn:DataCenter"

typedef std::string xsd__string;

struct BlockKeyType
{
	xsd__string	appKey;		//Ӧ�ó���ؼ���                                                                                            
	xsd__string	regionKey;	//�����ؼ���                                                                                                
	xsd__string	dataType;	//�������ͣ���Һ���������һ���������ͣ���������Ҳ��һ���������ͣ�
	xsd__string	dataKey;	//���ݼ�¼�ؼ��֣��������û�ID��Ҳ�������û�ID + ��ɫID��Ҳ���������˺š�ֻ�������������²���Ч��
};

struct BlockDataType
{
	xsd__string	dataCheckSum;	//����У���                                                                                                
	xsd__string	data;		//����                                                                                                      
};

int ns2__setData(struct BlockKeyType* key, struct BlockDataType* blockdata, void);
int ns2__getData(struct BlockKeyType* key, xsd__string& data);
int ns2__getDataCheckSum(struct BlockKeyType* key, xsd__string& checkSum);


int ns2__add(
    double                              a,	///< Request parameter
    double                              b,	///< Request parameter
    double                             &result	///< Response parameter
);


