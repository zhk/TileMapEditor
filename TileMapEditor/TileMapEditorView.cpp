
// TileMapEditorView.cpp : CTileMapEditorView 类的实现
//

#include "stdafx.h"
#include "TileMapEditor.h"

#include "TileMapEditorDoc.h"
#include "TileMapEditorView.h"

#include "ToolManager.h"
#include "ToolBase.h"

#include "MemDC.h"
#include "MainFrm.h"

using namespace Cactus;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define M_Margin	50

// CTileMapEditorView

IMPLEMENT_DYNCREATE(CTileMapEditorView, CScrollView)

BEGIN_MESSAGE_MAP(CTileMapEditorView, CScrollView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_MOUSELEAVE()
	ON_COMMAND(M_TOOL_EXPORT_MAP, &CTileMapEditorView::OnExportFullMap)
END_MESSAGE_MAP()

// CTileMapEditorView 构造/析构

CTileMapEditorView::CTileMapEditorView()
{
	_bMouseHover = false;
}

CTileMapEditorView::~CTileMapEditorView()
{
	//if( _bmpBackup.GetSafeHandle() )
	//{
	//	_memDC.SelectObject(_pOldBmp);
	//	_memDC.DeleteDC();
	//	_bmpBackup.DeleteObject();
	//}
}

BOOL CTileMapEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CScrollView::PreCreateWindow(cs);
}

// CTileMapEditorView 绘制

void CTileMapEditorView::OnDraw(CDC* pDC)
{
	GetClientRect(_rcClient);

	CTileMapEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pDC->OffsetViewportOrg(M_Margin, M_Margin);

	//if( !_bmpBackup.GetSafeHandle() )
	//{
	//	_bmpBackup.CreateCompatibleBitmap(pDC, _rcClient.Width(), _rcClient.Height());

	//	_memDC.CreateCompatibleDC(pDC);
	//	_pOldBmp = _memDC.SelectObject(&_bmpBackup);
	//}

	{
		MemDC memDC(pDC);

		CPoint ptLP = GetScrollPosition();
		ptLP.Offset(-M_Margin, -M_Margin);

		CRect rc = _rcClient;
		rc.OffsetRect(ptLP);

		pDoc->GetMap().Draw(&memDC, rc);
		ToolManager::getSingleton().GetCurrentTool()->Draw(&memDC);

		//memDC->PeakBuffer(&_memDC);
	}


	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd() );
	if (pMainFrame)
	{
		MapThumbnailView* pMapThumbView = pMainFrame->GetMapThumbnailView(); 
		if (pMapThumbView->GetSafeHwnd())
			pMapThumbView->Invalidate();
	}

}

void CTileMapEditorView::OnExportFullMap()
{
	CClientDC dc(this);

	CTileMapEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pDoc->GetMap().ExportFullMap(&dc);
}

//CDC* CTileMapEditorView::GetDrawingContent(CRect& rc, CSize& szDoc, CPoint& ptScroll)
//{
//	if( !_bmpBackup.GetSafeHandle() )
//		return 0;
//
//	rc	= _rcClient;
//	ptScroll = GetScrollPosition();
//
//	szDoc.cx	= GetDocument()->GetMap().GetPixelWidth() + M_Margin * 2;
//	szDoc.cy	= GetDocument()->GetMap().GetPixelHeight() + M_Margin * 2;
//
//	return &_memDC;
//}
void CTileMapEditorView::GetDrawingContent(CRect& rc, CSize& szDoc, CPoint& ptScroll)
{
	rc	= _rcClient;
	ptScroll = GetScrollPosition();

	if (GetDocument())
	{
		szDoc.cx	= GetDocument()->GetMap().GetPixelWidth() + M_Margin * 2;
		szDoc.cy	= GetDocument()->GetMap().GetPixelHeight() + M_Margin * 2;
	}
	else
	{
		szDoc.cx	= M_Margin * 2;
		szDoc.cy	= M_Margin * 2;
	}
}

void CTileMapEditorView::UpdateDataSizeScroll()
{
	CSize sizeTotal;
	sizeTotal.cx	= GetDocument()->GetMap().GetPixelWidth() + M_Margin * 2;
	sizeTotal.cy	= GetDocument()->GetMap().GetPixelHeight() + M_Margin * 2;
	SetScrollSizes(MM_TEXT, sizeTotal);

	CRect rcClient;
	GetClientRect(&rcClient);

	CPoint ptScroll = CPoint(sizeTotal.cx/2, sizeTotal.cy/2);
	ptScroll.Offset(-rcClient.Size().cx/2, -rcClient.Size().cy/2);
	if(ptScroll.x < 0)
		ptScroll.x = 0;
	if(ptScroll.y < 0)
		ptScroll.y = 0;

	ScrollToPosition(ptScroll);
}


void CTileMapEditorView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	UpdateDataSizeScroll();
}

void CTileMapEditorView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CTileMapEditorView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CTileMapEditorView 诊断

#ifdef _DEBUG
void CTileMapEditorView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CTileMapEditorView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CTileMapEditorDoc* CTileMapEditorView::GetDocument() const // 非调试版本是内联的
{
	//ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTileMapEditorDoc)));
	return (CTileMapEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CTileMapEditorView 消息处理程序

void CTileMapEditorView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	//CSize sizeTotal;
	//sizeTotal.cx	= GetDocument()->GetMap().GetPixelWidth() + M_Margin * 2;
	//sizeTotal.cy	= GetDocument()->GetMap().GetPixelHeight() + M_Margin * 2;
	//SetScrollSizes(MM_TEXT, sizeTotal);

	m_nMapMode = MM_TEXT;

	CScrollView::OnPrepareDC(pDC, pInfo);
}

void CTileMapEditorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CPoint ptLP = point;
	ptLP.Offset( GetScrollPosition() );
	ptLP.Offset(-M_Margin, -M_Margin);
	ToolManager::getSingleton().GetCurrentTool()->OnLButtonDown(nFlags, ptLP);

	CScrollView::OnLButtonDown(nFlags, point);
}

void CTileMapEditorView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CPoint ptLP = point;
	ptLP.Offset( GetScrollPosition() );
	ptLP.Offset(-M_Margin, -M_Margin);
	ToolManager::getSingleton().GetCurrentTool()->OnLButtonUp(nFlags, ptLP);

	CScrollView::OnLButtonUp(nFlags, point);
}

void CTileMapEditorView::OnMouseLeave()
{
	_bMouseHover = false;

	CScrollView::OnMouseLeave();
}

void CTileMapEditorView::OnMouseMove(UINT nFlags, CPoint point)
{
	CPoint ptLP = point;
	CPoint ptScroll = GetScrollPosition();
	ptLP.Offset( ptScroll );
	ptLP.Offset(-M_Margin, -M_Margin);
	ToolManager::getSingleton().GetCurrentTool()->OnMouseMove(nFlags, ptLP);

#if 0
	TRACKMOUSEEVENT		csTME;
	if(!_bMouseHover)
	{
		csTME.cbSize = sizeof(csTME);
		csTME.dwFlags = TME_LEAVE;
		csTME.hwndTrack = this->GetSafeHwnd();
		::_TrackMouseEvent(&csTME);

		_bMouseHover = true;
		_ptLast = point;
	}
	else
	{
		_SmartMoveViewport(point);
	}
#endif

	CScrollView::OnMouseMove(nFlags, point);
}

void CTileMapEditorView::_SmartMoveViewport(CPoint point)
{
	static int s_iLastTime = _timeGetTime();
	int iTime = _timeGetTime();
	if (iTime - s_iLastTime > 150)
	{
#define M_AutoScrollSpeed	120


		CPoint ptDir = point - _ptLast;
		_ptLast = point;

		s_iLastTime = iTime;

		CRect rcClient;
		GetClientRect(rcClient);
		CPoint ptOffset = CPoint(0, 0);
		if (point.x < 1.5*M_Margin)
		{
			if (ptDir.x < 0)
				ptOffset.x = -M_AutoScrollSpeed;
		}
		else if (point.x > rcClient.right - 1.5*M_Margin)
		{
			if (ptDir.x > 0)
				ptOffset.x = M_AutoScrollSpeed;
		}

		if (point.y < 1.5*M_Margin)
		{
			if (ptDir.y < 0)
				ptOffset.y = -M_AutoScrollSpeed;
		}
		else if (point.y > rcClient.bottom - 1.5*M_Margin)
		{
			if (ptDir.y > 0)
				ptOffset.y = M_AutoScrollSpeed;
		}

		CPoint ptScroll = GetScrollPosition();
		ptScroll.Offset(ptOffset);
		ScrollToPosition(ptScroll);
	}
}


void CTileMapEditorView::LogicInvalidate(CRect rc)
{
	CPoint pt = GetScrollPosition();
	rc.OffsetRect(-pt);
	rc.OffsetRect(M_Margin, M_Margin);

	InvalidateRect(rc);
}

void CTileMapEditorView::EnsureRectVisible(CRect rc)
{
	CPoint pt = GetScrollPosition();
	rc.OffsetRect(-pt);
	rc.OffsetRect(M_Margin, M_Margin);

	CRect rcClient;
	GetClientRect(rcClient);

	CRect rcUnion = rc;
	rcUnion.UnionRect(rcClient, rc);
	if (rcClient == rcUnion)
	{
		return;
	}

	CPoint ptOffset = CPoint(0, 0);
	if (rc.left < rcClient.left)
	{
		ptOffset.x = rc.left - rcClient.left - 10;
	}
	else if (rc.right < rcClient.right)
	{
		ptOffset.x = 0;
	}
	else
	{
		ptOffset.x = rc.right - rcClient.right + 10;
	}
	
	if (rc.top < rcClient.top)
	{
		ptOffset.y = rc.top - rcClient.top - 10;
	}
	else if (rc.bottom < rcClient.bottom)
	{
		ptOffset.y = 0;
	}
	else
	{
		ptOffset.y = rc.bottom - rcClient.bottom + 10;
	}
	pt.Offset(ptOffset);
	ScrollToPosition(pt);
}


BOOL CTileMapEditorView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CTileMapEditorView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	//if( _bmpBackup.GetSafeHandle() )
	//{
	//	_memDC.SelectObject(_pOldBmp);
	//	_memDC.DeleteDC();
	//	_bmpBackup.DeleteObject();
	//}
}

void CTileMapEditorView::OffsetScrollPos(CPoint pt)
{
	CPoint ptScroll = GetScrollPosition();
	ptScroll.Offset(pt);

	ScrollToPosition(ptScroll);

	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd() );
	if (pMainFrame)
	{
		MapThumbnailView* pMapThumbView = pMainFrame->GetMapThumbnailView(); 
		if (pMapThumbView->GetSafeHwnd())
			pMapThumbView->Invalidate();
	}
}

void CTileMapEditorView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
#define M_KeyMoveSpeed	18

	bool bSend = true;

	if (nChar == VK_LEFT || nChar == VK_RIGHT || nChar == VK_UP || nChar == VK_DOWN)
	{
		if ( (GetKeyState(VK_CONTROL) & 0x8000) == 0)
		{
			CPoint ptScroll = GetScrollPosition();
			CPoint ptOffset = CPoint(0, 0);

			if (nChar == VK_LEFT)
			{
				ptOffset.x -= M_KeyMoveSpeed;
			}
			else if (nChar == VK_RIGHT)
			{
				ptOffset.x += M_KeyMoveSpeed;
			}
			else if (nChar == VK_UP)
			{
				ptOffset.y -= M_KeyMoveSpeed;
			}
			else if (nChar == VK_DOWN)
			{
				ptOffset.y += M_KeyMoveSpeed;
			}

			if (ptOffset != CPoint(0, 0))
			{
				ptScroll.Offset(ptOffset);
				ScrollToPosition(ptScroll);
			}

			bSend = false;
		}
	}
	
	if (bSend)
		ToolManager::getSingleton().GetCurrentTool()->OnKeyDown(nChar, nRepCnt, nFlags);

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}
