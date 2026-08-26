
// SLOCToolView.cpp : implementation of the CSLOCToolView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SLOCTool.h"
#endif

#include "SLOCToolDoc.h"
#include "CntrItem.h"
#include "resource.h"
#include "SLOCToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSLOCToolView

IMPLEMENT_DYNCREATE(CSLOCToolView, CView)

BEGIN_MESSAGE_MAP(CSLOCToolView, CView)
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_COMMAND(ID_OLE_INSERT_NEW, &CSLOCToolView::OnInsertObject)
	ON_COMMAND(ID_CANCEL_EDIT_CNTR, &CSLOCToolView::OnCancelEditCntr)
	ON_COMMAND(ID_FILE_PRINT, &CSLOCToolView::OnFilePrint)
	ON_COMMAND(ID_CANCEL_EDIT_SRVR, &CSLOCToolView::OnCancelEditSrvr)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSLOCToolView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CSLOCToolView construction/destruction

CSLOCToolView::CSLOCToolView()
{
	m_pSelection = NULL;
	// TODO: add construction code here

}

CSLOCToolView::~CSLOCToolView()
{
}

BOOL CSLOCToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CSLOCToolView drawing

void CSLOCToolView::OnDraw(CDC* pDC)
{
	if (!pDC)
		return;

	CSLOCToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	// TODO: also draw all OLE items in the document

	// Draw the selection at an arbitrary position.  This code should be
	//  removed once your real drawing code is implemented.  This position
	//  corresponds exactly to the rectangle returned by CSLOCToolCntrItem,
	//  to give the effect of in-place editing.

	// TODO: remove this code when final draw code is complete.
	if (m_pSelection != NULL)
	{
		CSize size;
		CRect rect(10, 10, 210, 210);
		
		if (m_pSelection->GetExtent(&size, m_pSelection->m_nDrawAspect))
		{
			pDC->HIMETRICtoLP(&size);
			rect.right = size.cx + 10;
			rect.bottom = size.cy + 10;
		}
		m_pSelection->Draw(pDC, rect);
	}
}

void CSLOCToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();


	// TODO: remove this code when final selection model code is written
	m_pSelection = NULL;    // initialize selection

}


// CSLOCToolView printing


void CSLOCToolView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CSLOCToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSLOCToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSLOCToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CSLOCToolView::OnDestroy()
{
	// Deactivate the item on destruction; this is important
	// when a splitter view is being used
   COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
   if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
   {
      pActiveItem->Deactivate();
      ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
   }
   CView::OnDestroy();
}



// OLE Client support and commands

BOOL CSLOCToolView::IsSelected(const CObject* pDocItem) const
{
	// The implementation below is adequate if your selection consists of
	//  only CSLOCToolCntrItem objects.  To handle different selection
	//  mechanisms, the implementation here should be replaced

	// TODO: implement this function that tests for a selected OLE client item

	return pDocItem == m_pSelection;
}

void CSLOCToolView::OnInsertObject()
{
	// Invoke the standard Insert Object dialog box to obtain information
	//  for new CSLOCToolCntrItem object
	COleInsertDialog dlg;
	if (dlg.DoModal() != IDOK)
		return;

	BeginWaitCursor();

	CSLOCToolCntrItem* pItem = NULL;
	TRY
	{
		// Create new item connected to this document
		CSLOCToolDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		pItem = new CSLOCToolCntrItem(pDoc);
		ASSERT_VALID(pItem);

		// Initialize the item from the dialog data
		if (!dlg.CreateItem(pItem))
			AfxThrowMemoryException();  // any exception will do
		ASSERT_VALID(pItem);
		
        if (dlg.GetSelectionType() == COleInsertDialog::createNewItem)
			pItem->DoVerb(OLEIVERB_SHOW, this);

		ASSERT_VALID(pItem);
		// As an arbitrary user interface design, this sets the selection
		//  to the last item inserted

		// TODO: reimplement selection as appropriate for your application
		m_pSelection = pItem;   // set selection to last inserted item
		pDoc->UpdateAllViews(NULL);
	}
	CATCH(CException, e)
	{
		if (pItem != NULL)
		{
			ASSERT_VALID(pItem);
			pItem->Delete();
		}
		AfxMessageBox(IDP_FAILED_TO_CREATE);
	}
	END_CATCH

	EndWaitCursor();
}

// The following command handler provides the standard keyboard
//  user interface to cancel an in-place editing session.  Here,
//  the container (not the server) causes the deactivation
void CSLOCToolView::OnCancelEditCntr()
{
	// Close any in-place active item on this view.
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL)
	{
		pActiveItem->Close();
	}
	ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
}

// Special handling of OnSetFocus and OnSize are required for a container
//  when an object is being edited in-place
void CSLOCToolView::OnSetFocus(CWnd* pOldWnd)
{
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL &&
		pActiveItem->GetItemState() == COleClientItem::activeUIState)
	{
		// need to set focus to this item if it is in the same view
		CWnd* pWnd = pActiveItem->GetInPlaceWindow();
		if (pWnd != NULL)
		{
			pWnd->SetFocus();   // don't call the base class
			return;
		}
	}

	CView::OnSetFocus(pOldWnd);
}

void CSLOCToolView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL)
		pActiveItem->SetItemRects();
}

void CSLOCToolView::OnFilePrint()
{
	//By default, we ask the Active document to print itself
	//using IOleCommandTarget. If you don't want this behavior
	//remove the call to COleDocObjectItem::DoDefaultPrinting.
	//If the call fails for some reason, we will try printing
	//the docobject using the IPrint interface.
	CPrintInfo printInfo;
	ASSERT(printInfo.m_pPD != NULL); 
	if (S_OK == COleDocObjectItem::DoDefaultPrinting(this, &printInfo))
		return;
	
	CView::OnFilePrint();

}


void CSLOCToolView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSLOCToolView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

// OLE Server support

// The following command handler provides the standard keyboard
//  user interface to cancel an in-place editing session.  Here,
//  the server (not the container) causes the deactivation
void CSLOCToolView::OnCancelEditSrvr()
{
	GetDocument()->OnDeactivateUI(FALSE);
}


// CSLOCToolView diagnostics

#ifdef _DEBUG
void CSLOCToolView::AssertValid() const
{
	CView::AssertValid();
}

void CSLOCToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSLOCToolDoc* CSLOCToolView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSLOCToolDoc)));
	return (CSLOCToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CSLOCToolView message handlers
