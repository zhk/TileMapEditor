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

#pragma once

/**
GString means : Game String, a string class for game.
Use UCS-2 as the string code point(Term : code point = character).
Assume std::string and char* are in MBCS if no encoding is specified.
Assume std::wstring, wchar_t* and unsigned short are in UCS-2.
*/
class GString
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

	GString(const std::string& std_str, EGStringEncoding encode = eSE_Mbcs)
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

	GString(const std::wstring& std_wstr)
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

	GString&	operator=(const std::string& std_str)
	{
		FromMbcs(std_str.c_str());
		return *this;
	}

	GString&	operator=(const char* str)
	{
		FromMbcs(str);
		return *this;
	}

	GString&	operator=(const std::wstring& std_wstr)
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
	Get the internal std::wstring.
	*/
	std::wstring&	Str()
	{
		return _strIN;
	}

	const std::wstring&	Str() const
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
			delete [] _strBuffer;

		size_t s = _strIN.size() * 2;
		_strBuffer = new char [s + 1];
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
			delete [] _strBuffer;

		size_t s = _strIN.size() * 3;
		_strBuffer = new char [s + 1];
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

private:
	mutable char*	_strBuffer;
	std::wstring	_strIN;
	unsigned int	_uiOldHashCode;
};

GString 			operator+(const GString& str1, const GString& str2);
GString				operator+(const GString& str, const wchar_t* wstr);

bool				operator==(const GString& str1, const GString& str2);
bool				operator==(const GString& str, const wchar_t* wstr);

bool				operator!=(const GString& str1, const GString& str2);
bool				operator!=(const GString& str, const wchar_t* wstr);

bool				operator>(const GString& str1, const GString& str2);
bool				operator>(const GString& str, const wchar_t* wstr);

bool				operator>=(const GString& str1, const GString& str2);
bool				operator>=(const GString& str, const wchar_t* wstr);

bool				operator<(const GString& str1, const GString& str2);
bool				operator<(const GString& str, const wchar_t* wstr);

bool				operator<=(const GString& str1, const GString& str2);
bool				operator<=(const GString& str, const wchar_t* wstr);

std::ostream&	operator<<(std::ostream& s, const GString& str);
std::wostream&	operator<<(std::wostream& s, const GString& str);

