
#pragma once

#include "ViewTree.h"
#include "PanelToolBar.h"

class Map;
class MapLayer;

// µÿÕº√Ê∞Â

class MapView : public CDockablePane
{
public:
	MapView();
	virtual ~MapView();

	void	AdjustLayout();
	void	OnChangeVisualStyle();

	void	SetMapObject(Map* p);

	void	Reset();

protected:
	CClassToolBar	m_wndToolBar;
	CViewTree		_TreeMapItem;
	CImageList		_TreeImageList;

	HTREEITEM		_hLayerRoot;
	HTREEITEM		_hBrushRoot;

	MapLayer*		_pSelectedLayer;
	HTREEITEM		_hSelectedItem;

	Map*			_pTheMap;

	void FillClassView();

// ÷ÿ–¥
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnChangeActiveTab(WPARAM, LPARAM);
	afx_msg void OnNMClickedTreeDetails(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLayerSetCurrent();
	afx_msg void OnLayerInsert();
	afx_msg void OnLayerShow();
	afx_msg void OnLayerHide();
	afx_msg void OnLayerRemove();
	afx_msg void OnProperties();
	afx_msg void OnLayerMoveDown();
	afx_msg void OnLayerMoveUp();
	afx_msg void OnLayerMoveBottom();
	afx_msg void OnLayerMoveTop();
	DECLARE_MESSAGE_MAP()
};

