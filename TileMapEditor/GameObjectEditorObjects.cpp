#include "stdafx.h"
#include "GameObjectEditorObjects.h"

#include "MainFrm.h"

extern "C"
{
	GameObjectEditor*		GetGOEditor()
	{
		CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd() );
		return pMainFrame->GetGameObjectEditor();
	}

	GameObjectEditorView*	GetGOView()
	{
		return GetGOEditor()->_pView;
	}

	CDialogGameObject*		GetGODlg()
	{
		return &GetGOEditor()->_dlgPanel;
	}

	ToolGOEditor*			GetGOTool()
	{
		return GetGOView()->GetTool();
	}
};

