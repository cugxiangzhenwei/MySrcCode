// LineStyleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "绘图程序.h"
#include "LineStyleDlg.h"
#include "MainFrm.h"
#include "绘图程序Doc.h"
#include "绘图程序View.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLineStyleDlg dialog


CLineStyleDlg::CLineStyleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLineStyleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLineStyleDlg)
	m_nLineStyle = -1;
	//}}AFX_DATA_INIT
}


void CLineStyleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLineStyleDlg)
	DDX_Control(pDX, IDC_COMBO_LINESTYLE, m_ComboBox);
	DDX_CBIndex(pDX, IDC_COMBO_LINESTYLE, m_nLineStyle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLineStyleDlg, CDialog)
	//{{AFX_MSG_MAP(CLineStyleDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_LINESTYLE, OnSelchangeComboLinestyle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLineStyleDlg message handlers

BOOL CLineStyleDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_ComboBox.AddString("空画笔");  //增加新的列表项
	m_ComboBox.SetCurSel(0); //设置当前选择项为第一项

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLineStyleDlg::OnSelchangeComboLinestyle() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);   //促使数据交换
	int linestyle;   //临时变量
	switch (m_nLineStyle)      //这里的m_nLineStyle是combobox的变量
	{
	case 1 :
		linestyle=PS_DOT;     //点线
		break;
     case 2 :
		 linestyle=PS_DASH; //虚线
		 break;
	 case 3 :
		 linestyle=PS_DASHDOT;  //点划线
		 break;
	 case 4 :
		 linestyle=PS_DASHDOTDOT;  //双点划线
		 break;
	 case 5:
		 linestyle=PS_NULL;      //空画笔
		 break;
	 default:
		 linestyle=PS_SOLID;      //其他情况都为实线
		 break;
	}
     CMainFrame *pmainframe=(CMainFrame *)GetParent();  //获取框架窗口指针
	 CMyView *pview=(CMyView*)pmainframe->GetActiveView();  //获取视图窗口指针
	 pview->m_nLineStyle=linestyle;    //访问视图的成员变量并赋值
}
