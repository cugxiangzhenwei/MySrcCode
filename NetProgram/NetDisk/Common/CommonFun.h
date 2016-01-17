#include "UsrDefine.h"
#include <string>
#include <vector>
#include <WinSock2.h>
using namespace std;

std::string GetFileLenStr(__int64 iLen);
int CopyData( FileTransfer * Dst,FileTransfer * SrcTmp,int iSrcLen);
void ClearReveiveQueen(FileTransfer *Dst,int iLen);
std::string GetMD5(const char * pszFile);
__int64 FileSize(const char * strFile);
__int64 FileTimeLastModify(const char * strFile);
std::string FormatString(const char *fmt, ...);
std::string RepalceAll(const char * pszString,const char * pszOld,const char *pszNew);
std::string GetNewGUID();
bool SendData(SOCKET sockConn,char * pszData,int iDataLen,string & strErrMsg);
bool SendIntData(SOCKET sockConn,int iVal,string & strErrMsg);
bool RecvData(SOCKET sockConn,char * & pDataOut,int iDataLen,string & strErrMsg); // 需要外面释放空间
bool RecvData(SOCKET sockConn,void *pDataOut,int iDataLen,string & strErrMsg);
bool RecvIntData(SOCKET sockConn,int & iValOut,string & strErrMsg);
bool RecvStringData(SOCKET sockConn,int iDataLen,string & strOut,string & strErrMsg);
BOOL IsFile(LPCTSTR strPath);
BOOL IsDir(LPCTSTR strPath);