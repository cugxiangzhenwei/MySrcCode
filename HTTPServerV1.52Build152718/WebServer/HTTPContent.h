/* Copyright (C) 2011 阙荣文
 *
 * 这是一个开源免费软件,您可以自由的修改和发布.
 * 禁止用作商业用途.
 *
 * 联系原作者: querw@sina.com 
*/

#pragma once

#include "HTTPDef.h"

class CHTTPContent
{
public:
	CHTTPContent(void);
	virtual ~CHTTPContent(void);

protected:
	std::string m_strContentType;
	std::wstring m_strFileName;

	FILE* m_pFile;
	struct _stat32i64 m_FileInf;
	__int64 m_lFrom;
	__int64 m_lTo;

	int m_nBufferSize;
	char* m_pData;
	int m_nReadPos;
	int m_nWritePos;
	int m_nOpenType;

	std::string getContentTypeFromFileName(const char* pszFileName);

public:
	BOOL OpenFile(const wchar_t* pszFileName, __int64 &lFrom, __int64 &lTo);
	BOOL OpenFile(const wchar_t* pszFileName);
	BOOL OpenDir(const std::wstring &strUrl, const std::wstring &strFilePath);
	BOOL OpenText(const char* pszTxt, int nSize);
	BOOL OpenHtml(const char* pszHtmlTxt, int nSize);
	BOOL OpenData(const char* pData, int nSize);
	void Close();

	std::string GetContentType();
	__int64 GetContentLength();
	std::string GetLastModified();
	std::string GetETag();
	std::string GetContentRange();
	
	BOOL IsOpen();
	BOOL IsEOF();
	int WriteString(const char* pszString);
	int Read(void* pData, int nSize);
	int Write(void* pData, int nSize); // 为将来支持 PUT, POST 方法做准备.
	int Seek(int nOffset);
};
