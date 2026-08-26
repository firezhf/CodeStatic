
// SLOCToolDoc.h : interface of the CSLOCToolDoc class
//


#pragma once


class CSLOCToolSrvrItem;

class CSLOCToolDoc : public COleServerDocEx
{
protected: // create from serialization only
	CSLOCToolDoc();
	DECLARE_DYNCREATE(CSLOCToolDoc)

// Attributes
public:
	CSLOCToolSrvrItem* GetEmbeddedItem()
		{ return reinterpret_cast<CSLOCToolSrvrItem*>(COleServerDocEx::GetEmbeddedItem()); }

// Operations
public:

// Overrides
protected:
	virtual COleServerItem* OnGetEmbeddedItem();
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CSLOCToolDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//afx_msg void OnFileParse();
	afx_msg void OnUpdateFileParse(CCmdUI *pCmdUI);

};
