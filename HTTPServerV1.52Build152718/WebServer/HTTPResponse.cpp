/* Copyright (C) 2011 阙荣文
 *
 * 这是一个开源免费软件,您可以自由的修改和发布.
 * 禁止用作商业用途.
 *
 * 联系原作者: querw@sina.com 
*/

#include "StdAfx.h"
#include <sys/types.h>
#include <sys/stat.h>
#include "HTTPResponse.h"
#include "HTTPContent.h"

CHTTPResponse::CHTTPResponse(const wchar_t* pszIP, unsigned int nPort) :
	m_strIP(pszIP), m_nPort(nPort)
{
	m_pData = new char[MAX_RESPONSEHEADERSIZE + 1]; ASSERT(m_pData);
	ZeroMemory(m_pData, MAX_RESPONSEHEADERSIZE + 1);
	m_nReadPos = 0;
	m_nServerCode = SC_AUTO;

	m_nHeaderSize = 0;

	m_Method = METHOD_UNDEFINE;

	m_pContent = NULL;
}

CHTTPResponse::~CHTTPResponse(void)
{
	if(m_pData != NULL) delete[] m_pData;
	if(m_pContent != NULL)
	{
		m_pContent->Close();
		delete m_pContent;
	}
}

void CHTTPResponse::AttachContent(CHTTPContent *pContent)
{
	ASSERT(pContent);
	if(m_pContent != NULL)
	{
		m_pContent->Close();
		delete m_pContent;
		ASSERT(0);
	}
	m_pContent = pContent;
}

HTTP_METHOD CHTTPResponse::SetMethod(HTTP_METHOD mh)
{
	HTTP_METHOD mh_old = m_Method;
	m_Method = mh;
	return mh_old;
}

SERVER_CODE CHTTPResponse::SetServerCode(SERVER_CODE nNewCode)
{
	SERVER_CODE nOldCode = m_nServerCode;
	m_nServerCode = nNewCode;
	return nOldCode;
}

std::string CHTTPResponse::getFirstLine(SERVER_CODE nServerCode)
{
	std::string str = "HTTP/1.1 ";
	switch (nServerCode)
	{
	case SC_OK:
		{
			str += "200 OK";
			break;
		};
	case SC_NOCONTENT:
		{
			str += "204 No Content";
			break;
		};
	case SC_PARTIAL:
		{
			str += "206 Partial Content";
			break;
		};
	case SC_BADREQUEST:
		{
			str += "400 Bad Request";
			break;
		};
	case SC_NOTFOUND:
		{
			str += "404 Not Found";
			break;
		}
	case SC_BADMETHOD:
		{
			str += "405 Method Not Allowed";
			break;
		};
	default:
		str += "500 Internal Server Error";
	};

	str += "\r\n";
	return str;
}



BOOL CHTTPResponse::CookResponse()
{
	if(m_pContent != NULL)
	{
		
	}
	else
	{
		ASSERT(0);
		CHTTPContent *pContent = new CHTTPContent;
		AttachContent(pContent);
	}
	

	//// 准备内容
	//if( SC_AUTO == GetServerCode())
	//{
	//	// 内容为一个文件
	//	if(m_strFileName.size() > 0)
	//	{
	//		if(m_pContent->OpenFile(m_strFileName.c_str(), m_lFrom, m_lTo))
	//		{
	//			if(m_lFrom == 0 && m_lTo == -1)
	//			{
	//				SetServerCode(SC_OK);
	//			}
	//			else
	//			{
	//				SetServerCode(SC_PARTIAL);
	//			}
	//		}
	//		else
	//		{
	//			SetServerCode(SC_NOTFOUND);
	//			
	//			long lFrom = 0;
	//			long lTo = 0;
	//			m_pContent->OpenHtml(g_HTTP_Content_NotFound, strlen(g_HTTP_Content_NotFound));
	//		}
	//	}
	//	else
	//	{
	//		// 内容为目录的文件列表
	//		SetServerCode(SC_OK);
	//		m_pContent->OpenDir(m_strRoot.c_str(), m_strDir.c_str());
	//	}
	//}
	//else if(SC_BADMETHOD == GetServerCode())
	//{
	//	m_pContent->OpenText(g_HTTP_Bad_Method, strlen(g_HTTP_Bad_Method));
	//}
	//else if(SC_BADREQUEST == GetServerCode())
	//{
	//	m_pContent->OpenText(g_HTTP_Bad_Request, strlen(g_HTTP_Bad_Request));
	//}
	//else
	//{
	//	// 如果想自定义其他的错误码内容, 可以在这里加些代码.
	//}

	// 如果客户不要求 Body, 设置响应码为 204
	if(m_Method == METHOD_HEAD)
	{
		SetServerCode(SC_NOCONTENT);
	}

	// 第一行
	strcat(m_pData, getFirstLine(GetServerCode()).c_str());
	
	// Date
	strcat(m_pData, "Date: ");
	strcat(m_pData, FormatHTTPDate(NULL).c_str());
	strcat(m_pData, "\r\n");
	
	// 以下几行只有在有内容的时候才输出.
	if(m_pContent->IsOpen())
	{
		// Last-Modified
		strcat(m_pData, "Last-Modified: ");
		strcat(m_pData, m_pContent->GetLastModified().c_str());
		strcat(m_pData, "\r\n");

		// ETag
		strcat(m_pData, "ETag: ");
		strcat(m_pData, m_pContent->GetETag().c_str());
		strcat(m_pData, "\r\n");

		// Content-Type
		strcat(m_pData, "Content-Type: ");
		strcat(m_pData, m_pContent->GetContentType().c_str());
		strcat(m_pData, "\r\n");

		// Content-Length
		char szLen[200] = {0};
		__int64 lLen = m_pContent->GetContentLength();
		strcat(m_pData, "Content-Length: ");
		strcat(m_pData, _i64toa(lLen, szLen, 10));
		strcat(m_pData, "\r\n");

		// Content-Range: bytes %d-%d/%d\r\n"
		if(SC_PARTIAL == GetServerCode())
		{
			strcat(m_pData, "Content-Range: ");
			strcat(m_pData, m_pContent->GetContentRange().c_str());
			strcat(m_pData, "\r\n");
		}
	}
	else
	{
		// Content-Length 
		strcat(m_pData, "Content-Length: 0\r\n");
	}

	// "Accept-Ranges: bytes" 支持断点续传.
	strcat(m_pData, "Accept-Ranges: bytes\r\n");

	// 只支持 GET 和 HEAD 方法
	if(GetServerCode() == SC_BADMETHOD)
	{
		strcat(m_pData, "Allow: GET, HEAD\r\n");
	}

	// XServer
	strcat(m_pData, "Server: Que's HTTP Server/1.5\r\n");

	// connection,并结束
	strcat(m_pData, "Connection: close\r\n\r\n");

	// 客户端是否只要求响应头.
	if(m_Method == METHOD_HEAD)
	{
		m_pContent->Close();
	}

	// 计算响应头的长度
	m_nHeaderSize = strlen(m_pData);

	return TRUE;
}

int CHTTPResponse::PopData(void* pData, int nSize)
{	
	if(IsEOF()) return 0;

	int nReaded = m_nHeaderSize - m_nReadPos;

	// 如果请求头中还有数据先从 pData 中读取请求头
	if(nReaded > 0)
	{
		if(nReaded > nSize) nReaded = nSize;
		memcpy(pData, m_pData + m_nReadPos, nReaded);
		m_nReadPos += nReaded;
	}
	
	// 如果从请求头中读取到的数据不够,则从Content中读取
	if(nReaded < nSize && m_pContent->IsOpen())
	{
		nReaded += m_pContent->Read((char*)pData + nReaded, nSize - nReaded);
	}
	
	return nReaded;
}

BOOL CHTTPResponse::IsEOF()
{
	return m_nReadPos == m_nHeaderSize && ( m_pContent == NULL || m_pContent->IsOpen() && m_pContent->IsEOF());
}