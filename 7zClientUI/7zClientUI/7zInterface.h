#ifndef _7Z_INTERFACE_H
#define _7Z_INTERFACE_H

#include "stdafx.h"
#include "../7z/Common/IntToString.h"
#include "../7z/Common/MyInitGuid.h"
#include "../7z/Common/StringConvert.h"
#include "../7z/Common/SetProperties.h"

#include "../7z/Windows/DLL.h"
#include "../7z/Windows/FileDir.h"
#include "../7z/Windows/FileFind.h"
#include "../7z/Windows/FileName.h"
#include "../7z/Windows/NtCheck.h"
#include "../7z/Windows/PropVariant.h"
#include "../7z/Windows/PropVariantConversions.h"
#include "../7z/FileStream/FileStreams.h"
#include "../7z/Archive/IArchive.h"
#include "../7z/IPassword.h"


struct CDirItem
{
	UInt64 Size;
	FILETIME CTime;
	FILETIME ATime;
	FILETIME MTime;
	UString Name;
	FString FullPath;
	UInt32 Attrib;

	bool isDir() const { return (Attrib & FILE_ATTRIBUTE_DIRECTORY) != 0 ; }
};


class CArchiveUpdateCallback:
	public IArchiveUpdateCallback2,
	public ICryptoGetTextPassword2,
	public CMyUnknownImp
{
public:
	MY_UNKNOWN_IMP2(IArchiveUpdateCallback2, ICryptoGetTextPassword2)

		// IProgress
		STDMETHOD(SetTotal)(UInt64 size);
	STDMETHOD(SetCompleted)(const UInt64 *completeValue);

	// IUpdateCallback2
	STDMETHOD(EnumProperties)(IEnumSTATPROPSTG **enumerator);
	STDMETHOD(GetUpdateItemInfo)(UInt32 index,
		Int32 *newData, Int32 *newProperties, UInt32 *indexInArchive);
	STDMETHOD(GetProperty)(UInt32 index, PROPID propID, PROPVARIANT *value);
	STDMETHOD(GetStream)(UInt32 index, ISequentialInStream **inStream);
	STDMETHOD(SetOperationResult)(Int32 operationResult);
	STDMETHOD(GetVolumeSize)(UInt32 index, UInt64 *size);
	STDMETHOD(GetVolumeStream)(UInt32 index, ISequentialOutStream **volumeStream);

	STDMETHOD(CryptoGetTextPassword2)(Int32 *passwordIsDefined, BSTR *password);

public:
	CRecordVector<UInt64> VolumesSizes;
	UString VolName;
	UString VolExt;

	FString DirPrefix;
	const CObjectVector<CDirItem> *DirItems;

	bool PasswordIsDefined;
	UString Password;
	bool AskPassword;

	bool m_NeedBeClosed;

	FStringVector FailedFiles;
	CRecordVector<HRESULT> FailedCodes;

	CArchiveUpdateCallback(): PasswordIsDefined(false), AskPassword(false), DirItems(0) {};

	virtual ~CArchiveUpdateCallback() { Finilize(); }
	HRESULT Finilize();

	void Init(const CObjectVector<CDirItem> *dirItems)
	{
		DirItems = dirItems;
		m_NeedBeClosed = false;
		FailedFiles.Clear();
		FailedCodes.Clear();
	}
};


class CArchiveExtractCallback:
	public IArchiveExtractCallback,
	public ICryptoGetTextPassword,
	public CMyUnknownImp
{
public:
	MY_UNKNOWN_IMP1(ICryptoGetTextPassword)

		// IProgress
		STDMETHOD(SetTotal)(UInt64 size);
	STDMETHOD(SetCompleted)(const UInt64 *completeValue);

	// IArchiveExtractCallback
	STDMETHOD(GetStream)(UInt32 index, ISequentialOutStream **outStream, Int32 askExtractMode);
	STDMETHOD(PrepareOperation)(Int32 askExtractMode);
	STDMETHOD(SetOperationResult)(Int32 resultEOperationResult);

	// ICryptoGetTextPassword
	STDMETHOD(CryptoGetTextPassword)(BSTR *aPassword);

private:
	CMyComPtr<IInArchive> _archiveHandler;

	UString _filePath;       // name inside arcvhive
	FString _diskFilePath;   // full path to file on disk
	bool _extractMode;
	struct CProcessedFileInfo
	{
		FILETIME MTime;
		UInt32 Attrib;
		bool isDir;
		bool AttribDefined;
		bool MTimeDefined;
	} _processedFileInfo;

	COutFileStream *_outFileStreamSpec;
	CMyComPtr<ISequentialOutStream> _outFileStream;

public:
	void Init(IInArchive *archiveHandler, const FString &directoryPath);

	UInt64 NumErrors;
	bool PasswordIsDefined;
	UString Password;
	FString _directoryPath;  // Output directory

	CArchiveExtractCallback() : PasswordIsDefined(false) {}
};


/**
* @brief 压缩函数
* @param ArchiveName	压缩包路径
* @param SwitchStrings	压缩文件列表
* @param password	    压缩密码
* @param level	        压缩等级
*    压缩等级            说明
*    0                  不压缩.
*    1                  快速压缩：LZMA 快速算法、32KB 字典大小、HC3 Match finder、BCJ 过滤器。
*    5                  正常压缩：LZMA 标准算法、2 MB 字典大小、BT4 Match finder、单词大小为 32、BCJ 过滤器。
*    7                  最大压缩：LZMA 最大算法、8 MB 字典大小、BT4 Match finder、单词大小为 64、BCJ 过滤器。
*    9                  极限压缩：LZMA 最大算法、32 MB 字典大小、BT4b Match finder、单词大小为 64、BCJ2 过滤器。
* @param pProgress      压缩进度条
* @return 返回值，表示计算过程中出现的各种错误信息
*/
int compress(UString ArchiveName,UStringVector SwitchStrings,UString password = L"",int level = 1,CArchiveUpdateCallback * pProgress = NULL);

/**
* @brief 显示压缩包文件列表
* @param ArchiveName	压缩包路径
* @param ListVector	    压缩文件列表
* @return 返回值，表示计算过程中出现的各种错误信息
*/
int showlist(UString ArchiveName,UStringVector &ListVector/*,UString password = L""*/);

/**
* @brief 提取压缩包内特定文件
* @param ArchiveName	压缩包路径
* @param FileList	    所需提取的文件列表
* @param FilePath	    解压路径
* @param password	    解压密码
* @return 返回值，表示计算过程中出现的各种错误信息
*/
int extract(UString ArchiveName,UStringVector FileList,UString FilePath,UString password = L"");

/**
* @brief 解压压缩包
* @param ArchiveName	压缩包路径
* @param FilePath	    解压路径
* @param password	    解压密码
* @param pProgress      解压缩进度条
* @return 返回值，表示计算过程中出现的各种错误信息
*/
int decompress(UString ArchiveName,UString FilePath,UString password = L"",CArchiveExtractCallback * pProgress = NULL);

/**
* @brief char*转UString
* @param ustr	        转换后UString字符串
* @param ch	            char* 字符串
*/
void Char2UString(UString &ustr ,const char* ch);

#endif