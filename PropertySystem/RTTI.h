/*=============================================================================
*	Copyright (C) 2006-2008, Zhang Kun. All Rights Reserved.
*
*	File name:
*			RTTI.h
*	Description:
*			
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
=============================================================================*/
#pragma once

#include "PreInclude.h"
#include "BaseProperty.h"

namespace PropertySys
{
	class RTTIObject;

	typedef RTTIObject*		(*ClassFactoryFunc)();
	typedef bool			(*RegisterPropertyFunc)();

	//-------------------------------------------------------------------------------------------------
	class PROPERTYSYSTEM_API RTTI
	{

	public:

		// RTTI constructor.
		//
		// The RTTI structure constructor takes in the following parameters:
		//		szClassName		The name of the class type this RTTI structure represents
		//		pBaseClassRTTI	The parent RTTI structure for this RTTI structure
		//		pFactory		A factory function callback for creating an instance of the bound class type
		//		pPropertyFunc	Callback called by the system to register the reflective properties
		RTTI(const std::string& szClassName
			, RTTI* pBaseClassRTTI
			, ClassFactoryFunc pFactory
			, RegisterPropertyFunc pPropertyFunc
			);

		// Fills a vector with all properties of the represented class type, including all ancestor types.
		void						EnumProperties( std::vector<BaseProperty*>& outResult );

		// Returns true if the RTTI structure is of the type specified by CLID.
		inline bool					IsTypeOf(const std::string& strClassName);

		// Returns true if the RTTI structure is derived from the type specified by CLID.
		inline bool					IsDerivedFrom(const std::string& strClassName);

		// Gets base class' RTTI structure.
		inline RTTI*				GetBaseClassRTTI();

		// Gets the class name.
		inline std::string			GetClassName();

		// Return object count of such class and increase count 1
		inline int					IncreaseObjectCount();

		// Gets the class factory function.
		inline ClassFactoryFunc		GetClassFactory();

		// Provides access to the properties bound to this run-time type. Does not include ancestor class
		// properties. Use EnumProperties to include ancestor properties.
		inline std::list<BaseProperty*>::iterator	GetFirstProperty();
		inline std::list<BaseProperty*>::iterator	GetLastProperty();
		inline std::list<BaseProperty*>*			GetProperties();

		BaseProperty*				GetPropertyByName(const std::string& strPropName);

	private:

		std::string					_szClassName;			// Class name
		int							_iObjectCount;	
		RTTI*						_pBaseRTTI;				// Base class RTTI structure
		ClassFactoryFunc			_pObjectFactory;		// Factory function
		std::list<BaseProperty*>	_Properties;			// Property list
	};

	//-------------------------------------------------------------------------------------------------
	inline RTTI* RTTI::GetBaseClassRTTI()
	{
		return _pBaseRTTI;
	}

	inline ClassFactoryFunc RTTI::GetClassFactory()
	{
		return _pObjectFactory;
	}

	inline std::string RTTI::GetClassName()
	{
		return _szClassName;
	}

	inline int RTTI::IncreaseObjectCount()
	{
		return _iObjectCount++;
	}

	inline bool RTTI::IsDerivedFrom(const std::string& strClassName)
	{
		if( _szClassName == strClassName )
			return true;
		else if( _pBaseRTTI )
			return _pBaseRTTI->IsDerivedFrom( strClassName );

		return false;
	}

	inline bool RTTI::IsTypeOf(const std::string& strClassName)
	{
		return _szClassName == strClassName;
	}

	inline std::list<BaseProperty*>::iterator RTTI::GetFirstProperty()
	{
		return _Properties.begin();
	}

	inline std::list<BaseProperty*>* RTTI::GetProperties()
	{
		return &_Properties;
	}

	inline std::list<BaseProperty*>::iterator RTTI::GetLastProperty()
	{
		return _Properties.end();
	}

	//-------------------------------------------------------------------------------------------------
	class PROPERTYSYSTEM_API RTTIManager
	{
	public:
		void	RegisterRTTI(const std::string& str, RTTI *p);
		RTTI*	GetRTTI(const std::string& str);

		static	RTTIManager&	getSingleton()
		{
			static RTTIManager instance;
			return instance;
		}

	private:
		RTTIManager(){};

		typedef std::map<std::string, RTTI*>	RTTIMapType;
		RTTIMapType		_RTTIClasses;
	};
}

