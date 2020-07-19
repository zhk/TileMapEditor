#include "stdafx.h"
#include "MainFrm.h"
#include "MapThumbnailView.h"
#include "Resource.h"
#include "TileMapEditor.h"

#include "TileMapEditorView.h"
#include "ToolManager.h"
#include "Map.h"

#include "MemDC.h"

using namespace Cactus;


//////////////////////////////////////////////////////////////////////
// 构造/析构
//////////////////////////////////////////////////////////////////////

MapThumbnailView::MapThumbnailView()
: _bHold(false)
, _fRatio(1)
, _bEnableHoriz(true)
, _bEnableVert(true)
{
}

MapThumbnailView::~MapThumbnailView()
{
}

BEGIN_MESSAGE_MAP(MapThumbnailView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MapThumbnailView 消息处理程序

int MapThumbnailView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void MapThumbnailView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	Invalidate();
}

BOOL MapThumbnailView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void MapThumbnailView::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文

	CRect rcClient;
	GetClientRect(rcClient);

	MemDC memDC(&dc);
	memDC.FillSolidRect(rcClient, RGB(255, 255, 255));

	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST( CMainFrame, AfxGetMainWnd() );
	if (!pMainFrame)
		return;

	CTileMapEditorView* pView = (CTileMapEditorView*)pMainFrame->GetActiveView(); 
	if (!pView)
		return;

	CRect rcView;
	CSize szDoc;
	CPoint ptScroll;
	pView->GetDrawingContent(rcView, szDoc, ptScroll);
	//if (pDC)
	{
		float fRatio1 = 1.0f * rcClient.Width() / szDoc.cx;
		float fRatio2 = 1.0f * rcClient.Height() / szDoc.cy;
		_fRatio = fRatio1;
		if (fRatio1 > fRatio2)
			_fRatio = fRatio2;

		CRect rcDoc = CRect( CPoint( int(rcClient.Width() - _fRatio * szDoc.cx)/2
			, int(rcClient.Height() - _fRatio * szDoc.cy)/2 )
			, CSize( int(szDoc.cx * _fRatio), int(szDoc.cy * _fRatio) )
			);

		_rcThumbView = CRect(CPoint( rcDoc.TopLeft().x + int(_fRatio * ptScroll.x), rcDoc.TopLeft().y + int(_fRatio * ptScroll.y) )
			, CSize( int(rcView.Width() * _fRatio), int(rcView.Height() * _fRatio) )
			);

		_bEnableHoriz = rcDoc.Width() > _rcThumbView.Width();

		_bEnableVert = rcDoc.Height() > _rcThumbView.Height();

		//绘制地图内容
		//memDC.SetStretchBltMode(HALFTONE);
		//memDC.StretchBlt( rcDoc.TopLeft().x + fRatio * ptScroll.x + 1, rcDoc.TopLeft().y + fRatio * ptScroll.y + 1
		//	, rcView.Width() * fRatio - 1, rcView.Height() * fRatio - 1
		//	, pDC, 0, 0, rcView.Width(), rcView.Height(), SRCCOPY
		//	);

		//绘制文档范围
		CPen pen(PS_SOLID, 1, RGB(0, 255, 0));
		CPen* pOldPen = memDC.SelectObject(&pen);
		memDC.SelectStockObject(NULL_BRUSH);

		if (ToolManager::getSingleton().GetMap()->GetGridType() == eRectangle)
		{
			memDC.Rectangle(rcDoc);
		}
		else
		{
			CPoint pts[4];
			pts[0] = CPoint(rcDoc.left, rcDoc.top + rcDoc.Height()/2);
			pts[1] = CPoint(rcDoc.left + rcDoc.Width()/2, rcDoc.top);
			pts[2] = CPoint(rcDoc.right, rcDoc.top + rcDoc.Height()/2);
			pts[3] = CPoint(rcDoc.left + rcDoc.Width()/2, rcDoc.bottom);

			memDC.Polygon(pts, 4);
		}
		memDC.SelectObject(pOldPen);

		//绘制试图范围
		CPen pen2(PS_SOLID, 1, RGB(0, 0, 255));
		CPen* pOldPen2 = memDC.SelectObject(&pen2);
		//memDC.SelectStockObject(LTGRAY_BRUSH);
		memDC.Rectangle(_rcThumbView);
		memDC.SelectObject(pOldPen2);
	}
}

void MapThumbnailView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

}

void MapThumbnailView::OnContextMenu(CWnd* pWnd, CPoint point)
{
}

void MapThumbnailView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if( _rcThumbView.PtInRect(point) )
	{
		_ptOld = point;
		_bHold = true;
	}
	else
		CDockablePane::OnLButtonDown(nFlags, point);
}

void MapThumbnailView::OnLButtonUp(UINT nFlags, CPoint point)
{
	_bHold = false;

	CDockablePane::OnLButtonUp(nFlags, point);
}

void MapThumbnailView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (_bHold)
	{
		CPoint ptOffset = point - _ptOld;
		_ptOld = point;

		if (!_bEnableHoriz)
			ptOffset.x = 0;

		if (!_bEnableVert)
			ptOffset.y = 0;

		if (!_bEnableHoriz && !_bEnableVert)
			return;

		ptOffset.x = LONG(ptOffset.x / _fRatio);
		ptOffset.y = LONG(ptOffset.y / _fRatio);

		CMainFrame* pMainFrame = DYNAMIC_DOWNCAST( CMainFrame, AfxGetMainWnd() );
		if (!pMainFrame)
			return;

		CTileMapEditorView* pView = (CTileMapEditorView*)pMainFrame->GetActiveView(); 
		if (!pView)
			return;

		pView->OffsetScrollPos(ptOffset);
	}
	else
		CDockablePane::OnMouseMove(nFlags, point);
}
