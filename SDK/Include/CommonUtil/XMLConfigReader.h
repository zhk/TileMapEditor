#pragma once
#include "TGXML.h"
#include "GString.h"

typedef Cactus::vector<Cactus::String>::type StringVector;

COMMONUTIL_API extern StringVector split( const Cactus::String& str, const Cactus::String& delims = "\t\n ", unsigned int maxSplits = 0);

/*
XMLConfigReader���ڶ�ȡXML���ã����ݴ����XMLPATH����ROOT�µĽڵ㿪ʼ�ң����ڣ�
<root>
	<net>
		<port>9800</port>
		<ip>127.0.0.1</ip>
</root>
��Ӧ��XMLPATHΪ��
	
	"net/port"
	"net/ip"

ע��root�ǲ���Ҫ��д�ģ����õ�XML��ʽֻ��Ҫһ��ROOT�ڵ㡣��ȡ����ʱ���£�

XMLConfigReader config;
config.Load("xxx.xml");
int port = config.GetInt("net/port");

�����в�����ͬ��ͬ���Ľڵ㣬����ʱֻ���ҵ�һ�����ַ��ϵĽڵ㡣
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
