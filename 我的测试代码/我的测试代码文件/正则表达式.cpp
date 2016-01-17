#include <boost/xpressive/xpressive_dynamic.hpp>
using namespace boost::xpressive;
/*
*@brief 判断字符串是否是有效的身份证号码
*/
bool IsValidIdentityCardNum(const char *pszString)
{
	sregex_compiler rc;	//正则表达式工厂
	const char * strCardStyle =			  "^\\d{6}(1|2)\\d{3}(0|1)\\d[0-3]\\d{4}(X|\\d)$";
	rc[strCardStyle] = rc.compile(strCardStyle);
	if (regex_match(string(pszString),rc[strCardStyle]))
	{
		printf("%s Is a valid Identity Card Number!\n",pszString);
		return true;
	}

	printf("%s Is not a valid Identity Card Number!\n",pszString);
	return false;
}
/*
*@brief 判断字符串是否是有效的手机号码
*/
bool IsValidPhoneNum(const char *pszString)
{
	sregex_compiler rc;	//正则表达式工厂
	const char * strPhoneNumStyle =	"^1([3]\\d|[4][7]|[5]\\d|[8][2]|[8][9])\\d{8}$";
	rc[strPhoneNumStyle] = rc.compile(strPhoneNumStyle,icase);
	if (regex_match(string(pszString),rc[strPhoneNumStyle]))
	{
		printf("%s Is a valid PhoneNumber!\n",pszString);
		return true;
	}

	printf("%s Is not a valid PhoneNumber!\n",pszString);
	return false;
}
