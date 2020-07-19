#include "stdafx.h"
#include "GridTypes.h"
#include "ResourceManager.h"

using namespace Cactus;
using namespace PropertySys;

GridObject::GridObject()
: _iWidthInTiles(24)
, _iHeightInTiles(24)
, _iUnitTileWidth(160)
, _iUnitTileHeight(80)
, _eGridType(eDiamond)
{
}


GridObject::~GridObject()
{
}

void GridObject::RegisterReflection()
{
	BaseProperty* pProp = 0;

	M_ReflectionInit(0);

	pProp = M_RegisterPropertySimple(Cactus::String, Name, GridObject, Grid, "对象名字.", BaseProperty::eReadOnly, _strName);

	pProp = M_RegisterPropertySimple(int, WidthInTiles, GridObject, Grid, "宽度方向网格数量.", BaseProperty::eReadOnly, _iWidthInTiles);
	pProp = M_RegisterPropertySimple(int, HeightInTiles, GridObject, Grid, "高度方向网格数量.", BaseProperty::eReadOnly, _iHeightInTiles);
	pProp = M_RegisterPropertySimple(int, UnitTileWidth, GridObject, Grid, "网格单位的像素宽度.", BaseProperty::eReadOnly, _iUnitTileWidth);
	pProp = M_RegisterPropertySimple(int, UnitTileHeight, GridObject, Grid, "网格单位的像素高度.", BaseProperty::eReadOnly, _iUnitTileHeight);
	
	pProp = M_RegisterPropertySimple(int, GridType, GridObject, Grid, "网格类型.", BaseProperty::eDefault, _eGridType);
	pProp->SetValueSpecify(eValueList, "0;1");
}

void GridObject::OnPropertyChanged(const std::string& propName)
{
}

/*
	int xOffset = (GetPixelWidth() - _iUnitTileWidth)/2;

	ptGrid.x	= ptPixel.y/_iUnitTileHeight + (ptPixel.x - xOffset) /_iUnitTileWidth;
	ptGrid.y	= ptPixel.y/_iUnitTileHeight - (ptPixel.x - xOffset) /_iUnitTileWidth;

	return (ptGrid.x >= 0 && ptGrid.y >= 0
		&& ptGrid.x < _iWidthInTiles && ptGrid.y < _iHeightInTiles
		);
*/

/*======================================
k = H/W;

x[0, 0.5W]
y >= -kx + 0.5H;
y <= kx + 0.5H;

x[0.5W, W]
y >= kx - 0.5H;
y <= -kx + 1.5H;

//a, b为grid坐标；x, y为像素坐标；
a - b = 2x/W;
a + b = 2y/H;
======================================*/

bool GridObject::GetGridCoord(const CPoint& ptPixel, CPoint& ptGrid)
{
	if (ptPixel.x > GetPixelWidth() || ptPixel.y > GetPixelHeight() || ptPixel.x < 0 || ptPixel.y < 0)
	{
		return false;
	}

	if (_eGridType == 0)
	{
		ptGrid.x	= ptPixel.x / _iUnitTileWidth;
		ptGrid.y	= ptPixel.y / _iUnitTileHeight;

		return true;
	}

	bool bInRegion = false;

	int iMapW = GetPixelWidth();
	int iMapH = GetPixelHeight();

	int y = 0;
	float k = 1.0f * iMapH / iMapW;
	if (ptPixel.x <= iMapW/2)
	{
		if ( (ptPixel.y >= -k * ptPixel.x + iMapH/2) && (ptPixel.y <= k * ptPixel.x + iMapH/2) )
			bInRegion = true;
	}
	else
	{
		if ( (ptPixel.y >= k * ptPixel.x - iMapH/2) && (ptPixel.y <= -k * ptPixel.x + 1.5 * iMapH) )
			bInRegion = true;
	}

	if (bInRegion)
	{
		//计算y
		//y = kx + b;	b[-0.5H, 0.5H]
		float b = 1.0f * -iMapH/2;
		ptGrid.y = -1;
		while(ptPixel.y > k * ptPixel.x + b)
		{
			b += _iUnitTileHeight;
			ptGrid.y++;
		}

		//计算x
		//y = -kx + b;	b[0.5H, 1.5H]
		b = 1.0f * iMapH/2;
		ptGrid.x = -1;
		while(ptPixel.y > -k * ptPixel.x + b)
		{
			b += _iUnitTileHeight;
			ptGrid.x++;
		}
	}

	return bInRegion;
}

CRect GridObject::GetPixelCoordRect(const CPoint& ptGrid)
{
	if (_eGridType == eRectangle)
	{
		return CRect(CPoint(ptGrid.x * _iUnitTileWidth, ptGrid.y * _iUnitTileHeight), CSize(_iUnitTileWidth, _iUnitTileHeight));
	}
	else
	{
		int xOffset = (GetPixelWidth() - _iUnitTileWidth)/2;

		int xLeft	= xOffset + (ptGrid.x - ptGrid.y) * _iUnitTileWidth / 2;
		int yTop	= (ptGrid.x + ptGrid.y) * _iUnitTileHeight / 2;

		return CRect(CPoint(xLeft, yTop), CSize(_iUnitTileWidth, _iUnitTileHeight));
	}
}

void GridObject::DrawGrid(CDC* pDC, CPoint ptGrid, COLORREF ref, int deflate, int lineWidth/* = 2*/)
{
	CRect rcPixel = GetPixelCoordRect(ptGrid);

	CPen pen(PS_SOLID, lineWidth, ref);

	pDC->SelectStockObject(NULL_BRUSH);

	CPen* pOldPen = pDC->SelectObject(&pen);

	CRect rc = rcPixel;

	rc.DeflateRect(deflate, deflate, deflate, deflate);

	if (_eGridType == 0)
	{
		rc.DeflateRect(1, 1, 0, 0);

		pDC->Rectangle(rc);
	}
	else
	{
		rc.DeflateRect(1, 1, 1, 1);

		CPoint pts[4];
		pts[0] = CPoint(rc.left, rc.top + rc.Height()/2);
		pts[1] = CPoint(rc.left + rc.Width()/2, rc.top);
		pts[2] = CPoint(rc.right, rc.top + rc.Height()/2);
		pts[3] = CPoint(rc.left + rc.Width()/2, rc.bottom);

		pDC->Polygon(pts, 4);
	}

	pDC->SelectObject(pOldPen);
}

void GridObject::DrawGridText(CDC* pDC, CPoint ptGrid, COLORREF ref, const Cactus::String& str)
{
	CRect rcPixel = GetPixelCoordRect(ptGrid);

	COLORREF refOld = pDC->SetTextColor(ref);
	pDC->SetBkMode(TRANSPARENT);

	pDC->DrawText(str.c_str(), rcPixel, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	pDC->SetTextColor(refOld);
}

void GridObject::FillGrid( CDC* pDC, CPoint ptGrid, COLORREF ref )
{
	CRect rcPixel = GetPixelCoordRect(ptGrid);

	CBrush* pBrush;

	if (ResourceManager::s_Brushes.find(ref) == ResourceManager::s_Brushes.end())
	{
		pBrush = new CBrush;

		//pBrush->CreateHatchBrush(HS_DIAGCROSS, ref);
		pBrush->CreateSolidBrush(ref);

		ResourceManager::s_Brushes[ref] = pBrush;
	}
	else
	{
		pBrush = ResourceManager::s_Brushes[ref];
	}

	CBrush* pOldBrush = pDC->SelectObject(pBrush);

	CRect rc = rcPixel;
	rc.DeflateRect(1, 1, 1, 1);

	if (_eGridType == 0)
	{
		rc.DeflateRect(1, 1, 0, 0);

		pDC->Rectangle(rc);
	}
	else
	{
		rc.DeflateRect(1, 1, 1, 1);

		CPoint pts[4];
		pts[0] = CPoint(rc.left, rc.top + rc.Height()/2);
		pts[1] = CPoint(rc.left + rc.Width()/2, rc.top);
		pts[2] = CPoint(rc.right, rc.top + rc.Height()/2);
		pts[3] = CPoint(rc.left + rc.Width()/2, rc.bottom);

		pDC->Polygon(pts, 4);
	}

	pDC->SelectObject(pOldBrush);
}
