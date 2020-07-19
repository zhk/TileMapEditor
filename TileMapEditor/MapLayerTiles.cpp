#include "StdAfx.h"
#include "MapLayerTiles.h"
#include "ResourceManager.h"
#include "Map.h"
#include "ResourceGameObject.h"

#include "MainFrm.h"
#include "TileMapEditorView.h"

using namespace Cactus;
using namespace PropertySys;


MapLayerTiles::MapLayerTiles()
{
	_eLayerType = eMapLayerTiles;
}

MapLayerTiles::~MapLayerTiles()
{
	for (RegionTileMapType::iterator it = _GroupTiles.begin(); it != _GroupTiles.end(); ++it)
	{
		TileVectorType& tiles = it->second;

		for (size_t t = 0; t < tiles.size(); ++t)
		{
			delete tiles[t];
		}
	}
}


int MapLayerTiles::CheckResDepends()
{
	int iCount = 0;

	for (RegionTileMapType::iterator it = _GroupTiles.begin(); it != _GroupTiles.end(); ++it)
	{
		TileVectorType& tiles = it->second;

		for (size_t t = 0; t < tiles.size(); ++t)
		{
			STile* pTile = tiles[t];

			Resource* pRes = ResourceManager::getSingleton().GetResourceArtOrGOGroup(pTile->_strResGroup);
			if (!pRes)
			{
				Log_Error("资源组 '" << pTile->_strResGroup << "' 不存在！");
				pTile->_bValid = false;
				iCount++;
				continue;
			}

			if( !pRes->IsResItemValid(pTile->_strResItemID) )
			{
				Log_Error("资源组 '" << pTile->_strResGroup << "' 的资源 '" << pTile->_strResItemID << "' 不存在！");
				pTile->_bValid = false;
				iCount++;
			}
		}
	}

	return iCount;
}


void MapLayerTiles::Draw(CDC* pDC, const IntVectorType& regions)
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
	if (tilesList.size() > 0)
		tilesList.sort(_pParentMap->GetGridType() == eRectangle ? STile_Triangle_less : STile_Diamond_less);
	else
		return;

	CPen pen(PS_SOLID, 1, RGB(231, 165, 25));
	CPen* pOldPen = pDC->SelectObject(&pen);
	pDC->SelectStockObject(NULL_BRUSH);

	Cactus::list<CRect>::type PostDrawSelectedRectangle;

	for (Cactus::list<STile*>::type::iterator it = tilesList.begin(); it != tilesList.end(); ++it)
	{
		STile* pTile = *it;

		Resource* pRes = ResourceManager::getSingleton().GetResourceArtOrGOGroup(pTile->_strResGroup);
		if (!pRes)
		{
			//Log_Error("资源组 '" << pTile->_strResGroup << "' 不存在！");
			continue;
		}

		pRes->Draw(pDC, _pParentMap, pTile->_ptGrid, pTile->_strResItemID);

		if (pTile->_bSelected)
		{
			CRect rcPixel	= _pParentMap->GetPixelCoordRect(pTile->_ptGrid);
			CRect rcDest	= pRes->GetResItemBoundingRect(rcPixel, _pParentMap->GetGridType(), pTile->_strResItemID);
			PostDrawSelectedRectangle.push_back(rcDest);
		}
	}

	for (Cactus::list<CRect>::type::iterator it = PostDrawSelectedRectangle.begin();
		it != PostDrawSelectedRectangle.end();
		++it)
	{
		pDC->Rectangle(*it);
	}

	pDC->SelectObject(pOldPen);
}


STile* MapLayerTiles::TileHitTest(CPoint ptPixel, CPoint& ptGrid)
{
	bool bInRegion = _pParentMap->GetGridCoord(ptPixel, ptGrid);
	if (!bInRegion)
		return 0;

	int regionID = _pParentMap->GetRegionID(ptGrid);
	if( regionID == -1 )
		return 0;

	TileVectorType& tiles = _GroupTiles[regionID];

	//先判断点击点是否有Tile
	for (size_t t = 0; t < tiles.size(); ++t)
	{
		STile* pTile = tiles[t];

		if(pTile->_ptGrid == ptGrid)
		{
			return pTile;
		}
	}

	IntVectorType IDs = _pParentMap->GetAdjacentRegions(regionID);
	//IDs.insert(IDs.begin(), regionID);
	IDs.push_back(regionID);

	Cactus::list<STile*>::type	tilesList;

	//再判断点击点是否在某个Tile的包围盒范围内；搜索当前Region和增加相邻Region
	for (size_t k = 0; k < IDs.size(); ++k)
	{
		TileVectorType& tiles = _GroupTiles[IDs[k]];

		for (size_t t = 0; t < tiles.size(); ++t)
		{
			STile* pTile = tiles[t];

			Resource* pRes = ResourceManager::getSingleton().GetResourceArtOrGOGroup(pTile->_strResGroup);
			if (!pRes)
			{
				//Log_Error("资源组 '" << pTile->_strResGroup << "' 不存在！");
				continue;
			}

			CRect rcPixelGrid = _pParentMap->GetPixelCoordRect(pTile->_ptGrid);
			CRect rcDest = pRes->GetResItemBoundingRect(rcPixelGrid, _pParentMap->GetGridType(), pTile->_strResItemID);

			if (rcDest.PtInRect(ptPixel))
			{
				tilesList.push_back(pTile);
			}
		}
	}
	//找出最上面的那个
	if (tilesList.size() > 0)
	{
		tilesList.sort(_pParentMap->GetGridType() == eRectangle ? STile_Triangle_less : STile_Diamond_less);

		return tilesList.back();
	}

	return 0;
}


bool MapLayerTiles::AddOrUpdateTile(CPoint ptGrid, const Cactus::String& resGroup, const Cactus::String& strItemID)
{
	int regionID = _pParentMap->GetRegionID(ptGrid);
	if( regionID == -1 )
		return false;

	TileVectorType& tiles = _GroupTiles[regionID];

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

				UpdateTileInfoInPanelMapLayer(pTile, eTileOpUpdate);

				UpdateTileVisual(pTile, true);

				return true;
			}
		}
	}

	if (!bFound)
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


bool MapLayerTiles::MoveTile(STile* pTile, CPoint ptNewGrid)
{
	int regionID = _pParentMap->GetRegionID(ptNewGrid);
	if( regionID == -1 )
		return false;

	if (regionID != pTile->_regionID)
	{
		TileVectorType& tiles = _GroupTiles[pTile->_regionID];

		for (size_t t = 0; t < tiles.size(); ++t)
		{
			if(pTile == tiles[t])
			{
				tiles.erase(tiles.begin() + t);
				break;
			}
		}

		_GroupTiles[regionID].push_back(pTile);
	}

	STile tileOld = *pTile;

	pTile->_regionID	= regionID;
	pTile->_ptGrid		= ptNewGrid;

	UpdateTileInfoInPanelMapLayer(pTile, eTileOpUpdate);

	UpdateTileVisual(&tileOld);
	UpdateTileVisual(pTile, false);

	return true;
}


bool MapLayerTiles::RemoveTile(CPoint ptGrid)
{
	int regionID = _pParentMap->GetRegionID(ptGrid);
	if( regionID == -1 )
		return false;

	TileVectorType& tiles = _GroupTiles[regionID];

	if (tiles.size() == 0)
		return false;

	bool bFound = false;
	for (size_t t = 0; t < tiles.size(); )
	{
		STile* pTile = tiles[t];
		if(pTile->_ptGrid == ptGrid)
		{
			tiles.erase(tiles.begin() + t);

			UpdateTileInfoInPanelMapLayer(pTile, eTileOpRemove);

			UpdateTileVisual(pTile);

			delete pTile;
			bFound = true;
		}
		else
		{
			++t;
		}
	}

	return bFound;
}


bool MapLayerTiles::RemoveTile(STile* pTile)
{
	for (RegionTileMapType::iterator it = _GroupTiles.begin(); it != _GroupTiles.end(); ++it)
	{
		TileVectorType& tiles = it->second;

		for (size_t t = 0; t < tiles.size(); ++t)
		{
			if(tiles[t] == pTile)
			{
				tiles.erase(tiles.begin() + t);

				UpdateTileInfoInPanelMapLayer(pTile, eTileOpRemove);

				UpdateTileVisual(pTile);

				delete pTile;

				return true;

			}
		}
	}

	return false;
}


void MapLayerTiles::FillListInPanelMapLayer()
{
	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST( CMainFrame, AfxGetMainWnd() );
	if (!pMainFrame)
		return;

	LayerView* pView = pMainFrame->GetMapLayerView(); 
	if (!pView)
		return;

	for (RegionTileMapType::iterator it = _GroupTiles.begin(); it != _GroupTiles.end(); ++it)
	{
		TileVectorType& tiles = it->second;

		for (size_t t = 0; t < tiles.size(); ++t)
		{
			pView->AddTileInfo(tiles[t]);
		}
	}

}

void MapLayerTiles::Save(Cactus::XMLOutStream& xmlOut)
{
	xmlOut.NodeBegin("layer");

	xmlOut.AddAttribute("name", _strName);
	xmlOut.AddAttribute("type", _eLayerType);
	xmlOut.AddAttribute("enable", _bEnable);
	xmlOut.AddAttribute("visible", _bVisible);

	for (RegionTileMapType::iterator itG = _GroupTiles.begin(); itG != _GroupTiles.end(); ++itG)
	{
		xmlOut.NodeBegin("tilegroup");
			xmlOut.AddAttribute("region", itG->first);

			for (size_t t = 0; t < itG->second.size(); ++t)
			{
				STile* pTile = itG->second[t];
				xmlOut.NodeBegin("tile");
					xmlOut.AddAttribute("posx", pTile->_ptGrid.x);
					xmlOut.AddAttribute("posy", pTile->_ptGrid.y);
					xmlOut.AddAttribute("resGroup", pTile->_strResGroup);
					xmlOut.AddAttribute("resItemID", pTile->_strResItemID);
				xmlOut.NodeEnd("tile");
			}
		xmlOut.NodeEnd("tilegroup");
	}

	xmlOut.NodeEnd("layer");
}

void MapLayerTiles::AddRegionTiles(int iRegion, const TileVectorType& tiles)
{
	_GroupTiles[iRegion] = tiles;
}

void MapLayerTiles::CheckResourceUsage( StringResRefInfoMapType& mapGroupResRefInfo, StringResRefInfoMapType& groupSubResRefCountMap )
{
	for (RegionTileMapType::iterator it = _GroupTiles.begin(); it != _GroupTiles.end(); ++it)
	{
		TileVectorType& tiles = it->second;

		for (size_t t = 0; t < tiles.size(); ++t)
		{
			STile* pTile = tiles[t];

			CheckResourceUsageForTile(pTile, mapGroupResRefInfo, groupSubResRefCountMap);
		}
	}

}

void MapLayerTiles::ExportToFile( const Cactus::String& strPathName )
{
	CStdioFile stdFile;
	if( !stdFile.Open(strPathName.c_str(), CFile::modeCreate | CFile::modeReadWrite | CFile::typeText, 0) )
	{
		CString strError = strPathName.c_str();
		strError += _T(" 不能写！");
		AfxMessageBox(strError, MB_OK | MB_ICONERROR);

		return;
	}

	for (RegionTileMapType::iterator it = _GroupTiles.begin(); it != _GroupTiles.end(); ++it)
	{
		TileVectorType& tiles = it->second;

		for (size_t t = 0; t < tiles.size(); ++t)
		{
			STile* pTile = tiles[t];

			CString strTmp;
			strTmp.Format("%d,	%d,	%d,	%s,	%s\n", pTile->_ptGrid.x, pTile->_ptGrid.y, pTile->_regionID, pTile->_strResGroup.c_str(), pTile->_strResItemID.c_str());
			stdFile.WriteString(strTmp);
		}
	}

	stdFile.Close();
	::ShellExecute(0, "open", strPathName.c_str(), 0, 0, SW_SHOW);
}