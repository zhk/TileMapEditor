
#include "stdafx.h"
#include "MainFrm.h"
#include "TileResView.h"
#include "Resource.h"
#include "TileMapEditor.h"

#include "ToolManager.h"
#include "ToolBrush.h"
#include "GameObjectEditor.h"
#include "Map.h"

#include "ResourceManager.h"
#include "ResourceGameObject.h"

#include "GameObjectEditorObjects.h"

using namespace Cactus;

#define M_ListCtrl_ID	(WM_USER + 100)


/*
BUTTON      ID_TOOLBAR_RESDETAIL_RESGROUP_C
BUTTON      ID_TOOLBAR_REDETAIL_RESGROUP
SEPARATOR
BUTTON      ID_TOOLBAR_RESDETAIL_TYPE_C
BUTTON      ID_TOOLBAR_RESDETAIL_TYPE
SEPARATOR
BUTTON      ID_TOOLBAR_RESDETAIL_EDIT
*/
//
//BEGIN_MESSAGE_MAP(ResDetailToolBar, CClassToolBar)
//	//ON_WM_CTLCOLOR()
//END_MESSAGE_MAP()

//HBRUSH ResDetailToolBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	if(nCtlColor == CTLCOLOR_STATIC) 
//	{ 
//		//pDC->SetTextColor(RGB(0,0,255)); 
//		pDC->SetBkMode(TRANSPARENT);
//
//		return (HBRUSH)_brush;
//	} 
//
//	return CClassToolBar::OnCtlColor(pDC, pWnd, nCtlColor);
//}
//
//
//void ResDetailToolBar::CreateControls()
//{
//	_brush.CreateStockObject(NULL_BRUSH);
//	_font.CreateStockObject(DEFAULT_GUI_FONT);
//
//	int iIndex = 0;
//	while ( GetItemID(iIndex) != ID_TOOLBAR_RESDETAIL_RESGROUP_C) iIndex++;
//
//	CRect rc(5, 2, 55, 20);
//	_editResGroupC.Create("资源组：", WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, rc, this, ID_TOOLBAR_RESDETAIL_RESGROUP_C);
//	_editResGroupC.SetFont(&_font);
//
//
//	while ( GetItemID(iIndex) != ID_TOOLBAR_REDETAIL_RESGROUP) iIndex++;
//	rc.left		= rc.right + 5;
//	rc.right	= rc.left + 60;
//	_editResGroup.Create("", WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, rc, this, ID_TOOLBAR_REDETAIL_RESGROUP);
//	_editResGroup.SetFont(&_font);
//
//	while ( GetItemID(iIndex) != ID_TOOLBAR_RESDETAIL_TYPE_C) iIndex++;
//	rc.left		= rc.right + 5;
//	rc.right	= rc.left + 60;
//	_editResTypeC.Create("资源类型：", WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, rc, this, ID_TOOLBAR_RESDETAIL_TYPE_C);
//	_editResTypeC.SetFont(&_font);
//
//	while ( GetItemID(iIndex) != ID_TOOLBAR_RESDETAIL_TYPE) iIndex++;
//	rc.left		= rc.right + 5;
//	rc.right	= rc.left + 50;
//	_editResType.Create("", WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, rc, this, ID_TOOLBAR_RESDETAIL_TYPE);
//	_editResType.SetFont(&_font);
//
//
//	while ( GetItemID(iIndex) != ID_TOOLBAR_RESDETAIL_EDIT) iIndex++;
//	rc.left		= rc.right + 5;
//	rc.right	= rc.left + 36;
//	_btnEdit.Create("编辑", WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | BS_PUSHBUTTON, rc, this, ID_TOOLBAR_RESDETAIL_EDIT);
//	_btnEdit.SetFont(&_font);
//}


//////////////////////////////////////////////////////////////////////
// 构造/析构
//////////////////////////////////////////////////////////////////////

TileResView::TileResView()
: _iCurSelectItem(-1)
, _iOldCheck(-1)
{
}

TileResView::~TileResView()
{
}

BEGIN_MESSAGE_MAP(TileResView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_CLASS_ADD_MEMBER_FUNCTION, OnClassAddMemberFunction)
	ON_NOTIFY(LVN_ITEMCHANGED, M_ListCtrl_ID, OnItemChanged)
	ON_NOTIFY(NM_DBLCLK, M_ListCtrl_ID, &TileResView::OnNMDblclkListItem)
	ON_NOTIFY(NM_RCLICK, M_ListCtrl_ID, &TileResView::OnNMRclickListItem)

	ON_COMMAND(ID_MENU_RESVIEW_ADD, OnItemAdd)
	ON_COMMAND(ID_MENU_RESVIEW_EDIT, OnItemEdit)
	ON_COMMAND(ID_MENU_RESVIEW_REMOVE, OnItemRemove)
	ON_COMMAND(ID_MENU_RESVIEW_COPY, OnItemCopy)


	//ON_COMMAND_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnSort)
	//ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnUpdateSort)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassView 消息处理程序

int TileResView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();


	//创建工具条
	_dialogBar.Create(DialogResDetail::IDD, this);

	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | LVS_ICON | LVS_SORTASCENDING;
	_listImages.Create(dwViewStyle, CRect(0, 0, 100, 100), this, M_ListCtrl_ID);
	_listImages.SetExtendedStyle( _listImages.GetExtendedStyle() | LVS_EX_CHECKBOXES);
	//_listImages.SetBkColor(0);

	return 0;
}

void TileResView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = 50;

	_dialogBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);

	rectClient.top += cyTlb;
	_listImages.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	_listImages.Arrange(LVA_DEFAULT);
}

BOOL TileResView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void TileResView::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文

}

void TileResView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

}

void TileResView::OnClassAddMemberFunction()
{
	AfxMessageBox(_T("添加成员函数..."));
}

int CALLBACK __MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	String* pData1 = (String*)lParam1;
	String* pData2 = (String*)lParam2;


	return (int)(*pData1 < *pData2);
}


void TileResView::BuildImageAndInfoes(Resource* pResGroup)
{
	_listImages.DeleteAllItems();

	_iOldCheck		= -1;
	_strResGroup	= pResGroup->GetResourceName();
	_eType			= pResGroup->GetResourceType();
	_pCurResGroup	= pResGroup;

	_dialogBar._strGroupName	= _strResGroup.c_str();
	if (_eType == eResTypeTile || _eType == eResTypeBackground)
	{
		_dialogBar._strResType	= "美术资源";
	}
	else if (_eType == eResTypeGameObject)
	{
		_dialogBar._strResType	= "游戏对象";
	}

	if( _pCurResGroup->GetResourceType() == eResTypeGameObject )
	{
		ResourceGameObjectGroup* pResGO = (ResourceGameObjectGroup *)_pCurResGroup;
		if ( pResGO->GetGridType() == eRectangle)
			_dialogBar._strGridType = "矩形";
		else
			_dialogBar._strGridType = "菱形";

		_dialogBar._strGridSize.Format("(%d, %d)", pResGO->GetUnitTileSize().cx, pResGO->GetUnitTileSize().cy);
	}
	else
	{
		_dialogBar._strGridType = "N/A";
		_dialogBar._strGridSize = "N/A";
	}

	_dialogBar.UpdateData(FALSE);

	//Clear brush tool
	ToolManager::getSingleton().SelectTool(eToolSelect);

	_listImages.SetImageList(pResGroup->GetImageList(), LVSIL_NORMAL);

	const Cactus::StringVector& captions = *pResGroup->GetCaptions();
	for (size_t t = 0; t < captions.size(); ++t)
	{
		_listImages.InsertItem(t, captions[t].c_str(), t);
		//_listImages.SetItemData(t, (DWORD_PTR)&(captions[t]));
	}

	//_listImages.SortItems(__MyCompareProc, 0);
}

void TileResView::ReBuildContent()
{
	ResourceGameObjectGroup* pGOGroup = (ResourceGameObjectGroup*)ResourceManager::getSingleton().GetResourceGameObjectGroup(_strResGroup);
	if (pGOGroup)
	{
		pGOGroup->CreateImageList(GetDC(), true);

		BuildImageAndInfoes(pGOGroup);
	}
}

void TileResView::ClearContent()
{
	_listImages.DeleteAllItems();

	_iOldCheck		= -1;
	_strResGroup	= "";
	_eType			= eResTypeTile;
	_pCurResGroup	= 0;

	_dialogBar._strGroupName	= "";
	_dialogBar._strResType	= "N/A";
	_dialogBar._strGridType = "N/A";
	_dialogBar._strGridSize = "N/A";

	_dialogBar.UpdateData(FALSE);

	//Clear brush tool
	ToolManager::getSingleton().SelectTool(eToolSelect);
}


void TileResView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CListCtrl* pWndList = (CListCtrl*)&_listImages;
	ASSERT_VALID(pWndList);

	if (pWnd != pWndList)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	pWndList->SetFocus();
}

void TileResView::OnItemChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR); 

	if((pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(1)) /* old state : unchecked */ 
		&& (pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(2)) /* new state : checked */ 
		) 
	{ 
		//TRACE("Item %d is checked\n", pNMLV->iItem);

		Map* pMap = ToolManager::getSingleton().GetMap();

		if( pMap && _pCurResGroup->GetResourceType() == eResTypeGameObject )
		{
			ResourceGameObjectGroup* pResGO = (ResourceGameObjectGroup *)_pCurResGroup;
			if (pMap->_eGridType != pResGO->GetGridType() )
			{
				Log_Error("资源的网格类型 和 当前地图的网格类型 不匹配！");

				_listImages.SetCheck(pNMLV->iItem, FALSE);
				return;
			}
			else if( CSize(pMap->_iUnitTileWidth, pMap->_iUnitTileHeight) != pResGO->GetUnitTileSize() )
			{
				Log_Warn("资源的单位网格大小 和 当前地图的单位网格大小 不匹配！");
			}
		}

		if (_iOldCheck != -1)
		{
			_listImages.SetCheck(_iOldCheck, FALSE);
		}

		_iOldCheck = pNMLV->iItem;
		CString str = _listImages.GetItemText(_iOldCheck, 0);

		//Set brush tool
		ToolBrush* pBrush = (ToolBrush*)ToolManager::getSingleton().SelectTool(eToolBrush);
		pBrush->SetResource(_strResGroup, (LPCTSTR)str);

		CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd() );
		CView* pView = pMainFrame->GetActiveView(); 
		pView->SetFocus();
	} 
	else if((pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(2)) /* old state : checked */ 
		&& (pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(1)) /* new state : unchecked */ 
		) 
	{ 
		//TRACE("Item %d is unchecked\n", pNMLV->iItem); 
		_iOldCheck = -1;
		ToolManager::getSingleton().SelectTool(eToolSelect);
	} 
	else 
	{ 
		//TRACE("Item %d does't change the check-status\n", pNMLV->iItem); 
	} 

	*pResult = 0; 

}

void TileResView::OnNMDblclkListItem(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR); 

	//if (_iOldCheck != -1)
	//{
	//	_listImages.SetCheck(_iOldCheck, FALSE);
	//}

	//_iOldCheck = pNMLV->iItem;
	//_listImages.SetCheck(_iOldCheck, TRUE);

	//CString str = _listImages.GetItemText(_iOldCheck, 0);

	////Set brush tool
	//ToolBrush* pBrush = (ToolBrush*)ToolManager::getSingleton().SelectTool(eToolBrush);
	//pBrush->SetResource(_strResGroup, (LPCTSTR)str);

	*pResult = 0;
}


void TileResView::OnNMRclickListItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	_iCurSelectItem = -1;

	if (_eType == eResTypeTile || _eType == eResTypeBackground)
	{
		*pResult = 0;
		return;
	}

	LPNMITEMACTIVATE pia = (LPNMITEMACTIVATE)pNMHDR;
	CPoint point(pia->ptAction);

	// Select the item the user clicked on.
	UINT uFlags;
	_iCurSelectItem = _listImages.HitTest(point, &uFlags);
	if (uFlags & LVHT_ONITEMICON)
	{
		_listImages.ClientToScreen(&point);
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_MENU_POPUP_RESVIEW, point.x, point.y, this, TRUE);
	}
	else if (uFlags & LVHT_NOWHERE)
	{
		_listImages.ClientToScreen(&point);
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_MENU_POPUP_GAMEOBJECT_ADD, point.x, point.y, this, TRUE);
	}

	*pResult = 0;
}

void TileResView::OnItemAdd()
{
	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd() );

	GameObjectEditor* pGOEditor = pMainFrame->GetGameObjectEditor();
	pGOEditor->ShowPane(TRUE, FALSE, TRUE);

	ResourceGameObjectGroup* pGOGroup = (ResourceGameObjectGroup*)ResourceManager::getSingleton().GetResourceGameObjectGroup(_strResGroup);
	if (pGOGroup)
	{
		pGOEditor->AddGameObject(pGOGroup);
	}
}

void TileResView::OnItemEdit()
{
	if (_iCurSelectItem != -1)
	{
		CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd() );

		GameObjectEditor* pGOEditor = pMainFrame->GetGameObjectEditor();
		pGOEditor->ShowPane(TRUE, FALSE, TRUE);

		CString str = _listImages.GetItemText(_iCurSelectItem, 0);
		ResourceGameObjectGroup* pGOGroup = (ResourceGameObjectGroup*)ResourceManager::getSingleton().GetResourceGameObjectGroup(_strResGroup);
		if (pGOGroup)
		{
			ResourceGameObject* pGO = pGOGroup->GetGameObject((LPCTSTR)str);
			if (pGO)
			{
				pGOEditor->EditGameObject(pGOGroup, pGO);
			}
		}
	}
}

void TileResView::OnItemRemove()
{
	if (_iCurSelectItem != -1)
	{
		CString str = _listImages.GetItemText(_iCurSelectItem, 0);
		ResourceGameObjectGroup* pGOGroup = (ResourceGameObjectGroup*)ResourceManager::getSingleton().GetResourceGameObjectGroup(_strResGroup);
		if (pGOGroup)
		{
			if (GetGODlg() && GetGODlg()->_pGOGroup == pGOGroup)
			{
				Log_Error("不能删除，资源组有对象正在编辑！");
				return;
			}

			if( pGOGroup->RemoveGameObject((LPCTSTR)str) )
			{
				pGOGroup->CreateImageList(GetDC(), true);

				BuildImageAndInfoes(pGOGroup);
			}
		}
	}
}

void TileResView::OnItemCopy()
{
	if (_iCurSelectItem != -1)
	{
		CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd() );

		GameObjectEditor* pGOEditor = pMainFrame->GetGameObjectEditor();
		pGOEditor->ShowPane(TRUE, FALSE, TRUE);

		CString str = _listImages.GetItemText(_iCurSelectItem, 0);
		ResourceGameObjectGroup* pGOGroup = (ResourceGameObjectGroup*)ResourceManager::getSingleton().GetResourceGameObjectGroup(_strResGroup);
		if (pGOGroup)
		{
			ResourceGameObject* pGO = pGOGroup->GetGameObject((LPCTSTR)str);
			if (pGO)
			{
				pGOEditor->CopyGameObject(pGOGroup, pGO);
			}
		}
	}
}
