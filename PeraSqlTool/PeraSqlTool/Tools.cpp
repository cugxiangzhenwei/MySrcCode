#include "StdAfx.h"
#include "Tools.h"
#include <sstream>
#include <iomanip>
#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")

namespace ZTools
{
	//格式化字符串
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

	//格式化字符串
	std::string& FormatString(std::string& strResult, const char *fmt, ...) 
	{ 
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

	//字符串大写
	std::string& UpperString(std::string& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), std::toupper);
		return str;
	}

	//字符串小写
	std::string& LowerString(std::string& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), std::tolower);
		return str;
	}

	//UTF8编码转为多字节编码 
	bool UTF8ToMB( char*& pmb, const char* pu8 )
	{
		// convert an UTF8 string to widechar   
		int nLen = MultiByteToWideChar(CP_UTF8, 0, pu8, -1, NULL, 0);  

		WCHAR* lpszW = NULL;  
		try  
		{  
			lpszW = new WCHAR[nLen];  
		}  
		catch(std::bad_alloc &/*memExp*/)  
		{  
			return false;  
		}  

		int nRtn = MultiByteToWideChar(CP_UTF8, 0, pu8, -1, lpszW, nLen);  

		if(nRtn != nLen)  
		{  
			delete[] lpszW;  
			return false;  
		}  

		// convert an widechar string to Multibyte   
		int MBLen = WideCharToMultiByte(CP_ACP, 0, lpszW, nLen, NULL, 0, NULL, NULL);  
		if (MBLen <=0)  
		{  
			return false;  
		}	
		pmb = new char[MBLen+1];
		nRtn = WideCharToMultiByte(CP_ACP, 0, lpszW, nLen, pmb, MBLen, NULL, NULL);
		delete[] lpszW;  

		if(nRtn != MBLen)  
		{		
			delete pmb;
			return false;  
		}  
		return true;  
	}

	//多字节编码转为UTF8编码
	bool MBToUTF8( char*& pu8, const char* pmb )
	{
		// convert an MBCS string to widechar   
		int nLen = MultiByteToWideChar(CP_ACP, 0, pmb, -1, NULL, 0);  

		WCHAR* lpszW = NULL;  
		try  
		{  
			lpszW = new WCHAR[nLen];  
		}  
		catch(std::bad_alloc &/*memExp*/)  
		{  
			return false;  
		}  

		int nRtn = MultiByteToWideChar(CP_ACP, 0, pmb, -1, lpszW, nLen);  

		if(nRtn != nLen)  
		{  
			delete[] lpszW;  
			return false;  
		}  
		// convert an widechar string to utf8  
		int utf8Len = WideCharToMultiByte(CP_UTF8, 0, lpszW, nLen, NULL, 0, NULL, NULL);  
		if (utf8Len <= 0)  
		{  
			return false;  
		}  

		pu8 = new char[utf8Len + 1];

		nRtn = WideCharToMultiByte(CP_UTF8, 0, lpszW, nLen, pu8, utf8Len, NULL, NULL);
		pu8[utf8Len] = '\0';
		delete[] lpszW;  

		if (nRtn != utf8Len)  
		{  
			//pu8.clear();  
			delete pu8;
			return false;  
		}  
		return true;
	}

	//UTF8转多字节
	std::string& UTF8ToMB(std::string& str)
	{
		char* sTemp;
		UTF8ToMB(sTemp, str.c_str());
		str = sTemp;
		delete[] sTemp;
		sTemp = NULL;
		return str;
	}
	//多字节转UTF8
	std::string& MBToUTF8(std::string& str)
	{
		char* sTemp;
		MBToUTF8(sTemp, str.c_str());
		str = sTemp;
		delete[] sTemp;
		sTemp = NULL;
		return str;
	}

	std::string ws2s(const std::wstring& ws)
	{
		std::string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";

		setlocale(LC_ALL, "chs"); 

		const wchar_t* _Source = ws.c_str();
		size_t _Dsize = 2 * ws.size() + 1;
		char *_Dest = new char[_Dsize];
		memset(_Dest,0,_Dsize);
		size_t st;
		wcstombs_s(&st,_Dest,_Dsize,_Source,_Dsize);
		std::string result = _Dest;
		delete []_Dest;

		setlocale(LC_ALL, curLocale.c_str());

		return result;
	}

	std::wstring s2ws(const std::string& s)
	{
		std::string curLocale = setlocale(LC_ALL, NULL); 
		setlocale(LC_ALL, "chs"); 

		const char* _Source = s.c_str();
		size_t _Dsize = s.size() + 1;
		wchar_t *_Dest = new wchar_t[_Dsize];
		wmemset(_Dest, 0, _Dsize);
		size_t st;
		mbstowcs_s(&st,_Dest,_Dsize,_Source,_Dsize);
		std::wstring result = _Dest;
		delete []_Dest;

		setlocale(LC_ALL, curLocale.c_str());

		return result;
	}

	//数字转字符串
	template <class T> 
	std::string ConvertToString(T value) 
	{
		std::stringstream ss;
		ss << value;
		return ss.str();
	}

	//字符串转数字
	template <class T> 
	void convertFromString(T &value, const std::string &s)
	{
		std::stringstream ss(s);
		ss >> value;
	}

	std::string hexencode(const std::string& input)
	{
		std::ostringstream ssOut;
		ssOut << std::setbase(16);
		for(std::string::const_iterator i = input.begin(); i != input.end(); ++i)
		{
			//if(isalnum(*i))
			if ((*i<='9' && *i>='0') || (*i<='z' && *i>='a') || (*i<='Z' && *i>='A'))
			{
				ssOut << *i;
			}
			else
				ssOut << '%' << std::setw(2) << std::setfill('0') << ((unsigned int)(unsigned char)*i);
		}
		return ssOut.str();
	}

	void split(std::string& s, std::string& delim, std::vector< std::string >* ret)
	{
		size_t last = 0;
		size_t index = s.find(delim, last);
		while (index!=std::string::npos)
		{
			ret->push_back(s.substr(last,index-last));
			last=index+delim.length();
			index=s.find(delim,last);
		}
		if (s.length()-last>0)
		{
			ret->push_back(s.substr(last,s.length()-last));
		}
	}

	DISPID FindId(IDispatch *pObj, LPOLESTR pName)
	{
		DISPID id = 0;
		if(FAILED(pObj->GetIDsOfNames(IID_NULL,&pName,1,LOCALE_SYSTEM_DEFAULT,&id))) id = -1;
		return id;
	}

	HRESULT InvokeMethod(IDispatch *pObj, LPOLESTR pName, VARIANT *pVarResult, VARIANT *p, int cArgs)
	{
		DISPID dispid = FindId(pObj, pName);
		if(dispid == -1) return E_FAIL;

		DISPPARAMS ps;
		ps.cArgs = cArgs;
		ps.rgvarg = p;
		ps.cNamedArgs = 0;
		ps.rgdispidNamedArgs = NULL;

		return pObj->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &ps, pVarResult, NULL, NULL);
	}

	HRESULT GetProperty(IDispatch *pObj, LPOLESTR pName, VARIANT *pValue)
	{
		DISPID dispid = FindId(pObj, pName);
		if(dispid == -1) return E_FAIL;

		DISPPARAMS ps;
		ps.cArgs = 0;
		ps.rgvarg = NULL;
		ps.cNamedArgs = 0;
		ps.rgdispidNamedArgs = NULL;

		return pObj->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET, &ps, pValue, NULL, NULL);
	}

	HRESULT SetProperty(IDispatch *pObj, LPOLESTR pName, VARIANT *pValue)
	{
		DISPID dispid = FindId(pObj, pName);
		if(dispid == -1) return E_FAIL;

		DISPPARAMS ps;
		ps.cArgs = 1;
		ps.rgvarg = pValue;
		ps.cNamedArgs = 0;
		ps.rgdispidNamedArgs = NULL;

		return pObj->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYPUT, &ps, NULL, NULL, NULL);
	}
	//////////////////////////////////////////////////////////////////////////
	//用法举例
	/*
	virtual void OnDocumentCompleted()
	{
	VARIANT params[10];

	//获取window
	IDispatch *pHtmlWindow = GetHtmlWindow();

	//获取document
	CVariant document;
	params[0].vt = VT_BSTR;
	params[0].bstrVal = L"document";
	GetProperty(pHtmlWindow, L"document", &document);

	//获取button1
	CVariant button1;
	params[0].vt = VT_BSTR;
	params[0].bstrVal = L"button1";
	InvokeMethod(document.pdispVal, L"getElementById", &button1, params, 1);

	//处理button1的onclick事件
	params[0].vt = VT_DISPATCH;
	params[0].pdispVal = new JsFunction(button1_onclick, this);
	SetProperty(button1.pdispVal, L"onclick", params);
	}

	IDispatch* GetHtmlWindow()
	{
	if(_pHtmlWnd2 != NULL) return _pHtmlWnd2;

	IWebBrowser2* pWB2 = GetWebBrowser2();
	NULLTEST(pWB2);

	IHTMLDocument2 *pHtmlDoc2 = NULL;
	IDispatch* pDp =  NULL;
	HRTEST_SE(pWB2->get_Document(&pDp),L"DWebBrowser2::get_Document 错误");
	HRTEST_SE(pDp->QueryInterface(IID_IHTMLDocument2,(void**)&pHtmlDoc2),L"QueryInterface IID_IHTMLDocument2 失败");

	HRTEST_SE(pHtmlDoc2->get_parentWindow(&_pHtmlWnd2),L"IHTMLWindow2::get_parentWindow 错误");

	IDispatch *pHtmlWindown = NULL;
	HRTEST_SE(_pHtmlWnd2->QueryInterface(IID_IDispatch, (void**)&pHtmlWindown),L"GetDispHTMLWindow2错误");

	pHtmlDoc2->Release();

	return pHtmlWindown;

	RETURN:
	return NULL;
	}
	*/
	//////////////////////////////////////////////////////////////////////////

	void InitZToolsLog(const std::string strLogFileName/* = ""*/, const std::string strModuleName/* = ""*/)
	{
#ifdef _ZTOOLS_LOG4_
		if (s_bLogInit)
		{
			return;
		}
		s_bLogInit = true;

		std::string strLogFilePath;

		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];
		char exeFullPath[MAX_PATH]; // MAX_PATH
		HMODULE hMod = NULL;
		hMod = GetModuleHandle(strModuleName.c_str());
		GetModuleFileName(hMod, exeFullPath, MAX_PATH);//得到程序模块名称，全路径
		_splitpath_s( exeFullPath, drive, dir, fname, ext );

		if (strLogFileName.empty())
		{
			strLogFilePath = FormatString("%s%sLogZTools\\LogZTools_%s.log", drive, dir, fname);
		}
 		else if (strLogFileName.find('.') == std::string::npos)
 		{
			strLogFilePath = FormatString("%s%sLogZTools\\LogZTools_%s.log", drive, dir, strLogFileName.c_str());
 		}
		else
		{
			strLogFilePath = FormatString("%s%sLogZTools\\%s", drive, dir, strLogFileName.c_str());
		}

		MakeSureDirectoryPathExists(strLogFilePath.c_str());
  
		//log4cplus::initialize();
		helpers::LogLog::getLogLog()->setInternalDebugging(true);
 		SharedAppenderPtr _appendPtr(new RollingFileAppender(LOG4CPLUS_TEXT(strLogFilePath), 2000*1024, 5));  
 		std::string strPattern;  
		//strPattern = "%D{%Y-%m-%d %H:%M:%S.%q}  - %m [%l]%n";//可以输出代码中的行数等
		strPattern = "%D{%Y-%m-%d %H:%M:%S.%q}  - %m%n";
		_appendPtr->setName(LOG4CPLUS_TEXT("First"));
 		_appendPtr->setLayout( std::auto_ptr<Layout>(new PatternLayout(strPattern))); 

		Logger::getRoot().addAppender(_appendPtr); 

		Logger root = Logger::getRoot();
		Logger test = Logger::getInstance(LOG4CPLUS_TEXT("test"));
		Logger subTest = Logger::getInstance(LOG4CPLUS_TEXT("test.subtest"));
		subTest.setLogLevel(0);
#endif

	}

	void WriteZToolsFormatLog(const char *fmt, ...)
	{
#ifdef _ZTOOLS_LOG4_
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
		ZTools::WriteZToolsLog(strResult);
#endif
	}

	void WriteZToolsLog( const std::string &LogMessage, int LogLevel /*= 0/*TRACE_LOG_LEVEL*/ )
	{

#ifdef USE_DLGLOG
	CPeraWorkspaceApp* pApp = (CPeraWorkspaceApp*)AfxGetApp();
		if(pApp && pApp->m_dlgLog.GetSafeHwnd())
		{
			pApp->m_dlgLog.SendMessage(WM_LOG, (WPARAM)new CString(LogMessage.c_str()));
		}
#endif // USE_DLGLOG
		

#ifdef _ZTOOLS_LOG4_
		InitZToolsLog();
		Logger root = Logger::getRoot();
		Logger test = Logger::getInstance(LOG4CPLUS_TEXT("test"));
		Logger subTest = Logger::getInstance(LOG4CPLUS_TEXT("test.subtest"));

 		switch(LogLevel)
 		{
 		case NOT_SET_LOG_LEVEL:
 			break;
 		case TRACE_LOG_LEVEL:
 			LOG4CPLUS_TRACE(subTest, LogMessage);
 			break;
 		case DEBUG_LOG_LEVEL:
 			LOG4CPLUS_DEBUG(subTest, LogMessage);
 			break;
 		case INFO_LOG_LEVEL:
 			LOG4CPLUS_INFO(subTest, LogMessage);
 			break;
 		case ERROR_LOG_LEVEL:
 			LOG4CPLUS_ERROR(subTest, LogMessage);
 			break;
 		case FATAL_LOG_LEVEL:
 			LOG4CPLUS_FATAL(subTest, LogMessage);
 			break;
 		default:
 			break;
 		}
#endif
	}

	std::string GetNowTimeStr() 
	{
		//得到当前时间
		SYSTEMTIME systime;
		GetLocalTime(&systime);
		std::string strTm = ZTools::FormatString(
			"%04d-%02d-%02d %02d:%02d:%02d"
			, systime.wYear
			, systime.wMonth
			, systime.wDay
			, systime.wHour
			, systime.wMinute
			, systime.wSecond);
		return strTm;
	}

	std::string GetCurrentModulePath( std::string strModule /*= ""*/ )
	{
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];
		char exeFullPath[MAX_PATH]; // MAX_PATH
		HMODULE hMod = NULL;
		if (!strModule.empty())
		{
			hMod = GetModuleHandle(strModule.c_str());
		}
		GetModuleFileName(hMod,exeFullPath,MAX_PATH);//得到程序模块名称，全路径
		_splitpath_s( exeFullPath, drive, dir, fname, ext );
		return ZTools::FormatString("%s%s", drive, dir);
	}

	std::string GetCurrentTimeString16()
	{
		SYSTEMTIME systime;
		GetLocalTime(&systime);
		return FormatString(
			"%X%X%X%X%X%X%X"
			, systime.wYear
			, systime.wMonth
			, systime.wDay
			, systime.wHour
			, systime.wMinute
			, systime.wSecond
			, systime.wMilliseconds);
	}

	std::string GetLastErrorMessageString(DWORD dwError/* = 0*/)
	{
		if (dwError == 0)
		{
			dwError = GetLastError();
		}
		
		LPTSTR pszErrMSG;
		// Use the default system locale since we look for Windows messages.
		// Note: this MAKELANGID combination has 0 as value
		DWORD systemLocale = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);
		// Get the error code's textual description
		BOOL fOk = FormatMessage(
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS |
			FORMAT_MESSAGE_ALLOCATE_BUFFER, 
			NULL, dwError, systemLocale, 
			(LPTSTR) &pszErrMSG, 0, NULL);

		if (!fOk)
		{
			// Is it a network-related error?
			HMODULE hDll = LoadLibraryEx(TEXT("netmsg.dll"), NULL, 
				DONT_RESOLVE_DLL_REFERENCES);

			if (hDll != NULL)
			{
				fOk = FormatMessage(
					FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS |
					FORMAT_MESSAGE_ALLOCATE_BUFFER,
					hDll, dwError, systemLocale,
					(LPTSTR) &pszErrMSG, 0, NULL);
				FreeLibrary(hDll);
			}
		}

		std::string strMessage(pszErrMSG);
		LocalFree(pszErrMSG);  
		return strMessage;
	}
}