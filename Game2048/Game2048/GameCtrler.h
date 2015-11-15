#pragma once
class CGame2048Dlg;

typedef struct _BlockTag
{
	int m_iID;
	int m_iShowNum;
	_BlockTag *m_pNextBlock;
	_BlockTag()
	{
		m_iID = -1;
		m_iShowNum = 0;
		m_pNextBlock = NULL;
	}
}BlockNode;
class CGameCtrler
{
public:
	CGameCtrler(CGame2048Dlg * pMainWnd);
	~CGameCtrler(void);
	void MoveUp();
	void MoveRight();
	void MoveLeft();
	void MoveDown();
	BOOL IsGameOver();
	void MakeNewBlockInEmptyErea(); // 在空白区域产生新的数字
	void ProcessOneColumToDown(int iCulumId);
	void ProcessOneColumToUp(int iCulumId);
	void ProcessOneRowToLeft(int iRowId);
	void ProcessOneRowToRight(int iRowId);
	void ProcessDataListNode(BlockNode * NodeList);
	int GetAddScore() { return m_iCurScore;}
	int GetSumScore() { return m_iSumScore;}
	void SetSumScore(int iSum){ m_iSumScore = iSum;}
	void RestoreStatus();
	BOOL StatusChanged();
	void UpdateFlashTimeCount();
private:
	CGame2048Dlg *m_pMainWnd;
	int m_iSumScore;
	int m_iCurScore;
	int m_iRestoreStatus[16];
	int m_iFlashBlockID;
};

