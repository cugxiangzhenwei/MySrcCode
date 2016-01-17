#pragma once
#include <Ice/Ice.h>

class CIceService
{
public:
	CIceService(void);
	~CIceService(void);

	void StartService();
	void StartServiceInthread();
	virtual void OnStop();
	static DWORD WINAPI ThreadFunService(LPVOID);
protected:
	virtual std::string GetEndPoints();
	virtual Ice::CommunicatorPtr CIceService::InitCommunicator();
private:
	Ice::CommunicatorPtr m_communicatorPtr;
	std::string  m_strServerPort;
	HANDLE  m_hThread;
};