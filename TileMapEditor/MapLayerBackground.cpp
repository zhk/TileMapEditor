#include "StdAfx.h"
#include "MapLayerBackground.h"
#include "ResourceManager.h"
#include "Map.h"
#include "ResourceGameObject.h"

#include "MainFrm.h"
#include "TileMapEditorView.h"

using namespace Cactus;
using namespace PropertySys;

MapLayerBackground::MapLayerBackground()
{
	_eLayerType = eMapLayerBackground;
}

MapLayerBackground::~MapLayerBackground()
{
	for (size_t t = 0; t < _Tiles.size(); ++t)
	{
		delete _Tiles[t];
	}
}

int MapLayerBackground::CheckResDepends()
{
	int iCount = 0;

	for (size_t t = 0; t < _Tiles.size(); ++t)
	{
		STile* pTile = _Tiles[t];

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

	return iCount;
}

void MapLayerBackground::Draw(CDC* pDC, const IntVectorType& regions)
{
	if (!_bVisible || !_pParentMap)
		return;

	//排序
	Cactus::list<STile*>::type	tilesList;
	for (size_t t = 0; t < _Tiles.size(); ++t)
	{
		tilesList.push_back(_Tiles[t]);
	}
	if (tilesList.size() > 0)
		tilesList.sort(STile_Triangle_less);

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

		pRes->Draw(pDC, pTile->_ptGrid, pTile->_strResItemID);

		if (pTile->_bSelected)
		{
			CRect rcPixelGrid = CRect(pTile->_ptGrid, CSize(0, 0));
			CRect rcDest = pRes->GetResItemBoundingRect(rcPixelGrid, eGridNone, pTile->_strResItemID);
			PostDrawSelectedRectangle.push_back(rcDest);
		}
	}

	if (PostDrawSelectedRectangle.size())
	{
		CPen pen(PS_SOLID, 1, RGB(231, 165, 25));
		CPen* pOldPen = pDC->SelectObject(&pen);
		pDC->SelectStockObject(NULL_BRUSH);

		for (Cactus::list<CRect>::type::iterator it = PostDrawSelectedRectangle.begin();
			it != PostDrawSelectedRectangle.end();
			++it)
		{
			pDC->Rectangle(*it);
		}

		pDC->SelectObject(pOldPen);
	}
}


bool MapLayerBackground::ToolHitTest(CPoint ptPixel, CPoint& ptGrid, CRect& rcPixel)
{
	if (ptPixel.x > _pParentMap->GetPixelWidth() || ptPixel.y > _pParentMap->GetPixelHeight() || ptPixel.x < 0 || ptPixel.y < 0)
	{
		return false;
	}

	ptGrid	= ptPixel;
	rcPixel = CRect(ptPixel, CSize(0, 0));

	return true;
}


STile* MapLayerBackground::TileHitTest(CPoint ptPixel, CPoint& ptGrid)
{
	ptGrid = ptPixel;

	Cactus::list<STile*>::type	tilesList;

	for (size_t t = 0; t < _Tiles.size(); ++t)
	{
		STile* pTile = _Tiles[t];

		Resource* pRes = ResourceManager::getSingleton().GetResourceArtOrGOGroup(pTile->_strResGroup);
		if (!pRes)
		{
			//Log_Error("资源组 '" << pTile->_strResGroup << "' 不存在！");
			continue;
		}

		CRect rcPixelGrid = CRect(pTile->_ptGrid, CSize(0, 0));
		CRect rcDest = pRes->GetResItemBoundingRect(rcPixelGrid, eGridNone, pTile->_strResItemID);

		if (rcDest.PtInRect(ptPixel))
		{
			tilesList.push_back(pTile);
		}
	}

	//找出最上面的那个
	if (tilesList.size() > 0)
	{
		tilesList.sort(STile_Triangle_less);

		return tilesList.back();
	}

	return 0;
}


bool MapLayerBackground::AddOrUpdateTile(CPoint ptGrid, const Cactus::String& resGroup, const Cactus::String& strItemID)
{
	STile* pTile = new STile;
	pTile->_ptGrid			= ptGrid;
	pTile->_strResItemID	= strItemID;
	pTile->_strResGroup		= resGroup;
	pTile->_regionID		= 0;

	_Tiles.push_back(pTile);

	UpdateTileInfoInPanelMapLayer(pTile, eTileOpAdd);

	UpdateTileVisual(pTile, true);

	return true;
}


void MapLayerBackground::UpdateTileVisual(STile* pTile, bool bEnsureVisible/* = false*/)
{
	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST( CMainFrame, AfxGetMainWnd() );
	if (pMainFrame)
	{
		//更新绘制
		CTileMapEditorView* pMapView = (CTileMapEditorView*)pMainFrame->GetActiveView();

		Resource* pRes = ResourceManager::getSingleton().GetResourceArtOrGOGroup(pTile->_strResGroup);
		if (!pRes)
		{
			//Log_Error("资源组 '" << pTile->_strResGroup << "' 不存在！");
			return;
		}

		CRect rcPixelGrid = CRect(pTile->_ptGrid, CSize(0, 0));
		CRect rcDest = pRes->GetResItemBoundingRect(rcPixelGrid, eGridNone, pTile->_strResItemID);

		if (bEnsureVisible)
			pMapView->EnsureRectVisible(rcDest);

		pMapView->LogicInvalidate(rcDest);
	}

	_pParentMap->SetModified();
}

bool MapLayerBackground::MoveTile(STile* pTile, CPoint ptNewGrid)
{
	STile tileOld = *pTile;
	pTile->_ptGrid		= ptNewGrid;

	UpdateTileInfoInPanelMapLayer(pTile, eTileOpUpdate);

	UpdateTileVisual(&tileOld);
	UpdateTileVisual(pTile, false);

	return true;
}


bool MapLayerBackground::RemoveTile(CPoint ptGrid)
{
	return false;
}


bool MapLayerBackground::RemoveTile(STile* pTile)
{
	for (size_t t = 0; t < _Tiles.size(); ++t)
	{
		if(_Tiles[t] == pTile)
		{
			_Tiles.erase(_Tiles.begin() + t);

			UpdateTileInfoInPanelMapLayer(pTile, eTileOpRemove);

			UpdateTileVisual(pTile);

			delete pTile;

			return true;
		}
	}

	return false;
}


void MapLayerBackground::FillListInPanelMapLayer()
{
	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST( CMainFrame, AfxGetMainWnd() );
	if (!pMainFrame)
		return;

	LayerView* pView = pMainFrame->GetMapLayerView(); 
	if (!pView)
		return;

	for (size_t t = 0; t < _Tiles.size(); ++t)
	{
		pView->AddTileInfo(_Tiles[t]);
	}
}

void MapLayerBackground::Save(Cactus::XMLOutStream& xmlOut)
{
	xmlOut.NodeBegin("background");

		xmlOut.AddAttribute("name", _strName);
		xmlOut.AddAttribute("enable", _bEnable);
		xmlOut.AddAttribute("visible", _bVisible);

		xmlOut.NodeBegin("tilegroup");
		xmlOut.AddAttribute("region", 0);

		for (size_t t = 0; t < _Tiles.size(); ++t)
		{
			STile* pTile = _Tiles[t];

			xmlOut.NodeBegin("tile");
				xmlOut.AddAttribute("posx", pTile->_ptGrid.x);
				xmlOut.AddAttribute("posy", pTile->_ptGrid.y);
				xmlOut.AddAttribute("resGroup", pTile->_strResGroup);
				xmlOut.AddAttribute("resItemID", pTile->_strResItemID);
			xmlOut.NodeEnd("tile");
		}
		xmlOut.NodeEnd("tilegroup");

	xmlOut.NodeEnd("background");
}

void MapLayerBackground::AddRegionTiles(int iRegion, const TileVectorType& tiles)
{
	_Tiles = tiles;
}

void MapLayerBackground::CheckResourceUsage( StringResRefInfoMapType& mapGroupResRefInfo, StringResRefInfoMapType& groupSubResRefCountMap )
{
	for (size_t t = 0; t < _Tiles.size(); ++t)
	{
		STile* pTile = _Tiles[t];

		CheckResourceUsageForTile(pTile, mapGroupResRefInfo, groupSubResRefCountMap);
	}
}

void MapLayerBackground::ExportToFile( const Cactus::String& strPathName )
{
	CStdioFile stdFile;
	if( !stdFile.Open(strPathName.c_str(), CFile::modeCreate | CFile::modeReadWrite | CFile::typeText, 0) )
	{
		CString strError = strPathName.c_str();
		strError += _T(" 不能写！");
		AfxMessageBox(strError, MB_OK | MB_ICONERROR);

		return;
	}

	for (size_t t = 0; t < _Tiles.size(); ++t)
	{
		STile* pTile = _Tiles[t];

		CString strTmp;
		strTmp.Format("%d,	%d,	%d,	%s,	%s\n", pTile->_ptGrid.x, pTile->_ptGrid.y, pTile->_regionID, pTile->_strResGroup.c_str(), pTile->_strResItemID.c_str());
		stdFile.WriteString(strTmp);
	}

	stdFile.Close();
	::ShellExecute(0, "open", strPathName.c_str(), 0, 0, SW_SHOW);
}