#pragma once

//Standard headers
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdarg>
#include <cmath>
#include <typeinfo>

// STL containers
#include <vector>
#include <map>
#include <string>
#include <set>
#include <list>
#include <deque>
#include <queue>
#include <bitset>
#include <stack>

#include <hash_set>
#include <hash_map>

// STL algorithms & functions
#include <algorithm>
#include <functional>
#include <limits>

// C++ Stream stuff
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <iomanip>
#include <strstream>


#if _MSC_VER >= 1600 // VC++ 10.0
#	define HashMap ::std::tr1::unordered_map
#	define HashSet ::std::tr1::unordered_set
#elif _MSC_VER > 1300 && !defined(_STLP_MSVC)
#	define HashMap ::stdext::hash_map
#	define HashSet ::stdext::hash_set
#else
#	define HashMap ::std::hash_map
#	define HashSet ::std::hash_set
#endif


extern "C"
{
#	include <io.h>
#	include <direct.h>
#   include <sys/types.h>
#   include <sys/stat.h>
#	include <sys/timeb.h>
}

namespace PropertySys
{
	typedef unsigned char		uint8;
	typedef unsigned short		uint16;
	typedef unsigned short		ushort;
	typedef unsigned int		uint32;
	typedef unsigned int		uint;
	typedef unsigned long long	uint64;

	typedef signed char			int8;
	typedef signed short		int16;
	typedef signed int			int32;
	typedef long long			int64;
}

#ifdef WIN32
#	undef max
#	undef min
#endif


#pragma warning(disable : 4251 4355)


#include "PropertySystemExports.h"

