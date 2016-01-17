#pragma once
#include <vector>
#include <map>
#include "NetDiskOperation.h"
using namespace std;
// CVirtualList
class CListCtrlEx : public CListCtrl
{
	DECLARE_DYNAMIC(CListCtrlEx)

public:
	CListCtrlEx();
	virtual ~CListCtrlEx();
	virtual void InsertAll(const vector<CFileInfo> &);
	virtual void InsertItemEx(const CFileInfo &);
	virtual CString GetItemTextEx(int iItem,int iSubItem);
	virtual void SortItems(int iByColum);
	void   UpdateProgress(double dProgress,CString strOperation,CString strTaskKey);
	void   SetItemFileId(CString strFileName,CString strFileId);
	void   ClearProgressMsg(CString strTaskKey);
	void   SetProgressText(CString strText,CString strTaskKey);
	void   SetNetDiskDir(CString);
	const CString & GetNetDiskDir() { return m_strNetDiskDir;}
protected:
	virtual void DrawItem(_In_ LPDRAWITEMSTRUCT lpDrawItemStruct);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	afx_msg void OnLvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
public:
	vector<CFileInfo> m_FileInfoArry;
public:
	afx_msg void OnLvnOdcachehint(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
private:
	CString m_strNetDiskDir;
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
};

class CompareObject
{
public:
	CompareObject(int iSortBy)
	{
		m_iSortByCollum = iSortBy;
	}

	~CompareObject();
	bool operator()(const CFileInfo & str1,const CFileInfo &str2);
	static int m_iSortByCollum;
	static map<CString,CFileInfo *> m_CacheInfo;
	static int m_iLastSort[5];
};
