; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CAboutDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "D2XSpeed.h"

ClassCount=4
Class1=CD2XSpeedApp
Class2=CD2XSpeedDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_TABLEDLG (Chinese (P.R.C.))
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTBOX (Chinese (P.R.C.))
Class4=CTableDlg
Resource4=IDD_D2XSPEED_DIALOG (Chinese (P.R.C.))
Resource5=IDR_MENU1

[CLS:CD2XSpeedApp]
Type=0
HeaderFile=D2XSpeed.h
ImplementationFile=D2XSpeed.cpp
Filter=N

[CLS:CD2XSpeedDlg]
Type=0
HeaderFile=D2XSpeedDlg.h
ImplementationFile=D2XSpeedDlg.cpp
Filter=D
LastObject=ID_ABOUT
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=D2XSpeedDlg.h
ImplementationFile=D2XSpeedDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_ABIN

[CLS:CTableDlg]
Type=0
HeaderFile=TableDlg.h
ImplementationFile=TableDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CTableDlg

[MNU:IDR_MENU1]
Type=1
Class=?
Command1=ID_EXIT
Command2=ID_HELP
Command3=ID_ABOUT
CommandCount=3

[DLG:IDD_ABOUTBOX (Chinese (P.R.C.))]
Type=1
Class=CAboutDlg
ControlCount=6
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_CURETEAM,static,1342308352
Control6=IDC_ABIN,static,1342177280

[DLG:IDD_D2XSPEED_DIALOG (Chinese (P.R.C.))]
Type=1
Class=CD2XSpeedDlg
ControlCount=23
Control1=IDC_STATIC,static,1342308352
Control2=IDC_CLASSES,combobox,1344340227
Control3=IDC_STATIC,static,1342308352
Control4=IDC_ATTACKFORM,combobox,1344339971
Control5=IDC_STATIC,static,1342308352
Control6=IDC_FILTER,combobox,1344340227
Control7=IDC_STATIC,static,1342308352
Control8=IDC_WEAPONS,combobox,1344340227
Control9=IDC_IASSOURCE,button,1342177287
Control10=IDC_STATIC,static,1342308352
Control11=IDC_IAS,edit,1350631552
Control12=IDC_SPINIAS,msctls_updown32,1342177334
Control13=IDC_LBLFRENZY,static,1342177280
Control14=IDC_FRENZY,edit,1350631552
Control15=IDC_SPINFRENZY,msctls_updown32,1342177334
Control16=IDC_LBLFANA,static,1342177280
Control17=IDC_FANA,edit,1350631552
Control18=IDC_SPINFANA,msctls_updown32,1342177334
Control19=IDC_LBLBOS,static,1342177280
Control20=IDC_BOS,edit,1350631552
Control21=IDC_SPINBOS,msctls_updown32,1342177334
Control22=IDC_SHOWTABLE,button,1342242817
Control23=IDC_EXIT,button,1342242816

[DLG:IDD_TABLEDLG (Chinese (P.R.C.))]
Type=1
Class=CTableDlg
ControlCount=13
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342308352
Control3=IDC_CURIAS,static,1342177280
Control4=IDC_STATIC,static,1342308352
Control5=IDC_CURBASE,static,1342177280
Control6=IDC_STATIC,static,1342308352
Control7=IDC_CURWSM,static,1342177280
Control8=IDC_STATIC,static,1342308352
Control9=IDC_CURSIAS,static,1342177280
Control10=IDC_LIST1,SysListView32,1350666245
Control11=IDC_CURSPEED,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352

