#pragma once

#include "DialogGameObject.h"
#include "GameObjectEditorView.h"

//��Ϸ����༭�����
class GameObjectEditor : public CDockablePane
{
public:
	GameObjectEditor();
	virtual ~GameObjectEditor();

	void	AddGameObject(ResourceGameObjectGroup* pGOGroup);
	void	EditGameObject(ResourceGameObjectGroup* pGOGroup, ResourceGameObject* pGO);
	void	CopyGameObject(ResourceGameObjectGroup* pGOGroup, ResourceGameObject* pGO);

public:
	CDialogGameObject		_dlgPanel;
	GameObjectEditorView*	_pView;

// ��д
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnClassAddMemberFunction();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	DECLARE_MESSAGE_MAP()
};

