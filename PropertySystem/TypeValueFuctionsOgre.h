/*=============================================================================
*	Copyright (C) 2006-2010, Zhang Kun. All Rights Reserved.
*
*	File name:
*			TypeValueFunctionsOgre.h
*	Description:
*			
*	Revision history:
*			2010-6-22	* Created by Zhang Kun
*				Created
=============================================================================*/
#pragma once

namespace PropertySys
{
	//---------------------------------------------------------------------------------------
	template<>
	inline std::string TypeValueToString(const Cactus::String& val)
	{
		return val.c_str();
	}

	template<>
	inline std::string TypeValueToString(const CPoint& val)
	{
		std::ostringstream os;
		os << val.x << ", " << val.y;
		return os.str();
	}

	template<>
	inline std::string TypeValueToString(const CRect& val)
	{
		std::ostringstream os;
		os << val.left << ", " << val.top << ", " << val.right << ", " << val.bottom;
		return os.str();
	}

	template<>
	inline std::string TypeValueToString(const COLORREF& val)
	{
		std::ostringstream os;
		os << (Cactus::uint32)val;
		return os.str();
	}

	//---------------------------------------------------------------------------------------
	template<>
	inline float TypeValueFromString(const std::string& strVal)
	{
		return (float)atof(strVal.c_str());
	}

	template<>
	inline Cactus::String TypeValueFromString(const std::string& strVal)
	{
		return strVal.c_str();
	}

	template<>
	inline CPoint TypeValueFromString(const std::string& strVal)
	{
		float x, y;
		sscanf_s(strVal.c_str(), "%f,%f", &x, &y);
		return CPoint(x, y);
	}

	template<>
	inline CRect TypeValueFromString(const std::string& strVal)
	{
		float l, t, r, b;
		sscanf_s(strVal.c_str(), "%f,%f,%f,%f", &l, &t, &r, &b);
		return CRect(l, t, r, b);
	}

}
