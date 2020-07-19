/*=============================================================================
*	Copyright (C) 2006-2008, Zhang Kun. All Rights Reserved.
*
*	File name:
*			TypedProperty.h
*	Description:
*			
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
=============================================================================*/
#pragma once

#include "BaseProperty.h"

namespace PropertySys
{

	class RTTIObject;

	// This intermediate class defines a property that is typed, but not bound as a member of a particular
	// class.

	template <class ValueType>
	class TypedProperty : public BaseProperty
	{
	public:

		// Returns the type of this property.
		ePropertyType			GetTypeID() const;
		
		bool					ValidateValue(const ValueType& Value);

		// Determines the value of this property.
		virtual ValueType		GetValue( RTTIObject* pObject ) = 0;
		virtual void			SetValue( RTTIObject* pObject, const ValueType& Value ) = 0;

		// For XML serialization
		virtual std::string		GetValueAsString( RTTIObject* pObject ) = 0;
		virtual void			SetValueFromString( RTTIObject* pObject, const std::string& strVal ) = 0;
	};

	template <class ValueType>
	ePropertyType TypedProperty<ValueType>::GetTypeID() const
	{
		return PropertyType<ValueType>::GetTypeID();
	}

	template <class ValueType>
	bool TypedProperty<ValueType>::ValidateValue(const ValueType& Value)
	{
		switch (_eValueSpecify)
		{
		case eSpecifyNone:
			return true;

		case eValueRange:
			{
				if(_ValueSpecifies.size() != 2)
					return false;

				ValueType s = TypeValueFromString<ValueType>(_ValueSpecifies[0]);
				ValueType e = TypeValueFromString<ValueType>(_ValueSpecifies[1]);

				return (Value >= s && Value <= e);
			}

		case eValueList:
			{
				if(_ValueSpecifies.size() == 0)
					return false;

				bool bFound = false;
				for (size_t t = 0; t < _ValueSpecifies.size(); ++t)
				{
					ValueType val = TypeValueFromString<ValueType>(_ValueSpecifies[t]);
					if(val == Value)
					{
						bFound = true;
						break;
					}
				}

				return bFound;
			}

		case eFilePathName:		//for use CFileDialog browser
			return true;

		case eValueColor:		//for use CColorDialog
			return true;
		}

		return false;
	}

}