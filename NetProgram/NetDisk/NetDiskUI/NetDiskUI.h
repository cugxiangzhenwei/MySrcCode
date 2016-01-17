
// NetDiskUI.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "PageNetDisk.h"

// CNetDiskUIApp:
// 有关此类的实现，请参阅 NetDiskUI.cpp
//

class CNetDiskUIApp : public CWinApp
{
public:
	CNetDiskUIApp();

// 重写
public:
	virtual BOOL InitInstance();

	CString m_strUsrId;
	CString m_strUsrName;
	CString m_strPasswd;
	long long m_iAllSpace;
	long long m_iUsedSpace;
	CString m_strServerIp;
	CString m_strCacheDir;
	CPageNetDisk * m_pPageNetDisk;
private:
	HANDLE m_hMutex;
	BOOL   m_bRestart;

// 实现

	DECLARE_MESSAGE_MAP()
public:
	virtual int ExitInstance();
};

extern CNetDiskUIApp theApp;