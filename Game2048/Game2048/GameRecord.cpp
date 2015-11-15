#include "StdAfx.h"
#include "GameRecord.h"
#include "DataDefine.h"

CGameRecord::CGameRecord(void)
{
	AfxGetModuleFileName(NULL,m_strRecordConfigFile);
	m_strRecordConfigFile.Replace("Game2048.exe","Game2048_Record.ini");
}


CGameRecord::~CGameRecord(void)
{
}
void CGameRecord::WriteGameRecord(CGameNumBlock * pBlockArry,int iSize,int iSumScore)
{
	ASSERT(pBlockArry);
	ASSERT(iSize==16);
	for (int i=0;i<iSize;i++)
		m_iShowNumArry[i] = pBlockArry[i].m_iGameNumShow;

	CString strTmp,strTmp2;
	strTmp.Format("%d",iSumScore);
	WritePrivateProfileString("Record","SumScore",strTmp,m_strRecordConfigFile);

	strTmp2.Empty();
	for (int i=0;i<16;i++)
	{
		strTmp.Format("%d,",m_iShowNumArry[i]);
		strTmp2 += strTmp;
	}
	strTmp2.TrimRight(",");
	WritePrivateProfileString("Record","BlokShowNums",strTmp2,m_strRecordConfigFile);
}
BOOL CGameRecord::LoadGameRecord(CGameNumBlock * pBlockArry,int iSize,int &iSumScore)
{
	ASSERT(pBlockArry);
	ASSERT(iSize==16);

	iSumScore = 0;

	CString strTmp,strTmp2;
	GetPrivateProfileString("Record","SumScore","0",strTmp.GetBuffer(100),100,m_strRecordConfigFile);
	strTmp.ReleaseBuffer();
	m_iSumScore = atoi(strTmp);

	GetPrivateProfileString("Record","BlokShowNums","",strTmp2.GetBuffer(1000),1000,m_strRecordConfigFile);
	strTmp2.ReleaseBuffer();
	if (strTmp2.IsEmpty())
		return FALSE;

	int iStart = 0;
	int i = 0;
	while(1)
	{
		strTmp = strTmp2.Tokenize(",",iStart);
		if (strTmp.IsEmpty())
			break;

		m_iShowNumArry[i++] = atoi(strTmp);
	}
	for (int i=0;i<iSize;i++)
		pBlockArry[i].m_iGameNumShow = m_iShowNumArry[i];

	iSumScore = m_iSumScore;
	return TRUE;
}