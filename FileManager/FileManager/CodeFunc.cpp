#include "stdafx.h"
#include "CodeFunc.h"
#include<windows.h>
namespace CodeTools
{
	string MB_TO_UTF8(const string & str)
	{
		// convert an MBCS string to widechar   
		int nLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);  

		WCHAR* lpszW = NULL;  
		try  
		{  
			lpszW = new WCHAR[nLen];  
		}  
		catch(std::bad_alloc &/*memExp*/)  
		{  
			return "";  
		}  

		int nRtn = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, lpszW, nLen);  

		if(nRtn != nLen)  
		{  
			delete[] lpszW;  
			return "";  
		}  
		// convert an widechar string to utf8  
		int utf8Len = WideCharToMultiByte(CP_UTF8, 0, lpszW, nLen, NULL, 0, NULL, NULL);  
		if (utf8Len <= 0)  
		{  
			return "";  
		}  

		char* pu8 = new char[utf8Len + 1];

		nRtn = WideCharToMultiByte(CP_UTF8, 0, lpszW, nLen, pu8, utf8Len, NULL, NULL);
		pu8[utf8Len] = '\0';
		delete[] lpszW;  

		if (nRtn != utf8Len)  
		{  
			//pu8.clear();  
			delete pu8;
			return "";  
		}  
		string strOut = pu8;
		delete pu8;
		return strOut;

	}
	string UTF8_TO_MB(const string & str)
	{
		int nLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);  

		WCHAR * lpszW = NULL;  
		try  
		{  
			lpszW = new WCHAR[nLen];  
		}  
		catch(std::bad_alloc &/*memExp*/)  
		{  
			return "";  
		}  

		int nRtn = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, lpszW, nLen);  

		if(nRtn != nLen)  
		{  
			delete[] lpszW;  
			return "";  
		}  

		// convert an widechar string to Multibyte   
		int MBLen = WideCharToMultiByte(CP_ACP, 0, lpszW, nLen, NULL, 0, NULL, NULL);  
		if (MBLen <=0)  
		{  
			return false;  
		}	
		char * pmb = new char[MBLen+1];
		nRtn = WideCharToMultiByte(CP_ACP, 0, lpszW, nLen, pmb, MBLen, NULL, NULL);
		delete[] lpszW;  

		if(nRtn != MBLen)  
		{		
			delete pmb;
			return "";  
		}  

		string strOut = pmb;
		delete pmb;
		return strOut;  
	}
};