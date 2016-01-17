#include "Core.h"
#include "DesUtil.h"
#include "MyDiskInfo.h"
#include <time.h>
#include <windows.h>
#include<boost/date_time.hpp>
using namespace boost;
using namespace boost::posix_time;
using namespace boost::gregorian;
/*
**SYSTEMTIME转time_t
*/
time_t SystemTimeToTimet(SYSTEMTIME st)
{
	FILETIME ft;
	SystemTimeToFileTime( &st, &ft );
	LONGLONG nLL;
	ULARGE_INTEGER ui;
	ui.LowPart = ft.dwLowDateTime;
	ui.HighPart = ft.dwHighDateTime;
	nLL = ((LONGLONG)ft.dwHighDateTime << 32) + ft.dwLowDateTime;
	time_t pt = (long)((LONGLONG)(ui.QuadPart - 116444736000000000) / 10000000);
	return pt;
}

bool  IsLicenseTimeVaild(const char * pszTimeStart,int iHourNeed)
{
	int iY,iM,iD;
	sscanf_s(pszTimeStart,"%04d%2d%02d",&iY,&iM,&iD);
	date d(iY,iM,iD+1);
	ptime pEnd(d,time_duration(iHourNeed,0,0,0));
	ptime now = second_clock::local_time();
	if (now < pEnd)
	   return true;

	return false;
}
char* GetRegsiterKey(const char * pszCode,const char * pszSoftID,int iValidHourTime)
{
	char szTime[14] = {0};
	if (iValidHourTime > 0)
	{
		ptime now = second_clock::local_time();
		sprintf_s(szTime,"%04d%02d%02d%05d",now.date().year(),now.date().month(),now.date().day(),iValidHourTime);
	}
	else
		sprintf_s(szTime,"00000000%05d",iValidHourTime);

	DesUtil Utl(pszCode);
	string strKeySub =  Utl.Encrypt(string(szTime) + pszSoftID);
	string strOutLic= string(szTime) + string(pszSoftID) + strKeySub;
	return _strdup(strOutLic.c_str());
}
bool  IsValidCode(const char * pszClientPcCode,const char * pszSoftID,const char * pszKeyValue,string & strErrMsg)
{
	strErrMsg = "许可无效！";
	if (pszClientPcCode==NULL || pszSoftID == NULL || pszKeyValue==NULL)
		return false;

	if (strcmp(pszClientPcCode,"")==0 ||
		strcmp(pszSoftID,"")==0 ||
		strcmp(pszKeyValue,"")==0)
		return false;

	if (strlen(pszKeyValue) < 15)
		return false;

	string strTmp = pszKeyValue;
	string strTimeKey  = strTmp.substr(0,8);
	string strDayValidSubstr = strTmp.substr(8,5);

	DesUtil Utl(pszClientPcCode);
	string strKeySub   =  Utl.Encrypt(strTimeKey + strDayValidSubstr + pszSoftID);
	string strCheckNow = strTimeKey + strDayValidSubstr + string(pszSoftID) + strKeySub;

	bool bOk = false;
	if (strcmp(strCheckNow.c_str(),pszKeyValue)==0)
		bOk = true;
	else
		return false;

	if (bOk && strcmp(strTimeKey.c_str(),"00000000")==0) // 永久有效的不比较时间
	{
		strErrMsg.clear();
		return true;
	}
	
	int iHourNeed = atoi(strDayValidSubstr.c_str());

	if (!IsLicenseTimeVaild(strTimeKey.c_str(),iHourNeed))
	{
		strErrMsg = "许可已过期";
		return false;
	}

	strErrMsg.clear();
	return true;
}
// 获取硬件ID
char * GetPcIdStr()
{
	CMyDiskInfo info;
	if (info.GetDiskInfo(0)!=0)
		return _strdup("");

	string str = string(info.szModelNumber) +  string("-") + info.szSerialNumber;
	return _strdup(str.c_str());
}
bool WriteLicenseToReg(const char * pszKeyValue,const char * pszSoftID)
{
	HKEY RootKey = HKEY_LOCAL_MACHINE;      //注册表主键名称
	HKEY hKey;
	BYTE *pDATA = LPBYTE(pszKeyValue);    //值的内容
	DWORD cb= sizeof(unsigned char)*strlen(pszKeyValue); 
	if(RegCreateKey(RootKey,"Software\\CugRemoteSence",&hKey)!=ERROR_SUCCESS)
	{	
		OutputDebugString("错误：无法查询有关的注册表信息");
		return false;
	}

	if(RegSetValueEx(hKey,pszSoftID,NULL,REG_SZ,pDATA,cb)!=ERROR_SUCCESS)
		OutputDebugString("错误：无法设置有关的注册表信息");

	RegCloseKey(hKey);

	return true;
}
 char * GetRegsiterKeyFromReg(const char * pszSoftID)
 {

	 HKEY RootKey=HKEY_LOCAL_MACHINE;      //注册表主键名称
	 DWORD dwType = REG_SZ; //定义读取数据类型
	 DWORD dwLength=256;
	 HKEY hKey;

	 if(RegOpenKeyEx(RootKey,"Software\\CugRemoteSence",0,KEY_READ,&hKey)!=ERROR_SUCCESS)
	 {
		 return _strdup("");
	 }
	 char  content[1024]={0};
	 LSTATUS ls = RegQueryValueEx(hKey,pszSoftID,NULL,&dwType,(unsigned char *)content,&dwLength);
	 RegCloseKey(hKey);
	 if(ls!=ERROR_SUCCESS)
	 {
	
		 return _strdup("");
	 }

	 return _strdup(content);
 }
 void FreeStrMen(char * pData)
 {
	 if (pData == NULL)
		 return;

	 free(pData);
 }