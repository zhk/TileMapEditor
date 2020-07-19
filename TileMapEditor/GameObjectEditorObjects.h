#pragma once

#include "GameObjectEditor.h"
#include "DialogGameObject.h"
#include "GameObjectEditorView.h"
#include "ToolGOEditor.h"

extern "C"
{
	GameObjectEditor*		GetGOEditor();
	GameObjectEditorView*	GetGOView();
	CDialogGameObject*		GetGODlg();
	ToolGOEditor*			GetGOTool();
}