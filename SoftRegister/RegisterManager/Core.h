#ifndef H_CORE_REG_SOFT
#define H_CORE_REG_SOFT
#include <string>
using namespace std;

#ifndef LICENSE_MGR_DLL_EXPORT
	#define LICENSE_MGR  __declspec(dllimport)
#else
	#define LICENSE_MGR  __declspec( dllexport) 
#endif

 void LICENSE_MGR FreeStrMen(char * pData);

// 获取许可
 char LICENSE_MGR * GetRegsiterKey(const char * pszClientPcCode,const char * pszSoftID,int iValidHourTime = 0);

// 获取硬件ID
 char LICENSE_MGR * GetPcIdStr();

// 验证许可的有效性
extern "C"  bool  LICENSE_MGR IsValidCode(const char * pszClientPcCode,const char * pszSoftID,const char * pszKeyValue,string & strErrMsg);

// 将许可信息写入注册表
extern "C"  bool LICENSE_MGR  WriteLicenseToReg(const char * pszKeyValue,const char * pszSoftID);

// 从注册表获取许可信息，如获取失败，则返回空字符串
 char  LICENSE_MGR * GetRegsiterKeyFromReg(const char * pszSoftID);

#endif //H_CORE_REG_SOFT