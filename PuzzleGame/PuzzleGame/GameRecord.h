#pragma once
#include "PictueBlock.h"
#include <map>
#include <vector>
using namespace std;

class CGameRecord
{
public:
	CGameRecord(void);
	~CGameRecord(void);
	void WriteGameRecord(const CString & strBackImage,BOOL bShowGrid,int iGridSize,int iClickCount,const map<int,CPictueBlock> & mapBlocks);
	BOOL LoadGameRecord(CString & strImage,vector<int> & vIDs,BOOL & bShowGrid,int & iGridSize,int &iClickCount);
public:
	CString m_strRecordConfigFile;
};

