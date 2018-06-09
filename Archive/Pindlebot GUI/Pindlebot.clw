; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CCharListDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Pindlebot.h"
LastPage=0

ClassCount=11
Class1=CPindlebotApp
Class3=CMainFrame
Class4=CAboutDlg

ResourceCount=10
Resource1=IDD_SETTINGSDLG
Resource2=IDD_ACTIONDLG
Class2=CChildView
Resource3=IDD_COMBODLG
Class5=CSettingsDlg
Resource4=IDR_MAINFRAME
Class6=CCharDlg
Resource5=IDD_NEWCHARDLG
Class7=CCharListDlg
Resource6=IDR_MENU1
Class8=CActionDlg
Resource7=IDD_CHARLISTDLG
Class9=CActionListDlg
Resource8=IDD_ABOUTBOX
Class10=CComboDlg
Resource9=IDD_ACTIONLISTDLG
Class11=CStatisticsDlg
Resource10=IDD_STATISTICSDLG

[CLS:CPindlebotApp]
Type=0
HeaderFile=Pindlebot.h
ImplementationFile=Pindlebot.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC

[CLS:CChildView]
Type=0
HeaderFile=ChildView.h
ImplementationFile=ChildView.cpp
Filter=N

[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CFrameWnd
VirtualFilter=fWC
LastObject=CMainFrame




[CLS:CAboutDlg]
Type=0
HeaderFile=Pindlebot.cpp
ImplementationFile=Pindlebot.cpp
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
Command1=ID_APP_EXIT
Command2=ID_EDIT_UNDO
Command3=ID_EDIT_CUT
Command4=ID_EDIT_COPY
Command5=ID_EDIT_PASTE
Command6=ID_APP_ABOUT
CommandCount=6

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_EDIT_COPY
Command2=ID_EDIT_PASTE
Command3=ID_EDIT_UNDO
Command4=ID_EDIT_CUT
Command5=ID_NEXT_PANE
Command6=ID_PREV_PANE
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_CUT
Command10=ID_EDIT_UNDO
CommandCount=10

[MNU:IDR_MENU1]
Type=1
Class=?
Command1=ID_POPUP_SETTINGS
Command2=ID_POPUP_ITEMLOG
Command3=ID_POPUP_STATISTICS
Command4=ID_POPUP_HELP
Command5=ID_APP_ABOUT
Command6=ID_APP_EXIT
CommandCount=6

[DLG:IDD_SETTINGSDLG]
Type=1
Class=CSettingsDlg
ControlCount=48
Control1=IDC_STATIC,button,1342177287
Control2=IDC_BEFORERED,button,1342242816
Control3=IDC_INRED,button,1342242816
Control4=IDC_ONPINDLE,button,1342242816
Control5=IDC_ONCORPSE,button,1476460544
Control6=IDC_STATIC,button,1342177287
Control7=IDC_STATIC,static,1342308352
Control8=IDC_MALAHHP,edit,1350631552
Control9=IDC_STATIC,static,1342308352
Control10=IDC_MALAHMANA,edit,1350631552
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_POTIONHP,edit,1350631552
Control14=IDC_STATIC,static,1342308352
Control15=IDC_POTIONMANA,edit,1350631552
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,static,1342308352
Control18=IDC_QUITHP,edit,1350631552
Control19=IDC_STATIC,static,1342308352
Control20=IDC_QUITMANA,edit,1350631552
Control21=IDC_STATIC,static,1342308352
Control22=IDC_STATIC,static,1342308352
Control23=IDC_STAYNORMAL,button,1342341129
Control24=IDC_STAYCLOSE,button,1342210057
Control25=IDC_STAYFAR,button,1342210057
Control26=IDC_STATIC,button,1342177287
Control27=IDC_STATIC,static,1342308352
Control28=IDC_PETHP,edit,1350631552
Control29=IDC_STATIC,static,1342308352
Control30=IDC_QUITONPETDIE,button,1342275587
Control31=IDC_RESMERC,button,1342275587
Control32=IDC_STATIC,button,1342177287
Control33=IDC_STATIC,static,1342308352
Control34=IDC_REPAIR,edit,1350631552
Control35=IDC_STATIC,static,1342308352
Control36=IDC_STATIC,button,1342177287
Control37=IDC_WEAPONTYPE,button,1342341129
Control38=IDC_RADIO2,button,1342210057
Control39=IDC_RADIO3,button,1342210057
Control40=IDC_RADIO4,button,1342210057
Control41=IDC_STATIC,static,1342308352
Control42=IDC_DEADLYCOMBO,SysListView32,1342275593
Control43=IDC_ADDCOMBO,button,1342242816
Control44=IDC_DELCOMBO,button,1476460544
Control45=IDC_EDITCOMBO,button,1476460544
Control46=IDC_STATIC,static,1342177296
Control47=IDOK,button,1342242816
Control48=IDCANCEL,button,1342242816

[CLS:CSettingsDlg]
Type=0
HeaderFile=SettingsDlg.h
ImplementationFile=SettingsDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CSettingsDlg
VirtualFilter=dWC

[DLG:IDD_NEWCHARDLG]
Type=1
Class=CCharDlg
ControlCount=6
Control1=IDC_STATIC,static,1342308352
Control2=IDC_CHARNAME,edit,1350631552
Control3=IDC_STATIC,static,1342308352
Control4=IDC_CHARCLASS,combobox,1344339971
Control5=IDOK,button,1342242817
Control6=IDCANCEL,button,1342242816

[CLS:CCharDlg]
Type=0
HeaderFile=CharDlg.h
ImplementationFile=CharDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CCharDlg

[DLG:IDD_CHARLISTDLG]
Type=1
Class=CCharListDlg
ControlCount=15
Control1=IDC_LIST1,SysListView32,1342275597
Control2=IDC_ADD,button,1342242816
Control3=IDC_DEL,button,1476460544
Control4=IDC_REN,button,1476460544
Control5=IDC_EDIT,button,1476460544
Control6=IDC_STATIC,static,1342308352
Control7=IDC_IDLELEN,edit,1350631552
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_USESHOPBOT,button,1342275587
Control11=IDC_SHOPBOT,button,1342242816
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342177296
Control14=IDOK,button,1342242816
Control15=IDCANCEL,button,1342242816

[CLS:CCharListDlg]
Type=0
HeaderFile=CharListDlg.h
ImplementationFile=CharListDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_SHOPBOT

[DLG:IDD_ACTIONDLG]
Type=1
Class=CActionDlg
ControlCount=15
Control1=IDC_STATIC,static,1342308352
Control2=IDC_SKILLS,combobox,1344340227
Control3=IDC_LBL,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_LEFTRIGHT,button,1342308361
Control6=IDC_RIGHT,button,1342177289
Control7=IDC_STATIC,static,1342308352
Control8=IDC_TARGET,combobox,1344339971
Control9=IDC_STATIC,static,1342308352
Control10=IDC_DELAY,edit,1350631552
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_REPEAT,edit,1350631552
Control14=IDOK,button,1342242817
Control15=IDCANCEL,button,1342242816

[CLS:CActionDlg]
Type=0
HeaderFile=ActionDlg.h
ImplementationFile=ActionDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_SKILLS

[DLG:IDD_ACTIONLISTDLG]
Type=1
Class=CActionListDlg
ControlCount=9
Control1=IDC_LIST1,SysListView32,1342275589
Control2=IDC_ADD,button,1342242816
Control3=IDC_DEL,button,1476460544
Control4=IDC_EDIT,button,1476460544
Control5=IDC_UP,button,1476460544
Control6=IDC_DOWN,button,1476460544
Control7=IDOK,button,1342242816
Control8=IDCANCEL,button,1342242816
Control9=IDC_STATIC,static,1342177296

[CLS:CActionListDlg]
Type=0
HeaderFile=ActionListDlg.h
ImplementationFile=ActionListDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CActionListDlg
VirtualFilter=dWC

[DLG:IDD_COMBODLG]
Type=1
Class=CComboDlg
ControlCount=4
Control1=IDC_STATIC,static,1342308352
Control2=IDC_LIST1,SysListView32,1350664201
Control3=IDOK,button,1476460545
Control4=IDCANCEL,button,1342242816

[CLS:CComboDlg]
Type=0
HeaderFile=ComboDlg.h
ImplementationFile=ComboDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_LIST1
VirtualFilter=dWC

[DLG:IDD_STATISTICSDLG]
Type=1
Class=CStatisticsDlg
ControlCount=24
Control1=IDC_STATIC,static,1342308352
Control2=IDC_TOTALTIME,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_TOTALGAMES,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_NETEFFECTIVE,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_PINDLEKILLED,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_DEADLYCOMBO,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_RETREATED,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_CHARDIED,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDC_MERCDIED,static,1342308352
Control17=IDC_STATIC,static,1342308352
Control18=IDC_INSUFFICIENTSKILL,static,1342308352
Control19=IDC_STATIC,static,1342308352
Control20=IDC_POTIONDRUNK,static,1342308352
Control21=IDC_STATIC,static,1342308352
Control22=IDC_POTIONMERC,static,1342308352
Control23=IDC_STATIC,static,1342308352
Control24=IDCANCEL,button,1342242816

[CLS:CStatisticsDlg]
Type=0
HeaderFile=StatisticsDlg.h
ImplementationFile=StatisticsDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_TOTALTIME
VirtualFilter=dWC

