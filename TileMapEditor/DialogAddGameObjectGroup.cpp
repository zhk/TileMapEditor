// DialogAddGameObjectGroup.cpp : implementation file
//

#include "stdafx.h"
#include "TileMapEditor.h"
#include "DialogAddGameObjectGroup.h"

#include "ResourceManager.h"

using namespace Cactus;


// DialogAddGameObjectGroup dialog

IMPLEMENT_DYNAMIC(DialogAddGameObjectGroup, CDialog)

DialogAddGameObjectGroup::DialogAddGameObjectGroup(CWnd* pParent /*=NULL*/)
	: CDialog(DialogAddGameObjectGroup::IDD, pParent)
	, _strGroupName(_T("newtype"))
	, _iTileW(160)
	, _iTileH(80)
	, _iMapType(1)
{

}

DialogAddGameObjectGroup::~DialogAddGameObjectGroup()
{
}

void DialogAddGameObjectGroup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_GROUPNAME, _strGroupName);
	DDX_Control(pDX, IDC_COMBO_ARTRES, _comboArList);
	DDX_Text(pDX, IDC_EDIT_TILEW, _iTileW);
	DDV_MinMaxInt(pDX, _iTileW, 1, 1024);
	DDX_Text(pDX, IDC_EDIT_TILEH, _iTileH);
	DDV_MinMaxInt(pDX, _iTileH, 1, 1024);
	DDX_Radio(pDX, IDC_RADIO_MAPTYPE, _iMapType);
}


BEGIN_MESSAGE_MAP(DialogAddGameObjectGroup, CDialog)
	ON_BN_CLICKED(IDOK, &DialogAddGameObjectGroup::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_ARTRES, &DialogAddGameObjectGroup::OnCbnSelchangeComboArtres)
END_MESSAGE_MAP()


// DialogAddGameObjectGroup message handlers

BOOL DialogAddGameObjectGroup::OnInitDialog()
{
	CDialog::OnInitDialog();

	_ArtGroups = ResourceManager::getSingleton().GetResourceTileNames();
	for (size_t t = 0; t < _ArtGroups.size(); ++t)
	{
		_comboArList.InsertString(t, _ArtGroups[t].c_str());
	}

	if (_ArtGroups.size() != 0)
	{
		_comboArList.SetCurSel(0);

		_strGroupName = _ArtGroups[0].c_str();

		//"Art_"
		if (_strGroupName.GetLength() > 4 && _strGroupName.Left(4) == "Art_")
			_strGroupName = CString("GO_") + _strGroupName.Right(_strGroupName.GetLength() - 4);
		else
			_strGroupName = CString("GO_") + _strGroupName;
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void DialogAddGameObjectGroup::OnCbnSelchangeComboArtres()
{
	_strGroupName = _ArtGroups[_comboArList.GetCurSel()].c_str();

	if (_strGroupName.GetLength() > 4 && _strGroupName.Left(4) == "Art_")
		_strGroupName = CString("GO_") + _strGroupName.Right(_strGroupName.GetLength() - 4);
	else
		_strGroupName = CString("GO_") + _strGroupName;

	UpdateData(FALSE);
}


void DialogAddGameObjectGroup::OnBnClickedOk()
{
	UpdateData(TRUE);

	if( !ResourceManager::getSingleton().IsGameObjectGroupNameValid((LPCTSTR)_strGroupName) )
	{
		MessageBox("对象组名已经存在！", "错误", MB_OK | MB_ICONHAND);
		return;
	}

	if (_iTileW < 1 || _iTileH < 1)
	{
		MessageBox("请检查Tile大小！", "错误", MB_OK | MB_ICONHAND);
		return;
	}

	_strArtGroup = _ArtGroups[_comboArList.GetCurSel()];
	
	OnOK();
}
