#pragma once

#include "PanelToolBar.h"

class MapLayer;
struct STile;

//µÿ≤„√Ê∞Â
class LayerView : public CDockablePane
{
public:
	LayerView();
	virtual ~LayerView();

	void	SetCurrentLayer(MapLayer* pLayer);

	void	AddTileInfo(STile* pTile);
	bool	UpdateTileInfo(STile* pTile);
	bool	RemoveTileInfo(STile* pTile);
	void	SelectTile(STile* pTile);

protected:
	CClassToolBar	m_wndToolBar;
	CListCtrl		_listObjects;

	MapLayer*		_pLayer;

// ÷ÿ–¥
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);


protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnClassAddMemberFunction();
	afx_msg void OnInvertSelect();
	afx_msg void OnSelectSimilar();
	afx_msg void OnDeleteSelection();
	afx_msg void OnLayerExport();

	afx_msg void OnLvnColumnclickList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMclickList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemStateChanged(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnCustomdrawList(NMHDR*, LRESULT*);

	DECLARE_MESSAGE_MAP()
};

