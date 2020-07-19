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

	CPoint			_ptGrid;		///��������
	int				_regionID;		///���ڵ�RegionID
	Cactus::String	_strResGroup;	///ʹ�õ���Դ��
	Cactus::String	_strResItemID;	///ʹ�õ���Դ
	bool			_bSelected;
	bool			_bValid;
};

bool STile_Diamond_less(const STile* p1, const STile* p2);
bool STile_Triangle_less(const STile* p1, const STile* p2);

typedef Cactus::vector<STile*>::type		TileVectorType;

//-------------------------------------------------------------------------------------------------------
//�ز����
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

	/**��ȡ��ǰ���ص������������������ط�Χ
	*	@param	ptPixel	��ǰ��������
	*	@param	ptGrid	���ص���������
	*	@param	rcPixel	���ص���������ط�Χ
	*/
	virtual bool			ToolHitTest(CPoint ptPixel, CPoint& ptGrid, CRect& rcPixel);


	/**���µز������STile�б�
	*/
	virtual void			UpdateTileInfoInPanelMapLayer(STile* pTile, ETileOp e = eTileOpAdd);


	/**����ָ��STile����ͼ����
	*/
	virtual void			UpdateTileVisual(STile* pTile, bool bEnsureVisible = false);

	/**���������Դ��Ч��
	*	@return		������Ч��Tile����
	*/
	virtual int				CheckResDepends(){ return 0; }

	/**�����Դʹ�����
	*/
	virtual void			CheckResourceUsage(StringResRefInfoMapType& mapGroupResRefInfo, StringResRefInfoMapType& groupSubResRefCountMap){}
	void					CheckResourceUsageForTile(STile* pTile, StringResRefInfoMapType& mapGroupResRefInfo, StringResRefInfoMapType& groupSubResRefCountMap);

	/**��������Ϣ���ı��ļ�
	*/
	virtual void			ExportToFile(const Cactus::String& strPathName){}

public:

	virtual void			Draw(CDC* pDC, const IntVectorType& regions){}

	/**��ȡ��ǰ���ص��Tile
	*	@return	����ж�����ص�һ�������򷵻�0
	*/
	virtual STile*			TileHitTest(CPoint ptPixel, CPoint& ptGrid){ return 0; }

	/**�ڵ�ǰ�����������ӻ��޸�STile����������ͼ
	*/
	virtual bool			AddOrUpdateTile(CPoint ptGrid, const Cactus::String& resGroup, const Cactus::String& strItemID){ return false; }

	/**�ƶ�STile���µ��������꣬��������ͼ
	*/
	virtual bool			MoveTile(STile* pTile, CPoint ptNewGrid){ return false; }

	/**ɾ����ǰ���������STile����������ͼ
	*/
	virtual bool			RemoveTile(CPoint ptGrid){ return false; }
	virtual bool			RemoveTile(STile* pTile){ return false; }

	/**���ز������STile�б�
	*/
	virtual void			FillListInPanelMapLayer(){}

	/**���浽XML�ļ�
	*/
	virtual void			Save(Cactus::XMLOutStream& xmlOut){}

	/**��iRegion������һ��Tile
	*/
	virtual void			AddRegionTiles(int iRegion, const TileVectorType& tiles){}

protected:
	Map*				_pParentMap;

	bool				_bEnable;
	bool				_bVisible;

	EMapLayerType		_eLayerType;
};

