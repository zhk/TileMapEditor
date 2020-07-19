/*=============================================================================
*	Copyright (C) 2006-2008, Zhang Kun. All Rights Reserved.
*
*	File name:
*			RTTIObject.h
*	Description:
*			
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
=============================================================================*/
#pragma once

#include "PreInclude.h"
#include "RTTIClass.h"

//#include "../CommonUtil/XMLOutStream.h"

namespace PropertySys
{

	// NullClass is an empty class with no attached RTTI information. We use this class as an ancestor
	// to our RootClass which is the true ultimate ancestor for all our classes. Without the null class
	// we would have no template parameter for RootClass's ancestor. NullClass itself has no RTTI support.
	class PROPERTYSYSTEM_API NullClass
	{
	protected :

		// It is necessary to implement this to avoid compilation errors in the template RTTI code
		// In any case no RTTI support is provided from this class
		static inline RTTI* GetClassRTTI()			{ return 0;	}
	};

	// Ultimate ancestor for all classes. RTTIObject implements no specific functionality, apart from
	// the fact it provides the ultimate ancestor RTTI for all other classes.
	class PROPERTYSYSTEM_API RTTIObject : public SupportRTTI<RTTIObject, NullClass>
	{
	public:
		RTTIObject();
		virtual ~RTTIObject() {}

		virtual void			OnPropertyChanged(const std::string& propName){}

		inline void				CalcName()
		{
			std::ostringstream os;
			RTTI* pRTTI = GetRTTI();
			os << pRTTI->GetClassName() << "_I" << std::setfill('0') << std::setw(6) << pRTTI->IncreaseObjectCount();
			_strName = os.str();
		}

		std::string				GetName() const
		{
			return _strName;
		}

		//void		XMLSerializeOut(Cactus::XMLOutStream& xmlOut, const std::string& strRootTag);

	protected:
		std::string		_strName;
	};

}
