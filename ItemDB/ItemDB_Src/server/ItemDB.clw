; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMainFrame
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ItemDB.h"
LastPage=0

ClassCount=11
Class1=CItemDBApp
Class2=CItemDBDoc
Class3=CRealmView
Class4=CMainFrame

ResourceCount=13
Resource1=IDR_MAINFRAME_CHS
Resource2=IDR_SEARCH_ENG (English (U.S.))
Resource3=IDD_ITEMDBDLG
Resource4=IDR_POPUP_CHS
Class5=CAboutDlg
Class6=CCharList
Class7=CItemDBView
Resource5=IDR_MENU_ENG (English (U.S.))
Resource6=IDR_SEARCH_CHS
Resource7=IDR_MAINFRAME_ENG (English (U.S.))
Resource8=IDR_POPUP_ENG (English (U.S.))
Resource9=IDR_MAINFRAME
Class8=CCharView
Class9=CItemDBToolbar
Resource10=IDD_ABOUTBOX
Resource11=IDD_LISTDLG
Resource12=IDD_CHARVIEWDLG
Class10=CItemListView
Class11=CSearchOptDlg
Resource13=IDR_MAINFRAME (English (U.S.))

[CLS:CItemDBApp]
Type=0
HeaderFile=ItemDB.h
ImplementationFile=ItemDB.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC

[CLS:CItemDBDoc]
Type=0
HeaderFile=ItemDBDoc.h
ImplementationFile=ItemDBDoc.cpp
Filter=N

[CLS:CRealmView]
Type=0
HeaderFile=RealmView.h
ImplementationFile=RealmView.cpp
Filter=C
BaseClass=CTreeView
VirtualFilter=VWC


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=W
BaseClass=CFrameWnd
VirtualFilter=fWC
LastObject=ID_TBCHK




[CLS:CAboutDlg]
Type=0
HeaderFile=ItemDB.cpp
ImplementationFile=ItemDB.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_TITLE,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_URL,static,1342308352

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_MRU_FILE1
Command6=ID_APP_EXIT
Command10=ID_EDIT_PASTE
Command11=ID_VIEW_TOOLBAR
Command12=ID_VIEW_STATUS_BAR
Command13=ID_APP_ABOUT
CommandCount=13
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command7=ID_EDIT_UNDO
Command8=ID_EDIT_CUT
Command9=ID_EDIT_COPY

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_NEXT_PANE
CommandCount=13
Command4=ID_EDIT_UNDO
Command13=ID_PREV_PANE


[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_START_SEARCH
Command2=ID_EXPORT
Command3=ID_TO_END
Command4=ID_HELPHELP
Command5=ID_D2VIEW
Command6=ID_ITEMVIEW
Command7=ID_SQLVIEW
Command8=ID_TO_BEGIN
Command9=ID_TO_PREV
Command10=ID_TO_NEXT
CommandCount=10

[CLS:CItemDBView]
Type=0
HeaderFile=ItemDBView.h
ImplementationFile=ItemDBView.cpp
BaseClass=CFormView
Filter=D
LastObject=CItemDBView
VirtualFilter=VWC

[MNU:IDR_MENU_ENG (English (U.S.))]
Type=1
Class=?
Command1=ID_EXPORT_CHAR
Command2=ID_EXPORT_SQL
CommandCount=2

[MNU:IDR_POPUP_CHS]
Type=1
Class=CMainFrame
Command1=ID_POPUP_RESTORE
Command2=ID_LANG_ENG
Command3=ID_LANG_CHS
Command4=ID_HELPHELP
Command5=ID_APP_ABOUT
Command6=ID_APP_EXIT
CommandCount=6

[MNU:IDR_POPUP_ENG (English (U.S.))]
Type=1
Class=?
Command1=ID_POPUP_RESTORE
Command2=ID_LANG_ENG
Command3=ID_LANG_CHS
Command4=ID_HELPHELP
Command5=ID_APP_ABOUT
Command6=ID_APP_EXIT
CommandCount=6

[MNU:IDR_SEARCH_CHS]
Type=1
Class=CMainFrame
Command1=ID_BYKEYWORDS
Command2=ID_BYEXP
CommandCount=2

[MNU:IDR_SEARCH_ENG (English (U.S.))]
Type=1
Class=CMainFrame
Command1=ID_BYKEYWORDS
Command2=ID_BYEXP
CommandCount=2

[CLS:CCharList]
Type=0
HeaderFile=CharList.h
ImplementationFile=CharList.cpp
BaseClass=CListCtrl
Filter=W
LastObject=CCharList

[CLS:CCharView]
Type=0
HeaderFile=CharView.h
ImplementationFile=CharView.cpp
BaseClass=CFormView
Filter=D
LastObject=IDC_LIST1
VirtualFilter=VWC

[CLS:CItemDBToolbar]
Type=0
HeaderFile=ItemDBToolbar.h
ImplementationFile=ItemDBToolbar.cpp
BaseClass=CToolBar
Filter=W
LastObject=ID_START_SEARCH

[DLG:IDD_CHARVIEWDLG]
Type=1
Class=CCharView
ControlCount=1
Control1=IDC_LIST1,SysListView32,1342243084

[DLG:IDD_ITEMDBDLG]
Type=1
Class=CItemDBView
ControlCount=1
Control1=IDC_D2VIEW,static,1342177540

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_EXPORT
Command2=ID_DELETE
Command3=ID_D2VIEW
Command4=ID_ITEMVIEW
Command5=ID_SQLVIEW
Command6=ID_TBEDIT
Command7=ID_START_SEARCH
Command8=ID_TBCHK
Command9=ID_APP_ABOUT
CommandCount=9

[MNU:IDR_MAINFRAME_ENG (English (U.S.))]
Type=1
Class=?
Command1=ID_EXPORT
Command2=ID_DELETE
Command3=ID_APP_EXIT
Command4=ID_LANG_ENG
Command5=ID_LANG_CHS
Command6=ID_D2VIEW
Command7=ID_ITEMVIEW
Command8=ID_SQLVIEW
Command9=ID_VIEW_TOOLBAR
Command10=ID_VIEW_STATUS_BAR
Command11=ID_HELPHELP
Command12=ID_APP_ABOUT
CommandCount=12

[MNU:IDR_MAINFRAME_CHS]
Type=1
Class=CMainFrame
Command1=ID_EXPORT
Command2=ID_DELETE
Command3=ID_APP_EXIT
Command4=ID_LANG_ENG
Command5=ID_LANG_CHS
Command6=ID_D2VIEW
Command7=ID_ITEMVIEW
Command8=ID_SQLVIEW
Command9=ID_VIEW_TOOLBAR
Command10=ID_VIEW_STATUS_BAR
Command11=ID_HELPHELP
Command12=ID_APP_ABOUT
CommandCount=12

[DLG:IDD_LISTDLG]
Type=1
Class=CItemListView
ControlCount=2
Control1=IDC_ITEMLIST,SysListView32,1342242825
Control2=IDC_SQLLIST,SysListView32,1073807369

[CLS:CItemListView]
Type=0
HeaderFile=ItemListView.h
ImplementationFile=ItemListView.cpp
BaseClass=CFormView
Filter=D
LastObject=CItemListView
VirtualFilter=VWC

[CLS:CSearchOptDlg]
Type=0
HeaderFile=SearchOptDlg.h
ImplementationFile=SearchOptDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_EDIT1
VirtualFilter=dWC

