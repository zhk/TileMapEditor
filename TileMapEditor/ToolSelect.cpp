#include "StdAfx.h"
#include "ToolSelect.h"

#include "TileMapEditorDoc.h"
#include "ToolManager.h"
#include "Map.h"
#include "MapLayer.h"

#include "MainFrm.h"
#include "LayerView.h"

ToolSelect::ToolSelect()
{
	_refCursor = RGB(192, 192, 192);
	_pSelectedTile = 0;
}

ToolSelect::~ToolSelect()
{
}

void ToolSelect::Draw(CDC* pDC)
{
	ToolBase::Draw(pDC);

}

void ToolSelect::OnLButtonDown(UINT nFlags, CPoint point)
{
	ToolBase::OnLButtonDown(nFlags, point);

	MapLayer* pLayer = ToolManager::getSingleton().GetMap()->GetCurLayer();
	if (!pLayer)
		return;

	STile* pTile = pLayer->TileHitTest(point, _ptStartGrid);
	if (!pTile)
	{
		if (_pSelectedTile)
			SelectTile(_pSelectedTile, false);

		return;
	}

	SelectTile(pTile, true);
}

void ToolSelect::OnLButtonUp(UINT nFlags, CPoint point)
{
	ToolBase::OnLButtonUp(nFlags, point);

}

void ToolSelect::OnMouseMove(UINT nFlags, CPoint point)
{
	ToolBase::OnMouseMove(nFlags, point);

	//编辑
	MapLayer* pLayer = ToolManager::getSingleton().GetMap()->GetCurLayer();
	if (!pLayer)
		return;

	if ( _pSelectedTile && (nFlags & MK_LBUTTON) == MK_LBUTTON)
	{
		CPoint ptOffset = _ptGrid - _ptStartGrid;
		if (ptOffset.x != 0 || ptOffset.y != 0 )
		{
			CPoint ptNew = _pSelectedTile->_ptGrid;
			ptNew.Offset(ptOffset);
			_ptStartGrid = _ptGrid;

			if( pLayer->MoveTile(_pSelectedTile, ptNew) )
			{
				//Log_Info("Tile移动成功，位置: (" << _pSelectedTile->_ptGrid.x << ", " << _pSelectedTile->_ptGrid.y << ") 资源组：" << _pSelectedTile->_strResGroup << " : " << _pSelectedTile->_strResItemID);
			}
		}
	}

}


void ToolSelect::OnTurnOn()
{
	ToolBase::OnTurnOn();

}

void ToolSelect::OnTurnOff()
{
	ToolBase::OnTurnOff();

	if (_pSelectedTile)
	{
		MapLayer* pLayer = ToolManager::getSingleton().GetMap()->GetCurLayer();
		if (!pLayer)
			return;

		_pSelectedTile->_bSelected = false;
		pLayer->UpdateTileVisual(_pSelectedTile);
		_pSelectedTile = 0;
	}

}

void ToolSelect::SelectTile(STile* pTile, bool bSelect/* = true*/)
{
	MapLayer* pLayer = ToolManager::getSingleton().GetMap()->GetCurLayer();
	if (!pLayer)
		return;

	if (!bSelect)
	{
		pTile->_bSelected = false;
		pLayer->UpdateTileVisual(pTile);

		_pSelectedTile = 0;
		return;
	}

	if (_pSelectedTile)
	{
		if(pTile != _pSelectedTile)
		{
			_pSelectedTile->_bSelected = false;
			pLayer->UpdateTileVisual(_pSelectedTile);

			pTile->_bSelected = true;
			pLayer->UpdateTileVisual(pTile, true);

			_pSelectedTile = pTile;
		}
	}
	else
	{
		_pSelectedTile = pTile;
		_pSelectedTile->_bSelected = true;
		pLayer->UpdateTileVisual(_pSelectedTile, true);
	}

	//同步选择到LayerView
	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST( CMainFrame, AfxGetMainWnd() );
	if (!pMainFrame)
		return;

	LayerView* pView = pMainFrame->GetMapLayerView(); 
	if (!pView)
		return;

	pView->SelectTile(_pSelectedTile);
}

//VK_ESCAPE VK_DELETE VK_SHIFT
void ToolSelect::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_ESCAPE)
	{
		//DeSelect
		if (_pSelectedTile)
			SelectTile(_pSelectedTile, false);
	}
	else if (nChar == VK_DELETE)
	{
		if (!_pSelectedTile)
			return;

		//Delete
		MapLayer* pLayer = ToolManager::getSingleton().GetMap()->GetCurLayer();
		if (!pLayer)
			return;

		if( pLayer->RemoveTile(_pSelectedTile) )
		{
			_pSelectedTile = 0;

			Log_Info("删除Tile成功" );
		}
	}
	else if (nChar == VK_LEFT || nChar == VK_RIGHT || nChar == VK_UP || nChar == VK_DOWN)
	{
		MapLayer* pLayer = ToolManager::getSingleton().GetMap()->GetCurLayer();
		if (!pLayer)
			return;

		if (GetKeyState(VK_CONTROL) & 0x8000)
		{
			CPoint ptOffset = CPoint(0, 0);
			if (nChar == VK_LEFT)
				ptOffset.x = -1;
			else if (nChar == VK_RIGHT)
				ptOffset.x = 1;
			else if(nChar == VK_UP)
				ptOffset.y = -1;
			else if(nChar == VK_DOWN)
				ptOffset.y = 1;

			CPoint ptNew = _pSelectedTile->_ptGrid;
			ptNew.Offset(ptOffset);
			if( pLayer->MoveTile(_pSelectedTile, ptNew) )
			{
				//Log_Info("Tile移动成功，位置: (" << _pSelectedTile->_ptGrid.x << ", " << _pSelectedTile->_ptGrid.y << ") 资源组：" << _pSelectedTile->_strResGroup << " : " << _pSelectedTile->_strResItemID);
			}
		}
	}
}