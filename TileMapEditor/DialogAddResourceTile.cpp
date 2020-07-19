// DialogAddResourceTile.cpp : implementation file
//

#include "stdafx.h"
#include "TileMapEditor.h"
#include "DialogAddResourceTile.h"

#include "FolderDlg.h"
#include "ResourceManager.h"

using namespace Cactus;

// DialogAddResourceTile dialog

IMPLEMENT_DYNAMIC(DialogAddResourceTile, CDialog)

DialogAddResourceTile::DialogAddResourceTile(CWnd* pParent /*=NULL*/)
	: CDialog(DialogAddResourceTile::IDD, pParent)
	, _strName(_T("new_res"))
	, _iType(0)
	, _strResName(_T(""))
	, _strExt(_T("png"))
	, _iTileW(64)
	, _iTileH(64)
	, _iCount(10)
{

}

DialogAddResourceTile::~DialogAddResourceTile()
{
}

void DialogAddResourceTile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TILE_NAME, _strName);
	DDX_Radio(pDX, IDC_RADIO_TILE_TYPE2, _iType);
	DDX_Text(pDX, IDC_EDIT_RES_NAME, _strResName);
	DDX_Control(pDX, IDC_COMBO_ICONSIZE, _comboIconSize);
	DDX_Text(pDX, IDC_EDIT_EXT, _strExt);
	DDX_Text(pDX, IDC_EDIT_TILE_SIZE_W, _iTileW);
	DDX_Text(pDX, IDC_EDIT_TILE_SIZE_H, _iTileH);
	DDX_Text(pDX, IDC_EDIT_COUNT, _iCount);
}


BEGIN_MESSAGE_MAP(DialogAddResourceTile, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, &DialogAddResourceTile::OnBnClickedButtonBrowse)
	ON_BN_CLICKED(IDC_RADIO_TILE_TYPE2, &DialogAddResourceTile::OnBnClickedRadioTileType2)
	ON_BN_CLICKED(IDC_RADIO_TILE_TYPE, &DialogAddResourceTile::OnBnClickedRadioTileType)
	ON_BN_CLICKED(IDOK, &DialogAddResourceTile::OnBnClickedOk)
END_MESSAGE_MAP()


// DialogAddResourceTile message handlers

void DialogAddResourceTile::OnBnClickedButtonBrowse()
{
	CString strDataDir = ResourceManager::getSingleton().GetRootFolder().c_str();
	if (_iType == 0)
	{
		CFolderDialog dlg("请选择Tile资源所在路径：", strDataDir);
		if( dlg.DoModal() == IDOK )
		{
			_strResName = dlg.GetFolderPath();
			_strResName.MakeLower();
			
			strDataDir.MakeLower();
			_strResName = _strResName.Right(_strResName.GetLength() - strDataDir.GetLength());

			_strName = CString("Art_") + (LPCTSTR)dlg.GetFolderName();

			if (_strResName.GetLength() != 0)
			{
				_strResName += "\\";
			}

			UpdateData(FALSE);
		}
	}
	else
	{
		try
		{
			CFileDialog dlg(TRUE);
			if( dlg.DoModal() == IDOK)
			{
				CxImage image;
				if( !image.Load(dlg.GetPathName()) )
				{
					MessageBox("不支持的图像格式！", "错误", MB_OK | MB_ICONHAND);
				}
				else
				{
					_strResName = dlg.GetPathName();
					_strResName.MakeLower();

					strDataDir.MakeLower();
					_strResName = _strResName.Right(_strResName.GetLength() - strDataDir.GetLength());

					_strName = CString("Art_") + (LPCTSTR)dlg.GetFileTitle();

					UpdateData(FALSE);
				}
			}
		}
		catch(...)
		{
			_strResName = "";
		}
	}
}

void DialogAddResourceTile::ShowHideItems()
{
	if (_iType == 0)
	{
		GetDlgItem(IDC_STATIC_NAME)->SetWindowText("路径名：");
		GetDlgItem(IDC_STATIC_TILE_SIZE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_TILE_SIZE_W)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_TILE_SIZE_H)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_EXT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_EXT)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_STATIC_COUNT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_COUNT)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_STATIC_NAME)->SetWindowText("文件名：");
		GetDlgItem(IDC_STATIC_TILE_SIZE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_TILE_SIZE_W)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_TILE_SIZE_H)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_STATIC_EXT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_EXT)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_COUNT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_COUNT)->ShowWindow(SW_SHOW);
	}
}


void DialogAddResourceTile::OnBnClickedRadioTileType2()
{
	UpdateData(TRUE);

	ShowHideItems();
}

void DialogAddResourceTile::OnBnClickedRadioTileType()
{
	UpdateData(TRUE);

	ShowHideItems();
}

BOOL DialogAddResourceTile::OnInitDialog()
{
	CDialog::OnInitDialog();

	_comboIconSize.SetCurSel(3);

	ShowHideItems();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void DialogAddResourceTile::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (_strName.GetLength() == 0 || _strResName.GetLength() == 0)
	{
		MessageBox("请把数据填写完整！", "错误", MB_OK | MB_ICONHAND);
		return;
	}

	if (_iType == 0)
	{
		if (_strExt.GetLength() == 0)
		{
			MessageBox("请把数据填写完整！", "错误", MB_OK | MB_ICONHAND);
			return;
		}
	}
	else
	{
		if (_iTileW < 1 || _iTileH < 1 || _iCount < 1)
		{
			MessageBox("数据非法！", "错误", MB_OK | MB_ICONHAND);
			return;
		}
	}

	CString str;
	_comboIconSize.GetLBText(_comboIconSize.GetCurSel(), str);
	_iIconSize = atoi((LPCTSTR)str);

	OnOK();
}
