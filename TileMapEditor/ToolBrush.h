#pragma once
#include "toolbase.h"

class ToolBrush : public ToolBase
{
public:
	ToolBrush();
	virtual ~ToolBrush();

	virtual void Draw(CDC* pDC);

	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	virtual void OnTurnOn();
	virtual void OnTurnOff();

	virtual void OnNewMap(Map* pMap);

	void		SetResource(const Cactus::String& strRes, const Cactus::String& strID);


protected:
	Cactus::String		_strResGroup;
	Cactus::String		_strResID;

	CRect				_rcOldSelected;
};
