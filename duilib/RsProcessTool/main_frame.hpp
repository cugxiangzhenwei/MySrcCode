#ifndef MAINFRAME_HPP
#define MAINFRAME_HPP

#include "skin_change_event.hpp"
#include "UIFriends.hpp"
#include "ProductStruct.hpp"
class WindowImplBase;
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
	int GetProductGroupCount();
	CProductGroup GetProductGroup(int index);
	CProductGroup GetProductGroup(tString strGroutTitle);
	bool LoadConfigXml(tString strXml);
protected:	

	void Notify(TNotifyUI& msg);
	void OnPrepare(TNotifyUI& msg);
	void OnExit(TNotifyUI& msg);
	void OnTimer(TNotifyUI& msg);

private:

	void UpdateFriendsList();

	void UpdateGroupsList();

	void UpdateMicroBlogList();

	ProductStruct FindProductByName(const char * pszName);

	void BuildSearchKey();
	void GetSearchResultList(tString strKey);

private:
	int bk_image_index_;

	FriendListItemInfo myself_info_;
	std::vector<FriendListItemInfo> friends_;
	std::vector<FriendListItemInfo> searchResultList;

	SkinChangedObserver skin_changed_observer_;

	vector<CProductGroup> m_vProductGroup;
};

#endif // MAINFRAME_HPP