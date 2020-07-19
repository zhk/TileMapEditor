
// MainFrm.h : CMainFrame 类的接口
//

#pragma once
#include "MapView.h"
#include "ResourceTreeView.h"
#include "OutputWnd.h"
#include "PropertiesWnd.h"
#include "TileResView.h"
#include "LayerView.h"
#include "GameObjectEditor.h"
#include "MapThumbnailView.h"

class CMainFrame : public CFrameWndEx
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	TileResView*		GetTileResView(){ return &_TileResView; }
	MapView*			GetMapView(){ return &_MapPanel; }
	CPropertiesWnd*		GetPropertyWnd(){ return &m_wndProperties; }
	LayerView*			GetMapLayerView(){ return &_LayerPanel; }
	MapThumbnailView*	GetMapThumbnailView(){ return &_MapThumbnailPanel; }
	GameObjectEditor*	GetGameObjectEditor(){ return &_GameObjectEditor; }

	void				SetCurLayer(MapLayer* pLayer);
	void				SetCursorPosition(const CPoint& pt);


protected:  // 控件条嵌入成员
	CMFCRibbonApplicationButton	m_MainButton;
	CMFCToolBarImages			m_PanelImages;

	CMFCRibbonBar			m_wndRibbonBar;			//1
	CMFCRibbonStatusBar		m_wndStatusBar;			//2
	COutputWnd				m_wndOutput;			//3
	CPropertiesWnd			m_wndProperties;		//4

	ResourceTreeView	_ResTreePanel;				//5
	MapView				_MapPanel;					//6
	TileResView			_TileResView;				//7
	LayerView			_LayerPanel;				//8
	GameObjectEditor	_GameObjectEditor;			//9
	MapThumbnailView	_MapThumbnailPanel;			//10

	CMFCRibbonLabel*	_pCurLayerLabel;
	CMFCRibbonLabel*	_pCurPositionLabel;

	bool				_bFullScreen;
	UINT				_uPanelVisible;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);

	afx_msg void OnView_ResTreePanel();
	afx_msg void OnUpdateView_ResTreePanel(CCmdUI* pCmdUI);
	afx_msg void OnView_MapPanel();
	afx_msg void OnUpdateView_MapPanel(CCmdUI* pCmdUI);
	afx_msg void OnView_ResDetailPanel();
	afx_msg void OnUpdateView_ResDetailPanel(CCmdUI* pCmdUI);
	afx_msg void OnView_OutputPanel();
	afx_msg void OnUpdateView_OutputPanel(CCmdUI* pCmdUI);
	afx_msg void OnView_PropertyPanel();
	afx_msg void OnUpdateView_PropertyPanel(CCmdUI* pCmdUI);
	afx_msg void OnView_MayLayerPanel();
	afx_msg void OnUpdateView_MayLayerPanel(CCmdUI* pCmdUI);
	afx_msg void OnView_GameObjectEditorPanel();
	afx_msg void OnUpdateView_GameObjectEditorPanel(CCmdUI* pCmdUI);
	afx_msg void OnView_MapThumbnailPanel();
	afx_msg void OnUpdateView_MapThumbnailPanel(CCmdUI* pCmdUI);

	afx_msg void OnTool_Select();
	afx_msg void OnUpdateButton_Select(CCmdUI* pCmdUI);
	afx_msg void OnTool_Brush();
	afx_msg void OnUpdateButton_Brush(CCmdUI* pCmdUI);
	afx_msg void OnTool_RegionBrush();
	afx_msg void OnUpdateButton_RegionBrush(CCmdUI* pCmdUI);
	afx_msg void OnTool_ComboTerrainType();

	afx_msg void OnToggleFullScreen();

	afx_msg void OnResourceStat();

	DECLARE_MESSAGE_MAP()

	void InitializeRibbon();
	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
public:
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	afx_msg void OnDestroy();
};


