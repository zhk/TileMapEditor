#pragma once

#include "ResourceType.h"

//拥有多个图像子资源，不可实例化
class ResourceTile: public PropertySys::SupportRTTI<ResourceTile, Resource>
{
	friend class ResourceManager;
	friend class ResourceArt_xmlHandler;
public:
	ResourceTile();
	~ResourceTile();

	static void				RegisterReflection();
	virtual void			OnPropertyChanged(const std::string& propName);

	//子资源是否有效
	virtual bool			IsResItemValid(const Cactus::String& strName);
	
	virtual void			Save(Cactus::XMLOutStream& xmlOut){}

	virtual void			ScanAllResourceUsage(SubResRefCountMapType& subResIDCountInfo);

protected:
	int					_tilesCount;	//子资源的数量

	typedef Cactus::hashmap<Cactus::String, CBitmap*>::type		IDBitmapMapType;
	IDBitmapMapType		_BitmapTiles;	//生成子资源图像列表的中间CBitmap对象
};

