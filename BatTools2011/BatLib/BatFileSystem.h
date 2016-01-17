
#ifndef BATFILESYSTEM_H
#define BATFILESYSTEM_H

#include <boost/assign.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/xpressive/xpressive_dynamic.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/progress.hpp>
#include <boost/format.hpp>
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

typedef enum{
	/*! 返回0, 正确*/			RE_SUCCESS    =0,	
	/*! 返回1，失败*/			RE_FAILED     =1,		
	/*! 返回2，参数错误*/		RE_PARAMERROR =2,	
	/*! 返回3，用户取消*/		RE_USERCANCEL =3,	
	/*! 返回4，未找到对应项目*/	RE_NOTFOUND	  =4,	
	/*! 返回5，打开文件失败*/	RE_OPENFAILED =5	
} RE_VALUE;
struct FileInfo{
	/*! 扩展名*/
	string ext;
	/*! 文件名*/
	string filename;
	/*! 父路径*/
	string ParentPath;
	/*! 文件大小*/
	UINT64 filesize;
};
/*!导出接口*/
#ifndef BAT_TOOL_API
#define BAT_TOOL_API _declspec(dllexport)
#else	
#define BAT_TOOL_API _declspec(dllimport)
#endif
#ifdef _cplusplus
extern "C"{
#endif
/** 
* @brief 查找所给目录下的所有子目录
* @param dir 目录名
* @param ResultPath 存放查找到的目录列表的vector
* @param bIsRecurrence 是否递归查找，为true时查找子目录的子目录，false时只查找当前目录
* @return 返回错误信息
*/
int BAT_TOOL_API FindSubDirectory(path &dir,vector<path> &ResultPath ,bool bIsRecurrence=true);
/** 
* @brief 查找所给目录下的所有文件不包括目录
* @param dir 目录名
* @param ResultFileList 存放查找到的文件列表的vector
* @param bIsRecurrence 是否递归查找，为true时查找子目录的子目录，false时只查找当前目录
* @return 返回错误信息
*/
int BAT_TOOL_API FindAllFile(path &dir,vector<path> &ResultFileList,bool bIsRecurrence=true);
/** 
* @brief 查找所给目录下的所有文件和目录
* @param dir 目录名
* @param ResultList 存放查找到项目列表的vector
* @param bIsRecurrence 是否递归查找，为true时查找子目录的子目录，false时只查找当前目录
* @return 返回错误信息
*/
int BAT_TOOL_API FindAllFileAndDirectory(path &dir,vector<path> &ResultList,bool bIsRecurrence=true);
/** 
* @brief 查找所给目录下的指定扩展名的文件
* @param dir 目录名
* @param ResultList 存放查找到文件列表的vector
* @param extension 扩展名
* @param bIsRecurrence 是否递归查找，为true时查找子目录的子目录，false时只查找当前目录
* @return 返回错误信息
*/
int BAT_TOOL_API FindExtenFile(path &dir,vector<path> &ResultList,string extension=".*",bool bIsRecurrence=true );
/**
* @brief 在给定文件中添加指定的字符串
* @param filename  文件名
* @param strAppend 添加的字符串
* @return 返回出错信息
*/
int BAT_TOOL_API AppendStrToFile(const char* filename,string strAppend);
/** 
* @brief 复制文件到给定的目录
* @param theFile 要复制的文件的全路径
* @param vpath 目标目录列表
* @return 返回出错信息
*/
int  BAT_TOOL_API CopyFileToDirectory(path &theFile,vector<path>& vpath,bool bOverWritten=false);
/** 
* @brief 获取给定文件的信息
* @param pathFile 给定文件的全路径
* @param strInfo 文件信息
* @return 返回出错信息
*/
int  BAT_TOOL_API GetFileInfo(path & pathFile,FileInfo &strInfo);
/**
* @brief 删除某个目录下指定名字的对象
* @param dir 目录名
* @param theName 指定的名字
* @param IsFile 删除标识,true则删除文件，false 删除目录
* @param bIsRecurrence 为true则递归，否则不递归
* @return 返回出错信息
*/
int BAT_TOOL_API DelFile(path &dir,string theName,bool IsFile,bool bIsRecurrence=true);
/** 
* @brief 查找目录下指定格式的文件
* @param dir 目录名
* @param filename 要查找的文件名
* @param v 返回的路径vector
* @return 无返回值
*/
void BAT_TOOL_API find_file(const path &dir,const string &filename,vector<path>&v);
/**
* @brief 批量重命名
* @param vInpath 待命名的文件列表
* @param strPre 名字前缀
* @return 返回出错信息
*/
int  BAT_TOOL_API BatReName(vector<path> vInpath,string strPre);
#ifdef _cplusplus
}
#endif
#endif