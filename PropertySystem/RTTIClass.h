/*=============================================================================
*	Copyright (C) 2006-2008, Zhang Kun. All Rights Reserved.
*
*	File name:
*			RTTIClass.h
*	Description:
*			
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
=============================================================================*/

#pragma once

#include "RTTI.h"
#include "Property.h"

//#include "../CommonUtil/XMLOutStream.h"

namespace PropertySys
{

	// SupportRTTI is the RTTI "sandwich class" being used to augment a class with RTTI support. Classes
	// supporting RTTI need to derive from this class, with their ancestor specified as the BaseClass
	// template parameter.
	template <class OwnerClass, class BaseClass>
	class SupportRTTI : public BaseClass
	{

	public :

		// Constructor
		SupportRTTI();

		// Default factory function. Creates an instance of OwnerClass. Called by the system to dynamically create
		// class instances from class IDs.
		static OwnerClass*	Create();

		// Default reflection registration function. Does nothing by default.
		static void			RegisterReflection();

		// Registers a property. Takes in the property name, its getter and setter functions, and the property
		// type as a template parameter. Should be called from within a user-defined RegisterReflection function.
		template <class ValueType> 
		static BaseProperty*	RegisterProperty(
			  const char* szName
			, const char* szOwner
			, const char* szCategory
			, const char* szDescription
			, unsigned int uFlag
			, bool bCommonProperty
			, size_t szOffset
			, int iStartVersion
			, typename Property<OwnerClass, ValueType>::ValiderType Valider
			, typename Property<OwnerClass, ValueType>::GetterType Getter
			, typename Property<OwnerClass, ValueType>::SetterType Setter
			)
		{
			if(!bCommonProperty)
			{
				assert(Getter);
			}

			Property<OwnerClass, ValueType>* pProperty = new Property<OwnerClass, ValueType>(
				szName
				, szOwner
				, szCategory
				, szDescription
				, uFlag
				, bCommonProperty
				, szOffset
				, iStartVersion
				, Valider
				, Getter
				, Setter
				);

			OwnerClass::GetClassRTTI()->GetProperties()->push_back( pProperty );

			return pProperty;
		}

		// Returns RTTI info associated with this class type.
		static inline RTTI*		GetClassRTTI()
		{
			return &s_RTTI;
		}

		// Returns RTTI info associated with this class instance.
		virtual RTTI*			GetRTTI()
		{
			return &s_RTTI;
		}

		static inline void		SetClassVersion(int iVer)
		{
			s_iVersion = iVer;
		}

		virtual inline int		GetClassVersion()
		{
			return s_iVersion;
		}

	protected :

		static RTTI		s_RTTI;				// RTTI structure
		static int		s_iVersion;			// Class version																		
	};

	//---------------------------------------------------------------------------------------
	template <class OwnerClass, class BaseClass>
	RTTI SupportRTTI<OwnerClass, BaseClass>::s_RTTI
		(typeid(OwnerClass).name()
		, BaseClass::GetClassRTTI()
		, (ClassFactoryFunc)OwnerClass::Create
		, (RegisterPropertyFunc)OwnerClass::RegisterReflection
		);

	template <class OwnerClass, class BaseClass>
	int SupportRTTI<OwnerClass, BaseClass>::s_iVersion(0);

	template <class OwnerClass, class BaseClass> 
	SupportRTTI<OwnerClass, BaseClass>::SupportRTTI()
	{
	}

	template <class OwnerClass, class BaseClass> 
	OwnerClass* SupportRTTI<OwnerClass, BaseClass>::Create()
	{
		return new OwnerClass();
	}

	template <class OwnerClass, class BaseClass> 
	void SupportRTTI<OwnerClass, BaseClass>::RegisterReflection()
	{
	}

	//---------------------------------------------------------------------------------------
#define M_ReflectionInit(CurClassVersion)\
	SetClassVersion(CurClassVersion);\
	int iVersion = 0;

#define M_PropertyStartVersion(Ver) \
	assert(Ver > iVersion); \
	assert(Ver <= GetClassVersion()); \
	iVersion = Ver;

// No Getter/Setter functions, use variable's memory offset
#define	M_RegisterPropertySimple(ValueType, Name, Owner, Category, Description, Flag, Variable)\
	RegisterProperty<ValueType>(#Name, #Owner, #Category, Description, Flag, true, offsetof(Owner, Variable), iVersion, 0, 0, 0)

// Use Getter/Setter functions
#define	      M_RegisterProperty(ValueType, Name, Owner, Category, Description, Flag, Getter, Setter)\
	RegisterProperty<ValueType>(#Name, #Owner, #Category, Description, Flag, false, 0, iVersion, 0, Getter, Setter)

}