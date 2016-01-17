#include <Windows.h>
#include <CommDlg.h>
#include <iostream>

using namespace std;

static OPENFILENAMEA ofn ;

void PopFileInitialize (HWND hwnd)
{
	static char szFilter[] = "图像(*.jpg;*.bmp;*.gif;*.tif;*.img)\0*.jpg;*.bmp;*.gif;*.tif;*.img\0"\
							 "文档(*.doc;*.docx;*.txt;*.cpp;*.h)\0*.doc;*.docx;*.txt;*.cpp;*.h\0" \
							 "压缩文件(*.zip;*.rar;*.7z)\0*.zip;*.rar;*.7z\0" \
							 "音乐文件(*.mp3;*.wma;*.wav)\0*.mp3;*.wma;*.wav\0" \
							 "视频文件(*.rmvb;*.mp4;*.mkv;*.flv;*.avi)\0*.rmvb;*.mp4;*.mkv;*.flv;*.avi\0" \
							 "所有文件(*.*)\0*.*\0\0";
		                       
     ofn.lStructSize       = sizeof (OPENFILENAME) ;
     ofn.hwndOwner         = hwnd ;
     ofn.hInstance         = NULL ;
     ofn.lpstrFilter       = szFilter ;
     ofn.lpstrCustomFilter = NULL ;
     ofn.nMaxCustFilter    = 0 ;
     ofn.nFilterIndex      = 0 ;
     ofn.lpstrFile         = NULL ;          // Set in Open and Close functions
     ofn.nMaxFile          = MAX_PATH ;
     ofn.lpstrFileTitle    = NULL ;          // Set in Open and Close functions
     ofn.nMaxFileTitle     = MAX_PATH ;
     ofn.lpstrInitialDir   = NULL ;
     ofn.lpstrTitle        = NULL ;
     ofn.Flags             = 0 ;             // Set in Open and Close functions
     ofn.nFileOffset       = 0 ;
     ofn.nFileExtension    = 0 ;
     ofn.lpstrDefExt       = "jpg" ;
     ofn.lCustData         = 0L ;
     ofn.lpfnHook          = NULL ;
     ofn.lpTemplateName    = NULL ;
}

BOOL PopFileOpenDlg (HWND hwnd, LPSTR pstrFileName, LPSTR pstrTitleName)
{
     ofn.hwndOwner         = hwnd ;
     ofn.lpstrFile         = pstrFileName ;
     ofn.lpstrFileTitle    = pstrTitleName ;
     ofn.Flags             = OFN_HIDEREADONLY | OFN_CREATEPROMPT ;
     
     return GetOpenFileName (&ofn) ;
}

int main()
{
	static char     szFileName[MAX_PATH], szTitleName[MAX_PATH] ;
	HWND      hwnd;
	hwnd=GetForegroundWindow(); //获取前台窗口句柄。本程序中的前台窗口就是控制台窗口。 
	PopFileInitialize (hwnd);  //初始化ofn
	PopFileOpenDlg(hwnd, szFileName, szTitleName);//打开文件对话框
	cout<<szFileName<<endl;  //在控制台中显示选中文件的路径
	return 0;
}