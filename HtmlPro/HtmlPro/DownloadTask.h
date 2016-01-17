#include"TaskBase.h" 

extern BOOL DownloadFile(CString strLocalDir,CString strUrl);

class CDownloadTask: virtual public CTaskBase
{
public:
	CDownloadTask(const CString &strDir,const CString & strUrl)
	{
		m_strUrl = strUrl;
		m_strDir = strDir;
	}
	virtual BOOL Run()
	{
		return DownloadFile(m_strDir,m_strUrl);
	}
	virtual CTaskBase * GetCopy()
	{
		CDownloadTask * pTask = new CDownloadTask(m_strDir,m_strUrl);
		return pTask;
	}
private:
	CString m_strUrl;
	CString m_strDir;
};