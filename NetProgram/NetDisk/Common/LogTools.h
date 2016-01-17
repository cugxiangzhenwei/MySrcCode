#pragma once
#include <string>
using namespace  std;

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
#pragma comment(lib,"imagehlp.lib")
#endif


namespace LogTools
{
	//日志
	void InitZToolsLog(const std::string & strLogFileName = "", const std::string strModuleName = "");
	void WriteZToolsLog(const std::string &LogMessage, int LogLevel = 0/*TRACE_LOG_LEVEL*/);
	void WriteZToolsFormatLog(const char *fmt, ...);
};

#define ZTools LogTools
#define LogMsg LogTools::WriteZToolsFormatLog