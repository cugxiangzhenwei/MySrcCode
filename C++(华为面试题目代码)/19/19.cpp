#include<iostream>
using namespace std;
int main(int argc,char*argv[])
{
int a[4]={5,8,9,10};
int *p = a;
cout << (*(p+1)) << " " << (*(p++)) << " "<< (*(p++)) << endl; 
 int val=5;
 cout << val++ << ++val << val-- << --val << endl;
cout << sizeof(int) << endl;
int ax[3][5]={{232,435,768},{3423,64,78,6767,32},{454,4222,5666}};
cout<< *(ax[1]+2)<<endl;
return 0;
}
