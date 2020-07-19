#pragma once

enum EGridType
{
	eRectangle,		//��������
	eDiamond,		//��������
	eGridNone		//���ض���
};

typedef Cactus::list<CPoint>::type	ObstacleListType;

//�������ͻ��࣬�������Ͱ�����ͼ���ز㡢��Ϸ����༭����ͼ��
class GridObject : public PropertySys::SupportRTTI<GridObject, PropertySys::RTTIObject>
{
public:
	GridObject();
	~GridObject();

	static void			RegisterReflection();
	virtual void		OnPropertyChanged(const std::string& propName);

	/**�����������꣬��ȡ��������
	*	@param ptPixel	��ǰ��������
	*	@param ptGrid	���ص���������
	*	@return ����ڵ�ͼ�����ڣ������ɹ�������true������false
	*/
	bool			GetGridCoord(const CPoint& ptPixel, CPoint& ptGrid);

	/**��ȡ��ǰ����İ�Χ���Σ�������������㣩
	*	@param ptGrid	��ǰ����������
	*/
	CRect			GetPixelCoordRect(const CPoint& ptGrid);

	int				GetPixelWidth(){ return _iWidthInTiles * _iUnitTileWidth; }
	int				GetPixelHeight(){ return _iHeightInTiles * _iUnitTileHeight; }

	EGridType		GetGridType(){ return _eGridType; }

	Cactus::String	GetObjectName(){ return _strName; }

	//���Ƶ�λ����
	void			DrawGrid(CDC* pDC, CPoint ptGrid, COLORREF ref, int deflate, int lineWidth = 2);
	void			DrawGridText(CDC* pDC, CPoint ptGrid, COLORREF ref, const Cactus::String& str);
	void			FillGrid(CDC* pDC, CPoint ptGrid, COLORREF ref);

	virtual bool	IsDrawGOCenter(){ return true; }
	virtual bool	IsDrawObstacle(){ return true; }

public:
	int					_iWidthInTiles;			///��ȷ���Tile����
	int					_iHeightInTiles;		///�߶ȷ���Tile����
	int					_iUnitTileWidth;		///��λTile��ȣ�����
	int					_iUnitTileHeight;		///��λTile�߶ȣ�����
	EGridType			_eGridType;				///�������ͣ����ָ�������
	Cactus::String		_strName;
};

typedef Cactus::vector<int>::type	IntVectorType;


typedef::Cactus::map<Cactus::String, int>::type						SubResRefCountMapType;
typedef::Cactus::multimap<int, Cactus::String>::type				SubResRefCount_InvertMapType;
typedef::Cactus::map<Cactus::String, SubResRefCountMapType>::type	StringResRefInfoMapType;
typedef::Cactus::map<Cactus::String, StringResRefInfoMapType>::type	StringStringResRefInfoMapType;

#pragma warning(disable : 4503)