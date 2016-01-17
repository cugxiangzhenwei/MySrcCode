#include "7zInterface.h"

#ifdef _WIN32
HINSTANCE g_hInstance = 0;
#endif

DEFINE_GUID(CLSID_CFormat7z,
			0x23170F69, 0x40C1, 0x278A, 0x10, 0x00, 0x00, 0x01, 0x10, 0x07, 0x00, 0x00);

using namespace NWindows;

#if defined(_WIN64) || defined(UNDER_CE)
extern bool g_IsNT = true;
#else
extern bool g_IsNT = false;
#endif

#define kDllName "7z.dll"

typedef UINT32 (WINAPI * CreateObjectFunc)(
	const GUID *clsID,
	const GUID *interfaceID,
	void **outObject);




class CArchiveOpenCallback:
	public IArchiveOpenCallback,
	public ICryptoGetTextPassword,
	public CMyUnknownImp
{
public:
	MY_UNKNOWN_IMP1(ICryptoGetTextPassword)

		STDMETHOD(SetTotal)(const UInt64 *files, const UInt64 *bytes);
	STDMETHOD(SetCompleted)(const UInt64 *files, const UInt64 *bytes);

	STDMETHOD(CryptoGetTextPassword)(BSTR *password);

	bool PasswordIsDefined;
	UString Password;

	CArchiveOpenCallback() : PasswordIsDefined(false) {}
};


static AString FStringToConsoleString(const FString &s)
{
	return GetOemString(fs2us(s));
}

static FString CmdStringToFString(const char *s)
{
	return us2fs(GetUnicodeString(s));
}

static void PrintString(const UString &s)
{
	printf("%s", (LPCSTR)GetOemString(s));
}

static void PrintString(const AString &s)
{
	printf("%s", (LPCSTR)s);
}

static void PrintNewLine()
{
	PrintString("\n");
}

static void PrintStringLn(const AString &s)
{
	PrintString(s);
	PrintNewLine();
}

static void PrintError(const char *message, const FString &name)
{
	printf("Error: %s", (LPCSTR)message);
	PrintNewLine();
	PrintString(FStringToConsoleString(name));
	PrintNewLine();
}

static void PrintError(const AString &s)
{
	PrintNewLine();
	PrintString(s);
	PrintNewLine();
}

static HRESULT IsArchiveItemProp(IInArchive *archive, UInt32 index, PROPID propID, bool &result)
{
	NCOM::CPropVariant prop;
	RINOK(archive->GetProperty(index, propID, &prop));
	if (prop.vt == VT_BOOL)
		result = VARIANT_BOOLToBool(prop.boolVal);
	else if (prop.vt == VT_EMPTY)
		result = false;
	else
		return E_FAIL;
	return S_OK;
}

static HRESULT IsArchiveItemFolder(IInArchive *archive, UInt32 index, bool &result)
{
	return IsArchiveItemProp(archive, index, kpidIsDir, result);
}

static const wchar_t *kEmptyFileAlias = L"[Content]";


STDMETHODIMP CArchiveOpenCallback::SetTotal(const UInt64 * /* files */, const UInt64 * /* bytes */)
{
	return S_OK;
}

STDMETHODIMP CArchiveOpenCallback::SetCompleted(const UInt64 * /* files */, const UInt64 * /* bytes */)
{
	return S_OK;
}

STDMETHODIMP CArchiveOpenCallback::CryptoGetTextPassword(BSTR *password)
{
	if (!PasswordIsDefined)
	{
		// You can ask real password here from user
		// Password = GetPassword(OutStream);
		// PasswordIsDefined = true;
		PrintError("Password is not defined");
		return E_ABORT;
	}
	return StringToBstr(Password, password);
}

//////////////////////////////////////////////////////////////
// Archive Extracting callback class

static const char *kTestingString    =  "Testing     ";
static const char *kExtractingString =  "Extracting  ";
static const char *kSkippingString   =  "Skipping    ";

static const char *kUnsupportedMethod = "Unsupported Method";
static const char *kCRCFailed = "CRC Failed";
static const char *kDataError = "Data Error";
static const char *kUnknownError = "Unknown Error";






void CArchiveExtractCallback::Init(IInArchive *archiveHandler, const FString &directoryPath)
{
	NumErrors = 0;
	_archiveHandler = archiveHandler;
	_directoryPath = directoryPath;
	NFile::NName::NormalizeDirPathPrefix(_directoryPath);
}

STDMETHODIMP CArchiveExtractCallback::SetTotal(UInt64 /* size */)
{
	return S_OK;
}

STDMETHODIMP CArchiveExtractCallback::SetCompleted(const UInt64 * /* completeValue */)
{
	return S_OK;
}

STDMETHODIMP CArchiveExtractCallback::GetStream(UInt32 index,
												ISequentialOutStream **outStream, Int32 askExtractMode)
{
	*outStream = 0;
	_outFileStream.Release();

	{
		// Get Name
		NCOM::CPropVariant prop;
		RINOK(_archiveHandler->GetProperty(index, kpidPath, &prop));

		UString fullPath;
		if (prop.vt == VT_EMPTY)
			fullPath = kEmptyFileAlias;
		else
		{
			if (prop.vt != VT_BSTR)
				return E_FAIL;
			fullPath = prop.bstrVal;
		}
		_filePath = fullPath;
	}

	if (askExtractMode != NArchive::NExtract::NAskMode::kExtract)
		return S_OK;

	{
		// Get Attrib
		NCOM::CPropVariant prop;
		RINOK(_archiveHandler->GetProperty(index, kpidAttrib, &prop));
		if (prop.vt == VT_EMPTY)
		{
			_processedFileInfo.Attrib = 0;
			_processedFileInfo.AttribDefined = false;
		}
		else
		{
			if (prop.vt != VT_UI4)
				return E_FAIL;
			_processedFileInfo.Attrib = prop.ulVal;
			_processedFileInfo.AttribDefined = true;
		}
	}

	RINOK(IsArchiveItemFolder(_archiveHandler, index, _processedFileInfo.isDir));

	{
		// Get Modified Time
		NCOM::CPropVariant prop;
		RINOK(_archiveHandler->GetProperty(index, kpidMTime, &prop));
		_processedFileInfo.MTimeDefined = false;
		switch(prop.vt)
		{
		case VT_EMPTY:
			// _processedFileInfo.MTime = _utcMTimeDefault;
			break;
		case VT_FILETIME:
			_processedFileInfo.MTime = prop.filetime;
			_processedFileInfo.MTimeDefined = true;
			break;
		default:
			return E_FAIL;
		}

	}
	{
		// Get Size
		NCOM::CPropVariant prop;
		RINOK(_archiveHandler->GetProperty(index, kpidSize, &prop));
		bool newFileSizeDefined = (prop.vt != VT_EMPTY);
		UInt64 newFileSize;
		if (newFileSizeDefined)
			newFileSize = ConvertPropVariantToUInt64(prop);
	}


	{
		// Create folders for file
		int slashPos = _filePath.ReverseFind(WCHAR_PATH_SEPARATOR);
		if (slashPos >= 0)
			NFile::NDirectory::CreateComplexDirectory(_directoryPath + us2fs(_filePath.Left(slashPos)));
	}

	FString fullProcessedPath = _directoryPath + us2fs(_filePath);
	_diskFilePath = fullProcessedPath;

	if (_processedFileInfo.isDir)
	{
		NFile::NDirectory::CreateComplexDirectory(fullProcessedPath);
	}
	else
	{
		NFile::NFind::CFileInfo fi;
		if (fi.Find(fullProcessedPath))
		{
			if (!NFile::NDirectory::DeleteFileAlways(fullProcessedPath))
			{
				PrintError("Can not delete output file", fullProcessedPath);
				return E_ABORT;
			}
		}

		_outFileStreamSpec = new COutFileStream;
		CMyComPtr<ISequentialOutStream> outStreamLoc(_outFileStreamSpec);
		if (!_outFileStreamSpec->Open(fullProcessedPath, CREATE_ALWAYS))
		{
			PrintError("Can not open output file", fullProcessedPath);
			return E_ABORT;
		}
		_outFileStream = outStreamLoc;
		*outStream = outStreamLoc.Detach();
	}
	return S_OK;
}

STDMETHODIMP CArchiveExtractCallback::PrepareOperation(Int32 askExtractMode)
{
	_extractMode = false;
	switch (askExtractMode)
	{
	case NArchive::NExtract::NAskMode::kExtract:  _extractMode = true; break;
	};
	switch (askExtractMode)
	{
	case NArchive::NExtract::NAskMode::kExtract:  PrintString(kExtractingString); break;
	case NArchive::NExtract::NAskMode::kTest:  PrintString(kTestingString); break;
	case NArchive::NExtract::NAskMode::kSkip:  PrintString(kSkippingString); break;
	};
	PrintString(_filePath);
	return S_OK;
}

STDMETHODIMP CArchiveExtractCallback::SetOperationResult(Int32 operationResult)
{
	switch(operationResult)
	{
	case NArchive::NExtract::NOperationResult::kOK:
		break;
	default:
		{
			NumErrors++;
			PrintString("     ");
			switch(operationResult)
			{
			case NArchive::NExtract::NOperationResult::kUnSupportedMethod:
				PrintString(kUnsupportedMethod);
				break;
			case NArchive::NExtract::NOperationResult::kCRCError:
				PrintString(kCRCFailed);
				break;
			case NArchive::NExtract::NOperationResult::kDataError:
				PrintString(kDataError);
				break;
			default:
				PrintString(kUnknownError);
			}
		}
	}

	if (_outFileStream != NULL)
	{
		if (_processedFileInfo.MTimeDefined)
			_outFileStreamSpec->SetMTime(&_processedFileInfo.MTime);
		RINOK(_outFileStreamSpec->Close());
	}
	_outFileStream.Release();
	if (_extractMode && _processedFileInfo.AttribDefined)
		NFile::NDirectory::MySetFileAttributes(_diskFilePath, _processedFileInfo.Attrib);
	PrintNewLine();
	return S_OK;
}


STDMETHODIMP CArchiveExtractCallback::CryptoGetTextPassword(BSTR *password)
{
	if (!PasswordIsDefined)
	{
		// You can ask real password here from user
		// Password = GetPassword(OutStream);
		// PasswordIsDefined = true;
		PrintError("Password is not defined");
		return E_ABORT;
	}
	return StringToBstr(Password, password);
}




STDMETHODIMP CArchiveUpdateCallback::SetTotal(UInt64 /* size */)
{
	return S_OK;
}

STDMETHODIMP CArchiveUpdateCallback::SetCompleted(const UInt64 * /* completeValue */)
{
	return S_OK;
}


STDMETHODIMP CArchiveUpdateCallback::EnumProperties(IEnumSTATPROPSTG ** /* enumerator */)
{
	return E_NOTIMPL;
}

STDMETHODIMP CArchiveUpdateCallback::GetUpdateItemInfo(UInt32 /* index */,
													   Int32 *newData, Int32 *newProperties, UInt32 *indexInArchive)
{
	if (newData != NULL)
		*newData = BoolToInt(true);
	if (newProperties != NULL)
		*newProperties = BoolToInt(true);
	if (indexInArchive != NULL)
		*indexInArchive = (UInt32)-1;
	return S_OK;
}

STDMETHODIMP CArchiveUpdateCallback::GetProperty(UInt32 index, PROPID propID, PROPVARIANT *value)
{
	NCOM::CPropVariant prop;

	if (propID == kpidIsAnti)
	{
		prop = false;
		prop.Detach(value);
		return S_OK;
	}

	{
		const CDirItem &dirItem = (*DirItems)[index];
		switch(propID)
		{
		case kpidPath:  prop = dirItem.Name; break;
		case kpidIsDir:  prop = dirItem.isDir(); break;
		case kpidSize:  prop = dirItem.Size; break;
		case kpidAttrib:  prop = dirItem.Attrib; break;
		case kpidCTime:  prop = dirItem.CTime; break;
		case kpidATime:  prop = dirItem.ATime; break;
		case kpidMTime:  prop = dirItem.MTime; break;
		}
	}
	prop.Detach(value);
	return S_OK;
}

HRESULT CArchiveUpdateCallback::Finilize()
{
	if (m_NeedBeClosed)
	{
		PrintNewLine();
		m_NeedBeClosed = false;
	}
	return S_OK;
}

static void GetStream2(const wchar_t *name)
{
	PrintString("Compressing  ");
	if (name[0] == 0)
		name = kEmptyFileAlias;
	PrintString(name);
}

STDMETHODIMP CArchiveUpdateCallback::GetStream(UInt32 index, ISequentialInStream **inStream)
{
	RINOK(Finilize());

	const CDirItem &dirItem = (*DirItems)[index];
	GetStream2(dirItem.Name);

	if (dirItem.isDir())
		return S_OK;

	{
		CInFileStream *inStreamSpec = new CInFileStream;
		CMyComPtr<ISequentialInStream> inStreamLoc(inStreamSpec);
		FString path = DirPrefix + dirItem.FullPath;
		if (!inStreamSpec->Open(path))
		{
			DWORD sysError = ::GetLastError();
			FailedCodes.Add(sysError);
			FailedFiles.Add(path);
			// if (systemError == ERROR_SHARING_VIOLATION)
			{
				PrintNewLine();
				PrintError("WARNING: can't open file");
				// PrintString(NError::MyFormatMessageW(systemError));
				return S_FALSE;
			}
			// return sysError;
		}
		*inStream = inStreamLoc.Detach();
	}
	return S_OK;
}

STDMETHODIMP CArchiveUpdateCallback::SetOperationResult(Int32 /* operationResult */)
{
	m_NeedBeClosed = true;
	return S_OK;
}

STDMETHODIMP CArchiveUpdateCallback::GetVolumeSize(UInt32 index, UInt64 *size)
{
	if (VolumesSizes.Size() == 0)
		return S_FALSE;
	if (index >= (UInt32)VolumesSizes.Size())
		index = VolumesSizes.Size() - 1;
	*size = VolumesSizes[index];
	return S_OK;
}

STDMETHODIMP CArchiveUpdateCallback::GetVolumeStream(UInt32 index, ISequentialOutStream **volumeStream)
{
	wchar_t temp[16];
	ConvertUInt32ToString(index + 1, temp);
	UString res = temp;
	while (res.Length() < 2)
		res = UString(L'0') + res;
	UString fileName = VolName;
	fileName += L'.';
	fileName += res;
	fileName += VolExt;
	COutFileStream *streamSpec = new COutFileStream;
	CMyComPtr<ISequentialOutStream> streamLoc(streamSpec);
	if (!streamSpec->Create(us2fs(fileName), false))
		return ::GetLastError();
	*volumeStream = streamLoc.Detach();
	return S_OK;
}

STDMETHODIMP CArchiveUpdateCallback::CryptoGetTextPassword2(Int32 *passwordIsDefined, BSTR *password)
{
	if (!PasswordIsDefined)
	{
		if (AskPassword)
		{
			// You can ask real password here from user
			// Password = GetPassword(OutStream);
			// PasswordIsDefined = true;
			PrintError("Password is not defined");
			return E_ABORT;
		}
	}
	*passwordIsDefined = BoolToInt(PasswordIsDefined);
	return StringToBstr(Password, password);
}

void addDirItem(FString Path,CObjectVector<CDirItem> &dirItems,int length)
{
	FString temp = Path + L"\\";
	NFile::NFind::CEnumerator enumerator(Path + L"\\*");
	while(1)
	{
		NFile::NFind::CFileInfo fi;
		bool found;
		enumerator.Next(fi, found);
		if (!found)
			break;
		if(fi.IsDir())
			addDirItem(temp+fi.Name,dirItems,length);
		else
		{
			CDirItem di;
			di.Attrib = fi.Attrib;
			di.Size = fi.Size;
			di.CTime = fi.CTime;
			di.ATime = fi.ATime;
			di.MTime = fi.MTime;
			di.Name = temp+fi.Name;
			di.Name.Delete(0,length);
			di.FullPath = temp+fi.Name;
			dirItems.Add(di);
		}
	}
}

int compress(UString ArchiveName,UStringVector SwitchStrings,UString password,int level,CArchiveUpdateCallback * updateCallbackSpec)
{
	NT_CHECK

		NDLL::CLibrary lib;
	if (!lib.Load(NDLL::GetModuleDirPrefix() + FTEXT(kDllName)))
	{
		PrintError("Can not load 7-zip library");
		return 1;
	}
	CreateObjectFunc createObjectFunc = (CreateObjectFunc)lib.GetProc("CreateObject");
	if (createObjectFunc == 0)
	{
		PrintError("Can not get CreateObject");
		return 1;
	}

	FString archiveName = us2fs(ArchiveName);
	CObjectVector<CDirItem> dirItems;
	int i;
	for (i = 0; i < SwitchStrings.Size(); i++)
	{
		FString name = us2fs(SwitchStrings[i]);
		NFile::NFind::CFileInfo fi;
		if (!fi.Find(name))
		{
			PrintError("Can't find file", name);
			return 1;
		}
		if (!fi.IsDir())
		{
			CDirItem di;
			di.Attrib = fi.Attrib;
			di.Size = fi.Size;
			di.CTime = fi.CTime;
			di.ATime = fi.ATime;
			di.MTime = fi.MTime;
			di.Name = fi.Name;
			di.FullPath = name;
			dirItems.Add(di);
		}
		else
		{
			FStringVector FileList;
			int length = name.ReverseFind(WCHAR_PATH_SEPARATOR);
			addDirItem(name,dirItems,length+1);
		}
	}

	COutFileStream *outFileStreamSpec = new COutFileStream;
	CMyComPtr<IOutStream> outFileStream = outFileStreamSpec;
	if (!outFileStreamSpec->Create(archiveName, false))
	{
		PrintError("can't create archive file");
		return 1;
	}

	CMyComPtr<IOutArchive> outArchive;
	if (createObjectFunc(&CLSID_CFormat7z, &IID_IOutArchive, (void **)&outArchive) != S_OK)
	{
		PrintError("Can not get class object");
		return 1;
	}

	if (level != 999)
	{
		CObjectVector<CProperty> Properties;
		char* lv = new char[2];
		char* ch = new char[4];
		strcpy(ch,"x");
		ConvertUInt32ToString(level,lv);
		strcat(ch,lv);
		UString ulv;
		Char2UString(ulv ,ch);

		CProperty property;
		property.Name = ulv;
		Properties.Add(property);
		delete[] lv;
		delete[] ch;
		SetProperties(outArchive, Properties);
	}

	CMyComPtr<IArchiveUpdateCallback2> updateCallback(updateCallbackSpec);
	updateCallbackSpec->Init(&dirItems);
	if (!password.IsEmpty())
	{
		updateCallbackSpec->PasswordIsDefined = true;
		updateCallbackSpec->Password = password;
	}
	HRESULT result = outArchive->UpdateItems(outFileStream, dirItems.Size(), updateCallback);
	updateCallbackSpec->Finilize();
	return result;

	if (result != S_OK)
	{
		PrintError("Update Error");
		return 1;
	}
	for (i = 0; i < updateCallbackSpec->FailedFiles.Size(); i++)
	{
		PrintNewLine();
		PrintError("Error for file", updateCallbackSpec->FailedFiles[i]);
	}
	if (updateCallbackSpec->FailedFiles.Size() != 0)
		return 1;

	return 0;
}

int showlist(UString ArchiveName,UStringVector &ListVector/*,UString password*/)
{
	NT_CHECK

		NDLL::CLibrary lib;
	if (!lib.Load(NDLL::GetModuleDirPrefix() + FTEXT(kDllName)))
	{
		PrintError("Can not load 7-zip library");
		return 1;
	}
	CreateObjectFunc createObjectFunc = (CreateObjectFunc)lib.GetProc("CreateObject");
	if (createObjectFunc == 0)
	{
		PrintError("Can not get CreateObject");
		return 1;
	}

	FString archiveName = us2fs(ArchiveName);

	CMyComPtr<IInArchive> archive;
	if (createObjectFunc(&CLSID_CFormat7z, &IID_IInArchive, (void **)&archive) != S_OK)
	{
		PrintError("Can not get class object");
		return 1;
	}

	CInFileStream *fileSpec = new CInFileStream;
	CMyComPtr<IInStream> file = fileSpec;

	if (!fileSpec->Open(archiveName))
	{
		PrintError("Can not open archive file", archiveName);
		return 1;
	}

	{
		CArchiveOpenCallback *openCallbackSpec = new CArchiveOpenCallback;
		CMyComPtr<IArchiveOpenCallback> openCallback(openCallbackSpec);
		openCallbackSpec->PasswordIsDefined = false;
		//if (!password.IsEmpty())
		//{
		//	openCallbackSpec->PasswordIsDefined = true;
		//	openCallbackSpec->Password = password;
		//}
		if (archive->Open(file, 0, openCallback) != S_OK)
		{
			PrintError("Can not open file as archive", archiveName);
			return 1;
		}
	}

	UInt32 numItems = 0;
	archive->GetNumberOfItems(&numItems);
	for (UInt32 i = 0; i < numItems; i++)
	{
		{
			// Get uncompressed size of file
			NCOM::CPropVariant prop;
			archive->GetProperty(i, kpidSize, &prop);
			UString s = ConvertPropVariantToString(prop);
			PrintString(s);
			PrintString("  ");
		}
		{
			// Get name of file
			NCOM::CPropVariant prop;
			archive->GetProperty(i, kpidPath, &prop);
			UString s = ConvertPropVariantToString(prop);
			PrintString(s);
			ListVector.Add(s);
		}
		PrintNewLine();
	}
	return 0;
}

int extract(UString ArchiveName,UStringVector FileList,UString FilePath,UString password)
{
	NT_CHECK

		NDLL::CLibrary lib;
	if (!lib.Load(NDLL::GetModuleDirPrefix() + FTEXT(kDllName)))
	{
		PrintError("Can not load 7-zip library");
		return 1;
	}
	CreateObjectFunc createObjectFunc = (CreateObjectFunc)lib.GetProc("CreateObject");
	if (createObjectFunc == 0)
	{
		PrintError("Can not get CreateObject");
		return 1;
	}

	FString archiveName = us2fs(ArchiveName);
	CMyComPtr<IInArchive> archive;
	if (createObjectFunc(&CLSID_CFormat7z, &IID_IInArchive, (void **)&archive) != S_OK)
	{
		PrintError("Can not get class object");
		return 1;
	}

	CInFileStream *fileSpec = new CInFileStream;
	CMyComPtr<IInStream> file = fileSpec;

	if (!fileSpec->Open(archiveName))
	{
		PrintError("Can not open archive file", archiveName);
		return 1;
	}

	{
		CArchiveOpenCallback *openCallbackSpec = new CArchiveOpenCallback;
		CMyComPtr<IArchiveOpenCallback> openCallback(openCallbackSpec);
		openCallbackSpec->PasswordIsDefined = false;
		if (!password.IsEmpty())
		{
			openCallbackSpec->PasswordIsDefined = true;
			openCallbackSpec->Password = password;
		}

		if (archive->Open(file, 0, openCallback) != S_OK)
		{
			PrintError("Can not open file as archive", archiveName);
			return 1;
		}
	}

	UInt32 numItems = 0;
	CRecordVector<UInt32> realIndices;
	archive->GetNumberOfItems(&numItems);
	for (UInt32 i = 0; i < numItems; i++)
	{
		NCOM::CPropVariant prop;
		archive->GetProperty(i, kpidPath, &prop);
		UString s = ConvertPropVariantToString(prop);
		if (FileList.Find(s) != -1)
			realIndices.Add(i);
	}

	CArchiveExtractCallback *extractCallbackSpec = new CArchiveExtractCallback;
	CMyComPtr<IArchiveExtractCallback> extractCallback(extractCallbackSpec);
	extractCallbackSpec->Init(archive, FTEXT("")); // second parameter is output folder path
	extractCallbackSpec->PasswordIsDefined = false;
	extractCallbackSpec->_directoryPath = FilePath;
	if (!password.IsEmpty())
	{
		extractCallbackSpec->PasswordIsDefined = true;
		extractCallbackSpec->Password = password;
	}
	HRESULT result = archive->Extract(&realIndices.Front(), realIndices.Size(), 0, extractCallback);
	if (result != S_OK)
	{
		PrintError("Extract Error");
		return 1;
	}

	return 0;
}

int decompress(UString ArchiveName,UString FilePath,UString password,CArchiveExtractCallback* ExtractCallbackSpec)//函数返回值为int
{
	NT_CHECK

		NDLL::CLibrary lib;
	if (!lib.Load(NDLL::GetModuleDirPrefix() + FTEXT(kDllName)))
	{
		PrintError("Can not load 7-zip library");
		return 1;
	}
	CreateObjectFunc createObjectFunc = (CreateObjectFunc)lib.GetProc("CreateObject");
	if (createObjectFunc == 0)
	{
		PrintError("Can not get CreateObject");
		return 1;
	}

	FString archiveName = us2fs(ArchiveName);
	CMyComPtr<IInArchive> archive;
	if (createObjectFunc(&CLSID_CFormat7z, &IID_IInArchive, (void **)&archive) != S_OK)
	{
		PrintError("Can not get class object");
		return 1;
	}

	CInFileStream *fileSpec = new CInFileStream;
	CMyComPtr<IInStream> file = fileSpec;

	if (!fileSpec->Open(archiveName))
	{
		PrintError("Can not open archive file", archiveName);
		return 1;
	}

	{
		CArchiveOpenCallback *openCallbackSpec = new CArchiveOpenCallback;
		CMyComPtr<IArchiveOpenCallback> openCallback(openCallbackSpec);
		openCallbackSpec->PasswordIsDefined = false;
		if (!password.IsEmpty())
		{
			openCallbackSpec->PasswordIsDefined = true;
			openCallbackSpec->Password = password;
		}

		if (archive->Open(file, 0, openCallback) != S_OK)
		{
			PrintError("Can not open file as archive", archiveName);
			return 1;
		}
	}

	//CArchiveExtractCallback *extractCallbackSpec = new CArchiveExtractCallback;
	CMyComPtr<IArchiveExtractCallback> extractCallback(ExtractCallbackSpec);
	ExtractCallbackSpec->Init(archive, FTEXT("")); // second parameter is output folder path
	ExtractCallbackSpec->PasswordIsDefined = false;
	ExtractCallbackSpec->_directoryPath = FilePath;
	if (!password.IsEmpty())
	{
		ExtractCallbackSpec->PasswordIsDefined = true;
		ExtractCallbackSpec->Password = password;
	}
	HRESULT result = archive->Extract(NULL, (UInt32)(Int32)(-1), false, extractCallback);
	return result;

	if (result != S_OK)
	{
		PrintError("Extract Error");
		return 1;
	}
	return 0;
}

void Char2UString(UString &str ,const char* ch)
{
	int length = (strlen(ch)+1);
	wchar_t * wch = (wchar_t*)malloc(sizeof(wchar_t)*length);
	memset(wch,0,length*sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP,0,ch,strlen(ch),wch,length);
	str = wch;
}