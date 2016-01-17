#include <iostream>
using namespace std;
typedef int BOOL;
#define FALSE 0
#define TRUE  1

typedef struct _TagNode
{
	_TagNode()
	{
		m_ch = 0;
		m_pNext = NULL;
		m_pPrev = NULL;
	}
	char m_ch;
	_TagNode * m_pNext;
	_TagNode * m_pPrev;
}CharNode;

void PrintfCircleList(CharNode * pNodeFirst)
{
	CharNode * pCurNode = pNodeFirst;
	while(1)
	{
		printf("%c ",pCurNode->m_ch);
		if (pCurNode->m_pNext == pNodeFirst)
			break;

		pCurNode = pCurNode->m_pNext; //往后遍历 
	}

}
BOOL DeleteCircleNode()
{
	const char * strFile = "E:\\data.txt";
	FILE * pFile = NULL;
	fopen_s(&pFile,strFile,"r+");
	if (pFile==NULL)
	{
		printf("读取参数文件[%s]失败!",strFile);
		return FALSE;
	}

	CharNode *pPreNode   = NULL;
	CharNode *pNextNode  = NULL;
	CharNode *pNodeCur   = NULL; 
	CharNode *pNodeFirst = NULL;
	char * pChar = NULL;
	char szBuf[4] ={0};
	while(pChar=fgets(szBuf,2,pFile))
	{
		if (szBuf[0]==' ')
			continue;

		pNodeCur = new CharNode;
		if (pNodeFirst==NULL)
			pNodeFirst = pNodeCur;
		if (pPreNode) 
			pPreNode->m_pNext = pNodeCur;

		pNodeCur->m_ch = szBuf[0];
		pNodeCur->m_pPrev = pPreNode;
		pNodeCur->m_pNext = NULL;
		pPreNode = pNodeCur;
	}
	fclose(pFile);

	fopen_s(&pFile,"E:\\out_data.txt","a+");
	if (pFile==NULL)
	{
		printf("创建输出文件失败!\n");
		return FALSE;
	}

	pNodeCur->m_pNext = pNodeFirst;  //把最后一个节点的下一个节点指向首节点，形成环状
	pNodeFirst->m_pPrev = pNodeCur;  //把首节点的前一个节点指向尾节点，形成环状

	pNextNode = NULL;
	pPreNode  = NULL;

	//输出原始列表
	printf("原始节点列表为：\n");
	PrintfCircleList(pNodeFirst);
	printf("\n");

	pNodeCur = pNodeFirst; // 从节点A开始
	int iStart = 0; //从A开始的第几个节点
	int iDis  = 0; //偏移步长
	cout << "请输入删除的起始位置和间隔：\n";
	cin >>iStart>>iDis;
	fprintf(pFile,"起始位置：%d , 间隔 %d\n",iStart,iDis);

	iStart  = iStart - 1;
	iDis    = iDis - 1;

	if (iStart>=0) // 起始位置大于等于0
	{
		for (int i=0;i<iStart;i++)
			pNodeCur = pNodeCur->m_pNext;
	}
	else // 起始位置小于0
	{
		for (int i=0;i<iStart;i++)
			pNodeCur = pNodeCur->m_pPrev;
	}

	printf("开始删除节点:\n");
	while(pNodeCur)
	{
		if (iDis>=0) // 间隔大于等于0
		{
			for (int i=0;i<iDis;i++)
				pNodeCur = pNodeCur->m_pNext;
		}
		else // 间隔小于0
		{
			for (int i=0;i<iDis;i++)
				pNodeCur = pNodeCur->m_pPrev;
		}

		pNextNode = pNodeCur->m_pNext;
		if (pNextNode == pNodeCur)
		{
			printf("%c 移除\n",pNodeCur->m_ch);
			fprintf(pFile,"%c\n\n",pNodeCur->m_ch);
			delete pNodeCur;
			break;
		}
		pNodeCur->m_pPrev->m_pNext = pNextNode;
		pNextNode->m_pPrev = pNodeCur->m_pPrev;
		if (pNodeCur==pNodeFirst) // 首节点移除了，则首节点指向下一节点
			pNodeFirst = pNextNode;

		printf("%c 移除\n",pNodeCur->m_ch);
		fprintf(pFile,"%c ",pNodeCur->m_ch);

		delete pNodeCur;
		pNodeCur = pNextNode; //往后遍历 
		PrintfCircleList(pNodeFirst);
		printf("\n");
	}

	return TRUE;
}