#pragma once

#ifndef _MEMFILE_HEADER_PROTECT_
#define _MEMFILE_HEADER_PROTECT_

/*
memfile.h

内存缓冲,可以读写.

by 阙荣文 - Que's C++ Studio
2010-12-16

*/

class memfile
{
private:
	memfile(const memfile &other);
	memfile& operator = (const memfile &other);

public:
	memfile(int memInc = 1024, int maxSize = INT_MAX);
	~memfile();

	int write(const void *buf, int len); // 返回写入的字节数
	int puts(const char* buf); // 返回写入的字节数,不含结束符0
	int putc(char ch);
	int seekp(int offset, int origin); // 返回0 表示成功.
	int tellp() const;
	int settle();

	int read(void *buf, int size); // 返回读取字节数
	char getc();
	int getline(char *buf, int size); // 返回读取的字节数,包含换行符
	int seekg(int offset, int origin);
	int tellg() const;

	const void* data() const;
	int size() const;
	int reset();
	bool eof(bool readEof = true) const;

private:
	char *_data;
	int _dataSize;
	int _readPos;
	int _writePos;
	int _fileSize;
	int _maxSize;
	int _memInc;

	int _seek(int &pos, int offset, int origin);
};

#endif