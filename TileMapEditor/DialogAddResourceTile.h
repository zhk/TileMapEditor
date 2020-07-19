#pragma once
#include "afxwin.h"


// 资源树面板中，添加美术资源后，显示的对话框

class DialogAddResourceTile : public CDialog
{
	DECLARE_DYNAMIC(DialogAddResourceTile)

public:
	DialogAddResourceTile(CWnd* pParent = NULL);   // standard constructor
	virtual ~DialogAddResourceTile();

// Dialog Data
	enum { IDD = IDD_DIALOG_ADD_RESOUECE_TILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void	ShowHideItems();

	DECLARE_MESSAGE_MAP()
public:
	CString _strName;
	int _iType;
	CString _strResName;
	CComboBox _comboIconSize;
	CString _strExt;
	int _iTileW;
	int _iTileH;
	int _iCount;
	int _iIconSize;
	afx_msg void OnBnClickedButtonBrowse();
	afx_msg void OnBnClickedRadioTileType2();
	afx_msg void OnBnClickedRadioTileType();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
