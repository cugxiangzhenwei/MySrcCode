#include "CommonFun.h"
#include<Windows.h>
#include <shlwapi.h>
#include <sstream>
#include <comdef.h>
#include "LogTools.h"

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "cryptopp561/md5.h"
#include "cryptopp561/filters.h"
#include "cryptopp561/channels.h"
#include "cryptopp561/files.h"
#include "cryptopp561/hex.h"
#ifdef _DEBUG
#pragma comment(lib, "cryptlibd.lib")
#else
#pragma comment(lib, "cryptlib.lib")
#endif

#pragma comment(lib,"Shlwapi.lib")  //PathFileExists

std::string GetFileLenStr(__int64 iLen)
{
	char strFileSize[50] ={0};
	if (iLen > GB )
	{
		sprintf_s(strFileSize,"%fGB",iLen*1.0/GB);
	}
	else if (iLen > MB)
	{
		sprintf_s(strFileSize,"%fMB",iLen*1.0/MB);
	}
	else if (iLen > KB)
	{
		sprintf_s(strFileSize,"%fKB",iLen*1.0/KB);
	}
	else
		sprintf_s(strFileSize,"%dByte",iLen);

	return strFileSize;
}
int CopyData( FileTransfer * Dst,FileTransfer * SrcTmp,int iSrcLen)
{
	const static int ONE_DATA_LEN = sizeof(FileTransfer);
	static int iRecvLength = 0;
	memcpy((char*)Dst+iRecvLength,SrcTmp,iSrcLen); //将接收到的不完整数据拼到缓存数据包数组中
	iRecvLength += iSrcLen;
	if(iRecvLength >= ONE_DATA_LEN )
	{
		iRecvLength = iRecvLength - ONE_DATA_LEN; //已经有一个完整包了
		return iRecvLength; //返回完整包之外还剩余的数据大小
	}
	else
		return NOT_FULL_DATA_BAG;
}
void ClearReveiveQueen(FileTransfer *Dst,int iLen)
{
	const static int ONE_DATA_LEN = sizeof(FileTransfer);
	if (iLen == 0)
		return;

	char * pData = new char[iLen];
	memcpy(pData,(char*)Dst + ONE_DATA_LEN,iLen);
	memcpy(Dst,pData,iLen);
	delete pData;
}
std::string GetMD5(const char * pszFile)
{
	std::wstring wstr 	= (wchar_t * )_bstr_t(pszFile); 
	if (!PathFileExistsW(wstr.c_str()))
		return "";

	CryptoPP::Weak::MD5 md5;
	CryptoPP::HashFilter filter(md5);

	std::auto_ptr<CryptoPP::ChannelSwitch> channelSwitch(new CryptoPP::ChannelSwitch);
	channelSwitch->AddDefaultRoute(filter);

	CryptoPP::FileSource(wstr.c_str(), true, channelSwitch.release());

	std::stringstream result;
	CryptoPP::HexEncoder encoder(new CryptoPP::FileSink(result), false);
	filter.TransferTo(encoder);
	std::string strOutMd5 = result.str();
	return strOutMd5;
}
__int64 FileSize(const char * strFile)
{
	// stat 方式打开大文件和长文件名的文件有问题，fopen_s 打开已经在外部打开的Word文档失败，FindFirstFileW is all Ok!
	WIN32_FIND_DATAW DataFind;
	std::wstring strFileW = (WCHAR*)_bstr_t(strFile);
	HANDLE hFind = FindFirstFileW(strFileW.c_str(),&DataFind);
	if(DataFind.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		FindClose(hFind);
		return 0;
	}
	else if (hFind!=INVALID_HANDLE_VALUE)
	{
		ULONGLONG lSize = ULONGLONG(DataFind.nFileSizeHigh)<< 32;
		lSize = lSize + DataFind.nFileSizeLow;
		FindClose(hFind);
		return lSize;
	}

	return -1;
}

__int64 FileTimeLastModify(const char * strFile)
{
	WIN32_FIND_DATAW DataFind;
	wstring strFileW = (WCHAR*)_bstr_t(strFile);
	HANDLE hFind = FindFirstFileW(strFileW.c_str(),&DataFind);
	if(hFind == INVALID_HANDLE_VALUE)
		return -1;

	FindClose(hFind);
	if(DataFind.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		return -2;

	LONGLONG nLL;
	ULARGE_INTEGER ui;
	ui.LowPart  = DataFind.ftLastWriteTime.dwLowDateTime;
	ui.HighPart = DataFind.ftLastWriteTime.dwHighDateTime;

	nLL = (ULONGLONG(ui.HighPart) << 32) + ui.LowPart;
	time_t pt = (long)((LONGLONG)(ui.QuadPart - 116444736000000000) / 10000000);
	return pt;
}
std::string FormatString(const char *fmt, ...) 
{ 
	std::string strResult="";
	if (NULL != fmt)
	{
		va_list marker = NULL;            
		va_start(marker, fmt);                            //初始化变量参数
		size_t nLength = _vscprintf(fmt, marker) + 1;    //获取格式化字符串长度
		std::vector<char> vBuffer(nLength, '\0');        //创建用于存储格式化字符串的字符数组
		int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, fmt, marker);
		if (nWritten>0)
		{
			strResult = &vBuffer[0];
		}            
		va_end(marker);                                    //重置变量参数
	}
	return strResult; 
}
std::string RepalceAll(const char * pszString,const char * pszOld,const char *pszNew)
{
	string str = pszString!=NULL ? pszString:"";
	if (pszOld==NULL) return str;
	if (pszNew==NULL) pszNew="";

	int iOldLen = strlen(pszOld);
	int begin = 0;
	begin = str.find(pszOld,begin);  //查找空格在str中第一次出现的位置
	while(begin != -1)  //表示字符串中存在空格
	{
		str.replace(begin, iOldLen, pszNew);  
		begin = str.find(pszOld,begin);
	}

	return str;
}
std::string GetNewGUID()
{
	char buf[64] = {0};
	GUID guid;
	if (S_OK == ::CoCreateGuid(&guid))
	{
		_snprintf_s(
			buf, sizeof(buf)
			, "%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X"
			, guid.Data1
			, guid.Data2
			, guid.Data3
			, guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3]
		, guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]
		);
		return (char *)buf;
	}
	else
	{
		return "";
	}
}
bool SendData(SOCKET sockConn,char * pszData,int iDataLen,string & strErrMsg)
{
	int iSendSum = 0;
	int iLenNeedSend = iDataLen;
	char *pCur = pszData;
	strErrMsg.clear();
	while(1)
	{
		int iSend = send(sockConn,pCur,iLenNeedSend,0);
		if (iSend == -1)
		{
			strErrMsg = "发送数据出现错误...\n";
			LogMsg(strErrMsg.c_str());
			closesocket(sockConn);
			return false;
		}
		else if (iSend == 0)
		{
			strErrMsg = "发送数据失败,接收端断开了连接...\n";
			LogMsg(strErrMsg.c_str());
			closesocket(sockConn);
			return false;
		}

		iSendSum += iSend;
		if (iSendSum == iDataLen)
			break; // 发送完成

		pCur = pszData + iSendSum;
		iLenNeedSend = iDataLen - iSendSum;
	}

	return true;
}
bool SendIntData(SOCKET sockConn,int iVal,string & strErrMsg)
{
	int iData = iVal;
	bool bok = SendData(sockConn,(char*)&iData,sizeof(int),strErrMsg);
	return bok;
}
bool RecvData(SOCKET sockConn,char * & pDataOut,int iDataLen,string & strErrMsg)
{
	int iRecvSum = 0;
	int iLenNeedRecv = iDataLen;

	pDataOut = new char[iDataLen];
	if (pDataOut==NULL)
		return false;

	memset(pDataOut,0,sizeof(char)*iDataLen);
	char *pCur = pDataOut;

	while(1)
	{
		int iRecv = recv(sockConn,pCur,iLenNeedRecv,0);
		if (iRecv == -1)
		{
			strErrMsg = "接收数据失败...\n";
			LogMsg(strErrMsg.c_str());
			delete pDataOut;
			closesocket(sockConn);
			return false;
		}
		else if (iRecv==0)
		{
			strErrMsg = "接收数据失败,发送方断开了连接...\n";
			LogMsg(strErrMsg.c_str());
			delete pDataOut;
			closesocket(sockConn);
			return false;
		}

		iRecvSum += iRecv;
		if (iRecvSum == iDataLen)
			break; // 接收完成

		pCur = pDataOut + iRecvSum;
		iLenNeedRecv = iDataLen - iRecvSum;
	}

	return true;
}
bool RecvIntData(SOCKET sockConn,int & iValOut,string & strErrMsg)
{
	bool bOK = RecvData(sockConn,&iValOut,sizeof(int),strErrMsg);
	return bOK;
}
bool RecvStringData(SOCKET sockConn,int iDataLen,string & strOut,string & strErrMsg)
{
	char *pDataOut = NULL;
	strOut.empty();
	if (RecvData(sockConn,pDataOut,iDataLen,strErrMsg))
	{
		strOut = pDataOut;
		delete pDataOut;
		return true;
	}
	if (pDataOut)
		delete pDataOut;

	return false;
}
bool RecvData(SOCKET sockConn,void *pDataOut,int iDataLen,string & strErrMsg)
{

	int iRecvSum = 0;
	int iLenNeedRecv = iDataLen;
	memset(pDataOut,0,sizeof(char)*iDataLen);
	char *pCur = (char*)pDataOut;

	while(1)
	{
		int iRecv = recv(sockConn,pCur,iLenNeedRecv,0);
		if (iRecv == -1)
		{
			strErrMsg = "接收数据失败...\n";
			LogMsg(strErrMsg.c_str());
			closesocket(sockConn);
			return false;
		}
		else if(iRecv==0)
		{
			strErrMsg = "接收数据失败,发送端关闭了连接...\n";
			LogMsg(strErrMsg.c_str());
			closesocket(sockConn);
			return false;
		}

		iRecvSum += iRecv;
		if (iRecvSum == iDataLen)
			break; // 接收完成

		pCur = (char*)pDataOut + iRecvSum;
		iLenNeedRecv = iDataLen - iRecvSum;
	}

	return true;
}
BOOL IsFile(LPCTSTR strPath)
{
	WIN32_FIND_DATAW DataFind;
	std::wstring strFileW = (WCHAR*)_bstr_t(strPath);
	HANDLE hFind = FindFirstFileW(strFileW.c_str(),&DataFind);
	if (hFind==INVALID_HANDLE_VALUE)
		return FALSE;

	FindClose(hFind);
	if(DataFind.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		return FALSE;

	return TRUE;
}
BOOL IsDir(LPCTSTR strPath)
{
	WIN32_FIND_DATAW DataFind;
	std::wstring strFileW = (WCHAR*)_bstr_t(strPath);
	HANDLE hFind = FindFirstFileW(strFileW.c_str(),&DataFind);
	if (hFind==INVALID_HANDLE_VALUE)
		return FALSE;

	FindClose(hFind);
	if(DataFind.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		return TRUE;

	return FALSE;
}