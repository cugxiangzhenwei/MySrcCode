
// WordPdfViewDemoDoc.cpp : CWordPdfViewDemoDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "WordPdfViewDemo.h"
#endif

#include "PdfDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWordPdfViewDemoDoc

IMPLEMENT_DYNCREATE(CPdfDoc, CDocument)

BEGIN_MESSAGE_MAP(CPdfDoc, CDocument)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CPdfDoc, CDocument)
END_DISPATCH_MAP()

// 注意: 我们添加了对 IID_IWordPdfViewDemo 的支持
//  以支持来自 VBA 的类型安全绑定。此 IID 必须同附加到 .IDL 文件中的
//  调度接口的 GUID 匹配。

// {EAC5604D-2E09-41AB-B632-DCD3D12194C1}
static const IID IID_IWordPdfViewDemo =
{ 0xEAC5604D, 0x2E09, 0x41AB, { 0xB6, 0x32, 0xDC, 0xD3, 0xD1, 0x21, 0x94, 0xC1 } };

BEGIN_INTERFACE_MAP(CPdfDoc, CDocument)
	INTERFACE_PART(CPdfDoc, IID_IWordPdfViewDemo, Dispatch)
END_INTERFACE_MAP()


// CWordPdfViewDemoDoc 构造/析构

CPdfDoc::CPdfDoc()
{
	// TODO: 在此添加一次性构造代码

	EnableAutomation();

	AfxOleLockApp();
}

CPdfDoc::~CPdfDoc()
{
	AfxOleUnlockApp();
}

BOOL CPdfDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CWordPdfViewDemoDoc 序列化

void CPdfDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CPdfDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CPdfDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:  strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CPdfDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CWordPdfViewDemoDoc 诊断

#ifdef _DEBUG
void CPdfDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPdfDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CWordPdfViewDemoDoc 命令
