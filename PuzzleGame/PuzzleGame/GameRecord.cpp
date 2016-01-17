#include "StdAfx.h"
#include "GameRecord.h"
#include "BlockManager.h"
#include <vector>

CGameRecord::CGameRecord(void)
{
	AfxGetModuleFileName(NULL,m_strRecordConfigFile);
	m_strRecordConfigFile.Replace("PuzzleGame.exe","PuzzleGame_Record.ini");
}


CGameRecord::~CGameRecord(void)
{
}
void CGameRecord::WriteGameRecord(const CString & strBackImage,BOOL bShowGrid,int iGridSize,int iClickCount,const map<int,CPictueBlock> & mapBlocks)
{
	CString strIdList;
	map<int,CPictueBlock>::const_iterator iter = mapBlocks.begin(); 
	for (iter;iter!=mapBlocks.end();iter++)
	{
		CString str;
		str.Format("%d",iter->second.m_iCurPosID);
		strIdList +=str;
		strIdList +=",";
	}
	strIdList.TrimRight(",");
	WritePrivateProfileString("Record","BlockIds",strIdList,m_strRecordConfigFile);
	WritePrivateProfileString("Record","BackImage",strBackImage,m_strRecordConfigFile);
	CString strTmp;
	strTmp.Format("%d",bShowGrid);
	WritePrivateProfileString("Record","ShowGrid",strTmp,m_strRecordConfigFile);
	strTmp.Format("%d",iGridSize);
	WritePrivateProfileString("Record","GridSize",strTmp,m_strRecordConfigFile);
	strTmp.Format("%d",iClickCount);
	WritePrivateProfileString("Record","ClickCount",strTmp,m_strRecordConfigFile);
}
BOOL CGameRecord::LoadGameRecord(CString & strImage,vector<int> & vIDs,BOOL & bShowGrid,int & iGridSize,int &iClickCount)
{
	CString strTmp;
	GetPrivateProfileString("Record","BackImage","",strTmp.GetBuffer(500),500,m_strRecordConfigFile);
	strTmp.ReleaseBuffer();
	if (strTmp.IsEmpty())
		return FALSE;

	strImage = strTmp;
	GetPrivateProfileString("Record","BlockIds","",strTmp.GetBuffer(500),500,m_strRecordConfigFile);
	strTmp.ReleaseBuffer();
	if (strTmp.IsEmpty())
	{
		strImage = "";
		return FALSE;
	}

	int iStart = 0;
	vIDs.clear();
	CString strTmp2;
	while(1)
	{
		strTmp2 = strTmp.Tokenize(",",iStart);
		if (strTmp2.IsEmpty())
			break;

		vIDs.push_back(atoi(strTmp2));
	}

	GetPrivateProfileString("Record","ShowGrid","0",strTmp.GetBuffer(10),10,m_strRecordConfigFile);
	strTmp.ReleaseBuffer();
	bShowGrid = atoi(strTmp);

	GetPrivateProfileString("Record","GridSize","5",strTmp.GetBuffer(10),10,m_strRecordConfigFile);
	strTmp.ReleaseBuffer();
	iGridSize = atoi(strTmp);

	GetPrivateProfileString("Record","ClickCount","0",strTmp.GetBuffer(10),10,m_strRecordConfigFile);
	strTmp.ReleaseBuffer();
	iClickCount = atoi(strTmp);

	return TRUE;
}