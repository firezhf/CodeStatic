
// SrvrItem.h : interface of the CSLOCToolSrvrItem class
//

#pragma once

class CSLOCToolSrvrItem : public COleServerItem
{
	DECLARE_DYNAMIC(CSLOCToolSrvrItem)

// Constructors
public:
	CSLOCToolSrvrItem(CSLOCToolDoc* pContainerDoc);

// Attributes
	CSLOCToolDoc* GetDocument() const
		{ return reinterpret_cast<CSLOCToolDoc*>(COleServerItem::GetDocument()); }

// Overrides
	public:
	virtual BOOL OnDraw(CDC* pDC, CSize& rSize);
	virtual BOOL OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize);

// Implementation
public:
	~CSLOCToolSrvrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
};

