
// WordPdfViewDemoView.cpp : CWordPdfViewDemoView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "WordPdfViewDemo.h"
#endif

#include "WordDoc.h"
#include "WordView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "./word/CWordApplication.h"
#include "./word/CDocuments.h"
#include "./word/CDocument0.h"

BOOL DeleteDir(CString strDir)
{
	CString strWildCard  = strDir;
	strWildCard +="\\*.*";
	CFileFind finder;
	BOOL bFind = FALSE;
	// 查找文件
	bFind = finder.FindFile(strWildCard);
	while(bFind)
	{
		// 查找下一个文件
		bFind = finder.FindNextFileA();
		if (finder.IsDots())
			continue;

		//找到文件的路径
		CString strPathName = finder.GetFilePath();
		if (finder.IsDirectory())
		{
			DeleteDir(strPathName);
		}
		else
		{
			if (!::DeleteFile(strPathName))
			{
				return FALSE;
			}
		}
	}
	finder.Close();
	if (!::RemoveDirectory(strDir))
		return FALSE;

	return TRUE;
}
// CWordPdfViewDemoView

IMPLEMENT_DYNCREATE(CWordView, CView)

BEGIN_MESSAGE_MAP(CWordView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CWordView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_COMMAND(ID_OPEN_WORD_DOC, &CWordView::OnOpenWordDoc)
END_MESSAGE_MAP()

// CWordPdfViewDemoView 构造/析构

CWordView::CWordView()
{
	// TODO: 在此处添加构造代码

}

CWordView::~CWordView()
{
	RemoveTempHtmlFile();
}

BOOL CWordView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CWordPdfViewDemoView 绘制

void CWordView::OnDraw(CDC* /*pDC*/)
{
	CWordDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CWordPdfViewDemoView 打印


void CWordView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CWordView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CWordView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CWordView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CWordView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CWordView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CWordPdfViewDemoView 诊断

#ifdef _DEBUG
void CWordView::AssertValid() const
{
	CView::AssertValid();
}

void CWordView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWordDoc* CWordView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWordDoc)));
	return (CWordDoc*)m_pDocument;
}
#endif //_DEBUG


// CWordPdfViewDemoView 消息处理程序


void CWordView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	if (m_web.m_hWnd == NULL)
	{
		CRect rect;
		GetClientRect(&rect);
		m_web.Create(NULL,WS_CHILD|WS_VISIBLE,rect,this,ID_WEBBROWSER_DOC);
	}
	
	CString strWord = GetDocument()->GetPathName();
	if (!strWord.IsEmpty())
	{
		OpenDocFile(strWord);
	}
}

void CWordView::OpenDocFile(CString strFile)
{
	if (m_web.m_hWnd)
	{
		RemoveTempHtmlFile();
		CString strHtml = strFile;
		if (IsWordFile(strFile))
		{
			strHtml = strFile.Left(strFile.ReverseFind('.'))+ ".html";
			char szTmpBuf[256] = {0};
			char szFileName[256] = {0};
			::GetTempPath(256,szTmpBuf);
			CString strTmp = szTmpBuf;
			::GetTempFileName(szTmpBuf,"~htm",TRUE,szFileName);
			strHtml = szFileName;
			BOOL bOk = ConvertWordToHtml(strFile,strHtml);
			if (!bOk)
			{
				MessageBox("无法打开该文档","提示",MB_OK|MB_ICONERROR);
				return;
			}
			m_strLastTmpFile = strHtml;
		}
		m_web.Navigate(strHtml,NULL,NULL,NULL,NULL);
		GetDocument()->SetTitle(strFile);
	}
}
BOOL CWordView::IsWordFile(const CString &strFile)
{
	CString strTmp = strFile;
	strTmp.MakeLower();
	if (strFile.Right(3)=="doc"|| strFile.Right(4)=="docx")
		return TRUE;

	return FALSE;
}
BOOL CWordView::ConvertWordToHtml(CString strWordFile,CString strHtml)
{
	CWordApplication app;
	if (!app.CreateDispatch("Word.Application",NULL))
		return FALSE;

	CDocuments docxs = app.get_Documents();
	CComVariant FileName =  strWordFile;
	CComVariant passWd = "";
	CComVariant templateDoc = "";
	CComVariant wrietPassWdDoc = "";
	CComVariant wriePassWdtemplateDoc = "";
	CComVariant xmltrans = "";
	CComVariant vBoolTrue,vBoolFalse;
	vBoolTrue  = TRUE;
	vBoolFalse = FALSE;
	CComVariant vFormat = 0;
	CComVariant vEcode  = 0;
	CComVariant vDirection  = 0;
	app.put_Visible(FALSE);
	// 打开Word 文档
	CDocument0 docx = docxs.Open(&FileName,&vBoolFalse,&vBoolTrue,&vBoolTrue,
		&passWd,&templateDoc,&vBoolFalse,&wrietPassWdDoc,&wriePassWdtemplateDoc,&vFormat,&vEcode,&vBoolFalse,&vBoolTrue,&vDirection,&vBoolTrue,&xmltrans);

	// 另存为html 页面
	FileName = strHtml;
	vFormat = 8;
	docx.SaveAs(&FileName,&vFormat,&vBoolFalse,&passWd,&vBoolFalse,&wrietPassWdDoc,&vBoolFalse,&vBoolFalse,&vBoolFalse,&vBoolFalse,&vBoolFalse,&vEcode,
		&vBoolFalse,&vBoolFalse,&vBoolFalse,&vBoolFalse);

	// 退出Word
	CComVariant vRotuneDoc = 0;
	vFormat = 0;
	app.Quit(&vBoolFalse,&vFormat,&vRotuneDoc);
	return TRUE;
}
void CWordView::RemoveTempHtmlFile()
{
	if (m_strLastTmpFile.IsEmpty())
		return;

	::DeleteFile(m_strLastTmpFile);
	CString strFileDir = m_strLastTmpFile.Left(m_strLastTmpFile.ReverseFind('.')) + ".files";
	DeleteDir(strFileDir);
}

void CWordView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_web.m_hWnd)
	{
		CRect rect;
		GetClientRect(&rect);
		m_web.SetWindowPos(&wndTop,rect.left,rect.top,rect.Width(),rect.Height(),NULL);
		m_web.RedrawWindow();
	}
}


void CWordView::OnOpenWordDoc()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog dlg(TRUE,".docx",NULL,6UL,"支持的格式|*.doc;*.docx;*.pdf;*.htm;*.html;*.xml;*.txt;*.bmp;*.jpg;*.png|\
		Word 文档(*.doc;*.docx)|*.doc;*.docx|\
		PDF文档(*.pdf)|*.pdf|\
		网页文件(*.html;*.html)|*.html;*.html|\
		图片文件(*.jpg;*.bmp)|*.jpg;*.bmp|\
		文本文件(*.txt)|*.txt|\
		XML文件(*.xml)|*.xml||",this);

	if (dlg.DoModal()!=IDOK)
		return;

	CString strFile = dlg.GetPathName();
	OpenDocFile(strFile);
}
