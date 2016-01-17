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

 DWORD SendFileListToClient(SOCKET sockConn)
 {
	 int iParamJsLen = 0;
	 string strErrMsg;
	 if (!RecvIntData(sockConn,iParamJsLen,strErrMsg))
		 return 0;

	 string strDataJsParam;
	 if (!RecvStringData(sockConn,iParamJsLen,strDataJsParam,strErrMsg))
		 return 0;

	CJsonDataGetFileList DataGetList;
	if (!DataGetList.Parse(strDataJsParam.c_str()))
	{
		LogMsg("Error 解析JSon出现错误! %s,%s,%d\n",__FILE__ ,__FUNCTION__  ,__LINE__);
		return 0;
	}

	CJsonDataSendFileList DataSendList;
	DataSendList.m_bRecursion = DataGetList.m_bRecursion;
	DataSendList.m_strUsrId = DataGetList.m_strUsrId;
	DataSendList.m_strNetDiskDir = DataGetList.m_strNetDiskDir;


	int nRows,nColums;
	char ** pszDbResult = NULL;
	string strSQL = FormatString("select * from %s where ItemDir='%s';",DataSendList.m_strUsrId.c_str(),DataSendList.m_strNetDiskDir.c_str());
	g_FileDataBase->SelectRecord(strSQL.c_str(),nRows,nColums,pszDbResult);
	if (nRows > 0)
	{
		for (int i=0;i<nRows;i++)
		{
			FileListItem Item;
			strcpy_s(Item.szFileId,pszDbResult[nColums + i*nColums]);

			string strTmpName = CodeTools::UTF8_TO_MB(pszDbResult[nColums+1 + i*nColums]);
			strcpy_s(Item.szItemName,strTmpName.c_str());

			strcpy_s(Item.szItemType,pszDbResult[nColums+2 + i*nColums]);
			Item.iFileLength = _atoi64(pszDbResult[nColums+4 + i*nColums]);
			Item.iLastModifyTime = _atoi64(pszDbResult[nColums+5 + i*nColums]);
			strcpy_s(Item.szMd5,pszDbResult[nColums+6 + i*nColums]);
			DataSendList.m_vFileList.push_back(Item.GetCopied());
		}
		g_FileDataBase->FreeSelectResult(pszDbResult);
	}
	
	string strReturn = DataSendList.GetResultJson();
	int iDataLen = strReturn.length();
	if (iDataLen<=0)
	{
		LogMsg("Error 生成JSon串出现错误! %s,%s,%d\n",__FILE__ ,__FUNCTION__  ,__LINE__);
		return 0;
	}

	iDataLen +=1;
	if (!SendIntData(sockConn,iDataLen,strErrMsg))
		return 0;

	if (!SendData(sockConn,(char*)strReturn.c_str(),iDataLen,strErrMsg))
		return 0;

	closesocket(sockConn);
	return 0;
 }