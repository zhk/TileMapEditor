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

#ifndef _REFLECTIVE_UI_H
#define _REFLECTIVE_UI_H

class IPropretyChanged
{
public:
	virtual void	OnPropertyChanged(CMFCPropertyGridProperty* pProp) = 0;
	//virtual bool	ValidateItemData(CMFCPropertyGridProperty* pProp) = 0;
};

class CPropertyGrid : public CMFCPropertyGridCtrl
{
public:
	CPropertyGrid()
		: _pPropertyChanged(0)
	{
	}

	virtual void	OnPropertyChanged(CMFCPropertyGridProperty* pProp) const;
	//virtual BOOL	ValidateItemData(CMFCPropertyGridProperty* pProp);

	void			SetPropertyChangeManager(IPropretyChanged* p)
	{
		_pPropertyChanged = p;
	}

protected:
	IPropretyChanged*	_pPropertyChanged;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// CReflectiveUI
//
// This class builds and updates the property grid UI, as well as responding to its messages.
//
///////////////////////////////////////////////////////////////////////////////////////////////////

class CReflectiveUI : public IPropretyChanged
{

public :

	CReflectiveUI();

	// Builds a dynamic property grid for the specified object by looking up its reflective properties.
	void	BuildUIForObject(CMFCPropertyGridProperty* root, PropertySys::RTTIObject* pObject, const char* szName );

	// Messages received from the property grid control are routed here
	virtual void	OnPropertyChanged(CMFCPropertyGridProperty* pProp);
	//virtual bool	ValidateItemData(CMFCPropertyGridProperty* pProp);

	// The property grid control itself
	inline	CPropertyGrid*		GetGrid();
	inline	void				SetGrid( CPropertyGrid* pGrid );

	void						ResetGridPropertyMap(){ _GridPropertyMap.clear(); }

private :

	CPropertyGrid*		m_pGrid;				// Property grid control

	struct SPropertyObject
	{
		SPropertyObject(){}

		SPropertyObject(PropertySys::BaseProperty* pP, PropertySys::RTTIObject* pO)
		{
			_pProperty	= pP;
			_pObject	= pO;
		}
		PropertySys::BaseProperty*	_pProperty;
		PropertySys::RTTIObject*	_pObject;
	};
	typedef std::map<CMFCPropertyGridProperty*, SPropertyObject>	GridPropertyMapType;
	GridPropertyMapType	_GridPropertyMap;

	template<class T>
	void AddPropertyToGrid(CMFCPropertyGridProperty* root, PropertySys::RTTIObject* pObject, PropertySys::BaseProperty* pProp);

	template<class T>
	void AddPropertyToGrid_AsString(CMFCPropertyGridProperty* root, PropertySys::RTTIObject* pObject, PropertySys::BaseProperty* pProp);
};

inline CPropertyGrid* CReflectiveUI::GetGrid()
{
	return m_pGrid;
}

inline void CReflectiveUI::SetGrid( CPropertyGrid* pGrid )
{
	m_pGrid = pGrid;
}

#endif	// _REFLECTIVE_UI_H