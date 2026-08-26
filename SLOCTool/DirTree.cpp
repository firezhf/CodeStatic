// DirTree.cpp : implementation file
//

#include "stdafx.h"
#include "SLOCTool.h"
#include "DirTree.h"
#include "DirectoryView.h"

#include "CodeParser.h"


// CDirTree

IMPLEMENT_DYNAMIC(CDirTree, CTreeCtrl)

CDirTree::CDirTree()
{
	m_pDirView = NULL;
}

CDirTree::~CDirTree()
{
}


BEGIN_MESSAGE_MAP(CDirTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, &CDirTree::OnTvnItemexpanded)
	ON_NOTIFY_REFLECT(TVN_ITEMCHANGED, &CDirTree::OnTvnItemChanged)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CDirTree::OnTvnSelchanged)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TVN_SINGLEEXPAND, &CDirTree::OnTvnSingleExpand)
	ON_NOTIFY_REFLECT(TVN_SELCHANGING, &CDirTree::OnTvnSelchanging)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CDirTree::OnNMDblclk)
END_MESSAGE_MAP()



// CDirTree message handlers




void CDirTree::OnTvnItemexpanded(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	/*TVITEM item = pNMTreeView->itemNew;
	if (item.hItem == GetRootItem())
		return;
	HTREEITEM hChild = GetChildItem(item.hItem);
	while (hChild)
	{
		AddSubDir(hChild);
		hChild = GetNextItem(hChild, TVGN_NEXT);
	}*/
	*pResult = 0;
}


void CDirTree::OnTvnItemChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMTVITEMCHANGE *pNMTVItemChange = reinterpret_cast<NMTVITEMCHANGE*>(pNMHDR);
	// TODO: Add your control notification handler code here
	HTREEITEM hItem = pNMTVItemChange->hItem;
	if (hItem == GetRootItem())
		return;
	AddSubDir(hItem);
	//HTREEITEM hChild = GetChildItem(hItem);
	/*while (hChild)
	{
		AddSubDir(hChild);
		hChild = GetNextItem(hChild, TVGN_NEXT);
	}*/
	*pResult = 0;
}


BOOL CDirTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return bRes;
}


CString CDirTree::GetFullPath(HTREEITEM hCurrent)
{
	CString strTemp;
	CString strReturn = CString("");
	while (hCurrent != GetRootItem())
	{
		strTemp = GetItemText(hCurrent);
		if (strTemp.Right(1) != "\\")
			strTemp += "\\";
		strReturn = strTemp + strReturn;
		hCurrent = GetParentItem(hCurrent);
	}
	return strReturn;
}

void CDirTree::AddSubDir(HTREEITEM hParent)
{
	if (ItemHasChildren(hParent))
		return;
	CString strPath = GetFullPath(hParent);
	if (strPath.Right(1) != "\\")
		strPath += "\\";
	strPath += "*.*";
	CFileFind file;
	BOOL bContinue = file.FindFile(strPath);
	while (bContinue)
	{
		bContinue = file.FindNextFile();
		int nIcon = 0;
		CString sfullname = file.GetFilePath();
		if (file.IsDirectory() && !file.IsDots())
		{
			nIcon = GetIconIndex(sfullname, TRUE, TRUE);
			InsertItem(file.GetFileName(), nIcon, nIcon, hParent);
		}
		else if (!file.IsDirectory())
		{
			nIcon = GetIconIndex(sfullname, FALSE, TRUE);
			InsertItem(file.GetFileName(), nIcon, nIcon, hParent);
		}
	}

	/*
	if (m_pDirView == NULL)
		return;
	HTREEITEM hChild = GetChildItem(hParent);
	while (hChild)
	{
		CString strText = GetItemText(hChild);
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
				nIcon = GetIconIndex(sfullname, TRUE, TRUE);
			else if (!file.IsDirectory())
				nIcon = GetIconIndex(sfullname, FALSE, TRUE);
			InsertItem(file.GetFileName(), nIcon, nIcon, hChild);
		}
		AddSubDir(hChild);
		hChild = GetNextItem(hChild, TVGN_NEXT);
	}*/

}

void CDirTree::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	return;
	// TODO: Add your control notification handler code here
	TVITEM item = pNMTreeView->itemNew;
	if (item.hItem == GetRootItem())
		return;
	CString str = GetFullPath(item.hItem);
	if (str.Right(1) != "\\")
		str += "\\";
	str += "*.*";
	CFileFind file;
	BOOL bContinue = file.FindFile(str);
	while (bContinue)
	{
		bContinue = file.FindNextFile();
		if (file.IsDirectory() && !file.IsDots())
		{
			SHFILEINFO info;
			CString temp = str;
			CString t("*.*");
			int index = temp.Find(t);
			temp.Delete(index, 3);
			SHGetFileInfo(temp + file.GetFileName(), 0, &info, sizeof(&info), SHGFI_DISPLAYNAME | SHGFI_ICON);
			int i = m_Imglist.Add(info.hIcon);
			//this->InsertItem(file.GetFileName, hParent);
		}
		else if (!file.IsDirectory())
		{
			SHFILEINFO info;
			CString temp = str;
			int index = temp.Find(_T("*.*"));
			temp.Delete(index, 3);
			SHGetFileInfo(temp + file.GetFileName(), 0, &info, sizeof(&info), SHGFI_DISPLAYNAME | SHGFI_ICON);
			int i = m_Imglist.Add(info.hIcon);
			//this->InsertItem(file.GetFileName, hParent);
		}
	}

	*pResult = 0;
}


int CDirTree::GetIconIndex(LPCTSTR lpszPath, bool bIsDir, bool bSelected)
{
	SHFILEINFO sfi;
	memset(&sfi, 0, sizeof(sfi));

	if (bIsDir)
	{
		SHGetFileInfo(lpszPath, FILE_ATTRIBUTE_DIRECTORY, &sfi, sizeof(sfi), SHGFI_SMALLICON | SHGSI_SYSICONINDEX | (bSelected ? SHGFI_OPENICON : 0));
		return sfi.iIcon;
	}
	else
	{
		SHGetFileInfo(lpszPath, FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(sfi), SHGFI_SMALLICON | SHGSI_SYSICONINDEX | (bSelected ? SHGFI_OPENICON : 0));
		return sfi.iIcon;
	}
	return -1;
}


int CDirTree::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	CoInitialize(NULL);
	HIMAGELIST hTree;
	SHFILEINFO sfi;
	hTree = (HIMAGELIST)SHGetFileInfo((LPCTSTR)_T(""), 0, &sfi, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
	SetImageList(CImageList::FromHandle(hTree), TVSIL_NORMAL);

	return 0;
}


void CDirTree::InitTree()
{	
	HTREEITEM hRoot = InsertItem(_T("Computer"), 0, 0);
	SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
	InitDriverTree(hRoot);
}

void CDirTree::InitDriverTree(HTREEITEM hParent)
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
			int nIcon = GetIconIndex(s, TRUE, TRUE);
			HTREEITEM hDriver = InsertItem(s, nIcon, nIcon, hParent);
			szSingleDrive += wcslen(szSingleDrive) + 1;
		}
	}
}


void CDirTree::InitChildren(HTREEITEM hParent)
{
	//If this node already has child, no need to update it.
	if (ItemHasChildren(hParent))
		return;

	CString strPath = GetFullPath(hParent);
	if (strPath.Right(1) != "\\")
		strPath += "\\";
	strPath += "*.*";
	CFileFind file;
	BOOL bContinue = file.FindFile(strPath);
	while (bContinue)
	{
		bContinue = file.FindNextFile();
		int nIcon = 0;
		CString sfullname = file.GetFilePath();
		if (file.IsDirectory() && !file.IsDots())
		{
			nIcon = GetIconIndex(sfullname, TRUE, TRUE);
			InsertItem(file.GetFileName(), nIcon, nIcon, hParent);
		}
		else if (!file.IsDirectory())
		{
			nIcon = GetIconIndex(sfullname, FALSE, TRUE);
			InsertItem(file.GetFileName(), nIcon, nIcon, hParent);
		}
	}
}




void CDirTree::OnTvnSingleExpand(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CDirTree::OnTvnSelchanging(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CDirTree::ParseCurrentSelected()
{
	HTREEITEM htr = this->GetSelectedItem();
	CString str = GetFullPath(htr);
	CString str1(_T("\\"));
	if (str.Right(1) == "\\")
		str.TrimRight(str1);
	LONG time1 = ::GetTickCount();
	DWORD dwAttr = ::GetFileAttributes(str);
	if (dwAttr & FILE_ATTRIBUTE_DIRECTORY)
	{
		CCodeParser codeparser;
		codeparser.ParsePath(str);
		SYSTEMTIME Time;
		::GetSystemTime(&Time);
		CString strTime;
		strTime.Format(_T("\\Comment%d_%d_%d_%d_%d_%d.csv"), Time.wYear, Time.wMonth, Time.wDay, Time.wHour + 8 > 24 ? Time.wHour + 8 - 24 : Time.wHour + 8, Time.wMinute, Time.wSecond);
		TCHAR _szPath[MAX_PATH + 1] = { 0 };
		GetCurrentDirectory(MAX_PATH, _szPath);
		CString s(_szPath);
		s = s + strTime;
		codeparser.OutputToCSV(s);
		LONG time2 = ::GetTickCount();
		CString st;
		st.Format(_T("Parser finished! Elapsed time : %dms \n"), time2 - time1);
		int nRet = MessageBox(st + s + _T(" Created! Open ?"), _T("SLOC"), MB_ICONINFORMATION | MB_YESNO);
		if (nRet == IDYES)
		{
			ShellExecute(0, _T("open"), s, NULL, NULL, SW_SHOWNORMAL);
		}
	}
	else // normal file
	{
		CString strT(_T("."));
		int nIndex = str.Find(strT);
		int nCount = str.GetLength();
		CString sEx = str.Right(nCount - nIndex - 1);
		sEx.MakeLower();
		if (sEx == _T("cpp") || sEx == _T("c") || sEx == _T("h") || sEx == _T("rc") || sEx == _T("plc"))
		{
			CCodeParser codeparser;
			codeparser.ParseFile(str);
			SYSTEMTIME Time;
			::GetSystemTime(&Time);
			CString strTime;
			strTime.Format(_T("\\Comment%d_%d_%d_%d_%d_%d.csv"), Time.wYear, Time.wMonth, Time.wDay, Time.wHour + 8 > 24 ? Time.wHour + 8 - 24 : Time.wHour + 8, Time.wMinute, Time.wSecond);
			TCHAR _szPath[MAX_PATH + 1] = { 0 };
			GetCurrentDirectory(MAX_PATH, _szPath);
			CString s(_szPath);
			s = s + strTime;
			codeparser.OutputToCSV(s);
			LONG time2 = ::GetTickCount();
			CString st;
			st.Format(_T("Parser finished! Elapsed time : %dms \n"), time2 - time1);
			int nRet = MessageBox(st + s + _T(" Created! Open it or not?"), _T("SLOC"), MB_ICONINFORMATION | MB_YESNO);
			if (nRet == IDYES)
			{
				ShellExecute(0, _T("open"), s, NULL, NULL, SW_SHOWNORMAL);
			}
		}
	}

}

void CDirTree::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;



}
