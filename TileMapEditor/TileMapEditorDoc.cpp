
// TileMapEditorDoc.cpp : CTileMapEditorDoc 类的实现
//

#include "stdafx.h"
#include "TileMapEditor.h"
#include "TileMapEditorDoc.h"

#include "DialogFileNew.h"
#include "MainFrm.h"
#include "MapView.h"
#include "TileMapEditorView.h"

#include "ToolManager.h"
#include "MapLayer.h"
#include "MapLayerBackground.h"
#include "MapLayerLogic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTileMapEditorDoc

IMPLEMENT_DYNCREATE(CTileMapEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CTileMapEditorDoc, CDocument)
	ON_COMMAND(ID_FILE_NEW, &CTileMapEditorDoc::OnFileNew)
END_MESSAGE_MAP()


// CTileMapEditorDoc 构造/析构

CTileMapEditorDoc::CTileMapEditorDoc()
{
}

CTileMapEditorDoc::~CTileMapEditorDoc()
{
}

BOOL CTileMapEditorDoc::IsModified()
{
	bool b = _theMap.IsModified();
	return b;
}

void CTileMapEditorDoc::SetModifiedFlag(BOOL bModified/* = TRUE*/)
{
	_theMap.SetModified(bModified == TRUE);
}


BOOL CTileMapEditorDoc::OnNewDocument()
{
	SetTitle("NewTileMap");

	if (!CDocument::OnNewDocument())
		return FALSE;

	_theMap.CalculateRegionInfo();

	ToolManager::getSingleton().OnNewMap(&_theMap);

	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd() );
	pMainFrame->GetMapView()->Reset();
	pMainFrame->GetPropertyWnd()->Reset();

	pMainFrame->GetMapView()->SetMapObject(&_theMap);

	return TRUE;
}

// CTileMapEditorDoc 诊断

#ifdef _DEBUG
void CTileMapEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTileMapEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTileMapEditorDoc 命令

BOOL CTileMapEditorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	{
		char buffer[MAX_PATH];
		::GetModuleFileName(0, buffer, MAX_PATH);
		std::string strModulePath = buffer;

		strModulePath.erase(strModulePath.find_last_of('\\') + 1);
		SetCurrentDirectory(strModulePath.c_str());
	}

	//if (!CDocument::OnOpenDocument(lpszPathName))
	//	return FALSE;

	Map map;
	_theMap = map;

	if( !_theMap.Load(lpszPathName, false) )
	{
		AfxMessageBox("加载失败，请检查文件是否合法！", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	ToolManager::getSingleton().OnNewMap(&_theMap);

	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	pMainFrame->GetPropertyWnd()->Reset();

	MapView* pMapView = pMainFrame->GetMapView();
	pMapView->Reset();
	pMapView->SetMapObject(&_theMap);

	return TRUE;
}

BOOL CTileMapEditorDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	{
		char buffer[MAX_PATH];
		::GetModuleFileName(0, buffer, MAX_PATH);
		std::string strModulePath = buffer;

		strModulePath.erase(strModulePath.find_last_of('\\') + 1);
		SetCurrentDirectory(strModulePath.c_str());
	}

	_theMap.Save(lpszPathName);

	//return CDocument::OnSaveDocument(lpszPathName);
	return TRUE;
}

void CTileMapEditorDoc::OnFileNew()
{
	if (!CDocument::SaveModified())
		return;

	DialogFileNew dlg;
	if( dlg.DoModal() != IDOK)
	{
		return;
	}

	SetTitle("NewTileMap");

	if (!CDocument::OnNewDocument())
		return;


	Map map;
	_theMap = map;

	_theMap._strName				= (LPCTSTR)dlg._strMapName;
	_theMap._strFootnotes			= (LPCTSTR)dlg._strFootnotes;
	_theMap._iWidthInTiles			= dlg._iMapWidth;
	_theMap._iHeightInTiles			= dlg._iMapHeight;
	_theMap._iUnitTileWidth			= dlg._iUnitTileWidth;
	_theMap._iUnitTileHeight		= dlg._iUnitTileHeight;
	_theMap._eGridType				= EGridType(dlg._iType);
	_theMap._iRegionWidth			= dlg._iRegionW;
	_theMap._iRegionHeight			= dlg._iRegionH;

	if (dlg._bCreateLayer)
	{
		MapLayer* pLayer = new MapLayerBackground;
		pLayer->Init("Layer_Background", &_theMap);
		_theMap.AddLayer(pLayer);
	}

	//Create build-in layers
	{
		MapLayer* pLayer = new MapLayerTerrain;
		pLayer->Init("Layer_Region", &_theMap);
		_theMap.AddLayer(pLayer);

		pLayer = new MapLayerMonster;
		pLayer->Init("Layer_Monster", &_theMap);
		_theMap.AddLayer(pLayer);

		pLayer = new MapLayerEvent;
		pLayer->Init("Layer_Event", &_theMap);
		_theMap.AddLayer(pLayer);
	}

	_theMap.CalculateRegionInfo();
	_theMap.SetModified(false);

	ToolManager::getSingleton().OnNewMap(&_theMap);

	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd() );
	pMainFrame->GetMapView()->Reset();
	pMainFrame->GetPropertyWnd()->Reset();
	pMainFrame->GetMapView()->SetMapObject(&_theMap);

	CTileMapEditorView* pView = (CTileMapEditorView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	pView->UpdateDataSizeScroll();

}
