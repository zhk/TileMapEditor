#pragma once

#include "DialogResDetail.h"
#include "ResourceType.h"

//class ResDetailToolBar : public CClassToolBar
//{
//public:
//
//	void		CreateControls();
//
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor ); 
//
//	DECLARE_MESSAGE_MAP()
//
//protected:
//	CStatic	_editResGroupC;
//	CStatic	_editResGroup;
//	
//	CStatic	_editResTypeC;
//	CStatic	_editResType;
//
//	CButton	_btnEdit;
//
//	CBrush	_brush;
//	CFont	_font;
//};

class Resource;

//资源组面板

class TileResView : public CDockablePane
{
public:
	TileResView();
	virtual ~TileResView();

	void		BuildImageAndInfoes(Resource* pResGroup);

	void		ReBuildContent();

	void		ClearContent();

protected:
	DialogResDetail		_dialogBar;
	CListCtrl			_listImages;
	int					_iOldCheck;
	int					_iCurSelectItem;
	Cactus::String		_strResGroup;
	EResourceType		_eType;
	Resource*			_pCurResGroup;

// 重写
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnItemChanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListItem(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickListItem(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClassAddMemberFunction();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnItemAdd();
	afx_msg void OnItemEdit();
	afx_msg void OnItemRemove();
	afx_msg void OnItemCopy();

	DECLARE_MESSAGE_MAP()
};

