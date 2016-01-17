#include "PostGreSQLUser.h"
#include <libpq-fe.h>
#include <Windows.h>

void  ConvertToUnicode(string &str)
{
	int nLenOfWcharStr= MultiByteToWideChar(CP_UTF8,0, str.c_str(),-1,NULL,0);
	wchar_t *pWideCharStr=(wchar_t*)malloc(nLenOfWcharStr*sizeof(wchar_t));
	if(pWideCharStr==NULL)
		return;

	MultiByteToWideChar(CP_UTF8,0, str.c_str(),-1,pWideCharStr,nLenOfWcharStr);

	int j = WideCharToMultiByte(CP_ACP, 0, pWideCharStr, -1, NULL, 0, NULL, NULL);
	str.resize(j,0);//重新设置str的大小
	WideCharToMultiByte(CP_ACP, 0, pWideCharStr, -1, &str[0], j, NULL, NULL);
	free(pWideCharStr);
}

string ConvertToUTF8(const std::wstring strWide)
{
	std::string strANSI;
	int iLen = ::WideCharToMultiByte(CP_UTF8, 0, strWide.c_str(), -1, NULL, 0, NULL, NULL);

	if (iLen > 1)
	{ 
		strANSI.resize(iLen-1);
		::WideCharToMultiByte(CP_UTF8, 0, strWide.c_str(), -1, &strANSI[0], iLen, NULL, NULL);
	}

	return strANSI;
}
void IFormat(wchar_t * Dst,wstring format,int IntVal)
{
	wsprintf(Dst,format.c_str(),IntVal);
}
void IFormat(wchar_t * Dst,wstring format,double dVal)
{
	wsprintf(Dst,format.c_str(),dVal);
}
void IFormat(wchar_t * Dst,wstring format,const char * Val)
{
	wsprintf(Dst,format.c_str(),Val);
}
void IFormat(wchar_t * Dst,wstring format,const wchar_t * Val)
{
	wsprintf(Dst,format.c_str(),Val);
}

CPostGreDataBase::CPostGreDataBase(const char *pghost, const char *pgport,
								   const char *pgoptions, const char *pgtty,
								   const char *dbName,
								   const char *login, const char *pwd)
{
	m_res = NULL;
	m_conn = PQsetdbLogin(pghost, pgport, pgoptions, pgtty,dbName,login,pwd);
	int test = PQstatus(m_conn);
	if (PQstatus(m_conn) == CONNECTION_BAD) 
	{ 
		fprintf(stderr, "Connection to database '%s' failed.\n", dbName);
		fprintf(stderr, "%s", PQerrorMessage(m_conn));
		m_bIsConnected = false;
	}
	else
	{
		m_bIsConnected = true;
		printf("连接数据库成功!\n");
	}
}
CPostGreDataBase::~CPostGreDataBase()
{
	ClearExcuteResult();
	PQfinish(m_conn);
}
bool CPostGreDataBase::IsConnected()
{
	return m_bIsConnected;
}
bool CPostGreDataBase::ExcuteSQl(const char * pszSQl)
{
	ClearExcuteResult();
	m_res = PQexec(const_cast<PGconn *>(m_conn), pszSQl);
	ExecStatusType rev = PQresultStatus(m_res);
	if (rev!= PGRES_TUPLES_OK  && rev!= PGRES_TUPLES_OK && rev!=PGRES_COMMAND_OK)
	{
		const char * errStr = PQresultErrorMessage(m_res);
		printf("%s\n",errStr);
		ClearExcuteResult();

		return false;
	}

	return true;
}
void CPostGreDataBase::ClearExcuteResult()
{
	if (m_res)
	{
		PQclear(m_res);
		m_res = NULL;
	}
}
int CPostGreDataBase::GetRowsCount()
{
	if (m_res)
	{
		return PQntuples(m_res);
	}
	else
		return -1;
}
int  CPostGreDataBase::GetColumCount()
{
	if (m_res)
	{
		return PQnfields(m_res);
	}
	else
		return -1;

}
const char * CPostGreDataBase::GetColumName(int iColunIndex)
{
	return  PQfname(m_res,iColunIndex);
}
string CPostGreDataBase::GetFieldValue(int iRow,int iColum)
{
	char * pszFieldValue = PQgetvalue(m_res,iRow,iColum);
	string strValue = pszFieldValue;
	ConvertToUnicode(strValue);

	return strValue;
}
int CPostGreDataBase::GetColumIndex(const char *pszFieldName)
{
	return PQfnumber(m_res,pszFieldName);
}
int CPostGreDataBase::GetFieldValueAsInteger(int iRow,int iColum)
{
	string strValue = GetFieldValue(iRow,iColum);
	int iValue = 0;
	sscanf(strValue.c_str(),"%d",&iValue);
	return iValue;
}

int CPostGreDataBase::GetFieldLength(int iRow,int iColum)
{
	return PQgetlength(m_res,iRow,iColum);
}
unsigned int CPostGreDataBase::GetFieldType(int iColum)
{
	return PQparamtype(m_res,iColum);
}