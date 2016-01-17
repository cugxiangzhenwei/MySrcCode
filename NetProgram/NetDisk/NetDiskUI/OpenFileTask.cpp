#include "StdAfx.h"
#include "OpenFileTask.h"
#include "CommonFun.h"
#include "NetDiskUI.h"
#include "LogTools.h"
#include <imagehlp.h>
COpenFileTask::COpenFileTask(const CFileInfo & info,LPCTSTR strLocalFile)
{
	m_bFlag = TRUE;
	m_strErrMsg.Empty();
	m_strOperation = "OpenFile";
	m_FileInfo = info;
	m_strLocalFile = strLocalFile==NULL ? "":strLocalFile;
	m_strOpenTaskId = GetNewGUID().c_str();
}


COpenFileTask::~COpenFileTask(void)
{
}
BOOL COpenFileTask::Run()
{
	if (m_strLocalFile.IsEmpty() && !m_FileInfo.m_strFileId.IsEmpty())
		m_strOpenType = "netdisk";
	else if (!m_strLocalFile.IsEmpty() && m_FileInfo.m_strFileId.IsEmpty())
		m_strOpenType = "local";

	else if (m_strLocalFile.IsEmpty() && m_FileInfo.m_strFileId.IsEmpty())
	{
		m_strErrMsg = "参数错误！";
		m_bFlag = FALSE;
		return FALSE;
	}

	SOCKET sockConn = GetConnect(theApp.m_strServerIp);
	if (sockConn==0)
	{
		m_strErrMsg = "连接服务器失败！";
		m_bFlag = FALSE;
		return FALSE;
	}

	CString strDestFile = m_strLocalFile;
	if (m_strOpenType == "netdisk")
	{
		strDestFile = theApp.m_strCacheDir + "\\" + theApp.m_strUsrId + m_FileInfo.m_strNetDiskPath;
		strDestFile.Replace("/","\\");
		BOOL bNeedDownload = TRUE;
		if (PathFileExists(strDestFile))
		{
			long long iLocalFileSize = FileSize(strDestFile);
			CString strLocalFileSize  = TransferFileSize(iLocalFileSize);
			if (strLocalFileSize.Compare(m_FileInfo.m_strFileSize)==0)
			{
				string strLocalMd5 = GetMD5(strDestFile);
				if (strLocalFileSize.CompareNoCase(m_FileInfo.m_strFileSize)==0)
					bNeedDownload = FALSE;
			}
			if (bNeedDownload)
				remove(strDestFile);
		}

		MakeSureDirectoryPathExists(strDestFile);
		if (bNeedDownload && !DownToClient(sockConn,m_FileInfo.m_strFileId,strDestFile))
			return FALSE;
	}

	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS|SEE_MASK_FLAG_NO_UI;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = "Open";
	ShExecInfo.lpFile = strDestFile;        
	ShExecInfo.lpParameters = NULL;  
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = NULL;	
	ShellExecuteEx(&ShExecInfo);

	static string strSysVer = "";
	if (strSysVer.empty())
	{
		OSVERSIONINFO   osversioninfo;
		osversioninfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		GetVersionEx(&osversioninfo);
		if( (osversioninfo.dwMajorVersion == 5) && (osversioninfo.dwMinorVersion == 1)) 
			strSysVer = "xp";
		
		else if( (osversioninfo.dwMajorVersion == 6) && (osversioninfo.dwMinorVersion == 1))
			strSysVer =  "win7";

		else if( (osversioninfo.dwMajorVersion == 6) && (osversioninfo.dwMinorVersion == 2))
			strSysVer =  "win8";
		else
			strSysVer =  "other";

	}

	if ("xp" == strSysVer)
	{
		//解决xp机器上打开多个word、excel文件失败的问题
		CString strFile = strDestFile;
		strFile.MakeLower();
		if (  strFile.Right(4)==".doc" 
			|| strFile.Right(4)==".xls" 
			|| strFile.Right(4)==".ppt"
			|| strFile.Right(5)==".docx"
			|| strFile.Right(5)==".xlsx"
			|| strFile.Right(5)==".pptx")
		{
			Sleep(1000);
		}
	}
	int iCode = (int)ShExecInfo.hInstApp;
	if ((int)ShExecInfo.hInstApp > 32)
		return TRUE;
	else
	{
		DWORD dErrCode = GetLastError();
		if (dErrCode==1155)
		{
			ShExecInfo.lpVerb = "OpenAs";
			ShExecInfo.hInstApp = NULL;
			BOOL bOK = ShellExecuteEx(&ShExecInfo);
			LogMsg("Openfile 未找到关联程序，使用OpenAs模式由用户选择关联程序");
	
			if(!bOK)
			{
				CString strCmd;
				strCmd.Format("rundll32 shell32, OpenAs_RunDLL  %s", strDestFile);
				WinExec(strCmd, SW_SHOWNORMAL); //手动执行打开选择关联方式对话框
				return TRUE;
			}

			return TRUE; //返回真，外部可以根据hOut是否为NULL判断是否是OpenAs模式
		}
		return FALSE;
	}

	return TRUE;
}


CTaskBase * COpenFileTask::GetCopy()
{
	COpenFileTask * pTask = new COpenFileTask(*this);
	return pTask;
}

COpenFileTask::COpenFileTask(const COpenFileTask & other)
{
	m_strOpenTaskId = other.m_strOpenTaskId;
	m_FileInfo = other.m_FileInfo;


	m_bFlag = other.m_bFlag;
	m_strErrMsg = other.m_strErrMsg;
	m_strOperation = other.m_strOperation;
	m_dProgress = other.m_dProgress;
}
CString COpenFileTask::GetTaskId()
{
  return m_strOpenTaskId;
}

