/*=============================================================================
*	Copyright (C) 2006-2008, Zhang Kun. All Rights Reserved.
*
*	File name:
*			BaseProperty.h
*	Description:
*			
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
=============================================================================*/
#pragma once

#include "PreInclude.h"

namespace PropertySys
{

	enum ePropertyType
	{
		eptBool,
		eptByte,			// char
		eptShort,
		eptInt,
		eptLong,
		eptPtr,				// pointer

		eptFloat,
		eptString,			//Cactus String

		eptPoint,			//MFC CPoint
		eptRect,			//MFC CRect

		eptMAX
	};

	// This template class will associate compile-time types with unique enum members.
	template <class ValueType>
	class PropertyType
	{
	public :

		// Returns type ID associated with the template type.
		static	ePropertyType	GetTypeID();

	private:

		static	ePropertyType    s_TypeID;

	};

	template<class ValueType>
	ePropertyType PropertyType<ValueType>::GetTypeID()
	{
		return s_TypeID;
	}

	template<class ValueType> ePropertyType PropertyType<ValueType>::s_TypeID	= eptPtr;
	template<> ePropertyType PropertyType<bool>::s_TypeID				= eptBool;
	template<> ePropertyType PropertyType<char>::s_TypeID				= eptByte; 
	template<> ePropertyType PropertyType<short>::s_TypeID				= eptShort; 
	template<> ePropertyType PropertyType<int>::s_TypeID				= eptInt; 
	template<> ePropertyType PropertyType<long>::s_TypeID				= eptLong; 

	template<> ePropertyType PropertyType<float>::s_TypeID				= eptFloat;
	template<> ePropertyType PropertyType<Cactus::String>::s_TypeID		= eptString;
	template<> ePropertyType PropertyType<CPoint>::s_TypeID				= eptPoint;
	template<> ePropertyType PropertyType<CRect>::s_TypeID				= eptRect;
	
	enum EValueSpecify
	{
		eSpecifyNone,
		eValueRange,	// value must in a range
		eValueList,		// value must in a list
		eFilePathName,	// value is disk file name
		eValueColor		// value is color
	};


	// Base class for all properties.
	class PROPERTYSYSTEM_API BaseProperty
	{
	public:

		enum EPropertyFlag
		{
			eReadOnly				= 1 << 0,		// property is read only
			eSerializable			= 1 << 1,		// property can be saved into to file
			eEditable				= 1 << 2,		// property can be seen in editor
			eNetworkSerializable	= 1 << 4,		// do synchronization through network

			eDefault				= eSerializable | eEditable
		};

		// Constructor. Takes in property name.
		BaseProperty()
		{
			_eValueSpecify = eSpecifyNone;
		}

		// Returns the type of this property.
		virtual ePropertyType	GetTypeID() const = 0;
	    
		size_t				GetOffset() const { return _szOffset; }

		std::string			GetName() const { return _strName; }

		std::string			GetCategory() const { return _strCategory; }

		std::string			GetDescription()const { return _strDescription; }

		std::string			GetOwner() const { return _strOwner; }

		unsigned int		GetFlag() const { return _uFlag; }

		int					GetStartVersion() const { return _iStartClassVersion; }

		bool				IsSerializeable() const { return (_uFlag & eSerializable) != 0; }

		bool				IsNetworkSerializeable() const { return (_uFlag & eNetworkSerializable) != 0; }

		bool				IsReadOnly() const { return (_uFlag & eReadOnly) != 0; }

		bool				IsEditable() const { return (_uFlag & eEditable) != 0; }

		bool				IsCommonProperty() const { return _bCommonProperty; }

		EValueSpecify		GetValueSpecify(){ return _eValueSpecify; }
		void				SetValueSpecify(EValueSpecify e, const std::string& str);

		const std::vector<std::string>&	GetSpeValues(){ return _ValueSpecifies; }
	protected:

		std::string			_strName;				// ������
		unsigned int		_uFlag;					// ���
		size_t				_szOffset;				// ƫ����
		std::string			_strCategory;			// ���
		std::string			_strOwner;				// ��������
		std::string			_strDescription;		// ����
		int					_iStartClassVersion;	// ֧�ֵ����������Ͱ汾
		bool				_bCommonProperty;		// �Ƿ��Ǽ�����
		EValueSpecify		_eValueSpecify;			// ȡֵ��Χ���б��
		std::string			_strValueSpecify;		// ȡֵ����<s;e>��<a;b;c;d>

		std::vector<std::string>	_ValueSpecifies;
	};

}

