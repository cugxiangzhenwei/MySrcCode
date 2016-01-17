#include "StdAfx.h"
#include "memfile.h"

memfile::memfile(int memInc, int maxSize)
: _memInc(memInc), _maxSize(maxSize)
{
	_data = NULL;
	_dataSize = 0;
	_readPos = 0;
	_writePos = 0;
	_fileSize = 0;

	ASSERT(_memInc <= _maxSize);
	if( _memInc > _maxSize ) _memInc = _maxSize;
}

memfile::~memfile()
{
	reset();
}

int memfile::reset()
{
	if(_data != NULL) delete[] _data;

	_data = NULL;
	_dataSize = 0;
	_readPos = 0;
	_writePos = 0;
	_fileSize = 0;

	return 0;
}

int memfile::putc(char ch)
{
	return write(&ch, 1);
}

int memfile::puts(const char *buf)
{
	return write((const void*)buf, strlen(buf));
}

int memfile::write(const void *buf, int len)
{
	// 确保在不大于最大内存限制的情况下拥有足够的空间写入数据.
	int memLeft = _dataSize - _writePos;
	if( memLeft >= len)
	{
	}
	else
	{
		// 计算新增的大小
		int incSize = 0;
		int requiredIncSize = len - memLeft; // 请求增加的空间
		int maxIncSize = _maxSize - _dataSize; // 最多可以增加的空间
		int minIncSize = _memInc < maxIncSize ? _memInc: maxIncSize; // 一次最小增加的空间.

		if( requiredIncSize > maxIncSize )
		{
			incSize = maxIncSize;
		}
		else if( requiredIncSize > minIncSize ) 
		{
			incSize = requiredIncSize;
		}
		else
		{
			incSize = minIncSize;
		}

		// 分配新的空间
		if( incSize > 0 )
		{
			char *tmp = _data;
			_data = new char[_dataSize + incSize];
			if(_data == NULL)
			{
				ASSERT(!"can't alloc more memory.");
			}
			else
			{
				if(tmp != NULL)
				{
					memcpy(_data, tmp, _dataSize);
					delete[] tmp;
				}
				memset(_data + _dataSize, 0, incSize);
				_dataSize += incSize;
			}
		}
	}
	
	// 计算最多可以写入多少个字节
	int writeSize = _dataSize - _writePos > len ? len : _dataSize - _writePos;

	// 写入数据
	if(writeSize > 0)
	{
		memcpy( ((char*)_data) + _writePos, buf, writeSize );
		_writePos += writeSize;

		// 最大到达过的位置为文件大小. 通过 write() 增大, 通过 seekp() 和 settle() 修改
		if(_writePos > _fileSize)
		{
			settle();
		}
	}

	return writeSize;
}

int memfile::settle()
{
	return _fileSize = _writePos;
}

int memfile::tellp() const
{
	return _writePos;
}

char memfile::getc()
{
	char ch = 0;
	read(&ch, 1);
	return ch;
}

int memfile::getline(char *buf, int size)
{
	int readed = 0;
	char ch = 0;
	while(readed < size && (ch = getc()) != '\n') buf[readed++] = ch;
	return readed;
}

int memfile::read(void *buf, int size)
{
	int memLeft = _fileSize - _readPos;
	if(size <= 0 || buf == NULL || memLeft <= 0) return 0;

	if( memLeft > size )
	{
		memcpy(buf, _data + _readPos, size);
		_readPos += size;
		return size;
	}
	else
	{
		memcpy(buf, _data + _readPos, memLeft);
		_readPos += memLeft;
		return memLeft;
	}
}

int memfile::_seek(int &pos, int offset, int origin)
{
	if( SEEK_CUR == origin )
	{
		pos += offset;
	}
	else if( SEEK_END == origin )
	{
		pos = _fileSize - offset;
	}
	else // SEEK_SET
	{
		pos = offset;
	}

	if( pos < 0)
	{
		pos = 0;
		return 1;
	}
	if( pos > _fileSize)
	{
		pos = _fileSize;
		return 2;
	}
	return 0;
}

int memfile::seekg(int offset, int origin)
{
	return _seek(_readPos, offset, origin);
}

int memfile::seekp(int offset, int origin)
{
	return _seek(_writePos, offset, origin);
}

int memfile::tellg() const
{
	return _readPos;
}

const void* memfile::data() const
{
	return (const void*)_data;
}

int memfile::size() const
{
	return _fileSize;
}

bool memfile::eof(bool readEof) const
{
	if(readEof) return _readPos >= _fileSize;
	else return _writePos >= _fileSize;
}