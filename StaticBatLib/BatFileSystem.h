/**
*@ Author 向振伟
*@ Brief  该文件中定义了批处理的函数库和导出静态库的接口
*@ Vesion 1.0
*@ Email cugxiangzhenwei@sina.cn
*@ Phone 13488751151
*@ Time  2011-10-21
*/
#ifndef BATFILESYSTEM_H
#define BATFILESYSTEM_H

#include <boost\assign.hpp>
#include <boost\filesystem.hpp>
#include <boost\lexical_cast.hpp>
#include <boost\algorithm\string.hpp>
#include <boost\xpressive\xpressive_dynamic.hpp>
#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\xml_parser.hpp>
#include <boost\progress.hpp>
#include <vector>
#include <BaseTsd.h >
using namespace boost;
using namespace boost::xpressive;
using namespace boost::filesystem;
using namespace boost::assign;
using namespace std; //用vector必须使用命名空间，否则出错

typedef recursive_directory_iterator rd_iterator;  //迭代子目录的迭代器
typedef directory_iterator       urd_iterator; //只迭代本目录，不深层迭代的迭代器
namespace fs=boost::filesystem;

enum{
	RE_SUCCESS=0, /*!返回0则正确*/
	RE_FAILED,    /*!返回1，失败*/
	RE_PARAMERROR,/*!返回2，参数错误*/
	RE_USERCANCEL, /*!返回3，用户取消*/
	RE_NOTFOUND	,	/*!返回4，未找到对应项目*/
	RE_OPENFAILED  /*!返回5，打开文件失败*/
};
struct FileInfo{
	string ext;
	string filename;
	string ParentPath;
	UINT64 filesize;
};
extern "C"{
/** 
* @brief 查找所给目录下的所有子目录
* @param dir 目录名
* @param ResultPath 存放查找到的目录列表的vector
* @param bIsRecurrence 是否递归查找，为true时查找子目录的子目录，false时只查找当前目录
* @return 返回错误信息
*/
int FindSubDirectory(path &dir,vector<path> &ResultPath ,bool bIsRecurrence=true);
/** 
* @brief 查找所给目录下的所有文件不包括目录
* @param dir 目录名
* @param ResultFileList 存放查找到的文件列表的vector
* @param bIsRecurrence 是否递归查找，为true时查找子目录的子目录，false时只查找当前目录
* @param 返回错误信息
*/
int FindAllFile(path &dir,vector<path> &ResultFileList,bool bIsRecurrence=true);
/** 
* @brief 查找所给目录下的所有文件和目录
* @param dir 目录名
* @param ResultList 存放查找到项目列表的vector
* @param bIsRecurrence 是否递归查找，为true时查找子目录的子目录，false时只查找当前目录
* @param 返回错误信息
*/
int FindAllFileAndDirectory(path &dir,vector<path> &ResultList,bool bIsRecurrence=true);
/** 
* @brief 查找所给目录下的指定扩展名的文件
* @param dir 目录名
* @param ResultList 存放查找到文件列表的vector
* @param extension 扩展名
* @param bIsRecurrence 是否递归查找，为true时查找子目录的子目录，false时只查找当前目录
* @param 返回错误信息
*/
int FindExtenFile(path &dir,vector<path> &ResultList,string extension=".*",bool bIsRecurrence=true );
/**
* @brief 在给定文件中添加指定的字符串
* @param filename  文件名
* @param strAppend 添加的字符串
* @return 返回出错信息
*/
int AppendStrToFile(const char* filename,string strAppend);
/** 
* @brief 复制文件到给定的目录
* @param theFile 要复制的文件的全路径
* @param vpath 目标目录列表
* @return 返回出错信息
*/
int CopyFileToDirectory(path &theFile,vector<path>& vpath,bool bOverWritten=false);
/** 
* @brief 获取给定文件的信息
* @param pathFile 给定文件的全路径
* @param strInfo 文件信息
* @return 返回出错信息
*/
int GetFileInfo(path & pathFile,FileInfo &strInfo);
/**
* @brief 删除某个目录下指定名字的对象
* @param dir 目录名
* @param theName 指定的名字
* @param IsFile删除标识,true则删除文件，false 删除目录
* @param bIsRecurrence 为true则递归，否则不递归
* @param 返回出错信息
*/
int DelFile(path &dir,string theName,bool IsFile,bool bIsRecurrence=true);
/** 
* @brief 查找目录下指定格式的文件
* @param dir 目录名
* @param filename　要查找的文件名
* @param v返回的路径vector
* @return 无返回值
*/
void find_file(const path &dir,const string &filename,vector<path>&v);
}
#endif