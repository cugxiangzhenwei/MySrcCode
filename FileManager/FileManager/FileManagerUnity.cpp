#include "stdafx.h"
#include "FileManagerUnity.h"
#include "sqlite3.h"
#include "CodeFunc.h"
void ConvertFileToSqlInsert(vector<CString> & vInOut)
{
	int iSize = vInOut.size();
	#pragma omp parallel for
	for (int i=0;i<iSize;i++)
	{
		CString & str = vInOut[i];
		CString strSQl;
		if (str.Find('\'')>=0)
		{
			char * zSQl = sqlite3_mprintf("insert into FILE(path,type) values('%q',%d)",str,0);
			strSQl = zSQl;
			sqlite3_free(zSQl);
		}
		else
			strSQl.Format("insert into FILE(path,type) values('%s',%d)",str,0);

		str = CodeTools::MB_TO_UTF8((LPCTSTR)strSQl).c_str();
	}
}