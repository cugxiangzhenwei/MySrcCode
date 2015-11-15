#pragma once
#include <vector>
#include <map>
using namespace std;
// CVirtualList
class CListCtrlEx : public CListCtrl
{
	DECLARE_DYNAMIC(CListCtrlEx)

public:
	CListCtrlEx();
	virtual ~CListCtrlEx();
	virtual void InsertAll(int iTemCount,int ColumCount,char ** papszData);
	virtual CString GetItemTextEx(int iItem,int iSubItem);
	virtual void SortItems(int iByColum);
protected:
	virtual void DrawItem(_In_ LPDRAWITEMSTRUCT lpDrawItemStruct);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	afx_msg void OnLvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
private:
	vector<CString> m_strFileArry;
public:
	afx_msg void OnLvnOdcachehint(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult);
};

class CFileInfo;
class CompareObject
{
public:
	CompareObject(int iSortBy)
	{
		m_iSortByCollum = iSortBy;
	}

	~CompareObject();
	bool operator()(const CString & str1,const CString &str2);
	static void DelCache();
	static int m_iSortByCollum;
	static map<CString,CFileInfo *> m_CacheInfo;
	static int m_iLastSort[5];
};
