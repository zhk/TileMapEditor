/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			CommonUtil.cpp
*	Description:
*			
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
===============================================================================*/
#pragma	once

#if ( defined( __WIN32__ ) || defined( _WIN32 ) ) && !( defined(USE_COMMONUTIL_STATIC_LIB) || defined(_LIB) )
#	ifdef CommonUtil_EXPORTS
#		define COMMONUTIL_API __declspec(dllexport)
#	else
#		define COMMONUTIL_API __declspec(dllimport)
#	endif
#else
#   define COMMONUTIL_API
#endif
