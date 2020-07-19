#include "StdAfx.h"
#include "Map.h"
#include "MapLayerTiles.h"
#include "MapLayerBackground.h"
#include "MapLayerLogic.h"

#include "MainFrm.h"
#include "TileMapEditorView.h"

using namespace Cactus;
using namespace PropertySys;

//---------------------------------------------------------------------------------------------------------

class Map_xmlHandler : public Cactus::XMLHandler
{
public:
	Map_xmlHandler(Map& map) : _map(map)
	{
		_iCurRegionGroupTile = -1;
	}
	virtual ~Map_xmlHandler(){}

	virtual void elementStart(const Cactus::String& element, const Cactus::XMLAttributes& attributes)
	{
		if ( currentElementMatch("tilemap/") )
		{
			//<tilemap version="1" name="map_xx" width="200" height="200" wUnitTile="64" hUnitTile="64" footnotes="�����뱸ע��" 
			//backgroundcolor="0" drawgrid="true" gridcolor="32768" curlayer="layer_xx" regionW="10" regionH="10">
			_map._iVersion				= attributes.getValueAsInteger("version");
			_map._strName				= attributes.getValueAsString("name");
			_map._iWidthInTiles			= attributes.getValueAsInteger("width");
			_map._iHeightInTiles		= attributes.getValueAsInteger("height");
			_map._iUnitTileWidth		= attributes.getValueAsInteger("wUnitTile");
			_map._iUnitTileHeight		= attributes.getValueAsInteger("hUnitTile");
			_map._strFootnotes			= attributes.getValueAsString("footnotes");
			_map._colBKColor			= attributes.getValueAsInteger("backgroundcolor");
			_map._strCurLayerName		= attributes.getValueAsString("curlayer");
			_map._bDrawGrid				= attributes.getValueAsBool("drawgrid");
			_map._colGridColor			= attributes.getValueAsInteger("gridcolor");
			_map._eGridType				= EGridType( attributes.getValueAsInteger("type") );
			_map._iRegionWidth			= attributes.getValueAsInteger("regionW");
			_map._iRegionHeight			= attributes.getValueAsInteger("regionH");
			_map._bDrawGOCenter			= attributes.getValueAsBool("DrawGOCenter");
			_map._bDrawObstacle			= attributes.getValueAsBool("DrawObstacle");
			_map._bDrawRegionGrid		= attributes.getValueAsBool("DrawRegionGrid");
			_map._colRegionGridColor	= attributes.getValueAsInteger("regionGridcolor");
			
		}
		//else if ( currentElementMatch("tilemap/background/layers/") )
		//{
		//}
		else if ( currentElementMatch("tilemap/layers/layer/") )
		{
			MapLayer::EMapLayerType eType = MapLayer::EMapLayerType(attributes.getValueAsInteger("type"));
			
			if (eType == MapLayer::eMapLayerMonster)
			{
				_pCurLayer = new MapLayerMonster;
			}
			else if (eType == MapLayer::eMapLayerEvent)
			{
				_pCurLayer = new MapLayerEvent;
			}
			else if (eType == MapLayer::eMapLayerTerrain)
			{
				_pCurLayer = new MapLayerTerrain;
			}
			else
			{
				_pCurLayer = new MapLayerTiles;
			}

			_pCurLayer->Init(attributes.getValueAsString("name"), &_map);
			_pCurLayer->_bEnable		= attributes.getValueAsBool("enable");
			_pCurLayer->_bVisible		= attributes.getValueAsBool("visible");

			_map._layers.push_back(_pCurLayer);
		}
		else if ( currentElementMatch("tilemap/layers/background/") )
		{
			_pCurLayer = new MapLayerBackground;
			_pCurLayer->Init(attributes.getValueAsString("name"), &_map);
			_pCurLayer->_bEnable		= attributes.getValueAsBool("enable");
			_pCurLayer->_bVisible		= attributes.getValueAsBool("visible");

			_map._layers.push_back(_pCurLayer);
		}
		else if ( currentElementMatch("tilemap/layers/layer/tilegroup/") 
			||
			currentElementMatch("tilemap/layers/background/tilegroup/")
			)
		{
			_iCurRegionGroupTile	= attributes.getValueAsInteger("region");
		}
		else if ( currentElementMatch("tilemap/layers/layer/tilegroup/tile/")
			||
			currentElementMatch("tilemap/layers/background/tilegroup/tile/")
			)
		{
			STile* pTile = new STile;
			pTile->_ptGrid.x		= attributes.getValueAsInteger("posx");
			pTile->_ptGrid.y		= attributes.getValueAsInteger("posy");
			pTile->_strResGroup		= attributes.getValueAsString("resGroup");
			pTile->_strResItemID	= attributes.getValueAsString("resItemID");
			pTile->_regionID		= _iCurRegionGroupTile;
			_tiles.push_back(pTile);
		}
	}

	virtual void elementEnd(const Cactus::String& element)
	{
		if ( "tilemap" == element)
		{
			_map.SetModified(false);
		}
		else if ( currentElementMatch("tilemap/layers/layer/")
			||
			currentElementMatch("tilemap/layers/background/")
			)
		{
			if (!_map._bOnlyLoadData && _map._strCurLayerName == _pCurLayer->_strName)
			{
				_pCurLayer->_bVisible = true;
				_map.SetCurLayer(_pCurLayer);
			}

			_pCurLayer = 0;
		}
		else if ( currentElementMatch("tilemap/layers/layer/tilegroup/")
			||
			currentElementMatch("tilemap/layers/background/tilegroup/")
			)
		{
			if (_iCurRegionGroupTile != -1)
			{
				_pCurLayer->AddRegionTiles(_iCurRegionGroupTile, _tiles);
				_tiles.clear();
				_iCurRegionGroupTile = -1;
			}
		}
	}

	virtual void text(const Cactus::String& element, const Cactus::String& content){}

protected:

	Map&			_map;

	MapLayer*		_pCurLayer;
	int				_iCurRegionGroupTile;
	TileVectorType	_tiles;
};

//---------------------------------------------------------------------------------------------------------

Map::Map()
: _iVersion(1)
, _iRegionWidth(8)
, _iRegionHeight(8)
, _colBKColor(0)
, _pCurLayer(0)
, _strCurLayerName("")
, _bDrawGrid(true)
, _colGridColor(RGB(0, 128, 0))
, _bDrawRegionGrid(true)
, _colRegionGridColor(RGB(192, 192, 0))
, _bDrawGOCenter(true)
, _bDrawObstacle(true)
, _bModified(false)
{
}

Map::~Map()
{
	Reset();
}

void Map::RegisterReflection()
{
	BaseProperty* pProp = 0;

	M_ReflectionInit(0);

	pProp = M_RegisterPropertySimple(int, Version, Map, Map, "��ͼ�汾.", BaseProperty::eReadOnly, _iVersion);
	pProp = M_RegisterPropertySimple(Cactus::String, Footnotes, Map, Map, "��ͼ��ע.", BaseProperty::eDefault, _strFootnotes);

	pProp = M_RegisterPropertySimple(int, BackgroundColor, Map, Map, "������ɫ.", BaseProperty::eDefault, _colBKColor);
	pProp->SetValueSpecify(eValueColor, "");

	pProp = M_RegisterPropertySimple(bool, DrawGrid, Map, Map, "�Ƿ��������.", BaseProperty::eDefault, _bDrawGrid);
	pProp = M_RegisterPropertySimple(int, GridColor, Map, Map, "������ɫ.", BaseProperty::eDefault, _colGridColor);
	pProp->SetValueSpecify(eValueColor, "");

	pProp = M_RegisterPropertySimple(int, RegionWidth, Map, Map, "������.", BaseProperty::eReadOnly, _iRegionWidth);
	pProp = M_RegisterPropertySimple(int, RegionHeight, Map, Map, "����߶�.", BaseProperty::eReadOnly, _iRegionHeight);
	pProp = M_RegisterPropertySimple(bool, DrawRegionGrid, Map, Map, "�Ƿ������������.", BaseProperty::eDefault, _bDrawRegionGrid);
	pProp = M_RegisterPropertySimple(int, RegionGridColor, Map, Map, "����������ɫ.", BaseProperty::eDefault, _colRegionGridColor);
	pProp->SetValueSpecify(eValueColor, "");

	pProp = M_RegisterPropertySimple(bool, DrawGameObjectCenter, Map, Map, "�Ƿ������Ϸ������������.", BaseProperty::eDefault, _bDrawGOCenter);
	pProp = M_RegisterPropertySimple(bool, DrawObstacle, Map, Map, "�Ƿ�����ϰ�������.", BaseProperty::eDefault, _bDrawObstacle);
}

void Map::OnPropertyChanged(const std::string& propName)
{
	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd() );
	CView* pView = pMainFrame->GetActiveView(); 
	pView->Invalidate();

	SetModified();
}


bool Map::IsModified() const
{
	return _bModified;
}

void Map::SetModified(bool b/* = true*/)
{
	_bModified = b;
}


bool Map::Load(const Cactus::String& strPathName, bool bOnlyLoadData)
{
	_bOnlyLoadData = bOnlyLoadData;

	Map_xmlHandler handler(*this);

	try
	{
		XMLParser xmlParser(handler, strPathName, "");
	}
	catch (std::exception e)
	{
		Log_Error( "Map::Load, Load failed! File : " << strPathName );
		return false;
	}

	if (!bOnlyLoadData)
	{
		int iCount = 0;
		for(MapLayerListType::iterator it = _layers.begin(); it != _layers.end(); ++it)
		{
			iCount += (*it)->CheckResDepends();
		}

		if (iCount != 0)
		{
			Log_Error("��ͼ���������� ������Դ �� ��Ϸ������Դ �����ڣ�");
			Log_Error("	���� ArtĿ¼�Լ�ResourceArt.xml��ResourceGameObject.xml�ļ������������ʧ��");
			Log_Error("	������鿴��־�ļ���TileMapEditor.log");
			//AfxMessageBox("�� ������Դ �� ��Ϸ������Դ �����ڣ�\n�����ȼ����ԴĿ¼ �� �����ļ���������������ʧ��\n������鿴��־�ļ���", MB_OK | MB_ICONINFORMATION);
		}

		CalculateRegionInfo();
	}

	return true;
}

void Map::Save(const Cactus::String& strPathName)
{
	DataOutStreamOS os;
	if( !os.Open(strPathName) )
	{
		Log_Error("Map::Save, can not write file : " << strPathName);
		return;
	}

	String str = "<?xml version=\"1.0\" encoding=\"gb2312\"?>\r\n";
	//os << str;	//��������д������
	os.Write(&str[0], str.size());

	XMLOutStream xmlOut(&os);

	xmlOut.NodeBegin("tilemap");
		xmlOut.AddAttribute("version", _iVersion);
		xmlOut.AddAttribute("name", _strName);
		xmlOut.AddAttribute("type", _eGridType);
		xmlOut.AddAttribute("width", _iWidthInTiles);
		xmlOut.AddAttribute("height", _iHeightInTiles);
		xmlOut.AddAttribute("wUnitTile", _iUnitTileWidth);
		xmlOut.AddAttribute("hUnitTile", _iUnitTileHeight);
		xmlOut.AddAttribute("footnotes", _strFootnotes);
		xmlOut.AddAttribute("backgroundcolor", _colBKColor);
		xmlOut.AddAttribute("drawgrid",	 _bDrawGrid);
		xmlOut.AddAttribute("gridcolor", _colGridColor);
		xmlOut.AddAttribute("regionW", _iRegionWidth);
		xmlOut.AddAttribute("regionH", _iRegionHeight);
		xmlOut.AddAttribute("DrawGOCenter", _bDrawGOCenter);
		xmlOut.AddAttribute("DrawObstacle", _bDrawObstacle);
		xmlOut.AddAttribute("DrawRegionGrid", _bDrawRegionGrid);
		xmlOut.AddAttribute("regionGridcolor", _colRegionGridColor);

		if (_pCurLayer)
			xmlOut.AddAttribute("curlayer", _pCurLayer->GetObjectName());
		else
			xmlOut.AddAttribute("curlayer", "");


		xmlOut.NodeBegin("layers");
		for(MapLayerListType::iterator it = _layers.begin(); it != _layers.end(); ++it)
		{
			MapLayer* pLayer = (*it);

			pLayer->Save(xmlOut);
		}
		xmlOut.NodeEnd("layers");

		xmlOut.NodeBegin("brush");
		xmlOut.NodeEnd("brush");

	xmlOut.NodeEnd("tilemap");


	xmlOut.Flush();
}


void Map::Reset()
{
	for (MapLayerListType::iterator it = _layers.begin(); it != _layers.end(); ++it)
	{
		delete *it;
	}
}

void Map::Draw(CDC* pDC, const CRect& rcView)
{
	pDC->FillSolidRect(0, 0, _iWidthInTiles * _iUnitTileWidth, _iHeightInTiles * _iUnitTileHeight, _colBKColor);

	//��������
	_DrawGrid(pDC);

	//����MapLayer
	CRect rcDirty = rcView;
	rcDirty.InflateRect(_iRegionWidth * _iUnitTileWidth, _iRegionHeight * _iUnitTileHeight);

	IntVectorType Regions = GetIntersectRegions(rcDirty);
	if (Regions.size() == 0)
		return;

	for(MapLayerListType::iterator it = _layers.begin(); it != _layers.end(); ++it)
	{
		if (*it != _pCurLayer)
			(*it)->Draw(pDC, Regions);
	}

	if (_pCurLayer)
		_pCurLayer->Draw(pDC, Regions);

	//������������
	_DrawRegionGrid(pDC);

	//��������
	_DrawGrid(pDC);
}

void Map::Draw(CDC* pDC)
{
	pDC->FillSolidRect(0, 0, _iWidthInTiles * _iUnitTileWidth, _iHeightInTiles * _iUnitTileHeight, _colBKColor);

	//��������
	_DrawGrid(pDC);

	//����MapLayer
	IntVectorType IDs;
	for(RegionMapType::iterator it = _regions.begin(); it != _regions.end(); ++it)
	{
		IDs.push_back(it->first);
	}

	for(MapLayerListType::iterator it = _layers.begin(); it != _layers.end(); ++it)
	{
		if (*it != _pCurLayer)
			(*it)->Draw(pDC, IDs);
	}

	if (_pCurLayer)
		_pCurLayer->Draw(pDC, IDs);

	//������������
	_DrawRegionGrid(pDC);

	//��������
	_DrawGrid(pDC);
}


void Map::ExportFullMap(CDC* pDC)
{
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, _iWidthInTiles * _iUnitTileWidth, _iHeightInTiles * _iUnitTileHeight);
	CBitmap* pOld = memDC.SelectObject(&bmp);

	Draw(&memDC);

	memDC.SelectObject(pOld);

	CxImage image;
	image.CreateFromHBITMAP((HBITMAP)bmp.GetSafeHandle());

	CTime ct = CTime::GetCurrentTime(); 
	CString str = ct.Format("%Y-%m-%d_%H-%M-%S");
	str += ".jpg";
	image.Save((LPCTSTR)str, CXIMAGE_FORMAT_JPG);

	::ShellExecute(0, "open", str, 0, 0, SW_SHOW);
}


void Map::_DrawGrid(CDC* pDC)
{
	if (_bDrawGrid)
	{
		CPen pen(PS_SOLID, 1, _colGridColor);
		CPen* pOldPen = pDC->SelectObject(&pen);

		int iMapW = GetPixelWidth();
		int iMapH = GetPixelHeight();

		if (GetGridType() == eRectangle)
		{
			//������
			for (int i = 0; i <= _iHeightInTiles; i++)
			{
				pDC->MoveTo(0, i * _iUnitTileHeight);
				pDC->LineTo(iMapW, i * _iUnitTileHeight);
			}

			//������
			for (int i = 0; i <= _iWidthInTiles; i++)
			{
				pDC->MoveTo(i * _iUnitTileWidth, 0);
				pDC->LineTo(i * _iUnitTileWidth, iMapH);
			}
		}
		else
		{
			//��б����
			for (int i = 0; i <= _iHeightInTiles; i++)
			{
				pDC->MoveTo(iMapW/2 - i * _iUnitTileWidth / 2
					, i * _iUnitTileHeight / 2);

				pDC->LineTo(iMapW - i * _iUnitTileWidth / 2
					, iMapH/2 + i * _iUnitTileHeight / 2);
			}

			//��б����
			for (int i = 0; i <= _iWidthInTiles; i++)
			{
				pDC->MoveTo(i * _iUnitTileWidth / 2
					, iMapH/2 + i * _iUnitTileHeight / 2);

				pDC->LineTo(iMapW/2 + i * _iUnitTileWidth / 2
					, i * _iUnitTileHeight / 2);
			}
		}

		pDC->SelectObject(pOldPen);
	}
}

void Map::_DrawRegionGrid(CDC* pDC)
{
	if(_bDrawRegionGrid)
	{
		CPen pen(PS_SOLID, 1, _colRegionGridColor);
		CPen* pOldPen = pDC->SelectObject(&pen);
		pDC->SelectStockObject(NULL_BRUSH);

		for(RegionMapType::iterator it = _regions.begin(); it != _regions.end(); ++it)
		{
			CRect rc = it->second._rcPixel;
			rc.DeflateRect(1, 1, 1, 1);

			CString strTmp;
			strTmp.Format(_T("%d"), it->first);

			pDC->SetTextColor(_colRegionGridColor);
			pDC->SetBkMode(TRANSPARENT);

			if (_eGridType == eRectangle)
			{
				pDC->DrawText(strTmp, rc, DT_LEFT | DT_TOP);
				pDC->Rectangle(rc);
			}
			else
			{
				rc.DeflateRect(1, 1, 1, 1);

				CRect rcText = rc;
				rcText.left	= rc.left + rc.Width()/2 - 5;
				rcText.top	+= 10;

				pDC->DrawText(strTmp, rcText, DT_LEFT | DT_TOP);

				CPoint pts[4];
				pts[0] = CPoint(rc.left, rc.top + rc.Height()/2);
				pts[1] = CPoint(rc.left + rc.Width()/2, rc.top);
				pts[2] = CPoint(rc.right, rc.top + rc.Height()/2);
				pts[3] = CPoint(rc.left + rc.Width()/2, rc.bottom);

				pDC->Polygon(pts, 4);
			}
		}

		pDC->SelectObject(pOldPen);
	}
}

void Map::SetCurLayer(MapLayer* pLayer)
{
	MapLayer* pOldLayer = _pCurLayer;

	_pCurLayer = pLayer;

	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd() );
	if (_pCurLayer)
	{
		_pCurLayer->SetVisible(true);
		pMainFrame->SetCurLayer(_pCurLayer);
	}
	else
	{
		pMainFrame->SetCurLayer(0);
	}

	if (pOldLayer || _pCurLayer)
	{
		CView* pView = pMainFrame->GetActiveView(); 
		if (pView)
			pView->Invalidate();
	}

	SetModified();
}

bool Map::AddLayer(MapLayer* pLayer)
{
	for(MapLayerListType::iterator it = _layers.begin(); it != _layers.end(); ++it)
	{
		if ( (*it)->_strName == pLayer->_strName )
			return false;
	}

	_layers.push_back(pLayer);

	if (_layers.size() == 1)
	{
		SetCurLayer(pLayer);
	}

	SetModified();

	return true;
}

void Map::RemoveLayer(MapLayer* pLayer)
{
	bool bIsCurrent = (_pCurLayer == pLayer);

	MapLayerListType::iterator it = find(_layers.begin(), _layers.end(), pLayer);
	if (it != _layers.end())
	{
		delete *it;
		_layers.erase(it);
	}
	else
	{
		return;
	}

	if (bIsCurrent)
	{
		SetCurLayer(0);
	}
	else
	{
		CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd() );
		CView* pView = pMainFrame->GetActiveView(); 
		pView->Invalidate();
	}

	SetModified();
}

void Map::ShowLayer(MapLayer* pLayer, bool bShow, bool bMakeCurrent)
{
	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd() );

	bool bIsCurrent = (_pCurLayer == pLayer);

	if (pLayer->IsVisible() == bShow)
	{
		return;
	}

	pLayer->SetVisible(bShow);

	if (bIsCurrent && !bShow)
	{
		SetCurLayer(0);
	}
	else if (bMakeCurrent || _layers.size() == 1)
	{
		_pCurLayer = pLayer;
		pMainFrame->SetCurLayer(_pCurLayer);
	}

	CView* pView = pMainFrame->GetActiveView(); 
	pView->Invalidate();
}

int Map::GetRegionID(const CPoint& ptGrid)
{
	for(RegionMapType::iterator it = _regions.begin(); it != _regions.end(); ++it)
	{
		if (it->second._rcGrid.PtInRect(ptGrid))
		{
			return it->first;
		}
	}

	return -1;
}

bool Map::GetRegionGridRect(int ID, CRect& gridRect, CRect& pixelRect)
{
	RegionMapType::iterator it = _regions.find(ID);
	if (it == _regions.end())
	{
		return false;
	}

	gridRect	= it->second._rcGrid;
	pixelRect	= it->second._rcPixel;

	return true;
}

IntVectorType Map::GetIntersectRegions(const CRect& rcView)
{
	IntVectorType IDs;

	for(RegionMapType::iterator it = _regions.begin(); it != _regions.end(); ++it)
	{
		CRect rcRegion = it->second._rcPixel;
		CRect rcTmp;
		if( rcTmp.IntersectRect(rcRegion, rcView) )
		{
			IDs.push_back(it->first);
		}
	}

	return IDs;
}

IntVectorType Map::GetAdjacentRegions(int ID)
{
	IntVectorType ids;

	int id = ID - _iWRegionCount - 1;
	if (id >= 0)
		ids.push_back(id);

	id = ID - _iWRegionCount;
	if (id >= 0)
		ids.push_back(id);

	id = ID - _iWRegionCount + 1;
	if (id >= 0)
		ids.push_back(id);



	id = ID - 1;
	if (id >= 0)
		ids.push_back(id);

	id = ID + 1;
	if ( id < int(_regions.size()) )
		ids.push_back(id);



	id = ID + _iWRegionCount - 1;
	if ( id < int(_regions.size()) )
		ids.push_back(id);

	id = ID + _iWRegionCount;
	if ( id < int(_regions.size()) )
		ids.push_back(id);

	id = ID + _iWRegionCount + 1;
	if ( id < int(_regions.size()) )
		ids.push_back(id);

	return ids;
}


void Map::CalculateRegionInfo()
{
	_regions.clear();

	int W = (_iWidthInTiles + _iRegionWidth - 1)/ _iRegionWidth;
	int H = (_iHeightInTiles + _iRegionHeight - 1)/ _iRegionHeight;

	_iWRegionCount	= W;
	_iWRegionCount	= H;

	if (_eGridType == eRectangle)
	{
		for (int j = 0; j < H; ++j)
		{
			for (int i = 0; i < W; ++i)
			{
				SRegionInfo info;
				info._regionID	= j * W + i;
				info._rcGrid	= CRect(CPoint(i * _iRegionWidth, j * _iRegionHeight)
					, CSize(_iRegionWidth, _iRegionHeight)
					);
				info._rcPixel	= CRect(CPoint(i * _iRegionWidth * _iUnitTileWidth, j * _iRegionHeight * _iUnitTileHeight)
					, CSize(_iRegionWidth * _iUnitTileWidth, _iRegionHeight * _iUnitTileHeight)
					);

				_regions[info._regionID] = info;
			}
		}
	}
	else
	{
		for (int j = 0; j < H; ++j)
		{
			for (int i = 0; i < W; ++i)
			{
				SRegionInfo info;
				info._regionID	= j * W + i;
				info._rcGrid	= CRect(CPoint(i * _iRegionWidth, j * _iRegionHeight)
					, CSize(_iRegionWidth, _iRegionHeight)
					);

				CRect rcLeft	= GetPixelCoordRect(CPoint(i * _iRegionWidth , (j + 1) * _iRegionHeight - 1 ));
				CRect rcTop		= GetPixelCoordRect(CPoint(i * _iRegionWidth , j * _iRegionHeight));
				CRect rcRight	= GetPixelCoordRect(CPoint((i + 1) * _iRegionWidth - 1, j * _iRegionHeight));
				CRect rcBottom	= GetPixelCoordRect(CPoint((i + 1) * _iRegionWidth , (j + 1) * _iRegionHeight));

				CPoint ptPixel;
				ptPixel.x = rcLeft.TopLeft().x;
				ptPixel.y = rcTop.TopLeft().y;

				CSize szPixel;
				szPixel.cx = rcRight.BottomRight().x - rcLeft.TopLeft().x;
				szPixel.cy = rcBottom.TopLeft().y - rcTop.TopLeft().y;

				info._rcPixel	= CRect(ptPixel, szPixel);

				_regions[info._regionID] = info;
			}
		}
	}
}

bool Map::MoveLayerDown(MapLayer* pLayer)
{
	//�޲㣬����ֻ��1��
	if (_layers.size() < 2)
		return false;

	//�Ѿ�����ײ�
	if ( pLayer == *(_layers.begin()) )
		return false;

	MapLayerListType::iterator itLayer = std::find(_layers.begin(), _layers.end(), pLayer);
	MapLayerListType::iterator itPrev = itLayer;
	itPrev--;

	_layers.erase(itLayer);
	_layers.insert(itPrev, pLayer);

	SetModified();

	return true;
}

bool Map::MoveLayerUp(MapLayer* pLayer)
{
	if (_layers.size() < 2)
		return false;

	if ( pLayer == *(_layers.rbegin()) )
		return false;

	MapLayerListType::iterator itLayer = std::find(_layers.begin(), _layers.end(), pLayer);
	MapLayerListType::iterator itNext = itLayer;
	itNext++;

	MapLayer* pNextLayer = *itNext;

	_layers.erase(itNext);
	_layers.insert(itLayer, pNextLayer);

	SetModified();

	return true;
}

bool Map::MoveLayerBottom(MapLayer* pLayer)
{
	if (_layers.size() < 2)
		return false;

	if ( pLayer == *(_layers.begin()) )
		return false;

	MapLayerListType::iterator itLayer = std::find(_layers.begin(), _layers.end(), pLayer);

	_layers.erase(itLayer);
	_layers.push_front(pLayer);

	SetModified();

	return true;
}

bool Map::MoveLayerTop(MapLayer* pLayer)
{
	if (_layers.size() < 2)
		return false;

	if ( pLayer == *(_layers.rbegin()) )
		return false;

	MapLayerListType::iterator itLayer = std::find(_layers.begin(), _layers.end(), pLayer);

	_layers.erase(itLayer);
	_layers.push_back(pLayer);

	SetModified();

	return true;
}

void Map::CheckResourceUsage( StringResRefInfoMapType& mapGroupResRefInfo, StringResRefInfoMapType& groupSubResRefCountMap )
{
	for(MapLayerListType::iterator it = _layers.begin(); it != _layers.end(); ++it)
	{
		(*it)->CheckResourceUsage(mapGroupResRefInfo, groupSubResRefCountMap);
	}

}