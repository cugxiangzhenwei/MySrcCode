// DCompression.cpp : 实现文件
//

#include "stdafx.h"
#include "7zClientUI.h"
#include "DCompressionDlg.h"
#include "afxdialogex.h"
#include "7zInterface.h"
#include "FileSelectDlg.h"

static bool m_bCancel = false;
#define RE_USER_CANCEL  9999
#define WM_DCOMPRESS_FINISH_MSG WM_USER+2

class CMyProgressD: public CArchiveExtractCallback//定义解压缩进度条类
{
public:
	CMyProgressD(CProgressCtrl *bar):CArchiveExtractCallback()
	{
		m_ProgressBar = bar;
	}
	virtual ~CMyProgressD()
	{
	}
	CProgressCtrl * GetProgresCtrl()
	{
		return m_ProgressBar;
	}
	STDMETHOD(SetTotal)(UInt64 size)
	{
		m_iTotal64 = size;
		m_iTotal32 = m_iTotal64/1000;
		m_ProgressBar->SetRange32(0,m_iTotal32);
		m_bCancel = false; // 将全局变量取消设置为false
		return S_OK;
	}
	STDMETHOD(SetCompleted)(const UInt64 *completeValue)
	{
		if (m_bCancel)
			return RE_USER_CANCEL; // 捕捉到用户取消的消息，返回用户取消

		int iPos = *completeValue / 1000;
		if (*completeValue == m_iTotal64)
			iPos = m_iTotal32 ;

		m_ProgressBar->SetPos(iPos);
		return S_OK;
	}
private:
	CProgressCtrl *m_ProgressBar;
	UInt64 m_iTotal64;
	int    m_iTotal32;
};

typedef struct _TagThreadDataDCompress
{
	_TagThreadDataDCompress()
	{
		m_OutPath.Empty();
		m_InFile.Empty();
		m_PassWd.Empty();
		m_pProgress  = NULL;
		m_hWndRecvMsg = NULL;
	}
	UString m_InFile;
	UString m_OutPath;
	UString m_PassWd;
	int    m_iCompressLevel;
	CMyProgressD *m_pProgress;
	HWND   m_hWndRecvMsg;
}ThreadDataDCompress;

DWORD WINAPI ThreadDCompressFun(LPVOID pData)
{
	_TagThreadDataDCompress *pComressDataD = (_TagThreadDataDCompress*)(pData);
	CProgressCtrl *pProgressCtrl = pComressDataD->m_pProgress->GetProgresCtrl();

	int iRev = decompress(pComressDataD->m_InFile,
		pComressDataD->m_OutPath,
		pComressDataD->m_PassWd,
		pComressDataD->m_pProgress);

	if (iRev == 0)
	{
		int iRange[2]={0};
		pProgressCtrl->GetRange(iRange[0],iRange[1]);
		pProgressCtrl->SetPos(iRange[1]);
	}

	if (pComressDataD->m_hWndRecvMsg)
		::PostMessage(pComressDataD->m_hWndRecvMsg,WM_DCOMPRESS_FINISH_MSG,iRev,NULL);

	delete pComressDataD;
	return 0;
}

// CDCompression 对话框

IMPLEMENT_DYNAMIC(CDCompressionDlgPage, CDialogEx)

CDCompressionDlgPage::CDCompressionDlgPage(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDCompressionDlgPage::IDD, pParent)
{

}

CDCompressionDlgPage::~CDCompressionDlgPage()
{
}

void CDCompressionDlgPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FILE_LIST_INNER, m_FileList);
	DDX_Control(pDX, IDC_PROGRESS_DCOM, m_bProgressBar);
}


BEGIN_MESSAGE_MAP(CDCompressionDlgPage, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_OPEN_7ZFILE, &CDCompressionDlgPage::OnBnClickedBtnOpen7zfile)
	ON_BN_CLICKED(IDC_BTN_EXTRACT_TO, &CDCompressionDlgPage::OnBnClickedBtnExtractTo)
	ON_MESSAGE(WM_DCOMPRESS_FINISH_MSG,&CDCompressionDlgPage::OnMsgDCompressFinish)
END_MESSAGE_MAP()


// CDCompression 消息处理程序


void CDCompressionDlgPage::OnBnClickedBtnOpen7zfile()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE,NULL,NULL,6UL,"7z压缩文件(*.7z)|*.7z||",this,0,FALSE);
	if (dlg.DoModal()!=IDOK)
		return;

	CString str = dlg.GetPathName();
	UStringVector VectorStrings;
	UString InFile;
	Char2UString(InFile,str);
	//取出压缩包内的压缩文件名列表
	showlist(InFile,VectorStrings);
	for(int i = 0;i<VectorStrings.Size();i++)
	{
		CStringW wstr = VectorStrings[i];
		CString strTmp = (LPCTSTR)_bstr_t(wstr);
		m_FileList.AddString(strTmp);
	}
	m_strIn7zFile = str;
}


void CDCompressionDlgPage::OnBnClickedBtnExtractTo()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strText;
	GetDlgItem(IDC_BTN_EXTRACT_TO)->GetWindowText(strText);
	if (strText =="取消")
	{
		int iRev = MessageBox("是否要取消解压？","提示",MB_YESNO|MB_ICONQUESTION);
		if (iRev==IDYES)
		{
			m_bCancel = true;
			GetDlgItem(IDC_BTN_EXTRACT_TO)->EnableWindow(FALSE);
		}
		return;
	}

	CString strOutPath = GetSelectedFolder();
	if (strOutPath.IsEmpty())
		return;

	if (strOutPath.Right(1)!="\\")
		strOutPath.AppendChar('\\');

	UString InFile,OutPath;
	Char2UString(InFile,m_strIn7zFile);
	Char2UString(OutPath,strOutPath);


	//解压缩开始走进度条
	CMyProgressD* pProgress = new CMyProgressD(&m_bProgressBar);

	ThreadDataDCompress *pThreadData = new ThreadDataDCompress;
	pThreadData->m_InFile = InFile;
	pThreadData->m_OutPath = OutPath;
	pThreadData->m_pProgress = pProgress;
	pThreadData->m_PassWd = UString(L"");
	pThreadData->m_hWndRecvMsg = GetSafeHwnd();
	GetDlgItem(IDC_BTN_OPEN_7ZFILE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_EXTRACT_TO)->SetWindowText("取消");
	CreateThread(NULL,0,ThreadDCompressFun,pThreadData,0,NULL);

	int iRev = decompress(InFile,OutPath,L"",pProgress);//解压缩主程序
}

LRESULT CDCompressionDlgPage::OnMsgDCompressFinish(WPARAM wp,LPARAM)
{
	if (wp == 0)
	{
		MessageBox("解压完成！","提示",MB_OK|MB_ICONINFORMATION);
	}
	else if (wp == RE_USER_CANCEL)
	{
		m_bProgressBar.SetPos(0);
		MessageBox("用户取消解压！","提示",MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox("文件解压失败！","提示",MB_OK|MB_ICONINFORMATION);
	}
	GetDlgItem(IDC_BTN_OPEN_7ZFILE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_EXTRACT_TO)->SetWindowText("解压到>>");
	GetDlgItem(IDC_BTN_EXTRACT_TO)->EnableWindow(TRUE);
	return S_OK;
}
BOOL CDCompressionDlgPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_bProgressBar.SendMessage (PBM_SETBARCOLOR, 0, RGB(0,200,0));
	m_bProgressBar.SendMessage (PBM_SETBKCOLOR, 0, RGB(200,200,200));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


BOOL CDCompressionDlgPage::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			CString strText;
			GetDlgItem(IDC_BTN_EXTRACT_TO)->GetWindowText(strText);
			if (strText =="取消")
			{
				int iRev = MessageBox("是否要取消解压？","提示",MB_YESNO|MB_ICONQUESTION);
				if (iRev==IDYES)
				{
					m_bCancel = true;
					GetDlgItem(IDC_BTN_EXTRACT_TO)->EnableWindow(FALSE);
				}
			}
			return TRUE;
		}
		else if (pMsg->wParam == VK_RETURN)
			return TRUE;
		else if (pMsg->wParam == VK_SPACE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
