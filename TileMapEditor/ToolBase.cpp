#include "StdAfx.h"
#include "ToolBase.h"

#include "TileMapEditorDoc.h"
#include "ToolManager.h"
#include "Map.h"
#include "MapLayer.h"

#include "MainFrm.h"
#include "TileMapEditorView.h"

ToolBase::ToolBase()
: _bInRegion(false)
, _bDrawCursor(false)
{
}

ToolBase::~ToolBase()
{
}

void ToolBase::Draw(CDC* pDC)
{
	if (_bInRegion && _bDrawCursor)
	{
		CPen pen(PS_SOLID, 2, _refCursor);

		pDC->SelectStockObject(NULL_BRUSH);
		CPen* pOldPen = pDC->SelectObject(&pen);

		CRect rc = _rcTilePixel;

		if (ToolManager::getSingleton().GetMap()->GetGridType() == eRectangle)
		{
			rc.DeflateRect(1, 1, 0, 0);

			pDC->Rectangle(rc);
		}
		else
		{
			rc.DeflateRect(1, 1, 1, 1);

			if(rc.right > rc.left && rc.bottom > rc.top)
			{
				CPoint pts[4];
				pts[0] = CPoint(rc.left, rc.top + rc.Height()/2);
				pts[1] = CPoint(rc.left + rc.Width()/2, rc.top);
				pts[2] = CPoint(rc.right, rc.top + rc.Height()/2);
				pts[3] = CPoint(rc.left + rc.Width()/2, rc.bottom);

				pDC->Polygon(pts, 4);
			}
		}

		pDC->SelectObject(pOldPen);
	}
}

void ToolBase::OnLButtonDown(UINT nFlags, CPoint point)
{
	MapLayer* pLayer = ToolManager::getSingleton().GetMap()->GetCurLayer();
	if (!pLayer)
		return;

	_bInRegion = pLayer->ToolHitTest(point, _ptGrid, _rcTilePixel);
}

void ToolBase::OnLButtonUp(UINT nFlags, CPoint point)
{

}

void ToolBase::OnMouseMove(UINT nFlags, CPoint point)
{
	MapLayer* pLayer = ToolManager::getSingleton().GetMap()->GetCurLayer();
	if (!pLayer)
	{
		_bInRegion = false;
		return;
	}

	CTileMapEditorView* pView = (CTileMapEditorView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();

	CRect rcPixel;
	CPoint ptOldGird = _ptGrid;
	if(!pLayer->ToolHitTest(point, _ptGrid, rcPixel))
	{
		if (_bInRegion)
			pView->LogicInvalidate(&_rcTilePixel);	//刷新老的

		_bInRegion	= false;

		((CMainFrame*)AfxGetMainWnd())->SetCursorPosition(CPoint(0, 0));

		return;
	}

	if (ptOldGird != _ptGrid)
	{
		if (_bInRegion)
			pView->LogicInvalidate(&_rcTilePixel);	//刷新老的

		_rcTilePixel = rcPixel;
		pView->LogicInvalidate(&_rcTilePixel);		//绘制新的
	}

	_rcTilePixel = rcPixel;
	_bInRegion = true;

	((CMainFrame*)AfxGetMainWnd())->SetCursorPosition(_ptGrid);
}


void ToolBase::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

}


void ToolBase::OnTurnOn()
{

}

void ToolBase::OnTurnOff()
{
	if (_bInRegion)
	{
		_bInRegion = false;

		CTileMapEditorView* pView = (CTileMapEditorView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView(); 
		pView->LogicInvalidate(&_rcTilePixel);	//刷新老的
	}

}
