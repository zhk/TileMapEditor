/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			GString.h
*	Description:
*			
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
===============================================================================*/

#pragma once

#include "PreInclude.h"

namespace Cactus
{
	class GString;
	typedef Cactus::vector<GString>::type			GStringVector;

	/**
	GString means : Game String, string class for game.
	Use UCS-2 as the string code point(Term : code point = character).
	Assume Cactus::String and char* are in MBCS if no encoding is specified.
	Assume Cactus::WString, wchar_t* and unsigned short are in UCS-2.
	*/
	class COMMONUTIL_API GString : public Object
	{
	public:

		enum EGStringEncoding
		{
			eSE_Ucs2,
			eSE_Utf8,
			eSE_Mbcs,
			eSE_Ucs4,
			eSE_Max
		};

		GString();
		~GString();

		GString(const Cactus::String& std_str, EGStringEncoding encode = eSE_Mbcs)
			: _strBuffer(0)
			, _uiOldHashCode(0)
		{
			if(eSE_Mbcs == encode)
			{
				FromMbcs(std_str.c_str());
			}
			else if(eSE_Utf8 == encode)
			{
				FromUTF8(std_str.c_str());
			}
		}

		GString(const char* str, EGStringEncoding encode = eSE_Mbcs)
			: _strBuffer(0)
			, _uiOldHashCode(0)
		{
			if(eSE_Mbcs == encode)
			{
				FromMbcs(str);
			}
			else if(eSE_Utf8 == encode)
			{
				FromUTF8(str);
			}
		}

		GString(const Cactus::WString& std_wstr)
			: _strBuffer(0)
			, _uiOldHashCode(0)
		{
			_strIN = std_wstr;
		}

		GString(const wchar_t* wstr)
			: _strBuffer(0)
			, _uiOldHashCode(0)
		{
			_strIN = wstr;
		}

		GString&	operator=(const Cactus::String& std_str)
		{
			FromMbcs(std_str.c_str());
			return *this;
		}

		GString&	operator=(const char* str)
		{
			FromMbcs(str);
			return *this;
		}

		GString&	operator=(const Cactus::WString& std_wstr)
		{
			_strIN = std_wstr;
			return *this;
		}

		GString&	operator=(const wchar_t* wstr)
		{
			_strIN = wstr;
			return *this;
		}

		GString&	operator=(const GString& Str)
		{
			_strIN = Str.Str();
			return *this;
		}

		GString&	operator+=(const GString& Str)
		{
			_strIN += Str.Str();
			return *this;
		}

		GString&	operator+=(const wchar_t* wstr)
		{
			_strIN += wstr;
			return *this;
		}

		/**
		Get the internal Cactus::WString.
		*/
		Cactus::WString&	Str()
		{
			return _strIN;
		}

		const Cactus::WString&	Str() const
		{
			return _strIN;
		}

		/**
		Convert a C style string encoded in UTF8 into another C style string encoded in UCS2.
		*/
		size_t			UTF8_to_UCS2(const char* src, wchar_t* dest, size_t dest_len, size_t src_len = 0) const
		{
			if (src_len == 0)
				src_len = strlen(src);

			size_t destCapacity = dest_len;

			// while there is data in the source buffer, and space in the dest buffer
			for (size_t idx = 0; ((idx < src_len) && (destCapacity > 0));)
			{
				wchar_t			cp;
				unsigned char	cu = src[idx++];

				if (cu < 0x80)
				{
					cp = (wchar_t)(cu);
				}
				else if (cu < 0xE0)
				{
					cp = ((cu & 0x1F) << 6);
					cp |= (src[idx++] & 0x3F);
				}
				else if (cu < 0xF0)
				{
					cp = ((cu & 0x0F) << 12);
					cp |= ((src[idx++] & 0x3F) << 6);
					cp |= (src[idx++] & 0x3F);
				}
				else
				{
					cp = ((cu & 0x07) << 18);
					cp |= ((src[idx++] & 0x3F) << 12);
					cp |= ((src[idx++] & 0x3F) << 6);
					cp |= (src[idx++] & 0x3F);
				}

				*dest++ = cp;
				--destCapacity;
			}

			return dest_len - destCapacity;
		}


		/**
		Build GString from C style string encoded in UTF8.
		*/
		void			FromUTF8(const char* str);

		/**
		Build GString from C style string encoded in MBCS.
		*/
		void			FromMbcs(const char* str);

		/**
		Return a buffer encoded in MBCS of the GString.
		*/
		const char*		ToMbcs() const
		{
			if(_strBuffer && _uiOldHashCode == GetHashCode())
				return _strBuffer;

			if(_strBuffer)
				Mem_Free(_strBuffer);

			size_t s = _strIN.size() * 2;
			_strBuffer = Mem_Alloc_T(char, s + 1);
			memset(_strBuffer, 0, s + 1);
			wcstombs(_strBuffer, _strIN.c_str(), s);
			
			return _strBuffer;
		}

		/**
		Return a buffer encoded in UTF-8 of the GString.
		*/
		const char*		ToUtf8() const
		{
			if(_strBuffer && _uiOldHashCode == GetHashCode())
				return _strBuffer;

			if(_strBuffer)
				Mem_Free(_strBuffer);

			size_t s = _strIN.size() * 3;
			_strBuffer = Mem_Alloc_T(char, s + 1);
			char* pstr = _strBuffer;
			memset(_strBuffer, 0, s + 1);

			for(size_t i = 0; i < _strIN.size(); ++i)
			{
				wchar_t cp = _strIN[i];

				if (cp < 0x80)
				{
					*pstr++ = (char)cp;
					--s;
				}
				else if (cp < 0x0800)
				{
					*pstr++ = (char)((cp >> 6) | 0xC0);
					*pstr++ = (char)((cp & 0x3F) | 0x80);
					s -= 2;
				}
				else if (cp < 0x10000)
				{
					*pstr++ = (char)((cp >> 12) | 0xE0);
					*pstr++ = (char)(((cp >> 6) & 0x3F) | 0x80);
					*pstr++ = (char)((cp & 0x3F) | 0x80);
					s -= 3;
				}
				//else
				//{
				//	*pstr++ = (char)((cp >> 18) | 0xF0);
				//	*pstr++ = (char)(((cp >> 12) & 0x3F) | 0x80);
				//	*pstr++ = (char)(((cp >> 6) & 0x3F) | 0x80);
				//	*pstr++ = (char)((cp & 0x3F) | 0x80);
				//	s -= 4;
				//}
			}

			return _strBuffer;
		}

		/**
		Convert all characters in the GString to uppercase.
		*/
		void			ToUpper()
		{
			for(size_t t = 0; t < _strIN.size(); ++t)
			{
				wchar_t cp = _strIN[t];
				if(cp >= L'a' && cp <= L'z')
				{
					_strIN[t] -= L'a' - L'A';
				}
			}
		}

		/**
		Convert all characters in the GString to lowercase.
		*/
		void			ToLower()
		{
			for(size_t t = 0; t < _strIN.size(); ++t)
			{
				wchar_t cp = _strIN[t];
				if(cp >= L'A' && cp <= L'Z')
				{
					_strIN[t] += L'a' - L'A';
				}
			}
		}

		/**
		Replaces each format item in a specified GString with the text equivalent of
		a corresponding variable's value.
		*/
		void			Format(const wchar_t* fmt, ...);
		
		/**
		Removes all occurrences of white space characters from the beginning and end of this instance.
		*/
		bool			Trim();

		/**
		Returns the hash code for this instance.
		*/
		unsigned int	GetHashCode() const
		{
			unsigned int h = 0, g;
			
			const wchar_t* p = _strIN.c_str();
			while(*p)
			{
				wchar_t ch = *p++;
				h = (h << 4) + ch;
				if((g = (h & 0xF0000000)))
				{
					h = h ^ (g >> 24);
					h = h ^ g;
				}
			}

			return h;
		}

		bool			IsNumber();

		bool			Parse(int& value);
		bool			Parse(bool& value);
		bool			Parse(wchar_t& value);
		bool			Parse(float& value);

		bool			Match(const GString& strValue, GString& strOut);
		bool			Split(GStringVector& Groups, const GString& strDelimiter= L",");

		/**
		something like : Data/Config/config.xml
		*/
		bool			ParseFileName(GString& strOutPath		// Data/Config/
							, GString& strOutName				// config
							, GString& strOutExt);				// xml

		/**
		something like : (255, 255, 128)
		*/
		bool			ParseIntGroup3(int& x, int& y, int& z);

		/**
		something like : (12.0,123.0)
		*/
		bool			ParseFloatGroup2(float& x, float& y);


		/**
		something like : (12.0,123.0,1234.0)
		*/
		bool			ParseFloatGroup3(float& x, float& y, float& z);

		/**
		something like : (12.0,123.0,1234.0,12345.0)
		*/
		bool			ParseFloatGroup4(float& x, float& y, float& z, float& w);

		/**
		something like : <string0><strDelimiter><string1>...
		*/
		bool			ParseStr2StrVector(GStringVector& Groups, const GString& strDelimiter);

	private:
		mutable char*	_strBuffer;
		Cactus::WString	_strIN;
		unsigned int	_uiOldHashCode;
	};

	COMMONUTIL_API GString 			operator+(const GString& str1, const GString& str2);
	COMMONUTIL_API GString			operator+(const GString& str, const wchar_t* wstr);

	COMMONUTIL_API bool				operator==(const GString& str1, const GString& str2);
	COMMONUTIL_API bool				operator==(const GString& str, const wchar_t* wstr);

	COMMONUTIL_API bool				operator!=(const GString& str1, const GString& str2);
	COMMONUTIL_API bool				operator!=(const GString& str, const wchar_t* wstr);

	COMMONUTIL_API bool				operator>(const GString& str1, const GString& str2);
	COMMONUTIL_API bool				operator>(const GString& str, const wchar_t* wstr);

	COMMONUTIL_API bool				operator>=(const GString& str1, const GString& str2);
	COMMONUTIL_API bool				operator>=(const GString& str, const wchar_t* wstr);

	COMMONUTIL_API bool				operator<(const GString& str1, const GString& str2);
	COMMONUTIL_API bool				operator<(const GString& str, const wchar_t* wstr);

	COMMONUTIL_API bool				operator<=(const GString& str1, const GString& str2);
	COMMONUTIL_API bool				operator<=(const GString& str, const wchar_t* wstr);

	COMMONUTIL_API std::ostream&	operator<<(std::ostream& s, const GString& str);
	COMMONUTIL_API std::wostream&	operator<<(std::wostream& s, const GString& str);
}

/// convert to MBCS display string, source is utf8
#define _D(str) (Cactus::GString(str, Cactus::GString::eSE_Utf8).ToMbcs())