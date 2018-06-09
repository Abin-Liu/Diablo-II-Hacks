; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CD2NoMinDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "D2NoMin.h"

ClassCount=3
Class1=CD2NoMinApp
Class2=CD2NoMinDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_D2NOMIN_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTBOX
Resource4=IDR_POPUP

[CLS:CD2NoMinApp]
Type=0
HeaderFile=D2NoMin.h
ImplementationFile=D2NoMin.cpp
Filter=N

[CLS:CD2NoMinDlg]
Type=0
HeaderFile=D2NoMinDlg.h
ImplementationFile=D2NoMinDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=ID_SETTING

[CLS:CAboutDlg]
Type=0
HeaderFile=D2NoMinDlg.h
ImplementationFile=D2NoMinDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_D2NOMIN_DIALOG]
Type=1
Class=CD2NoMinDlg
ControlCount=2
Control1=IDC_LIST1,SysListView32,1342275589
Control2=IDC_REFRESH,button,1342242816

[MNU:IDR_POPUP]
Type=1
Class=CD2NoMinDlg
Command1=ID_POPUP_ENABLE
Command2=ID_POPUP_DISABLE
Command3=ID_SETTING
Command4=ID_POPUP_ABOUT
Command5=IDOK
CommandCount=5

