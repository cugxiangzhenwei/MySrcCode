#include "stdafx.h"
#include "KeyCode.h"
#include "APIHook.h"
#include "UsrDefine.h"
#include "StlHeader.h"

vector<CookiePair> m_HostCookieHistoryList;
CRITICAL_SECTION   g_CookieListCS;

//使用CAPIHook来Hook  HttpQueryInfoW到Hook_HttpQueryInfoW
//CAPIHook来自APIhook.h，是《Windows核心编程》书中附带的代码，用来Hook API
//在实际项目中可以根据需要，自行裁剪CAPIHook
CAPIHook sm_InternetSetCookieExW("WININET.DLL", "HttpQueryInfoW", reinterpret_cast<PROC>(Hook_HttpQueryInfoW));

CAPIHook sm_InternetSetCookieExA("WININET.DLL", "HttpQueryInfoA", reinterpret_cast<PROC>(Hook_HttpQueryInfoA));


BOOL __stdcall Hook_HttpQueryInfoW(HINTERNET hRequest, DWORD dwInfoLevel, 
	LPVOID lpBuffer, LPDWORD lpdwBufferLength, LPDWORD lpdwIndex)
{
	//获取HttpQueryInfoW的真实地址
	static const Proc_HttpQueryInfoW RealAddr_HttpQueryInfoW = 
		reinterpret_cast<Proc_HttpQueryInfoW>(::GetProcAddress(GetModuleHandle("WININET.DLL"), "HttpQueryInfoW"));
	//调用真实的HttpQueryInfoW
	BOOL bRet = RealAddr_HttpQueryInfoW(hRequest, dwInfoLevel, lpBuffer, lpdwBufferLength, lpdwIndex);
	//检查响应头的内容
	if (dwInfoLevel == HTTP_QUERY_RAW_HEADERS_CRLF)	//IE总是通过此标志取得整个响应头，从而提取里Set-Cookie:的内容
	{
		CStringW strHeaders(static_cast<wchar_t*>(lpBuffer));
		//如果有Set-Cookie:才处理，没有的话说明这个响应头不设置Cookie，直接忽略
		if (strHeaders.Find(L"Set-Cookie:") != -1)
		{
			//现在响应头是有了，但是这个响应头里的Cookie是哪个网站的？
			//所以我们要用hRequest这个句柄，再次调用HttpQueryInfoW查询这个响应头对应的请求头里面的HOST内容，才知道是哪个网站的
			//知道了HOST，才能为B进程调用InternetSetCookieExW设置这个Cookie时提供第一个参数LPCWSTR lpszUrl
			wchar_t *pwszHost = new wchar_t[500];
			DWORD dwBuffSize = sizeof(wchar_t) * 500;
			memset(pwszHost, 0 , dwBuffSize);
			if (RealAddr_HttpQueryInfoW(hRequest, HTTP_QUERY_FLAG_REQUEST_HEADERS | HTTP_QUERY_HOST, pwszHost, &dwBuffSize, NULL))
				SendCookieToShare(pwszHost, strHeaders,true);		//把这个头里的Cookie发给B进程
			delete [] pwszHost;
			pwszHost = NULL;
		}
	}
	return bRet;
}
BOOL __stdcall Hook_HttpQueryInfoA(HINTERNET hRequest, DWORD dwInfoLevel, 
	LPVOID lpBuffer, LPDWORD lpdwBufferLength, LPDWORD lpdwIndex)
{
	//获取HttpQueryInfoW的真实地址
	static const Proc_HttpQueryInfoA RealAddr_HttpQueryInfoA = 
		reinterpret_cast<Proc_HttpQueryInfoA>(::GetProcAddress(GetModuleHandle("WININET.DLL"), "HttpQueryInfoA"));
	//调用真实的HttpQueryInfoW
	BOOL bRet = RealAddr_HttpQueryInfoA(hRequest, dwInfoLevel, lpBuffer, lpdwBufferLength, lpdwIndex);
	//检查响应头的内容
	if (dwInfoLevel == HTTP_QUERY_RAW_HEADERS_CRLF)	//IE总是通过此标志取得整个响应头，从而提取里Set-Cookie:的内容
	{
		CString strHeaders(static_cast<char*>(lpBuffer));
		//如果有Set-Cookie:才处理，没有的话说明这个响应头不设置Cookie，直接忽略
		if (strHeaders.Find("Set-Cookie:") != -1)
		{
			//现在响应头是有了，但是这个响应头里的Cookie是哪个网站的？
			//所以我们要用hRequest这个句柄，再次调用HttpQueryInfoW查询这个响应头对应的请求头里面的HOST内容，才知道是哪个网站的
			//知道了HOST，才能为B进程调用InternetSetCookieExW设置这个Cookie时提供第一个参数LPCWSTR lpszUrl
			char *pwszHost = new char[500];
			DWORD dwBuffSize = sizeof(char) * 500;
			memset(pwszHost, 0 , dwBuffSize);
			if (RealAddr_HttpQueryInfoA(hRequest, HTTP_QUERY_FLAG_REQUEST_HEADERS | HTTP_QUERY_HOST, pwszHost, &dwBuffSize, NULL))
			{
				CStringW strHost = (wchar_t*)_bstr_t(pwszHost);
				CStringW strHeadersW = (wchar_t*)_bstr_t(strHeaders);
				SendCookieToShare(strHost, strHeadersW,true);		//把这个头里的Cookie发给B进程
			}
			delete [] pwszHost;
			pwszHost = NULL;
		}
	}
	return bRet;
}
DWORD WINAPI ThreadFunSendCookie(LPVOID pData)
{
	ThreadDataSendCookie *pSendData = (ThreadDataSendCookie*)(pData);
	CStringW strHost = pSendData->strHost;
	CStringW strHeaders = pSendData->strHeaders;
	bool bSaveToList = pSendData->bSaveToList;
	delete pSendData;

	const CStringW strUrl = L"http://" + strHost + L"/";

	int nPosL = 0, nPosR = 0;
	CStringW strTempW;		//保存每一条Cookie的内容
	bool IsHttpOnly;		//该条Cookie是否是HttpOnly的
	//在一个循环中取出这个响应头中的所有Set-Cookie:中的Cookie内容
	while (nPosL != -1)
	{
		nPosL = strHeaders.Find(L"Set-Cookie: ", nPosR);
		if (nPosL == -1)
			break;
		nPosL += wcslen(L"Set-Cookie: ");
		nPosR = strHeaders.Find(L"\r\n", nPosL);
		if (nPosR == -1)
			break;
		strTempW = strHeaders.Mid(nPosL, nPosR - nPosL);
		//检查是否是HttpOnly的Cookie，如果是的话在B进程中调用InternetSetCookieExW设置Cookie时
		//第四个参数DWORD dwFlags应为INTERNET_COOKIE_HTTPONLY
		if (strTempW.Find(L"HttpOnly") != -1)
			IsHttpOnly = true;
		else
			IsHttpOnly = false;
		//发送这条Cookie给进程B
		PackCookieDataToSend(strUrl, strTempW, IsHttpOnly);
	}

	if(bSaveToList)
	{
		CookiePair cp;
		cp.m_strHost = strHost;
		cp.m_strValue = strHeaders;
		EnterCriticalSection(&g_CookieListCS);
		if (m_HostCookieHistoryList.size() > MAX_COOKIE_SETS_COUNT)
			m_HostCookieHistoryList.erase(m_HostCookieHistoryList.begin());

		m_HostCookieHistoryList.push_back(cp);
		LeaveCriticalSection(&g_CookieListCS);
	}
	return 0;
}
bool SendCookieToShare(const CStringW &strHost, const CStringW &strHeaders,bool bSaveToList)
{
	//这个Cookie对应的网站URL，用作B进程中InternetSetCookieExW的第一个参数LPCWSTR lpszUrl

	ThreadDataSendCookie * pThreadParam = new ThreadDataSendCookie;
	pThreadParam->strHost = strHost;
	pThreadParam->strHeaders = strHeaders;
	pThreadParam->bSaveToList = bSaveToList;
	CreateThread(NULL,0,ThreadFunSendCookie,pThreadParam,0,NULL);
	return true;

}

void PackCookieDataToSend(LPCWSTR pwszUrl, LPCWSTR pwszCookieData, bool IsHttpOnly)
{
	//获得pwszUrl和pwszCookieData两个字符串所占内存大小
	DWORD dwSizeUrl = (wcslen(pwszUrl) + 1) * 2;
	DWORD dwSizeCookieData = (wcslen(pwszCookieData) + 1) * 2;
	DWORD dwTotalSize = sizeof(CookieData) + dwSizeUrl + dwSizeCookieData ;	//要发送的数据总大小

	void *pBuffer = malloc(dwTotalSize);
	memset(pBuffer, 0, dwTotalSize);

	CookieData *pData = static_cast<CookieData*>(pBuffer);
	pData->dwSizeUrl = dwSizeUrl;
	pData->dwSizeCookieName = 0;		//CookieName为空，这个参数在InternetSetCookieExW中一般不用，直接由CookieData提供Cookie名
	pData->dwSizeCookieData = dwSizeCookieData;
	pData->dwProcessId = GetCurrentProcessId();
	//如果是HttpOnly的Cookie，需要加上该标志
	if (IsHttpOnly)
		pData->dwFlags = INTERNET_COOKIE_HTTPONLY;
	else
		pData->dwFlags = NULL;
	pData->dwReserved = NULL;
	//拷贝两个字符串的数据到结构体尾部的柔性数组
	char *pTempChar = pData->pStrData;
	memcpy(pTempChar, pwszUrl, dwSizeUrl);
	pTempChar += dwSizeUrl;
	memcpy(pTempChar, pwszCookieData,dwSizeCookieData);
	pTempChar = NULL;
	pData = NULL;

	COPYDATASTRUCT stCopyData = {0};
	stCopyData.cbData = dwTotalSize;
	stCopyData.lpData = pBuffer;
	stCopyData.dwData = WM_CD_SETCOOKIE;

	CWnd * pDeskTopWnd = CWnd::GetDesktopWindow();
	CWnd *pFind = pDeskTopWnd->GetWindow(GW_CHILD);
	while(pFind)
	{
		if (::GetProp(pFind->GetSafeHwnd(),"use_hook_cookie"))
		{
				//通过WM_COPYDATA把数据发给另一个进程
			::SendMessage(pFind->GetSafeHwnd(), WM_COPYDATA, 
				reinterpret_cast<WPARAM>(AfxGetMainWnd()->GetSafeHwnd()),
				reinterpret_cast<LPARAM>(&stCopyData));
		}

		pFind = pFind->GetWindow(GW_HWNDNEXT);
	}
	
	free(pBuffer);
	pBuffer = NULL;
}