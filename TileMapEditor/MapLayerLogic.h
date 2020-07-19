#pragma once

#include "MapLayerTiles.h"

//------------------------------------------------------------------------------------------
class MapLayerTerrain : public PropertySys::SupportRTTI<MapLayerTerrain, MapLayerTiles>
{
	friend class Map;
	friend class Map_xmlHandler;

public:
	MapLayerTerrain();
	virtual ~MapLayerTerrain();

	virtual void			Draw(CDC* pDC, const IntVectorType& regions);
	virtual bool			AddOrUpdateTile(CPoint ptGrid, const Cactus::String& resGroup, const Cactus::String& strItemID);

	virtual int				CheckResDepends();

protected:

};

//------------------------------------------------------------------------------------------
class MapLayerMonster : public PropertySys::SupportRTTI<MapLayerMonster, MapLayerTiles>
{
	friend class Map;
	friend class Map_xmlHandler;

public:
	MapLayerMonster();
	virtual ~MapLayerMonster();

protected:

};

//------------------------------------------------------------------------------------------
class MapLayerEvent : public PropertySys::SupportRTTI<MapLayerEvent, MapLayerTiles>
{
	friend class Map;
	friend class Map_xmlHandler;

public:
	MapLayerEvent();
	virtual ~MapLayerEvent();

protected:

};
