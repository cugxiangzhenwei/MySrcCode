/* Copyright (C) 2011 阙荣文
 *
 * 这是一个开源免费软件,您可以自由的修改和发布.
 * 禁止用作商业用途.
 *
 * 联系原作者: querw@sina.com 
*/

#include "StdAfx.h"
#include "HTTPRequest.h"

CHTTPRequest::CHTTPRequest(const wchar_t* pszIP, unsigned int nPort) :
	m_strIP(pszIP), m_nPort(nPort)
{
	m_pData = new char[MAX_REQUESTHEADERSIZE + 1]; // 缓冲区
	ASSERT(m_pData);
	ZeroMemory(m_pData, MAX_REQUESTHEADERSIZE + 1);
	m_nPos = 0; // 缓冲区游标.
}

CHTTPRequest::~CHTTPRequest(void)
{
	if(m_pData != NULL) delete[] m_pData;
}

// nSize, 或者 0 - 缓冲期溢出
int CHTTPRequest::PushData(const char* pData, int nSize)
{
	int nBufferLeft = MAX_REQUESTHEADERSIZE - m_nPos;
	if( nSize > nBufferLeft ) return 0;

	memcpy(m_pData + m_nPos, pData, nSize);
	m_nPos += nSize;
	return nSize;
}

BOOL CHTTPRequest::IsEnd()
{
	return strstr(m_pData, "\r\n\r\n") != NULL;
}

BOOL CHTTPRequest::Verify()
{
	// 两个连续的换行应该在请求头的最后
	char* pszEndTag = strstr(m_pData, "\r\n\r\n");
	if(pszEndTag == NULL)
	{
		return FALSE;
	}

	if( (pszEndTag + 4) != (m_pData + m_nPos) )
	{
		return FALSE;
	}

	return TRUE;
}

HTTP_METHOD CHTTPRequest::GetMethod()
{
	// 取出 HTTP 方法
	char szMethod[MAX_METHODSIZE] = {0};
	int nMethodIndex = 0;
	for(int i = 0; i < MAX_METHODSIZE && i < m_nPos; ++i)
	{
		if(m_pData[i] != ' ')
		{
			szMethod[nMethodIndex++] = m_pData[i];
		}
		else
		{
			break;
		}
	}

	// 返回
	if( strcmp(szMethod, "GET") == 0 ) return METHOD_GET;
	if( strcmp(szMethod, "PUT") == 0 ) return METHOD_PUT;
	if( strcmp(szMethod, "POST") == 0 ) return METHOD_POST;
	if( strcmp(szMethod, "HEAD") == 0 ) return METHOD_HEAD;
	if( strcmp(szMethod, "DELETE") == 0 ) return METHOD_DELETE; // 删除
	if( strcmp(szMethod, "TRACE") == 0 ) return METHOD_TRACE;
	if( strcmp(szMethod, "CONNECT") == 0 ) return METHOD_CONNECT;

	return METHOD_UNKNOWN;
}

// 返回客户端请求对象, 如果返回空字符串,说明客户端请求格式错误.
std::wstring CHTTPRequest::GetUrlObject()
{
	std::wstring strObject(L"");
	const char* lpszRequest = m_pData;
	const char *pStart = NULL, *pEnd = NULL;

	// 第一行的第一个空格的下一个字符开始是请求的文件名开始.
	for(int i = 0; i < m_nPos; ++i)
	{
		if(lpszRequest[i] == ' ')
		{
			pStart = lpszRequest + i + 1; 
			break;
		}
		if(lpszRequest[i] == '\n') break;
	}
	if(pStart == NULL) return strObject;

	// 从第一行的末尾方向查找第一个空格,实例: GET / HTTP/1.1
	pEnd = strstr(lpszRequest, "\r\n"); ASSERT(pEnd);
	if(pEnd == NULL || pEnd < pStart) return strObject;
	while(pEnd >= pStart)
	{
		if(pEnd[0] == ' ')
		{
			pEnd--;
			break;
		}
		pEnd--;
	}

	if(pEnd == NULL || pEnd < pStart) return strObject;

	// 已经取到了开始和结束的位置.
	int nObjectLen = pEnd - pStart + 1;
	char *pszObject = new char[nObjectLen + 1]; ASSERT(pszObject);
	ZeroMemory(pszObject, nObjectLen + 1);
	
	// UTF8解码
	int nNewPos = 0;
	BOOL bUTF8 = FALSE;
	for( int i = 0; i < nObjectLen; ++i)
	{
		if(pStart[i] == '?')
		{
			// '?' 后面是参数,忽略.
			break;
		}

		if(pStart[i] == '%' && i + 1 < nObjectLen && i + 2 < nObjectLen)
		{
			char szValue[5] = {0}, *pStopPos = NULL;
			szValue[0] = '0';
			szValue[1] = 'x';
			szValue[2] = pStart[i + 1];
			szValue[3] = pStart[i + 2];
			szValue[4] = 0;
			pszObject[nNewPos++] = (char) strtol(szValue, &pStopPos, 16);
			
			i += 2;
			
			// 空格会被编码为 %20
			if(pszObject[nNewPos - 1] != ' ')
			{
				bUTF8 = TRUE;
			}
		}
		else
		{
			// 获取的是URL,不应该替换正斜杠
			//if(pStart[i] == '/') pszObject[nNewPos++] = '\\';
			//else pszObject[nNewPos++] = pStart[i];
			pszObject[nNewPos++] = pStart[i];
		}
	}

	if(bUTF8)
	{
		strObject = UTF8toW(pszObject);
		//std::wstring wURL = UTF8toW(pszObject);
		//strObject = WtoA(wURL.c_str());
	}
	else
	{
		strObject = AtoW(pszObject);
	}

	delete[] pszObject;
	return strObject;
}

std::string CHTTPRequest::GetField(const char* pszKey)
{
	// 正真的字段应该是从换行开始,: 结束.
	std::string strKey("\r\n");
	strKey += pszKey; strKey += "; "; //冒号后面有个空格.
	std::string strValue("");

	// 找到字段的开始
	const char* pszStart = strstr(m_pData, strKey.c_str());
	if(pszStart == NULL) return strValue;
	pszStart += strKey.size();

	// 找到字段结束
	const char* pszEnd = strstr(pszStart, "\r\n");
	if(pszEnd == NULL) return strValue;

	strValue.assign(pszStart, pszEnd - pszStart);
	return strValue;
}

BOOL CHTTPRequest::GetRange(__int64 &lFrom, __int64 &lTo)
{
	__int64 nFrom = 0;
	__int64 nTo = -1; // -1 表示到最后一个字节.

	const char* lpszRequest = m_pData;
	const char* pRange = strstr(lpszRequest, "\r\nRange: bytes=");
	if(pRange)
	{
		/*
		The first 500 bytes (byte offsets 0-499, inclusive):
		bytes=0-499
		The second 500 bytes (byte offsets 500-999, inclusive):
		bytes=500-999
		The final 500 bytes (byte offsets 9500-9999, inclusive):
		bytes=-500
		bytes=9500-
		The first and last bytes only (bytes 0 and 9999):
		bytes=0-0,-1
		Several legal but not canonical specifications of the second 500 bytes (byte offsets 500-999, inclusive):
		bytes=500-600,601-999
		bytes=500-700,601-999
		*/

		pRange += strlen("\r\nRange: bytes=");
		const char *pMinus = strchr(pRange, '-');
		if(pMinus)
		{
			char szFrom[200], szTo[200];
			memset(szFrom, 0, 200);
			memset(szTo, 0, 200);
			memcpy(szFrom, pRange, pMinus - pRange);
			nFrom = _atoi64(szFrom);

			pMinus++;
			pRange = strstr(pMinus, "\r\n");
			if(pMinus + 1 == pRange)
			{
				nTo = -1;
			}
			else
			{
				memcpy(szTo, pMinus, pRange - pMinus);
				nTo = _atoi64(szTo);
				if(nTo <= 0) nTo = -1;
			}

			lFrom = nFrom;
			lTo = nTo;

			return TRUE;
		}
		else
		{
		}
	}
	else
	{
	}
	return FALSE;
}