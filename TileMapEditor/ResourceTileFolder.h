#pragma once

#include "ResourceTile.h"

//原始子资源图像在一个子目录中，所有图像必须有相同的后缀，但是可以有不同大小
class ResourceTileFolder : public PropertySys::SupportRTTI<ResourceTileFolder, ResourceTile>
{
	friend class ResourceManager;
	friend class ResourceArt_xmlHandler;
public:
	ResourceTileFolder();
	ResourceTileFolder(const Cactus::String& strName, const Cactus::String& strArtResName, const Cactus::String& strExt, int iIconSize);
	~ResourceTileFolder();

	virtual bool			Load();
	virtual void			CreateImageList(CDC* pDC, bool bForceRecreate = false);
	virtual void			Draw(CDC* pDC, GridObject* pGridObject, const CPoint& ptGrid, const Cactus::String& strID);
	virtual void			Draw(CDC* pDC, const CPoint& ptTopLeft, const Cactus::String& strID);
	virtual CRect			GetResItemBoundingRect(const CRect& curTile, EGridType eGrid, const Cactus::String& strID);

	static void				RegisterReflection();
	virtual void			OnPropertyChanged(const std::string& propName);

	virtual void			Save(Cactus::XMLOutStream& xmlOut);

	virtual Cactus::String	GetSubResFileName(const Cactus::String& strID);
	virtual Cactus::String	GetResFolder(){ return _strFolderName; }

protected:
	Cactus::String		_strFolderName;		//目录名
	Cactus::String		_strFileExt;		//后缀名，不含'.'

	typedef Cactus::hashmap<Cactus::String, CxImage*>::type	IDImageMapType;
	IDImageMapType		_images;			//读入内存的所有子资源图像对象
};