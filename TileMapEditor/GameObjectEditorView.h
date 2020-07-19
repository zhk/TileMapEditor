#pragma once

#include "ToolGOEditor.h"

//游戏对象编辑器中，主视图
class GameObjectEditorView : public CScrollView
{
	DECLARE_DYNAMIC(GameObjectEditorView)

public:
	GameObjectEditorView();
	virtual ~GameObjectEditorView();

	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	virtual void OnInitialUpdate(); // 构造后第一次调用

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

