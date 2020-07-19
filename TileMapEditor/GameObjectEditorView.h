#pragma once

#include "ToolGOEditor.h"

//��Ϸ����༭���У�����ͼ
class GameObjectEditorView : public CScrollView
{
	DECLARE_DYNAMIC(GameObjectEditorView)

public:
	GameObjectEditorView();
	virtual ~GameObjectEditorView();

	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	virtual void OnInitialUpdate(); // ������һ�ε���

	void	LogicInvalidate(CRect rc);

	ToolGOEditor*	GetTool(){ return &_tool; }

protected:
	void	_DrawGrid(CDC* pDC);
	void	_DrawCenterGrid(CDC* pDC);

	ToolGOEditor	_tool;

protected:
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);

	DECLARE_MESSAGE_MAP()

	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};

