/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			CommonTemplate.h
*	Description:
*			
*	Revision history:
*			2009-2-83	* Created by Zhang Kun
*				Created
===============================================================================*/
#pragma once

namespace Cactus
{
	//----------------------------------------------------------------------------
	// Compile time check
	template<bool> struct CompileTimeError;
	template<> struct CompileTimeError<true> {};

	//----------------------------------------------------------------------------
	// BKDR Hash Function
	inline unsigned int BKDRHash(const char* str)
	{
		unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
		unsigned int hash = 0; 

		while (*str)
		{
			hash = hash * seed + (*str++);
		} 

		return (hash & 0x7FFFFFFF);
	}


	template <class T, class U>
	class Conversion
	{
		static char	test(U);
		static int	test(...);
		static T	makeT();
	public:
		enum{ exists = sizeof( test(makeT()) ) == sizeof(char) };
		enum{ exists2Way = exists && Conversion<U, T>::exists };
		enum { sameType = false };
	};

	template <class T>
	struct Conversion<T, T>    
	{
		enum { exists = 1, exists2Way = 1, sameType = 1 };
	};

	template <class T>
	struct Conversion<void, T>    
	{
		enum { exists = 0, exists2Way = 0, sameType = 0 };
	};

	template <class T>
	struct Conversion<T, void>    
	{
		enum { exists = 0, exists2Way = 0, sameType = 0 };
	};

	template <>
	struct Conversion<void, void>    
	{
	public:
		enum { exists = 1, exists2Way = 1, sameType = 1 };
	};

	template <class T, class U>
	struct SuperSubclass
	{
		enum { value = (Conversion<const volatile U*, const volatile T*>::exists &&
			!Conversion<const volatile T*, const volatile void*>::sameType) };
	};

	template<class T,class U>
	struct SuperSubclassStrict
	{
		enum { value = (Conversion<const volatile U*, const volatile T*>::exists &&
			!Conversion<const volatile T*, const volatile void*>::sameType &&
			!Conversion<const volatile T*, const volatile U*>::sameType) };
	};
}

#define M_STATIC_CHECK(expr, msg) \
{ Cactus::CompileTimeError<((expr) != 0)> Error_##msg; (void)Error_##msg; } 


#define M_SuperSubClass(T, U) \
	Cactus::SuperSubclass<T, U>::value


#define M_SuperSubClass_Strict(T, U) \
	Cactus::SuperSubclassStrict<T, U>::value
