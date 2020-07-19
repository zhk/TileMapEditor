/*=============================================================================
*	Copyright (C) 2006-2008, Zhang Kun. All Rights Reserved.
*
*	File name:
*			Property.h
*	Description:
*			
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
=============================================================================*/
#pragma once

#include "TypedProperty.h"
#include "TypeValueFuctions.h"
#include "TypeValueFuctionsOgre.h"

namespace PropertySys
{
	// Fully defined property class. Specifies property's name, type and the getter/setters are bound as
	// members of a specific class type.
	// Template parameter OwnerType is the class that the getter and setter are a member of and template
	// parameter vType is the property type.

	template <class OwnerType, class ValueType>
	class Property : public TypedProperty<ValueType>
	{
	public:

		typedef ValueType	(OwnerType::*GetterType)();					// Getter function
		typedef void		(OwnerType::*SetterType)(ValueType Value );	// Setter function
		typedef bool		(OwnerType::*ValiderType)(ValueType Value );	// Validate function

		// Constructor. Takes in property's name, getter and setter functions.
		inline Property( const char* szName
			, const char* szOwner
			, const char* szCategory
			, const char* szDescription
			, unsigned int uFlag
			, bool bCommonProperty
			, size_t szOffset
			, int iStartVersion
			, ValiderType Valider
			, GetterType Getter
			, SetterType Setter
			);

		// Determines the value of this property.
		virtual ValueType		GetValue( RTTIObject* pObject );
		virtual void			SetValue( RTTIObject* pObject, const ValueType& Value );

		virtual std::string		GetValueAsString( RTTIObject* pObject );
		virtual void			SetValueFromString( RTTIObject* pObject, const std::string& strVal );

	protected:

		GetterType		_Getter;
		SetterType		_Setter;
		ValiderType		_Valider;
	};

	template <class OwnerType, class ValueType>
	inline Property<OwnerType, ValueType>::Property( const char* szName
		, const char* szOwner
		, const char* szCategory
		, const char* szDescription
		, unsigned int uFlag
		, bool bCommonProperty
		, size_t szOffset
		, int iStartVersion
		, ValiderType Valider
		, GetterType Getter
		, SetterType Setter
		)
		: _Getter( Getter )
		, _Setter( Setter )
		, _Valider(Valider)
	{
		_strName			= szName;				
		_strOwner			= szOwner;				
		_strCategory		= szCategory;			
		_strDescription		= szDescription;		
		_uFlag				= uFlag;					
		_bCommonProperty	= bCommonProperty;
		_szOffset			= szOffset;				
		_iStartClassVersion	= iStartVersion;	
	}

	//---------------------------------------------------------------------------------------
	template <class OwnerType, class ValueType> 
	ValueType Property<OwnerType, ValueType>::GetValue( RTTIObject* pOwner )
	{
		if (_bCommonProperty)
		{
			return TypeGetValue<ValueType>(pOwner, _szOffset);
		}
		else
		{
			return (((OwnerType*)pOwner)->*_Getter)();
		}
	}

	template <class OwnerType, class ValueType> 
	void Property<OwnerType, ValueType>::SetValue(RTTIObject* pOwner, const ValueType& Value )
	{
		if (_bCommonProperty)
		{
			TypeSetValue<ValueType>(pOwner, _szOffset, Value);
			pOwner->OnPropertyChanged(_strName);
		}
		else
		{
			if ( !_Setter)
			{
				assert( false ); // Cannot write to a read-only property
				return;
			}
			(((OwnerType*)pOwner)->*_Setter)( Value );
		}

	}

	//---------------------------------------------------------------------------------------
	template <class OwnerType, class ValueType> 
	std::string Property<OwnerType, ValueType>::GetValueAsString( RTTIObject* pObject )
	{
		ValueType val = GetValue(pObject);

		//std::ostringstream os;
		//os << val;
		//return os.str();

		return TypeValueToString<ValueType>(val);
	}

	template <class OwnerType, class ValueType> 
	void Property<OwnerType, ValueType>::SetValueFromString( RTTIObject* pObject, const std::string& strVal )
	{
		ValueType val = TypeValueFromString<ValueType>(strVal);

		SetValue(pObject, val);
	}

}