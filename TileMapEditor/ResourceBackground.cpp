#include "StdAfx.h"
#include "ResourceBackground.h"
#include "ResourceManager.h"

using namespace Cactus;
using namespace PropertySys;

ResourceBackground::ResourceBackground()
: _iWidth(0)
, _iHeight(0)
, _pBmp(0)
{
	_eResType = eResTypeBackground;
}

ResourceBackground::~ResourceBackground()
{
	if (_pBmp)
	{
		_pBmp->DeleteObject();
		delete _pBmp;
	}
}

void ResourceBackground::RegisterReflection()
{
	BaseProperty* pProp = 0;

	M_ReflectionInit(0);

	pProp = M_RegisterPropertySimple(Cactus::String, Name, ResourceBackground, Resource, "图像文件.", BaseProperty::eReadOnly, _strName);

	pProp = M_RegisterPropertySimple(Cactus::String, ImagePathName, ResourceBackground, Resource, "图像文件.", BaseProperty::eReadOnly, _strImagePathName);
	//pProp->SetValueSpecify(eFilePathName, "");

	pProp = M_RegisterPropertySimple(int, ImageWidth, ResourceBackground, Resource, "图像宽度.", BaseProperty::eReadOnly, _iWidth);
	pProp = M_RegisterPropertySimple(int, ImageHeight, ResourceBackground, Resource, "图像高度.", BaseProperty::eReadOnly, _iHeight);

}

void ResourceBackground::OnPropertyChanged(const std::string& propName)
{
}

bool ResourceBackground::Load()
{
	String strFull = ResourceManager::getSingleton().GetRootFolder() + _strImagePathName;

	if( _image.Load(strFull.c_str()) )
	{
		_iWidth		= _image.GetWidth();
		_iHeight	= _image.GetHeight();

		return true;
	}

	return false;
}

void ResourceBackground::CreateImageList(CDC* pDC, bool bForceRecreate/* = false*/)
{
	if (_bHasImageList)
	{
		if (bForceRecreate)
		{
			_pBmp->DeleteObject();
			delete _pBmp;

			_captions.clear();
			_imageList.DeleteImageList();
		}
		else
		{
			return;
		}
	}

	BOOL b = _imageList.Create(_iIconSize, _iIconSize, ILC_COLOR32, 0, 4);

	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);

	_pBmp = new CBitmap;
	_pBmp->CreateCompatibleBitmap(pDC, _iIconSize, _iIconSize);
	CBitmap* bmpOld = dcMem.SelectObject(_pBmp);

	dcMem.FillSolidRect(0, 0, _iIconSize, _iIconSize, RGB(255, 255, 255));

	int iW = _image.GetWidth();
	int iH = _image.GetHeight();

	if (__max(iW, iH) > _iIconSize)
	{
		if (iW > iH)
		{
			float fR = 1.0f * _iIconSize / iW;
			_image.Draw(dcMem.GetSafeHdc(), 0, 0, _iIconSize, int(iH * fR), 0, true);
		}
		else
		{
			float fR = 1.0f * _iIconSize / iH;
			_image.Draw(dcMem.GetSafeHdc(), int((_iIconSize - fR * iW)/2), 0, int(fR * iW), _iIconSize, 0, true);
		}
	}
	else
	{
		_image.Draw(dcMem.GetSafeHdc(), (_iIconSize - iW)/2, 0, -1, -1, 0, true);
	}

	dcMem.SelectObject(bmpOld);

	//HBITMAP hBmp = _image.MakeBitmap(pDC->GetSafeHdc());
	//_pBmp = new CBitmap;
	//_pBmp->Attach(hBmp);

	_imageList.Add(_pBmp, (CBitmap*)0);
	_captions.push_back(_strResName);

	_bHasImageList = true;
}

void ResourceBackground::Draw(CDC* pDC, GridObject* pGridObject, const CPoint& ptGrid, const Cactus::String& strID)
{

}

void ResourceBackground::Draw(CDC* pDC, const CPoint& ptTopLeft, const Cactus::String& strID)
{

}

void ResourceBackground::Save(XMLOutStream& xmlOut)
{
	xmlOut.NodeBegin("item");
		xmlOut.AddAttribute("name", _strResName);
		xmlOut.AddAttribute("res", _strImagePathName);
		xmlOut.AddAttribute("iconsize", _iIconSize);
	xmlOut.NodeEnd("item");
}