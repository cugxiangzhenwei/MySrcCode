//关键实现代码
#pragma once

#include "wininet.h"

//VS2008中的wininet.h没有声明此标志，实际是可用的
#ifndef INTERNET_COOKIE_HTTPONLY
#define INTERNET_COOKIE_HTTPONLY        0x00002000
#endif

//该结构体用于从A进程发送InternetSetCookieExW需要的所有参数数据到B进程
struct CookieData
{
	DWORD dwSizeUrl;
	DWORD dwSizeCookieName;
	DWORD dwSizeCookieData;		//三个字符串各自的内存占用大小，包含末尾的0x0000，以字节为单位
	DWORD  dwProcessId;
	DWORD dwFlags;
	DWORD_PTR dwReserved;
	char pStrData[1];			//柔性数组，三个字符串的内容按顺序排列在该字节后
};

typedef struct 
{
	CStringW strHost;
	CStringW strHeaders;
	bool bSaveToList;
}ThreadDataSendCookie;

typedef BOOL (__stdcall *Proc_HttpQueryInfoW)(HINTERNET hRequest, DWORD dwInfoLevel,
	LPVOID lpBuffer, LPDWORD lpdwBufferLength, LPDWORD lpdwIndex);

typedef BOOL (__stdcall *Proc_HttpQueryInfoA)(HINTERNET hRequest, DWORD dwInfoLevel,
	LPVOID lpBuffer, LPDWORD lpdwBufferLength, LPDWORD lpdwIndex);

//把HttpQueryInfoW这个API函数Hook到我们自己定义的这个Hook_HttpQueryInfoW函数上
//因为IE是通过HttpQueryInfoW来获取HTTP响应头的，从而获取响应头中的Cookie
//我们拦截此API后，可以把获得的所有Cookie发给B进程进行设置
//如果担心效率问题，可以只把会话级Cookie发给B进程进行设置，持久化Cookie由于本身已经可以跨进程共享，不用发给B进程
//这里只是一个简单的Demo，所以一股脑的把所有Cookie发给B进程进行设置
BOOL __stdcall Hook_HttpQueryInfoW(HINTERNET hRequest, DWORD dwInfoLevel, 
	LPVOID lpBuffer, LPDWORD lpdwBufferLength, LPDWORD lpdwIndex);

BOOL __stdcall Hook_HttpQueryInfoA(HINTERNET hRequest, DWORD dwInfoLevel, 
	LPVOID lpBuffer, LPDWORD lpdwBufferLength, LPDWORD lpdwIndex);

//把一个响应头中的所有Cookie发给B进程，参数1：该头对应的网站HOST，参数2：整个响应头的内容
bool SendCookieToShare(const CStringW &strHost, const CStringW &strHeaders,bool bSaveToList = false);


//用于SendCookieToShare内部的辅助函数，把一条Cookie打包成CookieData结构体并发送到B进程
//参数1：该Cookie对应的网站URL，参数2：Cookie的内容，参数3：这条Cookie是否是HttpOnly的
void PackCookieDataToSend(LPCWSTR pwszUrl, LPCWSTR pwszCookieData, bool IsHttpOnly);