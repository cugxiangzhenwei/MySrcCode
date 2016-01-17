#include<iostream>
#include<string>
#include<stack>
#include<stdio.h>
using namespace std;

typedef struct SCharacter
{
char m_ch;
int  m_iPosIndex;
}SCharacter;

void TestErrMsg(const SCharacter & sc)
{
const char * tmsg  = "";
const char * tmsg2 = "";
if(sc.m_ch == '(')
{
	tmsg = "×ó¿ÚºÅ";
	tmsg2 = "ÓÒ¿ÚºÅ";
}
else if(sc.m_ch ==')')
{
	tmsg  = "ÓÒ¿ÚºÅ";
	tmsg2 = "×ó¿ÚºÅ";
}
	
	printf("\033[31mÎ»ÖÃ%d´¦%sÎÞÆ¥ÅäµÄ%s\n\033[0m",sc.m_iPosIndex+1,tmsg,tmsg2);
}

void Test2()
{
	cout << "ÇëÊäÈë×Ö·û´®£¬ÒÔÑéÖ¤À¨ºÅµ½Æ¥ÅäÇé¿ö£º\n";
	string str;
	cin >> str;
	int iLen = str.length();
	stack<SCharacter> tStackCirCle; // Ô²À¨ºÅ
	char ch = 0;
	SCharacter chStackTop;
	for(int i=0;i< iLen;i++)
	{
		ch = str[i];
		if(ch=='('|| ch ==')')
		{
			if(tStackCirCle.empty())
			{
				SCharacter sc;
				sc.m_ch = ch;
				sc.m_iPosIndex = i;
				tStackCirCle.push(sc);
			}
			else
			{
				chStackTop = tStackCirCle.top();
				if(chStackTop.m_ch == '(' && ch == ')') // mathced
				{
					printf("Î»ÖÃ%d:%cÆ¥ÅäÎ»ÖÃ%d:%c\n",chStackTop.m_iPosIndex+1,chStackTop.m_ch,i+1,ch);
					tStackCirCle.pop(); // É¾³ýÒ»¸ö±»Æ¥Åäµ½×Ö·û
				}
				else
				{
					SCharacter sc;
					sc.m_ch = ch;
					sc.m_iPosIndex = i;
					tStackCirCle.push(sc);
				}

			}
		}
	}

	if(tStackCirCle.empty())
		cout << "×Ö·û´®ÖÐÔ²À¨ºÅÍêÈ«Æ¥Åä£¡\n";
	else
	{
		cout << "\033[31mÔ²À¨ºÅ²»Æ¥Åä£¡\n\033[0m";
		while(!tStackCirCle.empty())
		{
			chStackTop = tStackCirCle.top();
			TestErrMsg(chStackTop);
			tStackCirCle.pop();
		}
	}
}
void Test()
{
	cout << "ÇëÊäÈë×Ö·û´®£¬ÒÔÑéÖ¤À¨ºÅµ½Æ¥ÅäÇé¿ö£º\n";
	string str;
	cin >> str;
	int iLen = str.length();
	stack<char> tStackCirCle; // Ô²À¨ºÅ
	stack<char> tStackBrace; // »¨À¨ºÅ
	stack<char> tStackSquareBrackets; // ·½À¨ºÅ
	char ch = 0;
	char chStackTop = 0;
	for(int i=0;i< iLen;i++)
	{
		ch = str[i];
		if(ch=='('|| ch ==')')
		{
			if(tStackCirCle.empty())
				tStackCirCle.push(ch);
			else
			{
				chStackTop = tStackCirCle.top();
				if(chStackTop == '(' && ch == ')') // mathced
					tStackCirCle.pop(); // É¾³ýÒ»¸ö±»Æ¥Åäµ½×Ö·û
				else
					tStackCirCle.push(ch);

			}
		}
		else if(ch=='{'|| ch =='}')
		{
			if(tStackBrace.empty())
				tStackBrace.push(ch);
			else
			{
				chStackTop = tStackBrace.top();
				if(chStackTop == '{' && ch == '}') // mathced
					tStackBrace.pop(); // É¾³ýÒ»¸ö±»Æ¥Åäµ½×Ö·û
				else
					tStackBrace.push(ch);

			}
		}
		else if(ch=='['|| ch ==']')
		{
			if(tStackSquareBrackets.empty())
				tStackSquareBrackets.push(ch);
			else
			{
				chStackTop = tStackSquareBrackets.top();
				if(chStackTop == '[' && ch == ']') // mathced
					tStackSquareBrackets.pop(); // É¾³ýÒ»¸ö±»Æ¥Åäµ½×Ö·û
				else
					tStackSquareBrackets.push(ch);

			}
		}
	}

	if(tStackCirCle.empty())
		cout << "×Ö·û´®ÖÐÔ²À¨ºÅÍêÈ«Æ¥Åä£¡\n";
	else
		cout << "\033[31mÔ²À¨ºÅ²»Æ¥Åä£¡\n\033[0m";

	if(tStackBrace.empty())
		cout << "×Ö·û´®ÖÐ»¨À¨ºÅÍêÈ«Æ¥Åä£¡\n";
	else
		cout << "\033[31m»¨À¨ºÅ²»Æ¥Åä£¡\n\033[0m";

	if(tStackSquareBrackets.empty())
		cout << "×Ö·û´®ÖÐ·½À¨ºÅÍêÈ«Æ¥Åä£¡\n";
	else
		cout << "\033[31m·½À¨ºÅ²»Æ¥Åä£¡\n\033[0m";

}
int main(int argc,char *argv[])
{
 
while(1)
{
Test2();
}
 return 0;
}
