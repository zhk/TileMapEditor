#pragma once

#include "GridTypes.h"

class MapLayer;
class MapBackground;

//��ͼ����
class Map : public PropertySys::SupportRTTI<Map, GridObject>
{
	friend class Map_xmlHandler;
	friend class CTileMapEditorView;
	friend class CTileMapEditorDoc;
	friend class MapView;

public:
	Map();
	~Map();

	/**��ͼ�Ļ��ư��յز������У�һ���������ٻ���һ�㡣
	*	ֻ�����뵱ǰ�ӿ��ཻ������
	*	ͬһ�ز��л��Ƶ��������������������
	*/
	void						Draw(CDC* pDC, const CRect& rcView);
	void						Draw(CDC* pDC);		//����ȫͼ
	void						ExportFullMap(CDC* pDC);
	bool						Load(const Cactus::String& strPathName, bool bOnlyLoadData);
	void						Save(const Cactus::String& strPathName);
	void						CheckResourceUsage(StringResRefInfoMapType& mapGroupResRefInfo, StringResRefInfoMapType& groupSubResRefCountMap);
	void						Reset();
	bool						IsModified() const;
	void						SetModified(bool b = true);

	static void					RegisterReflection();
	virtual void				OnPropertyChanged(const std::string& propName);

	virtual Cactus::String		GetObjectName(){ return _strName; }

	//���õ�ǰLayer�����Layer���ɼ�������Ϊ�ɼ���������ˢ��Ribbon��View
	void						SetCurLayer(MapLayer* pLayer);
	MapLayer*					GetCurLayer(){ return _pCurLayer; }

	//�����Ψһ��Layer����������Ϊ��ǰ��
	bool						AddLayer(MapLayer* pLayer);

	//ɾ����
	void						RemoveLayer(MapLayer* pLayer);

	//��MapLayer����һ��
	bool						MoveLayerDown(MapLayer* pLayer);
	//��MapLayer����һ��
	bool						MoveLayerUp(MapLayer* pLayer);
	//��MapLayer���͵���Ͳ�
	bool						MoveLayerBottom(MapLayer* pLayer);
	//��MapLayer��������߲�
	bool						MoveLayerTop(MapLayer* pLayer);

	//bMakeCurrentֻ����bShowΪtrueʱ������
	void						ShowLayer(MapLayer* pLayer, bool bShow, bool bMakeCurrent = false);

	int							GetPixelWidth(){ return _iWidthInTiles * _iUnitTileWidth; }
	int							GetPixelHeight(){ return _iHeightInTiles * _iUnitTileHeight; }

	/**����������Ϣ
	*/
	void						CalculateRegionInfo();


	/**��ȡ��ǰ�������ڵ�����
	*	@param ptGrid	��ǰ����������
	*	@param rcRegion	�������������Χ
	*	@return -1 Ϊ��Ч��
	*/
	int							GetRegionID(const CPoint& ptGrid);

	/**��ȡ���ΪID����������Χ
	*	@param ID			�����ID
	*	@param gridRegion	�������������Χ
	*	@param gridRegion	������������ط�Χ
	*/
	bool						GetRegionGridRect(int ID, CRect& gridRect, CRect& pixelRect);

	/**��ȡ��ǰ���ط�Χ�ཻ�Ͱ�������������
	*	@param rcView	��ǰ�����ط�Χ
	*/
	IntVectorType				GetIntersectRegions(const CRect& rcView);

	/**��ȡ��ǰ�����������������
	*	@param ID	��ǰ����
	*/
	IntVectorType				GetAdjacentRegions(int ID);

	virtual bool				IsDrawGOCenter(){ return _bDrawGOCenter; }
	virtual bool				IsDrawObstacle(){ return _bDrawObstacle; }

protected:

	void				_DrawGrid(CDC* pDC);
	void				_DrawRegionGrid(CDC* pDC);

	bool				_bModified;

	Cactus::String		_strFootnotes;			///��ͼ��ע
	Cactus::String		_strCurLayerName;		///��ǰ�ز�����
	int					_iVersion;				///��ͼ�汾

	int					_iRegionWidth;			///Region��ȣ�һ��Region���Ӧ��Ϊ��Ϸ�����ߵ�1/2
	int					_iRegionHeight;			///Region�߶ȣ�һ��Region���Ӧ��Ϊ��Ϸ�����ߵ�1/2
	int					_iWRegionCount;			///��ȷ���Region����
	int					_iHRegionCount;			///�߶ȷ���Region����

	COLORREF			_colBKColor;			///������ɫ
	bool				_bDrawGrid;				///�Ƿ��������
	COLORREF			_colGridColor;			///������ɫ
	bool				_bDrawRegionGrid;		///�Ƿ������������
	COLORREF			_colRegionGridColor;	///����������ɫ

	bool				_bDrawGOCenter;			///�Ƿ����Ϸ����������Ϣ
	bool				_bDrawObstacle;			///�Ƿ�����赲��Ϣ

	typedef Cactus::list<MapLayer*>::type		MapLayerListType;
	MapLayerListType	_layers;

	MapLayer*			_pCurLayer;				///��ǰ�ز�
	bool				_bOnlyLoadData;

	
	struct SRegionInfo 
	{
		int		_regionID;	///Region�ı��
		CRect	_rcGrid;	///Region��Grid��Χ
		CRect	_rcPixel;	///Region��Pixel��Χ
	};
	typedef Cactus::map<int, SRegionInfo>::type		RegionMapType;
	RegionMapType		_regions;
};
