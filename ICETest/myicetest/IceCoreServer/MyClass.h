#ifndef MYCLASS_H
#define MYCLASS_H
#include "IceCore.h"
#include <string>
using namespace std;
using namespace  CoreSpace;

class MyCore:public CoreBase
{
public:
	MyCore(){}
	~MyCore(){}
	virtual int  add(int a,int b,const Ice::Current&);
	virtual int  sub(int a,int b,const Ice::Current&);
	virtual void  PrintMsg(const string & s ,const Ice::Current&);
};


#endif //MYCLASS_H