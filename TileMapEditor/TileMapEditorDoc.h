
// TileMapEditorDoc.h : CTileMapEditorDoc ��Ľӿ�
//


#pragma once

#include "Map.h"


class CTileMapEditorDoc : public CDocument
{
protected: // �������л�����
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


