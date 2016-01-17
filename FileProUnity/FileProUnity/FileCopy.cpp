#include "stdafx.h"
#include "FileCopy.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include "FileProCom.h"
#include "comutil.h"

#define  COPY_ONCE_SIZE  (1024*1024)

CCopyManager * CCopyManager::m_PManagetr = NULL;

static DWORD WINAPI CopyFileCallBack(LARGE_INTEGER TotalFileSize, LARGE_INTEGER TotalBytesTransferred,
	LARGE_INTEGER StreamSize,LARGE_INTEGER StreamBytesTransferred,
	DWORD dwStreamNumber,DWORD dwCallbackReason,
	HANDLE hSourceFile, HANDLE hDestinationFile, LPVOID lpData)
{
	CFileCopy *parma = (CFileCopy*)lpData;
	parma->m_iTotalBytes = TotalFileSize.QuadPart;
	parma->m_iCurrentBytes = TotalBytesTransferred.QuadPart;

	if (parma->m_pMsgWnd)
		parma->m_pMsgWnd->PostMessage(WM_COMM,0,(LPARAM)parma);

	if (parma->m_bIsCancel)
		return PROGRESS_CANCEL;

	return PROGRESS_CONTINUE;
}
void CFileCopy::CheckParam()
{
	if (!m_bParamOK)
		return;

	struct stat buf;
	int result = stat (strSrc, &buf);
	if(result!=0)
	{
		m_strErrMsg.Format("输入文件【%s】无法打开",strSrc);
		m_bParamOK = false;
		return;
	}

	m_iTotalBytes = buf.st_size;

	if (access(strDst,0)==0) //输出文件已经存在
	{
		CString strMsg;
		strMsg.Format("输出目标文件“%s”已经存在,是否覆盖",strDst);
		if (MessageBox(NULL,strMsg,"提示",MB_ICONQUESTION|MB_YESNO) ==IDNO)
		{
			m_strErrMsg.Format("输出目标文件“%s”已经存在,且用户选择了不覆盖",strDst);
			m_bParamOK = false;
			return;
		}
		else
		{
			if(remove(strDst)!=0)
			{
				m_strErrMsg.Format("输出目标文件“%s”已经存在,且无法删除！",strDst);
				m_bParamOK = false;
				return;
			}
		}
	}
}
CFileCopy::CFileCopy(const char *pszSrc,const char *pszDest)
{
	m_bParamOK = true;
	if (NULL == pszSrc)
	{
		m_strErrMsg = "输入源文件路径为空";
		m_bParamOK = false;
		strSrc = "";
	}
	else
		strSrc = pszSrc;

	if(NULL == pszDest)
	{
		m_strErrMsg = "输出目标路径为空";
		m_bParamOK = false;
		strDst = "";
	}
	else
		strDst = pszDest;

	m_bForce = false;
	m_bIsCancel = false;
	m_iTotalBytes   = 0;
	m_iCurrentBytes = 0;
	m_pMsgWnd = NULL;
	m_hThreadHandle = NULL;
	m_CopyState = COPY_WAIT_SATRT;
	CheckParam();
}
bool CFileCopy::ExcuteCopy(bool bForce,MsgWnd *pMsgWnd)
{
	if (!m_bParamOK)
		return false;

	m_bForce = bForce;
	m_pMsgWnd = pMsgWnd;
	DWORD  ThreadId = 0;
	bool bStart = true;
	CCopyManager::Instance()->AddToManager(strDst,this,bStart);
	if (bStart)
		m_hThreadHandle = CreateThread(NULL,0,SysCopyFileThreadPro/*CopyFileThreadPro*/,this,0,&ThreadId);

	return true;
}
DWORD WINAPI CFileCopy::CopyFileThreadPro(LPVOID lpParamter)
{
	CFileCopy *param = (CFileCopy*)lpParamter;
	long iFileSize = param->m_iTotalBytes;

	int iReadBytes   = COPY_ONCE_SIZE;
	if (iFileSize<COPY_ONCE_SIZE)
		iReadBytes = iFileSize; //数据量极小时一次性读写

	FILE *pSrcFile = fopen(param->strSrc,"rb");
	if (NULL== pSrcFile)
	{
		param->m_strErrMsg.Format( _T("打开源文件【%s】失败！"),param->strSrc);
		return 1;
	}

	FILE *pDstFile = fopen(param->strDst,"wb");
	if (NULL == pDstFile)
	{
		param->m_strErrMsg.Format( _T("创建目标文件【%s】失败！"),param->strDst);
		fclose(pSrcFile);
		return 1;
	}
	int iAlloacSize = iReadBytes + 1;
	char *pData = new char[iAlloacSize];
	memset(pData,0,iAlloacSize);

	int iReaded = 0;
	int iWritten = 0;
	while(1)
	{
		int iReadThis =  fread(pData,1,iReadBytes,pSrcFile); //读
		if (0==iReadBytes)
			break;

		iReaded  += iReadThis;
		int iWriteThis = fwrite(pData,1,iReadThis,pDstFile); //写
		iWritten += iWriteThis;

		param->m_iCurrentBytes = iWritten;
		
		if ((iReaded + iReadBytes)>iFileSize)
			iReadBytes = iFileSize - iReaded;

		if (param->m_pMsgWnd)
			param->m_pMsgWnd->PostMessage(WM_COMM,0,(LPARAM)param);

		if (param->m_bIsCancel)
		{
			param->m_strErrMsg = _T("用户取消！");
			break;
		}
	}

	delete []pData;
	fclose(pSrcFile);
	fclose(pDstFile);
	if (param->m_bIsCancel)
	{
		MessageBox(NULL,"用户取消拷贝!","提示",MB_ICONINFORMATION);
		remove(param->strDst);
	}
	else
		MessageBox(NULL,"复制成功！","提示",MB_ICONINFORMATION);
	return 0;
}

DWORD WINAPI CFileCopy::SysCopyFileThreadPro(LPVOID lpParamter)
{
	CFileCopy *param = (CFileCopy*)lpParamter;
	BOOL bok = CopyFileEx(param->strSrc,
		param->strDst,
		CopyFileCallBack,
		param,
		(LPBOOL)&(param->m_bIsCancel),
		COPY_FILE_FAIL_IF_EXISTS);

	if (!bok)
	{
		if (param->m_bIsCancel)
		{
			MessageBox(NULL,"用户取消拷贝!","提示",MB_ICONINFORMATION);
		}
		else
		{
			CString str;
			str.Format("复制失败，目标文件【%s】可能已经存在！",param->strDst);
			MessageBox(NULL,str,"提示",MB_ICONERROR);
			param->m_strErrMsg = str;
		}

		return 1;
	}
	MessageBox(NULL,"复制成功！","提示",MB_ICONINFORMATION);
	return 0;
}

CCopyManager * CCopyManager::Instance()
{
	if (m_PManagetr == NULL)
		m_PManagetr = new CCopyManager;

	return m_PManagetr;
}
void CCopyManager::AddToManager(CString str ,CFileCopy * pcp,bool & bStart)
{
	g_CopyProgressInfo[str] = pcp;
	if (m_iCurrentRunningThreadCount == m_MaxThreadCount) //已经达到线程上限
	{
		bStart = false; //则加入等待队列
		pcp->m_CopyState = CFileCopy::COPY_WAIT_SATRT;
		return;
	}
	bStart = true; // 创建新的线程开始执行
	pcp->m_CopyState = CFileCopy::COPY_RUNNING;
	m_iCurrentRunningThreadCount++; //在执行的线程个数增加
	m_CopyQueue.push(str); // 向队列中增加一个任务
}
void CCopyManager::DestoyrCopyProgressInfo()
{
	map<CString,CFileCopy *>::iterator iter = g_CopyProgressInfo.begin();
	for (iter;iter!=g_CopyProgressInfo.end();iter++)
	{
		if (iter->second->m_pMsgWnd)
		{
			iter->second->m_pMsgWnd->DestroyWindow();
			iter->second->m_pMsgWnd = NULL;
		}

		delete iter->second;
	}

	g_CopyProgressInfo.clear();
	delete this;
}
CFileCopy * CCopyManager::GetCopyProgressInfo(CString strIndex)
{
	map<CString,CFileCopy*>::iterator iter = g_CopyProgressInfo.find(strIndex);
	if (iter==g_CopyProgressInfo.end())
		return NULL;

	return g_CopyProgressInfo[strIndex];
}
void CCopyManager::CancelCopy(CString strIndex)
{
	map<CString,CFileCopy*>::iterator iter = g_CopyProgressInfo.find(strIndex);
	if (iter==g_CopyProgressInfo.end())
		return;

	g_CopyProgressInfo[strIndex]->m_bIsCancel = true;
}
void CCopyManager::SetMaxThreadCount(int iThreadCount)
{
	if (iThreadCount > 10 || iThreadCount < 1)
	{
		m_MaxThreadCount = 5;
		return;
	}
	m_MaxThreadCount = iThreadCount;
}

void CCopyManager::StartManagerThread()
{
	 //在此启动一个监护进程，管理子线程的状态
}