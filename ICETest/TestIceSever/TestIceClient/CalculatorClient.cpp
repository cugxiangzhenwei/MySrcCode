#include <Ice/Ice.h>
#include <Ice/Application.h>
#include "Caculator.h"
#include<iostream>

using namespace std;
using namespace MCal;

//#define RC_List "/home/tester/Key_Checking/ATT_RC/RMT_B109P_EU_Set.txt"
//#define RC_Data "/home/tester/Key_Checking/ATT_RC/RMT_B109P_EU_Set.dat"

class MyApp : virtual public Ice::Application
{
public:
    virtual int run(int,char*[]);
};

int MyApp::run(int,char*[])
{
    Ice::ObjectPrx base = communicator()->stringToProxy("Cal:default -p 10000");
    CalPrx calculator = CalPrx::checkedCast(base);
    if(!calculator)
        throw "Invalid proxy!";
        cout<<calculator->add(1,2)<<endl;
       
        cout<<calculator->sub(56,12)<<endl;
   
   
    //Calc->Close();
    return 0;
}

int main(int argc,char* argv[])
{
    MyApp app;

    return app.main(argc,argv);
}