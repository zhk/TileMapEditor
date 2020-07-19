#include "StdAfx.h"
#include "ToolManager.h"

#include "ToolSelect.h"
#include "ToolBrush.h"
#include "ToolTerrain.h"

using namespace Cactus;

ToolManager* Cactus::Singleton<ToolManager>::s_pSingleton = 0;

ToolManager::ToolManager()
: _eCurToolType(eToolSelect)
, _pCurMap(0)
, _pCurLayer(0)
{
	_tools[eToolSelect]	= new ToolSelect;
	_tools[eToolBrush]	= new ToolBrush;
	_tools[eToolRegion]	= new ToolRegion;

	_tools[_eCurToolType]->OnTurnOn();
}

ToolManager::~ToolManager()
{
	for (TypToolMapType::iterator it = _tools.begin(); it != _tools.end(); ++it)
	{
		delete it->second;
	}
}

ToolBase* ToolManager::GetTool(EToolType e)
{
	return _tools[e];
}

ToolBase* ToolManager::SelectTool(EToolType e)
{
	if (e != _eCurToolType)
	{
		_tools[_eCurToolType]->OnTurnOff();

		_eCurToolType = e;

		_tools[_eCurToolType]->OnTurnOn();
	}

	return _tools[_eCurToolType];
}

ToolBase* ToolManager::GetCurrentTool()
{
	return _tools[_eCurToolType];
}

void ToolManager::OnNewMap(Map* pMap)
{
	for (TypToolMapType::iterator it = _tools.begin(); it != _tools.end(); ++it)
	{
		it->second->OnNewMap(_pCurMap);
	}

	SelectTool(eToolSelect);

	_pCurMap = pMap;
}

void ToolManager::OnMapClose(Map* pMap)
{
	_tools[_eCurToolType]->OnMapClose(pMap);
	_pCurMap = 0;
}

void ToolManager::OnCurrentMapLayer(MapLayer* pLayer)
{
	_tools[_eCurToolType]->OnCurrentMapLayer(pLayer);
	_pCurLayer = pLayer;
}
