
// TreeCtrlDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TreeCtrlDemo.h"
#include "TreeCtrlDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTreeCtrlDemoDlg 对话框




CTreeCtrlDemoDlg::CTreeCtrlDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTreeCtrlDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTreeCtrlDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE, m_TreeCtrl);
	DDX_Control(pDX, IDC_LIST, m_ListBox);
}

BEGIN_MESSAGE_MAP(CTreeCtrlDemoDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_DEL_SEl, &CTreeCtrlDemoDlg::OnBnClickedButtonDelSel)
	ON_BN_CLICKED(IDC_BUTTON_DEL_ALL, &CTreeCtrlDemoDlg::OnBnClickedButtonDelAll)
	ON_BN_CLICKED(IDC_BUTTON_RESET_TREE, &CTreeCtrlDemoDlg::OnBnClickedButtonResetTree)
	ON_BN_CLICKED(IDC_BUTTON_FOREACH, &CTreeCtrlDemoDlg::OnBnClickedButtonForeach)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, &CTreeCtrlDemoDlg::OnTvnSelchangedTree)
END_MESSAGE_MAP()


// CTreeCtrlDemoDlg 消息处理程序

BOOL CTreeCtrlDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	InitTree();
	//m_TreeCtrl.Expand(m_TreeCtrl.GetRootItem(),TVE_EXPAND);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTreeCtrlDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTreeCtrlDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTreeCtrlDemoDlg::OnBnClickedButtonDelSel()
{
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM hSelItem = m_TreeCtrl.GetSelectedItem();
	if (m_TreeCtrl.ItemHasChildren(hSelItem))
	{
		if(MessageBox(_T("选中节点有子节点,确实要删除该节点?"),_T("提示"),MB_YESNO) == IDNO)
			return;
	}
	m_TreeCtrl.DeleteItem(hSelItem);
}

void CTreeCtrlDemoDlg::OnBnClickedButtonDelAll()
{
	// TODO: 在此添加控件通知处理程序代码
	m_TreeCtrl.DeleteAllItems();
}

void CTreeCtrlDemoDlg::OnBnClickedButtonResetTree()
{
	// TODO: 在此添加控件通知处理程序代码
	m_TreeCtrl.DeleteAllItems();
	InitTree();
}

void CTreeCtrlDemoDlg::OnBnClickedButtonForeach()
{
	// TODO: 
	m_ListBox.ResetContent();
	HTREEITEM hRoot = m_TreeCtrl.GetRootItem();
	CString strText = m_TreeCtrl.GetItemText(hRoot);
	m_ListBox.AddString(strText);
	SearchTree(hRoot,1);
}
void CTreeCtrlDemoDlg::InitTree()
{
	m_TreeCtrl.ModifyStyle(0,TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS);//为树添加连线
	CString strText = _T("Root");
	HTREEITEM hRoot = m_TreeCtrl.InsertItem(strText,TVI_ROOT);
	for (int i=0;i<4;i++)
	{
		strText.Format(_T("Item %d"),i);
		HTREEITEM hParent = m_TreeCtrl.InsertItem(strText,hRoot,TVI_LAST);
		for (int j=0;j<4;j++)
		{
			strText.Format(_T("SubItem %d %d"),i,j);
			HTREEITEM hParent2 = m_TreeCtrl.InsertItem(strText,hParent,TVI_LAST);
		}
		m_TreeCtrl.Expand(hParent,TVE_EXPAND); //展开该节点
	}
	m_TreeCtrl.Expand(hRoot,TVE_EXPAND); //展开根节点
}

void CTreeCtrlDemoDlg::SearchTree(HTREEITEM hParent,int iStrOffset)
{
	CString strText = _T("");
	HTREEITEM hChild = NULL;
	if (!hParent)
		return;

	hChild = m_TreeCtrl.GetChildItem(hParent);
	while (hChild)
	{
		strText = m_TreeCtrl.GetItemText(hChild);
		for (int i=0;i< iStrOffset;i++)
			strText.Insert(0,_T(" "));

		m_ListBox.AddString(strText);
		if (m_TreeCtrl.ItemHasChildren(hChild))
			SearchTree(hChild,iStrOffset+1); // 遍历下级子节点

		hChild = m_TreeCtrl.GetNextItem(hChild,TVGN_NEXT);
	}
}
void CTreeCtrlDemoDlg::OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
