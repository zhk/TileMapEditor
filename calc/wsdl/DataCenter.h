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
	xsd__string	appKey;		//应用程序关键字                                                                                            
	xsd__string	regionKey;	//分区关键字                                                                                                
	xsd__string	dataType;	//数据类型，玩家好友数据是一种数据类型；联盟数据也是一种数据类型；
	xsd__string	dataKey;	//数据记录关键字，可以是用户ID，也可以是用户ID + 角色ID，也可以是联盟号。只有在数据类型下才有效；
};

struct BlockDataType
{
	xsd__string	dataCheckSum;	//数据校验和                                                                                                
	xsd__string	data;		//数据                                                                                                      
};

int ns2__setData(struct BlockKeyType* key, struct BlockDataType* blockdata, void);
int ns2__getData(struct BlockKeyType* key, xsd__string& data);
int ns2__getDataCheckSum(struct BlockKeyType* key, xsd__string& checkSum);


int ns2__add(
    double                              a,	///< Request parameter
    double                              b,	///< Request parameter
    double                             &result	///< Response parameter
);


