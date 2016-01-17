
#ifndef _ZTOOLS_TOOLS_H_
#define _ZTOOLS_TOOLS_H_

#include <stdarg.h>
#include <string>
#include <vector>
#include <cassert>
#include <iostream>
#include <cctype>
#include <algorithm>

#define _ZTOOLS_LOG4_ 1
#ifdef _ZTOOLS_LOG4_
#define LOG4CPLUS_DISABLE_DLL_RUNTIME_WARNING
////////////////////////////////////////日志头文件及宏定义
#include <log4cplus/logger.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/layout.h>
#include <log4cplus/ndc.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/loggingmacros.h>

using namespace log4cplus;

#ifdef _DEBUG
#pragma comment(lib, "log4cplusSD.lib")
#else
#pragma comment(lib, "log4cplusS.lib")
#endif
/////////////////////////////////////////日志头文件及宏定义
static bool s_bLogInit = false;
#endif

namespace ZTools
{

	//UTF8转多字节，用完须手动释放pmb
	bool UTF8ToMB(char*& pmb, const char* pu8);
	//多字节转UTF8，用完须手动释放pu8
	bool MBToUTF8(char*& pu8, const char* pmb);

	//UTF8转多字节
	std::string& UTF8ToMB(std::string& str);
	//多字节转UTF8
	std::string& MBToUTF8(std::string& str);

	std::string hexencode(const std::string& input);

	//格式化字符串
	std::string FormatString(const char *fmt, ...);
	//格式化字符串
	std::string& FormatString(std::string& strResult, const char *fmt, ...);
	//字符串大写
	std::string& UpperString(std::string& str);
	//字符串小写
	std::string& LowerString(std::string& str);
	std::wstring s2ws(const std::string& s);
	std::string ws2s(const std::wstring& ws);

	//数字转字符串
	template <class T> 
	std::string ConvertToString(T value);
	//字符串转数字
	template <class T> 
	void convertFromString(T &value, const std::string &s);
	void split(std::string& s, std::string& delim, std::vector< std::string >* ret);
	DISPID FindId(IDispatch *pObj, LPOLESTR pName);
	HRESULT InvokeMethod(IDispatch *pObj, LPOLESTR pName, VARIANT *pVarResult, VARIANT *p, int cArgs);
	HRESULT GetProperty(IDispatch *pObj, LPOLESTR pName, VARIANT *pValue);
	HRESULT SetProperty(IDispatch *pObj, LPOLESTR pName, VARIANT *pValue);

	//日志
	void InitZToolsLog(const std::string strLogFileName = "", const std::string strModuleName = "");
	void WriteZToolsLog(const std::string &LogMessage, int LogLevel = 0/*TRACE_LOG_LEVEL*/);
	void WriteZToolsFormatLog(const char *fmt, ...);
	//当前路径
	std::string GetCurrentModulePath(std::string strModule = "");
	std::string GetNowTimeStr();
	//当前时间生成的一个十六进制串
	std::string GetCurrentTimeString16();
	//取系统错误对应的字符串描述
	std::string GetLastErrorMessageString(DWORD dwError = 0);
}
#endif /*_ZTOOLS_TOOLS_H_*/