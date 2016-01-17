#ifndef POSTGRESQLUSER_H
#define POSTGRESQLUSER_H

#include <string>
using namespace std;

typedef struct pg_conn PGconn;
typedef struct pg_result PGresult;

class CPostGreDataBase
{
public:
	CPostGreDataBase(const char *pghost, const char *pgport,
		const char *pgoptions, const char *pgtty,
		const char *dbName,
		const char *login, const char *pwd);

	~CPostGreDataBase();

	bool IsConnected();
	bool ExcuteSQl(const char * pszSQl);
	void ClearExcuteResult();
	int  GetRowsCount();
	int  GetColumCount();
	int  GetColumIndex(const char *pszFieldName);
	const char * GetColumName(int iColunIndex);
	string GetFieldValue(int iRow,int iColum);
	int GetFieldValueAsInteger(int iRow,int iColum);
	int GetFieldLength(int iRow,int iColum);
	unsigned int GetFieldType(int iColum);
private:
	PGconn   * m_conn;
	PGresult *m_res;
	bool	m_bIsConnected;
};

void  ConvertToUnicode(string &str);
string ConvertToUTF8(const std::wstring strWide);
void IFormat(wchar_t * Dst,wstring format,int IntVal);
void IFormat(wchar_t * Dst,wstring format,double dVal);
void IFormat(wchar_t * Dst,wstring format,const char * Val);
void IFormat(wchar_t * Dst,wstring format,const wchar_t * Val);


template<typename TypeValue>
void GetType(/*wchar_t * Dst,wstring format,*/TypeValue value)
{
	if (typeid(value) ==typeid(int))
	{
		const char * pszTypeName = typeid(value).name();
		printf("%s\n",pszTypeName);
	}
	else if (typeid(value) ==typeid(double))
	{
		const char * pszTypeName = typeid(value).name();
		printf("%s\n",pszTypeName);
	}
	else if (typeid(value) ==typeid(const char *))
	{
		const char * pszTypeName = typeid(value).name();
		printf("%s\n",pszTypeName);
	}
	else if (typeid(value) ==typeid(const wchar_t * ))
	{
		const char * pszTypeName = typeid(value).name();
		printf("%s\n",pszTypeName);
	}
	else
		return;
}

#endif //POSTGRESQLUSER_H