/* Copyright (C) 2011 阙荣文
 *
 * 这是一个开源免费软件,您可以自由的修改和发布.
 * 禁止用作商业用途.
 *
 * 联系原作者: querw@sina.com 
*/

#include "StdAfx.h"
#include "Logger.h"
#include <fstream>
#include <assert.h>

using namespace slogger;
#define TIME_BUF_SZIE 128

//////////////////////////////////////////////////

// 调试器输出
class Win32DebugAppender : public Appender
{
public:
	int append(const tstring &logMsg);
	bool open() { return true; }
	void close() {}
};

// 控制台输出
class ConsoleAppender : public Appender
{
public:
	tostream& _os;

	ConsoleAppender(tostream &os);
	int append(const tstring &logMsg);
	bool open() { return true; }
	void close() {}
};

// Windows系统 - 窗口输出
#ifdef _WIN32
class HwndAppender : public Appender
{
public:
	HWND _hwnd;
	unsigned int _msgId;

	HwndAppender(HWND hWnd, unsigned int msgId);
	int append(const tstring &logMsg);
	bool open() { return _hwnd != NULL; }
	void close() {}
};


HwndAppender::HwndAppender(HWND hWnd, unsigned int msgId)
	: _hwnd(hWnd), _msgId(msgId)
{
}

/*
问题:
如果在A窗口的窗口函数中调用了日志输出函数,就会死锁.
比如,在A窗口的OnButton1Click()中调用了日志输出函数,那么在日志输出函数中会调用下面这个SendMessage()函数,而SendMessage()又要等待
窗口过程处理完毕,而此时A窗口的处理例程有阻塞在OnButton1Click()中,所以会死锁.
但是作为日志类,应该考虑所有这样的情况,如何解决呢?

1. 使用PostMessage可以解决,但是字符串的内存如何处理?
2. 另外建立一个线程,使用消息队列.

问题的根源:
SendMessage 是一个比较特别的函数.
前提:线程A中创建了窗口W
如果在线程A中向W用SendMessage函数发送了一个消息,则直接调用消息处理函数,不进入线程A的消息队列,这是所谓的"不入队"的情况.
所以,在同线程中的窗口函数中调用SendMessage并会导致死锁.
如果在线程B中向W用SendMessage发送消息,则会把消息放入线程A的消息队列,并等待这个消息被处理完成后才返回.所谓的"入队"情况.
*/
int HwndAppender::append(const tstring &logMsg)
{
	int len = logMsg.size();
	tchar *msg_ptr = new tchar[len + 1];
	assert(msg_ptr);
	_tcscpy(msg_ptr, logMsg.c_str());

	//SendMessage(_hwnd, _msgId, len, reinterpret_cast<LPARAM>(msg_ptr));
	//delete[] msg_ptr;

	PostMessage(_hwnd, _msgId, len, reinterpret_cast<LPARAM>(msg_ptr));
	return 0;
}

#endif

// 文件输出
class FileAppender : public Appender
{
public:
	std::locale _loc;
	tstring _fileName;
	tofstream _fs;
	unsigned long _maxSize;
	bool _multiFiles;
	unsigned long _curSize;
	int _fileIndex;

	FileAppender(const tstring &fileName, unsigned long maxSize, bool multiFile);
	~FileAppender();
	int append(const tstring &logMsg);
	bool open();
	void close();
};

int Win32DebugAppender::append(const tstring &logMsg)
{
	OutputDebugString(logMsg.c_str());
	return 0;
}

ConsoleAppender::ConsoleAppender(tostream &os)
: _os(os)
{
}

int ConsoleAppender::append(const tstring &logMsg)
{
	_os << logMsg << std::flush;
	return 0;
}

FileAppender::FileAppender(const tstring &fileName, unsigned long maxSize, bool multiFiles)
: _fileName(fileName), _loc(""), _maxSize(maxSize), _multiFiles(multiFiles), _curSize(0), _fileIndex(0)
{
	//if(_fs.is_open())
	//{
	//	_fs.imbue(_loc);
	//	_fs.seekp(0, std::ios_base::end);

	//	//std::ios::pos_type pos = _fs.tellp();
	//	//TRACE("log file postion:%d\r\n", static_cast<size_t>(pos));
	//}
}

bool FileAppender::open()
{
	// wofstream 输出时,接受wchar_t字符串, 输出时根据环境把宽字符串转化后再写文件. 
	// 如果没有设置, 那么是C现场, 无法输出中文.
	// 所以必须 imbue 当前系统的环境.

	_fs.open(_fileName.c_str(), std::ios::app | std::ios::out);
	if(_fs.is_open())
	{
		_fs.imbue(_loc);
		_fs.seekp(0, std::ios_base::end);
		_curSize = static_cast<unsigned long>(_fs.tellp());
		return true;
	}
	else
	{
		return false;
	}
}

void FileAppender::close()
{
	if(_fs.is_open()) _fs.close();
}

FileAppender::~FileAppender()
{
	close();
}

int FileAppender::append(const tstring &logMsg)
{
	if(_fs.is_open())
	{
		// need to create a new file?
		if( _multiFiles && _curSize > _maxSize )
		{
			// close current file
			_fs.close();
			ASSERT(!_fs.is_open());

			// make a new log file name: oldfile{time}_index.xx, eg. "mylog_20101229_235959_1.txt"
			__time64_t t;
			_time64( &t );
			struct tm *tt = _localtime64(&t);
			tchar tmString[TIME_BUF_SZIE + 1] = {0};
			_stprintf(tmString, _T("_%04d%02d%02d_%02d%02d%02d_%d"), tt->tm_year + 1900, tt->tm_mon + 1, tt->tm_mday, tt->tm_hour, tt->tm_min, tt->tm_sec, ++_fileIndex);
			//tstringstream tos;
			//tos << _T("_") << tt->tm_year + 1900 << tt->tm_mon << tt->tm_mday << _T("_") << tt->tm_hour << tt->tm_min << tt->tm_sec;

			tstring newFileName(_fileName);
			tstring::size_type indexDot = _fileName.find_last_of(_T('.'));
			if(tstring::npos == indexDot)
			{
				//newFileName += tos.str();
				newFileName += tmString;
			}
			else
			{
				// newFileName.insert(indexDot, tos.str());
				newFileName.insert(indexDot, tmString);
			}

			// open new log file
			_fs.open(newFileName.c_str(), std::ios::out);
			if(_fs.is_open())
			{
				_fs.imbue(_loc);
				_curSize = 0;
			}
			else
			{
				ASSERT(0);
			}
		}

		// write log message to file
		if(_fs.is_open()) 
		{
			_fs << logMsg << std::flush;
			_curSize = static_cast<unsigned long>(_fs.tellp());
		}
	}
	return 0;
}

Logger::Logger()
: _ll(ll_none), _timeFmt(_T("")), _mt(false), _isOpen(false)
{
}

Logger::~Logger(void)
{
	close();
}

tstringstream& Logger::operator << (LogLevel ll)
{
	_lock();

	_llTmp = ll;
	return _buf;
}

Logger& slogger::operator << (tostream &ss, Logger &logger)
{
	logger._flush();

	logger._unlock();
	return logger;
}

int Logger::addDebugAppender()
{
	Win32DebugAppender *appender = new Win32DebugAppender();
	if(!appender->open())
	{
		delete appender;
		return 0;
	}
	else
	{
		_lock();
		_appenders.push_back(appender);
		_unlock();

		return reinterpret_cast<int>(appender);
	}
}

int Logger::addFileAppender(const tstring &fileName, unsigned long maxBytes, bool multiFile)
{
	FileAppender *appender = new FileAppender(fileName, maxBytes, multiFile);
	if(appender->open())
	{
		_lock();
		_appenders.push_back(appender);
		_unlock();

		return reinterpret_cast<int>(appender);
	}
	else
	{
		delete appender;
		return 0;
	}
}

int Logger::addHwndAppender(HWND hWnd, unsigned int msgId)
{
	HwndAppender *appender = new HwndAppender(hWnd, msgId);
	if(!appender->open())
	{
		delete appender;
		return 0;
	}
	else
	{

		_lock();
		_appenders.push_back(appender);
		_unlock();

		return reinterpret_cast<int>(appender);
	}
}

int Logger::addConsoleAppender(tostream &os)
{
	ConsoleAppender *appender = new ConsoleAppender(os);
	if(!appender->open())
	{
		delete appender;
		return 0;
	}
	else
	{

		_lock();
		_appenders.push_back(appender);
		_unlock();

		return reinterpret_cast<int>(appender);
	}
}

bool Logger::removeAppender(int appender)
{
	bool ret = false;
	_lock();
	for(AppenderArr::iterator iter = _appenders.begin(); iter != _appenders.end(); ++iter)
	{
		if( appender == reinterpret_cast<int>(*iter) )
		{
			(*iter)->close();
			delete (*iter);
			_appenders.erase(iter);
			ret = true;
			break;
		}
	}
	_unlock();
	return ret;
}

LogLevel Logger::setLogLevel(LogLevel ll)
{
	LogLevel tmp = _ll;
	_lock();
	_ll = ll;
	_unlock();
	return tmp;
}

void Logger::_lock()
{
	if(_mt) 
	{
#ifdef _WIN32
		EnterCriticalSection(&_locker);
#else
		pthread_mutex_lock(*_locker);
#endif
	}
}

void Logger::_unlock()
{
	if(_mt) 
	{
#ifdef _WIN32
		LeaveCriticalSection(&_locker);
#else
		pthread_mutex_unlock(&_locker);
#endif
	}
}

int Logger::_flush()
{
	int ret = 0;
	if(_llTmp <= _ll)
	{
		ret = _doLog(_buf.str().c_str());
	}
	_llTmp = _ll;
	_buf.str(_T(""));
	return ret;
}

int Logger::_doLog(const tchar *msg)
{
	// get time string
	tchar tmString[TIME_BUF_SZIE + 1] = {0};
	__time64_t t;
	_time64( &t );
	_tcsftime(tmString, TIME_BUF_SZIE, _timeFmt.c_str(), _localtime64( &t));

	// format time and log message
	// 如果要输出更加复杂的格式, 可以为每个 appender 定义一个  layout .
	tstringstream o;
	o << tmString << _T(" - ") << msg;

	// call all appenders to write log
	for(AppenderArr::iterator iter = _appenders.begin(); iter != _appenders.end(); ++iter)
	{
		(*iter)->append(o.str());
	}
	return 0;
}

bool Logger::open(bool mt, LogLevel ll /*= ll_all*/, const tstring &timeFmt /*= _T("%m/%d/%Y-%H:%M:%S")*/)
{
	if(isOpen()) return false;

	_mt = mt;
	_ll = ll;
	_timeFmt = timeFmt;

	// initialize sync object
	if(_mt) 
	{
#ifdef _WIN32
		InitializeCriticalSection(&_locker);
#else
		pthread_mutex_init(&_locker, NULL);
#endif
	}

	_isOpen = true;
	return true;
}

bool Logger::close()
{
	if(!isOpen()) return false;

	_lock();
	for(AppenderArr::iterator iter = _appenders.begin(); iter != _appenders.end(); ++iter)
	{
		(*iter)->close();
		delete (*iter);
	}
	_appenders.clear();
	_unlock();

	if(_mt)
	{
#ifdef _WIN32
		DeleteCriticalSection(&_locker);
#else
		pthread_mutex_destroy(&_locker);
#endif
	}

	_mt = false;
	_isOpen = false;
	return true;
}

int Logger::log(LogLevel ll, const tchar *fmt, ...)
{
	if( ll <= _ll)
	{
		// calculate required buffer size
		va_list args;
		va_start(args, fmt);
		int count = _vsctprintf(fmt, args);
		va_end(args);

		// alloc memory
		tchar* logBuf = (tchar*) malloc( (count + 1) * sizeof(tchar) );
		if(logBuf == NULL) return 0;

		// output to buffer
		va_start(args, fmt);
		int realCount = _vstprintf(logBuf, fmt, args); ASSERT(count == realCount);
		va_end(args);
		logBuf[realCount] = 0;

		// write to log
		_lock();
		_doLog(logBuf);
		_unlock();

		// free memory
		free(logBuf);
		return realCount;
	}
	else
	{
		return 0;
	}
}