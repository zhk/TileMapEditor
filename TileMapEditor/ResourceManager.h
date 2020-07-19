#pragma once

#include "ResourceType.h"

class ResourceBackground;
class ResourceTile;
class ResourceGameObjectGroup;

//��Դ������
class ResourceManager : public Cactus::Singleton<ResourceManager>
{
	friend class ResourceArt_xmlHandler;
	friend class ResourceGameObject_xmlHandler;
	friend class TileTerrainType_xmlHandler;
	friend class ResourceTreeView;

public:
	ResourceManager();
	~ResourceManager();

	//������Դ���ú���Դ
	bool	Load(const Cactus::String& strRootPath);

	//����������Դ
	void	Reset();

	//��ȡ���ݸ�Ŀ¼
	Cactus::String	GetRootFolder(){ return _strRootFolder; }

	//ResourceTile������Դʮ����Ч
	bool			IsResourceTileIDValid(const Cactus::String& tile, const Cactus::String& strID);


	//��ȡ����ΪstrGroupName��ResourceTileGroup
	ResourceTile*	GetResourceTileGroup(const Cactus::String& strGroupName);

	/**����һ������ΪstrName��Tile������
	*/
	bool			AddResourceTileGroup(ResourceTile* p);

	/**ɾ������ΪstrName��Tile������
	*/
	bool			RemoveResourceTile(const Cactus::String& strName);

	//��ȡ����Ϊkey��ArtResource
	Resource*		GetResourceArtGroup(const Cactus::String& key);

	//��ȡ����Ϊkey��ArtResource����GameObject
	Resource*		GetResourceArtOrGOGroup(const Cactus::String& key);

	/**��ȡ����Ϊ����ResourceTile����
	*/
	Cactus::StringVector	GetResourceTileNames();



	/**�ж�GameObjectGroup�����Ƿ���Ч
	*/
	bool			IsGameObjectGroupNameValid(const Cactus::String& strName);

	/**����һ������ΪstrName����Ϸ������
	*/
	bool			AddGameObjectGroup(ResourceGameObjectGroup* p);

	//��ȡ����Ϊkey��ResourceGameObjectGroup
	ResourceGameObjectGroup*		GetResourceGameObjectGroup(const Cactus::String& key);

	/**ɾ������ΪstrName����Ϸ������
	*/
	bool			RemoveGameObjectGroup(const Cactus::String& strName);


	struct STerrainInfo 
	{
		Cactus::String		_strName;
		COLORREF			_refColor;
	};
	typedef Cactus::map<int, STerrainInfo>::type	IDTerrainMapType;
	const IDTerrainMapType&	GetTerrainTypes() const { return _IDTerrainMaps; }

	bool			IsDefaultTerrainType(const Cactus::String& name) const;
	COLORREF		GetTerrainColor(const Cactus::String& name) const;

	typedef  Cactus::map<COLORREF, CBrush*>::type	ColorBrushMapType;
	static	ColorBrushMapType s_Brushes;

protected:
	Cactus::String				_strRootFolder;			//���ݸ�Ŀ¼

	typedef	Cactus::map<Cactus::String, ResourceBackground*>::type		ResBackgroundType;
	ResBackgroundType			_ResBackgrounds;		//���еı�����Դ

	typedef	Cactus::map<Cactus::String, ResourceTile*>::type			ResTileType;
	ResTileType					_ResourceTiles;			//���е�ResourcTile��Դ

	typedef Cactus::map<Cactus::String, ResourceGameObjectGroup*>::type	ResGameObjectGroupMapType;
	ResGameObjectGroupMapType	_ResGameObjectGroups;	//���е�GameObjectGroup��Դ

	bool	_bResArtLoadOK;
	bool	_LoadResourceArt(const Cactus::String& strPathName);
	void	_SaveResourceArt(const Cactus::String& strPathName);

	bool	_bGameObjectLoadOK;
	bool	_LoadResourceGameObject(const Cactus::String& strPathName);
	void	_SaveResourceGameObject(const Cactus::String& strPathName);


	IDTerrainMapType	_IDTerrainMaps;
	bool	_LoadTerrainTypes(const Cactus::String& pathName);
};
