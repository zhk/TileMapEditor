#pragma once

#include "MapLayer.h"

//小块对象地层
class MapLayerTiles : public PropertySys::SupportRTTI<MapLayerTiles, MapLayer>
{
	friend class Map;
	friend class Map_xmlHandler;

public:
	MapLayerTiles();
	virtual ~MapLayerTiles();

	virtual void			Draw(CDC* pDC, const IntVectorType& regions);

	virtual STile*			TileHitTest(CPoint ptPixel, CPoint& ptGrid);

	virtual bool			AddOrUpdateTile(CPoint ptGrid, const Cactus::String& resGroup, const Cactus::String& strItemID);
	virtual bool			MoveTile(STile* pTile, CPoint ptNewGrid);
	virtual bool			RemoveTile(CPoint ptGrid);
	virtual bool			RemoveTile(STile* pTile);

	virtual void			FillListInPanelMapLayer();

	virtual void			Save(Cactus::XMLOutStream& xmlOut);
	virtual void			AddRegionTiles(int iRegion, const TileVectorType& tiles);

	virtual int				CheckResDepends();
	virtual void			CheckResourceUsage(StringResRefInfoMapType& mapGroupResRefInfo, StringResRefInfoMapType& groupSubResRefCountMap);

	virtual void			ExportToFile(const Cactus::String& strPathName);

protected:

	typedef Cactus::map<int, TileVectorType>::type		RegionTileMapType;
	RegionTileMapType	_GroupTiles;
};
