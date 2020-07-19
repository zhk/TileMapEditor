// DialogNewLayer.cpp : implementation file
//

#include "stdafx.h"
#include "TileMapEditor.h"
#include "DialogNewLayer.h"


// DialogNewLayer dialog

IMPLEMENT_DYNAMIC(DialogNewLayer, CDialog)

DialogNewLayer::DialogNewLayer(CWnd* pParent /*=NULL*/)
	: CDialog(DialogNewLayer::IDD, pParent)
	, _strLayerName(_T("layer_xx"))
	, _bBackgroundLayer(FALSE)
{
}

DialogNewLayer::~DialogNewLayer()
{
}

void DialogNewLayer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LAYERNAME, _strLayerName);
	DDX_Check(pDX, IDC_CHECK_LAYER_BACKGROUND, _bBackgroundLayer);
}


BEGIN_MESSAGE_MAP(DialogNewLayer, CDialog)
END_MESSAGE_MAP()


// DialogNewLayer message handlers
void DialogNewLayer::OnOK()
{
	UpdateData();

	if (_strLayerName.IsEmpty())
	{
		MessageBox("层名不能为空！", "提示", MB_OK | MB_ICONINFORMATION);
		return;
	}

	CDialog::OnOK();
}
