
// StaffManageDoc.cpp : CStaffManageDoc 类的实现
//

#include "stdafx.h"
#include "StaffManage.h"

#include "StaffManageSet.h"
#include "StaffManageDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStaffManageDoc

IMPLEMENT_DYNCREATE(CStaffManageDoc, CDocument)

BEGIN_MESSAGE_MAP(CStaffManageDoc, CDocument)
END_MESSAGE_MAP()


// CStaffManageDoc 构造/析构

CStaffManageDoc::CStaffManageDoc()
{
	// TODO: 在此添加一次性构造代码

}

CStaffManageDoc::~CStaffManageDoc()
{
}

BOOL CStaffManageDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CStaffManageDoc 诊断

#ifdef _DEBUG
void CStaffManageDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CStaffManageDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CStaffManageDoc 命令
