; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMainFrame
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ªÊÕº≥Ã–Ú.h"
LastPage=0

ClassCount=7
Class1=CMyApp
Class2=CMyDoc
Class3=CMyView
Class4=CMainFrame

ResourceCount=7
Resource1=IDD_ABOUTBOX
Class5=CAboutDlg
Resource2=IDR_GRAPHTOOL
Resource3=IDR_GRAPHTOOL1
Resource4=IDR_MAINFRAME
Resource5=IDD_LINESTYLE
Class6=CLineWidthDlg
Resource6=IDD_LINEWIDTH
Class7=CLineStyleDlg
Resource7=CG_IDR_POPUP_MY_VIEW

[CLS:CMyApp]
Type=0
HeaderFile=ªÊÕº≥Ã–Ú.h
ImplementationFile=ªÊÕº≥Ã–Ú.cpp
Filter=N
LastObject=CMyApp

[CLS:CMyDoc]
Type=0
HeaderFile=ªÊÕº≥Ã–ÚDoc.h
ImplementationFile=ªÊÕº≥Ã–ÚDoc.cpp
Filter=N
LastObject=CMyDoc
BaseClass=CDocument
VirtualFilter=DC

[CLS:CMyView]
Type=0
HeaderFile=ªÊÕº≥Ã–ÚView.h
ImplementationFile=ªÊÕº≥Ã–ÚView.cpp
Filter=C
BaseClass=CView
VirtualFilter=VWC
LastObject=CMyView


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=ID_SET_LINESTYLE
BaseClass=CFrameWnd
VirtualFilter=fWC




[CLS:CAboutDlg]
Type=0
HeaderFile=ªÊÕº≥Ã–Ú.cpp
ImplementationFile=ªÊÕº≥Ã–Ú.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_MENUITEM32773
Command2=ID_FILE_OPEN
Command3=ID_MYOPENDLG
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_EDIT_UNDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_VIEW_TOOLBAR
Command16=ID_VIEW_STATUS_BAR
Command17=ID_VIEW_GRAPHTOOL
Command18=ID_GRAPH_POINT
Command19=ID_GRAPH_LINE
Command20=ID_DRAW_ELLIPSE
Command21=ID_DARA_ROUND
Command22=ID_DRAW_POLYLINE
Command23=ID_DRAW_RECTANGLE
Command24=ID_DRAW_POLYGON
Command25=ID_BITMAP_SHOW
Command26=ID_SET_LINEWIDTH
Command27=ID_SET_COLOR
Command28=ID_SET_LINESTYLE
Command29=ID_APP_ABOUT
CommandCount=29

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_EDIT_COPY
Command2=ID_GRAPH_LINE
Command3=ID_FILE_NEW
Command4=ID_FILE_OPEN
Command5=ID_GRAPH_POINT
Command6=ID_FILE_PRINT
Command7=ID_DRAW_RECTANGLE
Command8=ID_FILE_SAVE
Command9=ID_EDIT_PASTE
Command10=ID_EDIT_UNDO
Command11=ID_EDIT_CUT
Command12=ID_NEXT_PANE
Command13=ID_PREV_PANE
Command14=ID_EDIT_COPY
Command15=ID_EDIT_PASTE
Command16=ID_EDIT_CUT
Command17=ID_EDIT_UNDO
CommandCount=17

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_GRAPH_POINT
Command9=ID_GRAPH_LINE
Command10=ID_APP_ABOUT
CommandCount=10

[MNU:CG_IDR_POPUP_MY_VIEW]
Type=1
Class=?
Command1=ID_GRAPH_POINT
Command2=ID_GRAPH_LINE
Command3=ID_EDIT_PASTE
CommandCount=3

[TB:IDR_GRAPHTOOL]
Type=1
Class=?
Command1=ID_GRAPH_POINT
Command2=ID_GRAPH_LINE
CommandCount=2

[TB:IDR_GRAPHTOOL1]
Type=1
Class=?
CommandCount=0

[DLG:IDD_LINEWIDTH]
Type=1
Class=CLineWidthDlg
ControlCount=5
Control1=IDC_EDIT_LINEWIDTH,edit,1350631553
Control2=IDOK,button,1342242817
Control3=IDCANCEL,button,1342242816
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352

[CLS:CLineWidthDlg]
Type=0
HeaderFile=LineWidthDlg.h
ImplementationFile=LineWidthDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_EDIT_LINEWIDTH
VirtualFilter=dWC

[DLG:IDD_LINESTYLE]
Type=1
Class=CLineStyleDlg
ControlCount=2
Control1=IDC_COMBO_LINESTYLE,combobox,1344339971
Control2=IDC_STATIC,static,1342308353

[CLS:CLineStyleDlg]
Type=0
HeaderFile=LineStyleDlg.h
ImplementationFile=LineStyleDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_COMBO_LINESTYLE
VirtualFilter=dWC

