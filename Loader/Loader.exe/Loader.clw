; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CLoaderDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Loader.h"

ClassCount=3
Class1=CLoaderApp
Class2=CLoaderDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_LOADER_DIALOG

[CLS:CLoaderApp]
Type=0
HeaderFile=Loader.h
ImplementationFile=Loader.cpp
Filter=N

[CLS:CLoaderDlg]
Type=0
HeaderFile=LoaderDlg.h
ImplementationFile=LoaderDlg.cpp
Filter=W
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CLoaderDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=LoaderDlg.h
ImplementationFile=LoaderDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_LOADER_DIALOG]
Type=1
Class=CLoaderDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST1,SysListView32,1350664197
Control4=IDC_LOADUNLOAD,button,1342242816

