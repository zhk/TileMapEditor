#pragma once
#include "TGXML.h"
#include "GString.h"

typedef Cactus::vector<Cactus::String>::type StringVector;

COMMONUTIL_API extern StringVector split( const Cactus::String& str, const Cactus::String& delims = "\t\n ", unsigned int maxSplits = 0);

/*
XMLConfigReader用于读取XML配置，根据传入的XMLPATH，从ROOT下的节点开始找，对于：
<root>
	<net>
		<port>9800</port>
		<ip>127.0.0.1</ip>
</root>
相应的XMLPATH为：
	
	"net/port"
	"net/ip"

注意root是不需要填写的，良好的XML格式只需要一个ROOT节点。读取配置时如下：

XMLConfigReader config;
config.Load("xxx.xml");
int port = config.GetInt("net/port");

配置中不能有同级同名的节点，查找时只会找第一个名字符合的节点。
*/

class COMMONUTIL_API XMLConfigReader
{
public:

	XMLConfigReader(void);
	~XMLConfigReader(void);

	bool Load(const Cactus::String& xmlFile);
	Cactus::String GetString(const Cactus::String& xmlPath);
	bool GetBool(const Cactus::String& xmlPath);
	float GetFloat(const Cactus::String& xmlPath);
	int GetInt(const Cactus::String& xmlPath);

protected:

	Cactus::GXMLDoc	_doc;
	Cactus::GXMLElement _root;
};
