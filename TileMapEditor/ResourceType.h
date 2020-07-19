#pragma once

#include "GridTypes.h"

enum EResourceType
{
	eResTypeBackground,
	eResTypeTile,
	eResTypeGameObject,
	eResTypeMax
};

/**
*	��Դ����ࣺһ����Դ��ӵ����������Դ
*/
class Resource : public PropertySys::SupportRTTI<Resource, PropertySys::RTTIObject>
{
public:
	Resource()
		: _bHasImageList(false)
	{
	}
	virtual ~Resource(){}

	//������Դ��
	virtual	bool			Load(){ return false; }

	//��������ResouceListView��Ҫ��ͼ���б�
	virtual void			CreateImageList(CDC* pDC, bool bForceRecreate = false){}

	/** ����Դ�Ƿ���Ч
	*	@param strName	����Դ��
	*	@return true����Ч
	*/ 
	virtual bool			IsResItemValid(const Cactus::String& strName){ return false; }

	//��ȡͼ���б�
	CImageList*				GetImageList(){ return &_imageList; }

	/** ���Ʊ��ΪstrID������Դ
	*	@param pDC			���Ƶ��豸
	*	@param pGridObject	����������
	*	@param ptGrid		��ǰ��������
	*	@param strID		����Դ��
	*/ 
	virtual void			Draw(CDC* pDC, GridObject* pGridObject, const CPoint& ptGrid, const Cactus::String& strID){}

	/** ���Ʊ��ΪstrID������Դ
	*	@param pDC			���Ƶ��豸
	*	@param ptTopLeft	��ǰTile�İ�Χ����
	*	@param strID		����Դ��
	*/ 
	virtual void			Draw(CDC* pDC, const CPoint& ptTopLeft, const Cactus::String& strID){}

	//��ȡ��Դ������
	Cactus::String			GetResourceName(){ return _strResName; }

	//��ȡ��Դ������
	EResourceType			GetResourceType(){ return _eResType; }

	//��ȡ����Դ������
	Cactus::StringVector*	GetCaptions(){ return &_captions; }

	/**��ȡ����Դ�İ�Χ����
	*	@param curTile	��ǰTile�İ�Χ����
	*	@param eGrid	��ǰ��������
	*	@param strID	����Դ��
	*/
	virtual CRect			GetResItemBoundingRect(const CRect& curTile, EGridType eGrid, const Cactus::String& strID){ return CRect(0, 0, 1, 1); }

	//����δ�����õ�����Դ
	virtual void			ScanAllResourceUsage(SubResRefCountMapType& subResIDCountInfo){}

	virtual Cactus::String	GetSubResFileName(const Cactus::String& strID){ return ""; }

	//��ȡ��Դ���Ŀ¼�������RootDir
	virtual Cactus::String	GetResFolder(){ return ""; }

protected:
	Cactus::String			_strResName;		//��Դ������
	CImageList				_imageList;			//����ResouceListView��ͼ���б�
	bool					_bHasImageList;		//ͼ���б��Ƿ��Ѿ�����
	Cactus::StringVector	_captions;			//����Դ����������
	int						_iIconSize;			//��Դ�б�����ͼ��Ĵ�С
	EResourceType			_eResType;			//��Դ������
};

