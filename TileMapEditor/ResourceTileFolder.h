#pragma once

#include "ResourceTile.h"

//ԭʼ����Դͼ����һ����Ŀ¼�У�����ͼ���������ͬ�ĺ�׺�����ǿ����в�ͬ��С
class ResourceTileFolder : public PropertySys::SupportRTTI<ResourceTileFolder, ResourceTile>
{
	friend class ResourceManager;
	friend class ResourceArt_xmlHandler;
public:
	ResourceTileFolder();
	ResourceTileFolder(const Cactus::String& strName, const Cactus::String& strArtResName, const Cactus::String& strExt, int iIconSize);
	~ResourceTileFolder();

	virtual bool			Load();
	virtual void			CreateImageList(CDC* pDC, bool bForceRecreate = false);
	virtual void			Draw(CDC* pDC, GridObject* pGridObject, const CPoint& ptGrid, const Cactus::String& strID);
	virtual void			Draw(CDC* pDC, const CPoint& ptTopLeft, const Cactus::String& strID);
	virtual CRect			GetResItemBoundingRect(const CRect& curTile, EGridType eGrid, const Cactus::String& strID);

	static void				RegisterReflection();
	virtual void			OnPropertyChanged(const std::string& propName);

	virtual void			Save(Cactus::XMLOutStream& xmlOut);

	virtual Cactus::String	GetSubResFileName(const Cactus::String& strID);
	virtual Cactus::String	GetResFolder(){ return _strFolderName; }

protected:
	Cactus::String		_strFolderName;		//Ŀ¼��
	Cactus::String		_strFileExt;		//��׺��������'.'

	typedef Cactus::hashmap<Cactus::String, CxImage*>::type	IDImageMapType;
	IDImageMapType		_images;			//�����ڴ����������Դͼ�����
};