#pragma once

#include "MapBase.h"
#include "GridTypes.h"

//-------------------------------------------------------------------------------------------------------

struct STile 
{
	STile()
	{
		_bSelected	= false;
		_bValid		= true;
	}

	CPoint			_ptGrid;		///网格坐标
	int				_regionID;		///所在的RegionID
	Cactus::String	_strResGroup;	///使用的资源组
	Cactus::String	_strResItemID;	///使用的资源
	bool			_bSelected;
	bool			_bValid;
};

bool STile_Diamond_less(const STile* p1, const STile* p2);
bool STile_Triangle_less(const STile* p1, const STile* p2);

typedef Cactus::vector<STile*>::type		TileVectorType;

//-------------------------------------------------------------------------------------------------------
//地层对象
class MapLayer : public PropertySys::SupportRTTI<MapLayer, MapBaseObject>
{
	friend class Map;
	friend class Map_xmlHandler;

public:
	MapLayer();
	virtual ~MapLayer();

	enum EMapLayerType
	{
		eMapLayerTiles,
		eMapLayerBackground,
		eMapLayerMonster,
		eMapLayerEvent,
		eMapLayerTerrain,
	};


	enum ETileOp
	{
		eTileOpAdd,
		eTileOpRemove,
		eTileOpUpdate
	};

	void					Init(const Cactus::String& strName, Map* pParent);

	Map*					GetParentMap(){ return _pParentMap; }

	static void				RegisterReflection();
	virtual void			OnPropertyChanged(const std::string& propName);

	bool					IsVisible(){ return _bVisible; }
	void					SetVisible(bool b);

	virtual Cactus::String	GetObjectName(){ return _strName; }
	virtual bool			IsLayer(){ return true; }

	EMapLayerType			GetMapLayerType(){ return _eLayerType; }
	void					SetMapLayerType(EMapLayerType e){ _eLayerType = e; }

	/**获取当前像素点的网格坐标和网格像素范围
	*	@param	ptPixel	当前像素坐标
	*	@param	ptGrid	返回的网格坐标
	*	@param	rcPixel	返回的网格的像素范围
	*/
	virtual bool			ToolHitTest(CPoint ptPixel, CPoint& ptGrid, CRect& rcPixel);


	/**更新地层面板中STile列表
	*/
	virtual void			UpdateTileInfoInPanelMapLayer(STile* pTile, ETileOp e = eTileOpAdd);


	/**更新指定STile的视图绘制
	*/
	virtual void			UpdateTileVisual(STile* pTile, bool bEnsureVisible = false);

	/**检查依赖资源有效性
	*	@return		返回无效的Tile数量
	*/
	virtual int				CheckResDepends(){ return 0; }

	/**检查资源使用情况
	*/
	virtual void			CheckResourceUsage(StringResRefInfoMapType& mapGroupResRefInfo, StringResRefInfoMapType& groupSubResRefCountMap){}
	void					CheckResourceUsageForTile(STile* pTile, StringResRefInfoMapType& mapGroupResRefInfo, StringResRefInfoMapType& groupSubResRefCountMap);

	/**导出层信息到文本文件
	*/
	virtual void			ExportToFile(const Cactus::String& strPathName){}

public:

	virtual void			Draw(CDC* pDC, const IntVectorType& regions){}

	/**获取当前像素点的Tile
	*	@return	如果有多个返回第一个，无则返回0
	*/
	virtual STile*			TileHitTest(CPoint ptPixel, CPoint& ptGrid){ return 0; }

	/**在当前网格坐标增加或修改STile，并更新视图
	*/
	virtual bool			AddOrUpdateTile(CPoint ptGrid, const Cactus::String& resGroup, const Cactus::String& strItemID){ return false; }

	/**移动STile到新的网格坐标，并更新视图
	*/
	virtual bool			MoveTile(STile* pTile, CPoint ptNewGrid){ return false; }

	/**删除当前网格坐标的STile，并更新视图
	*/
	virtual bool			RemoveTile(CPoint ptGrid){ return false; }
	virtual bool			RemoveTile(STile* pTile){ return false; }

	/**填充地层面板中STile列表
	*/
	virtual void			FillListInPanelMapLayer(){}

	/**保存到XML文件
	*/
	virtual void			Save(Cactus::XMLOutStream& xmlOut){}

	/**在iRegion内增加一组Tile
	*/
	virtual void			AddRegionTiles(int iRegion, const TileVectorType& tiles){}

protected:
	Map*				_pParentMap;

	bool				_bEnable;
	bool				_bVisible;

	EMapLayerType		_eLayerType;
};

