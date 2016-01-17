#ifndef UIFRIENDS_HPP
#define UIFRIENDS_HPP

#include "UIListCommonDefine.hpp"

class ChatDialog;
namespace DuiLib
{
struct FriendListItemInfo
{
	bool folder;
	bool empty;
	tString id;
	tString logo;
	tString nick_name;
	tString description;
	tString strIp;
	tString strHostName;
	ChatDialog * m_pChatDlg;
	FriendListItemInfo()
	{
		m_pChatDlg = NULL;
	}
};

struct HOST_INFO
{
	char szIp[15];
	char szHostName[20];
	char szNickName[20];
};

class CFriendsUI : public CListUI
{
public:
	enum {SCROLL_TIMERID = 10};

	CFriendsUI(CPaintManagerUI& paint_manager);

	~CFriendsUI();

	bool Add(CControlUI* pControl);

	bool AddAt(CControlUI* pControl, int iIndex);

	bool Remove(CControlUI* pControl);

	bool RemoveAt(int iIndex);

	void RemoveAll();

	void DoEvent(TEventUI& event);

	Node* GetRoot();

	Node* AddNode(const FriendListItemInfo& item, Node* parent = NULL);

	bool RemoveNode(Node* node);

	void SetChildVisible(Node* node, bool visible);

	bool CanExpand(Node* node) const;

	bool SelectItem(int iIndex, bool bTakeFocus = false);

private:
	Node*	root_node_;
	LONG	delay_deltaY_;
	DWORD	delay_number_;
	DWORD	delay_left_;
	CRect	text_padding_;
	int level_text_start_pos_;
	tString level_expand_image_;
	tString level_collapse_image_;
	CPaintManagerUI& paint_manager_;

    CDialogBuilder m_dlgBuilder;
};

} // DuiLib

#endif // UIFRIENDS_HPP