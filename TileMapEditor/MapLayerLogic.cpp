#include "StdAfx.h"
#include "MapLayerLogic.h"
#include "Map.h"
#include "ResourceManager.h"

using namespace Cactus;
using namespace PropertySys;

//------------------------------------------------------------------------------------------
MapLayerTerrain::MapLayerTerrain()
{
	_eLayerType = eMapLayerTerrain;
}

MapLayerTerrain::~MapLayerTerrain()
{
}

int MapLayerTerrain::CheckResDepends()
{
	return 0;
}


void MapLayerTerrain::Draw(CDC* pDC, const IntVectorType& regions)
{
	if (!_bVisible || !_pParentMap)
		return;

	// 只绘制区域内容
	Cactus::list<STile*>::type	tilesList;
	for (size_t t = 0; t < regions.size(); ++t)
	{
		TileVectorType& tiles = _GroupTiles[regions[t]];

		for (size_t k = 0; k < tiles.size(); ++k)
		{
			STile* pTile = tiles[k];
			tilesList.push_back(pTile);
		}
	}
	//if (tilesList.size() > 0)
	//	tilesList.sort(_pParentMap->GetGridType() == eRectangle ? STile_Triangle_less : STile_Diamond_less);

	if (tilesList.size() == 0)
		return;

	CPen pen(PS_SOLID, 1, RGB(231, 165, 25));
	CPen* pOldPen = pDC->SelectObject(&pen);

	//pDC->SelectStockObject(NULL_BRUSH);

	Cactus::list<CRect>::type PostDrawSelectedRectangle;

	for (Cactus::list<STile*>::type::iterator it = tilesList.begin(); it != tilesList.end(); ++it)
	{
		STile* pTile = *it;

		if ( !ResourceManager::getSingleton().IsDefaultTerrainType(pTile->_strResItemID) )
		{
			COLORREF refColor = ResourceManager::getSingleton().GetTerrainColor(pTile->_strResItemID);
			GetParentMap()->FillGrid(pDC, pTile->_ptGrid, refColor);

			GetParentMap()->DrawGridText(pDC, pTile->_ptGrid, RGB(255, 0, 255), pTile->_strResItemID);
		}

		if (pTile->_bSelected)
		{
		}
	}

	pDC->SelectObject(pOldPen);
}

bool MapLayerTerrain::AddOrUpdateTile( CPoint ptGrid, const Cactus::String& resGroup, const Cactus::String& strItemID )
{
	int regionID = _pParentMap->GetRegionID(ptGrid);
	if( regionID == -1 )
		return false;

	TileVectorType& tiles = _GroupTiles[regionID];

	bool bIsDefault = ResourceManager::getSingleton().IsDefaultTerrainType(strItemID);

	bool bFound = false;
	for (size_t t = 0; t < tiles.size(); ++t)
	{
		STile* pTile = tiles[t];

		if(pTile->_ptGrid == ptGrid)
		{
			if(pTile->_strResItemID == strItemID && resGroup == pTile->_strResGroup)
			{
				return false;
			}
			else
			{
				pTile->_strResGroup		= resGroup;
				pTile->_strResItemID	= strItemID;

				if (bIsDefault)
				{
					//UpdateTileInfoInPanelMapLayer(pTile, eTileOpRemove);
					RemoveTile(pTile);
				}
				else
				{
					UpdateTileInfoInPanelMapLayer(pTile, eTileOpUpdate);
					UpdateTileVisual(pTile, true);
				}

				return true;
			}
		}
	}

	if (!bFound && !bIsDefault)
	{
		STile* pTile = new STile;
		pTile->_ptGrid			= ptGrid;
		pTile->_strResItemID	= strItemID;
		pTile->_strResGroup		= resGroup;
		pTile->_regionID		= regionID;

		tiles.push_back(pTile);

		UpdateTileInfoInPanelMapLayer(pTile, eTileOpAdd);

		UpdateTileVisual(pTile, true);
	}

	return true;
}

//------------------------------------------------------------------------------------------
MapLayerMonster::MapLayerMonster()
{
	_eLayerType = eMapLayerMonster;
}

MapLayerMonster::~MapLayerMonster()
{
}

//------------------------------------------------------------------------------------------
MapLayerEvent::MapLayerEvent()
{
	_eLayerType = eMapLayerEvent;
}

MapLayerEvent::~MapLayerEvent()
{
}

//------------------------------------------------------------------------------------------
