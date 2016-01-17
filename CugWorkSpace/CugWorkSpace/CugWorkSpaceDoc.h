// CugWorkSpaceDoc.h : interface of the CCugWorkSpaceDoc class
//


#pragma once
class CMineDocHelper
{
	static CString m_strDocTitle;
};

class CCugWorkSpaceDoc : public CDocument
{
protected: // create from serialization only
	CCugWorkSpaceDoc();
	DECLARE_DYNCREATE(CCugWorkSpaceDoc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CCugWorkSpaceDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


