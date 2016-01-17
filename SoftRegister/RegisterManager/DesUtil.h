#ifndef DESUTIL_H_
#define DESUTIL_H_

#include <string>

class DesUtil {
private:
	const char *keystr;
	void Base64en(const unsigned char *ming, unsigned char* enstr);
	void Base64de(const unsigned char* mi, unsigned char* destr);
public:
	DesUtil(const char *key);
	std::string Encrypt(const std::string &content);
	std::string Decrypt(const std::string &content);
};

#endif /* DESUTIL_H_ */