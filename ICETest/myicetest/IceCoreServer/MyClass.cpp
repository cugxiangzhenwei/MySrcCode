#include<Ice/Ice.h>
#include "MyClass.h"
#include <iostream>
using namespace std;
int  MyCore::add(int a,int b,const Ice::Current&)
{
	int r = a + b;
	return r;
}
int  MyCore::sub(int a,int b,const Ice::Current&)
{
	int r = a - b;
	return r;
}
void  MyCore::PrintMsg(const string & s,const Ice::Current&)
{
	cout << s << endl; 
}