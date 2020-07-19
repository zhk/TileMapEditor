#include "StdAfx.h"
#include "ToolGOEditor.h"
#include "GameObjectEditorObjects.h"

using namespace Cactus;

ToolGOEditor::ToolGOEditor()
: _eCurToolMode(eToolModeSelect)
, _bInRegion(false)
, _bDrawCursor(true)
, _refCursor(RGB(231, 165, 25))
{
}

ToolGOEditor::~ToolGOEditor()
{
}

bool ToolGOEditor::ToolHitTest(CPoint ptPixel, CPoint& ptGrid, CRect& rcPixel)
{
	CDialogGameObject* pGODlg = GetGODlg();

	bool bInRegion = pGODlg->GetGridCoord(ptPixel, ptGrid);
	if (bInRegion)
	{
		rcPixel = pGODlg->GetPixelCoordRect(ptGrid);
	}
	else
	{
		ptGrid.x	= -1;
		ptGrid.y	= -1;
	}

	return bInRegion;
}

void ToolGOEditor::Draw(CDC* pDC)
{
	if (_bInRegion && _bDrawCursor)
	{
		CDialogGameObject* pGODlg = GetGODlg();
		pGODlg->DrawGrid(pDC, _ptGrid, _refCursor, 0);
	}
}

void ToolGOEditor::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDialogGameObject* pGODlg = GetGODlg();
	if (!pGODlg)
		return;

	_bInRegion = ToolHitTest(point, _ptGrid, _rcTilePixel);

	if (_eCurToolMode == eToolModeSelect)
	{
		_bSelected = pGODlg->HitTest(point);
		if (_bSelected)
		{
			_ptStart = point;
		}
	}
	else if (_eCurToolMode == eToolModeSetObstacle)
	{
		if( pGODlg->AddObstacle(point) )
		{
			GameObjectEditorView* pView = GetGOView();
			pView->Invalidate();
		}
	}
	else
	{
		if( pGODlg->ClearObstacle(point) )
		{
			GameObjectEditorView* pView = GetGOView();
			pView->Invalidate();
		}
	}
}

void ToolGOEditor::OnLButtonUp(UINT nFlags, CPoint point)
{
	_bSelected = false;
}

void ToolGOEditor::OnMouseMove(UINT nFlags, CPoint point)
{
	CDialogGameObject* pGODlg = GetGODlg();
	if (!pGODlg)
	{
		_bInRegion = false;
		return;
	}

	GameObjectEditorView* pView = GetGOView();

	if ( _bSelected && (nFlags & MK_LBUTTON) == MK_LBUTTON)
	{
		CPoint ptOffset = point - _ptStart;
		if (ptOffset.x != 0 || ptOffset.y != 0 )
		{
			_ptStart = point;

			pGODlg->MoveGameObject(ptOffset);
		}
	}

	CRect rcPixel;
	CPoint ptOldGird = _ptGrid;
	if(!ToolHitTest(point, _ptGrid, rcPixel))
	{
		if (_bInRegion)
			pView->LogicInvalidate(&_rcTilePixel);	//刷新老的

		_bInRegion	= false;

		//((CMainFrame*)AfxGetMainWnd())->SetCursorPosition(CPoint(0, 0));

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

	//((CMainFrame*)AfxGetMainWnd())->SetCursorPosition(_ptGrid);
}

void ToolGOEditor::SetCurToolMode(EToolMode e)
{
	_eCurToolMode = e;

	if (_eCurToolMode == eToolModeSelect)
	{
		_refCursor = RGB(231, 165, 25);
	}
	else if (_eCurToolMode == eToolModeSetObstacle)
	{
		_refCursor = RGB(255, 0, 0);
	}
	else
	{
		_refCursor = RGB(0, 192, 0);
	}
}
