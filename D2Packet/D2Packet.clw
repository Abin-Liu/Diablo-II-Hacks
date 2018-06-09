; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CD2PacketView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "D2Packet.h"
LastPage=0

ClassCount=8
Class1=CD2PacketApp
Class2=CD2PacketDoc
Class3=CD2PacketView
Class4=CMainFrame

ResourceCount=7
Resource1=IDR_POPUP
Resource2=IDR_MAINFRAME
Class5=CAboutDlg
Resource3=IDD_ABOUTBOX
Resource4=IDD_PACKETIDDLG
Resource5=IDD_D2PACKET_FORM
Resource6=IDD_FILTERDLG
Class6=CContainDlg
Class7=CFilterDlg
Class8=CPacketIDDlg
Resource7=IDD_CONTAINDLG

[CLS:CD2PacketApp]
Type=0
HeaderFile=D2Packet.h
ImplementationFile=D2Packet.cpp
Filter=N

[CLS:CD2PacketDoc]
Type=0
HeaderFile=D2PacketDoc.h
ImplementationFile=D2PacketDoc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC

[CLS:CD2PacketView]
Type=0
HeaderFile=D2PacketView.h
ImplementationFile=D2PacketView.cpp
Filter=D
BaseClass=CFormView
VirtualFilter=VWC
LastObject=IDC_LIST1


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=ID_POPUP_BLOCK
BaseClass=CFrameWnd
VirtualFilter=fWC




[CLS:CAboutDlg]
Type=0
HeaderFile=D2Packet.cpp
ImplementationFile=D2Packet.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_OPEN
Command2=ID_FILE_SAVE
Command3=ID_FILE_SAVE_AS
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_APP_ABOUT
CommandCount=8

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_POPUP_BLOCK
Command2=ID_POPUP_CLIPBOARD
Command3=ID_DEBUG
Command4=ID_CLEAR
Command5=ID_FILTER
Command6=ID_PLAYERINFO
Command7=ID_FILE_NEW
Command8=ID_FILE_OPEN
Command9=ID_PAUSE
Command10=ID_RESUME
Command11=ID_FILE_SAVE
Command12=ID_SEARCH
CommandCount=12

[DLG:IDD_D2PACKET_FORM]
Type=1
Class=CD2PacketView
ControlCount=1
Control1=IDC_LIST1,SysListView32,1342275593

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_OPEN
Command2=ID_FILE_SAVE
Command3=ID_PAUSE
Command4=ID_RESUME
Command5=ID_CLEAR
Command6=ID_POPUP_CLIPBOARD
Command7=ID_POPUP_BLOCK
Command8=ID_FILTER
Command9=ID_SEARCH
Command10=ID_PLAYERINFO
Command11=ID_APP_ABOUT
CommandCount=11

[MNU:IDR_POPUP]
Type=1
Class=CD2PacketView
Command1=ID_RCV_ALLOWALL
Command2=ID_RCV_BLOCKALL
Command3=ID_RCV_CUSTOM
Command4=ID_SNT_ALLOWALL
Command5=ID_SNT_BLOCKALL
Command6=ID_SNT_CUSTOM
Command7=ID_POPUP_BLOCK
Command8=ID_POPUP_CLIPBOARD
CommandCount=8

[DLG:IDD_PACKETIDDLG]
Type=1
Class=CPacketIDDlg
ControlCount=4
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDIT1,edit,1350631552
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816

[DLG:IDD_FILTERDLG]
Type=1
Class=CFilterDlg
ControlCount=10
Control1=IDC_STATIC,static,1342308352
Control2=IDC_LISTALLOWED,SysListView32,1342259225
Control3=IDC_STATIC,static,1342308352
Control4=IDC_LISTBLOCKED,SysListView32,1342259225
Control5=IDC_BLOCK,button,1476460544
Control6=IDC_ALLOW,button,1476460544
Control7=IDC_BLOCKALL,button,1342242816
Control8=IDC_ALLOWALL,button,1342242816
Control9=IDOK,button,1342242816
Control10=IDCANCEL,button,1342242816

[DLG:IDD_CONTAINDLG]
Type=1
Class=CContainDlg
ControlCount=5
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDIT1,edit,1350631552
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342242817
Control5=IDCANCEL,button,1342242816

[CLS:CContainDlg]
Type=0
HeaderFile=ContainDlg.h
ImplementationFile=ContainDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CContainDlg
VirtualFilter=dWC

[CLS:CFilterDlg]
Type=0
HeaderFile=FilterDlg.h
ImplementationFile=FilterDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_ALLOW
VirtualFilter=dWC

[CLS:CPacketIDDlg]
Type=0
HeaderFile=PacketIDDlg.h
ImplementationFile=PacketIDDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CPacketIDDlg
VirtualFilter=dWC

