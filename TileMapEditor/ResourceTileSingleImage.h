#pragma once

#include "ResourceTile.h"

//ԭʼͼ����Դ��һ�Ŵ�ͼ���У�����Դ����Ϊ����С��;
//����Դ�������Ǳ��0 - N-1ת���ɵ�String
class ResourceTileSingleImage : public PropertySys::SupportRTTI<ResourceTileSingleImage, ResourceTile>
{
	friend class ResourceManager;
	friend class ResourceArt_xmlHandler;
public:
	ResourceTileSingleImage();
	ResourceTileSingleImage(const Cactus::String& strName, const Cactus::String& strArtRes, int iTW, int iTH, int iIconSize, int iCount);
	~ResourceTileSingleImage();

	virtual bool			Load();
	virtual void			CreateImageList(CDC* pDC, bool bForceRecreate = false);
	virtual void			Draw(CDC* pDC, GridObject* pGridObject, const CPoint& ptGrid, const Cactus::String& strID);
	virtual void			Draw(CDC* pDC, const CPoint& ptTopLeft, const Cactus::String& strID);
	virtual CRect			GetResItemBoundingRect(const CRect& curTile, EGridType eGrid, const Cactus::String& strID);

	static void				RegisterReflection();
	virtual void			OnPropertyChanged(const std::string& propName);

	virtual void			Save(Cactus::XMLOutStream& xmlOut);

	virtual Cactus::String	GetSubResFileName(const Cactus::String& strID){ return _strImageName; }

protected:
	Cactus::String		_strImageName;	//ԭʼͼ���ļ���
	int					_imageWidth;	//ԭʼͼ����
	int					_imageHeight;	//ԭʼͼ��߶�
	int					_tileWidth;		//С��Ŀ��
	int					_tileHeight;	//С��ĸ߶�

	CxImage				_image;			//�����ڴ��ԭʼͼ�����
};
