#pragma once
#include "toolbase.h"

class ToolRegion : public ToolBase
{
public:
	ToolRegion();
	virtual ~ToolRegion();

	virtual void Draw(CDC* pDC);

	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnTurnOn();
	virtual void OnTurnOff();


	void				SetTerrainType(const Cactus::String& str){ _strTerrainType = str; }
	Cactus::String		GetTerrainType(){ return _strTerrainType; }

protected:

	bool			_bHasRange;
	
	CPoint			_ptStartGrid;		///��ʼʱ��������
	CPoint			_ptEndGrid;			///��ǰ��������

	CRect			_rcDirty;

	Cactus::String	_strTerrainType;
};
