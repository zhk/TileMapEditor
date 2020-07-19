#pragma	once

#if defined(USE_PROPERTYSYSTEM_STATIC_LIB) || defined(_LIB)
#	define PROPERTYSYSTEM_API
#else
#	ifdef PROPERTYSYSTEM_EXPORTS
#		define PROPERTYSYSTEM_API __declspec(dllexport)
#	else
#		define PROPERTYSYSTEM_API __declspec(dllimport)
#	endif
#endif