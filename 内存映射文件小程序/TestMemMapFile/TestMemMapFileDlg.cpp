
// TestMemMapFileDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestMemMapFile.h"
#include "TestMemMapFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTestMemMapFileDlg 对话框




CTestMemMapFileDlg::CTestMemMapFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestMemMapFileDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestMemMapFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestMemMapFileDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CTestMemMapFileDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CTestMemMapFileDlg 消息处理程序

BOOL CTestMemMapFileDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTestMemMapFileDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTestMemMapFileDlg::OnPaint()
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
HCURSOR CTestMemMapFileDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


const char* getline(const char* buf, int* len)   
{   
	const char* tmp = buf;   
	while(*tmp && (*tmp != 0x0d || *tmp != 0x0a)) ++tmp;    
	*len = tmp - buf;   
	if(*tmp == 0)   
		return NULL;   
	++tmp;   
	if(*tmp == 0x0d)   
	{   
		tmp += 2;   
		assert(*(tmp-1) == 0x0a);   
	}   
	else   
		++tmp;   
	return tmp;      
}      

void CTestMemMapFileDlg::OnBnClickedButton1()
{
	 CFileDialog fileDlg(TRUE, "*.txt", "*.txt", NULL, "文本文件 (*.txt)|*.txt|图像文件（*.tif *.jpg *.img）|*.tif|*.jpg|*.img||");   
    fileDlg.m_ofn.Flags |= OFN_FILEMUSTEXIST;   
    fileDlg.m_ofn.lpstrTitle = "通过内存映射文件读取数据";   
    if (fileDlg.DoModal() == IDOK)   
    {   
        HANDLE hFile=CreateFile(fileDlg.GetPathName(),GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);   
           
        SYSTEM_INFO sinf;   
        GetSystemInfo(&sinf);   
        DWORD dwAllocationGranularity = sinf.dwAllocationGranularity;   
           
        HANDLE hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);   
        DWORD dwFileSizeHigh;   
        __int64 qwFileSize = GetFileSize(hFile, &dwFileSizeHigh);   
        qwFileSize += (((__int64) dwFileSizeHigh) << 32);   
           
        CloseHandle(hFile);//We nolonger need access to the file object shandle.   
               
           
        __int64 qwFileOffset = 0;//A文件视图的偏移量   
           
           
           
           
        while(qwFileSize>0)   
        {   
            //Determinethenumberofbytestobemappedinthisview   
           
            DWORD dwBytesInBlock = sinf.dwAllocationGranularity;   
               
            if(qwFileSize < sinf.dwAllocationGranularity)//文件小于系统分配粒度   
                dwBytesInBlock = (DWORD)qwFileSize;//偏移量为文件大小   
               
            PBYTE pbFile = (PBYTE)MapViewOfFile(hFileMapping,FILE_MAP_READ,(DWORD)(qwFileOffset>>32),(DWORD)(qwFileOffset&0xFFFFFFFF),dwBytesInBlock);   
            CString   DataStr(_T(""));    
               
            DWORD   i   =   0;      
            while( *(pbFile + i)  && (*(pbFile + i) != 0x0d) && (*(pbFile + i) != 0x0a))      
            {      
                DataStr += *(pbFile + i);      
                ++i;                
            }   
               
    /*  
            const char* buf = (const char*)pbFile;     
            const char* start= (const char*)pbFile;  
            int len;  
            while(start != NULL)  
            {  
                start = getline(buf, &len);  
                CString   str(buf, len);  
                  
                buf = start;  
            }     
    */   
   
            UnmapViewOfFile(pbFile);   
                       
            //Skiptothenextsetofbytesinthefile.   
            qwFileOffset += dwBytesInBlock;   
            qwFileSize -= dwBytesInBlock;   
        }   
           
        CloseHandle(hFileMapping);   
    }   
       
}
