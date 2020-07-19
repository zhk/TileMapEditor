
#include "stdafx.h"
#include "MainFrm.h"
#include "MapView.h"
#include "Resource.h"
#include "TileMapEditor.h"
#include "MapLayerBackground.h"
#include "MapLayerTiles.h"
#include "Map.h"

#include "ToolManager.h"
#include "DialogNewLayer.h"

#define M_TreeID	(WM_USER + 100)

class CClassViewMenuButton : public CMFCToolBarMenuButton
{
	friend class MapView;

	DECLARE_SERIAL(CClassViewMenuButton)

public:
	CClassViewMenuButton(HMENU hMenu = NULL) : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
	{
	}

	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
		BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
	{
		pImages = CMFCToolBar::GetImages();

		CAfxDrawState ds;
		pImages->PrepareDrawImage(ds);

		CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

		pImages->EndDrawImage(ds);
	}
};

IMPLEMENT_SERIAL(CClassViewMenuButton, CMFCToolBarMenuButton, 1)

//////////////////////////////////////////////////////////////////////
// 构造/析构
//////////////////////////////////////////////////////////////////////

MapView::MapView()
: _pSelectedLayer(0)
, _pTheMap(0)
{
}

MapView::~MapView()
{
}

BEGIN_MESSAGE_MAP(MapView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_NOTIFY(NM_CLICK, M_TreeID, &MapView::OnNMClickedTreeDetails)
	ON_NOTIFY(NM_DBLCLK, M_TreeID, &MapView::OnNMDblclkTree)
	ON_NOTIFY(NM_RCLICK, M_TreeID, &MapView::OnNMRclickTree)
	ON_COMMAND(ID_LAYEROP_SETCURRENT, OnLayerSetCurrent)
	ON_COMMAND(ID_LAYEROP_INSERT, OnLayerInsert)
	ON_COMMAND(ID_LAYEROP_SHOW, OnLayerShow)
	ON_COMMAND(ID_LAYEROP_HIDE, OnLayerHide)
	ON_COMMAND(ID_LAYEROP_REMOVE, OnLayerRemove)

	ON_COMMAND(ID_LAYEROP_MOVE_DOWN, OnLayerMoveDown)
	ON_COMMAND(ID_LAYEROP_MOVE_UP, OnLayerMoveUp)
	ON_COMMAND(ID_LAYEROP_MOVE_BOTTOM, OnLayerMoveBottom)
	ON_COMMAND(ID_LAYEROP_MOVE_TOP, OnLayerMoveTop)

	ON_COMMAND(ID_PROPERTIES, OnProperties)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassView 消息处理程序

int MapView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建视图:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASBUTTONS | TVS_CHECKBOXES | TVS_HASLINES | TVS_LINESATROOT | TVS_DISABLEDRAGDROP | TVS_TRACKSELECT;
	if (!_TreeMapItem.Create(dwViewStyle, rectDummy, this, M_TreeID))
	{
		TRACE0("未能创建类视图\n");
		return -1;      // 未能创建
	}

	// 加载图像:
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* 已锁定*/);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// 所有命令将通过此控件路由，而不是通过主框架路由:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);


	// 填入一些静态树视图数据(此处只需填入虚拟代码，而不是复杂的数据)
	FillClassView();

	return 0;
}

void MapView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void MapView::SetMapObject(Map* p)
{
	//_TreeMapItem.SetItemData(_TreeMapItem.GetRootItem(), (DWORD_PTR)p);
	_pTheMap = p;

	for(Map::MapLayerListType::iterator it = p->_layers.begin(); it != p->_layers.end(); ++it)
	{
		MapLayer* pLayer = *it;

		int iIconType = 3;
		HTREEITEM hItem = _TreeMapItem.InsertItem(pLayer->GetObjectName().c_str(), iIconType, iIconType, _hLayerRoot);
		_TreeMapItem.SetItemData(hItem, (DWORD_PTR)pLayer);
		_TreeMapItem.SetCheck(hItem, pLayer->IsVisible());

		_TreeMapItem.Expand(_hLayerRoot, TVE_EXPAND);
	}

	CPropertiesWnd* pPropertyWnd = ((CMainFrame*)AfxGetMainWnd())->GetPropertyWnd(); 
	pPropertyWnd->AddPropertyData(_pTheMap, _pTheMap->GetObjectName());
	pPropertyWnd->ShowPane(TRUE, FALSE, TRUE);
}

void MapView::FillClassView()
{
	HTREEITEM hRoot = _TreeMapItem.InsertItem(_T("地图"), 0, 0);
	_TreeMapItem.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	//HTREEITEM hClass = _TreeMapItem.InsertItem(_T("背景"), 3, 3, hRoot);

	_hLayerRoot = _TreeMapItem.InsertItem(_T("地层"), 1, 1, hRoot);
	_hBrushRoot = _TreeMapItem.InsertItem(_T("画刷"), 1, 1, hRoot);
	_TreeMapItem.Expand(hRoot, TVE_EXPAND);

	//_TreeMapItem.Expand(_hBrushRoot, TVE_EXPAND);

	//可用Icon : 2, 5, 6

}

void MapView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&_TreeMapItem;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	pWndTree->SetFocus();
	if (point != CPoint(-1, -1))
	{
		// 选择已单击的项:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}
}

void MapView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	_TreeMapItem.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL MapView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void MapView::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文

	CRect rectTree;
	_TreeMapItem.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void MapView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	_TreeMapItem.SetFocus();
}

void MapView::OnChangeVisualStyle()
{
	//工具条
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* 锁定 */);


	//Tree的图标
	_TreeImageList.DeleteImageList();
	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;
	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("无法加载位图: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	_TreeImageList.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	_TreeImageList.Add(&bmp, RGB(255, 0, 0));

	_TreeMapItem.SetImageList(&_TreeImageList, TVSIL_NORMAL);

}

void MapView::Reset()
{
	_pSelectedLayer	= 0;
	_hSelectedItem	= 0;
	_TreeMapItem.DeleteAllItems();

	FillClassView();
}

void MapView::OnNMClickedTreeDetails(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint point;
	GetCursorPos(&point);
	_TreeMapItem.ScreenToClient(&point);

	UINT uFlag;
	HTREEITEM hItem = _TreeMapItem.HitTest(point, &uFlag);
	if (!hItem)
		return;

	if (TVHT_ONITEMSTATEICON & uFlag)	//点击的是CheckBox
	{
		DWORD_PTR ptr = _TreeMapItem.GetItemData(hItem);
		if(ptr)
		{
			MapLayer* pLayer = (MapLayer*)ptr;
			BOOL b = _TreeMapItem.GetCheck(hItem);	//得到的状态是点击前的...
			if (b == TRUE)
			{
				ToolManager::getSingleton().GetMap()->ShowLayer(pLayer, false);
			}
			else
			{
				ToolManager::getSingleton().GetMap()->ShowLayer(pLayer, true, true);
			}
		}
	}
	else
	{
		DWORD_PTR ptr = _TreeMapItem.GetItemData(hItem);
		if (ptr)
		{
			MapLayer* pLayer = (MapLayer*)ptr;

			if (pLayer->IsVisible())
				ToolManager::getSingleton().GetMap()->SetCurLayer(pLayer);
		}

		if(ptr)
		{
			MapBaseObject* pObject = (MapBaseObject*)ptr;

			CPropertiesWnd* pPropertyWnd = ((CMainFrame*)AfxGetMainWnd())->GetPropertyWnd(); 
			pPropertyWnd->AddPropertyData(pObject, pObject->GetObjectName());
			pPropertyWnd->ShowPane(TRUE, FALSE, TRUE);
		}
		else
		{
			CPropertiesWnd* pPropertyWnd = ((CMainFrame*)AfxGetMainWnd())->GetPropertyWnd(); 
			pPropertyWnd->AddPropertyData(_pTheMap, _pTheMap->GetObjectName());
			pPropertyWnd->ShowPane(TRUE, FALSE, TRUE);
		}
	}

	*pResult = 0;
}

void MapView::OnNMDblclkTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	HTREEITEM hItem = _TreeMapItem.GetSelectedItem();

	DWORD_PTR ptr = _TreeMapItem.GetItemData(hItem);
	if (ptr)
	{
		MapLayer* pLayer = (MapLayer*)ptr;

		if (!pLayer->IsVisible())
		{
			pLayer->SetVisible(true);
			_TreeMapItem.SetCheck(hItem);
		}

		ToolManager::getSingleton().GetMap()->SetCurLayer(pLayer);
	}

	*pResult = 0;
}

void MapView::OnNMRclickTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	_pSelectedLayer = 0;

	CPoint point;
	GetCursorPos(&point);

	CPoint pointScreen = point;
	_TreeMapItem.ScreenToClient(&pointScreen);

	UINT uFlags;
	HTREEITEM hItem = _TreeMapItem.HitTest(pointScreen, &uFlags);
	if (hItem == 0)
	{
		*pResult = 0;
		return;
	}

	if (hItem == _TreeMapItem.GetRootItem())
	{
	}
	else if (hItem == _hLayerRoot)
	{
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_MENU_MAPLAYER_OP, point.x, point.y, this, TRUE);
		*pResult = 0;
		return;
	}
	else if (hItem == _hBrushRoot)
	{
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_MENU_MAPBRUSH_OP, point.x, point.y, this, TRUE);
		*pResult = 0;
		return;
	}

	DWORD_PTR ptr = _TreeMapItem.GetItemData(hItem);
	if (ptr)
	{
		{
			_pSelectedLayer = (MapLayer*)ptr;
			_hSelectedItem	= hItem;
			theApp.GetContextMenuManager()->ShowPopupMenu(IDR_MENU_LAYER_OP, point.x, point.y, this, TRUE);
		}
		// Brush
		{

		}
	}

	*pResult = 0;
}


void MapView::OnLayerSetCurrent()
{
	if(_pSelectedLayer)
	{
		_TreeMapItem.SetCheck(_hSelectedItem);
		ToolManager::getSingleton().GetMap()->SetCurLayer(_pSelectedLayer);
	}
}

void MapView::OnLayerInsert()
{
	DialogNewLayer dlg;
	if (dlg.DoModal() == IDOK)
	{

		MapLayer* pLayer;
		if (dlg._bBackgroundLayer)
			pLayer = new MapLayerBackground;
		else
			pLayer = new MapLayerTiles;

		pLayer->Init((LPCTSTR)dlg._strLayerName, ToolManager::getSingleton().GetMap());

		if( ToolManager::getSingleton().GetMap()->AddLayer(pLayer) )
		{
			HTREEITEM hItem = _TreeMapItem.InsertItem(pLayer->GetObjectName().c_str(), 3, 3, _hLayerRoot);
			_TreeMapItem.SetItemData(hItem, (DWORD_PTR)pLayer);
			_TreeMapItem.SetCheck(hItem, pLayer->IsVisible());
			_TreeMapItem.Expand(_hLayerRoot, TVE_EXPAND);
		}
		else
		{
			delete pLayer;
			MessageBox("添加层错误，层名已经存在！", "错误", MB_OK | MB_ICONHAND);
		}
	}
}

void MapView::OnLayerShow()
{
	if(_pSelectedLayer && !_pSelectedLayer->IsVisible())
	{
		_TreeMapItem.SetCheck(_hSelectedItem);

		ToolManager::getSingleton().GetMap()->ShowLayer(_pSelectedLayer, true, true);
	}
}

void MapView::OnLayerHide()
{
	if(_pSelectedLayer && _pSelectedLayer->IsVisible())
	{
		_TreeMapItem.SetCheck(_hSelectedItem, 0);

		ToolManager::getSingleton().GetMap()->ShowLayer(_pSelectedLayer, false, false);
	}
}

void MapView::OnLayerRemove()
{
	if(!_pSelectedLayer)
		return;

	if (_pSelectedLayer->GetMapLayerType() == MapLayer::eMapLayerTerrain
		||
		_pSelectedLayer->GetMapLayerType() == MapLayer::eMapLayerMonster
		||
		_pSelectedLayer->GetMapLayerType() == MapLayer::eMapLayerEvent
		)
	{
		MessageBox("不能删除内建地层！", "提醒", MB_OK | MB_ICONINFORMATION);
		return;
	}

	Cactus::ostringstream os;
	os << "你确信要删除层: '" << _pSelectedLayer->GetObjectName() << "' 吗？";
	if( MessageBox(os.str().c_str(), "询问", MB_YESNO | MB_ICONQUESTION) == IDYES )
	{
		_TreeMapItem.DeleteItem(_hSelectedItem);

		ToolManager::getSingleton().GetMap()->RemoveLayer(_pSelectedLayer);
	}
}

void MapView::OnProperties()
{
	HTREEITEM hItem = _TreeMapItem.GetSelectedItem();
	if (!hItem)
		return;

	DWORD_PTR ptr = _TreeMapItem.GetItemData(hItem);
	if(ptr)
	{
		MapBaseObject* pObject = (MapBaseObject*)ptr;

		CPropertiesWnd* pPropertyWnd = ((CMainFrame*)AfxGetMainWnd())->GetPropertyWnd(); 
		pPropertyWnd->AddPropertyData(pObject, pObject->GetObjectName());
		pPropertyWnd->ShowPane(TRUE, FALSE, TRUE);
	}
	else
	{
		CPropertiesWnd* pPropertyWnd = ((CMainFrame*)AfxGetMainWnd())->GetPropertyWnd(); 
		pPropertyWnd->AddPropertyData(_pTheMap, _pTheMap->GetObjectName());
		pPropertyWnd->ShowPane(TRUE, FALSE, TRUE);
	}
}

void MapView::OnLayerMoveDown()
{
	if(!_pSelectedLayer)
		return;

	if (ToolManager::getSingleton().GetMap()->MoveLayerDown(_pSelectedLayer))
	{
		Reset();
		SetMapObject(ToolManager::getSingleton().GetMap());
	}
}

void MapView::OnLayerMoveUp()
{
	if(!_pSelectedLayer)
		return;

	if (ToolManager::getSingleton().GetMap()->MoveLayerUp(_pSelectedLayer))
	{
		Reset();
		SetMapObject(ToolManager::getSingleton().GetMap());
	}
}

void MapView::OnLayerMoveBottom()
{
	if(!_pSelectedLayer)
		return;

	if (ToolManager::getSingleton().GetMap()->MoveLayerBottom(_pSelectedLayer))
	{
		Reset();
		SetMapObject(ToolManager::getSingleton().GetMap());
	}
}

void MapView::OnLayerMoveTop()
{
	if(!_pSelectedLayer)
		return;

	if (ToolManager::getSingleton().GetMap()->MoveLayerTop(_pSelectedLayer))
	{
		Reset();
		SetMapObject(ToolManager::getSingleton().GetMap());
	}
}
