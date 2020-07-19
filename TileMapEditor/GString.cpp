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

#include "stdafx.h"
#include "GString.h"

using namespace std;

GString::GString()
: _strBuffer(0)
{
}

GString::~GString()
{
	if(_strBuffer)
	{
		delete [] _strBuffer;
	}
}

void GString::FromUTF8(const char* str)
{
	size_t s = strlen(str);

	static vector<wchar_t> wbuf;
	{
		if(wbuf.size() < s + 1)
			wbuf.resize(s + 1);

		memset(&wbuf[0], 0, wbuf.size() * sizeof(wchar_t));

		UTF8_to_UCS2(str, &wbuf[0], s, s);
	}

	_strIN = &wbuf[0];
}

void GString::FromMbcs(const char* str)
{
	size_t s = strlen(str);

	static vector<wchar_t> wbuf;
	{

		if(wbuf.size() < s + 1)
			wbuf.resize(s + 1);

		memset(&wbuf[0], 0, wbuf.size() * sizeof(wchar_t));

		mbstowcs(&wbuf[0], str, s);
	}

	_strIN = &wbuf[0];
}

//--------------------------------------------------------------------------------------------------
GString operator+(const GString& str1, const GString& str2)
{
	std::wstring ws = str1.Str() + str2.Str();
	return GString(ws);
}

GString operator+(const GString& str, const wchar_t* wstr)
{
	std::wstring ws = str.Str();
	ws += wstr;
	return GString(ws);
}

bool operator==(const GString& str1, const GString& str2)
{
	return (str1.Str() == str2.Str());
}

bool operator==(const GString& str, const wchar_t* wstr)
{
	return (str.Str() == wstr);
}

bool operator!=(const GString& str1, const GString& str2)
{
	return (str1.Str() != str2.Str());
}

bool operator!=(const GString& str, const wchar_t* wstr)
{
	return (str.Str() != wstr);
}

bool operator>(const GString& str1, const GString& str2)
{
	return (str1.Str() != str2.Str());
}

bool operator>(const GString& str, const wchar_t* wstr)
{
	return (str.Str() > wstr);
}

bool operator>=(const GString& str1, const GString& str2)
{
	return (str1.Str() >= str2.Str());
}

bool operator>=(const GString& str, const wchar_t* wstr)
{
	return (str.Str() >= wstr);
}

bool operator<(const GString& str1, const GString& str2)
{
	return (str1.Str() < str2.Str());
}

bool operator<(const GString& str, const wchar_t* wstr)
{
	return (str.Str() < wstr);
}

bool operator<=(const GString& str1, const GString& str2)
{
	return (str1.Str() <= str2.Str());
}

bool operator<=(const GString& str, const wchar_t* wstr)
{
	return (str.Str() <= wstr);
}

std::ostream& operator<<(std::ostream& s, const GString& str)
{
	return s << str.ToUtf8();
}

std::wostream& operator<<(std::wostream& s, const GString& str)
{
	return s << str.Str();
}
