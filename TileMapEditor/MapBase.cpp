#include "stdafx.h"
#include "MapBase.h"

using namespace Cactus;
using namespace PropertySys;


void MapBaseObject::RegisterReflection()
{
	BaseProperty* pProp = 0;

	M_ReflectionInit(0);

	pProp = M_RegisterPropertySimple(Cactus::String, Name, MapBaseObject, MapBaseObject, "¶ÔÏóÃû×Ö.", BaseProperty::eDefault, _strName);
}

void MapBaseObject::OnPropertyChanged(const std::string& propName)
{
}