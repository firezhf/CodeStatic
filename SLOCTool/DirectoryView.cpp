// DirectoryView.cpp : implementation file
//

#include "stdafx.h"
#include "SLOCTool.h"
#include "DirectoryView.h"
#include "MainFrm.h"
#include "Resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
// CDirectoryView

IMPLEMENT_DYNAMIC(CDirectoryView, CDockablePane)

CDirectoryView::CDirectoryView()
{

}

CDirectoryView::~CDirectoryView()
{
}


BEGIN_MESSAGE_MAP(CDirectoryView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_NEW_FOLDER, OnNewFolder)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
 	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_OPEN, OnFileOpen)
	ON_COMMAND(ID_OPEN_WITH, OnFileOpenWith)
	ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
END_MESSAGE_MAP()



// CDirectoryView message handlers

int CDirectoryView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create views:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndDirTree.Create(dwViewStyle, rectDummy, this, 2))
	{
		TRACE0("Failed to create Directory View\n");
		return -1;      // fail to create
	}

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* Is locked */);

	//OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// Fill in some static tree view data (dummy code, nothing magic here)
	m_wndDirTree.InitTree();
	AdjustLayout();

	return 0;
}

void CDirectoryView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CDirectoryView::FillDirectoryView()
{
	m_hRoot = m_wndDirTree.InsertItem(_T("My Computer"), 0, 0);
	m_wndDirTree.SetItemState(m_hRoot, TVIS_BOLD, TVIS_BOLD);
	GetLogicalDrives(m_hRoot,m_wndDirTree);
	GetDriverDir(m_hRoot, m_wndDirTree);
	m_wndDirTree.m_pDirView = this;
}

void CDirectoryView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndDirTree;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// Select clicked item:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);

}

void CDirectoryView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndDirTree.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CDirectoryView::OnProperties()
{
	//AfxMessageBox(_T("Parse Current Seleted Folder or File...."));
	//m_wndDirTree.OnFileParse();
}

void CDirectoryView::OnFileOpen()
{
	// TODO: Add your command handler code here
}

void CDirectoryView::OnFileOpenWith()
{
	// TODO: Add your command handler code here
}

void CDirectoryView::OnDummyCompile()
{
	// TODO: Add your command handler code here
}

void CDirectoryView::OnEditCut()
{
	// TODO: Add your command handler code here
}

void CDirectoryView::OnEditCopy()
{
	// TODO: Add your command handler code here
}

void CDirectoryView::OnEditClear()
{
	// TODO: Add your command handler code here
}

void CDirectoryView::OnNewFolder()
{
	AfxMessageBox(_T("New Folder..."));
}

void CDirectoryView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndDirTree.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CDirectoryView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndDirTree.SetFocus();
}

void CDirectoryView::OnChangeVisualStyle()
{
	m_DirTreeImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("Can't load bitmap: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_DirTreeImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_DirTreeImages.Add(&bmp, RGB(255, 0, 255));

	m_wndDirTree.SetImageList(&m_DirTreeImages, TVSIL_NORMAL);

}

void CDirectoryView::GetLogicalDrives(HTREEITEM hParent, CTreeCtrl & oTree)
{
	DWORD szAllDriveStrings = MAX_PATH;
	TCHAR cDriverStrings[MAX_PATH] = { 0 }; 
	DWORD dwResult = GetLogicalDriveStrings(szAllDriveStrings, (LPWSTR)cDriverStrings);
	if (dwResult > 0 && dwResult <= MAX_PATH)
	{
		TCHAR* szSingleDrive = cDriverStrings;
		while (*szSingleDrive)
		{
			CString s(szSingleDrive);
			int nIcon = m_wndDirTree.GetIconIndex(s, TRUE, TRUE);
			oTree.InsertItem(s,nIcon,nIcon,hParent);
			szSingleDrive += wcslen(szSingleDrive) + 1;
		}
	}
}


void CDirectoryView::GetDriverDir(HTREEITEM hParent, CTreeCtrl& oTree)
{
	HTREEITEM hChild = oTree.GetChildItem(hParent);
	while (hChild)
	{
		CString strText = oTree.GetItemText(hChild);
		if (strText.Right(1) != "\\")
			strText += _T("\\");
		strText += "*.*";
		CFileFind file;
		BOOL bContinue = file.FindFile(strText);
		while (bContinue)
		{
			bContinue = file.FindNextFile();
			int nIcon = 0; 
			CString sfullname = file.GetFilePath();
			if (file.IsDirectory() && !file.IsDots())
			{
				nIcon = m_wndDirTree.GetIconIndex(sfullname, TRUE, TRUE);
				oTree.InsertItem(file.GetFileName(), nIcon, nIcon, hChild);
			}
			else if (!file.IsDirectory())
			{
				nIcon = m_wndDirTree.GetIconIndex(sfullname, FALSE, TRUE);
				oTree.InsertItem(file.GetFileName(), nIcon, nIcon, hChild);
			}
		}
		GetDriverDir(hChild, oTree);
		hChild = oTree.GetNextItem(hChild, TVGN_NEXT);
	}
}


