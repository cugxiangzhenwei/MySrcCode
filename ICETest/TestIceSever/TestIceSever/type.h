#ifndef _TYPE_H
#define _TYPE_H

#include "Caculator.h"
using namespace std;
using namespace MCal;
class CalI:public Cal
{
public:
    CalI();
    ~CalI();
    virtual int add(int num1,int num2,const Ice::Current&);
    virtual int sub(int num1,int num2,const Ice::Current&);
private:
    int num1d;
    int num2d;
};

#endif