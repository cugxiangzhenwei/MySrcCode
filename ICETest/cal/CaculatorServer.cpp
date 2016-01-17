#include<Ice/Ice.h>
#include<Ice/Application.h>
#include<iostream>

#include"type.h"
using namespace std;

CalI::CalI()
{
   num1d=0;
   num2d=0;
}
CalI::~CalI()
{

}
int CalI::add(int num1, int num2, const Ice::Current &)
{
    int sumv=0;
    sumv=num1+num2;
    return sumv;
}
int CalI::sub(int num1,int num2,const Ice::Current&)
{
 int difv=0;
 if (num1>num2)
 {
     difv=num1-num2;
     return difv;
 }
 else
 {
    cout<<"there is some wrong!"<<endl;
    return 0;
 }
}

class CalApp:virtual public Ice::Application
{
public:
    virtual int run(int,char *[]);
};

int CalApp::run(int,char *[])
{
    //shutdownOnInterrupt();
    Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapterWithEndpoints(
        "Caladapter","default -p 10000");
    Ice::ObjectPtr object = new CalI();
    adapter->add(object,communicator()->stringToIdentity("Cal"));
    adapter->activate();
    communicator()->waitForShutdown();

    if(interrupted())
        cerr << appName() << ":recv signal,shutting down" << endl;

    return 0;
}
int main(int argc,char* argv[])
{
    CalApp app;
    return app.main(argc,argv);
}