
// SLOCToolDoc.cpp : implementation of the CSLOCToolDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SLOCTool.h"
#endif

#include "SLOCToolDoc.h"
#include "CntrItem.h"
#include "SrvrItem.h"

#include <propkey.h>

#include "CodeParser.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSLOCToolDoc

IMPLEMENT_DYNCREATE(CSLOCToolDoc, COleServerDocEx)

BEGIN_MESSAGE_MAP(CSLOCToolDoc, COleServerDocEx)
	// Enable default OLE container implementation
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &COleServerDocEx::OnUpdatePasteMenu)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_LINK, &COleServerDocEx::OnUpdatePasteLinkMenu)
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_CONVERT, &COleServerDocEx::OnUpdateObjectVerbMenu)
	ON_COMMAND(ID_OLE_EDIT_CONVERT, &COleServerDocEx::OnEditConvert)
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_LINKS, &COleServerDocEx::OnUpdateEditLinksMenu)
	ON_UPDATE_COMMAND_UI(ID_OLE_VERB_POPUP, &CSLOCToolDoc::OnUpdateObjectVerbPopup)
	ON_COMMAND(ID_OLE_EDIT_LINKS, &COleServerDocEx::OnEditLinks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OLE_VERB_FIRST, ID_OLE_VERB_LAST, &COleServerDocEx::OnUpdateObjectVerbMenu)
	//ON_COMMAND(ID_FILE_PARSE, &CSLOCToolDoc::OnFileParse)
	//ON_UPDATE_COMMAND_UI(ID_FILE_PARSE, &CSLOCToolDoc::OnUpdateFileParse)
END_MESSAGE_MAP()


// CSLOCToolDoc construction/destruction

CSLOCToolDoc::CSLOCToolDoc()
{
	// Use OLE compound files
	EnableCompoundFile();

	// TODO: add one-time construction code here

}

CSLOCToolDoc::~CSLOCToolDoc()
{
}

BOOL CSLOCToolDoc::OnNewDocument()
{
	if (!COleServerDocEx::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}


// CSLOCToolDoc server implementation

COleServerItem* CSLOCToolDoc::OnGetEmbeddedItem()
{
	// OnGetEmbeddedItem is called by the framework to get the COleServerItem
	//  that is associated with the document.  It is only called when necessary.

	CSLOCToolSrvrItem* pItem = new CSLOCToolSrvrItem(this);
	ASSERT_VALID(pItem);
	return pItem;
}




// CSLOCToolDoc serialization

void CSLOCToolDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
		CString str;
		ar >> str;
	}
	// Calling the base class COleServerDocEx enables serialization
	//  of the container document's COleClientItem objects.
	COleServerDocEx::Serialize(ar);
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CSLOCToolDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CSLOCToolDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CSLOCToolDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CSLOCToolDoc diagnostics

#ifdef _DEBUG
void CSLOCToolDoc::AssertValid() const
{
	COleServerDocEx::AssertValid();
}

void CSLOCToolDoc::Dump(CDumpContext& dc) const
{
	COleServerDocEx::Dump(dc);
}
#endif //_DEBUG


// CSLOCToolDoc commands


BOOL CSLOCToolDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	//if (!COleServerDocEx::OnOpenDocument(lpszPathName))
	//	return FALSE;

	// TODO:  Add your specialized creation code here
	//CStdioFile file;
	//if (file.Open(lpszPathName, CFile::modeRead | CFile::shareDenyWrite) == FALSE)
	//	return FALSE;
	//CString bufRead;
	//while (file.ReadString(bufRead) != FALSE)
	//{
	//	bufRead.TrimLeft();//Delete the left black of the string
	//	++nLines;

	//	if (bufRead.GetLength() == 0) //Blank line;
	//	{
	//		nBlankLines++;
	//		continue;
	//	}

	//	if (bufRead.Find(_T("//")) == 0 && bCommentSet == false && bufRead.Find(_T("@SSS")) == -1 && bufRead.Find(_T("@CSSSI")) == -1
	//		&& !(test == true && Str[i][3].Find(_T("Delete")) != -1)
	//		&& !(bsingle == true && Str[i][3].Find(_T("Delete")) != -1))
	//	{
	//		nCommentLines++;
	//		continue;
	//	}


	return TRUE;
}
//
//void CSLOCToolDoc::OnFileParse()
//{
//	// TODO: Add your command handler code here
//	AfxMessageBox(_T("Parse Current Seleted Folder or File...."));
//
//}


void CSLOCToolDoc::OnUpdateFileParse(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	/*HTREEITEM htr = this->GetSelectedItem();
	CString str = GetFullPath(htr);
	CString strT(_T("."));
	int nIndex = str.Find(strT);
	int nCount = str.GetLength();
	CString sEx = str.Right(nCount - nIndex - 1);
	sEx.MakeLower();
	if (sEx == _T("cpp") || sEx == _T("c") || sEx == _T("h"))
		pCmdUI->Enable();
	else
		pCmdUI->Enable(FALSE);*/
}

