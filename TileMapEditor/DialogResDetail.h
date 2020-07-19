#pragma once

#include "resource.h"

// 资源面板 中 资源组信息面板

class DialogResDetail : public CDialog
{
	DECLARE_DYNAMIC(DialogResDetail)

public:
	DialogResDetail(CWnd* pParent = NULL);   // standard constructor
	virtual ~DialogResDetail();

// Dialog Data
	enum { IDD = IDD_DIALOG_RES_DETAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString _strGroupName;
	CString _strResType;
	CString _strGridType;
	CString _strGridSize;
};
