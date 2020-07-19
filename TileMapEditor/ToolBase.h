#pragma once

class Map;
class MapLayer;

class ToolBase
{
public:
	ToolBase();
	virtual ~ToolBase();

	virtual void Draw(CDC* pDC);

	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	virtual void OnTurnOn();
	virtual void OnTurnOff();

	virtual void OnNewMap(Map* pMap){}
	virtual void OnMapClose(Map* pMap){}
	virtual void OnCurrentMapLayer(MapLayer* pLayer){}

protected:

	bool				_bInRegion;		///是否在有效区域内
	bool				_bDrawCursor;	///是否绘制鼠标所在的网格

	CRect				_rcTilePixel;	///当前网格像素矩形
	CPoint				_ptGrid;		///当前网格坐标

	COLORREF			_refCursor;
};
