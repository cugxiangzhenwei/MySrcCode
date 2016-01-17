// 绘图程序Doc.cpp : implementation of the CMyDoc class
//

#include "stdafx.h"
#include "绘图程序.h"

#include "绘图程序Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyDoc

IMPLEMENT_DYNCREATE(CMyDoc, CDocument)

BEGIN_MESSAGE_MAP(CMyDoc, CDocument)
	//{{AFX_MSG_MAP(CMyDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyDoc construction/destruction

CMyDoc::CMyDoc()
{
	// TODO: add one-time construction code here
m_nLineNum=0;
m_nPointNum=0;

}

CMyDoc::~CMyDoc()
{
}

BOOL CMyDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
    m_nLineNum=0;            //初始化文档的数据称成员
	m_nPointNum=0;
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMyDoc serialization

void CMyDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())          //IsStoring()返回真，则存盘，假则读盘
	{
		// TODO: add storing code here
		ar<<m_nPointNum;
		for (int i=0;i<m_nPointNum;i++)
		{
			ar<<pointarry[i][0];
			ar<<pointarry[i][1];
		}
		ar<<m_nLineNum;
		for (int i=0;i<m_nLineNum;i++)
		{
			ar<<linearry[i][0]<<linearry[i][1]<<linearry[i][2]<<linearry[i][3];
		}
	}
	else
	{
		// TODO: add loading code here
		ar>>m_nPointNum;
		for (int i=0;i<m_nPointNum;i++)
		{
			ar>>pointarry[i][0];
			ar>>pointarry[i][1];
		}
		ar>>m_nLineNum;
		for (int i=0;i<m_nLineNum;i++)
		{
			ar>>linearry[i][0]>>linearry[i][1]>>linearry[i][2]>>linearry[i][3];
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMyDoc diagnostics

#ifdef _DEBUG
void CMyDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMyDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyDoc commands

void CMyDoc::DeleteContents() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_nLineNum=0;
	m_nPointNum=0;
	CDocument::DeleteContents();
}
