// Compression.cpp : 实现文件
//

#include "stdafx.h"
#include "7zClientUI.h"
#include "CompressionDlg.h"
#include "afxdialogex.h"
#include "FileSelectDlg.h"
#include "7zInterface.h"

static bool m_bCancel = false;
#define RE_USER_CANCEL  9999
#define WM_COMPRESS_FINISH_MSG WM_USER+1

class CMyProgress: public CArchiveUpdateCallback//定义压缩进度条类
{
	friend CCompressionDlgPage;
public:
	CMyProgress(CProgressCtrl *bar):CArchiveUpdateCallback()
	{
		m_ProgressBar = bar;
	}
	virtual ~CMyProgress()
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

typedef struct _TagThreadDataCompress
{
	_TagThreadDataCompress()
	{
		m_OutPath.Empty();
		m_PassWd.Empty();
		m_vFileList.Clear();
		m_iCompressLevel = 0;
		m_pProgress  = NULL;
		m_hWndRecvMsg = NULL;
	}
	UString m_OutPath;
	UStringVector m_vFileList;
	UString m_PassWd;
	int    m_iCompressLevel;
	CMyProgress *m_pProgress;
	HWND   m_hWndRecvMsg;
}ThreadDataCompress;

DWORD WINAPI ThreadCompressFun(LPVOID pData)
{
	ThreadDataCompress *pComressData = (ThreadDataCompress*)(pData);
	CProgressCtrl *pProgressCtrl = pComressData->m_pProgress->GetProgresCtrl();

	int iRev = compress(pComressData->m_OutPath,
		pComressData->m_vFileList,
		pComressData->m_PassWd,
		pComressData->m_iCompressLevel,
		pComressData->m_pProgress);

	if (iRev == 0)
	{
		int iRange[2]={0};
		pProgressCtrl->GetRange(iRange[0],iRange[1]);
		pProgressCtrl->SetPos(iRange[1]);
	}
	if (iRev == RE_USER_CANCEL)
		;
		//remove(pComressData->m_OutPath);

	if (pComressData->m_hWndRecvMsg)
		::PostMessage(pComressData->m_hWndRecvMsg,WM_COMPRESS_FINISH_MSG,iRev,NULL);

	delete pComressData;
	return 0;
}
// CCompression 对话框

IMPLEMENT_DYNAMIC(CCompressionDlgPage, CDialogEx)

CCompressionDlgPage::CCompressionDlgPage(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCompressionDlgPage::IDD, pParent)
{

}

CCompressionDlgPage::~CCompressionDlgPage()
{
}

void CCompressionDlgPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_INPUT, m_FileList);
	DDX_Control(pDX, IDC_PROGRESS_COM, m_bProgressBar);
}


BEGIN_MESSAGE_MAP(CCompressionDlgPage, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_ADD_FILE, &CCompressionDlgPage::OnBnClickedBtnAddFile)
	ON_BN_CLICKED(IDC_BTN_EXCUTE_COMPRESS, &CCompressionDlgPage::OnBnClickedBtnExcuteCompress)
	ON_MESSAGE(WM_COMPRESS_FINISH_MSG,&CCompressionDlgPage::OnMsgCompressFinish)
END_MESSAGE_MAP()


// CCompression 消息处理程序


void CCompressionDlgPage::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}


void CCompressionDlgPage::OnBnClickedBtnAddFile()
{
	// TODO: 在此添加控件通知处理程序代码
	DWORD flags = OFN_ALLOWMULTISELECT|OFN_PATHMUSTEXIST;

	CFileSelectDlg dlg(TRUE,NULL,NULL,flags,"*.*||",NULL,0UL,FALSE);
	dlg.m_ofn.lpstrTitle   =   "选择要添加到压缩包的文件";                                    //至少允许选择100个文件
	dlg.m_ofn.lpstrFile = new TCHAR[MAX_PATH * SELECT_MAX_FILE_NUMBER]; //重新定义缓冲区大小          
	memset(dlg.m_ofn.lpstrFile, 0, MAX_PATH * SELECT_MAX_FILE_NUMBER);  //初始化定义的缓冲区
	dlg.m_ofn.nMaxFile = MAX_PATH*SELECT_MAX_FILE_NUMBER; // 指定缓冲区大小

	char szDir[MAX_PATH] = {0};
	GetModuleFileName(AfxGetInstanceHandle(),szDir,MAX_PATH);
	CString strConfigINI = szDir;
	strConfigINI.MakeLower();
	strConfigINI = strConfigINI.Left(strConfigINI.ReverseFind('.') + 1) + "ini";
	CHAR szDefaultpath[MAX_PATH];
	SHGetSpecialFolderPath(NULL,szDefaultpath,CSIDL_DESKTOPDIRECTORY,FALSE); // 找不到则默认为桌面路径
	CString strLastDir = "";
	GetPrivateProfileString("setting","LastSelectFolder",szDefaultpath,strLastDir.GetBuffer(512),512,strConfigINI);
	if (!PathFileExists(strLastDir))
		strLastDir = szDefaultpath; // 上次的路径不存在时，使用桌面路径
	dlg.m_ofn.lpstrInitialDir = strLastDir;
	strLastDir.ReleaseBuffer();

	if (dlg.DoModal() != IDOK)
	{
		delete[] dlg.m_ofn.lpstrFile;     //最后别忘了释放内存
		return;
	}

	strLastDir = CFileSelectDlg::m_strCurrendDirectory;
	WritePrivateProfileString("setting","LastSelectFolder",strLastDir,strConfigINI);
	int iSize = CFileSelectDlg::m_SelectedItemList.GetCount();

	for (int i=0;i<iSize;i++)
	{
		m_FileList.AddString(CFileSelectDlg::m_SelectedItemList[i]);
	}

	delete[] dlg.m_ofn.lpstrFile;     //最后别忘了释放内存
}


void CCompressionDlgPage::OnBnClickedBtnExcuteCompress()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strText;
	GetDlgItem(IDC_BTN_EXCUTE_COMPRESS)->GetWindowText(strText);
	if (strText =="取消")
	{
		int iRev = MessageBox("是否要取消压缩？","提示",MB_YESNO|MB_ICONQUESTION);
		if (iRev==IDYES)
		{
			m_bCancel = true;
			GetDlgItem(IDC_BTN_EXCUTE_COMPRESS)->EnableWindow(FALSE);
		}
		return;
	}
	CFileDialog dlg(FALSE,"7z",NULL,6UL,"*.7z||",this,0UL,FALSE);
	if (dlg.DoModal()!=IDOK)
		return;

	CString strFileOut = dlg.GetPathName();

	UString OutFilePath,InFilePath,PassWordCompress;
	UStringVector SwitchStrings;
	Char2UString(OutFilePath ,strFileOut);
	Char2UString(PassWordCompress,"");

	for (int i=0;i<m_FileList.GetCount();i++)
	{
		CString strFile;
		m_FileList.GetText(i,strFile);
		Char2UString(InFilePath ,strFile);
		SwitchStrings.Add(InFilePath);
	}

	CMyProgress* pProgress = new CMyProgress(&m_bProgressBar);
	ThreadDataCompress *pThreadData = new ThreadDataCompress;
	pThreadData->m_OutPath = OutFilePath;
	pThreadData->m_vFileList = SwitchStrings;
	pThreadData->m_pProgress = pProgress;
	pThreadData->m_iCompressLevel = 1;
	pThreadData->m_PassWd = UString(L"");
	pThreadData->m_hWndRecvMsg = GetSafeHwnd();
	GetDlgItem(IDC_BTN_ADD_FILE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_EXCUTE_COMPRESS)->SetWindowText("取消");
	CreateThread(NULL,0,ThreadCompressFun,pThreadData,0,NULL);
}


BOOL CCompressionDlgPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_bProgressBar.SendMessage (PBM_SETBARCOLOR, 0, RGB(0,200,0));
	m_bProgressBar.SendMessage (PBM_SETBKCOLOR, 0, RGB(200,200,200));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
LRESULT CCompressionDlgPage::OnMsgCompressFinish(WPARAM wp,LPARAM)
{
	if (wp == 0)
	{
		MessageBox("压缩完成！","提示",MB_OK|MB_ICONINFORMATION);
	}
	else if (wp == RE_USER_CANCEL)
	{
		m_bProgressBar.SetPos(0);
		MessageBox("用户压缩取消！","提示",MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox("文件压缩失败！","提示",MB_OK|MB_ICONINFORMATION);
	}
	GetDlgItem(IDC_BTN_ADD_FILE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_EXCUTE_COMPRESS)->SetWindowText("开始压缩");
	GetDlgItem(IDC_BTN_EXCUTE_COMPRESS)->EnableWindow(TRUE);
	return S_OK;
}

BOOL CCompressionDlgPage::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			CString strText;
			GetDlgItem(IDC_BTN_EXCUTE_COMPRESS)->GetWindowText(strText);
			if (strText =="取消")
			{
				int iRev = MessageBox("是否要取消压缩？","提示",MB_YESNO|MB_ICONQUESTION);
				if (iRev==IDYES)
				{
					m_bCancel = true;
					GetDlgItem(IDC_BTN_EXCUTE_COMPRESS)->EnableWindow(FALSE);
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
