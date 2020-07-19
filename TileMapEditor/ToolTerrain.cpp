#include "StdAfx.h"
#include "ToolTerrain.h"

#include "TileMapEditorDoc.h"
#include "ToolManager.h"
#include "Map.h"
#include "MapLayer.h"

#include "MainFrm.h"
#include "TileMapEditorView.h"
#include "ResourceManager.h"


ToolRegion::ToolRegion()
{
	_refCursor		= RGB(192, 0, 0);
	_bDrawCursor	= true;
	_ptStartGrid	= CPoint(-1, -1);
	_ptEndGrid		= CPoint(-1, -1);
	_bHasRange		= false;

	_strTerrainType	= "ȱʡ";

	_rcDirty.SetRectEmpty();
}

ToolRegion::~ToolRegion()
{
}

void ToolRegion::Draw(CDC* pDC)
{
	ToolBase::Draw(pDC);

	MapLayer* pLayer = ToolManager::getSingleton().GetMap()->GetCurLayer();
	if (!pLayer)
		return;

	if (pLayer->GetMapLayerType() != MapLayer::eMapLayerTerrain)
		return;

	if (_bHasRange)
	{
		int dx = _ptEndGrid.x - _ptStartGrid.x;
		int dy = _ptEndGrid.y - _ptStartGrid.y;

		if (dx == 0) dx = 1;
		else if (dx > 0) dx ++;
		else dx --;

		if (dy == 0) dy = 1;
		else if (dy > 0) dy ++;
		else dy --;

		for(int i = 0; i != dx; dx > 0 ? ++i : --i)
		{
			for(int j = 0; j != dy; dy > 0 ? ++j : --j)
			{
				CPoint ptCur = _ptStartGrid;
				ptCur.Offset(i, j);

				pLayer->GetParentMap()->DrawGrid(pDC, ptCur, RGB(255, 0, 0), 1, 1);

				COLORREF refColor = ResourceManager::getSingleton().GetTerrainColor(_strTerrainType);
				pLayer->GetParentMap()->FillGrid(pDC, ptCur, refColor);

				pLayer->GetParentMap()->DrawGridText(pDC, ptCur, RGB(255, 0, 255), _strTerrainType);
			}
		}
	}

}

void ToolRegion::OnLButtonDown(UINT nFlags, CPoint point)
{
	ToolBase::OnLButtonDown(nFlags, point);

	if (!_bInRegion)
	{
		_ptStartGrid	= CPoint(-1, -1);
		_ptEndGrid		= CPoint(-1, -1);
		_rcDirty.SetRectEmpty();
		return;
	}

	MapLayer* pLayer = ToolManager::getSingleton().GetMap()->GetCurLayer();
	if (!pLayer)
		return;

	if (pLayer->GetMapLayerType() != MapLayer::eMapLayerTerrain)
		return;

	_rcDirty.SetRectEmpty();
	_ptStartGrid	= _ptGrid;
	_ptEndGrid		= _ptGrid;
	_bHasRange		= true;
}

void ToolRegion::OnLButtonUp(UINT nFlags, CPoint point)
{
	ToolBase::OnLButtonUp(nFlags, point);

	MapLayer* pLayer = ToolManager::getSingleton().GetMap()->GetCurLayer();
	if (!pLayer)
		return;

	if (pLayer->GetMapLayerType() != MapLayer::eMapLayerTerrain)
		return;

	if (_bHasRange)
	{
		int dx = _ptEndGrid.x - _ptStartGrid.x;
		int dy = _ptEndGrid.y - _ptStartGrid.y;

		if (dx == 0) dx = 1;
		else if (dx > 0) dx ++;
		else dx --;

		if (dy == 0) dy = 1;
		else if (dy > 0) dy ++;
		else dy --;

		for(int i = 0; i != dx; dx > 0 ? ++i : --i)
		{
			for(int j = 0; j != dy; dy > 0 ? ++j : --j)
			{
				CPoint ptCur = _ptStartGrid;
				ptCur.Offset(i, j);

				pLayer->AddOrUpdateTile(ptCur, "Region", _strTerrainType);	//Terrain
			}
		}

		if (!_rcDirty.IsRectEmpty())
		{
			CTileMapEditorView* pView = (CTileMapEditorView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView(); 
			pView->LogicInvalidate(_rcDirty);
		}

		_rcDirty.SetRectEmpty();
		_ptStartGrid	= CPoint(-1, -1);
		_ptEndGrid		= CPoint(-1, -1);

		_bHasRange = false;
	}
}

void ToolRegion::OnMouseMove(UINT nFlags, CPoint point)
{
	ToolBase::OnMouseMove(nFlags, point);

	if (!_bInRegion)
		return;

	if ( (nFlags & MK_LBUTTON) == MK_LBUTTON)
	{
		MapLayer* pLayer = ToolManager::getSingleton().GetMap()->GetCurLayer();
		if (!pLayer)
			return;

		if (pLayer->GetMapLayerType() != MapLayer::eMapLayerTerrain)
			return;

		_ptEndGrid		= _ptGrid;

		int dx = _ptEndGrid.x - _ptStartGrid.x;
		int dy = _ptEndGrid.y - _ptStartGrid.y;

		if (dx == 0) dx = 1;
		else if (dx > 0) dx ++;
		else dx --;

		if (dy == 0) dy = 1;
		else if (dy > 0) dy ++;
		else dy --;

		for(int i = 0; i != dx; dx > 0 ? ++i : --i)
		{
			for(int j = 0; j != dy; dy > 0 ? ++j : --j)
			{
				CPoint ptCur = _ptStartGrid;
				ptCur.Offset(i, j);

				CRect rc	= pLayer->GetParentMap()->GetPixelCoordRect(ptCur);
				_rcDirty.UnionRect(_rcDirty, rc);
			}
		}

		if (!_rcDirty.IsRectEmpty())
		{
			CTileMapEditorView* pView = (CTileMapEditorView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView(); 
			pView->LogicInvalidate(_rcDirty);
		}
	}
}


void ToolRegion::OnTurnOn()
{
	ToolBase::OnTurnOn();

}

void ToolRegion::OnTurnOff()
{
	ToolBase::OnTurnOff();

}
