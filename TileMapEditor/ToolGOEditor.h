#pragma once

enum EToolMode
{
	eToolModeSelect,
	eToolModeSetObstacle,
	eToolModeClearObstacle,
	eToolModeMax
};

//��Ϸ����༭���У�����ͼ�й��߶�����
class ToolGOEditor
{
public:
	ToolGOEditor();
	~ToolGOEditor();

	EToolMode	GetCurToolMode(){ return _eCurToolMode; }
	void		SetCurToolMode(EToolMode e);

	void		Draw(CDC* pDC);

	void		OnLButtonDown(UINT nFlags, CPoint point);
	void		OnLButtonUp(UINT nFlags, CPoint point);
	void		OnMouseMove(UINT nFlags, CPoint point);

	bool		ToolHitTest(CPoint ptPixel, CPoint& ptGrid, CRect& rcPixel);

protected:

	EToolMode	_eCurToolMode;

	bool		_bInRegion;
	bool		_bDrawCursor;

	CRect		_rcTilePixel;
	CPoint		_ptGrid;

	COLORREF	_refCursor;

	CPoint		_ptStart;
	bool		_bSelected;
};
