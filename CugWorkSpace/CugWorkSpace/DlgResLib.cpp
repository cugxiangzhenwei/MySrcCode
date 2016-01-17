// DlgResLib.cpp : 实现文件
//

#include "stdafx.h"
#include "CugWorkSpace.h"
#include "DlgResLib.h"
#include "afxdialogex.h"
#include "UserDefine.h"
#include "MineSplitterView.h"

// CDlgResLib 对话框

IMPLEMENT_DYNAMIC(CDlgResLib, CDialogEx)

CDlgResLib::CDlgResLib(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgResLib::IDD, pParent)
{
	m_pFrameWnd = NULL;
}

CDlgResLib::~CDlgResLib()
{
}

void CDlgResLib::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgResLib, CDialogEx)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDlgResLib 消息处理程序


BOOL CDlgResLib::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect rcClient;
	GetClientRect(&rcClient);

	//m_wndSplitter.SubclassWindow(m_wndSplitter.GetSafeHwnd());
	//m_pFrameWnd->ModifyStyle(0, WS_CLIPSIBLINGS|WS_CLIPCHILDREN);
	m_pFrameWnd->MoveWindow(&rcClient); 
	m_pFrameWnd->ShowWindow(SW_SHOW); 

	//m_wndSplitter.ModifyStyle(WS_CLIPCHILDREN, 0); //Splitter 不能有 WS_CLIPCHILDREN 属性
	m_wndSplitter.MoveWindow(0,0, rcClient.Width(), rcClient.Height()); 
	m_wndSplitter.ShowWindow(SW_SHOW);

	m_wndSplitter.SetColumnInfo(0, rcClient.Width() / 4, 1);
	m_wndSplitter.SetColumnInfo(1, rcClient.Width() * 3 / 4, 1);
	m_wndSplitter.RecalcLayout();

	ModifyStyle(DS_3DLOOK, 0);
	ModifyStyleEx(WS_EX_CLIENTEDGE, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgResLib::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CRect rcClient;
	GetClientRect(&rcClient);
	//rcClient.left += -2;
	//rcClient.top += -1;
	//rcClient.right += 4;
	//rcClient.bottom += 3;

	m_pFrameWnd->MoveWindow(&rcClient);
	m_pFrameWnd->ShowWindow(SW_SHOW);
	m_wndSplitter.MoveWindow(0,0, rcClient.Width(), rcClient.Height());

	if ( cx < 330 + 10 )
	{
		m_wndSplitter.SetColumnInfo(0, cx / 4, 1);
		m_wndSplitter.SetColumnInfo(1, cx * 3 / 4, 1);
	}
	else
	{
		m_wndSplitter.SetColumnInfo(0, 330 - 2, 1);
		m_wndSplitter.SetColumnInfo(1, cx - 330 - 2, 1);
	}

	m_wndSplitter.RecalcLayout();
}


int CDlgResLib::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Initialize a context for the view. CMyTreeView is my view and 
	// is defined as :  class CMyTreeView : public CTreeView. 
	CCreateContext ccc; 
	ccc.m_pNewViewClass      = RUNTIME_CLASS(CMineSplitterView); 
	ccc.m_pCurrentDoc           = NULL; 
	ccc.m_pNewDocTemplate = NULL; 
	ccc.m_pLastView              = NULL; 
	ccc.m_pCurrentFrame       = NULL; 

	// Because the CFrameWnd needs a window class, we will create 
	// a new one. I just copied the sample from MSDN Help. 
	// When using it in your project, you may keep CS_VREDRAW and 
	// CS_HREDRAW and then throw the other three parameters. 
	CString strMyClass = AfxRegisterWndClass(CS_VREDRAW | 
		CS_HREDRAW, 
		::LoadCursor(NULL, IDC_ARROW), 
		(HBRUSH) ::GetStockObject(WHITE_BRUSH), 
		::LoadIcon(NULL, IDI_APPLICATION)); 

	// Create the frame window with "this" as the parent 
	m_pFrameWnd = new CFrameWnd(); 
	m_pFrameWnd->Create(strMyClass,"", WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 
		CRect(0,0,1,1), this); 
	m_pFrameWnd->ShowWindow(SW_SHOW); 
	m_pFrameWnd->MoveWindow(0,0,300,300); 

	m_wndSplitter.CreateStatic (m_pFrameWnd, 1, 2
		, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN
		, ID_JIANMO_COMPONENT_SPLITTER);

	theApp.m_nSpliterViewFlag = theApp.SPLVF_COMPONENT_TREE;
	m_wndSplitter.CreateView (0, 0, RUNTIME_CLASS (CMineSplitterView)
		, CSize (100, 100), &ccc);

	theApp.m_nSpliterViewFlag = theApp.SPLVF_COMPONENT_LIST;
	m_wndSplitter.CreateView (0, 1, RUNTIME_CLASS (CMineSplitterView), 
		CSize (200, 100), &ccc);

	m_wndSplitter.GetPane(0, 0)->ModifyStyle(NULL, WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	m_wndSplitter.GetPane(0, 1)->ModifyStyle(NULL, WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	theApp.m_nSpliterViewFlag = -1;
	return 0;
}


void CDlgResLib::OnDestroy()
{
	if ( m_pFrameWnd != NULL )
	{
		if ( IsWindow( m_pFrameWnd->GetSafeHwnd() ) )
		{
			m_pFrameWnd->DestroyWindow();
		}
		//Do not use the C++ delete operator to destroy a frame window. Use CWnd::DestroyWindow instead. The CFrameWnd implementation of PostNcDestroy will delete the C++ object when the window is destroyed. When the user closes the frame window, the default OnClose handler will call DestroyWindow.
		//delete m_pFrameWnd; 
		m_pFrameWnd = NULL;
	}
	//m_wndSplitter.UnsubclassWindow();
	CDialogEx::OnDestroy();
}


BOOL CDlgResLib::PreTranslateMessage(MSG* pMsg)
{
	ASSERT_VALID(this);
	ASSERT(m_hWnd != NULL);

	CWnd* pOwner = GetOwner();

	// don't translate dialog messages when in Shift+F1 help mode
	CFrameWnd* pFrameWnd = GetTopLevelFrame();
	if (pFrameWnd != NULL && pFrameWnd->m_bHelpMode)
		return FALSE;

	// since 'IsDialogMessage' will eat frame window accelerators,
	//   we call all frame windows' PreTranslateMessage first
	while (pOwner != NULL)
	{
		// allow owner & frames to translate before IsDialogMessage does
		if (pOwner->PreTranslateMessage(pMsg))
			return TRUE;

		// try parent frames until there are no parent frames
		if (IsWindow (pOwner->GetSafeHwnd ()))
		{
			pOwner = pOwner->GetParentFrame();
		}
		else
		{
			break;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}