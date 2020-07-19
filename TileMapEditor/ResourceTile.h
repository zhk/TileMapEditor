#pragma once

#include "ResourceType.h"

//ӵ�ж��ͼ������Դ������ʵ����
class ResourceTile: public PropertySys::SupportRTTI<ResourceTile, Resource>
{
	friend class ResourceManager;
	friend class ResourceArt_xmlHandler;
public:
	ResourceTile();
	~ResourceTile();

	static void				RegisterReflection();
	virtual void			OnPropertyChanged(const std::string& propName);

	//����Դ�Ƿ���Ч
	virtual bool			IsResItemValid(const Cactus::String& strName);
	
	virtual void			Save(Cactus::XMLOutStream& xmlOut){}

	virtual void			ScanAllResourceUsage(SubResRefCountMapType& subResIDCountInfo);

protected:
	int					_tilesCount;	//����Դ������

	typedef Cactus::hashmap<Cactus::String, CBitmap*>::type		IDBitmapMapType;
	IDBitmapMapType		_BitmapTiles;	//��������Դͼ���б���м�CBitmap����
};

