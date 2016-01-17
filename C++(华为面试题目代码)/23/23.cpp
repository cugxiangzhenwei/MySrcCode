#include<stdio.h>
enum NEW_SERVER_ID
{
NSID_SERVER_NULL = 0,
NSID_SERVER254 = 254,
NSID_SERVER255,
NSID_BUTT
};

unsigned char GetNisd(char nsid)
{
return (nsid);
}

int main()
{
int a = GetNisd(NSID_SERVER_NULL);
int b = GetNisd(NSID_SERVER255);
int c = GetNisd(NSID_BUTT);
printf("%d ,%d ,%d\n",a,b,c);
char x = 130;
printf("%d\n",x);
}

