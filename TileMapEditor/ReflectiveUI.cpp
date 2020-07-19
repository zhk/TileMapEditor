/*
-----------------------------------------------------------------------------
This source file is part of OMV(Ogre Mesh Viewer)
For the latest info, see http://code.google.com/p/ogremeshviewer/

Copyright (c) 2010 Zhang Kun(zhk.tiger@gmail.com)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.

-----------------------------------------------------------------------------
This software also uses Microsoft Fluent UI.
License terms to copy, use or distribute the Fluent UI are available separately.  
To learn more about our Fluent UI licensing program, please visit 
http://msdn.microsoft.com/officeui.

Generally speaking, Fluent UI is free, if you do not use it make a contest software
like Office.
-----------------------------------------------------------------------------
*/
*/
#include "stdafx.h"
#include "ReflectiveUI.h"

#include "GString.h"

using namespace std;
using namespace PropertySys;

//-------------------------------------------------------------------------------------
//CMFCPropertyGridColor32Property

#define ARGB(a,r,g,b)        ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16))|(((DWORD)(BYTE)(a))<<24))
#define GetAValue(argb)      (LOBYTE((argb)>>24))

class CMFCPropertyGridColor32Property : public CMFCPropertyGridColorProperty
{
public:
	CMFCPropertyGridColor32Property(const CString& strName,const COLORREF& color,CPalette* pPalette=NULL,LPCTSTR lpszDescr=NULL,DWORD_PTR dwData=0);

public:
	virtual BOOL OnUpdateValue();
	virtual void OnDrawValue(CDC* pDC, CRect rect);
	virtual CString FormatProperty();
};

CMFCPropertyGridColor32Property::CMFCPropertyGridColor32Property(const CString& strName, const COLORREF& color,CPalette* pPalette/*=NULL*/,LPCTSTR lpszDescr/*=NULL*/,DWORD_PTR dwData/*=0*/)
:CMFCPropertyGridColorProperty(strName,color,pPalette,lpszDescr,dwData)
{
}

BOOL CMFCPropertyGridColor32Property::OnUpdateValue()
{
	ASSERT_VALID(this);

	if (m_pWndInPlace == NULL)
	{
		return FALSE;
	}

	ASSERT_VALID(m_pWndInPlace);
	ASSERT(::IsWindow(m_pWndInPlace->GetSafeHwnd()));

	CString strText;
	m_pWndInPlace->GetWindowText(strText);

	COLORREF colorCurr = m_Color;
	int nA = 0,nR = 0, nG = 0, nB = 0;
	_stscanf_s(strText, _T("%2x%2x%2x%2x"), &nA, &nR, &nG, &nB);
	if(nA == 0 && (nR !=0 || nG != 0 || nB !=  0))
		nA = 0xFF;
	m_Color = ARGB(nA, nR, nG, nB);

	if (colorCurr != m_Color)
	{
		m_pWndList->OnPropertyChanged(this);
	}

	return TRUE;
}

void CMFCPropertyGridColor32Property::OnDrawValue(CDC* pDC, CRect rect)
{
	CRect rectColor = rect;

	rect.left += rect.Height();
	CMFCPropertyGridProperty::OnDrawValue(pDC, rect);

	rectColor.right = rectColor.left + rectColor.Height();
	rectColor.DeflateRect(1, 1);
	rectColor.top++;
	rectColor.left++;

	CBrush br(m_Color == (COLORREF)-1 ? m_ColorAutomatic : (m_Color&0x00FFFFFF));
	pDC->FillRect(rectColor, &br);
	pDC->Draw3dRect(rectColor, 0, 0);
}

CString CMFCPropertyGridColor32Property::FormatProperty()
{
	ASSERT_VALID(this);

	CString str;
	str.Format(_T("%02x%02x%02x%02x"),GetAValue(m_Color),GetRValue(m_Color),GetGValue(m_Color),GetBValue(m_Color));

	return str;
}

//-------------------------------------------------------------------------------------
void CPropertyGrid::OnPropertyChanged(CMFCPropertyGridProperty* pProp) const
{
	if (_pPropertyChanged)
	{
		_pPropertyChanged->OnPropertyChanged(pProp);
	}
}

#if 0
// MFC bug, this does not work!
// Details : can not retrieve the data just input.
BOOL CPropertyGrid::ValidateItemData(CMFCPropertyGridProperty* pProp)
{
	if (_pPropertyChanged)
	{
		return _pPropertyChanged->ValidateItemData(pProp);
	}

	return FALSE;
}
#endif

//-------------------------------------------------------------------------------------
CReflectiveUI::CReflectiveUI()
{
}

template<class T>
void CReflectiveUI::AddPropertyToGrid(CMFCPropertyGridProperty* root, RTTIObject* pObject, BaseProperty* pProp)
{
	CMFCPropertyGridProperty* pGrid;

	TypedProperty<T>* pTypedProperty = (TypedProperty<T>*)pProp;
	EValueSpecify e = pProp->GetValueSpecify();
	const std::vector<std::string>&	Values = pProp->GetSpeValues();

	if ( e == eValueRange )
	{
		T s = TypeValueFromString<T>(Values[0]);
		T e = TypeValueFromString<T>(Values[1]);

		pGrid = new CMFCPropertyGridProperty(
			pTypedProperty->GetName().c_str()
			, (_variant_t)pTypedProperty->GetValue( pObject )
			, pTypedProperty->GetDescription().c_str()
			);

		pGrid->EnableSpinControl(TRUE, s, e);
	}
	else if ( e == eValueList )
	{
		pGrid = new CMFCPropertyGridProperty(
			pTypedProperty->GetName().c_str()
			, (_variant_t)pTypedProperty->GetValue( pObject )
			, pTypedProperty->GetDescription().c_str()
			);

		for (size_t t = 0; t < Values.size(); ++t)
		{
			pGrid->AddOption(Values[t].c_str());
		}
		pGrid->AllowEdit(FALSE);
	}
	else if ( e == eFilePathName )
	{
		pGrid = new CMFCPropertyGridFileProperty(
			pTypedProperty->GetName().c_str()
			, (_variant_t)pTypedProperty->GetValue( pObject )
			, pTypedProperty->GetDescription().c_str()
			);
	}
	else if ( e == eValueColor )
	{
		pGrid = new CMFCPropertyGridColorProperty(
			pTypedProperty->GetName().c_str()
			, pTypedProperty->GetValue(pObject)
			, 0
			, pTypedProperty->GetDescription().c_str()
			);

		((CMFCPropertyGridColorProperty*)pGrid)->EnableOtherButton(_T("ÆäËû..."));
		((CMFCPropertyGridColorProperty*)pGrid)->EnableAutomaticButton(_T("Ä¬ÈÏ"), ::GetSysColor(COLOR_3DFACE));

	}
	else
	{
		pGrid = new CMFCPropertyGridProperty(
			pTypedProperty->GetName().c_str()
			, (_variant_t)pTypedProperty->GetValue( pObject )
			, pTypedProperty->GetDescription().c_str()
			);
	}

	if (!pGrid)
		return;

	if (pTypedProperty->IsReadOnly())
		pGrid->AllowEdit(FALSE);

	root->AddSubItem( pGrid );

	CReflectiveUI::SPropertyObject PO(pProp, pObject);
	_GridPropertyMap[pGrid] = PO;
}

template<class T>
void CReflectiveUI::AddPropertyToGrid_AsString(CMFCPropertyGridProperty* root, PropertySys::RTTIObject* pObject, PropertySys::BaseProperty* pProp)
{
	CMFCPropertyGridProperty* pGrid = 0;

	TypedProperty<T>* pTypedProperty = (TypedProperty<T>*)pProp;
	EValueSpecify e = pProp->GetValueSpecify();
	const std::vector<std::string>&	Values = pProp->GetSpeValues();

	if ( e == eValueRange )
	{
	}
	else if ( e == eValueList )
	{
		pGrid = new CMFCPropertyGridProperty(
			pTypedProperty->GetName().c_str()
			, (_variant_t)(pTypedProperty->GetValueAsString( pObject ).c_str())
			, pTypedProperty->GetDescription().c_str()
			);

		for (size_t t = 0; t < Values.size(); ++t)
		{
			pGrid->AddOption(Values[t].c_str());
		}
		pGrid->AllowEdit(FALSE);
	}
	else if ( e == eFilePathName )
	{
		pGrid = new CMFCPropertyGridFileProperty(
			pTypedProperty->GetName().c_str()
			, (_variant_t)(pTypedProperty->GetValueAsString( pObject ).c_str())
			, pTypedProperty->GetDescription().c_str()
			);
	}
	else if ( e == eValueColor )
	{
	}
	else
	{
		pGrid = new CMFCPropertyGridProperty(
			pTypedProperty->GetName().c_str()
			, (_variant_t)(pTypedProperty->GetValueAsString( pObject ).c_str())
			, pTypedProperty->GetDescription().c_str()
			);
	}

	if (!pGrid)
		return;

	if (pTypedProperty->IsReadOnly())
		pGrid->AllowEdit(FALSE);

	root->AddSubItem( pGrid );

	CReflectiveUI::SPropertyObject PO(pProp, pObject);
	_GridPropertyMap[pGrid] = PO;
}


void CReflectiveUI::BuildUIForObject(CMFCPropertyGridProperty* root, RTTIObject* pObject, const char* szName )
{
	CMFCPropertyGridProperty* pRoot = new CMFCPropertyGridProperty(szName);
	if (root)
	{
		root->AddSubItem( pRoot );
	}
	else
	{
		m_pGrid->AddProperty(pRoot);
	}

	std::vector<BaseProperty*> Properties;
	pObject->GetRTTI()->EnumProperties( Properties );

	for ( size_t i = 0; i < Properties.size(); i++ )
	{
		switch ( Properties[i]->GetTypeID() )
		{
		case eptBool:
			{
				AddPropertyToGrid<bool>(pRoot, pObject, Properties[i]);
				break;
			}
		case eptByte:
			{
				AddPropertyToGrid<char>(pRoot, pObject, Properties[i]);
				break;
			}
		case eptInt:
			{
				AddPropertyToGrid<int>(pRoot, pObject, Properties[i]);
				break;
			}
		case eptShort:
			{
				AddPropertyToGrid<short>(pRoot, pObject, Properties[i]);
				break;
			}
		case eptLong :
			{
				AddPropertyToGrid<long>(pRoot, pObject, Properties[i]);
				break;
			}
		case eptPtr:
			{
				TypedProperty<RTTIObject*>* pTypedProperty = (TypedProperty<RTTIObject*>*)Properties[i];
				BuildUIForObject(pRoot
					, pTypedProperty->GetValue( pObject )
					, pTypedProperty->GetName().c_str()
					);
				break;
			}
		case eptFloat:
			{
				AddPropertyToGrid<float>(pRoot, pObject, Properties[i]);
				break;
			}
		case eptString:
			{
				AddPropertyToGrid_AsString<Cactus::String>(pRoot, pObject, Properties[i]);
				break;
			}
		case eptPoint:
			{
				AddPropertyToGrid_AsString<CPoint>(pRoot, pObject, Properties[i]);
			}
			break;
		case eptRect:
			{
				AddPropertyToGrid_AsString<CRect>(pRoot, pObject, Properties[i]);
			}
			break;
		};
	}

	pRoot->Expand();
}

void CReflectiveUI::OnPropertyChanged(CMFCPropertyGridProperty* pProp)
{
	//if (!ValidateItemData(pProp))
	//{
	//	return;
	//}

	CMFCPropertyGridProperty* pItem = pProp;

	const COleVariant variant = pItem->GetValue();

	assert( _GridPropertyMap.find(pItem) != _GridPropertyMap.end() );
	SPropertyObject PO = _GridPropertyMap[pItem];

	switch ( PO._pProperty->GetTypeID() )
	{
	case eptBool:
		{
			TypedProperty<bool>* pTypedProperty = (TypedProperty<bool>*)PO._pProperty;
			pTypedProperty->SetValue( PO._pObject, pItem->GetValue().boolVal != 0);
			break;
		}
	case eptByte:
		{
			TypedProperty<char>* pTypedProperty = (TypedProperty<char>*)PO._pProperty;
			pTypedProperty->SetValue( PO._pObject, pItem->GetValue().bVal );
			break;
		}
	case eptShort:
		{
			TypedProperty<short>* pTypedProperty = (TypedProperty<short>*)PO._pProperty;
			pTypedProperty->SetValue( PO._pObject, pItem->GetValue().iVal );
			break;
		}
	case eptInt:
		{
			TypedProperty<int>* pTypedProperty = (TypedProperty<int>*)PO._pProperty;
			pTypedProperty->SetValue( PO._pObject, pItem->GetValue().intVal );
			break;
		}
	case eptLong:
		{
			TypedProperty<long>* pTypedProperty = (TypedProperty<long>*)PO._pProperty;
			pTypedProperty->SetValue( PO._pObject, pItem->GetValue().lVal );
			break;
		}
	case eptPtr:
		{
			break;
		}
	case eptFloat:
		{
			TypedProperty<float>* pTypedProperty = (TypedProperty<float>*)PO._pProperty;
			pTypedProperty->SetValue( PO._pObject, pItem->GetValue().fltVal );
			break;
		}
	case eptString:
		{
			TypedProperty<Cactus::String>* pTypedProperty = (TypedProperty<Cactus::String>*)PO._pProperty;
			GString gStr(pItem->GetValue().bstrVal);
			pTypedProperty->SetValueFromString( PO._pObject, gStr.ToMbcs() );
			break;
		}
	case eptPoint:
		{
			TypedProperty<CPoint>* pTypedProperty = (TypedProperty<CPoint>*)PO._pProperty;
			GString gStr(pItem->GetValue().bstrVal);
			pTypedProperty->SetValueFromString( PO._pObject, gStr.ToMbcs() );
		}
		break;
	case eptRect:
		{
			TypedProperty<CRect>* pTypedProperty = (TypedProperty<CRect>*)PO._pProperty;
			GString gStr(pItem->GetValue().bstrVal);
			pTypedProperty->SetValueFromString( PO._pObject, gStr.ToMbcs() );
		}
		break;
	};
}

#if 0
// MFC bug, this does not work!
// Details : can not retrieve the data just input.
bool CReflectiveUI::ValidateItemData(CMFCPropertyGridProperty* pProp)
{
	CMFCPropertyGridProperty* pItem = pProp;

	assert( _GridPropertyMap.find(pItem) != _GridPropertyMap.end() );
	SPropertyObject PO = _GridPropertyMap[pItem];

	bool bValid = true;

	switch ( PO._pProperty->GetTypeID() )
	{
	case eptBool:
		{
			TypedProperty<bool>* pTypedProperty = (TypedProperty<bool>*)PO._pProperty;
			bValid = pTypedProperty->ValidateValue( pItem->GetValue().boolVal != 0);
			break;
		}
	case eptByte:
		{
			TypedProperty<char>* pTypedProperty = (TypedProperty<char>*)PO._pProperty;
			bValid = pTypedProperty->ValidateValue( pItem->GetValue().bVal );
			break;
		}
	case eptShort:
		{
			TypedProperty<short>* pTypedProperty = (TypedProperty<short>*)PO._pProperty;
			bValid = pTypedProperty->ValidateValue( pItem->GetValue().iVal );

			break;
		}
	case eptInt:
		{
			TypedProperty<int>* pTypedProperty = (TypedProperty<int>*)PO._pProperty;
			bValid = pTypedProperty->ValidateValue( pItem->GetValue().intVal );

			break;
		}
	case eptLong:
		{
			TypedProperty<long>* pTypedProperty = (TypedProperty<long>*)PO._pProperty;
			bValid = pTypedProperty->ValidateValue( pItem->GetValue().lVal );
			break;
		}
	case eptPtr:
		{
			break;
		}
	case eptOgreReal:
		{
			TypedProperty<Ogre::Real>* pTypedProperty = (TypedProperty<Ogre::Real>*)PO._pProperty;
			bValid = pTypedProperty->ValidateValue( pItem->GetValue().fltVal );

			break;
		}
	case eptOgreString:
		{
			TypedProperty<Ogre::String>* pTypedProperty = (TypedProperty<Ogre::String>*)PO._pProperty;
			GString gStr(pItem->GetValue().bstrVal);
			bValid = pTypedProperty->ValidateValue( gStr.ToMbcs() );

			break;
		}
	};

	if (!bValid)
	{
		pItem->SetValue(pItem->GetOriginalValue());
	}

	return bValid;
}
#endif