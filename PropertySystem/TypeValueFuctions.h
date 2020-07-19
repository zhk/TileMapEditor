/*=============================================================================
*	Copyright (C) 2006-2008, Zhang Kun. All Rights Reserved.
*
*	File name:
*			TypeValueFunctions.h
*	Description:
*			
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
=============================================================================*/
#pragma once

namespace PropertySys
{

	//---------------------------------------------------------------------------------------
	template<class ValueType>
	inline ValueType TypeGetValue(RTTIObject* p, size_t szOffset)
	{
		return *(ValueType*)((unsigned char*)p + szOffset);
	}

	template<class ValueType>
	inline void TypeSetValue(RTTIObject* p, size_t szOffset, const ValueType& Value)
	{
		*(ValueType*)((unsigned char*)p + szOffset) = Value;
	}

	//---------------------------------------------------------------------------------------
	// Example : Partial Template Specialization for special type
	//template<>
	//inline void TypeSetValue(RTTIObject* p, size_t szOffset, const std::string& Value)
	//{
	//	*(std::string*)((unsigned char*)p + szOffset) = Value;
	//}


	//---------------------------------------------------------------------------------------
	template<class ValueType>
	inline std::string TypeValueToString(const ValueType& val)
	{
		std::ostringstream os;
		os << val;
		return os.str();
	}

	//---------------------------------------------------------------------------------------
	// This function is just a template specification, do not use it.
	template<class ValueType>
	inline ValueType TypeValueFromString(const std::string& strVal)
	{
		// New type added? You need add a new TypeValueFromString() for the type.
		assert(false);

		ValueType val;
		return val;
	}

	template<>
	inline bool TypeValueFromString(const std::string& strVal)
	{
		return (strVal == "1"
			||
			strVal == "true"
			||
			strVal == "True"
			||
			strVal == "TRUE"
			);
	}

	template<>
	inline char TypeValueFromString(const std::string& strVal)
	{
		return atoi(strVal.c_str());
	}


	template<>
	inline short TypeValueFromString(const std::string& strVal)
	{
		return atoi(strVal.c_str());
	}

	template<>
	inline int TypeValueFromString(const std::string& strVal)
	{
		return atoi(strVal.c_str());
	}

	template<>
	inline long TypeValueFromString(const std::string& strVal)
	{
		return atol(strVal.c_str());
	}
}
