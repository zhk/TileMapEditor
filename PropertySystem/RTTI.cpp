/*=============================================================================
*	Copyright (C) 2006-2008, Zhang Kun. All Rights Reserved.
*
*	File name:
*			RTTI.cpp
*	Description:
*			
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
=============================================================================*/
#include "stdafx.h"
#include "RTTI.h"


using namespace std;
using namespace PropertySys;

void RTTIManager::RegisterRTTI(const std::string& str, RTTI *p)
{
	_RTTIClasses[str] = p;
}

RTTI* RTTIManager::GetRTTI(const std::string& str)
{
	RTTIMapType::iterator it = _RTTIClasses.find(str);
	if (it != _RTTIClasses.end())
	{
		return it->second;
	}

	return 0;
}

//-----------------------------------------------------------------------------------------------

RTTI::RTTI(const string& szClassName
		   , RTTI* pBaseClassRTTI
		   , ClassFactoryFunc pFactory
		   , RegisterPropertyFunc pPropertyFunc
		   )
: _pBaseRTTI( pBaseClassRTTI )
, _pObjectFactory( pFactory )
, _iObjectCount(0)
{
	_szClassName = szClassName.substr(sizeof("class"));

	RTTIManager::getSingleton().RegisterRTTI(_szClassName, this);

	if ( pPropertyFunc )
		pPropertyFunc();
}

void RTTI::EnumProperties( vector<BaseProperty*>& outResult )
{
	if ( _pBaseRTTI )
		_pBaseRTTI->EnumProperties( outResult );

	for ( list<BaseProperty*>::iterator it = _Properties.begin(); it != _Properties.end(); ++it )
		outResult.push_back( *it );
}

BaseProperty* RTTI::GetPropertyByName(const std::string& strPropName)
{
	for ( list<BaseProperty*>::iterator it = _Properties.begin(); it != _Properties.end(); ++it )
		if ((*it)->GetName() == strPropName)
		{
			return *it;
		}

	if ( _pBaseRTTI )
		_pBaseRTTI->GetPropertyByName( strPropName );

	return 0;
}
