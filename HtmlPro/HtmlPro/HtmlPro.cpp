// HtmlPro.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "HtmlPro.h"
#include "ThreadPoolManager.h"
#include "DownloadTask.h"
#include "BaiduPicture.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define  STREQUAL(a,b) _stricmp(a,b)==0

// 唯一的应用程序对象

CWinApp theApp;

using namespace std;
BOOL PeraseCmd(int argc, TCHAR* argv[],BOOL &bIsFile,CString & strOut,CString &strUrl)
{
	bIsFile = FALSE;
	strOut = "";
	strUrl  = "";

	for (int i=1;i< argc;i++)
	{
		if (STREQUAL(argv[i],"-bf"))
		{
			CString str = argv[++i];
			str.MakeLower();
			if (STREQUAL(str,"true"))
			{
				bIsFile =  TRUE;
			}
			else
				bIsFile =  FALSE;

		}
		else if (STREQUAL(argv[i],"-url"))
		{
			strUrl = argv[++i];
		}
		else if (STREQUAL(argv[i],"-out"))
		{
			strOut = argv[++i];
		}
	}

	if (strUrl.IsEmpty())
		return FALSE;

	if (strOut.IsEmpty())
	{
		if (bIsFile)
			printf("请指定输出html文件的路径！\n");
		else
			printf("请指定下载文件存储目录！\n");
		return FALSE;
	}

	return TRUE;
}

void PrintHelp()
{
	printf("使用方法：\nHtmlPro.exe -bf TRUE[FALSE],-url url -out out.html\
		   \n示例1：HtmlPro.exe -bf FALSE,-url \"www.baidu.com\" -out \"C:\\downloaddir\"\n示例2：HtmlPro.exe -bf TRUE,-url \"www.baidu.com\" -out \"C:\\out.html\"\n");
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// 初始化 MFC 并在失败时显示错误
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: 更改错误代码以符合您的需要
			_tprintf(_T("错误: MFC 初始化失败\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: 在此处为应用程序的行为编写代码。

			vector<CString> VBaiduImgLinks;
			CString strCon = GetJsFromFile("E:\\js——part.txt");
			if (strCon.IsEmpty())
				return 0;

			GetAllImgUrl(VBaiduImgLinks,strCon);
			CString strOutPath = "E:\\baidu";
			for (size_t i=0;i<VBaiduImgLinks.size();i++)
			{
				//DownloadFile(strOutDir,VImgLinks[i]);
				CDownloadTask DownloadTask(strOutPath,VBaiduImgLinks[i]);
				theDownLoadPoolManager.AddTask(DownloadTask);
			}
			while(1);

			CString strOut = "";
			BOOL bIsFile = FALSE;
			CString strUrl = "http://www.netbian.com/";

			if (!PeraseCmd(argc,argv,bIsFile,strOut,strUrl))
			{
				PrintHelp();
				return 1;
			}
			CString strOutDir = strOut;
			DownloadHtml(strOut,strUrl,bIsFile);
			//DownloadFile("C:\\","http://zhangmenshiting.baidu.com/data2/music/119826740/1197655931400515261128.mp3?xcode=e095060edcb512008a268d64a327f6a2e36664380262dc7e");
			
			if (!bIsFile)
			{
				vector<CString> VImgLinks;
				GetImgLink(VImgLinks,strOut,strUrl);
				for (size_t i=0;i<VImgLinks.size();i++)
				{
					//DownloadFile(strOutDir,VImgLinks[i]);
					CDownloadTask DownloadTask(strOutDir,VImgLinks[i]);
					theDownLoadPoolManager.AddTask(DownloadTask);
				}

				while(1);
				//theDownLoadPoolManager.WaitFinish();
			}
			
		}
	}
	else
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: GetModuleHandle 失败\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
