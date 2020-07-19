#pragma once

//µ¼º½Í¼Ãæ°å

class MapThumbnailView : public CDockablePane
{
public:
	MapThumbnailView();
	virtual ~MapThumbnailView();

	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnItemChanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	DECLARE_MESSAGE_MAP()

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	CRect	_rcThumbView;
	CPoint	_ptOld;
	bool	_bHold;
	float	_fRatio;
	bool	_bEnableHoriz;
	bool	_bEnableVert;
};

