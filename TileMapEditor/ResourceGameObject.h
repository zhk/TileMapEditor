#pragma once

#include "ResourceType.h"

//游戏对象资源
class ResourceGameObject
{
	friend class ResourceGameObject_xmlHandler;
	friend class ResourceGameObjectGroup;
	friend class GameObjectEditor;
	friend class CDialogGameObject;
public:
	ResourceGameObject();
	~ResourceGameObject();

	//加载对象
	virtual bool		Load(const Cactus::String& strTile);
	void				Save(Cactus::XMLOutStream& xmlOut);
	void				CalculateBaryCentric();

	Cactus::String		GetName(){ return _strName; }
	Cactus::String		GetResID(){ return _ArtResID; }

protected:
	Cactus::String		_strName;		//名字
	Cactus::String		_ArtResID;		//依赖的子图像资源名称
	CPoint				_ptOffset;		//对象定位在(xOffset, yOffset)，图像左上角离定位点的偏差，定位点所在的格子是(0, 0)
	float				_xBaryCentric;	//所有阻挡点的均值
	float				_yBaryCentric;
	Cactus::String		_strAIType;		//AI类型

	ObstacleListType	_obstacles;		//所有的阻挡点

	//EGameObjectType		_eType;			//GameObject类型
};

//--------------------------------------------------------------------------------------------------------
/*
<!--
iconres是依赖的美术ResourceTile名称；
mapType表示用于的Map类型，0是矩形，1是菱形；
unitTileW和unitTileH用来检查是否和地图tile的尺寸是否匹配；
iconid是子资源名；
对象定位在(xOffset,yOffset)，图像左上角离定位点的偏差，定位点所在的格子是(0,0)；
重心坐标是所有阻挡点均值，float类型；
如果obstacle为空，对象不影响阻挡信息。
-->
*/
//游戏对象组，一个游戏对象组只能依赖一个图像资源对象
class ResourceGameObjectGroup : public PropertySys::SupportRTTI<ResourceGameObjectGroup, Resource>
{
	friend class ResourceManager;
	friend class ResourceGameObject_xmlHandler;
	friend class GameObjectEditor;
public:
	ResourceGameObjectGroup();
	ResourceGameObjectGroup(const Cactus::String& strName, const Cactus::String& strArtGroup, CPoint szTile, EGridType e);
	~ResourceGameObjectGroup();

	virtual bool			Load(){ return _ResGameObjects.size() != 0; }
	virtual void			CreateImageList(CDC* pDC, bool bForceRecreate = false);
	virtual void			Draw(CDC* pDC, GridObject* pGridObject, const CPoint& ptGrid, const Cactus::String& strID);
	virtual void			Draw(CDC* pDC, const CPoint& ptTopLeft, const Cactus::String& strID);

	virtual bool			IsResItemValid(const Cactus::String& strName);
	virtual CRect			GetResItemBoundingRect(const CRect& curTile, EGridType eGrid, const Cactus::String& strItemID);

	static void				RegisterReflection();
	virtual void			OnPropertyChanged(const std::string& propName);

	ResourceGameObject*		GetGameObject(const Cactus::String& strItemID);

	void					Save(Cactus::XMLOutStream& xmlOut);

	bool					UpdateGameObject(ResourceGameObject* pGO);
	bool					AddGameObject(ResourceGameObject* pGO);
	bool					RemoveGameObject(const Cactus::String& strName);

	EGridType				GetGridType(){ return _eGridType; }
	CSize					GetUnitTileSize(){ return _szUnitTile; }

	Cactus::String			GetResArtKey(){ return _strArtResKey; }

protected:
	typedef Cactus::list<ResourceGameObject*>::type		ResGameObjectListType;
	ResGameObjectListType	_ResGameObjects;

	Cactus::String			_strArtResKey;	//依赖的图像资源名称
	CPoint					_szUnitTile;	//单元格的大小
	EGridType				_eGridType;		//适用的地图类型
};


