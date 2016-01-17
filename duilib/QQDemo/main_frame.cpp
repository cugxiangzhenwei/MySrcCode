#include "stdafx.h"
#include <windows.h>
#if !defined(UNDER_CE)
#include <shellapi.h>
#endif

#include "UIGroups.hpp"
#include "UIMicroBlog.hpp"

#include "win_impl_base.hpp"
#include "main_frame.hpp"

#include "color_skin.hpp"
#include "chat_dialog.hpp"
#include <WinSock2.h>
#include   "mmsystem.h"
#include "resource.h"
#include "UIMenu.h"

#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"winmm.lib")

const TCHAR* const kTitleControlName = _T("apptitle");
const TCHAR* const kCloseButtonControlName = _T("closebtn");
const TCHAR* const kMinButtonControlName = _T("minbtn");
const TCHAR* const kMaxButtonControlName = _T("maxbtn");
const TCHAR* const kRestoreButtonControlName = _T("restorebtn");

const TCHAR* const kTabControlName = _T("tabs");

const TCHAR* const kFriendButtonControlName = _T("friendbtn");
const TCHAR* const kGroupButtonControlName = _T("groupbtn");
const TCHAR* const kMicroBlogButtonControlName = _T("microblogbtn");

const TCHAR* const kFriendsListControlName = _T("friends");
const TCHAR* const kGroupsListControlName = _T("groups");
const TCHAR* const kMicroBlogListControlName = _T("microblog");

const TCHAR* const kHideLeftMainPannelControlName = _T("btnleft");
const TCHAR* const kShowLeftMainPannelControlName = _T("btnright");
const TCHAR* const kLeftMainPannelControlName = _T("LeftMainPanel");

const TCHAR* const kSignatureTipsControlName = _T("signaturetip");
const TCHAR* const kSignatureControlName = _T("signature");

const TCHAR* const kSearchEditTipControlName = _T("search_tip");
const TCHAR* const kSearchEditControlName = _T("search_edit");

const TCHAR* const kChangeBkSkinControlName = _T("bkskinbtn");
const TCHAR* const kChangeColorSkinControlName = _T("colorskinbtn");

const TCHAR* const kBackgroundControlName = _T("bg");
const TCHAR* const kDisPlayHostUsrName =	_T("hostUsrname");

const int kBackgroundSkinImageCount = 3;

//对应的类声明文件中定义消息  
#define WM_NOTIFYICON_MSG	 WM_USER+2  

tString GetExeDir()
{
	char szFileName[256]={0};
	GetModuleFileName(NULL,szFileName,256);
	int iIndex = strlen(szFileName) - 1;
	while(szFileName[iIndex] != '\\' && szFileName[iIndex] !='/')
		iIndex--;

	szFileName[iIndex] = 0;

	return szFileName;
}

unsigned  long   WINAPI  ThreadPro(PVOID   pvoid)
{
	MainFrame * pMain = (MainFrame*)(pvoid);
	pMain->RecvOnlineUsrinfoByBroadCast();
	return 0;
}
unsigned  long   WINAPI  ThreadRecvUdpMsg(PVOID   pvoid)
{
	MainFrame * pMain = (MainFrame*)(pvoid);
	SOCKET sockSrv = socket(AF_INET,SOCK_DGRAM,0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(5000);

	bind(sockSrv,(sockaddr*)&addrSrv,sizeof(sockaddr));

	SOCKADDR_IN addClient; //发送数据方的地址信息
	int iLen =sizeof(SOCKADDR_IN);

	NEW_MSG msg;
	while (1)
	{
		//等待接受发送到该主机的消息
		int iRecvStrLen = recvfrom(sockSrv,(char*)&msg,sizeof(NEW_MSG),0,(sockaddr*)&addClient,&iLen);
		if (iRecvStrLen < 1)
			continue;

// 		const char * pszFriendIp = inet_ntoa(addClient.sin_addr);
		::PostMessage(*pMain, WM_USER_RECEIVED_NEW_MSG, 0,(LPARAM)&msg);
	}
	return 0;
}

MainFrame::MainFrame()
: bk_image_index_(0)
{
	m_bSockOk = InitSocket();
}

MainFrame::~MainFrame()
{
	WSACleanup();
	PostQuitMessage(0);
	m_bSockOk = false;
}

LPCTSTR MainFrame::GetWindowClassName() const
{
	return _T("TXGuiFoundation");
}

CControlUI* MainFrame::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("FriendList")) == 0)
	{
		return new CFriendsUI(paint_manager_);
	}
	else if (_tcsicmp(pstrClass, _T("GroupList")) == 0)
	{
		return new CGroupsUI(paint_manager_);
	}
	else if (_tcsicmp(pstrClass, _T("MicroBlog")) == 0)
	{
		return new CMicroBlogUI(paint_manager_);
	}

	return NULL;
}

void MainFrame::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

tString MainFrame::GetSkinFile()
{
	return _T("main_frame.xml");
}

tString MainFrame::GetSkinFolder()
{
	return tString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");
}

LRESULT MainFrame::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	paint_manager_.KillTimer(paint_manager_.FindControl(kFriendsListControlName),1000);

	//如何等待处理结束呢
	if (m_bProcess)
	{
		Sleep(200);//等待好友列表处理完成
	}

	bHandled = FALSE;
	return 0;
}

LRESULT MainFrame::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#if defined(WIN32) && !defined(UNDER_CE)
	BOOL bZoomed = ::IsZoomed(m_hWnd);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if (::IsZoomed(m_hWnd) != bZoomed)
	{
		if (!bZoomed)
		{
			CControlUI* pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kMaxButtonControlName));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kRestoreButtonControlName));
			if( pControl ) pControl->SetVisible(true);
		}
		else 
		{
			CControlUI* pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kMaxButtonControlName));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kRestoreButtonControlName));
			if( pControl ) pControl->SetVisible(false);
		}
	}
	if (wParam == SC_MINIMIZE)
	{
		:: ShowWindow(m_hWnd,SW_HIDE);
		return 0;
	}
	if (wParam == SC_CLOSE)
	{
		::Shell_NotifyIcon(NIM_DELETE,&m_NotifData); //关闭时删除系统托盘图标 
		return 0;
	}
#else
	return __super::OnSysCommand(uMsg, wParam, lParam, bHandled);
#endif

	return 0;
}

LRESULT MainFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT MainFrame::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return FALSE;
}

void MainFrame::OnTimer(TNotifyUI& msg)
{
	BroadCastHostInfo();
}

void MainFrame::OnExit(TNotifyUI& msg)
{
	Close();
}

void MainFrame::Init()
{}

DWORD MainFrame::GetBkColor()
{
	CControlUI* background = paint_manager_.FindControl(kBackgroundControlName);
	if (background != NULL)
		return background->GetBkColor();

	return 0;
}

void MainFrame::SetBkColor(DWORD dwBackColor)
{
	CControlUI* background = paint_manager_.FindControl(kBackgroundControlName);
	if (background != NULL)
	{
		background->SetBkImage(_T(""));
		background->SetBkColor(dwBackColor);
		background->NeedUpdate();

		SkinChangedParam param;
		param.bkcolor = background->GetBkColor();
		param.bgimage = background->GetBkImage();
		skin_changed_observer_.Broadcast(param);
	}
}

void MainFrame::UpdateFriendsList()
{
	// 获取好友列表
	CFriendsUI* pFriendsList = static_cast<CFriendsUI*>(paint_manager_.FindControl(kFriendsListControlName));
	if (pFriendsList != NULL)
	{
		if (!friends_.empty())
			friends_.clear();
		if (pFriendsList->GetCount() > 0)
			pFriendsList->RemoveAll();

		FriendListItemInfo item;


		//添加好友列表

		item.id = _T("0");
		item.folder = true;
		item.empty = false;
		item.nick_name = _T("在线好友");

		Node* root_parent = pFriendsList->AddNode(item, NULL);
		friends_.push_back(item);

		map<tString,HOST_INFO>::iterator it;
		for(it=mapUsrHostInfo.begin();it!=mapUsrHostInfo.end();++it)
		{
			item.id = _T("2");
			item.folder = false;
			item.empty = false;
			item.logo = _T("man.png");

			HOST_INFO info = it->second;

			item.nick_name = info.szNickName;
			item.description = info.szIp;
			item.strHostName = info.szHostName;
			item.strIp = info.szIp;

		}

		item.id = _T("3");
		item.folder = true;
		item.empty = false;
		item.nick_name = _T("我自己");
		Node* root_myself = pFriendsList->AddNode(item, NULL);
		friends_.push_back(item);
		myself_info_.folder = false;
		myself_info_.empty = false;
		myself_info_.logo = _T("man.png");
		pFriendsList->AddNode(myself_info_, root_myself);
		friends_.push_back(myself_info_);

	}
}
void MainFrame::AddNewUserToFriendList(HOST_INFO info)
{
	CFriendsUI* pFriendsList = static_cast<CFriendsUI*>(paint_manager_.FindControl(kFriendsListControlName));
	if (pFriendsList == NULL)
		return;

	Node* root_parent = pFriendsList->GetRoot();
	if (!root_parent->has_children())
		return;

	Node *nodeFriend = root_parent->child(0);

	m_bProcess = true;
	map<tString,HOST_INFO>::iterator it;
	bool bHave = false;
	for(it=mapUsrHostInfo.begin();it!=mapUsrHostInfo.end();++it)
	{
		if(_tcsicmp(it->first.c_str(),info.szIp)==0)
		{
			bHave = true;
			break;
		}
	}
	if (!bHave)
	{
		int iFriendCount = nodeFriend->num_children();
		char szFriendId[4]={0};
		sprintf(szFriendId,"%d",iFriendCount+1);

		FriendListItemInfo item;
		item.id = szFriendId;
		item.folder = false;
		item.empty = false;
		item.logo = _T("man.png");
		item.nick_name = info.szNickName;
		item.description = info.szIp;
		item.strHostName = info.szHostName;
		item.strIp = info.szIp;

		pFriendsList->AddNode(item, nodeFriend);
		friends_.push_back(item);

		tString strMusic = GetExeDir() + "\\Music\\Global.wav";
		PlaySound(strMusic.c_str(),NULL,SND_FILENAME | SND_ASYNC);  // 好友上线时播放音乐
	}

	mapUsrHostInfo[info.szIp] = info;

	m_bProcess = false;
	printf("\a"); //好友上线响铃
}

void MainFrame::UpdateGroupsList()
{
	CGroupsUI* pGroupsList = static_cast<CGroupsUI*>(paint_manager_.FindControl(kGroupsListControlName));
	if (pGroupsList != NULL)
	{
		if (pGroupsList->GetCount() > 0)
			pGroupsList->RemoveAll();

		GroupsListItemInfo item;

		item.folder = true;
		item.empty = false;
		item.nick_name = _T("我的QQ群");

		Node* root_parent = pGroupsList->AddNode(item, NULL);

		item.folder = false;
		item.logo = _T("duilib.png");
		item.nick_name = _T("Duilib官方交流群");
		item.description = _T("153787916");
		pGroupsList->AddNode(item, root_parent);

		item.folder = false;
		item.logo = _T("groups.png");
		item.nick_name = _T("Duilib官方交流群2");
		item.description = _T("79145400");
		pGroupsList->AddNode(item, root_parent);
	}
}

void MainFrame::UpdateMicroBlogList()
{
	CMicroBlogUI* pMicroBlogList = static_cast<CMicroBlogUI*>(paint_manager_.FindControl(kMicroBlogListControlName));
	if (pMicroBlogList != NULL)
	{}
}
void MainFrame::CreateNotifyIcon()
{
	m_NotifData.cbSize=sizeof(NOTIFYICONDATA);   
	m_NotifData.hWnd=this->m_hWnd;   
	m_NotifData.uFlags=NIF_MESSAGE|NIF_ICON|NIF_TIP;   
	m_NotifData.uCallbackMessage=WM_NOTIFYICON_MSG;  

	_tcscpy(m_NotifData.szTip, "托盘图标实例");  //鼠标在托盘上移动时显示文本   
	m_NotifData.uID=IDI_ICON_NOTIFY;   

	HINSTANCE hinstance =  CPaintManagerUI::GetInstance();
	HICON hIcon = LoadIcon(hinstance,MAKEINTRESOURCE(IDI_ICON_NOTIFY));
	m_NotifData.hIcon=hIcon;   
	::Shell_NotifyIcon(NIM_ADD,&m_NotifData);   
	if(hIcon)::DestroyIcon(hIcon);  
}
void MainFrame::OnPrepare(TNotifyUI& msg)
{
	CControlUI* background = paint_manager_.FindControl(kBackgroundControlName);
	if (background != NULL)
	{
		TCHAR szBuf[MAX_PATH] = {0};
#if defined(UNDER_WINCE)
		_stprintf(szBuf, _T("file='bg%d.png' corner='600,200,1,1'"), bk_image_index_);
#else
		_stprintf_s(szBuf, MAX_PATH - 1, _T("file='bg%d.png' corner='600,200,1,1'"), bk_image_index_);
#endif
		background->SetBkImage(szBuf);
	}

	BroadCastHostInfo();

	CControlUI* HostUsrName = paint_manager_.FindControl(kDisPlayHostUsrName);
	if (HostUsrName != NULL)
		HostUsrName->SetText(myself_info_.nick_name.c_str());
	

	paint_manager_.SetTimer(paint_manager_.FindControl(kFriendsListControlName),1000,1000);

	DWORD   dwThreadId;   
	void * m_pThread   =   CreateThread(     
		NULL,       //   no   security   attributes     
		0,          //   use   default   stack   size       
		ThreadPro,  //   thread   function     
		this,       //   argument   to   thread   function     
		0,      //   use   default   creation   flags     
		&dwThreadId);     
	WaitForSingleObject(m_pThread,1000);   

	UpdateFriendsList();

	UpdateGroupsList();

	UpdateMicroBlogList();

	DWORD   dwThreadId2;   
	void * m_pThreadRecvUdp   =   CreateThread(     
		NULL,       //   no   security   attributes     
		0,          //   use   default   stack   size       
		ThreadRecvUdpMsg,  //   thread   function     
		this,       //   argument   to   thread   function     
		0,      //   use   default   creation   flags     
		&dwThreadId2);   

	WaitForSingleObject(m_pThreadRecvUdp,10); 
	CreateNotifyIcon();
}

void MainFrame::Notify(TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType, _T("windowinit")) == 0)
	{
		OnPrepare(msg);
	}
	else if (_tcsicmp(msg.sType, _T("killfocus")) == 0)
	{
		if (_tcsicmp(msg.pSender->GetName(), kSignatureControlName) == 0)
		{
			msg.pSender->SetVisible(false);
			CControlUI* signature_tip = paint_manager_.FindControl(kSignatureTipsControlName);
			if (signature_tip != NULL)
			{
				CRichEditUI* signature = static_cast<CRichEditUI*>(msg.pSender);
				if (signature != NULL)
					signature_tip->SetText(signature->GetText());
				signature_tip->SetVisible(true);
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kSearchEditControlName) == 0)
		{
			msg.pSender->SetVisible(false);
			CControlUI* search_tip = static_cast<CRichEditUI*>(paint_manager_.FindControl(kSearchEditTipControlName));
			if (search_tip != NULL)
			{
				CRichEditUI* search_edit = static_cast<CRichEditUI*>(msg.pSender);
				if (search_edit != NULL)
					search_tip->SetText(search_edit->GetText());
				search_tip->SetVisible(true);
			}
		}
	}
	else if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		if (_tcsicmp(msg.pSender->GetName(), kCloseButtonControlName) == 0)
		{
			OnExit(msg);
		}
		else if (_tcsicmp(msg.pSender->GetName(), kMinButtonControlName) == 0)
		{
#if defined(UNDER_CE)
			::ShowWindow(m_hWnd, SW_MINIMIZE);
#else
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
#endif
		}
		else if (_tcsicmp(msg.pSender->GetName(), kMaxButtonControlName) == 0)
		{
#if defined(UNDER_CE)
			::ShowWindow(m_hWnd, SW_MAXIMIZE);
			CControlUI* pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kMaxButtonControlName));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kRestoreButtonControlName));
			if( pControl ) pControl->SetVisible(true);
#else
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
#endif
		}
		else if (_tcsicmp(msg.pSender->GetName(), kRestoreButtonControlName) == 0)
		{
#if defined(UNDER_CE)
			::ShowWindow(m_hWnd, SW_RESTORE);
			CControlUI* pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kMaxButtonControlName));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kRestoreButtonControlName));
			if( pControl ) pControl->SetVisible(false);
#else
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
#endif
		}
		else if (_tcsicmp(msg.pSender->GetName(), kHideLeftMainPannelControlName) == 0)
		{
			CControlUI* left_main_pannel = paint_manager_.FindControl(kLeftMainPannelControlName);
			CControlUI* hide_left_main_pannel = paint_manager_.FindControl(kHideLeftMainPannelControlName);
			CControlUI* show_left_main_pannel = paint_manager_.FindControl(kShowLeftMainPannelControlName);
			if ((left_main_pannel != NULL) && (show_left_main_pannel != NULL) && (hide_left_main_pannel != NULL))
			{
				hide_left_main_pannel->SetVisible(false);
				left_main_pannel->SetVisible(false);
				show_left_main_pannel->SetVisible(true);
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kShowLeftMainPannelControlName) == 0)
		{
			CControlUI* left_main_pannel = paint_manager_.FindControl(kLeftMainPannelControlName);
			CControlUI* hide_left_main_pannel = paint_manager_.FindControl(kHideLeftMainPannelControlName);
			CControlUI* show_left_main_pannel = paint_manager_.FindControl(kShowLeftMainPannelControlName);
			if ((left_main_pannel != NULL) && (show_left_main_pannel != NULL) && (hide_left_main_pannel != NULL))
			{
				hide_left_main_pannel->SetVisible(true);
				left_main_pannel->SetVisible(true);
				show_left_main_pannel->SetVisible(false);
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kSignatureTipsControlName) == 0)
		{
			msg.pSender->SetVisible(false);
			CRichEditUI* signature = static_cast<CRichEditUI*>(paint_manager_.FindControl(kSignatureControlName));
			if (signature != NULL)
			{
				signature->SetText(msg.pSender->GetText());
				signature->SetVisible(true);
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kSearchEditTipControlName) == 0)
		{
			msg.pSender->SetVisible(false);
			CRichEditUI* search_edit = static_cast<CRichEditUI*>(paint_manager_.FindControl(kSearchEditControlName));
			if (search_edit != NULL)
			{
				search_edit->SetText(msg.pSender->GetText());
				search_edit->SetVisible(true);
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kChangeBkSkinControlName) == 0)
		{
			CControlUI* background = paint_manager_.FindControl(kBackgroundControlName);
			if (background != NULL)
			{
				TCHAR szBuf[MAX_PATH] = {0};
				++bk_image_index_;
				if (kBackgroundSkinImageCount < bk_image_index_)
					bk_image_index_ = 0;

#if defined(UNDER_WINCE)
				_stprintf(szBuf, _T("file='bg%d.png' corner='600,200,1,1'"), bk_image_index_);
#else
				_stprintf_s(szBuf, MAX_PATH - 1, _T("file='bg%d.png' corner='600,200,1,1'"), bk_image_index_);
#endif
				background->SetBkImage(szBuf);

				SkinChangedParam param;
				CControlUI* background = paint_manager_.FindControl(kBackgroundControlName);
				if (background != NULL)
				{
					param.bkcolor = background->GetBkColor();
					if (_tcslen(background->GetBkImage()) > 0)
					{
#if defined(UNDER_WINCE)
						_stprintf(szBuf, _T("bg%d.png"), bk_image_index_);
#else
						_stprintf_s(szBuf, MAX_PATH - 1, _T("bg%d.png"), bk_image_index_);
#endif
					}

					param.bgimage = szBuf;
				}
				skin_changed_observer_.Broadcast(param);
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kChangeColorSkinControlName) == 0)
		{
			CRect rcWindow;
			GetWindowRect(m_hWnd, &rcWindow);
			rcWindow.top = rcWindow.top + msg.pSender->GetPos().bottom;
			new ColorSkinWindow(this, rcWindow);
		}
	}
	else if (_tcsicmp(msg.sType, _T("timer")) == 0)
	{
		return OnTimer(msg);
	}
	else if (_tcsicmp(msg.sType, _T("selectchanged")) == 0)
	{
		CTabLayoutUI* pTabControl = static_cast<CTabLayoutUI*>(paint_manager_.FindControl(kTabControlName));
		if (_tcsicmp(msg.pSender->GetName(), kFriendButtonControlName) == 0)
		{
			if (pTabControl && pTabControl->GetCurSel() != 0)
			{
				pTabControl->SelectItem(0);
				UpdateFriendsList();
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kGroupButtonControlName) == 0)
		{
			if (pTabControl && pTabControl->GetCurSel() != 1)
			{
				pTabControl->SelectItem(1);
				UpdateGroupsList();
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kMicroBlogButtonControlName) == 0)
		{
			if (pTabControl && pTabControl->GetCurSel() != 2)
			{
				pTabControl->SelectItem(2);
				UpdateMicroBlogList();
			}
		}
	}
	else if (_tcsicmp(msg.sType, _T("itemactivate")) == 0)
	{
		CTabLayoutUI* pTabControl = static_cast<CTabLayoutUI*>(paint_manager_.FindControl(kTabControlName));
		if (pTabControl != NULL)
		{
			if (pTabControl->GetCurSel() == 0)
			{
				CFriendsUI* pFriendsList = static_cast<CFriendsUI*>(paint_manager_.FindControl(kFriendsListControlName));
				if ((pFriendsList != NULL) &&  pFriendsList->GetItemIndex(msg.pSender) != -1)
				{
					if (_tcsicmp(msg.pSender->GetClass(), _T("ListContainerElementUI")) == 0)
					{
						Node* node = (Node*)msg.pSender->GetTag();

						CControlUI* background = paint_manager_.FindControl(kBackgroundControlName);
						if (!pFriendsList->CanExpand(node) && (background != NULL))
						{
							FriendListItemInfo friend_info;

							std::vector<FriendListItemInfo>::iterator citer;
							for (citer = friends_.begin(); citer != friends_.end(); ++citer)
							{
								if (_tcsicmp(citer->id.c_str(), node->data().value.c_str()) == 0)
								{
									friend_info = *citer;
									break;
								}
							}

							ChatDialog* pChatDialog = 	GetThisFriendChatDialog(citer->strIp.c_str());
							pChatDialog->CenterWindow();
							::ShowWindow(*pChatDialog, SW_SHOW);
						}
					}
				}
			}
		}
	}
	else if (_tcsicmp(msg.sType, _T("itemclick")) == 0)
	{
		CTabLayoutUI* pTabControl = static_cast<CTabLayoutUI*>(paint_manager_.FindControl(kTabControlName));
		if (pTabControl != NULL)
		{
			if (pTabControl->GetCurSel() == 0)
			{
				CFriendsUI* pFriendsList = static_cast<CFriendsUI*>(paint_manager_.FindControl(kFriendsListControlName));
				if ((pFriendsList != NULL) &&  pFriendsList->GetItemIndex(msg.pSender) != -1)
				{
					if (_tcsicmp(msg.pSender->GetClass(), _T("ListContainerElementUI")) == 0)
					{
						Node* node = (Node*)msg.pSender->GetTag();

						if (pFriendsList->CanExpand(node))
						{
							pFriendsList->SetChildVisible(node, !node->data().child_visible_);
						}
					}
				}
			}
			else if (pTabControl->GetCurSel() == 1)
			{
				CGroupsUI* pGroupsList = static_cast<CGroupsUI*>(paint_manager_.FindControl(kGroupsListControlName));
				if ((pGroupsList != NULL) &&  pGroupsList->GetItemIndex(msg.pSender) != -1)
				{
					if (_tcsicmp(msg.pSender->GetClass(), _T("ListContainerElementUI")) == 0)
					{
						Node* node = (Node*)msg.pSender->GetTag();

						if (pGroupsList->CanExpand(node))
						{
							pGroupsList->SetChildVisible(node, !node->data().child_visible_);
						}
					}
				}
			}

		}
	}
}

LRESULT MainFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == WM_USER_RECEIVED_NEW_MSG)
	{
		NEW_MSG *pMsg = (NEW_MSG *)lParam;
		ReceivedNewMsg(*pMsg);
	}
	else if (uMsg == WM_NOTIFYICON_MSG)
	{
		if(lParam==WM_LBUTTONDBLCLK)     
			::ShowWindow(m_hWnd,SW_SHOW); //鼠标双击时主窗口出现
		else if(lParam==WM_RBUTTONDOWN)
		{    
			CMenuWnd *pMenu = new CMenuWnd(m_hWnd);
			CPoint pos;   
			GetCursorPos(&pos); 
			STRINGorID xml(IDR_XML_MENU);
			pMenu->Init(NULL, xml, _T("xml"), pos);
		}
	}
	return 0;
}
void MainFrame::RecvOnlineUsrinfoByBroadCast()
{
	if (!m_bSockOk)
		return;

//	paint_manager_.KillTimer(paint_manager_.FindControl(kFriendsListControlName),1000);

	//绑定套接字的地址
	SOCKADDR_IN sBind;
	sBind.sin_family=AF_INET;
	sBind.sin_addr.s_addr=htonl(INADDR_ANY); ///本机
	sBind.sin_port=htons(6000);

	int fromlength=sizeof(SOCKADDR);

	//用UDP初始化套接字
	SOCKET s=socket(AF_INET,SOCK_DGRAM,0);

	//设置该套接字为广播类型
	bool optval =true;
	setsockopt(s,SOL_SOCKET,SO_BROADCAST,(char FAR *)&optval,sizeof(optval));


	int iReturn = bind(s,(sockaddr *)&sBind,sizeof(sockaddr_in));

	int isss = 0;
	while(1)
	{
		HOST_INFO usrInfo;
		//从广播地址接收消息，注意用来绑定的地址和接收消息的地址是不一样的
		SOCKADDR_IN addrFrom;
		printf("等待接收广播消息...\n");
		iReturn  =  recvfrom(s,(char*)&usrInfo,sizeof(HOST_INFO),0,(sockaddr*)&addrFrom,(int FAR *)&fromlength);

		if (usrInfo.szIp[0] != '0' && usrInfo.szIp[0] !='1' && usrInfo.szIp[0] !='2')
			continue;

		//paint_manager_.SetTimer(paint_manager_.FindControl(kFriendsListControlName),1000,1000);
		AddNewUserToFriendList(usrInfo);
		isss++;
	}
}
void MainFrame::BroadCastHostInfo()
{
	if (!m_bSockOk)
		return;

	char host_name[255];
	if (gethostname(host_name, sizeof(host_name)) == SOCKET_ERROR)
	{
		return;
	}

	struct hostent *phe = gethostbyname(host_name);
	if (phe == 0) {
		return;
	}
	struct in_addr addrHost;
	memcpy(&addrHost, phe->h_addr_list[0], sizeof(struct in_addr));

	DWORD dLendth = 256;
	char szUserName[256] = {0};
	if (!GetUserName(szUserName,&dLendth))
		return;

	const char *pszIpInfo = inet_ntoa(addrHost);

	myself_info_.strHostName = host_name;
	myself_info_.strIp = pszIpInfo;

	myself_info_.nick_name = szUserName;
	myself_info_.description = pszIpInfo;

	HOST_INFO hostiInfo;
	strcpy(hostiInfo.szHostName,host_name);
	strcpy(hostiInfo.szIp,pszIpInfo);
	strcpy(hostiInfo.szNickName,szUserName);

	SOCKET s = WSASocket(AF_INET,SOCK_DGRAM,0,NULL,0,WSA_FLAG_OVERLAPPED);

	SOCKADDR_IN addrTo;
	addrTo.sin_family = AF_INET;
	addrTo.sin_addr.s_addr = INADDR_BROADCAST;
	addrTo.sin_port = htons(6000);

	//设置该套接字为广播类型
	bool opt = true;
	setsockopt(s,SOL_SOCKET,SO_BROADCAST,(char FAR *)&opt,sizeof(opt));

	//从广播地址发送消息
	int nlen=sizeof(addrTo);
	int ret=sendto(s,(char*)&hostiInfo,sizeof(HOST_INFO)+1,0,(sockaddr*)&addrTo,nlen);
	if(ret==SOCKET_ERROR)
		fprintf(stdout,"%s\n",WSAGetLastError());
	else
	{		
		fprintf(stdout,"广播消息发送成功!\n");	
	}
}
bool MainFrame::InitSocket()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 1, 1 );

	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		return false;
	}

	/* Confirm that the WinSock DLL supports 1.1.*/
	/* Note that if the DLL supports versions greater    */
	/* than 1.1 in addition to 1.1, it will still return */
	/* 1.1 in wVersion since that is the version we      */
	/* requested.                                        */

	if ( LOBYTE( wsaData.wVersion ) != 1||
		HIBYTE( wsaData.wVersion ) != 1 ) {
			/* Tell the user that we could not find a usable */
			/* WinSock DLL.                                  */
			WSACleanup( );
			return false ; 
	}

	return true;
}
void MainFrame::ReceivedNewMsg(NEW_MSG msg)
{
	tString strMusic = GetExeDir();
	if (msg.iMsgType == ChatMsg)
	{
		strMusic += "\\Music\\msg.wav";
	}
	else if ( msg.iMsgType == CommondMsg && strcmp(msg.szNewMsg,"ShakeWindow")==0 )
	{
		strMusic += "\\Music\\shake.wav";
	}

	PlaySound(strMusic.c_str(),NULL,SND_FILENAME | SND_ASYNC);  // 接收到新的消息时播放音乐

	ChatDialog* pChatDialog = GetThisFriendChatDialog(msg.szIp);
	pChatDialog->CenterWindow();
	::ShowWindow(*pChatDialog, SW_SHOW);
	NEW_MSG *pMsg = new NEW_MSG;
	memcpy(pMsg,&msg,sizeof(NEW_MSG));
	::PostMessage(*pChatDialog, WM_USER_RECEIVED_NEW_MSG, 0,(LPARAM)pMsg);
}
void SendMsgToFriend(const char *pszIp,NEW_MSG msg)
{
	SOCKET sockClient = socket(AF_INET,SOCK_DGRAM,0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr =inet_addr(pszIp);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(5000);

	int iLen = sizeof(SOCKADDR);
	sendto(sockClient,(char*)&msg,sizeof(NEW_MSG),0,(sockaddr*)&addrSrv,sizeof(SOCKADDR));
}
ChatDialog * MainFrame::GetThisFriendChatDialog(const char *pszIp)
{
	CControlUI* background = paint_manager_.FindControl(kBackgroundControlName);

	FriendListItemInfo friend_info;
	std::vector<FriendListItemInfo>::iterator citer;
	for (citer = friends_.begin(); citer != friends_.end(); ++citer)
	{
		if (_tcsicmp(citer->strIp.c_str(),pszIp) == 0)
		{
			friend_info = *citer;
			break;
		}
	}

	if (citer->m_pChatDlg)
		return citer->m_pChatDlg;


	TCHAR szBuf[MAX_PATH] = {0};
	if (_tcslen(background->GetBkImage()) > 0)
	{
#if defined(UNDER_WINCE)
		_stprintf(szBuf, _T("bg%d.png"), bk_image_index_);
#else
		_stprintf_s(szBuf, MAX_PATH - 1, _T("bg%d.png"), bk_image_index_);
#endif
	}

	ChatDialog* pChatDialog = new ChatDialog(this,szBuf, background->GetBkColor(), myself_info_, friend_info);
	if( pChatDialog == NULL )
		return NULL;

#if defined(WIN32) && !defined(UNDER_CE)
	pChatDialog->Create(NULL, _T("ChatDialog"), UI_WNDSTYLE_FRAME | WS_POPUP,  NULL, 0, 0, 0, 0);
#else
	pChatDialog->Create(NULL, _T("ChatDialog"), UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 0, 0);
#endif
	skin_changed_observer_.AddReceiver(pChatDialog);
	citer->m_pChatDlg = pChatDialog;
	return pChatDialog;
}
void MainFrame::DestroyChatDialogPointer(tString szIp)
{
	FriendListItemInfo friend_info;
	std::vector<FriendListItemInfo>::iterator citer;
	bool bFind = false;
	for (citer = friends_.begin(); citer != friends_.end(); ++citer)
	{
		if (_tcsicmp(citer->strIp.c_str(),szIp.c_str()) == 0)
		{
			friend_info = *citer;
			bFind = true;
			break;
		}
	}
	if (bFind)
		citer->m_pChatDlg = NULL;
}