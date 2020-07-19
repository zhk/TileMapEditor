/*=============================================================================
*	Copyright (C) 2006-2008, Zhang Kun. All Rights Reserved.
*
*	File name:
*			BaseProperty.cpp
*	Description:
*			
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
=============================================================================*/

#include "stdafx.h"
#include "BaseProperty.h"

using namespace PropertySys;

void BaseProperty::SetValueSpecify(EValueSpecify e, const std::string& str)
{
	_eValueSpecify = e;

	std::string strTmp = str;

	char* token_next = 0;
	char* token = strtok_s((char*)strTmp.c_str(), ";", &token_next);

	while( token != 0 )
	{
		_ValueSpecifies.push_back(token);

		token = strtok_s( 0, ";", &token_next);
	}

}
