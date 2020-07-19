#pragma once

#include "GridTypes.h"

class MapLayer;
class MapBackground;

//地图对象
class Map : public PropertySys::SupportRTTI<Map, GridObject>
{
	friend class Map_xmlHandler;
	friend class CTileMapEditorView;
	friend class CTileMapEditorDoc;
	friend class MapView;

public:
	Map();
	~Map();

	/**地图的绘制按照地层来进行，一层绘制完成再绘制一层。
	*	只绘制与当前视口相交的区域。
	*	同一地层中绘制的所有区域进行总体排序。
	*/
	void						Draw(CDC* pDC, const CRect& rcView);
	void						Draw(CDC* pDC);		//绘制全图
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

	//设置当前Layer，如果Layer不可见，设置为可见，并负责刷新Ribbon和View
	void						SetCurLayer(MapLayer* pLayer);
	MapLayer*					GetCurLayer(){ return _pCurLayer; }

	//如果是唯一的Layer，将被设置为当前的
	bool						AddLayer(MapLayer* pLayer);

	//删除层
	void						RemoveLayer(MapLayer* pLayer);

	//将MapLayer降低一层
	bool						MoveLayerDown(MapLayer* pLayer);
	//将MapLayer提升一层
	bool						MoveLayerUp(MapLayer* pLayer);
	//将MapLayer降低到最低层
	bool						MoveLayerBottom(MapLayer* pLayer);
	//将MapLayer提升到最高层
	bool						MoveLayerTop(MapLayer* pLayer);

	//bMakeCurrent只有在bShow为true时才有用
	void						ShowLayer(MapLayer* pLayer, bool bShow, bool bMakeCurrent = false);

	int							GetPixelWidth(){ return _iWidthInTiles * _iUnitTileWidth; }
	int							GetPixelHeight(){ return _iHeightInTiles * _iUnitTileHeight; }

	/**计算区域信息
	*/
	void						CalculateRegionInfo();


	/**获取当前网格所在的区域
	*	@param ptGrid	当前的网格坐标
	*	@param rcRegion	区域包含的网格范围
	*	@return -1 为无效点
	*/
	int							GetRegionID(const CPoint& ptGrid);

	/**获取编号为ID的网格网格范围
	*	@param ID			区域的ID
	*	@param gridRegion	区域包含的网格范围
	*	@param gridRegion	区域包含的像素范围
	*/
	bool						GetRegionGridRect(int ID, CRect& gridRect, CRect& pixelRect);

	/**获取当前像素范围相交和包含的所有区域
	*	@param rcView	当前的像素范围
	*/
	IntVectorType				GetIntersectRegions(const CRect& rcView);

	/**获取当前区域的所有相邻区域
	*	@param ID	当前区域
	*/
	IntVectorType				GetAdjacentRegions(int ID);

	virtual bool				IsDrawGOCenter(){ return _bDrawGOCenter; }
	virtual bool				IsDrawObstacle(){ return _bDrawObstacle; }

protected:

	void				_DrawGrid(CDC* pDC);
	void				_DrawRegionGrid(CDC* pDC);

	bool				_bModified;

	Cactus::String		_strFootnotes;			///地图备注
	Cactus::String		_strCurLayerName;		///当前地层名字
	int					_iVersion;				///地图版本

	int					_iRegionWidth;			///Region宽度，一个Region宽高应该为游戏界面宽高的1/2
	int					_iRegionHeight;			///Region高度，一个Region宽高应该为游戏界面宽高的1/2
	int					_iWRegionCount;			///宽度方向Region数量
	int					_iHRegionCount;			///高度方向Region数量

	COLORREF			_colBKColor;			///背景颜色
	bool				_bDrawGrid;				///是否绘制网格
	COLORREF			_colGridColor;			///网格颜色
	bool				_bDrawRegionGrid;		///是否绘制区域网格
	COLORREF			_colRegionGridColor;	///区域网格颜色

	bool				_bDrawGOCenter;			///是否绘游戏对象中心信息
	bool				_bDrawObstacle;			///是否绘制阻挡信息

	typedef Cactus::list<MapLayer*>::type		MapLayerListType;
	MapLayerListType	_layers;

	MapLayer*			_pCurLayer;				///当前地层
	bool				_bOnlyLoadData;

	
	struct SRegionInfo 
	{
		int		_regionID;	///Region的编号
		CRect	_rcGrid;	///Region的Grid范围
		CRect	_rcPixel;	///Region的Pixel范围
	};
	typedef Cactus::map<int, SRegionInfo>::type		RegionMapType;
	RegionMapType		_regions;
};
