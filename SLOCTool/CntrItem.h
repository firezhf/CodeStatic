
// CntrItem.h : interface of the CSLOCToolCntrItem class
//

#pragma once

class CSLOCToolDoc;
class CSLOCToolView;

class CSLOCToolCntrItem : public COleClientItem
{
	DECLARE_SERIAL(CSLOCToolCntrItem)

// Constructors
public:
	CSLOCToolCntrItem(CSLOCToolDoc* pContainer = NULL);
		// Note: pContainer is allowed to be NULL to enable IMPLEMENT_SERIALIZE
		//  IMPLEMENT_SERIALIZE requires the class have a constructor with
		//  zero arguments.  Normally, OLE items are constructed with a
		//  non-NULL document pointer

// Attributes
public:
	CSLOCToolDoc* GetDocument()
		{ return reinterpret_cast<CSLOCToolDoc*>(COleClientItem::GetDocument()); }
	CSLOCToolView* GetActiveView()
		{ return reinterpret_cast<CSLOCToolView*>(COleClientItem::GetActiveView()); }

public:
	virtual void OnChange(OLE_NOTIFICATION wNotification, DWORD dwParam);
	virtual void OnActivate();

protected:
	virtual void OnGetItemPosition(CRect& rPosition);
	virtual void OnDeactivateUI(BOOL bUndoable);
	virtual BOOL OnChangeItemPosition(const CRect& rectPos);
	virtual BOOL OnShowControlBars(CFrameWnd* pFrameWnd, BOOL bShow);
	virtual BOOL CanActivate();

// Implementation
public:
	~CSLOCToolCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void Serialize(CArchive& ar);
};

