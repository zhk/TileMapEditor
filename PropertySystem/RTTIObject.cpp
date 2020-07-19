/*=============================================================================
*	Copyright (C) 2006-2008, Zhang Kun. All Rights Reserved.
*
*	File name:
*			RTTIObject.cpp
*	Description:
*			
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
=============================================================================*/
#include "stdafx.h"
#include "RTTIObject.h"

using namespace std;
using namespace PropertySys;

RTTIObject::RTTIObject()
{
}

#if 0
//---------------------------------------------------------------------------------------
void RTTIObject::XMLSerializeOut(Cactus::XMLOutStream& xmlOut, const std::string& strRootTag)
{
	xmlOut.NodeBegin(strRootTag);

	xmlOut.AddAttribute("Class", GetRTTI()->GetClassName());
	xmlOut.AddAttribute("Version", GetClassVersion());
	xmlOut.AddAttribute("Name", GetObjectName());

	std::vector<BaseProperty*> Properties;
	GetRTTI()->EnumProperties( Properties );

	for( size_t i = 0; i < Properties.size(); i++ )
	{
		if( !Properties[i]->IsSerializeable() )
			continue;

		switch ( Properties[i]->GetTypeID() )
		{
		case eptPtr:
			{
				TypedProperty<RTTIObject*>* pTypedProperty = (TypedProperty<RTTIObject*>*)Properties[i];
				xmlOut.NodeBegin("Property");
				xmlOut.AddAttribute( "Name", pTypedProperty->GetName() );
				xmlOut.AddAttribute( "Value", "eptPtr" );
				pTypedProperty->GetValue(this)->XMLSerializeOut(xmlOut, strRootTag);
				xmlOut.NodeEnd("Property");
			}
			break;

		case eptBool:
			{
				TypedProperty<bool>* pTypedProperty = (TypedProperty<bool>*)Properties[i];
				xmlOut.NodeBegin("Property");
				xmlOut.AddAttribute( "Name", pTypedProperty->GetName() );
				xmlOut.AddAttribute( "Value", pTypedProperty->GetValueAsString(this) );
				xmlOut.NodeEnd("Property");
			}
			break;

		case eptLong:
			{
				TypedProperty<long>* pTypedProperty = (TypedProperty<long>*)Properties[i];
				xmlOut.NodeBegin("Property");
				xmlOut.AddAttribute( "Name", pTypedProperty->GetName() );
				xmlOut.AddAttribute( "Value", pTypedProperty->GetValueAsString(this) );
				xmlOut.NodeEnd("Property");
			}
			break;

		case eptInt:
			{
				TypedProperty<int>* pTypedProperty = (TypedProperty<int>*)Properties[i];
				xmlOut.NodeBegin("Property");
				xmlOut.AddAttribute( "Name", pTypedProperty->GetName() );
				xmlOut.AddAttribute( "Value", pTypedProperty->GetValueAsString(this) );
				xmlOut.NodeEnd("Property");
			}
			break;

		case eptFloat:
			{
				TypedProperty<float>* pTypedProperty = (TypedProperty<float>*)Properties[i];
				xmlOut.NodeBegin("Property");
				xmlOut.AddAttribute( "Name", pTypedProperty->GetName() );
				xmlOut.AddAttribute( "Value", pTypedProperty->GetValueAsString(this) );
				xmlOut.NodeEnd("Property");
			}
			break;

		case eptDouble:
			{
				TypedProperty<double>* pTypedProperty = (TypedProperty<double>*)Properties[i];
				xmlOut.NodeBegin("Property");
				xmlOut.AddAttribute( "Name", pTypedProperty->GetName() );
				xmlOut.AddAttribute( "Value", pTypedProperty->GetValueAsString(this) );
				xmlOut.NodeEnd("Property");
			}
			break;

		case eptCString:
			{
				TypedProperty<char*>* pTypedProperty = (TypedProperty<char*>*)Properties[i];
				xmlOut.NodeBegin("Property");
				xmlOut.AddAttribute( "Name", pTypedProperty->GetName() );
				xmlOut.AddAttribute( "Value", pTypedProperty->GetValueAsString(this) );
				xmlOut.NodeEnd("Property");
			}
			break;

		case eptStdString:
			{
				TypedProperty<string>* pTypedProperty = (TypedProperty<string>*)Properties[i];
				xmlOut.NodeBegin("Property");
				xmlOut.AddAttribute( "Name", pTypedProperty->GetName() );
				xmlOut.AddAttribute( "Value", pTypedProperty->GetValueAsString(this) );
				xmlOut.NodeEnd("Property");
			}
			break;
		};
	}
	xmlOut.NodeEnd(strRootTag);
}
#endif