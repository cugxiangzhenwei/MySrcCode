#include <WinSock2.h>
#include <iostream>
#include <io.h>
#include <time.h>
#include <string>
#include "CommonFun.h"
#include "DataJsDefine.h"
#include "SqliteBase.h"
#include "CodeFunc.h"
#include "LogTools.h"
using namespace std;

extern CSqliteBase * g_FileDataBase;

DWORD RegisterUser(SOCKET sockConn)
{
	int iParamJsLen = 0;
	string strErrMsg;
	if (!RecvIntData(sockConn,iParamJsLen,strErrMsg))
		return 0;

	string strJsParam;
	if (!RecvStringData(sockConn,iParamJsLen,strJsParam,strErrMsg))
		return 0;

	CJsonDataRegisterUser DataRegister;
	if (!DataRegister.Parse(strJsParam.c_str()))
	{
		LogMsg("Error 解析JSon出现错误! %s,%s,%d\n",__FILE__ ,__FUNCTION__  ,__LINE__);
		SendData(sockConn,"0",1,strErrMsg);
		closesocket(sockConn);
		return 0;
	}

	int nRows,nColums;
	char ** pszDbResult = NULL;
	string strSQL = FormatString("select * from USER where UserId ='%s';",DataRegister.m_strUserId.c_str());
	g_FileDataBase->SelectRecord(strSQL.c_str(),nRows,nColums,pszDbResult);
	g_FileDataBase->FreeSelectResult(pszDbResult);
	if (nRows>0)
	{
		LogMsg("UserId=%s已经注册，请尝试其他Id\n",DataRegister.m_strUserId.c_str());
		SendData(sockConn,"0",1,strErrMsg);
		closesocket(sockConn);
		return 0;
	}

	// 创建用户文件列表表
	strSQL = FormatString("create table %s(FileId varchar,ItemName varchar,ItemType varchar,ItemDir varchar,FileSize,FileTime,FileMd5);",DataRegister.m_strUserId.c_str());
	if (!g_FileDataBase->ExcuteSqlCmd(strSQL.c_str()))
	{
		LogMsg("注册失败，创建用户文件列表失败,UserId=%s\n",DataRegister.m_strUserId.c_str());
		SendData(sockConn,"0",1,strErrMsg);
		closesocket(sockConn);
		return 0;
	}

	strSQL = FormatString("create index ItemDir_Index on %s(ItemDir);",DataRegister.m_strUserId.c_str());
	g_FileDataBase->ExcuteSqlCmd(strSQL.c_str()); //创建查询索引

	strSQL = FormatString("insert into USER(UserId,UserName,PassWord,FileTable,AllSpace,UsedSpace) values('%s','%s','%s','%s','%s','%s');",
		DataRegister.m_strUserId.c_str(),DataRegister.m_strUserName.c_str(),DataRegister.m_strPassWord.c_str(),
		DataRegister.m_strUserId.c_str(),"21990232555520","0");

	bool bOk = g_FileDataBase->ExcuteSqlCmd(strSQL.c_str());
	if (bOk)
	{
		LogMsg("注册成功，UserId=%s\n",DataRegister.m_strUserId.c_str());
		SendData(sockConn,"1",1,strErrMsg);
		closesocket(sockConn);
	}
	else
	{
		LogMsg("注册失败，UserId=%s\n",DataRegister.m_strUserId.c_str());
		SendData(sockConn,"0",1,strErrMsg);
		closesocket(sockConn);
	}

	return 0;
}

DWORD Login(SOCKET sockConn)
{
	CDataLogin LoginData;
	string strErrMsg;
	if (!RecvData(sockConn,&LoginData,sizeof(CDataLogin),strErrMsg))
		return 0;

	CDataLoginResult LoginResData;
	int nRows,nColums;
	char ** pszDbResult = NULL;
	string strSQL = FormatString("select * from USER where UserId ='%s';",LoginData.m_szUsrId);
	g_FileDataBase->SelectRecord(strSQL.c_str(),nRows,nColums,pszDbResult);
	if (nRows!=1)
	{
		g_FileDataBase->FreeSelectResult(pszDbResult);
		LogMsg("UserId=%s账户不存在，登录失败!\n",LoginData.m_szUsrId);
		strcpy_s(LoginResData.m_szErrMsg,"账户不存在!");
	}
	else
	{
		if (_stricmp(LoginData.m_szPassWd,pszDbResult[nColums + 2])!=0)
		{
			LogMsg("UserId=%s登录密码错误，登录失败!\n",LoginData.m_szUsrId);
			strcpy_s(LoginResData.m_szErrMsg,"密码错误!");
			g_FileDataBase->FreeSelectResult(pszDbResult);
		}
		else
		{
			LogMsg("UserId=%s,登录成功!",LoginData.m_szUsrId);
			strcpy_s(LoginResData.m_szUsrId,LoginData.m_szUsrId);
			string strUsrName = CodeTools::UTF8_TO_MB(pszDbResult[nColums+1]);
			strcpy_s(LoginResData.m_szUserName,strUsrName.c_str());
			LoginResData.m_iAllSpace  = _atoi64(pszDbResult[nColums+4]);
			LoginResData.m_iUsedSpace = _atoi64(pszDbResult[nColums+5]);
			g_FileDataBase->FreeSelectResult(pszDbResult);
		}
	}

	if (!SendData(sockConn,(char*)&LoginResData,sizeof(CDataLoginResult),strErrMsg))
		return 0;

	closesocket(sockConn);
	return 0;
}

void UpdateUserSpace(const char *pszUsrId,LONGLONG iSpace,bool bAdd)
{
	ULONGLONG iUsedSpace = 0;
	int nRows,nColums;
	char ** pszDbResult = NULL;
	string strSQL = FormatString("select UsedSpace from USER where UserId ='%s';", pszUsrId);
	g_FileDataBase->SelectRecord(strSQL.c_str(),nRows,nColums,pszDbResult);
	if (pszDbResult)
	{
		iUsedSpace = _atoi64(pszDbResult[1]);
		g_FileDataBase->FreeSelectResult(pszDbResult);
	}

	if (iUsedSpace > 21990232555520)
	{
		LogMsg("查询已经使用的空间出现异常,大于21990232555520，大小为%lld",iUsedSpace);
	}

	if (bAdd)
		iUsedSpace += iSpace;
	else
		iUsedSpace -= iSpace;

	if (iUsedSpace > 21990232555520)
	{
		LogMsg("计算得出新的已经使用的空间出现异常,大于21990232555520，大小为%lld",iUsedSpace);
	}

	strSQL = FormatString("update USER set UsedSpace = %lld where UserId ='%s';",iUsedSpace, pszUsrId);
	g_FileDataBase->ExcuteSqlCmd(strSQL.c_str());
}