// DialogGameObject.cpp : implementation file
//

#include "stdafx.h"
#include "TileMapEditor.h"
#include "DialogGameObject.h"

#include "ResourceManager.h"
#include "ResourceTile.h"
#include "ResourceGameObject.h"

#include "GameObjectEditorObjects.h"
#include "MainFrm.h"

// CDialogGameObject dialog

IMPLEMENT_DYNAMIC(CDialogGameObject, CDialog)

CDialogGameObject::CDialogGameObject(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogGameObject::IDD, pParent)
	, _strGOGroupName(_T(""))
	, _strGOName(_T(""))
	, _strCenterOffset(_T(""))
	, _strResArtGroup(_T(""))
	, _iMode(0)
	, _strMapType(_T(""))
{
	_ptSelected = CPoint(0, 0);
	_pResGO		= 0;
	_pGOGroup	= 0;

	_iWidthInTiles	= 10;
	_iHeightInTiles	= 10;
}

CDialogGameObject::~CDialogGameObject()
{
}

void CDialogGameObject::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_GO_GROUP, _strGOGroupName);
	DDX_Text(pDX, IDC_EDIT_GO_OBJ_NAME, _strGOName);
	DDX_Text(pDX, IDC_EDIT_GO_TILEW, _iUnitTileWidth);
	DDX_Text(pDX, IDC_EDIT_GO_TILEH, _iUnitTileHeight);
	DDX_Text(pDX, IDC_EDIT_GO_TILE_COUNT_X, _iWidthInTiles);
	DDX_Text(pDX, IDC_EDIT_GO_CENTER_POS, _strCenterOffset);
	DDX_Text(pDX, IDC_EDIT_GO_ARTGROUP, _strResArtGroup);
	DDX_Control(pDX, IDC_COMBO_GO_ARTID, _comboArt);
	DDX_Radio(pDX, IDC_RADIO_GO_SELECT, _iMode);
	DDX_Text(pDX, IDC_EDIT_GO_MAPTYPE, _strMapType);
	DDX_Control(pDX, IDC_SPIN, _spinTileCount);
	DDX_Text(pDX, IDC_EDIT_AITYPE, _strAIType);
}


BEGIN_MESSAGE_MAP(CDialogGameObject, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_GO_OK, &CDialogGameObject::OnBnClickedButtonGoOk)
	ON_BN_CLICKED(IDC_BUTTON_GO_CANCEL, &CDialogGameObject::OnBnClickedButtonGoCancel)
	ON_EN_CHANGE(IDC_EDIT_GO_TILE_COUNT_X, &CDialogGameObject::OnEnChangeEditGoTileCount)
	ON_CBN_SELCHANGE(IDC_COMBO_GO_ARTID, &CDialogGameObject::OnCbnSelchangeComboGoArtid)
	ON_BN_CLICKED(IDC_RADIO_GO_SET_OBSTACLE, &CDialogGameObject::OnBnClickedRadioGoSetObstacle)
	ON_BN_CLICKED(IDC_RADIO_GO_CLEAR_OBSTACLE, &CDialogGameObject::OnBnClickedRadioGoClearObstacle)
	ON_BN_CLICKED(IDC_RADIO_GO_SELECT, &CDialogGameObject::OnBnClickedRadioGoSelect)
END_MESSAGE_MAP()


BOOL CDialogGameObject::OnInitDialog()
{
	CDialog::OnInitDialog();

	_spinTileCount.SetRange(10, 300);
	_spinTileCount.SetBuddy(GetDlgItem(IDC_EDIT_GO_TILE_COUNT_X));

	UpdateCenterInfo();

	//TODO : 加载AITyps

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// CDialogGameObject message handlers

void CDialogGameObject::OnBnClickedButtonGoOk()
{
	if (!_pResGO)
		return;

	UpdateData(TRUE);

	CString strLabel;
	_comboArt.GetLBText(_comboArt.GetCurSel(), strLabel);

	_pResGO->_strName		= _strGOName;
	_pResGO->_ptOffset		= _ptOffset;
	_pResGO->_obstacles		= _obstacles;
	_pResGO->_ArtResID		= (LPCTSTR)strLabel;
	_pResGO->_strAIType		= (LPCTSTR)_strAIType;


	bool bRet = false;
	if (_bAdd)
	{
		bRet = _pGOGroup->AddGameObject(_pResGO);
	}
	else
	{
		bRet = _pGOGroup->UpdateGameObject(_pResGO);
	}

	if (bRet)
	{
		_bAdd = false;	//切换为编辑模式

		CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd() );
		pMainFrame->GetTileResView()->ReBuildContent();
	}
}

void CDialogGameObject::OnBnClickedButtonGoCancel()
{
	if (!_pResGO)
		return;

	if (_bAdd)
		delete _pResGO;

	_pResGO		= 0;
	_pGOGroup	= 0;

	_strGOGroupName		= "";	
	_strGOName			= "";	
	_strCenterOffset	= "";	
	_strResArtGroup		= "";	
	_comboArt.ResetContent();
	_strMapType			= "";		

	_obstacles.clear();

	UpdateData(FALSE);
	
	GameObjectEditorView* pView = GetGOView();
	pView->Invalidate();
}


void CDialogGameObject::OnEnChangeEditGoTileCount()
{
	if (!_pResGO)
		return;

	UpdateData(TRUE);

	if (_iWidthInTiles < 10)
	{
		_iWidthInTiles = 10;
		UpdateData(FALSE);
	}

	_iHeightInTiles	= _iWidthInTiles;

	CRect rcCenter	= GetPixelCoordRect(CPoint(_iWidthInTiles/2, _iWidthInTiles/2));
	_ptSelected		= _ptOffset + rcCenter.CenterPoint();


	GameObjectEditorView* pView = GetGOView();

	CSize sizeTotal;
	sizeTotal.cx	= GetPixelWidth();
	sizeTotal.cy	= GetPixelHeight();
	pView->SetScrollSizes(MM_TEXT, sizeTotal);

	pView->Invalidate();
}

void CDialogGameObject::OnCbnSelchangeComboGoArtid()
{
	if (!_pResGO)
		return;

	CString strLabel;
	_comboArt.GetLBText(_comboArt.GetCurSel(), strLabel);

	Resource* pResTile = ResourceManager::getSingleton().GetResourceArtGroup((LPCTSTR)_strResArtGroup);
	if (pResTile)
	{
		CRect rc = CRect(_ptSelected, _szSelected);

		rc = pResTile->GetResItemBoundingRect(rc, eGridNone, (LPCTSTR)strLabel);
		_szSelected = rc.Size();

		GameObjectEditorView* pView = GetGOView();
		pView->Invalidate();
	}

	if (_strGOName.IsEmpty())
	{
		_strGOName = CString("goi_") + strLabel;
	}

	UpdateData(FALSE);
}

void CDialogGameObject::OnBnClickedRadioGoSelect()
{
	if (!_pResGO)
		return;

	_iMode = 0;
	GetGOTool()->SetCurToolMode(eToolModeSelect);
}

void CDialogGameObject::OnBnClickedRadioGoSetObstacle()
{
	if (!_pResGO)
		return;

	_iMode = 1;
	GetGOTool()->SetCurToolMode(eToolModeSetObstacle);
}

void CDialogGameObject::OnBnClickedRadioGoClearObstacle()
{
	if (!_pResGO)
		return;

	_iMode = 2;
	GetGOTool()->SetCurToolMode(eToolModeClearObstacle);
}


//----------------------------------------------------------------------------------------------------------
void CDialogGameObject::AfterSetData(bool bCopy)
{
	_comboArt.ResetContent();

	ResourceTile* pResTile = ResourceManager::getSingleton().GetResourceTileGroup((LPCTSTR)_strResArtGroup);
	if (pResTile)
	{
		Cactus::StringVector* vectorCaps = pResTile->GetCaptions();
		for (size_t t = 0; t < vectorCaps->size(); ++t)
		{
			_comboArt.InsertString(t, (*vectorCaps)[t].c_str() );
		}

		if (vectorCaps->size())
		{
			if (_pResGO->_ArtResID.size())
				_comboArt.SelectString(0, _pResGO->_ArtResID.c_str());
			else
				_comboArt.SetCurSel(0);
		}
	}
	OnCbnSelchangeComboGoArtid();


	CRect rcCenter = GetPixelCoordRect(CPoint(_iWidthInTiles/2, _iWidthInTiles/2));
	if (_bAdd && !bCopy)
	{
		_ptSelected		= CPoint(_iUnitTileWidth * _iWidthInTiles/3, _iUnitTileHeight * _iWidthInTiles/3);
		_ptOffset		= _ptSelected - rcCenter.CenterPoint();
	}
	else
	{
		_ptSelected		= _ptOffset + rcCenter.CenterPoint();
	}
}

void CDialogGameObject::DrawEditingObject(CDC* pDC)
{
	ResourceTile* pResTile = ResourceManager::getSingleton().GetResourceTileGroup((LPCTSTR)_strResArtGroup);
	if (pResTile)
	{
		CString strLabel;
		_comboArt.GetLBText(_comboArt.GetCurSel(), strLabel);

		if (strLabel.GetLength())
		{
			pResTile->Draw(pDC, _ptSelected, (LPCTSTR)strLabel);
		}

		// 绘制阻挡信息
		for (ObstacleListType::iterator it = _obstacles.begin(); it != _obstacles.end(); ++it)
		{
			CPoint pt = *it;
			DrawGrid(pDC, CPoint(_iWidthInTiles/2 + pt.x, _iWidthInTiles/2 + pt.y), RGB(255, 0, 0), 2);
		}
	}
}


bool CDialogGameObject::HitTest(CPoint ptPixel)
{
	CRect rc = CRect(_ptSelected, _szSelected);

	return rc.PtInRect(ptPixel) == TRUE;
}

void CDialogGameObject::MoveGameObject(CPoint ptOffset)
{
	CRect rcOld = CRect(_ptSelected, _szSelected);

	_ptSelected.Offset(ptOffset);
	CRect rcNew = CRect(_ptSelected, _szSelected);

	rcNew.UnionRect(rcNew, rcOld);

	UpdateCenterInfo();

	GameObjectEditorView* pView = GetGOView();
	pView->LogicInvalidate(&rcNew);
}

void CDialogGameObject::UpdateCenterInfo()
{
	CRect rcCenter = GetPixelCoordRect(CPoint(_iWidthInTiles/2, _iWidthInTiles/2));

	_ptOffset = _ptSelected - rcCenter.CenterPoint();
	
	_strCenterOffset.Format("(%d, %d)", _ptOffset.x, _ptOffset.y);

	UpdateData(FALSE);
}

bool CDialogGameObject::AddObstacle(CPoint ptPixel)
{
	CPoint ptGrid;
	if( !GetGridCoord(ptPixel, ptGrid) )
		return false;

	CPoint ptOffset = ptGrid - CPoint(_iWidthInTiles/2, _iWidthInTiles/2);

	bool bFound = false;
	for (ObstacleListType::iterator it = _obstacles.begin(); it != _obstacles.end(); ++it)
	{
		if (it->x == ptOffset.x && it->y == ptOffset.y)
		{
			bFound = true;
			break;
		}
	}

	if (!bFound)
	{
		_obstacles.push_back(ptOffset);
	}

	return bFound;
}

bool CDialogGameObject::ClearObstacle(CPoint ptPixel)
{
	CPoint ptGrid;
	if( !GetGridCoord(ptPixel, ptGrid) )
		return false;

	CPoint ptOffset = ptGrid - CPoint(_iWidthInTiles/2, _iWidthInTiles/2);

	for (ObstacleListType::iterator it = _obstacles.begin(); it != _obstacles.end(); ++it)
	{
		if (it->x == ptOffset.x && it->y == ptOffset.y)
		{
			_obstacles.erase(it);
			return true;
		}
	}

	return false;
}
