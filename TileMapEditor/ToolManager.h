#pragma once

enum EToolType
{
	eToolSelect,
	eToolBrush,
	eToolRegion,
	eToolMax
};

class ToolBase;
class Map;
class MapLayer;

class ToolManager : public Cactus::Singleton<ToolManager>
{
public:
	ToolManager();
	~ToolManager();

	void	OnNewMap(Map* pMap);
	void	OnMapClose(Map* pMap);
	void	OnCurrentMapLayer(MapLayer* pLayer);

	Map*				GetMap(){ return _pCurMap; }

	ToolBase*			SelectTool(EToolType e);
	ToolBase*			GetCurrentTool();
	EToolType			GetCurrentToolType(){ return _eCurToolType; }

	ToolBase*			GetTool(EToolType e);

protected:

	Map*				_pCurMap;
	MapLayer*			_pCurLayer;
	EToolType			_eCurToolType;

	typedef Cactus::map<EToolType, ToolBase*>::type		TypToolMapType;
	TypToolMapType		_tools;

};
