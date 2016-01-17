#ifndef PRODUCTSTRUCT_H 
#define PRODUCTSTRUCT_H

#ifndef STREQUAL
#define STREQUAL(a,b) (_tcsicmp(a,b)==0)
#endif

typedef struct
{
	tString m_StrExeName;
	tString m_StrTitle;
	tString m_StrArguments;
	string  m_StrToolTip;
	string  m_strLogoPng;
	string  m_strURL;
	bool    m_bValid;
}ProductStruct;

class CProductGroup
{
public:
	CProductGroup(){}
	~CProductGroup(){}
	ProductStruct GetGroupItem(int iIndex){return m_VGroupItem[iIndex];}
	int GetGroupItemCount(){return m_VGroupItem.size();}
	tString GetGroupTitle(){return m_strGroupTitle;}
	void SetGroupTitle(const char *  psztitle){m_strGroupTitle = psztitle;}
	void AddProduct(const ProductStruct &product){ m_VGroupItem.push_back(product);}
private:
	vector<ProductStruct> m_VGroupItem;
	tString m_strGroupTitle;
};

#endif //PRODUCTSTRUCT_H