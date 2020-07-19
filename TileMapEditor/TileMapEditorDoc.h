
// TileMapEditorDoc.h : CTileMapEditorDoc 类的接口
//


#pragma once

#include "Map.h"


class CTileMapEditorDoc : public CDocument
{
protected: // 仅从序列化创建
	CTileMapEditorDoc();
	DECLARE_DYNCREATE(CTileMapEditorDoc)

public:
	virtual BOOL OnNewDocument();
	virtual BOOL IsModified();
	virtual void SetModifiedFlag(BOOL bModified = TRUE);

	Map&	GetMap(){ return _theMap; }

public:
	virtual ~CTileMapEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	Map		_theMap;


protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	afx_msg void OnFileNew();
};


