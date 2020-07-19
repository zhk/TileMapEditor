#pragma once

#include "ResourceType.h"

//һ�Ŵ�ͼ����Դ���Ѿ�����
class ResourceBackground : public PropertySys::SupportRTTI<ResourceBackground, Resource>
{
	friend class ResourceManager;
	friend class ResourceArt_xmlHandler;
public:
	ResourceBackground();
	~ResourceBackground();

	virtual bool		Load();
	virtual void		CreateImageList(CDC* pDC, bool bForceRecreate = false);

	virtual void		Draw(CDC* pDC, GridObject* pGridObject, const CPoint& ptGrid, const Cactus::String& strID);
	virtual void		Draw(CDC* pDC, const CPoint& ptTopLeft, const Cactus::String& strID);

	static void			RegisterReflection();
	virtual void		OnPropertyChanged(const std::string& propName);

	virtual void		Save(Cactus::XMLOutStream& xmlOut);

protected:
	Cactus::String		_strResName;
	Cactus::String		_strImagePathName;
	int					_iWidth;
	int					_iHeight;

	CxImage				_image;

	CBitmap*			_pBmp;
};
