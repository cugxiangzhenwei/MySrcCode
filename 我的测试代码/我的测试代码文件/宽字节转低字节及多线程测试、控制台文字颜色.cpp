string ConvertWStringToString(const wstring &ws)
{
	std::string curLocale = setlocale(LC_ALL, NULL); // curLocale = "C";  
	setlocale(LC_ALL, "chs");  
	//从wstring到wchar *  
	const wchar_t* _Source = ws.c_str();  

	//string对应的长度  
	size_t _Dsize = 2 * ws.size() + 1;  
	char *_Dest = new char[_Dsize];  
	//初始化内存  
	memset(_Dest,0,_Dsize);  

	//从unicode码转成ANSI码  
	wcstombs(_Dest, _Source, _Dsize);  

	//从char *转为string  
	string result = _Dest;  

	delete []_Dest;  
	return result;  
}


wstring ConvertStringToWString(const string &s)
{
	//从string转为char *  
	const char * _Source = s.c_str();  

	//获取字符串长度  
	size_t _Dsize = s.size() + 1;  
	wchar_t *_Dest = new wchar_t[_Dsize];  
	//初始化内存  
	wmemset(_Dest, 0, _Dsize);  

	//从ANSI到Unicode码  
	mbstowcs(_Dest,_Source,_Dsize);  

	std::wstring result = _Dest;  

	delete []_Dest;  
	return result;  
}

//设置控制台输出颜色  
BOOL SetConsoleColor(WORD wAttributes)  
{  
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);  
	if (hConsole == INVALID_HANDLE_VALUE)  
		return FALSE;     
	return SetConsoleTextAttribute(hConsole, wAttributes);  
}

HANDLE g_event;
struct ThreadParam
{
	string  strSrc;
	string  strDst;
	int iColorID;
	CProcessBase *pProgress;
};
static DWORD WINAPI ThreadPro(LPVOID pPram)
{
	ThreadParam * ThreadParmaMine = reinterpret_cast<ThreadParam *>(pPram);
	if ( NULL == ThreadParmaMine)
		return RE_PARAMERROR;

	printf("线程 %d 开始解码...\n",GetCurrentThreadId());
	string strSrc = ThreadParmaMine->strSrc;
	string strDst = ThreadParmaMine->strDst;
	CProcessBase *pProgress = ThreadParmaMine->pProgress;
	int iColorId = ThreadParmaMine->iColorID;
	SetEvent(g_event); // 触发事件,允许主线程进行下次循环
	if (iColorId ==0)
		SetConsoleColor(FOREGROUND_RED);  
	else if (iColorId ==1)
		SetConsoleColor(FOREGROUND_GREEN);  
	else if (iColorId ==2)
		SetConsoleColor(FOREGROUND_BLUE);  
	else if (iColorId ==3)
		SetConsoleColor(FOREGROUND_RED|FOREGROUND_GREEN);  
	else if (iColorId ==4)
		SetConsoleColor(FOREGROUND_GREEN|FOREGROUND_BLUE);  
	else if (iColorId ==5)
		SetConsoleColor(FOREGROUND_BLUE|FOREGROUND_RED);

	bool bNewHere = false;
	if (pProgress==NULL)
	{
		bNewHere = true;
		pProgress = new CConsoleProcess();
	}
	int iRev = SJ9_DataExtract(strSrc.c_str(), strDst.c_str(),pProgress);
	if (bNewHere)
	{
		delete pProgress;
		pProgress = NULL;
	}
	return iRev;
}
static int TestSJ9Extract(char ** pszFileList,const char * pszOutDir)
{
	//SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	int iCount = CSLCount(pszFileList);
	HANDLE *handleArry =  new HANDLE[iCount];
	ThreadParam * ThreadParamArry = new ThreadParam[iCount];

	g_event = CreateEvent(NULL,TRUE,FALSE,NULL);
	string strDst = pszOutDir;

	for (int i=0;i< iCount;i++)
	{
		ThreadParamArry[i].strSrc = pszFileList[i];
		ThreadParamArry[i].strDst = strDst;
		ThreadParamArry[i].pProgress = NULL;
		ThreadParamArry[i].iColorID = i;
		handleArry[i] = CreateThread(NULL,NULL,ThreadPro,&ThreadParamArry[i],0,NULL);
		WaitForSingleObject(g_event,INFINITE); //等待事件被触发
	}
	int iRev = WaitForMultipleObjects(iCount,handleArry,TRUE,INFINITE);

	for (int i=0;i<iCount;i++)
		CloseHandle(handleArry[i]);

	delete handleArry;
	handleArry = NULL;

	delete ThreadParamArry;
	ThreadParamArry = NULL;

	CloseHandle(g_event);
	return iRev;
}
