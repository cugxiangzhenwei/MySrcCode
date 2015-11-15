// ShellContextMenu.h: Schnittstelle für die Klasse CShellContextMenu.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHELLCONTEXTMENU_H__A358AACF_7C7C_410D_AD29_67310B2DDC22__INCLUDED_)
#define AFX_SHELLCONTEXTMENU_H__A358AACF_7C7C_410D_AD29_67310B2DDC22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////
// class to show shell contextmenu of files/folders/shell objects
// developed by R. Engels 2003
/////////////////////////////////////////////////////////////////////

class CShellContextMenu  
{
public:
	CMenu * GetMenu ();
	void SetObjects (IShellFolder * psfFolder, LPITEMIDLIST pidlItem);
	void SetObjects (IShellFolder * psfFolder, LPITEMIDLIST * pidlArray, int nItemCount);
	void SetObjects (LPITEMIDLIST pidl);
	void SetObjects (CString strObject);
	void SetObjects (CStringArray &strArray);
	UINT ShowContextMenu (CWnd* pWnd, CPoint pt);
	CShellContextMenu();
	virtual ~CShellContextMenu();

private:
	int nItems;
	BOOL bDelete;
	CMenu * m_Menu;
	IShellFolder * m_psfFolder;
	LPITEMIDLIST * m_pidlArray;	
	
	void InvokeCommand (LPCONTEXTMENU pContextMenu, UINT idCommand);
	BOOL GetContextMenu (void ** ppContextMenu, int & iMenuType);
	HRESULT SHBindToParentEx (LPCITEMIDLIST pidl, REFIID riid, VOID **ppv, LPCITEMIDLIST *ppidlLast);
	static LRESULT CALLBACK HookWndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void FreePIDLArray (LPITEMIDLIST * pidlArray);
	LPITEMIDLIST CopyPIDL (LPCITEMIDLIST pidl, int cb = -1);
	UINT GetPIDLSize (LPCITEMIDLIST pidl);
	LPBYTE GetPIDLPos (LPCITEMIDLIST pidl, int nPos);
	int GetPIDLCount (LPCITEMIDLIST pidl);
};

#endif // !defined(AFX_SHELLCONTEXTMENU_H__A358AACF_7C7C_410D_AD29_67310B2DDC22__INCLUDED_)
