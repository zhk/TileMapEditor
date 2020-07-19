
#include "stdafx.h"
#include "mainfrm.h"
#include "ResourceTreeView.h"
#include "Resource.h"
#include "TileMapEditor.h"

#include "ResourceManager.h"
#include "TileResView.h"
#include "ResourceGameObject.h"
#include "ResourceTileFolder.h"
#include "ResourceTileSingleImage.h"

#include "DialogAddGameObjectGroup.h"
#include "DialogAddResourceTile.h"

#define M_TreeID	(WM_USER + 100)


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// ArtSourceTreeView

ResourceTreeView::ResourceTreeView()
: _pSelectedGroup(0)
, _hSelectedItem(0)
{
}

ResourceTreeView::~ResourceTreeView()
{
}

BEGIN_MESSAGE_MAP(ResourceTreeView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	
	ON_COMMAND(ID_GAMEOBJECTGROUP_ADDGROUP, OnGameObjectGroupAdd)
	ON_UPDATE_COMMAND_UI(ID_GAMEOBJECTGROUP_ADDGROUP, OnUpdateCmdUI_GroupAdd)
	ON_COMMAND(ID_GAMEOBJECTGROUP_REMOVEGROUP, OnGameObjectGroupRemove)
	ON_UPDATE_COMMAND_UI(ID_GAMEOBJECTGROUP_REMOVEGROUP, OnUpdateCmdUI_GroupRemove)

	ON_COMMAND(ID_RESOURCE_TILE_ADD, OnResourceTileAdd)
	ON_UPDATE_COMMAND_UI(ID_RESOURCE_TILE_ADD, OnUpdateCmdUI_ResourceTileAdd)
	ON_COMMAND(ID_RESOURCE_TILE_REMOVE, OnResourceTileRemove)
	ON_UPDATE_COMMAND_UI(ID_RESOURCE_TILE_REMOVE, OnUpdateCmdUI_ResourceTileRemove)

	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_NOTIFY(TVN_SELCHANGED, M_TreeID, &ResourceTreeView::OnTvnSelchangedTreeDetails)
	ON_NOTIFY(NM_RCLICK, M_TreeID, &ResourceTreeView::OnNMRclickTree)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar 消息处理程序


int ResourceTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建视图:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_DISABLEDRAGDROP | TVS_TRACKSELECT;

	if (!_ResourceTree.Create(dwViewStyle, rectDummy, this, M_TreeID))
	{
		TRACE0("未能创建文件视图\n");
		return -1;      // 未能创建
	}

	// 加载视图图像:
	_TreeImageList.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	_ResourceTree.SetImageList(&_TreeImageList, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* 已锁定*/);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// 所有命令将通过此控件路由，而不是通过主框架路由:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// 填入一些静态树视图数据(此处只需填入虚拟代码，而不是复杂的数据)
	FillFileView();
	AdjustLayout();

	return 0;
}

void ResourceTreeView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void ResourceTreeView::FillFileView()
{
	HTREEITEM hRoot = _ResourceTree.InsertItem(_T("Root"), 0, 0);
	_ResourceTree.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	HTREEITEM hArtSrc = _ResourceTree.InsertItem(_T("美术资源"), 0, 0, hRoot);

	//_treeArtBackgroundRes	= _ResourceTree.InsertItem(_T("背景资源"), 0, 0, hArtSrc);
	//for (ResourceManager::ResBackgroundType::iterator it = ResourceManager::getSingleton()._ResBackgrounds.begin(); 
	//	it != ResourceManager::getSingleton()._ResBackgrounds.end(); ++it)
	//{
	//	HTREEITEM hItem = _ResourceTree.InsertItem(it->first.c_str(), 2, 2, _treeArtBackgroundRes);
	//	_ResourceTree.SetItemData(hItem, (DWORD_PTR)it->second);
	//}

	_treeArtTileRes			= _ResourceTree.InsertItem(_T("Tile资源"), 0, 0, hArtSrc);
	for (ResourceManager::ResTileType::iterator it = ResourceManager::getSingleton()._ResourceTiles.begin(); 
		it != ResourceManager::getSingleton()._ResourceTiles.end(); ++it)
	{
		HTREEITEM hItem = _ResourceTree.InsertItem(it->first.c_str(), 2, 2, _treeArtTileRes);
		_ResourceTree.SetItemData(hItem, (DWORD_PTR)it->second);
	}

	_treeGameObjectRes	= _ResourceTree.InsertItem(_T("游戏对象组"), 0, 0, hRoot);
	for (ResourceManager::ResGameObjectGroupMapType::iterator it = ResourceManager::getSingleton()._ResGameObjectGroups.begin();
		it != ResourceManager::getSingleton()._ResGameObjectGroups.end();
		++it
		)
	{
		HTREEITEM hItem = _ResourceTree.InsertItem(it->first.c_str(), 2, 2, _treeGameObjectRes);
		_ResourceTree.SetItemData(hItem, (DWORD_PTR)it->second);
	}

	_ResourceTree.Expand(hRoot, TVE_EXPAND);

	_ResourceTree.Expand(hArtSrc, TVE_EXPAND);
	//_ResourceTree.Expand(_treeArtBackgroundRes, TVE_EXPAND);
	_ResourceTree.Expand(_treeArtTileRes, TVE_EXPAND);
	
	_ResourceTree.Expand(_treeGameObjectRes, TVE_EXPAND);
	//_ResourceTree.Expand(_treeGameEventRes, TVE_EXPAND);
}

void ResourceTreeView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &_ResourceTree;
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

void ResourceTreeView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	_ResourceTree.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void ResourceTreeView::OnProperties()
{
	//AfxMessageBox(_T("属性...."));
}

void ResourceTreeView::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文

	CRect rectTree;
	_ResourceTree.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void ResourceTreeView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	_ResourceTree.SetFocus();
}

void ResourceTreeView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* 锁定 */);

	_TreeImageList.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

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
	_TreeImageList.Add(&bmp, RGB(255, 0, 255));

	_ResourceTree.SetImageList(&_TreeImageList, TVSIL_NORMAL);
}

void ResourceTreeView::OnTvnSelchangedTreeDetails(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	CString strText = _ResourceTree.GetItemText(pNMTreeView->itemNew.hItem);

	DWORD_PTR ptr = _ResourceTree.GetItemData(pNMTreeView->itemNew.hItem);
	if (ptr)
	{
		CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd() );
		TileResView* pTileResView = pMainFrame->GetTileResView();
		pTileResView->ShowPane(TRUE, FALSE, TRUE);

		Resource* pRes = (Resource*)ptr;
		pRes->CreateImageList(pTileResView->GetDC());

		//insert image into TileResView
		pTileResView->BuildImageAndInfoes(pRes);


		CPropertiesWnd* pPropertyWnd = ((CMainFrame*)AfxGetMainWnd())->GetPropertyWnd(); 
		pPropertyWnd->AddPropertyData(pRes, pRes->GetResourceName());
		pPropertyWnd->ShowPane(TRUE, FALSE, TRUE);
	}

	*pResult = 0;
}

void ResourceTreeView::OnNMRclickTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	_pSelectedGroup = 0;
	_hSelectedItem	= 0;

	CPoint point;
	GetCursorPos(&point);

	CPoint pointScreen = point;
	_ResourceTree.ScreenToClient(&pointScreen);

	UINT uFlags;
	HTREEITEM hItem = _ResourceTree.HitTest(pointScreen, &uFlags);
	if (hItem == 0)
	{
		*pResult = 0;
		return;
	}

	if (hItem == _treeGameObjectRes)
	{
		_hSelectedItem = hItem;
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_MENU_GAMEOBJECTGROUP, point.x, point.y, this, TRUE);
		*pResult = 0;
		return;
	}
	else if (hItem == _treeArtTileRes)
	{
		_hSelectedItem = hItem;
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_MENU_RESOURCE_TILE, point.x, point.y, this, TRUE);
		*pResult = 0;
		return;
	}

	DWORD_PTR ptr = _ResourceTree.GetItemData(hItem);
	if (ptr)
	{
		Resource* pGroup = (Resource*)ptr;
		if (pGroup && pGroup->GetResourceType() == eResTypeGameObject)
		{
			_pSelectedGroup	= pGroup;
			_hSelectedItem	= hItem;
			theApp.GetContextMenuManager()->ShowPopupMenu(IDR_MENU_GAMEOBJECTGROUP, point.x, point.y, this, TRUE);
		}
		else if (pGroup && pGroup->GetResourceType() == eResTypeTile)
		{
			_pSelectedGroup	= pGroup;
			_hSelectedItem	= hItem;
			theApp.GetContextMenuManager()->ShowPopupMenu(IDR_MENU_RESOURCE_TILE, point.x, point.y, this, TRUE);
		}
	}

	*pResult = 0;
}

void ResourceTreeView::OnGameObjectGroupAdd()
{
	DialogAddGameObjectGroup dlg;
	if(dlg.DoModal() == IDOK)
	{
		ResourceGameObjectGroup* pGO = new ResourceGameObjectGroup((LPCSTR)dlg._strGroupName
			, dlg._strArtGroup
			, CPoint(dlg._iTileW, dlg._iTileH)
			, EGridType(dlg._iMapType)
			);
		if (!ResourceManager::getSingleton().AddGameObjectGroup(pGO))
		{
			delete pGO;
			MessageBox("添加游戏对象组失败！", "错误", MB_OK | MB_ICONHAND);

			return;
		}

		HTREEITEM hItem = _ResourceTree.InsertItem(pGO->GetResourceName().c_str(), 2, 2, _treeGameObjectRes);
		_ResourceTree.SetItemData(hItem, (DWORD_PTR)pGO);
		_ResourceTree.Expand(_treeGameObjectRes, TVE_EXPAND);
	}
}
void ResourceTreeView::OnUpdateCmdUI_GroupAdd(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(_pSelectedGroup || _hSelectedItem == _treeGameObjectRes);
}

#include "GameObjectEditorObjects.h"
void ResourceTreeView::OnGameObjectGroupRemove()
{
	Cactus::ostringstream os;
	os << "你确信要删除游戏对象组: '" << _pSelectedGroup->GetResourceName() << "' 吗？";
	if( MessageBox(os.str().c_str(), "询问", MB_YESNO | MB_ICONQUESTION) != IDYES )
		return;

	if (GetGODlg()->_pGOGroup == _pSelectedGroup)
	{
		Log_Error("资源正在使用！");
		return;
	}

	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd() );
	TileResView* pTileResView = pMainFrame->GetTileResView();

	if( ResourceManager::getSingleton().RemoveGameObjectGroup(_pSelectedGroup->GetResourceName()) )
	{
		pTileResView->ClearContent();
		_ResourceTree.DeleteItem(_hSelectedItem);
	}
}
void ResourceTreeView::OnUpdateCmdUI_GroupRemove(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(_pSelectedGroup != 0);
}


void ResourceTreeView::OnResourceTileAdd()
{
	DialogAddResourceTile dlg;
	if(dlg.DoModal() == IDOK)
	{
		ResourceTile* pTile;
		if (dlg._iType == 0)
		{
			pTile = new ResourceTileFolder((LPCSTR)dlg._strName, (LPCSTR)dlg._strResName, (LPCSTR)dlg._strExt, dlg._iIconSize);
		}
		else
		{
			pTile = new ResourceTileSingleImage((LPCSTR)dlg._strName, (LPCSTR)dlg._strResName, dlg._iTileW, dlg._iTileH, dlg._iIconSize, dlg._iCount);
		}

		if (!pTile->Load())
		{
			delete pTile;
			MessageBox("无效的Tile资源组！", "错误", MB_OK | MB_ICONHAND);

			return;
		}

		if (!ResourceManager::getSingleton().AddResourceTileGroup(pTile))
		{
			delete pTile;
			MessageBox("添加ResourceTile资源组失败！请检查组名是否重复。", "错误", MB_OK | MB_ICONHAND);

			return;
		}

		HTREEITEM hItem = _ResourceTree.InsertItem(pTile->GetResourceName().c_str(), 2, 2, _treeArtTileRes);
		_ResourceTree.SetItemData(hItem, (DWORD_PTR)pTile);
	}
}
void ResourceTreeView::OnUpdateCmdUI_ResourceTileAdd(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(_pSelectedGroup || _hSelectedItem == _treeArtTileRes);
}

void ResourceTreeView::OnResourceTileRemove()
{
	Cactus::ostringstream os;
	os << "你确信要删除Tile资源组: '" << _pSelectedGroup->GetResourceName() << "' 吗？";
	if( MessageBox(os.str().c_str(), "询问", MB_YESNO | MB_ICONQUESTION) != IDYES )
		return;

	if (GetGODlg()->_pGOGroup && GetGODlg()->_pGOGroup->GetResArtKey() == _pSelectedGroup->GetResourceName())
	{
		Log_Error("资源正在使用！");
		return;
	}
	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd() );
	TileResView* pTileResView = pMainFrame->GetTileResView();

	if( ResourceManager::getSingleton().RemoveResourceTile(_pSelectedGroup->GetResourceName()) )
	{
		pTileResView->ClearContent();
		_ResourceTree.DeleteItem(_hSelectedItem);
	}

}
void ResourceTreeView::OnUpdateCmdUI_ResourceTileRemove(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(_pSelectedGroup != 0);
}
