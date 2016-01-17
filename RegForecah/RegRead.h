#ifndef REGREAD_H
#define REGREAD_H

#include <string>
#include <vector>
using namespace  std;

typedef struct _Tag_NewFileTypeReg
{
	string m_strExt;
	string m_strKeyPath;
}SNewFileTypeReg;

typedef struct 
{
	string m_strExt; // ºó×ºÃû
	string m_strDesrciption; //ÃèÊö·û
}NewType;

bool GetAllNewFileTypeReg(vector<SNewFileTypeReg> & vNewType);

void GetAllDesktopRightMenuNewItems(vector<NewType> &vAllItems);
bool CreateNewItem(string strItemType,string strDir);

string GetWindowVesion();
#endif //REGREAD_H