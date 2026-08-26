#pragma once

// CDirTree
class CDirectoryView;
class CDirTree : public CTreeCtrl
{
	DECLARE_DYNAMIC(CDirTree)

public:
	CDirTree();
	virtual ~CDirTree();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnItemexpanded(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnItemChanged(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	CString GetFullPath(HTREEITEM hCurrent);
	void AddSubDir(HTREEITEM hParent);
	void ParseCurrentSelected();

	CDirectoryView* m_pDirView;
	CImageList m_Imglist;
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	int GetIconIndex(LPCTSTR lpszPath, bool bIsDir, bool bSelected);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void InitTree();
	void InitDriverTree(HTREEITEM hParent);
	void InitChildren(HTREEITEM hParent);

	afx_msg void OnTvnSingleExpand(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchanging(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
};


