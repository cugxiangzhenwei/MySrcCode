// CugWorkSpaceDoc.cpp : implementation of the CCugWorkSpaceDoc class
//

#include "stdafx.h"
#include "CugWorkSpace.h"

#include "CugWorkSpaceDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCugWorkSpaceDoc

IMPLEMENT_DYNCREATE(CCugWorkSpaceDoc, CDocument)

BEGIN_MESSAGE_MAP(CCugWorkSpaceDoc, CDocument)
END_MESSAGE_MAP()


// CCugWorkSpaceDoc construction/destruction

CCugWorkSpaceDoc::CCugWorkSpaceDoc()
{
	// TODO: add one-time construction code here

}

CCugWorkSpaceDoc::~CCugWorkSpaceDoc()
{
}

BOOL CCugWorkSpaceDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	this->SetTitle(theApp.m_strNewDocTitle);
	theApp.m_strNewDocTitle = "";
	return TRUE;
}




// CCugWorkSpaceDoc serialization

void CCugWorkSpaceDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CCugWorkSpaceDoc diagnostics

#ifdef _DEBUG
void CCugWorkSpaceDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCugWorkSpaceDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CCugWorkSpaceDoc commands
