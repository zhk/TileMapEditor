
#pragma once

#include "XListBox.h"

class COutputList : public CXListBox
{
// 构造
public:
	COutputList();

// 实现
public:
	virtual ~COutputList();

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnViewOutput();

	DECLARE_MESSAGE_MAP()
};

// Log输出面板

class COutputWnd : public CDockablePane, public Cactus::ILogListener
{
// 构造
public:
	COutputWnd();

	virtual void	OnLogContent(const char* loggerName, Cactus::ELogLevel elevel, const char* content);


// 属性
protected:
	CFont m_Font;

	CMFCTabCtrl	m_wndTabs;

	COutputList m_wndOutputLog;
	//COutputList m_wndOutputDebug;
	//COutputList m_wndOutputFind;

protected:

	void AdjustHorzScroll(CListBox& wndListBox);

// 实现
public:
	virtual ~COutputWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};

