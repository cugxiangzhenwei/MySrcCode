#include "stdafx.h"
#include "NetDiskOperation.h"
#include "LogTools.h"
SOCKET GetConnect(const char *pszServerIp)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 1, 1 );

	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		return NULL;
	}

	/* Confirm that the WinSock DLL supports 1.1.*/
	/* Note that if the DLL supports versions greater    */
	/* than 1.1 in addition to 1.1, it will still return */
	/* 1.1 in wVersion since that is the version we      */
	/* requested.                                        */

	if ( LOBYTE( wsaData.wVersion ) != 1||
		HIBYTE( wsaData.wVersion ) != 1 ) {
			/* Tell the user that we could not find a usable */
			/* WinSock DLL.                                  */
			WSACleanup( );
			return NULL; 
	}

	// 创建套接字
	SOCKET sockClent = socket(AF_INET,SOCK_STREAM,0);

	//设置套接字发送缓冲区
	int nBuf = SOCKET_BUFF;
	int nBufLen = sizeof(nBuf);
	int nRe = setsockopt(sockClent, SOL_SOCKET, SO_SNDBUF, (char*)&nBuf, nBufLen);
	if(SOCKET_ERROR == nRe)
		LogMsg("setsockopt error!");

	//检查缓冲区是否设置成功
	nRe = getsockopt(sockClent, SOL_SOCKET, SO_SNDBUF, (char*)&nBuf, &nBufLen);
	if(SOCKET_BUFF != nBuf)
		LogMsg("检查设置不成功!");

	const char  * pszHostIp =  pszServerIp;
	// 客户端不需要bind，直接连接
	SOCKADDR_IN addrSrv; //服务器端地址
	addrSrv.sin_addr.S_un.S_addr = inet_addr(pszHostIp); // 本地回路地址，对应服务器地址（多机时对应服务器端地址）
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000); // 必须与服务器端等待的端口一致，且为网络字节序
	LogMsg("正在连接服务器%s...\n",pszHostIp);

	int iResult = connect(sockClent,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));

	if (iResult == SOCKET_ERROR)
	{
		LogMsg("与服务器%s连接失败!\n",pszHostIp);
		closesocket(sockClent);
		WSACleanup(); // 终止对套接字库的使用
		return NULL;
	}

	int timeout = 3000; //3s

	//nRe = setsockopt(sockClent,SOL_SOCKET,SO_SNDTIMEO,(const char*)&timeout,sizeof(timeout));
	//if(0 != nRe)
	//	LogMsg("设置发送消息超时失败!\n");

	//nRe = setsockopt(sockClent,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout,sizeof(timeout));
	//if(0 != nRe)
	//	LogMsg("设置接收消息超时失败!\n");

	return sockClent;
}

CString TransferFileSize(ULONGLONG fSize)
{
	CString strSize;
	if (fSize >= TB)
	{
		double dTB = fSize * 1.0 / TB;
		strSize.Format("%.2fTB",dTB);
	}
	else if (fSize >= GB)
	{
		double dGB = fSize * 1.0 / GB;
		strSize.Format("%.2fGB",dGB);
	}
	else if (fSize >= MB)
	{
		double dMB = fSize * 1.0 / MB;
		strSize.Format("%.2fMB",dMB);
	}
	else if (fSize >= KB)
	{
		double dKB = fSize * 1.0 / KB;
		strSize.Format("%.2fKB",dKB);
	}
	else
		strSize.Format("%lldByte",fSize);

	return strSize;
}
CString TransferFileTime(ULONGLONG iFileTime)
{
	time_t t = iFileTime;
	tm temptm;
	errno_t er = localtime_s(&temptm,&t);
	SYSTEMTIME st = {1900 + temptm.tm_year, 
		1 + temptm.tm_mon, 
		temptm.tm_wday, 
		temptm.tm_mday, 
		temptm.tm_hour, 
		temptm.tm_min, 
		temptm.tm_sec, 
		0};

	CString strFileTime;
	strFileTime.Format("%04d/%02d/%02d %02d:%02d:%02d",st.wYear,st.wMonth,st.wDay, st.wHour, st.wMinute, st.wSecond);
	return strFileTime;
}
BOOL GetFileInfo(const CString & strFile,CFileInfo & fInfo,BOOL bLocalFile)
{
	static int iDirImageId = -1;
	SHFILEINFO sfi;
	if (iDirImageId == -1)
	{
		CString strDirWindows;
		GetWindowsDirectory(strDirWindows.GetBuffer(MAX_PATH),MAX_PATH);
		strDirWindows.ReleaseBuffer();
		SHGetFileInfo(strDirWindows, FILE_ATTRIBUTE_DIRECTORY,&sfi,sizeof(sfi),SHGFI_DISPLAYNAME|SHGFI_SMALLICON|SHGFI_SYSICONINDEX|SHGFI_TYPENAME|SHGFI_USEFILEATTRIBUTES);
		iDirImageId = sfi.iIcon;
	}
	if (!bLocalFile)
	{
		if (fInfo.m_strType.CompareNoCase("Folder")==0)
		{
			fInfo.m_iImageID = iDirImageId;
			fInfo.m_strType = "文件夹";
			fInfo.m_strFileSize   = "";
			fInfo.m_strModifyTime = "";
			return TRUE;
		}
		SHGetFileInfo(strFile,0,&sfi,sizeof(sfi),SHGFI_DISPLAYNAME|SHGFI_SMALLICON|SHGFI_SYSICONINDEX|SHGFI_TYPENAME|SHGFI_USEFILEATTRIBUTES);
		fInfo.m_iImageID = sfi.iIcon;
		int iPos = strFile.ReverseFind(_T('\\'));
		fInfo.m_strName = strFile.Mid(iPos+1);
		fInfo.m_strType  =  sfi.szTypeName;
		return TRUE;
	}

	int iPos = strFile.ReverseFind(_T('\\'));
	fInfo.m_strName = strFile.Mid(iPos+1);

	WIN32_FIND_DATAW DataFind;
	CStringW strFileW = (WCHAR*)_bstr_t(strFile);
	HANDLE hFind = FindFirstFileW(strFileW,&DataFind);
	if(DataFind.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) //本地的文件夹
	{
		FindClose(hFind);
		fInfo.m_iImageID = iDirImageId;
		fInfo.m_strType = "文件夹";
		fInfo.m_strFileSize   = "";
		fInfo.m_strModifyTime = "";
		return TRUE;
	}
	else if (DataFind.dwFileAttributes & FILE_ATTRIBUTE_DEVICE)
		return FALSE;

	SHGetFileInfo(strFile,0,&sfi,sizeof(sfi),SHGFI_DISPLAYNAME|SHGFI_SMALLICON|SHGFI_SYSICONINDEX|SHGFI_TYPENAME|SHGFI_USEFILEATTRIBUTES);
	fInfo.m_iImageID = sfi.iIcon;
	fInfo.m_strType  =  sfi.szTypeName;

	LONGLONG nLL;
	ULARGE_INTEGER ui;
	ui.LowPart  = DataFind.ftLastWriteTime.dwLowDateTime;
	ui.HighPart = DataFind.ftLastWriteTime.dwHighDateTime;

	nLL = (ULONGLONG(ui.HighPart) << 32) + ui.LowPart;
	time_t pt = (long)((LONGLONG)(ui.QuadPart - 116444736000000000) / 10000000);
	fInfo.m_strModifyTime = TransferFileTime(pt);

	ULONGLONG lSize = ULONGLONG(DataFind.nFileSizeHigh)<< 32;
	lSize = lSize + DataFind.nFileSizeLow;
	fInfo.m_strFileSize = TransferFileSize(lSize);
	FindClose(hFind);
	return TRUE;
}