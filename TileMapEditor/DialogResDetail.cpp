// DialogResDetail.cpp : implementation file
//

#include "stdafx.h"
#include "TileMapEditor.h"
#include "DialogResDetail.h"


// DialogResDetail dialog

IMPLEMENT_DYNAMIC(DialogResDetail, CDialog)

DialogResDetail::DialogResDetail(CWnd* pParent /*=NULL*/)
	: CDialog(DialogResDetail::IDD, pParent)
	, _strGroupName(_T("N/A"))
	, _strResType(_T("N/A"))
	, _strGridType(_T(""))
	, _strGridSize(_T(""))
{

}

DialogResDetail::~DialogResDetail()
{
}

void DialogResDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RES_DETAIL_GROUPNAME, _strGroupName);
	DDX_Text(pDX, IDC_EDIT_RES_DETAIL_RESTYPE, _strResType);
	DDX_Text(pDX, IDC_EDIT_RES_GRIDTYPE, _strGridType);
	DDX_Text(pDX, IDC_EDIT_RES_GRIDSIZE, _strGridSize);
}


BEGIN_MESSAGE_MAP(DialogResDetail, CDialog)
END_MESSAGE_MAP()


// DialogResDetail message handlers
