#include "StdAfx.h"
#include "MapLayer.h"
#include "ResourceManager.h"
#include "Map.h"
#include "ResourceGameObject.h"

#include "MainFrm.h"
#include "TileMapEditorView.h"

using namespace Cactus;
using namespace PropertySys;

//---------------------------------------------------------------------------
bool STile_Diamond_less(const STile* p1, const STile* p2)
{
#if 0
	Resource* pRes = ResourceManager::getSingleton().GetResource(_strResGroup);
	if (pRes)
	{
		if(pRes->GetResourceType() == eResTypeArt)
		{
			return _ptGrid.x + _ptGrid.y < right._ptGrid.x + right._ptGrid.y;
		}
		else
		{
			//ResourceGameObjectGroup* pGameObjectGroup = (ResourceGameObjectGroup*)pRes;
			//ResourceGameObject* pObject = pGameObjectGroup->GetGameObject(_strResItemID);
			//if (pObject)
			//{
			//	return pObject->operator < (right);
			//}
		}
	}

	return _ptGrid.x + _ptGrid.y < right._ptGrid.x + right._ptGrid.y;
#endif

	return p1->_ptGrid.x + p1->_ptGrid.y < p2->_ptGrid.x + p2->_ptGrid.y;
}

bool STile_Triangle_less(const STile* p1, const STile* p2)
{
	return p1->_ptGrid.y < p2->_ptGrid.y;
}

//---------------------------------------------------------------------------
MapLayer::MapLayer()
: _bVisible(true)
, _eLayerType(eMapLayerTiles)
{
}

MapLayer::~MapLayer()
{
}

void MapLayer::Init(const Cactus::String& strName, Map* pParent)
{
	_pParentMap	= pParent;
	_strName	= strName;
}

void MapLayer::SetVisible(bool b)
{
	_bVisible = b;

	_pParentMap->SetModified();
}


void MapLayer::RegisterReflection()
{
	BaseProperty* pProp = 0;

	M_ReflectionInit(0);

	pProp = M_RegisterPropertySimple(bool, Enable, MapLayer, Map, "是否激活，不激活将不会导出到游戏地图.", BaseProperty::eDefault, _bEnable);
	pProp = M_RegisterPropertySimple(bool, Visible, MapLayer, Map, "是否显示.", BaseProperty::eDefault, _bVisible);
}

void MapLayer::OnPropertyChanged(const std::string& propName)
{
	if (propName == "DrawGrid")
	{
		CView* pView = ((CFrameWnd*)AfxGetMainWnd())->GetActiveView(); 
		pView->Invalidate(TRUE);
	}
}


bool MapLayer::ToolHitTest(CPoint ptPixel, CPoint& ptGrid, CRect& rcPixel)
{
	bool bInRegion = _pParentMap->GetGridCoord(ptPixel, ptGrid);
	if (bInRegion)
	{
		rcPixel = _pParentMap->GetPixelCoordRect(ptGrid);
	}
	else
	{
		ptGrid.x	= -1;
		ptGrid.y	= -1;
	}

	return bInRegion;
}


void MapLayer::UpdateTileInfoInPanelMapLayer(STile* pTile, ETileOp e/* = eTileOpAdd*/)
{
	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST( CMainFrame, AfxGetMainWnd() );
	if (pMainFrame)
	{
		//更新地层列表
		LayerView* pView = pMainFrame->GetMapLayerView(); 
		if (pView)
		{
			if (e == eTileOpAdd)
				pView->AddTileInfo(pTile);
			else if (e == eTileOpRemove)
				pView->RemoveTileInfo(pTile);
			else
				pView->UpdateTileInfo(pTile);
		}
	}
}


void MapLayer::UpdateTileVisual(STile* pTile, bool bEnsureVisible/* = false*/)
{
	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST( CMainFrame, AfxGetMainWnd() );
	if (pMainFrame)
	{
		//更新绘制
		CTileMapEditorView* pMapView = (CTileMapEditorView*)pMainFrame->GetActiveView();
		CRect rcPixelGrid = _pParentMap->GetPixelCoordRect(pTile->_ptGrid);

		Resource* pRes = ResourceManager::getSingleton().GetResourceArtOrGOGroup(pTile->_strResGroup);
		if (!pRes)
		{
			//Log_Error("资源组 '" << pTile->_strResGroup << "' 不存在！");
			return;
		}

		CRect rcDest = pRes->GetResItemBoundingRect(rcPixelGrid, _pParentMap->GetGridType(), pTile->_strResItemID);

		if (bEnsureVisible)
			pMapView->EnsureRectVisible(rcDest);

		pMapView->LogicInvalidate(rcDest);
	}

	_pParentMap->SetModified();
}

void MapLayer::CheckResourceUsageForTile(STile* pTile, StringResRefInfoMapType& mapGroupResRefInfo, StringResRefInfoMapType& groupSubResRefCountMap)
{
	//加入本地图资源使用信息
	StringResRefInfoMapType::iterator itGroup = mapGroupResRefInfo.find(pTile->_strResGroup);
	if ( itGroup == mapGroupResRefInfo.end() )
	{
		SubResRefCountMapType subResCount;
		subResCount[pTile->_strResItemID] = 1;

		mapGroupResRefInfo[pTile->_strResGroup] = subResCount;
	}
	else
	{
		SubResRefCountMapType::iterator itSubItem = itGroup->second.find(pTile->_strResItemID);
		if ( itSubItem == itGroup->second.end() )
		{
			itGroup->second[pTile->_strResItemID] = 1;
		}
		else
		{
			itGroup->second[pTile->_strResItemID] = itSubItem->second + 1;
		}
	}

	//加入全局资源使用信息
	itGroup = groupSubResRefCountMap.find(pTile->_strResGroup);
	if ( itGroup == groupSubResRefCountMap.end() )
	{
		SubResRefCountMapType subResCount;
		subResCount[pTile->_strResItemID] = 1;

		groupSubResRefCountMap[pTile->_strResGroup] = subResCount;
	}
	else
	{
		SubResRefCountMapType::iterator itSubItem = itGroup->second.find(pTile->_strResItemID);
		if ( itSubItem == itGroup->second.end() )
		{
			itGroup->second[pTile->_strResItemID] = 1;
		}
		else
		{
			itGroup->second[pTile->_strResItemID] = itSubItem->second + 1;
		}
	}

	//处理GO引用情况
	ResourceGameObjectGroup* pGOResGroup = ResourceManager::getSingleton().GetResourceGameObjectGroup(pTile->_strResGroup);
	if (pGOResGroup)
	{
		//GO依赖的美术资源组
		String artGroup = pGOResGroup->GetResArtKey();
		ResourceGameObject*	pGO = pGOResGroup->GetGameObject(pTile->_strResItemID);

		String strResID;
		if (pGO)
			strResID = pGO->GetResID();
		else
			return;

		itGroup = groupSubResRefCountMap.find(artGroup);
		if ( itGroup == groupSubResRefCountMap.end() )
		{
			SubResRefCountMapType subResCount;
			subResCount[strResID] = 1;

			groupSubResRefCountMap[artGroup] = subResCount;
		}
		else
		{
			SubResRefCountMapType::iterator itSubItem = itGroup->second.find(strResID);
			if ( itSubItem == itGroup->second.end() )
			{
				itGroup->second[strResID] = 1;
			}
			else
			{
				itGroup->second[strResID] = itSubItem->second + 1;
			}
		}
	}

}
