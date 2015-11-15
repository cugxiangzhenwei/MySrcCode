#include "SqliteBase.h"
#include "sqlite3.h"
#include "CodeFunc.h"

CSqliteBase::CSqliteBase(void)
{
	m_pDB = NULL;
}


CSqliteBase::~CSqliteBase(void)
{
	if (m_pDB)
	{
		sqlite3_close(m_pDB); 
		m_pDB = NULL;
	}
}


// 创建数据库
CSqliteBase * CSqliteBase::CreateDataBase(const char * pszDataBaseName)
{
	return NULL;
}
// 打开数据库
CSqliteBase * CSqliteBase::OpenDataBase(const char * pszDataBaseName)
{
	//打开路径采用utf-8编码
	//如果路径中包含中文，需要进行编码转换
	CSqliteBase *pDataBase = new CSqliteBase;
	string strUTF8FileName = CodeTools::MB_TO_UTF8(pszDataBaseName);
	int nRes = sqlite3_open(strUTF8FileName.c_str(), &pDataBase->m_pDB);
	if (nRes != SQLITE_OK)
	{
		delete pDataBase;
		pDataBase = NULL;
	}

	return pDataBase;
}
bool CSqliteBase::ExcuteSqlCmd(const char * pszSQL)
{
	string strUTF8Sql = CodeTools::MB_TO_UTF8(pszSQL);

	char* cErrMsg = "";
	int nRes = sqlite3_exec(m_pDB , strUTF8Sql.c_str() ,0 ,0, &cErrMsg);
	if (nRes != SQLITE_OK)  
	{
		printf("%s\n",cErrMsg);
		return false;
	}

	return true;
}
// 添加记录
bool CSqliteBase::AddRecord(const CRecordBase & record)
{
	string strSql = "insert into ";
	strSql += record.GetTableName() + "(";

	const MAP_ATTRIBUTES & aMap = record.GetAttributeMap(); 
	for (MAP_ATTRIBUTES::const_iterator citer = aMap.begin(); citer != aMap.end();citer++)
	{
		if (citer != aMap.begin())
			strSql +=",";

		strSql += citer->first;
	}
	strSql +=") values(";
	for (MAP_ATTRIBUTES::const_iterator citer = aMap.begin(); citer != aMap.end();citer++)
	{
		if (citer != aMap.begin())
			strSql +=",";

		if (citer->second.m_iType== 1) // 字符串类型加引号
		{
			strSql +="'";
			strSql += citer->second.m_strValue;
			strSql +="'";
		}
		else
			strSql += citer->second.m_strValue;
	}
	strSql +=");";

	return ExcuteSqlCmd(strSql.c_str());
}


// 删除记录
bool CSqliteBase::DeleteRecord(const char * pszTable,const char * pszConditonName,const CAttirbuteValue & ConditonValue)
{
	char szSQlBuf[1000] ={0};
	sprintf_s(szSQlBuf,"delete from %s where %s =",pszTable,pszConditonName);
	string strSql = szSQlBuf;
	if (ConditonValue.m_iType==1)
	{
		strSql +="'";
		strSql +=ConditonValue.m_strValue;
		strSql +="';";
	}
	else
		strSql += ConditonValue.m_strValue + ";";

	return ExcuteSqlCmd(strSql.c_str());
}


// 更新记录
bool CSqliteBase::ModifyRecord(const char * pszTable,const char * pszConditonName,const CAttirbuteValue & ConditonValue,const MAP_ATTRIBUTES & newValueMap)
{
	char szSQlBuf[300] ={0};
	sprintf_s(szSQlBuf,"update %s set ",pszTable);
	string strSql = szSQlBuf;

	for (MAP_ATTRIBUTES::const_iterator citer = newValueMap.begin(); citer != newValueMap.end();citer++)
	{
		if (citer != newValueMap.begin())
			strSql +=",";

		strSql += citer->first;
		strSql += "=";
		if (citer->second.m_iType==1)
		{
			strSql +="'";
			strSql +=citer->second.m_strValue;
			strSql +="'";
		}
		else
			strSql += citer->second.m_strValue;
	}

	if (ConditonValue.m_iType ==1 )
		sprintf_s(szSQlBuf," where %s = '%s';",pszConditonName,ConditonValue.m_strValue.c_str());
	else
		sprintf_s(szSQlBuf," where %s = %s;",pszConditonName,ConditonValue.m_strValue.c_str());

	strSql += szSQlBuf;
	return ExcuteSqlCmd(strSql.c_str());
}
// 查询记录
bool CSqliteBase::SelectRecord(const char * pszSQL,SelectResultFunc pSelResult,void *pFunData)
{
	string strUTF8SQl = CodeTools::MB_TO_UTF8(pszSQL);
	char* cErrMsg;
	int nRes = sqlite3_exec(m_pDB , strUTF8SQl.c_str() ,pSelResult ,pFunData, &cErrMsg);
	if (nRes != SQLITE_OK)  
		return false;

	return true;
}
bool CSqliteBase::SelectRecord(const char * pszSQL,int &nRows,int &nColums,char ** & pszResult)
{
	string strUTF8SQl = CodeTools::MB_TO_UTF8(pszSQL);
	char * pErrMsg = NULL;
	int ret = sqlite3_get_table(m_pDB,strUTF8SQl.c_str(),&pszResult,&nRows,&nColums,&pErrMsg);
	if(ret == SQLITE_OK)
		return true;

	if (pErrMsg) printf("%s\n",pErrMsg);
	return false;
}
void CSqliteBase::FreeSelectResult(char ** & pszDbReslut)
{
	sqlite3_free_table(pszDbReslut);
	pszDbReslut = NULL;
}
bool CSqliteBase::CreateTable(const char * pszTable,bool DropIfExist)
{
	char szSqlBuf[1000] ={0};
	if (DropIfExist)
	{
		sprintf_s(szSqlBuf,"drop table if exists %s;",pszTable);
		if (!ExcuteSqlCmd(szSqlBuf))
			return false;
	}
	
	sprintf_s(szSqlBuf,"create table %s(id int primary key,msg varchar(128));",pszTable);
	return ExcuteSqlCmd(szSqlBuf);
}
bool CSqliteBase::IsTableExist(const char * pszTable)
{
	char szSqlBuf[1000] ={0};
	sprintf_s(szSqlBuf,"select * from %s;",pszTable);
	string strUTF8SQl = CodeTools::MB_TO_UTF8(szSqlBuf);
	char * pErrMsg = NULL;
	char ** pszResult = NULL;
	int nRwows(-1),nColums(-1);
	int ret = sqlite3_get_table(m_pDB,strUTF8SQl.c_str(),&pszResult,&nRwows,&nColums,&pErrMsg);
	FreeSelectResult(pszResult);
	if(ret == SQLITE_OK)
	{
		return true;
	}
	return false;
}