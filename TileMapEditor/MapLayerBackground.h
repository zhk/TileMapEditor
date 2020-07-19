#pragma once

#include "MapLayer.h"

//背景地层对象，里面对象按像素级别定位
class MapLayerBackground : public PropertySys::SupportRTTI<MapLayerBackground, MapLayer>
{
	friend class Map;
	friend class Map_xmlHandler;

public:
	MapLayerBackground();
	virtual ~MapLayerBackground();

	virtual void			Draw(CDC* pDC, const IntVectorType& regions);

	virtual bool			ToolHitTest(CPoint ptPixel, CPoint& ptGrid, CRect& rcPixel);
	virtual STile*			TileHitTest(CPoint ptPixel, CPoint& ptGrid);

	virtual bool			AddOrUpdateTile(CPoint ptGrid, const Cactus::String& resGroup, const Cactus::String& strItemID);
	virtual bool			MoveTile(STile* pTile, CPoint ptNewGrid);
	virtual bool			RemoveTile(CPoint ptGrid);
	virtual bool			RemoveTile(STile* pTile);

	virtual void			FillListInPanelMapLayer();
	virtual void			UpdateTileVisual(STile* pTile, bool bEnsureVisible = false);

	virtual void			Save(Cactus::XMLOutStream& xmlOut);
	virtual void			AddRegionTiles(int iRegion, const TileVectorType& tiles);

	virtual int				CheckResDepends();
	virtual void			CheckResourceUsage(StringResRefInfoMapType& mapGroupResRefInfo, StringResRefInfoMapType& groupSubResRefCountMap);

	virtual void			ExportToFile(const Cactus::String& strPathName);

protected:

	TileVectorType			_Tiles;
};