#ifndef MAINFRAME_HPP
#define MAINFRAME_HPP

#include "skin_change_event.hpp"
#include "UIFriends.hpp"
#include <shellapi.h>

#define WM_USER_RECEIVED_NEW_MSG   WM_USER + 1     // 接收到新的消息

#define ChatMsg    0
#define CommondMsg 1


struct NEW_MSG
{
	int  iMsgType;
	char szIp[15];
	char szNewMsg[500];

	NEW_MSG()
	{
		iMsgType = ChatMsg;
	}
};
class WindowImplBase;
class ChatDialog;
class MainFrame : public WindowImplBase
{
public:

	MainFrame();
	~MainFrame();

public:

	LPCTSTR GetWindowClassName() const;	

	virtual void OnFinalMessage(HWND hWnd);

	virtual void Init();

	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

	virtual tString GetSkinFile();

	virtual tString GetSkinFolder();

	virtual CControlUI* CreateControl(LPCTSTR pstrClass);

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    DWORD GetBkColor();
    void SetBkColor(DWORD dwBackColor);
	void RecvOnlineUsrinfoByBroadCast();
	void UpdateFriendsList();
	void ReceivedNewMsg(NEW_MSG);
	void DestroyChatDialogPointer(tString szIp);
protected:	

	void Notify(TNotifyUI& msg);
	void OnPrepare(TNotifyUI& msg);
	void OnExit(TNotifyUI& msg);
	void OnTimer(TNotifyUI& msg);

private:
	bool InitSocket();

	void UpdateGroupsList();

	void UpdateMicroBlogList();

	void BroadCastHostInfo();
	void AddNewUserToFriendList(HOST_INFO info);
	ChatDialog * GetThisFriendChatDialog(const char *pszIp);
	void CreateNotifyIcon();

private:
	int bk_image_index_;

	FriendListItemInfo myself_info_;
	std::vector<FriendListItemInfo> friends_;
	std::map<tString,HOST_INFO> mapUsrHostInfo;

	SkinChangedObserver skin_changed_observer_;

	bool m_bSockOk;
	bool m_bProcess;
	NOTIFYICONDATA m_NotifData;
};

void SendMsgToFriend(const char *pszIp,NEW_MSG msg);

#endif // MAINFRAME_HPP