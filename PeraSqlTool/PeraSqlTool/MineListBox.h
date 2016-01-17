#pragma once


// CMineListBox

class CMineListBox : public CListBox
{
	DECLARE_DYNAMIC(CMineListBox)

public:
	CMineListBox();
	virtual ~CMineListBox();
	int AddString(LPCTSTR lpszItem);
	int DeleteString(UINT nIndex);
	int InsertString(int nIndex, LPCTSTR lpszItem);
	void ResetContent();
protected:
	int           m_nMaxWidth;
protected:
	DECLARE_MESSAGE_MAP()
};


