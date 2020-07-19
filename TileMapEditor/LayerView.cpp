
#include "stdafx.h"
#include "MainFrm.h"
#include "LayerView.h"
#include "Resource.h"
#include "TileMapEditor.h"

#include "MapLayer.h"
#include "ToolManager.h"
#include "ToolSelect.h"

using namespace Cactus;

#define M_ListCtrl_ID	(WM_USER + 100)
#define M_TOOLBAR_ID	(WM_USER + 101)

//////////////////////////////////////////////////////////////////////
// 构造/析构
//////////////////////////////////////////////////////////////////////

LayerView::LayerView()
: _pLayer(0)
{
}

LayerView::~LayerView()
{
}

BEGIN_MESSAGE_MAP(LayerView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_CLASS_ADD_MEMBER_FUNCTION, OnClassAddMemberFunction)
	ON_COMMAND(ID_TOOLBAR_MAPLAYER_SELECT_SIMILAR, OnInvertSelect)
	ON_COMMAND(ID_TOOLBAR_MAPLAYER_INVERT_SELECT, OnSelectSimilar)
	ON_COMMAND(ID_TOOLBAR_MAPLAYER_DELETE_SELECT, OnDeleteSelection)
	ON_COMMAND(ID_TOOLBAR_MAPLAYER_EXPORT, OnLayerExport)

	ON_NOTIFY(LVN_COLUMNCLICK, M_ListCtrl_ID, OnLvnColumnclickList)
	ON_NOTIFY(NM_CLICK, M_ListCtrl_ID, OnNMclickList)
	ON_NOTIFY(LVN_ITEMCHANGED, M_ListCtrl_ID, OnItemStateChanged)

	ON_NOTIFY(NM_CUSTOMDRAW, M_ListCtrl_ID, OnCustomdrawList)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassView 消息处理程序

int LayerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();


	// 加载图像:
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, M_TOOLBAR_ID);
	m_wndToolBar.LoadToolBar(IDR_TOOLBAR_MAPLAYER, 0, 0, TRUE /* 已锁定*/);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// 所有命令将通过此控件路由，而不是通过主框架路由:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | LVS_REPORT | LVS_SORTASCENDING | LVS_SHOWSELALWAYS ;
	_listObjects.Create(dwViewStyle, CRect(0, 0, 100, 100), this, M_ListCtrl_ID);
	//_listObjects.SetBkColor(0);
	_listObjects.SetExtendedStyle(_listObjects.GetExtendedStyle() | LVS_EX_FULLROWSELECT); 

	_listObjects.InsertColumn(0, "资源类"	, LVCFMT_LEFT, 50);
	_listObjects.InsertColumn(1, "资源ID"	, LVCFMT_LEFT, 100);
	_listObjects.InsertColumn(2, "X", LVCFMT_LEFT, 40);
	_listObjects.InsertColumn(3, "Y", LVCFMT_LEFT, 40);
	_listObjects.InsertColumn(4, "区域"	, LVCFMT_LEFT, 40);
	_listObjects.InsertColumn(5, "有效"	, LVCFMT_LEFT, 40);

	return 0;
}

void LayerView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);

	rectClient.top += cyTlb;
	_listObjects.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	_listObjects.Arrange(LVA_DEFAULT);
}

BOOL LayerView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void LayerView::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文

}

void LayerView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

}

void LayerView::OnClassAddMemberFunction()
{
	AfxMessageBox(_T("添加成员函数..."));
}

int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	STile* pData1 = (STile*)lParam1;
	STile* pData2 = (STile*)lParam2;

	int iColum = (int)lParamSort;
	if (iColum == 0)		//资源类
	{
		return pData1->_strResGroup > pData2->_strResGroup;
	}
	else if (iColum == 1)	//资源ID
	{
		return pData1->_strResItemID > pData2->_strResItemID;
	}
	else if (iColum == 2)	//X
	{
		return pData1->_ptGrid.x - pData2->_ptGrid.x;
	}
	else if (iColum == 3)	//Y
	{
		return pData1->_ptGrid.y - pData2->_ptGrid.y;
	}
	else if (iColum == 4)	//区域
	{
		return pData1->_regionID - pData2->_regionID;
	}

	return (int)(lParam1 - lParam2);
}


void LayerView::OnLvnColumnclickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	_listObjects.SortItems(MyCompareProc, pNMLV->iSubItem);

	*pResult = 0;
}

void LayerView::OnNMclickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION pos = _listObjects.GetFirstSelectedItemPosition();
	if(pos != NULL)
	{
		int nItem = _listObjects.GetNextSelectedItem(pos);
	}

	*pResult = 0;
}

void LayerView::OnItemStateChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW lpView = (LPNMLISTVIEW) pNMHDR;

	if (lpView->iItem != -1 && lpView->uChanged != 0)
	{
		STile* pData = (STile*)_listObjects.GetItemData(lpView->iItem);

		if (lpView->uNewState & LVIS_SELECTED)
		{
			Log_Info("选择Tile，位置: (" << pData->_ptGrid.x << ", " << pData->_ptGrid.y << ") 资源组：" << pData->_strResGroup << " : " << pData->_strResItemID);

			ToolSelect* pSelectTool = (ToolSelect*)ToolManager::getSingleton().GetTool(eToolSelect);
			pSelectTool->SelectTile(pData, true);
		}
		else if (lpView->uOldState & LVIS_SELECTED)
		{
			Log_Info("选择Tile，位置: (" << pData->_ptGrid.x << ", " << pData->_ptGrid.y << ") 资源组：" << pData->_strResGroup << " : " << pData->_strResItemID);

			ToolSelect* pSelectTool = (ToolSelect*)ToolManager::getSingleton().GetTool(eToolSelect);
			pSelectTool->SelectTile(pData, false);
		}
	}

	*pResult = 0;
}

void LayerView::SetCurrentLayer(MapLayer* pLayer)
{
	if (_pLayer == pLayer)
		return;

	_pLayer = pLayer;
	_listObjects.DeleteAllItems();

	if (_pLayer)
	{
		_pLayer->FillListInPanelMapLayer();
	}
}

void LayerView::AddTileInfo(STile* pTile)
{
	int iIndex = _listObjects.InsertItem(_listObjects.GetItemCount(), _T(""));

	_listObjects.SetItemText(iIndex, 0, pTile->_strResGroup.c_str());
	_listObjects.SetItemText(iIndex, 1, pTile->_strResItemID.c_str());
	
	CString strTmp;
	strTmp.Format("%d", pTile->_ptGrid.x);
	_listObjects.SetItemText(iIndex, 2, strTmp);

	strTmp.Format("%d", pTile->_ptGrid.y);
	_listObjects.SetItemText(iIndex, 3, strTmp);

	strTmp.Format("%d", pTile->_regionID);
	_listObjects.SetItemText(iIndex, 4, strTmp);

	_listObjects.SetItemText(iIndex, 5, pTile->_bValid ? "是" : "否");

	_listObjects.SetItemData(iIndex, DWORD_PTR(pTile));
}

bool LayerView::UpdateTileInfo(STile* pTile)
{
	for (int t = 0; t < _listObjects.GetItemCount(); ++t)
	{
		STile* pData = (STile*)_listObjects.GetItemData(t);
		if (pTile == pData)
		{
			CString strTmp;
			strTmp.Format("%d", pTile->_ptGrid.x);
			_listObjects.SetItemText(t, 2, strTmp);

			strTmp.Format("%d", pTile->_ptGrid.y);
			_listObjects.SetItemText(t, 3, strTmp);

			strTmp.Format("%d", pTile->_regionID);
			_listObjects.SetItemText(t, 4, strTmp);
			
			return true;
		}
	}

	return false;
}

bool LayerView::RemoveTileInfo(STile* pTile)
{
	for (int t = 0; t < _listObjects.GetItemCount(); ++t)
	{
		STile* pData = (STile*)_listObjects.GetItemData(t);
		if (pTile == pData)
		{
			_listObjects.DeleteItem(t);
			
			return true;
		}
	}

	return false;
}

void LayerView::OnInvertSelect()
{
}

void LayerView::OnSelectSimilar()
{
}

void LayerView::OnDeleteSelection()
{
	POSITION pos = _listObjects.GetFirstSelectedItemPosition();
	if(pos != NULL)
	{
		int nItem = _listObjects.GetNextSelectedItem(pos);

		STile* pData = (STile*)_listObjects.GetItemData(nItem);
		if (pData)
		{
			_pLayer->RemoveTile(pData);
			_listObjects.DeleteItem(nItem);

			Log_Info("对象删除成功。");
		}
	}
}

void LayerView::OnLayerExport()
{
	//_pLayer->GetParentMap()

	CFileDialog dlg(FALSE, "csv", _pLayer->GetObjectName().c_str(), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "csv Files (*.csv)|*.csv|text Files (*.txt)|*.txt||" );
	if( dlg.DoModal() == IDOK)
	{
		_pLayer->ExportToFile((LPCTSTR)dlg.GetPathName());
	}
}


void LayerView::SelectTile( STile* pTile )
{
	for (int t = 0; t < _listObjects.GetItemCount(); ++t)
	{
		STile* pData = (STile*)_listObjects.GetItemData(t);
		if (pTile == pData)
		{
			_listObjects.SetItemState(t, LVIS_SELECTED, LVIS_SELECTED); 
			_listObjects.EnsureVisible(t, FALSE); 
		}
		else
		{
			_listObjects.SetItemState(t, ~LVIS_SELECTED, LVIS_SELECTED); 
		}
	}

	_listObjects.Invalidate();
}

void LayerView::OnCustomdrawList ( NMHDR* pNMHDR, LRESULT* pResult )
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

	// Take the default processing unless we set this to something else below.
	*pResult = 0;

	// First thing - check the draw stage. If it's the control's prepaint
	// stage, then tell Windows we want messages for every item.

	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		// This is the notification message for an item.  We'll request
		// notifications before each subitem's prepaint stage.

		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
	{
		// This is the beginning of an item's paint cycle.
		LVITEM   rItem;
		int      nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec );

		BOOL     bListHasFocus = ( _listObjects.GetSafeHwnd() == ::GetFocus() );

		// Get the image index and selected/focused state of the item being drawn.
		ZeroMemory ( &rItem, sizeof(LVITEM) );
		rItem.mask  = LVIF_IMAGE | LVIF_STATE;
		rItem.iItem = nItem;
		rItem.stateMask = LVIS_SELECTED | LVIS_FOCUSED;
		_listObjects.GetItem ( &rItem );

		// Draw the background of the list item.  Colors are selected 
		// according to the item's state.

		COLORREF crText, crBkgnd;
		if ( rItem.state & LVIS_SELECTED )
		{
			if ( bListHasFocus )
			{
				crBkgnd = GetSysColor ( COLOR_HIGHLIGHT );
				crText = GetSysColor ( COLOR_HIGHLIGHTTEXT );
			}
			else
			{
				crBkgnd = GetSysColor ( COLOR_BTNFACE );
				crText = GetSysColor ( COLOR_BTNTEXT );
			}
		}
		else
		{
			crBkgnd = GetSysColor ( COLOR_WINDOW );
			crText = GetSysColor ( COLOR_BTNTEXT );
		}

		if ( 5 == pLVCD->iSubItem )
		{
			CString sText = _listObjects.GetItemText ( nItem, 5);
			if (sText == "否")
			{
				crText = RGB(255, 0, 0);
			}
		}

		// Store the colors back in the NMLVCUSTOMDRAW struct.
		pLVCD->clrText = crText;
		pLVCD->clrTextBk = crBkgnd;

		// Tell Windows to paint the control itself.
		*pResult = CDRF_DODEFAULT;
	}
}