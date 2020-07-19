#include "StdAfx.h"
#include "ResourceManager.h"

#include "ResourceBackground.h"
#include "ResourceTile.h"
#include "ResourceTileSingleImage.h"
#include "ResourceTileFolder.h"
#include "ResourceGameObject.h"

using namespace Cactus;

ResourceManager* Cactus::Singleton<ResourceManager>::s_pSingleton = 0;

//---------------------------------------------------------------------------------------------------------

class ResourceArt_xmlHandler : public Cactus::XMLHandler
{
public:
	ResourceArt_xmlHandler(ResourceManager& resMan) : _resManager(resMan)
	{
		_bInBackground	= false;
		_bInTiles		= false;
	}
	virtual ~ResourceArt_xmlHandler(){}

	virtual void elementStart(const Cactus::String& element, const Cactus::XMLAttributes& attributes)
	{
		if (element == "background")
		{
			_bInBackground	= true;
		}
		else if (element == "tiles")
		{
			_bInTiles		= true;
		}
		else if (element == "item")
		{
			if (_bInBackground)
			{
				//<item name="bg1" res="art/background/bg1.png"/>
				ResourceBackground* p = new ResourceBackground;
				p->_strResName				= attributes.getValueAsString("name");
				p->_strImagePathName	= attributes.getValueAsString("res");
				p->_iIconSize			= attributes.getValueAsInteger("iconsize");

				if( p->Load() && _resManager._ResBackgrounds.find(p->_strResName) == _resManager._ResBackgrounds.end() )
				{
					_resManager._ResBackgrounds[p->_strResName] = p;
				}
				else
				{
					Log_Error("ResourceArt_xmlHandler::elementStart, ResourceBackground load failed! " << p->_strImagePathName);
					delete p;
				}
			}
			else if (_bInTiles)
			{
				String strType	= attributes.getValueAsString("type");
				if (strType == "image")
				{
					//<item type="image" name="tile1" file="art/tiles/tile1.png" tilew="32" tileh="32" count="80"/>
					ResourceTileSingleImage* p = new ResourceTileSingleImage;
					p->_strResName			= attributes.getValueAsString("name");
					p->_tileWidth		= attributes.getValueAsInteger("tilew");
					p->_tileHeight		= attributes.getValueAsInteger("tileh");
					p->_tilesCount		= attributes.getValueAsInteger("count");
					p->_strImageName	= attributes.getValueAsString("file");
					p->_iIconSize		= attributes.getValueAsInteger("iconsize");

					if( p->Load() && _resManager._ResourceTiles.find(p->_strResName) == _resManager._ResourceTiles.end() )
					{
						_resManager._ResourceTiles[p->_strResName] = p;
					}
					else
					{
						Log_Error("ResourceArt_xmlHandler::elementStart, ResourceTileSingleImage load failed! " << p->_strResName);
						delete p;
					}
				}
				else if (strType == "folder")
				{
					//<item type="folder" name="tile2" folder="art/tiles/tile2/" tilew="32" tileh="32" count="8" bits="2" ext=".png" />
					ResourceTileFolder* p = new ResourceTileFolder;
					p->_strResName			= attributes.getValueAsString("name");
					p->_strFolderName	= attributes.getValueAsString("folder");
					p->_strFileExt		= attributes.getValueAsString("ext");
					p->_iIconSize		= attributes.getValueAsInteger("iconsize");

					if( p->Load() && _resManager._ResourceTiles.find(p->_strResName) == _resManager._ResourceTiles.end() )
					{
						_resManager._ResourceTiles[p->_strResName] = p;
					}
					else
					{
						Log_Error("ResourceArt_xmlHandler::elementStart, ResourceTileFolder load failed! " << p->_strResName);
						delete p;
					}
				}
				else
				{
				}
			}
		}
	}

	virtual void elementEnd(const Cactus::String& element)
	{
		if (element == "background")
		{
			_bInBackground	= false;
		}
		else if (element == "tiles")
		{
			_bInTiles		= false;
		}
	}

	virtual void text(const Cactus::String& element, const Cactus::String& content){}

private:
	ResourceManager&	_resManager;
	bool				_bInBackground;
	bool				_bInTiles;
};

//---------------------------------------------------------------------------------------------------------

class ResourceGameObject_xmlHandler : public Cactus::XMLHandler
{
public:
	ResourceGameObject_xmlHandler(ResourceManager& resMan) : _resManager(resMan)
	{
		pGameObjectGroup	= 0;
		pGameObject			= 0;
	}
	virtual ~ResourceGameObject_xmlHandler(){}

	virtual void elementStart(const Cactus::String& element, const Cactus::XMLAttributes& attributes)
	{
		if ( element == "group")
		{
			//<group name="npc" iconres="tile2" unitTileW="100" unitTileH="50">
			pGameObjectGroup = new ResourceGameObjectGroup;
			pGameObjectGroup->_strResName		= attributes.getValueAsString("name");
			pGameObjectGroup->_strArtResKey = attributes.getValueAsString("iconres");
			pGameObjectGroup->_eGridType	= EGridType(attributes.getValueAsInteger("mapType"));
			pGameObjectGroup->_szUnitTile.x = attributes.getValueAsInteger("unitTileW");
			pGameObjectGroup->_szUnitTile.y = attributes.getValueAsInteger("unitTileH");
		}
		else if (element == "item")
		{
			//<item name="npc01" iconid="000" xOffset="25" yOffset="16" xBarycentric="0" yBarycentric="0" >
			//	<obstacle x="0" y="0"/>
			//	<obstacle x="0" y="1"/>
			//	<obstacle x="0" y="-1"/>
			//</item>
			ResourceGameObject* p = new ResourceGameObject;
			p->_strName			= attributes.getValueAsString("name");
			p->_ArtResID		= attributes.getValueAsString("iconid");
			p->_ptOffset.x		= attributes.getValueAsInteger("xOffset");
			p->_ptOffset.y		= attributes.getValueAsInteger("yOffset");
			p->_xBaryCentric	= attributes.getValueAsFloat("xBarycentric");
			p->_yBaryCentric	= attributes.getValueAsFloat("yBarycentric");
			p->_strAIType		= attributes.getValueAsString("AIType");
		
			if (p->Load(pGameObjectGroup->_strArtResKey))
			{
				pGameObjectGroup->_ResGameObjects.push_back(p);

				pGameObject = p;
			}
			else
			{
				Log_Error("ResourceGameObject_xmlHandler::elementStart, GameObject load failed! " << pGameObjectGroup->_strArtResKey << ", " << p->_strName);
				delete p;
			}
		}
		else if (element == "obstacle")
		{
			if (pGameObject)
			{
				CPoint pt;
				pt.x	= attributes.getValueAsInteger("x");
				pt.y	= attributes.getValueAsInteger("y");
				pGameObject->_obstacles.push_back(pt);
			}
		}
	}

	virtual void elementEnd(const Cactus::String& element)
	{
		if (element == "group")
		{
			if (pGameObjectGroup->Load())
				_resManager._ResGameObjectGroups[pGameObjectGroup->_strResName] = pGameObjectGroup;
			else
			{
				Log_Error("ResourceGameObject_xmlHandler::elementStart, GameObjectGroup is NULL! " << pGameObjectGroup->_strResName);
				delete pGameObjectGroup;
			}

			pGameObjectGroup = 0;
		}
		else if (element == "item")
		{
			pGameObject = 0;
		}
	}

	virtual void text(const Cactus::String& element, const Cactus::String& content){}

private:

	ResourceManager&			_resManager;
	ResourceGameObjectGroup*	pGameObjectGroup;
	ResourceGameObject*			pGameObject;
};

//---------------------------------------------------------------------------------------------------------
class TileTerrainType_xmlHandler : public Cactus::XMLHandler
{
public:
	TileTerrainType_xmlHandler(ResourceManager& res) : _resManager(res)
	{

	}
	virtual ~TileTerrainType_xmlHandler(){}

	virtual void elementStart(const Cactus::String& element, const Cactus::XMLAttributes& attributes)
	{
		if ( currentElementMatch("TileTerrainType/") )
		{
			_iCount = 0;
		}
		else if ( currentElementMatch("TileTerrainType/item/") )
		{
			ResourceManager::STerrainInfo info;
			info._strName	= attributes.getValueAsString("name");
			BYTE R = (BYTE)attributes.getValueAsInteger("R");
			BYTE G = (BYTE)attributes.getValueAsInteger("G");
			BYTE B = (BYTE)attributes.getValueAsInteger("B");
			info._refColor	= RGB(R, G, B);
			_resManager._IDTerrainMaps[_iCount++] = info;
		}

	}

	virtual void elementEnd(const Cactus::String& element)
	{
		if ( "TileTerrainType" == element)
		{
		}
	}

	virtual void text(const Cactus::String& element, const Cactus::String& content){}

protected:

	ResourceManager&	_resManager;
	int					_iCount;

};
//---------------------------------------------------------------------------------------------------------

ResourceManager::ColorBrushMapType ResourceManager::s_Brushes;

ResourceManager::ResourceManager()
: _bResArtLoadOK(false)
, _bGameObjectLoadOK(false)
{
}

ResourceManager::~ResourceManager()
{
	String strPathName;
	String strTmp;

	if (_bGameObjectLoadOK)
	{
		strPathName	= _strRootFolder + "Editor/ResourceGameObject.xml";
		strTmp		= _strRootFolder + "Editor/ResourceGameObject.xml.bak";

		try
		{
			CFile::Remove(strTmp.c_str());
		}
		catch (...)
		{
		}
		try
		{
			CFile::Rename(strPathName.c_str(), strTmp.c_str());
		}
		catch (...)
		{
		}
		_SaveResourceGameObject(strPathName);
	}

	if (_bResArtLoadOK)
	{
		strPathName	= _strRootFolder + "Editor/ResourceArt.xml";
		strTmp		= _strRootFolder + "Editor/ResourceArt.xml.bak";

		try
		{
			CFile::Remove(strTmp.c_str());
		}
		catch (...)
		{
		}
		try
		{
			CFile::Rename(strPathName.c_str(), strTmp.c_str());
		}
		catch (...)
		{
		}
		_SaveResourceArt(strPathName);
	}


	for (ColorBrushMapType::iterator it = s_Brushes.begin(); it != s_Brushes.end(); ++it)
	{
		it->second->DeleteObject();
		delete it->second;
	}

	Reset();
}

bool ResourceManager::_LoadResourceArt(const Cactus::String& strPathName)
{
	ResourceArt_xmlHandler handler(*this);

	try
	{
		XMLParser xmlParser(handler, strPathName, "");
	}
	catch (std::exception& e)
	{
		Log_Error("ResourceManager::_LoadResourceArt, Load failed! File : " << strPathName << ", Error: " << e.what() );
		return false;
	}

	Log_Info("ResourceManager::_LoadResourceArt, Load OK.");

	return true;
}

void ResourceManager::_SaveResourceArt(const Cactus::String& strPathName)
{
	DataOutStreamOS os;
	if( !os.Open(strPathName) )
	{
		Log_Error("ResourceManager::_SaveResourceArt, can not write file : " << strPathName);
		return;
	}

	String str = "<?xml version=\"1.0\" encoding=\"gb2312\"?>\r\n";
	//os << str;	//这样会先写个长度
	os.Write(&str[0], str.size());

	XMLOutStream xmlOut(&os);

	xmlOut.NodeBegin("resourceart");

		xmlOut.NodeBegin("background");
		ResBackgroundType::iterator it = _ResBackgrounds.begin();
		for(; it != _ResBackgrounds.end(); ++it)
		{
			ResourceBackground* pBK = it->second;
			pBK->Save(xmlOut);
		}
		xmlOut.NodeEnd("background");

		xmlOut.NodeBegin("tiles");
		ResTileType::iterator itTile = _ResourceTiles.begin();
		for(; itTile != _ResourceTiles.end(); ++itTile)
		{
			ResourceTile* pTile = itTile->second;
			pTile->Save(xmlOut);
		}
		xmlOut.NodeEnd("tiles");

	xmlOut.NodeEnd("resourceart");
	xmlOut.Flush();
}


bool ResourceManager::_LoadResourceGameObject(const Cactus::String& strPathName)
{
	ResourceGameObject_xmlHandler handler(*this);

	try
	{
		XMLParser xmlParser(handler, strPathName, "");
	}
	catch (std::exception& e)
	{
		Log_Error("ResourceManager::_LoadResourceGameObject, Load failed! File : " << strPathName << ", Error: " << e.what() );
		return false;
	}

	Log_Info("ResourceManager::_LoadResourceGameObject, Load OK.");

	return true;
}

bool ResourceManager::Load(const Cactus::String& strRootPath)
{
	_strRootFolder	= strRootPath;

	Cactus::String strPathName;

	strPathName = strRootPath + "Editor/ResourceArt.xml";
	_bResArtLoadOK = _LoadResourceArt(strPathName);
	if (!_bResArtLoadOK)
	{
		Reset();
		return false;
	}

	strPathName = strRootPath + "Editor/ResourceGameObject.xml";
	_bGameObjectLoadOK = _LoadResourceGameObject(strPathName);
	if (!_bGameObjectLoadOK)
	{
		Reset();
		return false;
	}


	strPathName = strRootPath + "Editor/TileTerrainType.xml";
	if ( !_LoadTerrainTypes(strPathName) )
	{
		return false;
	}

	return true;
}

void delete_resourceBackground(std::pair<Cactus::String, ResourceBackground*> p)
{
	delete p.second;
}

void delete_resourceTile(std::pair<Cactus::String, ResourceTile*> p)
{
	delete p.second;
}

void delete_resourceGameObjectGroup(std::pair<Cactus::String, ResourceGameObjectGroup*> p)
{
	delete p.second;
}

void ResourceManager::Reset()
{
	for_each(_ResBackgrounds.begin(), _ResBackgrounds.end(), delete_resourceBackground);
	_ResBackgrounds.clear();

	for_each(_ResourceTiles.begin(), _ResourceTiles.end(), delete_resourceTile);
	_ResourceTiles.clear();

	for_each(_ResGameObjectGroups.begin(), _ResGameObjectGroups.end(), delete_resourceGameObjectGroup);
	_ResGameObjectGroups.clear();
}

bool ResourceManager::IsResourceTileIDValid(const Cactus::String& tile, const Cactus::String& strID)
{
	ResTileType::iterator it = _ResourceTiles.find(tile);
	if (it != _ResourceTiles.end())
	{
		ResourceTile* pTile = it->second;
		return pTile->IsResItemValid(strID);
	}

	return false;
}

ResourceTile* ResourceManager::GetResourceTileGroup(const Cactus::String& strGroupName)
{
	ResTileType::iterator it = _ResourceTiles.find(strGroupName);
	if (it != _ResourceTiles.end())
	{
		return it->second;
	}

	return 0;
}

Resource* ResourceManager::GetResourceArtGroup(const Cactus::String& key)
{
	if (_ResBackgrounds.find(key) != _ResBackgrounds.end())
	{
		return _ResBackgrounds[key];
	}

	if (_ResourceTiles.find(key) != _ResourceTiles.end())
	{
		return _ResourceTiles[key];
	}

	return 0;
}

Resource* ResourceManager::GetResourceArtOrGOGroup(const Cactus::String& key)
{
	Resource* pRes = ResourceManager::getSingleton().GetResourceArtGroup(key);
	if (!pRes)
		pRes = ResourceManager::getSingleton().GetResourceGameObjectGroup(key);

	return pRes;
}

bool ResourceManager::AddResourceTileGroup(ResourceTile* p)
{
	if (_ResourceTiles.find(p->_strResName) != _ResourceTiles.end())
	{
		Log_Error("同名美术资源组已存在！" << p->_strName);
		return false;
	}

	_ResourceTiles[p->_strResName] = p;

	return true;
}


bool ResourceManager::RemoveResourceTile(const Cactus::String& strName)
{
	ResTileType::iterator it = _ResourceTiles.find(strName);
	if(it != _ResourceTiles.end())
	{
		delete it->second;
		_ResourceTiles.erase(it);

		return true;
	}

	Log_Error("资源组不存在！" << strName);

	return false;
}


ResourceGameObjectGroup* ResourceManager::GetResourceGameObjectGroup(const Cactus::String& key)
{
	if (_ResGameObjectGroups.find(key) != _ResGameObjectGroups.end())
	{
		return _ResGameObjectGroups[key];
	}

	return 0;
}

Cactus::StringVector ResourceManager::GetResourceTileNames()
{
	StringVector vs;

	for (ResTileType::iterator it = _ResourceTiles.begin(); it != _ResourceTiles.end(); ++it)
	{
		vs.push_back(it->first);
	}

	return vs;
}

bool ResourceManager::IsGameObjectGroupNameValid(const Cactus::String& strName)
{
	return (_ResGameObjectGroups.find(strName) == _ResGameObjectGroups.end());
}

bool ResourceManager::AddGameObjectGroup(ResourceGameObjectGroup* p)
{
	if (
		_ResourceTiles.find(p->_strResName) != _ResourceTiles.end()
		||
		_ResGameObjectGroups.find(p->_strResName) != _ResGameObjectGroups.end()
		)
	{
		Log_Error("游戏对象组不能重名，也不能和美术资源组重名！" << p->_strName);
		return false;
	}

	_ResGameObjectGroups[p->_strResName] = p;

	return true;
}


bool ResourceManager::RemoveGameObjectGroup(const Cactus::String& strName)
{
	ResGameObjectGroupMapType::iterator it = _ResGameObjectGroups.find(strName);
	if(it != _ResGameObjectGroups.end())
	{
		delete it->second;
		_ResGameObjectGroups.erase(it);
		
		return true;
	}

	Log_Error("资源组不存在！" << strName);

	return false;
}

void ResourceManager::_SaveResourceGameObject(const Cactus::String& strPathName)
{
	DataOutStreamOS os;
	if( !os.Open(strPathName) )
	{
		Log_Error("ResourceManager::_SaveResourceGameObject, can not write file : " << strPathName);
		return;
	}

	String str = "<?xml version=\"1.0\" encoding=\"gb2312\"?>\r\n";
	//os << str;	//这样会先写个长度
	os.Write(&str[0], str.size());

	XMLOutStream xmlOut(&os);

	xmlOut.NodeBegin("resoucegameobject");

	ResGameObjectGroupMapType::iterator it = _ResGameObjectGroups.begin();
	for(; it != _ResGameObjectGroups.end(); ++it)
	{
		ResourceGameObjectGroup* pGOGroup = it->second;
		pGOGroup->Save(xmlOut);
	}
	xmlOut.NodeEnd("resoucegameobject");

	xmlOut.Flush();
}

bool ResourceManager::_LoadTerrainTypes(const Cactus::String& pathName)
{
	TileTerrainType_xmlHandler handler(*this);

	try
	{
		XMLParser xmlParser(handler, pathName, "");
	}
	catch (std::exception e)
	{
		Log_Error( "ResourceManager::_LoadTerrainTypes, Load failed! File : " << pathName );
		return false;
	}

	return true;
}

bool ResourceManager::IsDefaultTerrainType(const Cactus::String& name) const
{
	if (_IDTerrainMaps.size() == 0)
	{
		return false;
	}

	return (name == _IDTerrainMaps.begin()->second._strName);
}

COLORREF ResourceManager::GetTerrainColor(const Cactus::String& name) const
{

	for (IDTerrainMapType::const_iterator cit = _IDTerrainMaps.begin(); cit != _IDTerrainMaps.end(); ++cit)
	{
		if (cit->second._strName == name)
		{
			return cit->second._refColor;
		}
	}

	return 0;
}
