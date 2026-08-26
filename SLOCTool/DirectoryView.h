#pragma once

#include "ViewTree.h"
#include "DirTree.h"

class CDirectoryToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

// CDirectoryView

class CDirectoryView : public CDockablePane
{
	DECLARE_DYNAMIC(CDirectoryView)

public:
	CDirectoryView();
	virtual ~CDirectoryView();

	void AdjustLayout();
	void OnChangeVisualStyle();


	CDirectoryToolBar m_wndToolBar;
	CDirTree m_wndDirTree;
	CImageList m_DirTreeImages;

	HTREEITEM m_hRoot;
	HICON m_hIcon;

	void FillDirectoryView();

	// Overrides

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnNewFolder();
	afx_msg void OnProperties();
	afx_msg void OnFileOpen();
	afx_msg void OnFileOpenWith();
	afx_msg void OnDummyCompile();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

protected:
	DECLARE_MESSAGE_MAP()
public:
	void GetDriverDir(HTREEITEM hParent,CTreeCtrl& oTree);
	void GetLogicalDrives(HTREEITEM hParent, CTreeCtrl & oTree);
};


