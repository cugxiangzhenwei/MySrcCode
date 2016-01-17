// PeraFileSelectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FileSelectDlg.h"
#include <dlgs.h>
#include <map>
#include <atlbase.h>			 // for CComQIPtr smart pointers
using namespace std;
using namespace ATL;

const int SELECT_MAX_FILE_NUMBER = 500; 
// CFileSelectDlg 对话框
WNDPROC CFileSelectDlg::m_wndProc = NULL;
CString CFileSelectDlg::m_strCurrendDirectory;
CStringArray CFileSelectDlg::m_SelectedItemList;

IMPLEMENT_DYNAMIC(CFileSelectDlg, CFileDialog)

	CFileSelectDlg::CFileSelectDlg(BOOL bOpenFileDialog,
	LPCTSTR lpszDefExt /*NULL*/,
	LPCTSTR lpszFileName /*NULL*/,
	DWORD dwFlags /* OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT*/,
	LPCTSTR lpszFilter /*NULL*/,
	CWnd* pParentWnd /*NULL*/,
	DWORD dwSize /*0*/,
	BOOL bVistaStyle /*TRUE*/)
	: CFileDialog(bOpenFileDialog,lpszDefExt,lpszFileName,dwFlags|OFN_ENABLEINCLUDENOTIFY|OFN_EXPLORER | OFN_HIDEREADONLY & (~OFN_SHOWHELP),lpszFilter,pParentWnd,dwSize,bVistaStyle)
{
   // m_ofn.lpTemplateName=MAKEINTRESOURCE(CFileSelectDlg::IDD);
	dwFlags |= (OFN_EXPLORER | OFN_HIDEREADONLY & (~OFN_SHOWHELP));
	m_strSelectListTemp.RemoveAll();
}

CFileSelectDlg::~CFileSelectDlg()
{
}
map<CString,CString> GetSystemDrives()
{
	char  szDrives[512];
	char* pDrive;

	map<CString,CString> vDriverMap;
	if ( !GetLogicalDriveStrings( sizeof(szDrives), szDrives ) )
		return vDriverMap;

	pDrive = szDrives;
	while( *pDrive )
	{
		CString strDrive = pDrive;
		SHFILEINFO    sfi;
		SHGetFileInfo((LPCTSTR)strDrive,0,&sfi, sizeof(SHFILEINFO), SHGFI_DISPLAYNAME);
		CString strDisplayName = sfi.szDisplayName;
		vDriverMap.insert(make_pair(strDisplayName,strDrive));
		pDrive += strlen( pDrive ) + 1;
	}
	return vDriverMap;
}

//void CFileSelectDlg::DoDataExchange(CDataExchange* pDX)
//{
//	CFileDialog::DoDataExchange(pDX);
//}


BEGIN_MESSAGE_MAP(CFileSelectDlg, CFileDialog)
END_MESSAGE_MAP()


// CFileSelectDlg 消息处理程序
void CFileSelectDlg::OnInitDone()
{
	// TODO: 在此添加专用代码和/或调用基类
	CFileDialog::OnInitDone();
	CWnd* pWnd=FromHandle(::GetParent(m_hWnd))->GetDlgItem(IDOK);
	ASSERT(pWnd);
	pWnd->SetWindowText(_T("添加到压缩包"));
	CWnd *Parent = this->GetParent(); 
	Parent->SendMessage(CDM_SETCONTROLTEXT, stc3, (LPARAM)_T("名称："));
	Parent->SendMessage(CDM_SETCONTROLTEXT, stc2, (LPARAM)_T("类型："));

	m_wndProc = (WNDPROC)::SetWindowLong(::GetParent(m_hWnd), GWL_WNDPROC, (long)WindowProcNew);
}
void CFileSelectDlg::OnFolderChange()
{
	m_strCurrendDirectory = GetFolderPath();
	m_strSelectListTemp.RemoveAll();
	CFileSelectDlg::m_SelectedItemList.RemoveAll();
	CListCtrl *pLsit = GetListCtrl();
	if (pLsit) // 切换目录后清空选择
	{
		CString strEmpty;
		GetParent()->SendMessage(CDM_SETCONTROLTEXT, edt1, (LPARAM)strEmpty.GetBuffer());
		strEmpty.ReleaseBuffer();

		POSITION pos = pLsit->GetFirstSelectedItemPosition();
		while(pos)
		{
			int iSelID  = pLsit->GetNextSelectedItem(pos);
			pLsit->SetItemState(iSelID,0,LVIS_SELECTED);
		}
		pLsit->SetSelectionMark(-1);
		//pLsit->SetFocus();
	}
	CFileDialog::OnFolderChange();
}
int CFileSelectDlg::GetSelectCount()
{
	CListCtrl *pList = GetListCtrl();
	if (pList==NULL)
		return 0;

	ASSERT(pList!=NULL);
	return pList->GetSelectedCount();
}
CStringArray & CFileSelectDlg::GetSelectList()
{
	return m_strSelectListNow;
}
CListCtrl* CFileSelectDlg::GetListCtrl()
{
	CWnd* pWnd = GetParent()->GetDlgItem(lst2);	//getting list
	if (pWnd == NULL)
		return NULL;

	CListCtrl* wndLst1 = (CListCtrl*)(pWnd->GetDlgItem(1));
	return wndLst1;
}
BOOL CFileSelectDlg::OnNotify(WPARAM wp, LPARAM lp, LRESULT* pRes)
{
	OFNOTIFYEX& of = *(OFNOTIFYEX*)lp;
	switch(of.hdr.code) {
	case CDN_INCLUDEITEM:
		{
			CString strItemPath;
			IShellFolder *ish = (IShellFolder*)of.psf;
			LPCITEMIDLIST pidl = (LPCITEMIDLIST)of.pidl;
			STRRET str;
			if (NO_ERROR==ish->GetDisplayNameOf(pidl, SHGDN_NORMAL|SHGDN_FORPARSING, &str))
			{
				strItemPath = str.pOleStr;
			   m_strSelectListTemp.Add(strItemPath);
			}
			return TRUE;
		}
	}
	return CFileDialog::OnNotify(wp, lp, pRes);
}
void CFileSelectDlg::OnFileNameChange()
{
	int iSelCount = GetSelectCount();
	if(iSelCount<=0)
	{
		m_strSelectListTemp.RemoveAll();
		m_strSelectListNow.RemoveAll();
		CFileSelectDlg::m_SelectedItemList.RemoveAll();
		return;
	}

	int iNotifyCount = m_strSelectListTemp.GetCount();
	if (iNotifyCount != iSelCount) 
	{
		// 1. xp下选择磁盘时,notify选择的个数和树上选择的个数不一致
		// 2. win7下会出现iNotifyCount>iSelCount
		// 3. xp下notify过滤了文件夹，所以以list控件上选择的为标准
		BOOL bSingleFileSuccess = FALSE;
		m_strSelectListTemp.RemoveAll();
		if (iSelCount==1)
		{
			CString fileslist = _T("");
			GetParent()->SendMessage(CDM_GETSPEC, (WPARAM)MAX_PATH * SELECT_MAX_FILE_NUMBER,
				(LPARAM)fileslist.GetBuffer(MAX_PATH * SELECT_MAX_FILE_NUMBER));
			fileslist.ReleaseBuffer();
			if (!fileslist.IsEmpty())
			{
				CString strItemText = m_strCurrendDirectory + CString("\\")  + fileslist;
				if (PathFileExists(strItemText))
				{
					bSingleFileSuccess = TRUE;
					m_strSelectListTemp.Add(strItemText);
				}
			}
		}

		if (bSingleFileSuccess == FALSE)
		{
			CListCtrl * wndLst1 = GetListCtrl();
			int nItem = wndLst1->GetNextItem(-1,LVNI_SELECTED);
			map<CString,CString> vDriverMap =  GetSystemDrives();
			for (int i=0;i<iSelCount;i++)
			{
				CString  strItemText;
				if (m_strCurrendDirectory.IsEmpty())
					strItemText = wndLst1->GetItemText(nItem,0);
				else
					strItemText = m_strCurrendDirectory + CString("\\") +  wndLst1->GetItemText(nItem,0);

				LPCITEMIDLIST li = (LPCITEMIDLIST)wndLst1->GetItemData(nItem);
				if (li!=NULL)
				{
					CString strTmp = GetDisplayNameOf(li,SHGDN_NORMAL|SHGDN_FORPARSING); // xp下GetItemData返回非空，win7下不会走本行代码
					if (!strTmp.IsEmpty())
						strItemText = strTmp;

				}

				if (PathFileExists(strItemText))
					m_strSelectListTemp.Add(strItemText);
				else 
				{
					map<CString,CString>::iterator iter = vDriverMap.find(strItemText);
					if (iter!=vDriverMap.end())
					{
						CString strValue = iter->second;
						m_strSelectListTemp.Add(strValue);
					}
					else
					{
					}
				}
				nItem = wndLst1->GetNextItem(nItem, LVNI_SELECTED);
			}
		}
		

	}

	m_strSelectListNow.RemoveAll();
	m_strSelectListNow.Append(m_strSelectListTemp);
	m_strSelectListTemp.RemoveAll();
	CFileSelectDlg::m_SelectedItemList.RemoveAll();
	CString strEditText;
	for (int i=0;i<m_strSelectListNow.GetCount();i++)
	{
		if (PathFileExists(m_strSelectListNow[i]))
		{
			CFileSelectDlg::m_SelectedItemList.Add(m_strSelectListNow[i]);
			CString strName = PathFindFileName(m_strSelectListNow[i]);
			strEditText += CString("\"") + strName + CString("\"");
			strEditText += " ";
		}
		else
		{
		}
	}
	strEditText.TrimRight(" ");
	if (m_strSelectListNow.GetCount()==1) // 选择一个时不加引号
	{
		strEditText.TrimLeft("\"");
		strEditText.TrimRight("\"");
	}

	GetParent()->SendMessage(CDM_SETCONTROLTEXT, edt1, (LPARAM)strEditText.GetBuffer());
	strEditText.ReleaseBuffer();

	CString strFile = GetPathName();
	if (!strFile.IsEmpty() && m_strCurrendDirectory.IsEmpty())
	{
		TCHAR* pName = PathFindFileName(strFile); 
		CString strPath = strFile.Left(strFile.Find(pName));
		m_strCurrendDirectory = strPath;
	}
	CFileDialog::OnFileNameChange();
}
LRESULT CALLBACK CFileSelectDlg::WindowProcNew(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message ==  WM_COMMAND)
	{
		if (HIWORD(wParam) == BN_CLICKED)
		{
			if (LOWORD(wParam) == IDOK)
			{
				if (CFileDialog* pDlg = (CFileDialog*)CWnd::FromHandle(hwnd))
				{
					pDlg->EndDialog(IDOK);
					return NULL;

					//m_SelectedItemList.RemoveAll();			// emptying list
					//CWnd* pWnd = pDlg->GetDlgItem(lst2);	//getting list
					//if (pWnd == NULL)
					//	return FALSE;

					//CListCtrl* wndLst1 = (CListCtrl*)(pWnd->GetDlgItem(1));

					//int nSelected = wndLst1->GetSelectedCount();
					//if (!nSelected)		// nothing selected -- don't retrieve list
					//	return FALSE;
					//CString strItemText, strDirectory = m_strCurrendDirectory;
					//if (strDirectory.Right(1) != _T("\\"))
					//	strDirectory += _T("\\");
					//
					//int nItem = wndLst1->GetNextItem(-1,LVNI_SELECTED);
					//map<CString,CString> vDriverMap =  GetSystemDrives();
					////////////////////   Add directory names to list
					//while((nSelected--) > 0)
					//{
					//	strItemText = wndLst1->GetItemText(nItem,0);
					//	CString strListText = strItemText;
					//	strItemText = strDirectory + strItemText;
					//	DWORD attr = GetFileAttributes(strItemText);
					//	if((attr != 0xFFFFFFFF) && (attr & FILE_ATTRIBUTE_DIRECTORY))
					//		m_SelectedItemList.Add(strItemText);
					//	else
					//	{
					//		map<CString,CString>::iterator iter = vDriverMap.find(strListText);
					//		if (iter!=vDriverMap.end())
					//		{
					//			CString strValue = iter->second;
					//			m_SelectedItemList.Add(strValue);
					//		}
					//	}
					//	nItem = wndLst1->GetNextItem(nItem, LVNI_SELECTED);
					//}
					////////////////////   Add FILE names to list
					//strItemText = _T("");
					//nSelected = wndLst1->GetSelectedCount();
					//CString fileslist = _T("");
					//pDlg->SendMessage(CDM_GETSPEC, (WPARAM)MAX_PATH * SELECT_MAX_FILE_NUMBER,
					//	(LPARAM)fileslist.GetBuffer(MAX_PATH * SELECT_MAX_FILE_NUMBER));
					//fileslist.ReleaseBuffer();

					//if(nSelected > m_SelectedItemList.GetCount())
					//{
					//	int MoreThanOnFile = fileslist.Find(_T("\""));
					//	if(MoreThanOnFile != -1)
					//	{
					//		for(int i=0; i<fileslist.GetLength(); i++)
					//			if(fileslist[i] != '\"')
					//			{
					//				strItemText.AppendFormat(_T("%c"),fileslist[i]);
					//				if(fileslist[i-1] == '\"' && fileslist[i] == ' ')
					//					strItemText.Delete(strItemText.GetLength()-1);
					//			}
					//			else if(!strItemText.IsEmpty())
					//			{
					//				m_SelectedItemList.Add((strDirectory+strItemText));
					//				strItemText.Empty();
					//			}
					//	}
					//	else
					//		m_SelectedItemList.Add(strDirectory+fileslist);
					//}
					//pDlg->EndDialog(IDOK);
					//return NULL;
				} // if IDOK
			}
		} // if BN_CLICKED
	}// if WM_COMMAND
	return CallWindowProc(m_wndProc, hwnd, message, wParam, lParam);
}
CString CFileSelectDlg::GetDisplayNameOf(LPCITEMIDLIST pidl, DWORD flags)
{
	int len = GetParent()->SendMessage(CDM_GETFOLDERIDLIST, 0,NULL);
	if (len<=0)
		return "";

	CComQIPtr<IMalloc> imalloc;
	VERIFY(SUCCEEDED(SHGetMalloc(&imalloc)));
	LPCITEMIDLIST pidlFolder = (LPCITEMIDLIST)imalloc->Alloc(len);
	ASSERT(pidlFolder);
	GetParent()->SendMessage(CDM_GETFOLDERIDLIST, len,(LPARAM)(void*)pidlFolder);

	// Now get IShellFolder for pidlFolder
	CComQIPtr<IShellFolder> ishDesk;
	SHGetDesktopFolder(&ishDesk);
	CComQIPtr<IShellFolder> ishFolder;
	HRESULT hr = ishDesk->BindToObject(pidlFolder, NULL,IID_IShellFolder, (void**)&ishFolder);
	if (!SUCCEEDED(hr))
	{
		ishFolder = ishDesk;
	}

	CString strPath;
	STRRET str;
	if (NO_ERROR==ishFolder->GetDisplayNameOf(pidl, SHGDN_NORMAL|SHGDN_FORPARSING, &str))
		strPath = str.pOleStr;

	SHFree(imalloc);
	return strPath;
}

int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData)
{
	switch(uMsg)
	{
	case BFFM_INITIALIZED:
		::SendMessage(hwnd,BFFM_SETSELECTION,TRUE,lpData);
		HWND hFind = ::GetWindow(hwnd,GW_CHILD);
		while(hFind)
		{
			char szName[256] ={0};
			GetClassName(hFind,szName,256);
			if (_stricmp(szName,"Edit")==0)
			{
				CEdit *pEdit = (CEdit*)CWnd::FromHandle(hFind);
				//pEdit->EnableWindow(FALSE);
				pEdit->SetReadOnly(TRUE); //将控件设置为只读，方便复制，但不允许修改
				break;
			}
			hFind = ::GetWindow(hFind,GW_HWNDNEXT);
		}

		break;
	}
	return 0;
}
CString GetSelectedFolder()
{
	BROWSEINFO bi;  
	ZeroMemory(&bi,sizeof(BROWSEINFO)); 
	bi.hwndOwner = GetActiveWindow();   
	bi.lpszTitle = "请选择目录：";  

	static CString strLastDir = "";
	bi.ulFlags = BIF_USENEWUI | BIF_RETURNONLYFSDIRS;
	bi.lpfn = BrowseCallbackProc;
	bi.lParam = (LPARAM)(LPCTSTR)strLastDir;


	LPMALLOC pMalloc;  

	if(::GetCapture())  
		::ReleaseCapture(); 
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);  

	if(!pidl )
	{
		return "";
	}

	CString strFolderOut = "";
	//TCHAR * path = new TCHAR[MAX_PATH];   
	WCHAR * path = new WCHAR[MAX_PATH];  
	SHGetPathFromIDListW(pidl,path);
	//SHGetPathFromIDList(pidl,path);  
	if(SUCCEEDED(SHGetMalloc(&pMalloc))) 
	{  
		pMalloc->Free(pidl);  
		pMalloc->Release();  
	}

	strFolderOut = path;
	delete [] path;
	if (!strFolderOut.IsEmpty())
		strLastDir = strFolderOut; // 记住上次打开的目录
	else
		return "";

	return strFolderOut;
}