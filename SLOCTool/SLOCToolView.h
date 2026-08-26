
// SLOCToolView.h : interface of the CSLOCToolView class
//

#pragma once

class CSLOCToolCntrItem;

class CSLOCToolView : public CView
{
protected: // create from serialization only
	CSLOCToolView();
	DECLARE_DYNCREATE(CSLOCToolView)

// Attributes
public:
	CSLOCToolDoc* GetDocument() const;
	// m_pSelection holds the selection to the current CSLOCToolCntrItem.
	// For many applications, such a member variable isn't adequate to
	//  represent a selection, such as a multiple selection or a selection
	//  of objects that are not CSLOCToolCntrItem objects.  This selection
	//  mechanism is provided just to help you get started

	// TODO: replace this selection mechanism with one appropriate to your app
	CSLOCToolCntrItem* m_pSelection;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL IsSelected(const CObject* pDocItem) const;// Container support

// Implementation
public:
	virtual ~CSLOCToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnInsertObject();
	afx_msg void OnCancelEditCntr();
	afx_msg void OnFilePrint();
	afx_msg void OnCancelEditSrvr();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SLOCToolView.cpp
inline CSLOCToolDoc* CSLOCToolView::GetDocument() const
   { return reinterpret_cast<CSLOCToolDoc*>(m_pDocument); }
#endif

