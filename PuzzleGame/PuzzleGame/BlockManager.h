#pragma once
#include "PictueBlock.h"
#include "GameCtrler.h"
#include <map>
using namespace std;
class CBlockManager
{
public:
	friend CGameCtrler;

	static CBlockManager * GetInstance();
	~CBlockManager(void);
	BOOL IntilizeBlock(const CRect & rectPictureWnd,int iRow,int iCol);
	BOOL IntilizeBlockWithRecord(const CRect & rectPictureWnd,int iRow,int iCol,const vector<int> & vIDs);
	BOOL LoadPuzzlePicture(const WCHAR * pszImg,CDC &dc);
	BOOL GetBlockPicture(int iID,CDC & dcBlock);
	void DrawAllBlock(CDC & dc);
	void DrawGridLine(CDC & dc);
	void Clear();
	const map<int,CPictueBlock> & GetBlocks(){return m_BlockMap;}
protected:
	CBlockManager(void);
	void SwapBlock(const int &iID1,const int &iID2);
private:
	class GabageCollector // 定义私有内嵌类
	{
	public:
		GabageCollector()
		{
			OutputDebugString("垃圾回收器CUploadManager::GabageCollector构建...");
		}
		~GabageCollector()
		{
			if (m_pManager)
			{
				delete m_pManager;
				m_pManager = 0;
				OutputDebugString("垃圾回收器CUploadManager::GabageCollector执行单例对象清理工作结束...");
			}
		}

	};
	static CBlockManager * m_pManager;
	static GabageCollector m_gc; // 定义垃圾回收器的静态对象，程序结束后，系统会调用该对象的析构函数，以执行对单例对象的析构
	CDC   m_MemPictureAll;
	CRect  m_WndPictureRect;
	map<int,CPictueBlock> m_BlockMap;
	int m_iRowBlockCount;
	int m_iColBlockCount;
};

