#pragma once
#include "afxwin.h"


// 新建地图对话框

class DialogFileNew : public CDialog
{
	DECLARE_DYNAMIC(DialogFileNew)

public:
	DialogFileNew(CWnd* pParent = NULL);   // standard constructor
	virtual ~DialogFileNew();

// Dialog Data
	enum { IDD = IDD_DIALOG_FILE_NEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
public:
	int		_iMapWidth;
	int		_iMapHeight;
	int		_iUnitTileWidth;
	int		_iUnitTileHeight;
	CString _strFootnotes;
	CString _strMapName;
	int _iType;
	BOOL _bCreateLayer;
	int _iRegionW;
	int _iRegionH;
	int _iPixelH;
	int _iPixelW;
	int _iPixelW_R;
	int _iPixelH_R;

	afx_msg void OnCbnSelchangeComboType();
	afx_msg void OnEnChangeEdits();
};
