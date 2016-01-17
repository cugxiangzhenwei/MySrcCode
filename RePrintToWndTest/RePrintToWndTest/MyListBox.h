#pragma once


// CMyListBox

class CMyListBox : public CListBox
{
	DECLARE_DYNAMIC(CMyListBox)

public:
	CMyListBox();
	virtual ~CMyListBox();
	 int   AddString(   LPCTSTR   lpszItem   );  
	 void   CMyListBox::ResetContent();  

protected:
	DECLARE_MESSAGE_MAP()
private:
	  int           m_nMaxWidth;
};


