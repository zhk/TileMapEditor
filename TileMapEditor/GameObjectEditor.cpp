#include "stdafx.h"
#include "GameObjectEditor.h"

#include "Resource.h"
#include "ResourceGameObject.h"


using namespace Cactus;


#define M_VIEW_ID	(WM_USER + 10)

//////////////////////////////////////////////////////////////////////
// 构造/析构
//////////////////////////////////////////////////////////////////////

GameObjectEditor::GameObjectEditor()
{
	_pView		= 0;
}

GameObjectEditor::~GameObjectEditor()
{
}

BEGIN_MESSAGE_MAP(GameObjectEditor, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_PAINT()
	ON_COMMAND(ID_CLASS_ADD_MEMBER_FUNCTION, OnClassAddMemberFunction)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassView 消息处理程序

int GameObjectEditor::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	_dlgPanel.CDialog::Create(CDialogGameObject::IDD, this);

	_pView = new GameObjectEditorView;
	_pView->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | AFX_WS_DEFAULT_VIEW, rectDummy, this, M_VIEW_ID);
	_pView->OnInitialUpdate();
	_pView->ShowWindow(SW_SHOW);

	return 0;
}

void GameObjectEditor::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	CRect rcDlg = rectClient;
	rcDlg.bottom = 55;
	_dlgPanel.MoveWindow(&rcDlg);

	CRect rcView = rectClient;
	rcView.top = rcDlg.bottom;
	_pView->SetWindowPos(&_dlgPanel, rcView.left, rcView.top, rcView.Width(), rcView.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	_pView->Invalidate();
}

BOOL GameObjectEditor::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void GameObjectEditor::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文

}


void GameObjectEditor::OnClassAddMemberFunction()
{
	AfxMessageBox(_T("添加成员函数..."));
}


void GameObjectEditor::OnContextMenu(CWnd* pWnd, CPoint point)
{
	if (pWnd != _pView && pWnd != &_dlgPanel)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	pWnd->SetFocus();
}

void GameObjectEditor::AddGameObject(ResourceGameObjectGroup* pGOGroup)
{
	_dlgPanel._bAdd			= true;
	_dlgPanel._pResGO		= new ResourceGameObject;
	_dlgPanel._pGOGroup		= pGOGroup;

	_dlgPanel._strGOGroupName	= pGOGroup->GetResourceName().c_str();
	_dlgPanel._strGOName		= "";
	_dlgPanel._iUnitTileWidth	= pGOGroup->_szUnitTile.x;
	_dlgPanel._iUnitTileHeight	= pGOGroup->_szUnitTile.y;
	_dlgPanel._strResArtGroup	= pGOGroup->_strArtResKey.c_str();
	_dlgPanel._eGridType		= pGOGroup->_eGridType;
	_dlgPanel._strCenterOffset	= "(n/a, n/a)";
	_dlgPanel._obstacles.clear();
	_dlgPanel._strAIType		= "defaultAI";

	if (_dlgPanel._eGridType == 0)
		_dlgPanel._strMapType = "矩形";
	else
		_dlgPanel._strMapType = "菱形";

	_dlgPanel.UpdateData(FALSE);
	_dlgPanel.AfterSetData(false);

	_pView->Invalidate();
}

void GameObjectEditor::EditGameObject(ResourceGameObjectGroup* pGOGroup, ResourceGameObject* pGO)
{
	_dlgPanel._bAdd			= false;
	_dlgPanel._pResGO		= pGO;
	_dlgPanel._pGOGroup		= pGOGroup;

	_dlgPanel._strGOGroupName	= pGOGroup->GetResourceName().c_str();
	_dlgPanel._strGOName		= pGO->_strName.c_str();
	_dlgPanel._iUnitTileWidth	= pGOGroup->_szUnitTile.x;
	_dlgPanel._iUnitTileHeight	= pGOGroup->_szUnitTile.y;
	_dlgPanel._strResArtGroup	= pGOGroup->_strArtResKey.c_str();
	_dlgPanel._eGridType		= pGOGroup->_eGridType;
	_dlgPanel._ptOffset			= pGO->_ptOffset;
	_dlgPanel._strCenterOffset.Format("(%d, %d)", pGO->_ptOffset.x, pGO->_ptOffset.y);
	_dlgPanel._obstacles		= pGO->_obstacles;
	_dlgPanel._strAIType		= pGO->_strAIType.c_str();

	if (_dlgPanel._eGridType == 0)
		_dlgPanel._strMapType = "矩形";
	else
		_dlgPanel._strMapType = "菱形";

	_dlgPanel.UpdateData(FALSE);
	_dlgPanel.AfterSetData(false);

	_pView->Invalidate();
}

void GameObjectEditor::CopyGameObject(ResourceGameObjectGroup* pGOGroup, ResourceGameObject* pGO)
{
	_dlgPanel._bAdd			= true;
	_dlgPanel._pResGO		= new ResourceGameObject;
	_dlgPanel._pGOGroup		= pGOGroup;

	_dlgPanel._pResGO->_strName		= pGO->_strName + "_Copy";
	_dlgPanel._pResGO->_ArtResID	= pGO->_ArtResID.c_str();
	_dlgPanel._pResGO->_ptOffset	= pGO->_ptOffset;
	_dlgPanel._pResGO->_obstacles	= pGO->_obstacles;
	_dlgPanel._pResGO->_strAIType	= pGO->_strAIType;


	_dlgPanel._strGOGroupName	= pGOGroup->GetResourceName().c_str();
	_dlgPanel._strGOName		= _dlgPanel._pResGO->_strName.c_str();
	_dlgPanel._iUnitTileWidth	= pGOGroup->_szUnitTile.x;
	_dlgPanel._iUnitTileHeight	= pGOGroup->_szUnitTile.y;
	_dlgPanel._strResArtGroup	= pGOGroup->_strArtResKey.c_str();
	_dlgPanel._eGridType		= pGOGroup->_eGridType;
	_dlgPanel._ptOffset			= pGO->_ptOffset;
	_dlgPanel._strCenterOffset.Format("(%d, %d)", pGO->_ptOffset.x, pGO->_ptOffset.y);
	_dlgPanel._obstacles		= pGO->_obstacles;
	_dlgPanel._strAIType		= pGO->_strAIType.c_str();

	if (_dlgPanel._eGridType == 0)
		_dlgPanel._strMapType = "矩形";
	else
		_dlgPanel._strMapType = "菱形";

	_dlgPanel.UpdateData(FALSE);
	_dlgPanel.AfterSetData(true);

	_pView->Invalidate();
}
