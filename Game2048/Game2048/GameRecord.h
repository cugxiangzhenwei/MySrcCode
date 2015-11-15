#pragma once
class CGameNumBlock;
class CGameRecord
{
public:
	CGameRecord(void);
	~CGameRecord(void);
	void WriteGameRecord(CGameNumBlock * pBlockArry,int iSize,int iSumScore);
	BOOL LoadGameRecord(CGameNumBlock * pBlockArry,int iSize,int &iSumScore);
public:
	int m_iShowNumArry[16];
	int m_iSumScore;
	CString m_strRecordConfigFile;
};

