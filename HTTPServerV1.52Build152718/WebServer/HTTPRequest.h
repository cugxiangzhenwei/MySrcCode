/* Copyright (C) 2011 阙荣文
 *
 * 这是一个开源免费软件,您可以自由的修改和发布.
 * 禁止用作商业用途.
 *
 * 联系原作者: querw@sina.com 
*/

#pragma once

#include "HTTPDef.h"


class CHTTPRequest
{
public:
	CHTTPRequest(const wchar_t* pszIP, unsigned int nPort);
	virtual ~CHTTPRequest(void);

protected:
	std::wstring m_strIP; // 客户请求的IP地址
	unsigned int m_nPort; // 端口

	char* m_pData; // 缓冲区
	int m_nPos; // 缓冲区游标.

public:
	int PushData(const char* pData, int nSize); // 套接字收到数据后,推入到 HTTP Request 实例中.

	BOOL IsEnd(); // 请求头是否结束.
	BOOL Verify(); // 验证请求头的有效性.

	HTTP_METHOD GetMethod(); // 返回HTTP 方法
	std::wstring GetUrlObject(); // 返回客户端请求的对象(已经经过UTF8解码,所以返回宽字符串)
	std::string GetField(const char* pszKey); // 返回请求头中的一个字段(HTTP头中只有ANSI字符,所以返回string).
	BOOL GetRange(__int64 &lFrom, __int64 &lTo);

	const std::wstring& GetIP() { return m_strIP; }
	unsigned int GetPort() { return m_nPort; }
};
