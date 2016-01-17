#include "PostGreSQLUser.h"
#include <Windows.h>
#include <memory>
void Test()
{
	char *pghost, *pgport, *pgoptions, *pgtty; 
	char *dbName;
	pghost = "localhost";
	pgport = "5432";
	pgoptions = NULL;
	pgtty = NULL;
	dbName = "jps_cn";
	auto_ptr<CPostGreDataBase> DataSet(new CPostGreDataBase(pghost, pgport, pgoptions, pgtty,dbName,"postgres","acfgkp1017"));
	const char * sqltxt = "SELECT * FROM task";
	bool bSuccess = DataSet->ExcuteSQl(sqltxt);
	if(!bSuccess)
		return;

	int iRowCount = DataSet->GetRowsCount();
	int iColums = DataSet->GetColumCount();
	unsigned int iFieldType = DataSet->GetFieldType(1);
	int iFieldLength = DataSet->GetFieldLength(0,1);

	for (int i=0;i<iColums;i++)
	{
		const char *pszFieldName = DataSet->GetColumName(i);
		printf("%s\t",pszFieldName);
	}

	for (int i=0;i<iRowCount;i++)
	{
		printf("\n");
		for (int j=0;j<iColums;j++)
		{
			string strValue = DataSet->GetFieldValue(i,j);
			printf("%s\t",strValue.c_str());
		}
	}
	printf("\n");

	sqltxt = "SELECT MAX(task_id) FROM task";
	bSuccess = DataSet->ExcuteSQl(sqltxt);
	if (!bSuccess)
		return;

	iRowCount = DataSet->GetRowsCount();
	iColums = DataSet->GetColumCount();
	const char *pszGetFieldName =  DataSet->GetColumName(0);
	string strValue = DataSet->GetFieldValue(0,0);
	int iMaxTaskId = DataSet->GetFieldValueAsInteger(0,0);

	wchar_t wszField[] = L"INSERT INTO task(task_id,line_id,next_id,status,params,job_id,\"task_TypeId\",priority,task_pos)";
	wchar_t wxml[] = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	wchar_t wszValue[500] = {0};
	wsprintf(wszValue,L"VALUES(%d,%d,%d,%d,'%s',%d,%d,%d,'%s')",iMaxTaskId+1,6,-1,-1,wxml,-1,45,50,L"9898,344");

	wstring wstr = wstring(wszField)  + wszValue;

	string strSQl = ConvertToUTF8(wstr);

	bSuccess = DataSet->ExcuteSQl(strSQl.c_str());
	if (!bSuccess)
		return;

	iRowCount = DataSet->GetRowsCount();
	iColums = DataSet->GetColumCount();

	GetType(L"3434");
	GetType(34);
	GetType("dsfsd");
	GetType(43.76);
}
void main()
{
	for (int i=0;i<10000;i++)
	{
		Test();
	}
	
}